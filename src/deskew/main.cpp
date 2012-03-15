#include <stdio.h>
#include <string.h>
#include <stdlib.h>
                        
#include "log.h"
#include "config.h"
#include "options.h"            
#include "imagetypes.h"
#include "imageutils.h"
#include "imagedata.h"
#include "analyzer.h"          
#include "utils.h"
#include "template.h"
#include "JSON.h"


/***********

Special thanks to:

Slab Bulkhead
Fridge Largemeat
Punt Speedchunk
Butch Deadlift
Bold Bigflank

Splint Chesthair
Flint Ironstag
Bolt VanderHuge
Thick McRunFast
Blast Hardcheese

Buff Drinklots
Trunk Slamchest

Fist Rockbone
Stump Beefknob
Smash Lampjaw
Punch Rockgroin
Buck Plankchest

Stump Chunkman
Dirk Hardpec
Rip Steakface
Slate Slabrock
Crud Bonemeal

Rip Slagcheek
Punch Sideiron
Gristle McThornBody
Slake Fistcrunch
Buff Hardback

Bob Johnson
Blast Thickneck
Crunch Buttsteak
Slab Squatthrust
Lump Beefbroth

Touch Rustrod
Reef Blastbody
Big McLargeHuge
Smoke Manmuscle
Pete Punchbeef

Pack Blowfist
Roll Fizzlebeef

*/

#define PROGNAME "deskew"                    


void usage();

void
usage()
{
    fprintf(stderr, "%s version 1.0\n", PROGNAME);
    fprintf(stderr, "utils version %d\n", imageutils::version());
    fprintf(stderr, "Syntax:\n");
    fprintf(stderr, "%s <options> <image file>\n", PROGNAME);          
    fprintf(stderr, "\t-u             (units, degrees|radians)\n");
    fprintf(stderr, "\t-o             output path / file\n");
    fprintf(stderr, "\t-a             angle, if mode = rotate\n");
    fprintf(stderr, "\t-t             template file\n");
    fprintf(stderr, "\t-m             mode, info|rotate|crop\n");
    fprintf(stderr, "\t-v             (be verbose)\n");
    fprintf(stderr, "\t-c             crop box XxY+W+H\n");
    
    exit(1);
}                           


enum {
    MODE_INFO    = 0x01,
    MODE_ROTATE  = 0x02,
    MODE_PROCESS = 0x04
};


int main(int argc, char** argv)
{
    FIBITMAP*     bmp;
    const char*   inputFile = NULL;
    ParsedOptions opt;
    Option        o;
    bool          degrees = false;
    const char*   tmp;
    int           mode    = MODE_INFO;
    const char*   outp    = NULL;
    const char*   tmlp    = NULL;
    double        angle   = 0;
    bool          doCrop  = false;
    Rect          cropBox;

    
    if (!options_parse(argc, argv, NULL, &opt))      
    {
        ERROR("Unable to parse options argc=%d\n", argc);
        usage();
    }

    if ((o = options_find("u", &opt)) != NULL)
    {
        tmp = options_strval(o);
        if (NULL != tmp && strcmp(tmp, "degrees") == 0)
            degrees = true;
    }

    if ((o = options_find("o", &opt)) != NULL)
    {
        outp = options_strval(o);
        TRACE("OUTPUT: %s", outp);
    }

    if ((o = options_find("a", &opt)) != NULL)
    {
        angle = atof(options_strval(o));
        TRACE("ANGLE: %f", angle);
    }

    if ((o = options_find("t", &opt)) != NULL)
    {
        tmlp = options_strval(o);
        mode = MODE_PROCESS;
        TRACE("FILE: %s", tmlp);
    }

    if ((o = options_find("c", &opt)) != NULL)
    {
        const char* tmp = options_strval(o);
        get_crop_box(tmp, cropBox);
        doCrop = true;

    }
    
    if ((o = options_find("m", &opt)) != NULL)
    {
        mode = 0x0;

        tmp = options_strval(o);

        TRACE("MODE: %s", tmp);
        
        if (tmp != NULL)
        {
            if (strstr(tmp, "info") != NULL)
                mode |= MODE_INFO;
            if (strstr(tmp, "rotate") != NULL)
                mode |= MODE_ROTATE;
            if (strstr(tmp, "crop") != NULL)
                mode |= MODE_PROCESS;
        }

    }
    
    if ((o = options_find("arg0", &opt)) != NULL)
        inputFile = options_strval(o);
    else
        usage();
    

    if (tmlp != NULL)
    {
        Template t;
        
        if (!t.read(tmlp))
        {
            ERROR("Unable to read %s", tmlp);
            return 1;
        }
                          
        if (degrees)
            angle = degToRad(angle);
        
        t.setSkewAngle(angle);

        if (doCrop)
            t.setCropBox(cropBox);
        
        crop_and_exit(inputFile, &t, outp);
        
    }

    
    if ((mode & MODE_INFO))
        calculate_skew_and_exit(inputFile, degrees, outp);


    if ((bmp = imageutils::LoadBitmap(inputFile)) == NULL)
    {
        ERROR("Unable to read file: %s", inputFile);
        return 1;
    }

    if ((mode & MODE_ROTATE) && angle != 0.0)
    {
        if (degrees)
            angle = degToRad(angle);

        TRACE("Rotating input: -%f", angle);
        
        FIBITMAP* rmp;
        
        if ((rmp = imageutils::GetRotatedBitmap(bmp, -angle, NULL)) == NULL)
        {
            ERROR("Rotate %f %c failed on input", angle, ((degrees) ? 'd' : 'r'));
            return 1;
        }
        
        imageutils::FreeBitmap(bmp);

        bmp = rmp;

        if (outp != NULL && tmlp == NULL)
        {
            TRACE("Writing rotated bitmap to: %s", outp);
            imageutils::SaveBitmapToFile(bmp, outp);
            return 0;
        }                   
                
    }
    
    return 0;
}
