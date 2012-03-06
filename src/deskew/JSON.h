#ifndef JSON_H_
#define JSON_H_

#include <yajl/yajl_tree.h>
                 
class JSONData
{
public:

    JSONData();
    virtual ~JSONData();

    void append(const char* name, const char* value);
    void append(const char* name, int value);
    void append(const char* name, double value);
                   
    void open();    
    void close();
    
        
    void write(const char* path);
    bool read(const char* path);
    
private:
    char*    data;      
    yajl_val root;
    int      mode;
};

#endif