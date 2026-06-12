#include "ext_cio.h"
#include <stdarg.h>
char et_read__(){
    char c__ = c_read__();
    if(c__ == EOF || c__ == ET_CHAR_LN) return ET_CHAR_NULL;
    return c__;
}
char * et_reads__(char * buff, size_t buff_size){
    if(!buff || buff_size == 0) return NULL;
    char tmp_c__ = ET_CHAR_NULL;
    size_t i__ = 0x0;
    buff_size -= 1;
    while(i__ < buff_size && (tmp_c__ = et_read__())) *(buff + i__++) = tmp_c__; 
    *(buff + i__) = ET_CHAR_NULL;
    /**
     * 5
     * char [0,1,2,3,4]
     * 5 - 1 = 4
     * 4 >= 4
    */
    if(i__ >= buff_size) while((tmp_c__ = et_read__()));
    return buff;
}
char * dy_reads__(){
    Dynamic_String dy_str = init_dynamic_string(10);
    if(!dy_str.s_string) return NULL;
    char tmp_c__ = ET_CHAR_NULL;
    while((tmp_c__ = et_read__())) if(!add_char_to_dynamic_string(&dy_str, tmp_c__)) break;
    return dy_str.s_string;
}
int static_puts_e(const char * str, ...){
    va_list va;
    va_start(va, str);
    int done = vfprintf(stderr, str, va);
    va_end(va);
    return done;
}

int static_puts(const char * str, ...){
    va_list va;
    va_start(va, str);
    int done = vfprintf(stdout, str, va);
    va_end(va);
    return done;
}