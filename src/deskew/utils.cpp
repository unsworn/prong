//
//  utils.cpp
//  prong
//
//  Created by Nicklas Marelius on 3/6/12.
//  Copyright (c) 2012 assertfail.org. All rights reserved.
//

#include <stdio.h>                
#include <string.h>
#include <stdlib.h>

#include "log.h"
#include "config.h"
#include "options.h"            
#include "imagetypes.h"
#include "imageutils.h"
#include "imagedata.h"
#include "analyzer.h"          
#include "JSON.h"

#include "utils.h"

FIBITMAP*
addAlphaChannel(FIBITMAP* in, unsigned int WHITE)
{

    if (in == NULL)
    {
        ERROR("addAlphaChannel() invalid input %d", in);
        return NULL;
    }

    unsigned int bpp    = FreeImage_GetBPP(in);

    FIBITMAP* bmp = NULL;

    if ((bmp = FreeImage_ConvertTo32Bits(in)) == NULL)
    {
        ERROR("addAlphaChannel(), %d => 32 bits conversion failed", bpp);
        return NULL;
    }

    bpp = FreeImage_GetBPP(bmp);
    
    //TRACE("addAlphaChannel(), bpp = %d", bpp);

    unsigned int width  = FreeImage_GetWidth(bmp);
    unsigned int height = FreeImage_GetHeight(bmp);
    unsigned int pitch  = FreeImage_GetPitch(bmp);

    unsigned int r,g,b,a;
    
    BYTE* bits = (BYTE*) FreeImage_GetBits(bmp);

    for (int y=0 ; y < height ; y++)
    {
        BYTE* pixels = (BYTE*)bits;

        for (int x=0 ; x < width ; x++)
        {
            r = pixels[FI_RGBA_RED];
            g = pixels[FI_RGBA_GREEN];
            b = pixels[FI_RGBA_BLUE];

            pixels[FI_RGBA_ALPHA] = WHITE - ((r + g + b)/3);
            
            pixels += 4;
        }

        bits += pitch;

    }

    return bmp;
}

unsigned int
getIntensityAverage(Box* box, FIBITMAP* bmp)
{

    if (box == NULL || bmp == NULL)
        return 255;

    unsigned int width  = FreeImage_GetWidth(bmp);
    unsigned int height = FreeImage_GetHeight(bmp);

    unsigned long gray    = 0;
    
    unsigned int x    = (box->rel.origin.x    * width) + 5;
    unsigned int y    = (box->rel.origin.y    * height)+ 5;            
    unsigned int w    = (box->rel.size.width  * width) -10;        
    unsigned int h    = (box->rel.size.height * height)-10;

    Rect crop;
    crop.origin.x    = x;
    crop.origin.y    = y;
    crop.size.width  = w;
    crop.size.height = h;
    
    FIBITMAP* copy;
    
    if ((copy = imageutils::GetCroppedBitmap(bmp, crop)) == NULL)
    {
        ERROR("getColorAverage() copy pixels failed %d", 1);
        return 255;
    }

    bmp = copy;

#if 0
    imageutils::SaveBitmapToFile(copy, "/tmp/out/intensity_average.png");
#endif
    
    FREE_IMAGE_TYPE type = FreeImage_GetImageType(bmp);

    width  = FreeImage_GetWidth(bmp);
    height = FreeImage_GetHeight(bmp);

    unsigned int pitch  = FreeImage_GetPitch(bmp);
    unsigned int bpp    = FreeImage_GetBPP(bmp);
    unsigned int nc     = 0;

    unsigned int sum = 0;
    
    if ((type == FIT_BITMAP))
    {

        switch(bpp)
        {
            case 32: nc = 4; break;
            case 24: nc = 3; break;
            case 16: nc = 2; break;                
            case 8:
            default: nc = 1; break;
        }
                
        BYTE* bits = (BYTE*) FreeImage_GetBits(bmp);

        for (y=0 ; y < height ; y++)
        {
            BYTE* pixels = (BYTE*)bits;
            
            for (x=0 ; x < width ; x++)
            {
                sum    = (pixels[FI_RGBA_RED]*77)+(pixels[FI_RGBA_GREEN]*151)+(pixels[FI_RGBA_BLUE]*28);
                gray   += ((sum>>8)&0x000000FF);
                pixels += nc;              
            }

            bits += pitch;
        }
    }
    
    imageutils::FreeBitmap(bmp);

    return (gray / (width * height));
    
}

unsigned int
getBaseline(Box* box, FIBITMAP* bmp)
{
    unsigned int base  = 200;
    unsigned int delta = 0;
    
    base = getIntensityAverage(box, bmp);

    if (base < 200)
        delta = 200 - base;

    return (255 - delta);
    
}

