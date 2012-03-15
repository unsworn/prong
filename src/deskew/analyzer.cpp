#include <stdio.h>

#include "round.h"
#include "analyzer.h"
                                   

#define max(a, b) (a > b) ? a : b
#define min(a, b) (a < b) ? a : b

Analyzer::Analyzer()
{
    m_max_rows = 0;
    m_rows = NULL;
}

Analyzer::~Analyzer()
{
    if(m_rows)
        delete[] m_rows;
}

bool 
Analyzer::GetSkewAngle(bool isTextImage, ImageData& image, double& outAngle, int canny_slices/*=0*/)
{
    double* angle = &outAngle;

    if(isTextImage)
    {
        // shouldn't blur if text image
        //if(!(image.BlurToEdges(canny_slices)))
        //{
        //  return false;
        //}
        *angle = GetTextSkewAngle(image);
    }
    else
    {
        if(!(image.BlurToEdges(canny_slices)))
        {
            return false;
        }
        
        struct angle_info
        {
            double indicator1;
            double indicator2;
            double angle;
        };
        
        angle_info edge1_angles[4];
        angle_info edge2_angles[4];
        int chosen_edge = 0;
        
        edge2_angles[ET_LEFT].angle = GetImageSkewAngle(image, 50, ET_LEFT, 2, &edge2_angles[ET_LEFT].indicator1, &edge2_angles[ET_LEFT].indicator2);
        edge2_angles[ET_TOP].angle = GetImageSkewAngle(image, 50, ET_TOP, 2, &edge2_angles[ET_TOP].indicator1, &edge2_angles[ET_TOP].indicator2);
        edge2_angles[ET_RIGHT].angle = GetImageSkewAngle(image, 50, ET_RIGHT, 2, &edge2_angles[ET_RIGHT].indicator1, &edge2_angles[ET_RIGHT].indicator2);
        edge2_angles[ET_BOTTOM].angle = GetImageSkewAngle(image, 50, ET_BOTTOM, 2, &edge2_angles[ET_BOTTOM].indicator1, &edge2_angles[ET_BOTTOM].indicator2);
        
        int num_edges = 4;
        for(int i = 0; i < 4; i++)       
        {
            if(edge2_angles[i].indicator1 < 0.2999)
            {
                edge2_angles[i].indicator1 = 0;
                edge2_angles[i].indicator2 = 0;
                edge2_angles[i].angle = 0;
                num_edges--;
            }
        }
        
        if(num_edges < 2)
        {
            edge1_angles[ET_LEFT].angle = GetImageSkewAngle(image, 50, ET_LEFT, 1, &edge1_angles[ET_LEFT].indicator1, &edge1_angles[ET_LEFT].indicator2);
            edge1_angles[ET_TOP].angle = GetImageSkewAngle(image, 50, ET_TOP, 1, &edge1_angles[ET_TOP].indicator1, &edge1_angles[ET_TOP].indicator2);
            edge1_angles[ET_RIGHT].angle = GetImageSkewAngle(image, 50, ET_RIGHT, 1, &edge1_angles[ET_RIGHT].indicator1, &edge1_angles[ET_RIGHT].indicator2);
            edge1_angles[ET_BOTTOM].angle = GetImageSkewAngle(image, 50, ET_BOTTOM, 1, &edge1_angles[ET_BOTTOM].indicator1, &edge1_angles[ET_BOTTOM].indicator2);
            
            for(int i = 0; i < 4; i++)       
            {
                if(edge1_angles[i].indicator1 <= 0.39)
                {
                    edge1_angles[i].indicator1 = 0;
                    edge1_angles[i].indicator2 = 0;
                    edge1_angles[i].angle = 0;
                }
            }
            
            chosen_edge = 0;
            if(edge1_angles[ET_LEFT].indicator1 >= 0.4999 && edge1_angles[ET_RIGHT].indicator1 >= 0.4999)
            {
                if(edge1_angles[ET_LEFT].indicator2 > edge1_angles[ET_RIGHT].indicator2)
                {
                    chosen_edge = ET_LEFT;
                }
                else
                {
                    chosen_edge = ET_RIGHT;
                }
            }
            else
            {
                for(int i = 0; i < 4; i++)       
                {
                    if(i != chosen_edge)
                    {
                        if(edge1_angles[i].indicator1 >= 0.6999 && edge1_angles[chosen_edge].indicator1 >= 0.6999)
                        {
                            if(edge1_angles[i].indicator2 > edge1_angles[chosen_edge].indicator2)
                            {
                                chosen_edge = i;
                            }
                        }
                        else
                        {
                            if(edge1_angles[i].indicator1 > edge1_angles[chosen_edge].indicator1)
                            {
                                chosen_edge = i;
                            }
                        }
                    }
                }
            }
            
            *angle = edge1_angles[chosen_edge].angle;
        }
        else
        {
            chosen_edge = 0;
            if(edge2_angles[ET_LEFT].indicator1 >= 0.4999 && edge2_angles[ET_RIGHT].indicator1 >= 0.4999)
            {
                if(edge2_angles[ET_LEFT].indicator2 > edge2_angles[ET_RIGHT].indicator2)
                {
                    chosen_edge = ET_LEFT;
                }
                else
                {
                    chosen_edge = ET_RIGHT;
                }
            }
            else
            {
                for(int i = 0; i < 4; i++)       
                {
                    if(i != chosen_edge)
                    {
                        if(edge2_angles[i].indicator1 >= 0.6999 && edge2_angles[chosen_edge].indicator1 >= 0.6999)
                        {
                            if(edge2_angles[i].indicator2 > edge2_angles[chosen_edge].indicator2)
                            {
                                chosen_edge = i;
                            }
                        }
                        else
                        {
                            if(edge2_angles[i].indicator1 > edge2_angles[chosen_edge].indicator1)
                            {
                                chosen_edge = i;
                            }
                        }
                    }
                }
            }

            *angle = edge2_angles[chosen_edge].angle;
        }
    }
    
    return true;
}

