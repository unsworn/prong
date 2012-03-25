#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#include "list.h"
#include "JSON.h"
              
#include "log.h"
#include "template.h"
                  
static const char* json_path_absoluteX[] = {"absoluteX", NULL};
static const char* json_path_absoluteY[] = {"absoluteY", NULL};
static const char* json_path_absoluteW[] = {"absoluteW", NULL};
static const char* json_path_absoluteH[] = {"absoluteH", NULL};
static const char* json_path_relativeX[] = {"relativeX", NULL};
static const char* json_path_relativeY[] = {"relativeY", NULL};
static const char* json_path_relativeW[] = {"relativeW", NULL};
static const char* json_path_relativeH[] = {"relativeH", NULL};
static const char* json_path_name[]      = {"name", NULL};
static const char* json_path_type[]      = {"type", NULL};
static const char* json_path_parent[]    = {"parent", NULL};
static const char* json_path_clz[]       = {"clz", NULL};
static const char* json_path_width[]     = {"width", NULL};
static const char* json_path_height[]    = {"height", NULL};
static const char* json_path_marksx[]    = {"marksx", NULL};
static const char* json_path_marksy[]    = {"marksy", NULL};
static const char* json_path_units[]     = {"units", NULL};

/* Box */
Box::Box() :
next(NULL),        
owner(NULL),
name(NULL),
parent(NULL),
path(NULL),
enabled(false)
{
}

Box::~Box()
{
    if (name != NULL)
        free(name);
    name = NULL;
    if (parent != NULL)
        free(parent);
    parent = NULL;
}                      

         
/* Template */
Template::Template() :
        box(NULL),
        points(false)
{
    scale = 1.0;
}   

Template::~Template()
{
    Box* tmp;
    Box* ptr = box;
    
    while(ptr != NULL)
    {
        tmp = ptr;
        ptr = ptr->next;
        delete tmp;
    }              
    
    box = NULL;
}             

bool
Template::read(const char* path)
{
    JSONData data;
    
    if (!data.read(path))
    {
        TRACE("Read %s failed", path);
        return false;
    }
    
    yajl_val root = data.get();
    
    if (!YAJL_IS_ARRAY(root))
    {
        ERROR("read(), root element != array %d", 0);
        return false;
    }                
    
    int count = root->u.array.len;
    
    TRACE("Num boxes %d", count);
    
    for (int i=0 ; i < count ; i++)
    {
        yajl_val obj = root->u.array.values[i];
        
        if (!YAJL_IS_OBJECT(obj))
        {
            TRACE("Skipping index %d, not an object", i);
            continue;
        }            
        
        this->fromObject(obj);
        
    }

    return true;
}             

void
Template::setCrop(Rect &r)
{
    crop.origin.x    = r.origin.x;
    crop.origin.y    = r.origin.y;
    crop.size.width  = r.size.width;
    crop.size.height = r.size.height;
}

