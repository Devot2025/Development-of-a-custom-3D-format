#ifndef _EXT_DEBUGGER_H_
#define _EXT_DEBUGGER_H_
#include <stdio.h>
#include <macro_inf.hint>
#include <c_header_helper.hint>
typedef struct Ext_Debugger_Package{
    size_t db_lines__;
    long db_code__;
    const char * db_file_name__;
    const char * db_func_name__;
    const char * db_usr_str__;
}Ext_Debugger_Package;
#define set_debug_information(out_code, out_str) (Ext_Debugger_Package){.db_lines__ = __LINE__, .db_code__ = out_code, .db_file_name__ = __FILE__, .db_func_name__ = __func__, .db_usr_str__ = out_str}
#define ext_dp_si_strap base_si_strap
_EXTERN_C_BEGIN_

ext_dp_si_strap int dp_puts_ln(Ext_Debugger_Package * edp, long out_base){
    /*error code.*/
    if(edp->db_code__ < out_base){
        printf("[ERROR]\n");
        tmp_printf_e("* file name : %s\n", edp->db_file_name__);
        tmp_printf_e("* lines     : %lu\n", edp->db_lines__);
        tmp_printf_e("* func name : %s\n", edp->db_func_name__);
        tmp_printf_e("* out code  : %ld ", edp->db_code__);
        tmp_printf_e("* log str   : %s\n", edp->db_usr_str__);
    }
    else{
        printf("[OUT]\n");
        printf("* file name : %s\n", edp->db_file_name__);
        printf("* lines     : %lu \n", edp->db_lines__);
        printf("* func name : %s\n", edp->db_func_name__);
        printf("* out code  : %ld\n", edp->db_code__);
        printf("* log str   : %s\n", edp->db_usr_str__);

    }
    putchar('\n');
    return 0;
}
_EXTERN_C_END_
#endif// _EXT_DEBUGGER_H_
