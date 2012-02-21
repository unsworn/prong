#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "options.h"

#include "list.h"

typedef struct __tagOption
{
    char*       name;
    char*       value;
} option_t;

//------------------------------------------------------------------------------------------------------------

static void
option_dealloc( void* ptr )
{
    if ( NULL == ptr )
        return ;
    
    option_t* opt = (option_t*) ptr;
    
    free( opt->name );
    free( opt->value );
    free( opt );
    
}

//------------------------------------------------------------------------------------------------------------

static void
options_print( void* ptr, void* ud )
{
    option_t* opt = (option_t*) ptr;
    if ( NULL == opt )
        return ;
    fprintf(stderr, "Option name=%s, value=%s\n", opt->name, (opt->value == NULL) ? "" : opt->value );
}

//------------------------------------------------------------------------------------------------------------

int
options_parse(int argc, char** _argv, const char* sarg, ParsedOptions* options)
{
    char*     ptr;
    char**    argv  = _argv;
    int       count = 0;
    int       skip  = 0;
    
    options->parsed = NULL;
    
    if (argc <= 1)
        return 1;
    
    list_t*   p     = ( list_t* ) malloc (sizeof(list_t));
    
    
    list_init( p, option_dealloc );
    
    argv++; // skip prog name..
    
    while ((*argv) != NULL)
    {
        ptr = NULL;
        
        if ( strchr((*argv), '-') == (*argv) )
        {
            ptr = (*argv);
            ptr++;

            if ((*ptr) == '-')
            {
                ptr++;
                
                option_t* opt = ( option_t* ) malloc (sizeof(option_t));
                opt->name = strdup(ptr);
                opt->value = strchr( opt->name, '=');
                if ( opt->value != NULL )
                {
                    (*(opt->value)++) = '\0';
                    opt->value = strdup( opt->value );
                }
                list_pushback( p, opt->name, opt );
            }
            else
            {
                option_t* opt = ( option_t* ) malloc (sizeof(option_t));
                opt->name  = strdup(ptr);
                opt->value = NULL;
                skip = ( NULL != sarg && strstr(sarg, opt->name) != NULL);
                if (!skip && *(argv+1) != NULL && strchr( *(argv+1), '-' ) == NULL)
                {
                    argv++;
                    opt->value = strdup((*argv));
                }
                list_pushback( p, opt->name, opt);
            }

        }
        else
        {
            option_t* opt = ( option_t* ) malloc (sizeof(option_t));
            opt->name = (char*)malloc( 16 );
            sprintf( opt->name, "arg%d" , count++ );
            opt->value = strdup( (*argv) );
            list_pushback( p, opt->name, opt );
        }

        argv++;

    }
    
    options->parsed = p;
    
    return 1;
}

//------------------------------------------------------------------------------------------------------------

Option
options_find(const char* option,  ParsedOptions* options)
{
    option_t* opt = (option_t*) list_find( (list_t*)options->parsed, option);
    return opt;
}

//------------------------------------------------------------------------------------------------------------

int
options_intval(Option option)
{
    const option_t* opt = (const option_t*) option;
    
    if (opt->value == NULL)
        return 0;

    return atol( opt->value );
}

//------------------------------------------------------------------------------------------------------------

const char*
options_strval(Option option)
{
    const option_t* opt = (const option_t*) option;
    return opt->value;
}

//------------------------------------------------------------------------------------------------------------

void
options_free(ParsedOptions* options)
{
    list_clear( (list_t*)options->parsed );
    free( (list_t*)options->parsed );
    options->parsed = NULL;
}

//------------------------------------------------------------------------------------------------------------

void
options_dump(ParsedOptions* options)
{
    list_foreach( (list_t*)options->parsed, options_print, NULL );
}
