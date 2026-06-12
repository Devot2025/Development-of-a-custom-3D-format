
#ifndef _EXT_CIO_H_
#define _EXT_CIO_H_
#include "ext_alloc.h"
#include "ext_c.hint"
#include "ext_dy_str.h"
#include <c_header_helper.hint>
#define ext_cio_si_strap static inline
_EXTERN_C_BEGIN_
ext_cio_si_strap char c_read__(){return(char)getc(stdin);}
char et_read__();
char * et_reads__(char * buff, size_t buff_size);
char * dy_reads__();
ext_cio_si_strap char * static_gets(char * buff, size_t buff_size){ return et_reads__(buff, buff_size);}
ext_cio_si_strap char * static_print_gets(const char * print_buff, char * buff, size_t buff_size){ if(print_buff)printf("%s", print_buff); return static_gets(buff, buff_size);}
ext_cio_si_strap char * dynamic_gets(){return dy_reads__();}
int static_puts_e(const char * str, ...);
int static_puts(const char * str, ...);



_EXTERN_C_END_
#endif //_EXT_CIO_H_