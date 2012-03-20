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
    fprintf(stderr, "\t-a             angle, with -r \n");
    fprintf(stderr, "\t-j             use job file\n");
    fprintf(stderr, "\t-r             rotate\n");
    fprintf(stderr, "\t-c             crop\n");
    fprintf(stderr, "\t-s             scale\n");
    fprintf(stderr, "\t-i             info\n");    
    fprintf(stderr, "\t-v             (be verbose)\n");
    fprintf(stderr, "\t-g             geometry XxY+W+H with -c or -s\n");
    
    exit(1);
}                           


enum {
    MODE_INFO    = 0x01,
    MODE_ROTATE  = 0x02,
    MODE_SCALE   = 0x04,
    MODE_CROP    = 0x08,
    MODE_PROCESS = 0x10
};


int main(int argc, char** argv)
{
    FIBITMAP*     bmp;
    const char*   inputFile = NULL;
    ParsedOptions opt;
    Option        o;
    bool          degrees = false;
    const char*   tmp;
    int           mode    = 0;
    const char*   outp    = NULL;
    const char*   tmlp    = NULL;
    double        angle   = 0;
    bool          gset    = false;
    Rect          geometry;
    float         scale   = 1.0;
    
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

    if ((o = options_find("g", &opt)) != NULL)
    {
        const char* tmp = options_strval(o);
        get_crop_box(tmp, geometry);        
        gset = true;
        TRACE("geometry [%f, %f, %f, %f]", geometry.origin.x, geometry.origin.y, geometry.size.width, geometry.size.height);
    }

    if ((o = options_find("i", &opt)) != NULL)
    {
        mode |= MODE_INFO;
    }
    
    if ((o = options_find("r", &opt)) != NULL)
    {
        mode |= MODE_ROTATE;
    }

    if ((o = options_find("c", &opt)) != NULL)
    {
        mode |= MODE_CROP;
    }

    if ((o = options_find("s", &opt)) != NULL)
    {
        mode |= MODE_SCALE;
        if (!gset)
            scale = atof(options_strval(o));
    }

    if ((o = options_find("j", &opt)) != NULL)
    {
        tmlp = options_strval(o);
        mode = MODE_PROCESS;
        TRACE("FILE: %s", tmlp);
    }

    
    if ((o = options_find("arg0", &opt)) != NULL)
        inputFile = options_strval(o);
    else
        usage();
    

    if (mode == MODE_PROCESS && tmlp != NULL)
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

        if (gset)
            t.setCrop(geometry);

        t.setFinalScale(scale);
        
        crop_and_exit(inputFile, &t, outp);

        return 0;
        
    }

    
    if ((mode & MODE_INFO))
    {
        TRACE("Gathering info %d\n", mode);
        calculate_skew_and_exit(inputFile, degrees, outp);
    }

    TRACE("Work %d", mode);
    
    if ((bmp = imageutils::LoadBitmap(inputFile)) == NULL)
    {
        ERROR("Unable to read file: %s", inputFile);
        return 1;
    }

    if ((mode & MODE_CROP) && gset)
    {
        TRACE("Cropping.. %d\n", mode);
        
        FIBITMAP* rmp;

        if ((rmp = imageutils::GetCroppedBitmap(bmp, geometry)) == NULL)
        {
            ERROR("Crop failed %d", 0);
            return 1;
        }

        imageutils::FreeBitmap(bmp);

        bmp = rmp;
        
    }

    if ((mode & MODE_SCALE) && gset)
    {
        TRACE("Scaling.. %d\n", mode);
        
        FIBITMAP* rmp;

        if (!gset)
        {
            geometry.origin.x = geometry.origin.y = 0;
            geometry.size.width  = (float)FreeImage_GetWidth(bmp)  * scale;
            geometry.size.height = (float)FreeImage_GetHeight(bmp) * scale;
        }
        
        if ((rmp = imageutils::GetScaledBitmap(bmp, geometry)) == NULL)
        {
            ERROR("Unable to scale %d", 0);
            return 1;
        }

        imageutils::FreeBitmap(bmp);

        bmp = rmp;
        
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

    }

    if (outp != NULL && tmlp == NULL)
    {
        imageutils::SaveBitmapToFile(bmp, outp);
        return 0;
    }                   
                

    return 0;
}
