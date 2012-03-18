#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "buffer.h"


buffer::buffer() : data(NULL)
{
}

buffer::~buffer()
{
    if (data != NULL)
        free(data);
}


void
buffer::append(const char* str)
{
    char* ptr = NULL;
    
    if (str == NULL)
        return ;

    unsigned nsize = strlen(str);

    if ((ptr = alloc(nsize)) != NULL)
        strcpy(ptr, str);

}

unsigned
buffer::length()
{
    if (data == NULL)
        return 0;
    return strlen(data);
}

unsigned
buffer::size()
{
    if (data == NULL)
        return 0;
    return sizeof(data);
}

void
buffer::clear()
{
    if (data != NULL)
        free(data);
    data = NULL;
}

char
buffer::last()
{
    unsigned len = this->length();
    if (len > 0 && data != NULL)
        return data[len-1];
    return 0;
}

const char*
buffer::get()
{
    return data;
}

char*
buffer::alloc(unsigned size)
{
    unsigned len = this->length();

    char* tmp = NULL;

    if ((tmp = (char*)realloc(data, len + size + 1)) != NULL)
    {
        data = tmp;
        return (data+len);
    }

    tmp = (char*)malloc(len + size + 1);

    if (data != NULL)
    {
        memcpy(tmp, data, len);
        free(data);
    }

    memset(tmp+len, 0, size+1);

    data = tmp;

    return (data+len);
    
}