bool
checkBoolPixels(Box* box, FIBITMAP* bmp, unsigned int WHITE)
{
    unsigned int a = getIntensityAverage(box, bmp);
    return (a < (WHITE/2));
}

void
calculate_skew_and_exit(const char* path, bool degrees, const char* outp)
{
    JSONData   json;
    FIBITMAP*  bmp;
    ImageData* i;
    Analyzer   a;
    
    double skewAngle = 0;
    
    if((bmp = imageutils::LoadBitmap(path)) == NULL)
    {
        ERROR("Unable to open file %s", path);
        exit(1);
    }
    
    double scale = 1.0;
    
    int i_width  = FreeImage_GetWidth(bmp);
    int i_height = FreeImage_GetHeight(bmp);
    
    int width  = i_width;
    int height = i_height;
    
    TRACE("Input is bitmap [%dx%d]", width, height);
    
    if (CONFIG_IMAGE_ORIENTATION == CONFIG_PORTRAIT && width > CONFIG_IMAGE_MAX_SIDE)
    {
        scale = (CONFIG_IMAGE_MAX_SIDE / (float)width);
        
        INFO("Scaling %f input (PORTRAIT) to [%dx%d]", scale, CONFIG_IMAGE_MAX_SIDE, (int)(height * scale));
        
        FIBITMAP* tmp = FreeImage_Rescale(bmp, CONFIG_IMAGE_MAX_SIDE, height * scale, FILTER_BILINEAR);
        
        imageutils::FreeBitmap(bmp);
        
        bmp = tmp;
    }
    else if (CONFIG_IMAGE_ORIENTATION == CONFIG_LANDSCAPE && height > CONFIG_IMAGE_MAX_SIDE)
    {
        scale = (CONFIG_IMAGE_MAX_SIDE / (float)height);
        
        INFO("Scaling %f input (LANDSCAPE) to [%dx%d]", scale, (int)(width * scale), CONFIG_IMAGE_MAX_SIDE);
        
        FIBITMAP* tmp = FreeImage_Rescale(bmp, width * scale, CONFIG_IMAGE_MAX_SIDE, FILTER_BILINEAR);
        
        imageutils::FreeBitmap(bmp);
        
        bmp = tmp;        
    }
    
    width = FreeImage_GetWidth(bmp);
    height = FreeImage_GetHeight(bmp);
    
    TRACE("Finding skew angle: [%dx%d]", width, height);
    
    if ((i = imageutils::GetOp(bmp)) != NULL)
    {
        if (!a.GetSkewAngle(false, *i, skewAngle, 0))
            INFO("GetSkewAngle() bmp == %s failed", path);
        
        delete i;
    }
    else
        INFO("GetOp() bmp == %s failed", path);
    
    imageutils::FreeBitmap(bmp);
    
    if (degrees)
        skewAngle = radToDeg(skewAngle);
    
    if (outp != NULL)
    {
        json.open();
        json.append("width", i_width);
        json.append("height", i_height);
        json.append("skew", skewAngle);
        json.close();
        json.write(outp);
    }
    else
    {        
        printf("%f\n", skewAngle);
    }
    
    exit(0);
    
}   

