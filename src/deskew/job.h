#ifndef job_H_
#define job_H_

#include "list.h"


enum
{
    BOX_TYPE_UNKNOWN,
    BOX_TYPE_BACKGROUND,
    BOX_TYPE_ACTOR,
    BOX_TYPE_ELEMENT,
    BOX_TYPE_PROPERTY
};

enum
{
    UNITS_PIXELS,
    UNITS_FACTOR,
    UNITS_MILLIMETER
};

class Box
{
public:
    Box();
    virtual ~Box() {}

public:
    Box*   next;    
    char*  name;
    int    type;
    int    units;
    double top;
    double left;
    double right;
    double bottom;
};
    
class Job
{

public:
    Job();
    virtual ~Job();

    void read(const char* jobFile);

};
#endif
