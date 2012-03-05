#ifndef imagetypes_H_
#define imagetypes_H_

#include <math.h>

#define P_B 100000  // Previous Pixel Black
#define C_B    100  // Current Pixel Black
#define P_G  10000  // Previous Pixel Gray
#define C_G     10  // Current Pixel Gray
#define P_W   1000  // Previous Pixel White
#define C_W      1  // Current Pixel White

// Color Patterns 

// Changing Patterns
#define B2G 100010  // Black to Gray
#define G2B  10100  // Gray to Black
#define G2W  10001  // Gray to White
#define W2G   1010  // White to Gray
#define B2W 100001  // Black to White
#define W2B   1100  // White to Black   

// Static Patterns
#define W2W   1001  // White to White
#define B2B 100100  // Black to Black
#define G2G  10010  // Gray to Gray

#ifndef byte
    #define byte unsigned char
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define radToDeg(r)  (r * 57.2957795)
#define degToRad(d)  (d / 57.2957795)

#endif
