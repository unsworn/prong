#include <stdio.h>
#include <stdlib.h>
#include <string.h>
                              
#include "log.h"

#include "imageutils.h"

#include "imagedata.h"

namespace imageutils
{
    static int gImageVersion = 0;

    int version()
    {
        if (gImageVersion == 0)
        {
            int v0=0, v1=0, v2=0;
            const char* fiv = FreeImage_GetVersion();
            if (fiv != NULL)
            {
                int l = sscanf(fiv, "%d.%d.%d", &v0, &v1, &v2);

                if (l >= 1)
                    gImageVersion += v0 * 100;
                if (l >= 2)
                    gImageVersion += v1;
            }
        }
        return gImageVersion;
    }

    
    bool requiredVersion(int v)
    {
        return (version() >= v);
    }

    FIBITMAP* LoadBitmap(const char* path)
    {
        FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
        FIBITMAP* bmp;
                         
        TRACE("LoadBitmap(%s)", path);
        
        fif = FreeImage_GetFileType(path);
                
        TRACE("LoadBitmap() s1 fif = 0x%X", fif);
        
        if (fif == FIF_UNKNOWN)          
            fif = FreeImage_GetFIFFromFilename(path);

        TRACE("LoadBitmap() s1 fif = 0x%X", fif);
            
        if (fif == FIF_UNKNOWN)
        {
            ERROR("Undetermined image file format: %s\n", path);
            return NULL;
        }
            
        if (!FreeImage_FIFSupportsReading(fif))
        {   
            ERROR("Unsupported image file format: %s\n", path);
            return NULL;                                                 
        }         
        
        bmp = FreeImage_Load(fif, path); 
        
        return bmp;
    }   
    
    void FreeBitmap(FIBITMAP* bmp)
    {
        if (bmp == NULL)
            return ;
        
        FreeImage_Unload(bmp);
    }
    
    ImageData* GetOp(FIBITMAP* bmp)
    {
        ImageData* i = NULL;
        BYTE*      bits;
        byte*      data;
        int        channels;
        long       memsize;
        
        
        if (NULL == bmp)
            return NULL;
        
        i = new ImageData();
            
        int width  = (int)FreeImage_GetWidth(bmp);
        int height = (int)FreeImage_GetHeight(bmp);
        int bpp    = (int)FreeImage_GetBPP(bmp);
                         
        TRACE("GetOp() width=%d, height=%d, bpp=%d", width, height, bpp);
        
        if (bpp < 24)
        {
            TRACE("GetOp() converting from bits %d\n", bpp);            
            FIBITMAP* tmp = FreeImage_ConvertTo24Bits(bmp);
            FreeImage_Unload(bmp);
            bmp = tmp;
            bpp  = FreeImage_GetBPP(bmp);
        }
                                              
        channels = (bpp == 32) ? 4 : 3;
                
        bits = FreeImage_GetBits(bmp);
                                        
        i->Load(bits, width, height, channels);
                                    
        return i;
        
    }
           
    FIBITMAP* Create8bppBitmap(int width, int height, void* data)
    {        
        FIBITMAP* bmp;
        BYTE* bits;
        
        if ((bmp = FreeImage_Allocate(width, height, 8)) == NULL)
            return NULL;
        
        
        if ((bits = FreeImage_GetBits(bmp)) == NULL)
        {
            FreeImage_Unload(bmp);
            return NULL;
        }
        
        if (data != NULL)
            memcpy(bits, data, width * height);
        
        return bmp;
        
    }

    FIBITMAP* CreateBitmapFromPixels(int width, int height, int bitsPerPixel, void* data)
    {
        FIBITMAP* bmp;
        BYTE* bits;
        int channels = (bitsPerPixel / 8);
        
        if ((bmp = FreeImage_Allocate(width, height, bitsPerPixel)) == NULL)
            return NULL;
        
        
        if ((bits = FreeImage_GetBits(bmp)) == NULL)
        {
            FreeImage_Unload(bmp);
            return NULL;
        }
        
        if (data != NULL)
            memcpy(bits, data, (width * height) * channels);
        
        return bmp;
    }

