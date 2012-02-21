#ifndef imageutils_H_
#define imageutils_H_

#if HAVE_FREEIMAGE
#include <FreeImage.h>
#endif

#include <math.h>

#define P_B 100000  // Previous Pixel Black
#define C_B    100  // Current Pixel Black
#define P_G  10000  // Previous Pixel Gray
#define C_G     10  // Current Pixel Gray
#define P_W   1000  // Previous Pixel White
#define C_W      1  // Current Pixel White

// Color Patterns 

// Changing Patterns
#define B2G 100010  // Black to Gray
#define G2B  10100  // Gray to Black
#define G2W  10001  // Gray to White
#define W2G   1010  // White to Gray
#define B2W 100001  // Black to White
#define W2B   1100  // White to Black   

// Static Patterns
#define W2W   1001  // White to White
#define B2B 100100  // Black to Black
#define G2G  10010  // Gray to Gray

#ifndef byte
    #define byte unsigned char
#endif

namespace imageutils
{   

#if WIN32_LEAN                                        

    static HBITMAP Create8bppBitmap(HDC hdc, int width, int height, LPVOID pBits = NULL)
    {
        BITMAPINFO *bmi = (BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);  
        BITMAPINFOHEADER &bih(bmi->bmiHeader);  
        bih.biSize = sizeof (BITMAPINFOHEADER); 
        bih.biWidth         = width;  
        bih.biHeight        = -height;  
        bih.biPlanes        = 1;  
        bih.biBitCount      = 8;  
        bih.biCompression   = BI_RGB;  
        bih.biSizeImage     = 0;  
        bih.biXPelsPerMeter = 14173;  
        bih.biYPelsPerMeter = 14173;  
        bih.biClrUsed       = 0;  
        bih.biClrImportant  = 0;  
        for (int I = 0; I <= 255; I++)  
        {    
            bmi->bmiColors[I].rgbBlue = bmi->bmiColors[I].rgbGreen = bmi->bmiColors[I].rgbRed = (BYTE)I;
            bmi->bmiColors[I].rgbReserved = 0;  
        }
        
        void *Pixels = NULL;  
        HBITMAP hbmp = CreateDIBSection(hdc, bmi, DIB_RGB_COLORS, &Pixels, NULL, 0);

        if(pBits != NULL)
        {
            //fill the bitmap 
            BYTE* pbBits = (BYTE*)pBits;
            BYTE *Pix = (BYTE *)Pixels;  
            memcpy(Pix, pbBits, width * height);
        }

        free(bmi);

        return hbmp;
    }

    static HBITMAP CreateBitmapFromPixels( HDC hDC, UINT uWidth, UINT uHeight, UINT uBitsPerPixel, LPVOID pBits)
    {
           if(uBitsPerPixel < 8) // NOT IMPLEMENTED YET
              return NULL;

          if(uBitsPerPixel == 8)
              return Create8bppBitmap(hDC, uWidth, uHeight, pBits);

          HBITMAP hBitmap = 0;
          if ( !uWidth || !uHeight || !uBitsPerPixel )
             return hBitmap;
          LONG lBmpSize = uWidth * uHeight * (uBitsPerPixel/8) ;
          BITMAPINFO bmpInfo = { 0 };
          bmpInfo.bmiHeader.biBitCount = uBitsPerPixel;
          bmpInfo.bmiHeader.biHeight = uHeight;
          bmpInfo.bmiHeader.biWidth = uWidth;
          bmpInfo.bmiHeader.biPlanes = 1;
          bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
               // Pointer to access the pixels of bitmap       
          UINT * pPixels = 0;
          hBitmap = CreateDIBSection( hDC, (BITMAPINFO *)&
                                      bmpInfo, DIB_RGB_COLORS, (void **)&
                                      pPixels , NULL, 0); 

          if ( !hBitmap )
              return hBitmap; // return if invalid bitmaps
          
          //SetBitmapBits( hBitmap, lBmpSize, pBits);
          // Directly Write
          memcpy(pPixels, pBits, lBmpSize );
          
          return hBitmap;
    }

