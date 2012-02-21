
#ifndef options_H_
#define options_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef const void* Option;

typedef struct
{
    Option parsed;
} ParsedOptions;

int
options_parse(int argc, char** argv, const char* sarg, ParsedOptions* options);

Option
options_find(const char* option, ParsedOptions* options);

int
options_intval(Option option);

const char*
options_strval(Option option);

void
options_free(ParsedOptions* options);

void
options_dump(ParsedOptions* options);
                                                 

#ifdef __cplusplus
}
#endif

#endif