double 
Analyzer::GetTextSkewAngle(ImageData& image, int sample_skip/*=10*/)
{
    m_sample_skip = sample_skip;

    double var_test;
    double var_opt     = 0.0;

    double step        = 10.0;
    double angle       = 0.0;
    double opt         = 0.0;
    int    i;
    int    j;
    int    iter        = 0;
    int    low         = -25;
    int    high        = 25;

    for (i = 0; i < 2; ++i) 
    {     
        angle = opt + low * step;
        for (j = low; j <= high; ++j) 
        {
            var_test = Histogram( image, angle/10);
            if (var_test > var_opt) 
            {
                var_opt = var_test;
                opt  = angle;
            } 
            else 
            {
                /* do nothing - I'd like to short-circuit, but it doesn't work :( */
            }
            angle += step;
        }
        step /= 10.0;
        low  = -10;
        high = 10;
    }
    
    m_skew = opt / 10.0;
    
    return ((m_skew * M_PI) / 180.0);   
}

unsigned int 
Analyzer::CastRay(ImageData& raster, int row, const double& dx /*const Fixed &dx*/, int dy)
{
    unsigned  bits  = 0;
    //Fixed     start = 0;
    //Fixed     end   = 0;
    //Fixed     width = static_cast<int>( raster.GetWidth());  
    double start = 0;
    double end = 0;
    int width = raster.GetWidth();

    while (start < width)
    {
        if(row < 0 || row >= raster.GetHeight())
        {
            break;  
        }

        end = start + dx;
        if (end > width) 
        {
          end = width;
        }
        /*
        bits += bit_count( raster.GetRow(row)
                         , static_cast<int>( start)
                         , static_cast<int>( end));
        */
        bits += bit_count( raster.GetRow(row), (unsigned int)floor(start), (unsigned int)floor(end));
        start = end;
        row += dy;
    }
  
    return bits;
}