    static BOOL SaveBitmapToFile(HBITMAP hBitmap, LPCTSTR lpFileName, HPALETTE hPal = NULL)
    {
        BOOL bResult = FALSE;

        PICTDESC stPictDesc;
        stPictDesc.cbSizeofstruct = sizeof(PICTDESC);
        stPictDesc.picType = PICTYPE_BITMAP;
        stPictDesc.bmp.hbitmap = hBitmap;
        stPictDesc.bmp.hpal = hPal;

        LPPICTURE pPicture;

        HRESULT hr = OleCreatePictureIndirect( &stPictDesc, IID_IPicture, FALSE, reinterpret_cast<void**>(&pPicture) );

        if ( SUCCEEDED(hr) )
        {
            LPSTREAM pStream;
            hr = CreateStreamOnHGlobal( NULL, TRUE, &pStream );
            if ( SUCCEEDED(hr) )
            {
                long lBytesStreamed = 0;
                hr = pPicture->SaveAsFile( pStream, TRUE, &lBytesStreamed );

                if ( SUCCEEDED(hr) )
                {
                    HANDLE hFile = CreateFile( lpFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
                        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

                    if ( hFile )
                    {
                        HGLOBAL hMem = NULL;
                        GetHGlobalFromStream( pStream, &hMem );

                        LPVOID lpData = GlobalLock( hMem );

                        DWORD dwBytesWritten;
                        bResult = WriteFile( hFile, lpData, lBytesStreamed, &dwBytesWritten, NULL );

                        bResult &= ( dwBytesWritten == (DWORD)lBytesStreamed );

                        // clean up

                        GlobalUnlock(hMem);
                        CloseHandle(hFile);
                    }
                }

                // clean up 
                pStream->Release();
            }

            // clean up 
            pPicture->Release();
        }

        return bResult; 

    }
    
    // GetRotatedBitmap - Create a new bitmap with rotated image
    // Returns      - Returns new bitmap with rotated image
    // hBitmap      - Bitmap to rotate
    // radians      - Angle of rotation in radians
    // clrBack      - Color of pixels in the resulting bitmap that do
    //            not get covered by source pixels
    // Note         - If the bitmap uses colors not in the system palette 
    //            then the result is unexpected. You can fix this by
    //            adding an argument for the logical palette.
    static HBITMAP GetRotatedBitmap( HBITMAP hBitmap, float radians, COLORREF clrBack )
    {
        // Create a memory DC compatible with the display
        CDC sourceDC, destDC;
        sourceDC.CreateCompatibleDC( NULL );
        destDC.CreateCompatibleDC( NULL );

        // Get logical coordinates
        BITMAP bm;
        ::GetObject( hBitmap, sizeof( bm ), &bm );

        float cosine = (float)cos(radians);
        float sine = (float)sin(radians);

        // Compute dimensions of the resulting bitmap
        // First get the coordinates of the 3 corners other than origin
        int x1 = (int)(-bm.bmHeight * sine);
        int y1 = (int)(bm.bmHeight * cosine);
        int x2 = (int)(bm.bmWidth * cosine - bm.bmHeight * sine);
        int y2 = (int)(bm.bmHeight * cosine + bm.bmWidth * sine);
        int x3 = (int)(bm.bmWidth * cosine);
        int y3 = (int)(bm.bmWidth * sine);

        int minx = min(0,min(x1, min(x2,x3)));
        int miny = min(0,min(y1, min(y2,y3)));
        int maxx = max(x1, max(x2,x3));
        int maxy = max(y1, max(y2,y3));

        int w = maxx - minx;
        int h = maxy - miny;
        
        // Create a bitmap to hold the result
        HBITMAP hbmResult = ::CreateCompatibleBitmap(CClientDC(NULL), w, h);
        if(hbmResult == NULL)
        {
            // compatible bitmap creation may fail due to width and height being substantially large
            // so use the nice API CreateBitmapFromPixels which ensures creation of the bitmap if enough memory is available
            hbmResult = CreateBitmapFromPixels(NULL, w, h, bm.bmBitsPixel, bm.bmBits);          
        }

        HBITMAP hbmOldSource = (HBITMAP)::SelectObject( sourceDC.m_hDC, hBitmap );
        HBITMAP hbmOldDest = (HBITMAP)::SelectObject( destDC.m_hDC, hbmResult );

        // Draw the background color before we change mapping mode
        HBRUSH hbrBack = CreateSolidBrush( clrBack );
        HBRUSH hbrOld = (HBRUSH)::SelectObject( destDC.m_hDC, hbrBack );
        destDC.PatBlt( 0, 0, w, h, PATCOPY );
        ::DeleteObject( ::SelectObject( destDC.m_hDC, hbrOld ) );

        // Set mapping mode so that +ve y axis is upwords
        sourceDC.SetMapMode(MM_ISOTROPIC);
        sourceDC.SetWindowExt(1,1);
        sourceDC.SetViewportExt(1,-1);
        sourceDC.SetViewportOrg(0, bm.bmHeight-1);

        destDC.SetMapMode(MM_ISOTROPIC);
        destDC.SetWindowExt(1,1);
        destDC.SetViewportExt(1,-1);
        destDC.SetWindowOrg(minx, maxy);

        // Now do the actual rotating - a pixel at a time
        // Computing the destination point for each source point
        // will leave a few pixels that do not get covered
        // So we use a reverse transform - e.i. compute the source point
        // for each destination point

        for( int y = miny; y < maxy; y++ )
        {
            for( int x = minx; x < maxx; x++ )
            {
                int sourcex = (int)(x*cosine + y*sine);
                int sourcey = (int)(y*cosine - x*sine);
                if( sourcex >= 0 && sourcex < bm.bmWidth && sourcey >= 0 
                        && sourcey < bm.bmHeight )
                    destDC.SetPixel(x,y,sourceDC.GetPixel(sourcex,sourcey));
            }
        }

        // Restore DCs
        ::SelectObject( sourceDC.m_hDC, hbmOldSource );
        ::SelectObject( destDC.m_hDC, hbmOldDest );

        return hbmResult;
    }


    // GetRotatedBitmapNT   - Create a new bitmap with rotated image
    // Returns      - Returns new bitmap with rotated image
    // hBitmap      - Bitmap to rotate
    // radians      - Angle of rotation in radians
    // clrBack      - Color of pixels in the resulting bitmap that do
    //            not get covered by source pixels
    // seems to work for small sized bitmaps only :|

    static HBITMAP GetRotatedBitmapNT( HBITMAP hBitmap, float radians, COLORREF clrBack )
    {
        // Create a memory DC compatible with the display
        CDC sourceDC, destDC;
        sourceDC.CreateCompatibleDC( NULL );
        destDC.CreateCompatibleDC( NULL );

        // Get logical coordinates
        BITMAP bm;
        ::GetObject( hBitmap, sizeof( bm ), &bm );

        float cosine = (float)cos(radians);
        float sine = (float)sin(radians);

        // Compute dimensions of the resulting bitmap
        // First get the coordinates of the 3 corners other than origin
        int x1 = (int)(bm.bmHeight * sine);
        int y1 = (int)(bm.bmHeight * cosine);
        int x2 = (int)(bm.bmWidth * cosine + bm.bmHeight * sine);
        int y2 = (int)(bm.bmHeight * cosine - bm.bmWidth * sine);
        int x3 = (int)(bm.bmWidth * cosine);
        int y3 = (int)(-bm.bmWidth * sine);

        int minx = min(0,min(x1, min(x2,x3)));
        int miny = min(0,min(y1, min(y2,y3)));
        int maxx = max(0,max(x1, max(x2,x3)));
        int maxy = max(0,max(y1, max(y2,y3)));

        int w = maxx - minx;
        int h = maxy - miny;

        // Create a bitmap to hold the result
        HBITMAP hbmResult = ::CreateCompatibleBitmap(CClientDC(NULL), w, h);

        HBITMAP hbmOldSource = (HBITMAP)::SelectObject( sourceDC.m_hDC, hBitmap );
        HBITMAP hbmOldDest = (HBITMAP)::SelectObject( destDC.m_hDC, hbmResult );

        // Draw the background color before we change mapping mode
        HBRUSH hbrBack = CreateSolidBrush( clrBack );
        HBRUSH hbrOld = (HBRUSH)::SelectObject( destDC.m_hDC, hbrBack );
        destDC.PatBlt( 0, 0, w, h, PATCOPY );
        ::DeleteObject( ::SelectObject( destDC.m_hDC, hbrOld ) );
        
        SetGraphicsMode(destDC.m_hDC, GM_ADVANCED);
        XFORM xform;
        xform.eM11 = cosine;
        xform.eM12 = -sine;
        xform.eM21 = sine;
        xform.eM22 = cosine;
        xform.eDx = (float)-minx;
        xform.eDy = (float)-miny;

        SetWorldTransform( destDC.m_hDC, &xform );

        // Now do the actual rotating - a pixel at a time
        destDC.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &sourceDC, 0, 0, SRCCOPY );


        // Restore DCs
        ::SelectObject( sourceDC.m_hDC, hbmOldSource );
        ::SelectObject( destDC.m_hDC, hbmOldDest );

        return hbmResult;
    }

    
    /*the pitch is defined as follows: 
    DWORD pitch = (bmWidth+3) & ~3; // round to nearest DWORD 
    DWORD stride = (pitch * bmBitCount ) >> 3; // number of DWORD 
    aligned bytes in one scanline. 

    PBYTE pScanline = pDIBits + ( bmHeight - 1 - y ) * stride; // pDIBits 
    points to the end of the bitmap. Where y = 0 to bmHeight - 1. 

    You will experience a memory fault or buffer corruption if you 
    don't follow the specification that Microsoft dictates for their 
    bitmaps. 

    luckily we don't have to deal with all these info since
    // we are taking advantage of the device context features such as BitBlt    
    */

