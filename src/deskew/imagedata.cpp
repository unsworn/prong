#include <stdio.h>
#include <string.h>

#include "imagedata.h"
#include "canny_edge.h"


ImageData::ImageData()
{
    m_data = NULL;
    m_edgeData = NULL;
    m_id = -1;
    m_allocated = false;
    ResetErrorCode();                   
}

ImageData::~ImageData()
{
    Cleanup();  
}

void ImageData::Cleanup()
{
    if(m_allocated)
    {
        DeleteData();
    }
    else
    {
        m_data = NULL;
    }
}

bool ImageData::Load(byte* data, unsigned int width, unsigned int height, int bytesPerPixel)
{
    bool bRet = false;
    if(data != NULL)
    {
        m_data = data;
        m_width = width;
        m_height = height;
        m_bytesPerPixel = bytesPerPixel;
        bRet = true;
    }   
    return bRet;
}



ImageData* ImageData::Clone()
{
    ImageData* img_data = new ImageData;        
    img_data->m_bytesPerPixel = m_bytesPerPixel;
    img_data->AllocDataSpace(GetWidth(), GetHeight(), GetBytesPerPixel() * 8);
    memcpy(img_data->m_data, m_data, GetSize());     
    img_data->m_width = m_width;
    img_data->m_height = m_height;

    return img_data;
}

bool ImageData::BlurToEdges(int slices/*=0*/)
{
    bool bRet = false;      

    if(m_edgeData != NULL)
    {
        // already blurred
        return true;
    }

    if(m_data == NULL)
        return false;
    
    byte* edgeData = NULL;
        
    if(slices == 0)
    {       
        canny(m_data, GetHeight(), GetWidth(), (float)10, (float)0.1, (float)0.87, &edgeData, NULL);
        Cleanup();

        m_data = edgeData;          
        m_allocated = true;
        bRet = true;
    }
    else if(slices == 1)
    {
        int topHalfHeight = GetHeight() / 2;
        int bottomHalfHeight = GetHeight() - topHalfHeight;
        
        canny(m_data, topHalfHeight, GetWidth(), (float)10, (float)0.1, (float)0.9, &edgeData, NULL);
        Cleanup();

        memcpy(m_data, edgeData, GetWidth() * topHalfHeight);
        free(edgeData);
        
        canny(m_data + (GetWidth() * topHalfHeight), bottomHalfHeight, GetWidth(), (float)10, (float)0.1, (float)0.9, &edgeData, NULL);
        memcpy(m_data + (GetWidth() * topHalfHeight), edgeData, GetWidth() * bottomHalfHeight);
        free(edgeData);                                 
        
        m_allocated = true;
        bRet = true;
    }
    
    m_edgeData = m_data;
    
    return bRet;
}
    
bool ImageData::AllocDataSpace(int w, int h, unsigned int bitdepth/*=24*/)
{   
    unsigned int size = imageutils::GetDataSize(w, h, bitdepth);
    m_data = new byte[size];
    if (m_data)
    {     
      m_width = w;
      m_height = h;  
      m_allocated = true;     
    }
   
    return m_data ? true : false;
}

unsigned int ImageData::GetSize()
{
    return imageutils::GetDataSize(GetWidth(), GetHeight(), GetBytesPerPixel() * 8);
}

byte* ImageData::getpixel(byte* pixelBuf, int i, int j)
{
    byte* rowData = &pixelBuf[j * GetRowStride()];
    rowData = rowData + i * GetBytesPerPixel();
    return rowData;
}

void ImageData::putpixel(byte* pixelBuf, int i, int j, byte* color)
{
    byte* rowData = &pixelBuf[j * GetRowStride()];
    rowData = rowData + i * GetBytesPerPixel();
    rowData[0] = color[0];
    rowData[1] = color[1];
    rowData[2] = color[2];
}

void ImageData::BlurToEdgesRobust()
{
    if(m_edgeData != NULL)
        return;

    byte* edges = new byte [GetSize()];

    byte *color, *color1, *color2;
    byte makecolor[3] = {128, 128, 128};
    
    byte r,g,b, r1, g1, b1, r2, g2, b2;
    double bb = 85;
    int w = GetWidth();
    int h = GetHeight();
    for(int i=0;i<w-1;i++)
    {
        for(int j=0;j<h-1;j++)
        {
            color=getpixel(GetData(), i,j);
            r=getr32(color);
            g=getg32(color);
            b=getb32(color);
     
            color1=getpixel(GetData(), i+1,j);
            r1=getr32(color1);
            g1=getg32(color1);
            b1=getb32(color1);

            color2=getpixel(GetData(), i,j+1);
            r2=getr32(color2);
            g2=getg32(color2);
            b2=getb32(color2);
     
            double s1 = sqrt((double)(r-r1)*(r-r1)+(g-g1)*(g-g1)+(b-b1)*(b-b1));
            double s2 = sqrt((double)(r-r2)*(r-r2)+(g-g2)*(g-g2)+(b-b2)*(b-b2));
            if((s1>=bb)||(s2>=bb))
            {
                int clr = 0;
                makecolor[0] = clr;
                makecolor[1] = clr;
                makecolor[2] = clr;
                putpixel(edges,i,j,makecolor);
     
            }
            else
            {
                int clr = 255;
                makecolor[0] = clr;
                makecolor[1] = clr;
                makecolor[2] = clr;
                putpixel(edges,i,j,makecolor);
            }
         }
    }

    Cleanup();
    m_data = edges;
    m_edgeData = m_data;
    m_allocated = true;
}