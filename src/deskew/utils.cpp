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
                  
bool
checkBoolPixels(Box* box, FIBITMAP* bmp)
{
    return false;
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

        imageutils::FreeBitmap(bmp);

        bmp = rmp;

        width = FreeImage_GetWidth(bmp);
        height = FreeImage_GetHeight(bmp);
        
    }

    Box* ptr = t->getFirstBox();
                     
    while (ptr != NULL)
    {                      
        

        if (ptr->type == GAME_TYPE_PROPERTY)
        {
            if (ptr->parent != NULL && ptr->owner == NULL)
                ptr->owner = t->getBox(ptr->parent, GAME_TYPE_GRAPHIC);                
        
            ptr->enabled = checkBoolPixels(ptr, bmp);
        }
        else if (ptr->type == GAME_TYPE_GRAPHIC)
        {
            Rect crop;
            crop.origin.x    = ptr->rel.origin.x * width;
            crop.origin.y    = ptr->rel.origin.y * height;            
            crop.size.width  = ptr->rel.size.width * width;
            crop.size.height = ptr->rel.size.height * height;            
            
                        
            sprintf(path, "%s/%s.png", outPath, ptr->name);
                        
            TRACE("crop[%f, %f, %f, %f] %s", crop.origin.x, crop.origin.y, crop.size.width, crop.size.height, path);

            FIBITMAP* copy;

            if ((copy = imageutils::GetCroppedBitmap(bmp, crop)) != NULL)
            {
                imageutils::SaveBitmapToFile(copy, path);
                                             
                imageutils::FreeBitmap(copy);
            
                ptr->path = strdup(path);
            }
            
        }                                                              
                        
        ptr = ptr->next;
    }
    exit(0);
}

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
