#ifndef JSON_H_
#define JSON_H_

#include "ytree.h"
#include "buffer.h"

class JSONData
{
public:

    typedef const char* Path;
    
    JSONData();
    virtual ~JSONData();

    void append(const char* name, const char* value);
    void append(const char* name, int value);
    void append(const char* name, double value);
                   
    const char* find(const Path path[]);

    void array();
    
    void open();    

    void close();
    
    yajl_val get() { return root; }
    
    void write(const char* path);
    
    bool read(const char* path);
    
private:
    buffer   data;      
    yajl_val root;
    int      mode;
    int      clz;
};

#endif