double
Analyzer::Histogram(ImageData& raster, double angle)
{
    int    i;
    double sum;
    double mean;
    double angle_diff  = tan(angle / (180.0 / M_PI));
    int    diff_y      = -static_cast<int>( raster.GetWidth() * angle_diff);
    int    min_y       = max(0, diff_y);
    int    max_y       = min(static_cast<int>( raster.GetHeight())
                          , raster.GetHeight() + diff_y);
    int    num_rows;
    //Fixed  dx;
    double dx;
    int    dy;


    if (raster.GetHeight() > m_max_rows) 
    {
        if(m_rows)
        {
          delete []m_rows;
        }
        m_rows = new unsigned[ raster.GetHeight()];
        m_max_rows = raster.GetHeight();
    }
    
    num_rows = (max_y - min_y) / m_sample_skip + 1;  
    //num_rows = (max_y - min_y) / m_sample_skip;  

    if (angle < 0) 
        dy = -1;
    else 
        dy = +1;
  
    if ((-0.05 < angle) && (angle < 0.05)) 
    {
        dx = static_cast<int>( raster.GetWidth());
    }
    else
    {
        dx = dy / (tan( angle / (180.0 / M_PI)));
    }

    for (i = 0; i < num_rows; ++i) 
    {
        m_rows[ i] = CastRay( raster, min_y + i * m_sample_skip, dx, dy);
    }

    sum = 0.0;
    for (i = 0; i < num_rows; ++i) 
    {
        sum += m_rows[ i];
    }
    mean = sum / num_rows;  
    sum = 0.0;
    for (i = 0; i < num_rows; ++i) 
    {
        sum += sqr( m_rows[ i] - mean);
    }

    return sum / num_rows;
}

unsigned int
Analyzer::bit_count(unsigned char* buf, unsigned start, unsigned end)
{
    unsigned int count = 0;
    unsigned int start_byte = start;
    unsigned int end_byte = end;

    for(unsigned int i = start_byte; i < end_byte; ++i)
    {
        if(buf[i] < (unsigned char)128)
            count++;
    }

    return count;
}