void
Template::fromObject(void* ptr)
{
    yajl_val obj = (yajl_val) ptr;    
    yajl_val v;
        
    const char* tmp = NULL;                          
    char* name = NULL;        
    char* parent = NULL;          
    int   clazz = GAME_CLASS_NONE;
    int   type = GAME_TYPE_UNKNOWN;
                                    
    double aX=0, aY=0, aW=0, aH=0;
    double rX=0, rY=0, rW=0, rH=0;
    
    if ((v = yajl_tree_get(obj, json_path_name, yajl_t_string)) != NULL)
    {
        tmp = YAJL_GET_STRING(v);
        if (tmp)
            name = strdup(tmp);
    }   
    
    if ((v = yajl_tree_get(obj, json_path_type, yajl_t_string)) != NULL)
    {
        tmp = YAJL_GET_STRING(v);
        if (tmp && strcmp("property", tmp) == 0)
            type = GAME_TYPE_PROPERTY;
        else if (tmp && strcmp("mark", tmp) == 0)
            type = GAME_TYPE_MARK;
        else if (tmp && strcmp("baseline", tmp) == 0)
            type = GAME_TYPE_BASE;
        else if (tmp && strcmp("info", tmp) == 0)
            type = GAME_TYPE_INFO;
        else
            type = GAME_TYPE_GRAPHIC;
    }                                 

    if (type == GAME_TYPE_INFO)
    {
        if ((v = yajl_tree_get(obj, json_path_width, yajl_t_number)) != NULL)
            this->size.width = YAJL_GET_DOUBLE(v);

        if ((v = yajl_tree_get(obj, json_path_height, yajl_t_number)) != NULL)
            this->size.height = YAJL_GET_DOUBLE(v);

        if ((v = yajl_tree_get(obj, json_path_marksx, yajl_t_number)) != NULL)
            this->markSz.width = YAJL_GET_DOUBLE(v);

        if ((v = yajl_tree_get(obj, json_path_marksy, yajl_t_number)) != NULL)
            this->markSz.height = YAJL_GET_DOUBLE(v);

        if ((v = yajl_tree_get(obj, json_path_units, yajl_t_string)) != NULL)
            this->points = (strcmp("points", YAJL_GET_STRING(v)) == 0);
        
        return ;
    }

    
    if (type == GAME_TYPE_PROPERTY && (v = yajl_tree_get(obj, json_path_parent, yajl_t_string)) != NULL)
    {
        tmp = YAJL_GET_STRING(v);
        if (tmp)
            parent = strdup(tmp);            
    }

    if (type == GAME_TYPE_GRAPHIC && (v = yajl_tree_get(obj, json_path_clz, yajl_t_string)) != NULL)
    {
        tmp = YAJL_GET_STRING(v);
        
        
        // background, player, entity, static
        if (strcmp("background", tmp) == 0)
        {
            clazz = GAME_CLASS_BACKGROUND;
        }   
        else if(strcmp("player", tmp) == 0)
        {
            clazz = GAME_CLASS_PLAYER;
        }
        else if(strcmp("entity", tmp) == 0)
        {
            clazz = GAME_CLASS_ENTITY;
        }   
        else if(strcmp("static", tmp) == 0)
        {
            clazz = GAME_CLASS_STATIC;
        }
    }       

    
    if ((v = yajl_tree_get(obj, json_path_absoluteX, yajl_t_number)) != NULL)
        aX = YAJL_GET_DOUBLE(v);

    if ((v = yajl_tree_get(obj, json_path_absoluteY, yajl_t_number)) != NULL)
        aY = YAJL_GET_DOUBLE(v);

    if ((v = yajl_tree_get(obj, json_path_absoluteW, yajl_t_number)) != NULL)
        aW = YAJL_GET_DOUBLE(v);

    if ((v = yajl_tree_get(obj, json_path_absoluteH, yajl_t_number)) != NULL)
        aH = YAJL_GET_DOUBLE(v);

    if ((v = yajl_tree_get(obj, json_path_relativeX, yajl_t_number)) != NULL)
        rX = YAJL_GET_DOUBLE(v);

    if ((v = yajl_tree_get(obj, json_path_relativeY, yajl_t_number)) != NULL)
        rY = YAJL_GET_DOUBLE(v);

    if ((v = yajl_tree_get(obj, json_path_relativeW, yajl_t_number)) != NULL)
        rW = YAJL_GET_DOUBLE(v);

    if ((v = yajl_tree_get(obj, json_path_relativeH, yajl_t_number)) != NULL)
        rH = YAJL_GET_DOUBLE(v);

    
    if (name != NULL)
    {

        if (type == GAME_TYPE_MARK)
        {
            
            if (strcmp(name, "markTL") == 0)
            {
                mark[GAME_MARK_TL].x = aX + (aW/2);
                mark[GAME_MARK_TL].y = aY + (aH/2);
            }
            else if (strcmp(name, "markTR") == 0)
            {
                mark[GAME_MARK_TR].x = aX + (aW/2);
                mark[GAME_MARK_TR].y = aY + (aH/2);
            }
            else if (strcmp(name, "markBL") == 0)
            {
                mark[GAME_MARK_BL].x = aX + (aW/2);
                mark[GAME_MARK_BL].y = aY + (aH/2);
            }
            else if (strcmp(name, "markBR") == 0)
            {
                mark[GAME_MARK_BR].x = aX + (aW/2);
                mark[GAME_MARK_BR].y = aY + (aH/2);
            }

            return ;
            
        }
        
        Box* b = new Box();
                
        TRACE("\nLoading definition:\n"
            "%s {\n"
            "\tparent   %s\n"
            "\ttype     %d\n"
            "\tclazz    %d\n"
            "\tabs      [%f, %f, %f, %f]\n"
            "\tres      [%f, %f, %f, %f]\n"
            "\trel      [%1.14f, %1.14f, %1.14f, %1.14f]\n"
            "}\n",
            name, parent, type, clazz, aX, aY, aW, aH, rX, rY ,rW, rH, rX, rY, rW, rH);
        
        b->name = name;
        b->parent = parent;
        b->type = type;
        b->clazz = clazz;
                                  
        b->abs.origin.x = aX;
        b->abs.origin.y   = aY;
        b->abs.size.width = aW;
        b->abs.size.height = aH;
        
        b->rel.origin.x = rX;
        b->rel.origin.y = rY;
        b->rel.size.width = rW;
        b->rel.size.height = rH;

        b->next = this->box;
        
        this->box = b;
        
    }
}        

Box*
Template::getBox(const char* name, int type)
{
    Box* ptr = this->box;
    
    while(ptr != NULL)
    {
        if ((ptr->type == type || type == -1) && strcmp(name, ptr->name) == 0)
            return ptr;
        ptr = ptr->next;
    }
    return NULL;
}
