#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "JSON.h"
                           
#define MODE_READ 0
#define MODE_WRITE 1
#define FILE_MAX 1<<16

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
        data(NULL),           
        root(NULL),
        mode(MODE_WRITE)
{
    
}


JSONData::~JSONData()
{
    if (mode == MODE_WRITE && data != NULL)
        free(data);
    if (mode == MODE_READ && root != NULL)
        yajl_tree_free(root);
        
    data = NULL;
    root = NULL;
}

void
JSONData::append(const char* name, const char* value)
{
    char* tmp;
    
    if (data == NULL || mode != MODE_WRITE)
        return ;

    int l0 = (int)strlen(data);
    int l1 = (int)strlen(name);
    int l2 = (int)strlen(value);
    int l3 = 2;

    TRACE("append() %d realloc(%d) (%s %s)", l0, l0 + l1 + l2 + l3, name, value);

    tmp = data;
    
    data = (char*)malloc(l0 + l1 + l2 + l3);
    
    if (data == NULL)
    {
        ERROR("append() malloc failed %d", 2);
        return ;
    }

    memcpy(data, tmp, l0);

    free(tmp);
    
    if (l0 == 1)
        sprintf(data + l0, "%s:%s", name, value);
    else
        sprintf(data + l0, ",%s:%s", name, value);
    
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
    int  i = 0;
    
    while (path[i] != NULL)
    {
        TRACE("Path: %s", path[i++]);
        
    }
    
    return NULL;
}

void
JSONData::open()
{                      
    if (mode != MODE_WRITE)
        return ;
                
    if (data != NULL)
        return ;

    data = (char*) malloc(2);

    if (data == NULL)
        return ;
    
    data[0] = '{';
    data[1] = '\0';
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
    
    if (data == NULL)
        return ;
    
    int len = (int)strlen(data);    

    data = (char*) realloc(data, len + 3);

    sprintf(data + len , "};");
    
}

void
JSONData::write(const char* f)
{             
    
    if (mode != MODE_WRITE)
        return ;
        
    if (data == NULL || f == NULL)
        return ;

    FILE* fp;

    if ((fp = fopen(f, "w+")) != NULL)
    {
        fwrite(data, 1, strlen(data), fp);
        fflush(fp);
        fclose(fp);
    }
}
                 
bool
JSONData::read(const char* f)
{   
    unsigned char* buf = NULL;
    char err[1024];
    
    if (data != NULL)
        return false;   
        
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