double
Analyzer::GetImageSkewAngle(ImageData& image, int slices, EdgeType edge_type, int edge_depth, double* indicator1, double* indicator2)
{
    byte* buffer = image.GetData();

    double  count = 0, pos_count = 0, neg_count = 0, inf_count = 0;

    int index = 0;

    //SlopeInfo* arrSlopes = new SlopeInfo[slices];

    SlopeInfo* arrSlopes = (SlopeInfo*)malloc(slices * sizeof(SlopeInfo));
    
    int arrIndex = 0;

    byte clrBlack = 0;
    
    int x = 0, y = 0;
    int map_x = -1, map_y = -1;
    int max_x = -1, max_y = -1;
    
    int sign = 1;
    
    switch(edge_type)
    {
        case ET_LEFT:
        {
            max_x = image.GetWidth();
            max_y = image.GetHeight();
            break;
        }           
    
        case ET_TOP:
        {
            max_x = image.GetHeight();
            max_y = image.GetWidth();
            break;
        }

        case ET_RIGHT:
        {
            max_x = image.GetWidth();
            max_y = image.GetHeight();          
            sign = -1;
            break;
        }

        case ET_BOTTOM:
        {
            max_x = image.GetHeight();
            max_y = image.GetWidth();
            sign = -1;
            break;
        }
    }

    int ed;
    for(y = max_y/slices; y < max_y; y = y + max_y/slices)
    {       
        ed = edge_depth;

        for(x = 0; x < max_x; x++)
        {
            switch(edge_type)
            {
                case ET_LEFT:
                {
                    map_x = x;
                    map_y = (max_y - 1) - y;
                    break;
                }           
        
                case ET_TOP:
                {
                    map_x = y;
                    map_y = x;//(max_x - 1) - x;
                    break;
                }

                case ET_RIGHT:
                {
                    map_x = (max_x - 1) - x;
                    map_y = (max_y - 1) - y;                    
                    break;
                }           
        
                case ET_BOTTOM:
                {
                    map_x = y;
                    map_y = (max_x - 1) - x;//x;
                    break;
                }
            }

            index = map_y * image.GetWidth() + map_x;
            //data->buffer[index] &= 128;            
            
            if(buffer[index] == clrBlack)
            {       
                if(ed > 0)
                    ed--;
                if(!ed)
                {
                    if(arrIndex == 0)
                    {
                        arrSlopes[arrIndex].x = x;
                        arrSlopes[arrIndex].y = y;
                        arrSlopes[arrIndex].slope = 0;                      
                        arrSlopes[arrIndex].rounded_slope = 0;
                    }
                    else
                    {
                        arrSlopes[arrIndex].x = x;
                        arrSlopes[arrIndex].y = y;
                        if(arrSlopes[arrIndex].x != arrSlopes[arrIndex - 1].x)
                        {
                            arrSlopes[arrIndex].slope = (arrSlopes[arrIndex].y - arrSlopes[arrIndex - 1].y) / (arrSlopes[arrIndex].x - arrSlopes[arrIndex - 1].x);
                            arrSlopes[arrIndex].rounded_slope = round(arrSlopes[arrIndex].slope, 1);
                        }
                        else
                        {
                            arrSlopes[arrIndex].slope = INFINITE;
                            arrSlopes[arrIndex].rounded_slope = INFINITE;
                        }

                        count++;
                        if(arrSlopes[arrIndex].x != arrSlopes[arrIndex - 1].x)
                        {
                            if(arrSlopes[arrIndex].slope < 0)
                            {
                                neg_count++;
                            }
                            else
                            {
                                pos_count++;
                            }
                        }
                        else
                        {
                            inf_count++;
                        }
                    }

                    arrSlopes[arrIndex].frequency = 0;
                    arrIndex++;
                    break;
                }
                else if(ed > 0)
                {
                    x += 3;                 
                }               
            }
        }
    }
    
    int max_freq_val = 0;
    double diff_x, diff_y, slope;
    for(int p = 0; p < arrIndex; p++)
    {       
        for(int q = p + 1; q < arrIndex; q++)
        {
            if(arrSlopes[p].rounded_slope == arrSlopes[q].rounded_slope)
            {
                diff_x = arrSlopes[q].x - arrSlopes[p].x;
                diff_y = arrSlopes[q].y - arrSlopes[p].y;
                if(diff_x == 0)
                {
                    slope = INFINITE;
                }
                else
                {
                    slope = diff_y / diff_x;
                }
                if(fabs(fabs(atan(slope)) - fabs(atan(arrSlopes[p].slope))) < 0.0349)
                {
                    arrSlopes[p].frequency++;
                    arrSlopes[q].frequency++;
                    if(max_freq_val < arrSlopes[p].frequency)
                    {
                        max_freq_val = arrSlopes[p].frequency;
                    }
                }
            }
        }
    }   
    
    SlopeInfo temp1, temp2;
    bool bFound = false;
    for(int t = 1; t < arrIndex; t++)
    {
        if(arrSlopes[t].frequency == max_freq_val)
        {
            if(bFound == false)
            {
                temp1.x = arrSlopes[t].x;
                temp1.y = arrSlopes[t].y;
                bFound = true;
            }
            else
            {
                temp2.x = arrSlopes[t].x;
                temp2.y = arrSlopes[t].y;
            }           
        }
    }

    double theta = 0.0;
    
    double rad_theta;
    if((temp2.x - temp1.x) == 0)
    {
        rad_theta = M_PI / 2;
    }
    else
    {
        rad_theta = sign * (atan((temp2.y - temp1.y) / (temp2.x - temp1.x)));
    }
    
    if(rad_theta < 0)
    { 
        theta = -((M_PI / 2) - (-rad_theta));
    }
    else
    {
        theta = (M_PI / 2) - rad_theta;
    }
    
    //double deg_theta = (theta * 180) / M_PI;
    
    // delete arrSlopes;   
    
    if(pos_count > neg_count)
    {
        pos_count += inf_count;
    }
    else
    {
        neg_count += inf_count;
    }
    if(indicator1)
    {
        *indicator1 = fabs((pos_count/count) - (neg_count/count));
    }
    if(indicator2)
    {
        *indicator2 = fabs(temp2.y - temp1.y);
    }
    
    return theta;
}
