#ifndef JSON_H_
#define JSON_H_

#include <yajl/yajl_tree.h>
                 
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
    
    void open();    
    void close();
    
    yajl_val get() { return root; }
    
    void write(const char* path);
    bool read(const char* path);
    
private:
    char*    data;      
    yajl_val root;
    int      mode;
};

#endif