    bool SaveBitmapToFile(FIBITMAP* bmp, const char* path, FREE_IMAGE_FORMAT fif)
    {        
        
        if (bmp == NULL)
            return false;
        
        //int bpp = FreeImage_GetBPP(bmp);
        
        return FreeImage_Save(fif, bmp, path, 0);
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
    FIBITMAP* GetRotatedBitmap(FIBITMAP* bmp, float radians, byte* clrBack )
    {
        
        double angle = radToDeg(radians);
        
        if (bmp == NULL)
            return NULL;
            
        FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(bmp);
        
        switch(image_type)
        {
            case FIT_BITMAP:
            {
                switch(FreeImage_GetBPP(bmp))
                {
                    case 8:                             
                    {                        
                        BYTE color = (clrBack != NULL) ? *clrBack : 128;                        
                        return RotateBitmap(bmp, angle, &color); 
                    }
                    case 24:
                    case 32:
                    {
                        RGBQUAD color = {255, 255, 255, 255};
                        if (clrBack != NULL)
                        {
                            color.rgbRed = clrBack[0]; color.rgbGreen = clrBack[1]; color.rgbBlue = clrBack[2];
                        }   
                        return RotateBitmap(bmp, angle, &color);
                    }
                }
                break;
            }
            case FIT_UINT16:
            {
                WORD color = (clrBack != NULL) ? *clrBack : 128;
                return RotateBitmap(bmp, angle, &color);
            }
            case FIT_RGB16:
            case FIT_RGBA16:
            {
                FIRGBA16 color = {255, 255, 255, 255};
                if (clrBack != NULL)
                {
                    color.red = clrBack[0]; color.green = clrBack[1]; color.blue = clrBack[2]; color.alpha = clrBack[3];
                }
                return RotateBitmap(bmp, angle, &color);
            }
            case FIT_FLOAT:
            {
                float color = (clrBack != NULL) ? (float)(*clrBack/255.0) : 1.f;
                return RotateBitmap(bmp, angle, &color);
            }
            case FIT_RGBF:
            case FIT_RGBAF:
            {
                FIRGBAF color = { 1.0, 1.0, 1.0, 1.0};
                if (clrBack != NULL)
                {
                    color.red = (float)clrBack[0] / 255.f; 
                    color.green = (float)clrBack[1] / 255.f; 
                    color.blue = (float)clrBack[2] / 255.f; 
                    color.alpha = (float)clrBack[3] / 255.f;
                }                                       
                return RotateBitmap(bmp, angle, &color);
            }
        }
        return NULL;
    }


    // GetRotatedBitmapNT   - Create a new bitmap with rotated image
    // Returns      - Returns new bitmap with rotated image
    // hBitmap      - Bitmap to rotate
    // radians      - Angle of rotation in radians
    // clrBack      - Color of pixels in the resulting bitmap that do
    //            not get covered by source pixels
    // seems to work for small sized bitmaps only :|

    FIBITMAP* GetRotatedBitmapNT(FIBITMAP* bmp, float radians, byte* clrBack )
    { 
        return NULL;
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

    FIBITMAP* GetGrayScaleImage(FIBITMAP* bmp, int width, int height, unsigned int bytesPerPixel, int pitch, byte** ppGrayScaleBits)
    {
        return NULL;
    }

    bool IsTextImage(byte* imageData, int imageWidth, int imageHeight)
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
                                                                                              
    int absolute(int x)
    {
        return (x < 0) ? -x : x;
    }
    
    
    unsigned int OneBitRowStride(unsigned int w)
    {
       return (w + 7) / 8;
    }

    unsigned int GetDataSize(unsigned int w, unsigned int h, unsigned int bitdepth)
    {
       unsigned int size = w * h * (bitdepth / 8);
       if (bitdepth == 1)
       {
          size = OneBitRowStride(w) * h;
       }

       return size;
    }

    FIBITMAP* RotateBitmap(FIBITMAP* bmp, double angle, const void* ptr)
    {

#if (FREEIMAGE_MAJOR_VERSION == 3 && FREEIMAGE_MINOR_VERSION >= 15)
        return FreeImage_Rotate(bmp, angle, ptr);
#else
        return FreeImage_RotateClassic(bmp, angle);
#endif
        
    }


    FIBITMAP* GetScaledBitmap(FIBITMAP* bmp, Rect &geometry)
    {
        return FreeImage_Rescale(bmp, (int)geometry.size.width, (int)geometry.size.height, FILTER_BILINEAR);
    }
    

    FIBITMAP* GetCroppedBitmap(FIBITMAP* bmp, Rect &geometry)
    {
        return FreeImage_Copy(
                bmp,
                (int)(geometry.origin.x),
                (int)(geometry.origin.y),
                (int)(geometry.origin.x + geometry.size.width),
                (int)(geometry.origin.y + geometry.size.height));

    }


}
