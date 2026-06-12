#ifndef _EXT_DY_STR_H_
#define _EXT_DY_STR_H_

#include "ext_strs.h"
#define ext_dy_str_si_strap static inline
#define DEFAULT_STRING_SIZE 10
typedef struct Dynamic_String{
    char * s_string;
    size_t s_index;
    size_t s_size;
}Dynamic_String;
ext_dy_str_si_strap const char * get_dy_string(Dynamic_String * dy_str){return dy_str->s_string;}
Dynamic_String init_dynamic_string(size_t init_size);
Dynamic_String * add_char_to_dynamic_string(Dynamic_String * dy_str, const char word);
Dynamic_String * add_dynamic_string_to_dynamic_string(Dynamic_String * dy_str, const Dynamic_String * dy_str_1);
Dynamic_String * set_char_to_dynamic_string(Dynamic_String * dy_str, const size_t dy_str_index, const char word);
void  delete_dynamic_string(Dynamic_String * dy_str);
#endif //EXT_DY_STR_H_