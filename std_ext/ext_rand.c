#include "ext_rand.h"

uint64_t split_mix_64(uint64_t *state){
    uint64_t z = ((*state) += 0x9e3779b97f4a7c15ull);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
    return z ^ (z >> 31);
}
uint64_t xos_256_next(Xo_Shiro_256 * xos_256) {
	const uint64_t rotl_result = xos_256_rotl(xos_256->s[0] + xos_256->s[3], 23) + xos_256->s[0];
	const uint64_t t = xos_256->s[1] << 17;

	xos_256->s[2] ^= xos_256->s[0];
	xos_256->s[3] ^= xos_256->s[1];
	xos_256->s[1] ^= xos_256->s[2];
	xos_256->s[0] ^= xos_256->s[3];

	xos_256->s[2] ^= t;

	xos_256->s[3] = xos_256_rotl(xos_256->s[3], 45);
	return rotl_result;
}

void xos_256_init(Xo_Shiro_256 * xos_256, const uint64_t state){
    uint64_t x_state = state;
    for(int i = 0; i < 4; ++i) xos_256->s[i] = split_mix_64(&x_state);
    if(xos_256->s[0] == 0 && xos_256->s[1] == 0 && xos_256->s[2] == 0 && xos_256->s[3] == 0) xos_256->s[0] = 1;
}

uint32_t pcg_32_next(Pcg_32 * pcg_32){
    uint64_t x = pcg_32->state;
    unsigned count = (unsigned)(x >> 59);
    pcg_32->state = x * PCG_32_MULTIPLIER + pcg_32->increment;
    x ^= x >> 18;
    return pcg_32_rotr((uint32_t)(x >> 27), count);
}

void pcg_32_init(Pcg_32 * pcg_32, uint64_t init_state, uint64_t init_seq){
    
    pcg_32->increment = (init_seq << 1u) | 1u;
    pcg_32->state = 0;
    (void)pcg_32_next(pcg_32);
    pcg_32->state += init_state;
    (void)pcg_32_next(pcg_32);
}
