#ifndef _STB_LINK_HELPER_H_
#define _STB_LINK_HELPER_H_
/**
 * @brief Include stb_image.h (using the full include path) and declare it in the header file.
 *
 * This source file must be added to the executable target using add_executable in CMake.
 */

#include "c_header_helper.hint"
_EXTERN_C_BEGIN_
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

_EXTERN_C_END_
#endif
