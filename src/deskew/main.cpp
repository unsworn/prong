#include <stdio.h>
#include <stdlib.h>
                        
#include "log.h"
#include "options.h"            
#include "imagetypes.h"
#include "imageutils.h"
#include "imagedata.h"
#include "analyzer.h"          

#define PROGNAME "deskew"                    

const char* gDefaultOutputDir = ".";
    
void
usage()
{
    fprintf(stderr, "%s version 1.0\n", PROGNAME);
    fprintf(stderr, "%s -o <outputdir> <file>\n", PROGNAME);    
    exit(1);
}                           

int main(int argc, char** argv)
{
    ImageData i;
    Analyzer  a;
                          
    double skewAngle = 0;
    
    const char* inputFile;
    const char* outputPath;
    
    ParsedOptions opt;
    Option o;
    
    if (!options_parse(argc, argv, NULL, &opt))      
    {
        printf("unable to parse options\n");
        usage();
    }
                
    if ((o = options_find("o", &opt)) != NULL)
        outputPath = options_strval(o);
    else
        outputPath = gDefaultOutputDir;
    
    if ((o = options_find("arg0", &opt)) != NULL)
        inputFile = options_strval(o);
    else
        usage();
    
    if (!imageutils::LoadImageData(inputFile, &i))
    {
        printf("Unable to load input file %s\n", inputFile);
        return 1;
    }
                                                                      
    if (a.GetSkewAngle(false, i, skewAngle, 0))
    {
        TRACE("Image skew r = %f, d = %f", skewAngle, radToDeg(skewAngle));
    }
    else
        printf("Failed to detect image skew, assuming it's aligned..\n");
        
    return 0;
}