    static HBITMAP GetGrayScaleImage(HBITMAP hBmp, int width, int height, unsigned int bytesPerPixel, int pitch, byte** ppGrayScaleBits = NULL)
    {   
        if(bytesPerPixel == 0 || bytesPerPixel == 2)
            return NULL;    
        
        int width_in_bytes = width * bytesPerPixel;
        
        int unusedBytes = absolute(pitch) - width_in_bytes;                     
        if(!(unusedBytes % 3))
            unusedBytes = 0;
        
        HBITMAP hBmp8 = Create8bppBitmap(NULL, width + unusedBytes, height, NULL); 

        HDC hdcScr = ::GetDC(NULL);

        HDC hdc = ::CreateCompatibleDC(hdcScr);
        HBITMAP hBmpOld = (HBITMAP)::SelectObject(hdc, hBmp);

        HDC hdc8 = ::CreateCompatibleDC(hdcScr);
        HBITMAP hBmpOld8 = (HBITMAP)::SelectObject(hdc8, hBmp8);

        ::BitBlt(hdc8, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
        if(ppGrayScaleBits)
        {   
            int grayScaleSize = width * height;     

            byte* p8Bit = new byte [grayScaleSize];     
            ::GetBitmapBits(hBmp8, grayScaleSize, p8Bit);

            *ppGrayScaleBits = p8Bit;           
        }       
        
        ::SelectObject(hdc, hBmpOld);
        ::SelectObject(hdc8, hBmpOld8);

        ::DeleteDC(hdc);
        ::DeleteDC(hdc8);

        ::ReleaseDC(NULL, hdcScr);              
        
        return hBmp8;       
    }

#endif // WIN32_LEAN


#if HAVE_FREEIMAGE                                        

    static FIBITMAP* Create8bppBitmap(int width, int height, void* data = NULL)
    {
    }

    static FIBITMAP* CreateBitmapFromPixels(int width, int height, int bitsPerPixel, void* data = NULL)
    {
    }

    static bool SaveBitmapToFile(FIBITMAP* bmp, const char* path)
    { 
    }
    
    // GetRotatedBitmap - Create a new bitmap with rotated image
    // Returns      - Returns new bitmap with rotated image
    // hBitmap      - Bitmap to rotate
    // radians      - Angle of rotation in radians
    // clrBack      - Color of pixels in the resulting bitmap that do
    //            not get covered by source pixels
    // Note         - If the bitmap uses colors not in the system palette 
    //            then the result is unexpected. You can fix this by
    //            adding an argument for the logical palette.
    static FIBITMAP* GetRotatedBitmap(FIBITMAP* bmp, float radians, byte* clrBack )
    {
    }


    // GetRotatedBitmapNT   - Create a new bitmap with rotated image
    // Returns      - Returns new bitmap with rotated image
    // hBitmap      - Bitmap to rotate
    // radians      - Angle of rotation in radians
    // clrBack      - Color of pixels in the resulting bitmap that do
    //            not get covered by source pixels
    // seems to work for small sized bitmaps only :|

    static FIBITMAP* GetRotatedBitmapNT(FIBITMAP* bmp, float radians, byte* clrBack )
    {        
    }

    
    /*the pitch is defined as follows: 
    DWORD pitch = (bmWidth+3) & ~3; // round to nearest DWORD 
    DWORD stride = (pitch * bmBitCount ) >> 3; // number of DWORD 
    aligned bytes in one scanline. 

    PBYTE pScanline = pDIBits + ( bmHeight - 1 - y ) * stride; // pDIBits 
    points to the end of the bitmap. Where y = 0 to bmHeight - 1. 

    You will experience a memory fault or buffer corruption if you 
    don't follow the specification that Microsoft dictates for their 
    bitmaps. 

    luckily we don't have to deal with all these info since
    // we are taking advantage of the device context features such as BitBlt    
    */

    static FIBITMAP* GetGrayScaleImage(FIBITMAP* bmp, int width, int height, unsigned int bytesPerPixel, int pitch, byte** ppGrayScaleBits = NULL)
    {   
    }

#endif // HAVE_FREEIMAGE

    static bool IsTextImage(byte* imageData, int imageWidth, int imageHeight)
    {
        const int blacklimit = 20;
        const int greylimit = 140;

        const int contrast_offset = 80;

        // Holds the count of different patterns in the processed data
        long color_pattern_count[200000];
        
        //to avoid division by zero.. set it to 1
        color_pattern_count[B2G] = 1;
        color_pattern_count[G2B] = 1;
        color_pattern_count[G2W] = 1;
        color_pattern_count[W2G] = 1;
        color_pattern_count[B2W] = 1;
        color_pattern_count[W2B] = 1;   
        
        color_pattern_count[B2B] = 1;
        color_pattern_count[W2W] = 1;
        color_pattern_count[G2G] = 1;   
        
        
        long prev_color[256];   
        long cur_color[256];        

        int i;
        for(i = 0; i < 256; i++)
        {
            cur_color[i]  = 0;      
            prev_color[i] = 0;
        }

        for(i = 0; i <= blacklimit; i++)
        {
            cur_color[i]  = C_B;
            prev_color[i] = P_B;
        }

        for(i = blacklimit + 1 + contrast_offset; i <= greylimit; i++)
        {
            cur_color[i]  = C_G;        
            prev_color[i] = P_G;
        }

        for(i = greylimit + 1 + contrast_offset; i <= 255; i++)
        {
            cur_color[i]  = C_W;    
            prev_color[i] = P_W;
        }
        
        byte* buffer = imageData;
        
        int y, x;
        
        int line_count = 0;

        byte prev_pixel;
        byte cur_pixel;
        long change_pattern = 0;
        long white_amt = 0;
        long n = -1;
        for(y = 0; y < imageHeight; y+=10)
        {       
            n++;
            white_amt = 0;
            for(x = 1; x < imageWidth; x++)
            {
                prev_pixel = buffer[y*imageWidth + (x-1)];
                cur_pixel = buffer[y*imageWidth + x];
                    
                change_pattern = prev_color[prev_pixel] + cur_color[cur_pixel];
                if((prev_color[prev_pixel]) && (cur_color[cur_pixel]))
                {           
                    color_pattern_count[change_pattern]++;                              
                    if(change_pattern == W2W)
                    {
                        white_amt++;
                    }
                }
            }
            
            if(((double)white_amt/(double)x) > 0.85) //it's a white line
            {
                line_count++;
            }       
        }                   
        
        double line_count_ratio = (n != 0) ? (double)line_count/(double)n : 0.0;
            
        bool bRet = true;
        if(line_count_ratio < 0.4 || line_count_ratio > 1.0)
        {
            bRet = false;
        }               
        
        return bRet;
    }
                                                                                              
    static int absolute(int x)
    {
        return (x < 0) ? -x : x;
    }
    
    
    static unsigned int OneBitRowStride(unsigned int w)
    {
       return (w + 7) / 8;
    }

    static unsigned int GetDataSize(unsigned int w, unsigned int h, unsigned int bitdepth)
    {
       unsigned int size = w * h * (bitdepth / 8);
       if (bitdepth == 1)
       {
          size = OneBitRowStride(w) * h;
       }

       return size;
    }
}
                                                                                                       

#endif