//
//  utils.cpp
//  prong
//
//  Created by Nicklas Marelius on 3/6/12.
//  Copyright (c) 2012 assertfail.org. All rights reserved.
//

#include <stdio.h>
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