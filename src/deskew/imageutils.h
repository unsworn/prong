#ifndef imageutils_H_
#define imageutils_H_

#include "imagetypes.h"
#include "template.h"

#include <FreeImage.h>

class ImageData;

namespace imageutils
{   

    int version();

    bool requiredVersion(int v);
    
    FIBITMAP* LoadBitmap(const char* path);

    void FreeBitmap(FIBITMAP* bmp);
    
    ImageData* GetOp(FIBITMAP* bmp);

    FIBITMAP* Create8bppBitmap(int width, int height, void* data = NULL);

    FIBITMAP* CreateBitmapFromPixels(int width, int height, int bitsPerPixel, void* data = NULL);

    bool SaveBitmapToFile(FIBITMAP* bmp, const char* path, FREE_IMAGE_FORMAT fif=FIF_JPEG);

    // GetRotatedBitmap - Create a new bitmap with rotated image
    // Returns      - Returns new bitmap with rotated image
    // hBitmap      - Bitmap to rotate
    // radians      - Angle of rotation in radians
    // clrBack      - Color of pixels in the resulting bitmap that do
    //            not get covered by source pixels
    // Note         - If the bitmap uses colors not in the system palette 
    //            then the result is unexpected. You can fix this by
    //            adding an argument for the logical palette.
    FIBITMAP* GetRotatedBitmap(FIBITMAP* bmp, float radians, byte* clrBack );

    // GetRotatedBitmapNT   - Create a new bitmap with rotated image
    // Returns      - Returns new bitmap with rotated image
    // hBitmap      - Bitmap to rotate
    // radians      - Angle of rotation in radians
    // clrBack      - Color of pixels in the resulting bitmap that do
    //            not get covered by source pixels
    // seems to work for small sized bitmaps only :|

    FIBITMAP* GetRotatedBitmapNT(FIBITMAP* bmp, float radians, byte* clrBack );


    
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

    FIBITMAP* GetGrayScaleImage(FIBITMAP* bmp, int width, int height, unsigned int bytesPerPixel, int pitch, byte** ppGrayScaleBits = NULL);
                                                                              

    bool IsTextImage(byte* imageData, int imageWidth, int imageHeight);
                                                                                              
    int absolute(int x);

    unsigned int OneBitRowStride(unsigned int w);

    unsigned int GetDataSize(unsigned int w, unsigned int h, unsigned int bitdepth);

    FIBITMAP* RotateBitmap(FIBITMAP* bmp, double angle, const void* ptr = NULL);

    FIBITMAP* GetScaledBitmap(FIBITMAP* bmp, Rect &geometry);

    FIBITMAP* GetCroppedBitmap(FIBITMAP* bmp, Rect &geometry);
    
}
                                                                                                       

#endif
