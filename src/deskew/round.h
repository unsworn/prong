#ifndef round_H_
#define round_H_

#include <math.h>

double
round(double value, int precision)
{
    double temp1, temp2;

    temp1 = value * pow(10.0, (double)(precision + 1));

    if(value < 0.0)
    {
        temp1 -= 5.0;
    }
    else
    {
        temp1 += 5.0;
    }

    temp1 /= 10.0;
    modf(temp1, &temp2);

    return temp2 / pow(10.0, (double)precision);
}

#endif

