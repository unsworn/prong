#ifndef analyzer_H_
#define analyzer_H_

#include "imagedata.h"
                                                       
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define INFINITE ((1 << (8*sizeof (int) - 6)) - 4)

class Analyzer
{
public:
    Analyzer();
    virtual ~Analyzer();
    
    bool GetSkewAngle(bool isTextImage, ImageData& image, double& outAngle, int canny_slices = 0);

private:
    
    enum EdgeType
    {
        ET_LEFT = 0,
        ET_TOP,
        ET_RIGHT,
        ET_BOTTOM
    };
    struct SlopeInfo
    {
        double x;
        double y;
        double slope;
        double rounded_slope;
        int frequency;

        SlopeInfo()
        {
            x = INFINITE;
            y = INFINITE;
            slope = INFINITE;
            rounded_slope = INFINITE;
            frequency = INFINITE;
        }
    };
    
    bool IsText(ImageData& image);                   
    
    double GetTextSkewAngle(ImageData& image, int sample_skip = 10);
    
    double GetImageSkewAngle(ImageData& image, int slices, EdgeType edge_type, int edge_depth, double* indicator1, double* indicator2);
    
    double Histogram(ImageData& image, double angle);   
    
    unsigned int CastRay(ImageData& raster, int row, const double &dx, int dy);
    
    unsigned int bit_count(unsigned char* buf, unsigned int start, unsigned int end);
    
    double sqr(double x)
    {
        return x * x;
    }
    
    double m_skew;
    int m_sample_skip;
    int m_max_rows;
    unsigned int* m_rows;
};

#endif