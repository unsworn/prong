#ifndef utils_H_
#define utils_H_
                                    
#include "template.h"
            
void
calculate_skew_and_exit(const char*, bool, const char*);
              
void
crop_and_exit(const char*, Template*, const char*);

void
get_crop_box(const char*, Rect &r);


#endif
