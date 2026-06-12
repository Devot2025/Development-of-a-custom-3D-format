#ifndef _SPEED_CHARM_IMAGE_H_
#define _SPEED_CHARM_IMAGE_H_
#include <stb_image.h>
#define sci_si_strap base_si_strap
typedef struct Speed_Charm_Image_Data{
    sc_size x;
    sc_size y;
    sc_ubyte * img_data;
}Speed_Charm_Image_Data;
_EXTERN_C_END_
#endif
