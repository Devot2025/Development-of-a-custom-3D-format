#ifndef _EXT_OS_STRS_H_
#define _EXT_OS_STRS_H_
#define ext_os_strs_si_strap static inline

ext_os_strs_si_strap void set_os_string(char * strs){
    #if defined(_WIN64)
    strs[0] = 'W';
    strs[1] = 'I';
    strs[2] = 'N';
    strs[3] = '6';
    strs[4] = '4';
    #elif defined(_WIN32)
    strs[0] = 'W';
    strs[1] = 'I';
    strs[2] = 'N';
    strs[3] = '3';
    strs[4] = '2';
    #elif defined(__linux__)
    strs[0] = 'L';
    strs[1] = 'I';
    strs[2] = 'N';
    strs[3] = 'U';
    strs[4] = 'X';
    #else
    strs[0] = '#';
    strs[1] = '#';
    strs[2] = '#';
    strs[3] = 'O';
    strs[4] = 'S';
    #endif
}
#endif //_EXT_OS_STRS_H_
