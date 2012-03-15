#ifndef template_H_
#define template_H_

#include "list.h"
#include "JSON.h"
                
enum 
{            
    GAME_TYPE_UNKNOWN,
    GAME_TYPE_GRAPHIC,
    GAME_TYPE_PROPERTY
};

enum
{                    
    GAME_CLASS_NONE,
    GAME_CLASS_PLAYER,
    GAME_CLASS_BACKGROUND,
    GAME_CLASS_ENTITY,
    GAME_CLASS_STATIC
};
                        
/**
 * Point
 */
class Point
{         
public:    
    float x;
    float y;
};
   
/**
 * Dimension
 */
class Size
{         
public:    
    float width;
    float height;
};

/**
 * Rectangle
 */
class Rect
{
public:    
    Point origin;
    Size  size;
};
                                       
/**
 * Template Box
 */        
class Box
{
public:
    Box();
    virtual ~Box();
    
    Box*  next;  
    Box*  owner;                          
    char* name;       
    char* parent;
    int   type;
    int   clazz;      
    Rect  abs;
    Rect  rel;      
    char* path;
    bool  enabled;
};
   
        
/**
 * Game Template
 */           
class Template
{
public:
    Template();
    virtual ~Template();      
    
    bool   read(const char* path);
                                
    void   setSkewAngle(double angle) { skew = angle; }
    
    double getSkewAngle() { return skew; }
    
    Box*   getFirstBox()  { return box; }
                    
    Box*   getBox(const char* name, int type=-1);

    void   setCropBox(Rect &r) {
        crop.origin.x    = r.origin.x;
        crop.origin.y    = r.origin.y;
        crop.size.width  = r.size.width;
        crop.size.height = r.size.height;
    }
    
    Rect*  getCropBox()      { return &crop; }

protected:
    void   fromObject(void* ptr);
        
private:
    Box*   box;    
    Rect   crop;
    double skew;
    
    
};                      

#endif
