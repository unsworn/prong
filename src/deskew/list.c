
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

//------------------------------------------------------------------------------------------------------------

typedef struct list_item
{
    unsigned int      key;
    void*             value;
    struct list_item* next;    
} list_item_t;

//------------------------------------------------------------------------------------------------------------

unsigned int hash( const char* key)
{

    int len = strlen(key);
    
    const unsigned int m = 0x5bd1e995;
	const int r = 24;

	unsigned int h = 32 ^ len;

	const unsigned char * data = (const unsigned char *)key;

	while(len >= 4)
	{
		unsigned int k = *(unsigned int *)data;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h *= m; 
		h ^= k;

		data += 4;
		len -= 4;
	}
	
	switch(len)
	{
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
	        h *= m;
	};

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

//------------------------------------------------------------------------------------------------------------

list_item_t*
list_lookup( list_t* list, unsigned int key)
{
    list_item_t* c   = NULL;

    if ( NULL == list )
        return NULL;

    c = (list_item_t*)list->top;

    while ( c != NULL )
    {
        if ( c->key == key )
            return c;

        c = c->next;
    }

    return NULL;
}

//------------------------------------------------------------------------------------------------------------

void
list_init( list_t* list, void (*dealloc)(void *) )
{
    if ( NULL == list )
        return ;

    list->length  = 0;
    list->dealloc = dealloc;
    list->top     = NULL;
}

//------------------------------------------------------------------------------------------------------------

void
list_pushback( list_t* list, const char* name, void* data )
{
    unsigned int key = 0;
    
    if ( NULL == list )
        return ;

    if ( name != NULL )
        key = hash(name);

    if ( key )
        if ( list_lookup( list, key ) != NULL )
            return ;
    
    list_item_t* item = ( list_item_t *) malloc ( sizeof(list_item_t) );

    item->key   = key;
    item->value = data;
    item->next  = (list_item_t*)list->top;
    list->top   = (list_item_ptr)item;

    list->length ++;

    /*
    TRACE("list_pushback( %08X, %s, %08X ) key == %d length == %d\n",
          list, (name) ? name : "anonymou$", data, item->key, list->length);
    */
}

//------------------------------------------------------------------------------------------------------------

void*
list_pop( list_t* list )
{
    list_item_t* item = NULL;
    void*        val  = NULL;
    
    if ( NULL == list )
        return NULL;

    item = (list_item_t*)list->top;

    if ( item != NULL )
    {
        list->top = (list_item_ptr)item->next;

        val = item->value;

        free( item );
        
        list->length --;
    }

    return val;
}

//------------------------------------------------------------------------------------------------------------

void*
list_remove( list_t* list, const char* name )
{
    list_item_t* first = NULL;
    list_item_t* prev  = NULL;
    void*        val   = NULL;
    unsigned int key   = hash(name);
    
    if ( NULL == list )
        return NULL;

    first = (list_item_t*)list->top;

    while (first != NULL)
    {

        if ( first->key == key )
        {
            if ( prev != NULL )
                prev = first->next;
            else
                list->top = (list_item_ptr)first->next;

            val = first->value;
            free( first );

            list->length --;

            return val;
        }
        
        prev = first;
        first = first->next;
    }

    return NULL;
}

//------------------------------------------------------------------------------------------------------------

void*
list_find( list_t* list, const char* name )
{
    list_item_t* c = list_lookup( list, hash(name) );
    
    if ( NULL == c )
        return NULL;

    return c->value;
    
}

//------------------------------------------------------------------------------------------------------------

void*
list_item( list_t* list, int index )
{
    list_item_t* c = (list_item_t*) list->top;

    while ( c != NULL && --index >= 0 )
        c = c->next;

    if ( NULL == c )
        return NULL;

    return c->value;
}

//------------------------------------------------------------------------------------------------------------

void
list_clear( list_t* list )
{
    list_item_t* c;
    list_item_t* t;

    if ( NULL == list )
        return ;

    c = (list_item_t*)list->top;

    while ( c != NULL )
    {
        t = c;
        c = t->next;
                
        if (list->dealloc)
            if (t->value)
                list->dealloc( t->value );

        free( t);
        
    }

    list->length = 0;
    
}

//------------------------------------------------------------------------------------------------------------

int
list_count( list_t* list )
{
    list_item_t* ptr;
    int count = 0;

    if ( NULL == list )
        return 0;

    ptr = (list_item_t*)list->top;

    while( ptr != NULL )
    {
        count ++;
        ptr = ptr->next;
    }

    return count;
}


//------------------------------------------------------------------------------------------------------------

void  list_foreach( list_t* list, void (*elemcb)(void*, void*), void* userdata )
{
    list_item_t* c   = NULL;
    
    if ( NULL == list || NULL == elemcb)
        return ;

    c = (list_item_t*)list->top;

    while ( c != NULL )
    {
        elemcb( c->value, userdata );        
        c = c->next;
    }

}