void
crop_and_exit(const char* inputFile, Template* t, const char* outPath)
{               
    FIBITMAP*  bmp;
    ImageData* i;
              
    char path[2048];
    
    double skewAngle = t->getSkewAngle();

    Rect& cropBox = t->getCrop();

    Size& makeSize = t->getMarkSize();
    
    if((bmp = imageutils::LoadBitmap(inputFile)) == NULL)
    {
        ERROR("Unable to open file %s", inputFile);
        exit(1);
    }
    
    double scale = 1.0;
        
    int width  = FreeImage_GetWidth(bmp);
    int height = FreeImage_GetHeight(bmp);
                   
    TRACE("Rotating input: -%f", skewAngle);
            
    if (skewAngle != 0)
    {
        FIBITMAP* rmp = NULL;
        
        if ((rmp = imageutils::GetRotatedBitmap(bmp, -skewAngle, NULL)) == NULL)
        {
            ERROR("Rotate %f failed on input", skewAngle);
            exit(1);
        }      

        //sprintf(path, "%s/rotated.png", outPath);     

        //imageutils::SaveBitmapToFile(rmp, path);

        imageutils::FreeBitmap(bmp);

        bmp = rmp;
        
        width  = FreeImage_GetWidth(bmp);
        height = FreeImage_GetHeight(bmp);
        
    }
    

    if (!cropBox.isEmpty())
    {

        FIBITMAP* rmp = NULL;
        
        if ((rmp = imageutils::GetCroppedBitmap(bmp, cropBox)) == NULL)
        {
            ERROR("Crop failed %d", 112);
            return ;
        }

#if 0        
        imageutils::SaveBitmapToFile(rmp, "/tmp/out/cropped.jpg", FIF_JPEG);
#endif
        
        imageutils::FreeBitmap(bmp);

        bmp = rmp;

        width = FreeImage_GetWidth(bmp);
        height = FreeImage_GetHeight(bmp);
        
    }

    unsigned int WHITE = 255;
    
    Box* ptr = t->getBox("base", GAME_TYPE_BASE);

    if (ptr != NULL)
        WHITE = getBaseline(ptr, bmp);

    TRACE("Image Background WHITE: %d\n", WHITE);
    
    ptr = t->getFirstBox();

    double finalScale = t->getFinalScale();

    unsigned int insetsX = (0.01 * width);
    unsigned int insetsY = (0.01 * height);
    
    while (ptr != NULL)
    {                      

        if (ptr->type == GAME_TYPE_PROPERTY)
        {
            if (ptr->parent != NULL && ptr->owner == NULL)
                ptr->owner = t->getBox(ptr->parent, GAME_TYPE_GRAPHIC);                

            ptr->enabled = checkBoolPixels(ptr, bmp, WHITE);

        }
        else if (ptr->type == GAME_TYPE_GRAPHIC)
        {
            Rect crop;

            crop.origin.x    = ptr->rel.origin.x * width;
            crop.origin.y    = ptr->rel.origin.y * height;            
            crop.size.width  = ptr->rel.size.width * width;
            crop.size.height = ptr->rel.size.height * height;            
                 

            crop.origin.x += insetsX;
            crop.origin.y += insetsY;

            crop.size.width  -= (2 * insetsX);
            crop.size.height -= (2 * insetsY);
            
            sprintf(path, "%s/%s.png", outPath, ptr->name);
                        
            TRACE("crop[%f, %f, %f, %f] %s", crop.origin.x, crop.origin.y, crop.size.width, crop.size.height, path);

            FIBITMAP* copy;

            if ((copy = imageutils::GetCroppedBitmap(bmp, crop)) != NULL)
            {
                if (finalScale != 1.0)
                {
                    crop.origin.x = crop.origin.y = 0;
                    crop.size.width  *= finalScale;
                    crop.size.height *= finalScale;
                    FIBITMAP* scaled = NULL;

                    if ((scaled = imageutils::GetScaledBitmap(copy, crop)) != NULL)
                    {
                        imageutils::FreeBitmap(copy);
                        copy = scaled;
                    }

                }

                if (ptr->clazz != GAME_CLASS_BACKGROUND)
                {
                    FIBITMAP* alpha = NULL;

                    if ((alpha = addAlphaChannel(copy, WHITE)) != NULL)
                    {
                        imageutils::FreeBitmap(copy);
                        copy = alpha;
                    }
                }
                else
                    TRACE("skipping alpha on background.. %d", 0);
                
                imageutils::SaveBitmapToFile(copy, path, FIF_PNG);
                                             
                imageutils::FreeBitmap(copy);
            
                ptr->path = strdup(path);
            }
            
        }                                                              
                        
        ptr = ptr->next;
    }


    JSONData data;

    data.array();
    
    ptr = t->getFirstBox();
                     
    while (ptr != NULL)
    {                      

        if (ptr->type == GAME_TYPE_PROPERTY)
        {
            data.open();
            
            data.append("name", ptr->name);
            data.append("parent", ptr->parent);
            data.append("enabled", (ptr->enabled) ? "true" : "false");

            data.close();

        }

        ptr = ptr->next;
        
    }


    sprintf(path, "%s/properties.js", outPath);

    data.write(path);
    
    exit(0);
}

//------------------------------------------------------------------------------------------------------------

// optimal cut for 150dpi A3, 18x18+1713+2443
// ./deskew -o /tmp/out -j ../../example/template.js -c -g 19x19+1714+2442 ../../example/template.jpg

//------------------------------------------------------------------------------------------------------------

void
get_crop_box(const char* str, Rect &r)
{
    int o;
    
    float x=0,y=0,w=0,h=0;
    
    if (str == NULL)
    {
        ERROR("get_crop_box() str == %d", 0);
        return ;
    }

        
    if ((o = sscanf(str, "%fx%f+%f+%f", &x, &y, &w, &h)) == 4)
    {
        r.origin.x = x;
        r.origin.y = y;
        r.size.width = w;
        r.size.height = h;
        return;
    }

    if ((o = sscanf(str, "+%f+%f", &w, &h)) == 2)
    {
        r.origin.x = 0;
        r.origin.y = 0;
        r.size.width = w;
        r.size.height = h;
        return ;
    }

    if ((o = sscanf(str, "%fx%f", &x, &y)) == 2)
    {
        r.origin.x = x;
        r.origin.y = y;
        r.size.width = 0;
        r.size.height = 0;
        return ;
    }


    ERROR("get_crop_box(), unable to parse geometry XxY+W+H, XxY, +W+H %d", 0);

    exit(1);
    
    
}
