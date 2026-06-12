#include "ext_dy_str.h"
Dynamic_String init_dynamic_string(size_t init_size){
    if(init_size < 2) init_size = DEFAULT_STRING_SIZE;
    Dynamic_String ds;
    ds.s_string = smart_calloc(char, init_size);
    if(!ds.s_string) return ds;
    ds.s_size = init_size;
}
Dynamic_String * add_char_to_dynamic_string(Dynamic_String * dy_str, const char word){
    /**
     * 2 2
     * 0
     * 1
     * 2
    */
    if(!word) return dy_str;
    if(!safety_realloc((void **)dy_str->s_string, dy_str->s_index + 1, &dy_str->s_size, sizeof(char))) return NULL;
    dy_str->s_string[dy_str->s_index] = word;
    dy_str->s_string[dy_str->s_index + 1] = ET_CHAR_NULL;
    dy_str->s_index++;
}
Dynamic_String * add_dynamic_string_to_dynamic_string(Dynamic_String * dy_str, const Dynamic_String * dy_str_1){
    if(!safety_realloc((void **)dy_str->s_string, dy_str->s_index + dy_str_1->s_index + 1, &dy_str->s_size, sizeof(char))) return NULL;
    memcmp(dy_str->s_string + dy_str->s_index, dy_str_1->s_string, sizeof(char) * dy_str_1->s_index);
    return dy_str;
}
Dynamic_String * set_char_to_dynamic_string(Dynamic_String * dy_str, const size_t dy_str_index, const char word){
    if(dy_str->s_size - 1 >= dy_str_index) return dy_str;
    dy_str->s_string[dy_str_index] = word;
}
void delete_dynamic_string(Dynamic_String * dy_str){
    s_free(dy_str->s_string);
    dy_str->s_index = 0;
    dy_str->s_size = 0;
}