#ifndef list_H_
#define list_H_
                                                   
#ifdef __cplusplus
extern "C" {
#endif
    
//------------------------------------------------------------------------------------------------------------

typedef struct list_item_t* list_item_ptr;

//------------------------------------------------------------------------------------------------------------

typedef struct list
{
    int            length;
    void         (*dealloc)(void*);
    list_item_ptr  top;  
} list_t;

//------------------------------------------------------------------------------------------------------------

void  list_init( list_t* list, void (*dealloc)(void*) );

void  list_pushback( list_t* list, const char* name, void* data );

void* list_pop( list_t* list );

void* list_remove( list_t* list, const char* name );

void* list_find( list_t* list, const char* name );

void* list_item( list_t* list, int index );

void  list_clear( list_t* list );

int   list_count( list_t* list );

void  list_foreach( list_t* list, void (*elemcb)(void*, void*), void* userdata );

#ifdef __cplusplus
}
#endif

#endif
                                            

