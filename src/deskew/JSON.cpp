#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "JSON.h"
                           
#define MODE_READ 0
#define MODE_WRITE 1
#define FILE_MAX 1<<16

#define CLAZ_OBJECT 0
#define CLAZ_ARRAY 1

bool
JSONReadFile(const char* path, unsigned char** buf);

bool
JSONReadFile(const char* path, unsigned char** buf)
{
    FILE* fp = NULL;
    long  len = 0;
    
    if (buf == NULL)
        return false;
        
    if ((fp = fopen(path, "r")) == NULL)
        return false;
       
    fseek(fp, 0L, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
                
    if (len > FILE_MAX)
    {              
        ERROR("ReadFile(), file size %d > MAX", len);
        fclose(fp);
        return false;
    }
        
    if (((*buf) = (unsigned char*) malloc(len + 1)) == NULL )
    {
        ERROR("ReadFile(), malloc failed %d", len);
        fclose(fp);
        return false;
    }                      

    fread((*buf), 1, len, fp);
             
    TRACE("Read %d bytes", len);
    
    (*buf)[len] = '\0';
                   
    fclose(fp);
    
    return true;   
}   

JSONData::JSONData() :
        root(NULL),
        mode(MODE_WRITE),
        clz(CLAZ_OBJECT)
{
    
}


JSONData::~JSONData()
{
    if (mode == MODE_READ && root != NULL)
        yajl_tree_free(root);
        
    root = NULL;
}

void
JSONData::append(const char* name, const char* value)
{
    if (mode != MODE_WRITE)
        return ;

    char c;

    if ((c = data.last()) != 0)
        if (c != '{')
            data.append(",");

    data.append("\"");
    data.append(name);
    data.append("\":");
    data.append(value);
}

void
JSONData::append(const char* name, int value)
{
    char tmp[256];
    sprintf(tmp, "%d", value);
    return this->append(name, tmp);
}

void
JSONData::append(const char* name, double value)
{
    char tmp[256];
    sprintf(tmp, "%f", value);
    return this->append(name, tmp);
}

const char* 
JSONData::find(const Path path[])
{
    return NULL;
}

void
JSONData::array()
{
    clz = CLAZ_ARRAY;
}

void
JSONData::open()
{                      
    if (mode != MODE_WRITE)
        return ;
                
    if (clz == CLAZ_ARRAY)
    {
        if (data.length() == 0)
        {
            data.append("[{");
            return ;
        }
        if (data.last() == '}')
            data.append(",");
        data.append("{");        
        return ;

    }

    data.append("{");
    
}

void
JSONData::close()
{                   
    if (mode == MODE_READ && root != NULL)
    {
        yajl_tree_free(root);
        root = NULL;
        mode = MODE_WRITE;
        return ;
    }                     
    
    data.append("}");
    
}

void
JSONData::write(const char* f)
{             
    
    if (mode != MODE_WRITE)
        return ;
        
    if (f == NULL)
        return ;

    FILE* fp;

    if (clz == CLAZ_ARRAY)
        data.append("]");
    
    if ((fp = fopen(f, "w+")) != NULL)
    {
        fwrite(data.get(), 1, data.length(), fp);
        fflush(fp);
        fclose(fp);
    }
}
                 
bool
JSONData::read(const char* f)
{   
    unsigned char* buf = NULL;
    char err[1024];
        
    mode = MODE_READ;
    
    if (root != NULL)
        return false;
    
    if (!JSONReadFile(f, &buf))
    {
        TRACE("read() failed to read %s", f);
        return false;
    }                
    
    if (buf == NULL)
    {
        TRACE("read(), no data returned from file operation %d", 0);
        return false;
    }                
    
    root = yajl_tree_parse((char*)buf, err, sizeof(err));
    
    free(buf);
    
    if (root == NULL)
    {
        ERROR("read(), could not parse data %d", 0);
        fprintf(stderr, "PARSE ERROR: ");
        if (strlen(err))
            fprintf(stderr, " %s", err);
        else
            fprintf(stderr, " unknown error");
        fprintf(stderr, "\n");
        
        return false;
    }                
    
    return true;
    
}
