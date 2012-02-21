#ifndef imagedata_H_
#define imagedata_H_                                                  

#include "imageutils.h"

struct ImageData  
{
public:
    ImageData();
    virtual ~ImageData();
    void Cleanup();

    bool Load(byte* data, unsigned int width, unsigned int height, int bytesPerPixel);  
    ImageData* Clone();
    bool BlurToEdges(int slices = 0); 
    void BlurToEdgesRobust(); 
    
    inline int& GetID()
    {
        return m_id;
    }
    inline int SetID(int id)
    {
        m_id = id;
    }
    inline unsigned int GetWidth()
    {
        return m_width;
    }
    inline unsigned int GetHeight()
    {
        return m_height;
    }
    inline byte* GetData()
    {
        return m_data;
    }
    inline byte* GetEdgeData()
    {
        return m_edgeData;
    }
    inline void SetEdgeData(byte* edgeData)
    {
        m_edgeData = edgeData;
        
        Cleanup();
        m_data = edgeData;

        m_allocated = false;
    }
    inline byte* GetRow(int row)
    {
        return &m_data[row * GetWidth()];
    }
    inline unsigned int GetRowStride()
    {
        return m_width * GetBytesPerPixel();
    }
    inline void DeleteData()
    {
        if(m_data)
            delete[] m_data;

        m_data = 0L;
    }
    inline int GetErrorCode()
    {
        return m_error_code;
    }
    void ResetErrorCode()
    {
        m_error_code = 0;
    }       
    int GetBytesPerPixel()
    {
        return m_bytesPerPixel;
    }   
    void SetBytesPerPixel(unsigned int Bpp)
    {
        m_bytesPerPixel = Bpp;
    }
    unsigned int GetSize();
    
    byte* getpixel(byte* pixelBuf, int i, int j);
    void putpixel(byte* pixelBuf, int i, int j, byte* color);

    byte getr32(byte* color)
    {
        return color[0];
    }
    byte getg32(byte* color)
    {
        return color[1];
    }
    byte getb32(byte* color)
    {
        return color[2];
    }
    
private:
    bool AllocDataSpace(int w, int h, unsigned int bitdepth);       
    unsigned int m_width, m_height; 
    byte* m_data;   
    byte* m_edgeData;
    int m_id;   
    int m_error_code;   
    
    bool m_allocated;

    unsigned int m_bytesPerPixel;
};

#endif
