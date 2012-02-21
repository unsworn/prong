#include <stdio.h>

#include "analyzer.h"
    
#define PROGNAME "deskew"                    
    
void
usage()
{
    fprintf(stderr, "%s version 1.0\n", PROGNAME);
    fprintf(stderr, "%s -i inputfile -o outputdir\n", PROGNAME);    
}                           

int main(int argc, char** argv)
{            
    usage();
    return 0;
}