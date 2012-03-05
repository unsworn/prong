#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "JSON.h"

JSONData::JSONData() :
        data(NULL)
{
    
}


JSONData::~JSONData()
{
    if (data != NULL)
        free(data);
}

void
JSONData::append(const char* name, const char* value)
{
    char* tmp;
    
    if (data == NULL)
        return ;

    int l0 = strlen(data);
    int l1 = strlen(name);
    int l2 = strlen(value);
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

void
JSONData::open()
{
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
    if (data == NULL)
        return ;
    
    int len = strlen(data);    

    data = (char*) realloc(data, len + 3);

    sprintf(data + len , "};");
    
}

const char*
JSONData::get()
{
    return data;
}

void
JSONData::write(const char* f)
{
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
