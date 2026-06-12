#ifndef _EXT_RAND_H_
#define _EXT_RAND_H_
#include <stdint.h>
#include "macro_inf.hint"
#include "c_header_helper.hint"
#define ext_rand_si_strap static inline
#define xo_shiro_256_si_strap ext_rand_si_strap
#define pcg_32_si_strap ext_rand_si_strap
#define PCG_32_MULTIPLIER 6364136223846793005u
#define FIX_RAND_UINT64_TO_TYPE(type, size, x) (type)x * (1.0 / (type)SHIFT_POW_2(size))
#define FIX_RAND_UINT32_TO_TYPE(type, x) (type)x / (UINT32_MAX + 1.0)
#define XO_SHIRO_SIZE 4
#define XO_SHIRO_64_BIT_SEED_SIZE 64

typedef struct Xo_Shiro_256{uint64_t s[XO_SHIRO_SIZE];}Xo_Shiro_256;

typedef struct Pcg_32{uint64_t state; uint64_t increment;}Pcg_32;

_EXTERN_C_BEGIN_

/**
 * xoshiro and pcg32 source code url --->
 * https://prng.di.unimi.it/
*/
ext_rand_si_strap double fix_rand_uint64_t_to_double(uint64_t x){
    x >>= 11;//64 - 53
    return FIX_RAND_UINT64_TO_TYPE(double, 53, x);
}

ext_rand_si_strap float fix_rand_uint64_t_to_float(uint64_t x){
    uint32_t s_x = x >> 40;//64 - 24
    return FIX_RAND_UINT64_TO_TYPE(float, 24, s_x);
}

ext_rand_si_strap double fix_rand_range_lf(double x, double min, double max){
    return x * (max - min) + min;
}

ext_rand_si_strap int32_t fix_rand_range_int32_t(int32_t x, int32_t min, int32_t max){
    int32_t diff = max - min + 1;
    return min + (x) % diff;
}
ext_rand_si_strap uint64_t fix_rand_range_uin64_t(uint64_t x, uint64_t min, uint64_t max){
    uint64_t diff = max - min + 1;
    return min + (x) % diff;
}

ext_rand_si_strap uint32_t fix_rand_range_uin32_t(uint32_t x, uint32_t min, uint32_t max){
    uint32_t diff = max - min + 1;
    return min + (x) % diff;
}
ext_rand_si_strap float fix_rand_range_f(float x, float min, float max){
    return x * (max - min) + min;
}

/*Begin, define to the split_min function*/
uint64_t split_mix_64(uint64_t * state);
/*End*/



xo_shiro_256_si_strap uint64_t xos_256_rotl(const uint64_t x, int k){
    return (x << k) | (x >> (XO_SHIRO_64_BIT_SEED_SIZE - k));
}

uint64_t xos_256_next(Xo_Shiro_256 * xos_256);

void xos_256_init(Xo_Shiro_256 * xos_256, const uint64_t state);

xo_shiro_256_si_strap double get_xos_256_lf(Xo_Shiro_256 * xos_256){
    /*64 - 53*/
    return fix_rand_uint64_t_to_double(xos_256_next(xos_256));
}

xo_shiro_256_si_strap float get_xos_256_f(Xo_Shiro_256 * xos_256){
    return fix_rand_uint64_t_to_float(xos_256_next(xos_256));
}
xo_shiro_256_si_strap double get_xos_256_range_lf(Xo_Shiro_256 * xos_256, double min, double max){
    return fix_rand_range_lf(get_xos_256_lf(xos_256), min, max);
}

xo_shiro_256_si_strap float get_xos_256_range_f(Xo_Shiro_256 * xos_256, float min, float max){
    return fix_rand_range_f(get_xos_256_f(xos_256), min, max);
}

pcg_32_si_strap uint32_t pcg_32_rotr(uint32_t k, unsigned r){
    return k >> r | k << (-r & 31);
}
uint32_t pcg_32_next(Pcg_32 * pcg_32);
void pcg_32_init(Pcg_32 * pcg_32, uint64_t init_state, uint64_t init_seq);

pcg_32_si_strap uint64_t get_pcg_64_uint64_t(Pcg_32 * pcg_32){
    uint32_t under_byte = pcg_32_next(pcg_32);
    uint32_t top_byte = pcg_32_next(pcg_32);
    return ((uint64_t)top_byte << 32) | under_byte;
}
pcg_32_si_strap double get_pcg_32_lf(Pcg_32 * pcg_32){
    return fix_rand_uint64_t_to_double(get_pcg_64_uint64_t(pcg_32));
}
pcg_32_si_strap float get_pcg_32_f(Pcg_32 * pcg_32){
    uint32_t x = pcg_32_next(pcg_32);
    return FIX_RAND_UINT32_TO_TYPE(float, x);
}

pcg_32_si_strap double get_pcg_32_range_lf(Pcg_32 * pcg_32, double min, double max){
    return fix_rand_range_lf(get_pcg_32_lf(pcg_32), min, max);
}

pcg_32_si_strap float get_pcg_32_range_f(Pcg_32 * pcg_32, float min, float max){
    return fix_rand_range_f(get_pcg_32_f(pcg_32), min, max);
}
_EXTERN_C_END_
#endif // _EXT_RAND_H_
