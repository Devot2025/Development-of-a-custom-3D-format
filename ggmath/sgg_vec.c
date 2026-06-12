#include "sgg_vec.h"
//#define _USE_MATH_DEFINES
//#include <math.h>
void sgg_trs_mat4(SGG_Mat4 * dst, const SGG_Quaternion * rot, const SGG_Vector3 * pos, const SGG_Vector3 * scale){

     SGG_Quaternion x_rot = {
        rot->x * rot->x,
        rot->x * rot->y,
        rot->x * rot->z,
        rot->x * rot->w
    };
    SGG_Vector3 y_rot = {
        rot->y * rot->y, 
        rot->y * rot->z,
        rot->y * rot->w
    };
    SGG_Vector2 z_rot = {
        rot->z * rot->z,
        rot->z * rot->w
    };
    SGG_Vector3 scales = {
        2.0f * scale->x,
        2.0f * scale->y,
        2.0f * scale->z
    };

    INSERT_COL_MAT4(dst, 0,
        scales.x * ( - y_rot.x - z_rot.x + 0.5f),
        scales.y * ( - z_rot.y + x_rot.y),
        scales.z * (   x_rot.z + y_rot.z),

        pos->x
    );
    INSERT_COL_MAT4(dst, 1,
        scales.x * (   x_rot.y + z_rot.y),
        scales.y * ( - x_rot.x - z_rot.x + 0.5f),
        scales.z * ( - x_rot.w + y_rot.y),

        pos->y
    );

    INSERT_COL_MAT4(dst, 2,
        scales.x * ( - y_rot.z + x_rot.z),
        scales.y * (   x_rot.w + y_rot.y),
        scales.z * ( - x_rot.x - y_rot.x + 0.5f),
        pos->z
    );

    INSERT_COL_MAT4(dst, 3, 0.0f, 0.0f, 0.0f, 1.0f);

}

void sgg_swap(float *a, float *b){
    float c = *a;
    *a = *b;
    *b = c;
}
bool sgg_equal_vector1(float a, float b){
    return fabs(a - b) < EPS;
}
bool sgg_equal_vector2(const SGG_Vector2 * a, const SGG_Vector2 * b){
    
    return sgg_equal_vector1(a->x, b->x) && sgg_equal_vector1(a->y, b->y);
}

bool sgg_equal_vector3(const SGG_Vector3 * a, const SGG_Vector3 * b){
    return sgg_equal_vector1(a->x, b->x) && sgg_equal_vector1(a->y, b->y) && sgg_equal_vector1(a->z, b->z);
}

bool sgg_equal_vector4(const SGG_Vector4 * a, const SGG_Vector4 * b){
    return sgg_equal_vector1(a->x, b->x) && sgg_equal_vector1(a->y, b->y) && sgg_equal_vector1(a->z, b->z) && sgg_equal_vector1(a->w, b->w);
}

bool sgg_equal_quaternion(const SGG_Quaternion * a, const SGG_Quaternion * b){
    return sgg_equal_vector1(a->x, b->x) && sgg_equal_vector1(a->y, b->y) && sgg_equal_vector1(a->z, b->z) && sgg_equal_vector1(a->w, b->w);
}

void sgg_neg_vector4(SGG_Vector4 * a){
    a->x = -a->x;
    a->y = -a->y;
    a->z = -a->z;
    a->w = -a->w;
}

void sgg_neg_quaternion(SGG_Quaternion * a){
    a->x = -a->x;
    a->y = -a->y;
    a->z = -a->z;
    a->w = -a->w;
}

void sgg_identity(SGG_Mat4* dst){
    for (int i = 0; i < 16; i++)
        dst->m[i] = 0.0f;

    dst->m[0]  = 1.0f;
    dst->m[5]  = 1.0f;
    dst->m[10] = 1.0f;
    dst->m[15] = 1.0f;
}

float length_sgg_vector2(const SGG_Vector2 * src_vec2){
    return sqrt(
        src_vec2->x * src_vec2->x + 
        src_vec2->y * src_vec2->y
    );
}

float length_sgg_vector3(const SGG_Vector3 * src_vec3){
    return sqrt(
        src_vec3->x * src_vec3->x + 
        src_vec3->y * src_vec3->y + 
        src_vec3->z * src_vec3->z
    );
}

float length_sgg_vector4(const SGG_Vector4 * src_vec4){
    return sqrt(
        src_vec4->x * src_vec4->x + 
        src_vec4->y * src_vec4->y + 
        src_vec4->z * src_vec4->z +
        src_vec4->w * src_vec4->w
    );
}

float length_sgg_quaternion(const SGG_Quaternion * src_quat){
    return sqrt(
        src_quat->x * src_quat->x + 
        src_quat->y * src_quat->y + 
        src_quat->z * src_quat->z +
        src_quat->w * src_quat->w
    );
}

void normalize_vector4(SGG_Vector4 * dst_vec4){
    float v_len = length_sgg_vector4(dst_vec4);
    if(v_len >= FLT_MIN){
        v_len = 1.0f / v_len;
        mul_vector4(dst_vec4, v_len);
    }
    else assignment_vector4(dst_vec4, 0.0f);
}
void normalize_quaternion(SGG_Quaternion * dst_quat){
    float v_len = length_sgg_quaternion(dst_quat);
    if(v_len >= FLT_MIN){
        v_len = 1.0f / v_len;
        mul_quaternion(dst_quat, v_len);
    }
    else assignment_quaternion(dst_quat, 0.0f);
}

void normalize_vector3(SGG_Vector3 * dst_vec3){
    float v_len = length_sgg_vector3(dst_vec3);
    if(v_len >= FLT_MIN){
        v_len = 1.0f / v_len;
        mul_vector3(dst_vec3, v_len);
    }
    else assignment_vector3(dst_vec3, 0.0f);
}

void assignment_vector2_vector2(SGG_Vector2 * vec2, const SGG_Vector2 * a_vec2){
    vec2->x = a_vec2->x;
    vec2->y = a_vec2->y;
}
void assignment_vector2_xyz(SGG_Vector2 * vec2, float ax, float ay){
    vec2->x = ax;
    vec2->y = ay;
}
void assignment_vector2(SGG_Vector2 * vec2, float ass_vec){
    vec2->x = vec2->y = ass_vec;
}

void add_vector2_vector2(SGG_Vector2 * vec2, const SGG_Vector2 * a_vec2){
    vec2->x += a_vec2->x;
    vec2->y += a_vec2->y;
}
void add_vector2_xyz(SGG_Vector2 * vec2, float ax, float ay){
    vec2->x += ax;
    vec2->y += ay;
}
void add_vector2(SGG_Vector2 * vec2, float add_vec){
    vec2->x += add_vec;
    vec2->y += add_vec;
}
void sub_vector2_vector2(SGG_Vector2 * vec2, const SGG_Vector2 * a_vec2){
    vec2->x -= a_vec2->x;
    vec2->y -= a_vec2->y;
}

void sub_vector2_xyz(SGG_Vector2 * vec2, float ax, float ay){
    vec2->x -= ax;
    vec2->y -= ay;
}

void sub_vector2(SGG_Vector2 * vec2, float sub_vec){
    vec2->x -= sub_vec;
    vec2->y -= sub_vec;
}

void mul_vector2_vector2(SGG_Vector2 * vec2, const SGG_Vector2 * a_vec2){
    vec2->x *= a_vec2->x;
    vec2->y *= a_vec2->y;
}

void mul_vector2_xyz(SGG_Vector2 * vec2, float ax, float ay){
    vec2->x *= ax;
    vec2->y *= ay;
}

void mul_vector2(SGG_Vector2 * vec2, float mul_vec){
    vec2->x *= mul_vec;
    vec2->y *= mul_vec;
}

void div_vector2_vector2(SGG_Vector2* vec2, const SGG_Vector2 * a_vec2){
    vec2->x /= a_vec2->x;
    vec2->y /= a_vec2->y;
}

void div_vector2_xyz(SGG_Vector2 * vec2, float ax, float ay){
    vec2->x /= ax;
    vec2->y /= ay;
}

void div_vector2(SGG_Vector2 * vec2, float div_vec){
    vec2->x /= div_vec;
    vec2->y /= div_vec;
}

void assignment_vector3_vector3(SGG_Vector3 * vec3, const SGG_Vector3 * a_vec3){
    vec3->x = a_vec3->x;
    vec3->y = a_vec3->y;
    vec3->z = a_vec3->z;
}
void assignment_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az){
    vec3->x = ax;
    vec3->y = ay;
    vec3->z = az;
}
void assignment_vector3(SGG_Vector3 * vec3, float ass_vec){
    vec3->x = vec3->y = vec3->z = ass_vec;
}

void add_vector3_vector3(SGG_Vector3 * vec3, const SGG_Vector3 * a_vec3){
    vec3->x += a_vec3->x;
    vec3->y += a_vec3->y;
    vec3->z += a_vec3->z;
}
void add_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az){
    vec3->x += ax;
    vec3->y += ay;
    vec3->z += az;
}
void add_vector3(SGG_Vector3 * vec3, float add_vec){
    vec3->x += add_vec;
    vec3->y += add_vec;
    vec3->z += add_vec;
}
void sub_vector3_vector3(SGG_Vector3 * vec3, const SGG_Vector3 * a_vec3){
    vec3->x -= a_vec3->x;
    vec3->y -= a_vec3->y;
    vec3->z -= a_vec3->z;
}

void sub_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az){
    vec3->x -= ax;
    vec3->y -= ay;
    vec3->z -= az;
}

void sub_vector3(SGG_Vector3 * vec3, float sub_vec){
    vec3->x -= sub_vec;
    vec3->y -= sub_vec;
    vec3->z -= sub_vec;
}

void mul_vector3_vector3(SGG_Vector3 * vec3, const SGG_Vector3 * a_vec3){
    vec3->x *= a_vec3->x;
    vec3->y *= a_vec3->y;
    vec3->z *= a_vec3->z;
}

void mul_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az){
    vec3->x *= ax;
    vec3->y *= ay;
    vec3->z *= az;
}

void mul_vector3(SGG_Vector3 * vec3, float mul_vec){
    vec3->x *= mul_vec;
    vec3->y *= mul_vec;
    vec3->z *= mul_vec;
}

void div_vector3_vector3(SGG_Vector3 * vec3, const SGG_Vector3 * a_vec3){
    vec3->x /= a_vec3->x;
    vec3->y /= a_vec3->y;
    vec3->z /= a_vec3->z;
}

void div_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az){
    vec3->x /= ax;
    vec3->y /= ay;
    vec3->z /= az;
}

void div_vector3(SGG_Vector3 * vec3, float div_vec){
    vec3->x /= div_vec;
    vec3->y /= div_vec;
    vec3->z /= div_vec;
}

SGG_Quaternion mul_quaternion_quaternion(const SGG_Quaternion * quat, const SGG_Quaternion * a_quat){
    SGG_Quaternion r;
    r.w = quat->w * a_quat->w - quat->x * a_quat->x - quat->y * a_quat->y - quat->z * a_quat->z;
    r.x = quat->w * a_quat->x + quat->x * a_quat->w + quat->y * a_quat->z - quat->z * a_quat->y;
    r.y = quat->w * a_quat->y - quat->x * a_quat->z + quat->y * a_quat->w + quat->z * a_quat->x;
    r.z = quat->w * a_quat->z + quat->x * a_quat->y - quat->y * a_quat->x + quat->z * a_quat->w;
    return r;
}

void mul_quaternion(SGG_Quaternion * quat, float ass_vec){
    quat->x *= ass_vec;
    quat->y *= ass_vec;
    quat->z *= ass_vec;
    quat->w *= ass_vec;
}
void mul_vector4(SGG_Vector4 * vec4, float mul_vec){
    vec4->x *= mul_vec;
    vec4->y *= mul_vec;
    vec4->z *= mul_vec;
    vec4->w *= mul_vec;
}

void assignment_vector4(SGG_Vector4 * vec4, float ass_vec){
    vec4->x = ass_vec;
    vec4->y = ass_vec;
    vec4->z = ass_vec;
    vec4->w = ass_vec;
}

void sub_vector4_vector4(SGG_Vector4* vec4, const SGG_Vector4* a_vec4) {

    vec4->x -= a_vec4->x;
    vec4->y -= a_vec4->y;
    vec4->z -= a_vec4->z;
    vec4->w -= a_vec4->w;
}
void add_vector4_vector4(SGG_Vector4* vec4, const SGG_Vector4* a_vec4) {

    vec4->x += a_vec4->x;
    vec4->y += a_vec4->y;
    vec4->z += a_vec4->z;
    vec4->w += a_vec4->w;
}
void div_vector4_vector4(SGG_Vector4* vec4, const SGG_Vector4* a_vec4) {
    vec4->x /= a_vec4->x;
    vec4->y /= a_vec4->y;
    vec4->z /= a_vec4->z;
    vec4->w /= a_vec4->w;
}

void div_vector4_xyz(SGG_Vector4* vec4, float ax, float ay, float az, float aw) {
    vec4->x /= ax;
    vec4->y /= ay;
    vec4->z /= az;
    vec4->w /= aw;
}

void div_vector4(SGG_Vector4* vec4, float div_vec) {
    vec4->x /= div_vec;
    vec4->y /= div_vec;
    vec4->z /= div_vec;
    vec4->w /= div_vec;
}

void assignment_mat4(SGG_Mat4 * dst, float x){
    for(size_t i = 0; i < 16; ++i)dst->m[i] = x;
}
void mul_mat4_vector4(SGG_Vector4 *dst, const SGG_Mat4 *a, const SGG_Vector4 *b) {
    dst->x = a->m[0]  * b->x + a->m[4]  * b->y + a->m[8]  * b->z + a->m[12] * b->w;
    dst->y = a->m[1]  * b->x + a->m[5]  * b->y + a->m[9]  * b->z + a->m[13] * b->w;
    dst->z = a->m[2]  * b->x + a->m[6]  * b->y + a->m[10] * b->z + a->m[14] * b->w;
    dst->w = a->m[3]  * b->x + a->m[7]  * b->y + a->m[11] * b->z + a->m[15] * b->w;
}
void mul_mat4_mat4(SGG_Mat4 *dst, const SGG_Mat4 *a, const SGG_Mat4 *b) {

#if defined(SGG_MULTI_MAT)

    MT_128_T mt = { GEN_MT_128_T(*a) };
    for (int col = 0; col < 4; ++col) {
        mf128t rt = mul_mft128(mt.a0, set_mat4_broad1_mtf(*b, col, 0));
        rt = add_mft128(rt, mul_mft128(mt.a1, set_mat4_broad1_mtf(*b, col, 1)));
        rt = add_mft128(rt, mul_mft128(mt.a2, set_mat4_broad1_mtf(*b, col, 2)));
        rt = add_mft128(rt, mul_mft128(mt.a3, set_mat4_broad1_mtf(*b, col, 3)));
        mov_mft128_to_mat4(*dst, col, rt);
    }
#elif defined(SC_VEC_DEBUG)


    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            dst->m[col * 4 + row] =
                a->m[0 * 4 + row] * b->m[col * 4 + 0] +
                a->m[1 * 4 + row] * b->m[col * 4 + 1] +
                a->m[2 * 4 + row] * b->m[col * 4 + 2] +
                a->m[3 * 4 + row] * b->m[col * 4 + 3];
        }
    }

#else
    /**
    * a00 a01 a02 a03 | b00 b01 b02 b03
    * a10 a11 a12 a13 | b10 b11 b12 b13
    * a20 a21 a22 a23 | b20 b21 b22 b23
    * a30 a31 a32 a33 | b30 b31 b32 b33
    * a00*b00 + a10*b01 + a20*b02 + a30*b03
    * a01*b00 + a11*b01 + a20*b02 + a30*b03
    **/
    dst->m[0] = a->m[0] * b->m[0] + a->m[4] * b->m[1] + a->m[8] * b->m[2] + a->m[12] * b->m[3];
    dst->m[1] = a->m[1] * b->m[0] + a->m[5] * b->m[1] + a->m[9] * b->m[2] + a->m[13] * b->m[3];
    dst->m[2] = a->m[2] * b->m[0] + a->m[6] * b->m[1] + a->m[10] * b->m[2] + a->m[14] * b->m[3];
    dst->m[3] = a->m[3] * b->m[0] + a->m[7] * b->m[1] + a->m[11] * b->m[2] + a->m[15] * b->m[3];

    dst->m[4] = a->m[0] * b->m[4] + a->m[4] * b->m[5] + a->m[8] * b->m[6] + a->m[12] * b->m[7];
    dst->m[5] = a->m[1] * b->m[4] + a->m[5] * b->m[5] + a->m[9] * b->m[6] + a->m[13] * b->m[7];
    dst->m[6] = a->m[2] * b->m[4] + a->m[6] * b->m[5] + a->m[10] * b->m[6] + a->m[14] * b->m[7];
    dst->m[7] = a->m[3] * b->m[4] + a->m[7] * b->m[5] + a->m[11] * b->m[6] + a->m[15] * b->m[7];

    dst->m[8] = a->m[0] * b->m[8] + a->m[4] * b->m[9] + a->m[8] * b->m[10] + a->m[12] * b->m[11];
    dst->m[9] = a->m[1] * b->m[8] + a->m[5] * b->m[9] + a->m[9] * b->m[10] + a->m[13] * b->m[11];
    dst->m[10] = a->m[2] * b->m[8] + a->m[6] * b->m[9] + a->m[10] * b->m[10] + a->m[14] * b->m[11];
    dst->m[11] = a->m[3] * b->m[8] + a->m[7] * b->m[9] + a->m[11] * b->m[10] + a->m[15] * b->m[11];


    dst->m[12] = a->m[0] * b->m[12] + a->m[4] * b->m[13] + a->m[8] * b->m[14] + a->m[12] * b->m[15];
    dst->m[13] = a->m[1] * b->m[12] + a->m[5] * b->m[13] + a->m[9] * b->m[14] + a->m[13] * b->m[15];
    dst->m[14] = a->m[2] * b->m[12] + a->m[6] * b->m[13] + a->m[10] * b->m[14] + a->m[14] * b->m[15];
    dst->m[15] = a->m[3] * b->m[12] + a->m[7] * b->m[13] + a->m[11] * b->m[14] + a->m[15] * b->m[15];

#endif // !SC_VEC_DEBUG

}

void assignment_quaternion(SGG_Quaternion * quat, float ass_vec){
    quat->x = ass_vec;
    quat->y = ass_vec;
    quat->z = ass_vec;
    quat->w = ass_vec;
}

void cross_sgg_vector3(SGG_Vector3 * vec3, const SGG_Vector3 * src_a, const SGG_Vector3 * src_b){
    vec3->x = src_a->y * src_b->z - src_a->z * src_b->y;
    vec3->y = src_a->z * src_b->x - src_a->x * src_b->z;
    vec3->z = src_a->x * src_b->y - src_a->y * src_b->x;
}

void sgg_perspective(SGG_Mat4 * dst_per, float vertical_rad, float aspect, float near, float far){

    //verticalRad = verticalRad * M_PI / 180.0f;
    float scale_coeff = 1.0f / tanf(vertical_rad / 2.0f);
    dst_per->m[0] = scale_coeff / aspect;
    for(unsigned int i = 1;i<10;++i)dst_per->m[i] = 0.0f;
    dst_per->m[5] = scale_coeff;
    dst_per->m[10] = (far + near)/(near - far);
    dst_per->m[11] = -1.0f;
    dst_per->m[12] = dst_per->m[13] = 0.0f;
    dst_per->m[14] = (2.0f * far * near) / (near - far);
    dst_per->m[15] = 0.0f;
}
void sgg_lookAt(SGG_Mat4 * dst_mat4, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ){
    SGG_Vector3 for_word = {centerX - eyeX, centerY - eyeY, centerZ - eyeZ};
    float for_word_normal = sqrtf(for_word.x * for_word.x + for_word.y * for_word.y + for_word.z * for_word.z);
    
    div_vector3(&for_word, for_word_normal);
    SGG_Vector3 side = {for_word.y * upZ - for_word.z * upY,for_word.z * upX - for_word.x * upZ,for_word.x * upY - for_word.y * upX};
    float side_normal = 1.0f/ sqrtf(side.x * side.x + side.y * side.y + side.z * side.z);
    mul_vector3(&side, side_normal);
    SGG_Vector3 up;
    cross_sgg_vector3(&up, &side, &for_word);
    
    /*rotation matrix*/
    dst_mat4->m[0] = side.x;   dst_mat4->m[1] = up.x;   dst_mat4->m[2]  = -for_word.x;   dst_mat4->m[3]  = 0.0f;
    dst_mat4->m[4] = side.y;   dst_mat4->m[5] = up.y;   dst_mat4->m[6]  = -for_word.y;   dst_mat4->m[7]  = 0.0f;
    dst_mat4->m[8] = side.z;   dst_mat4->m[9] = up.z;   dst_mat4->m[10] = -for_word.z;   dst_mat4->m[11] = 0.0f;
    
    /*move Matrix*/
    dst_mat4->m[12] = -(side.x * eyeX + side.y * eyeY + side.z * eyeZ);
    dst_mat4->m[13] = -(up.x * eyeX + up.y * eyeY + up.z * eyeZ);
    dst_mat4->m[14] = (for_word.x * eyeX + for_word.y * eyeY + for_word.z * eyeZ);
    dst_mat4->m[15] = 1.0f;
}

void sgg_ortho(SGG_Mat4 * dst_mat, float left, float right, float bottom, float top, float near_z, float far_z){
    
    const float world_width  = right  - left;
    const float world_height = top    - bottom;
    const float world_depth  = far_z   - near_z;
    dst_mat->m[0] = 2 / world_width;/* 2 / left_Right => opengl world |+1~-1| = 2. the result is vertex fix to 2.scale(mult) vertex x pos.*/
    dst_mat->m[1] = dst_mat->m[2] = dst_mat->m[3] = 0.0f;
    dst_mat->m[4] = dst_mat->m[6] = dst_mat->m[7] = 0.0f;
    dst_mat->m[5] = 2 / world_height;
    dst_mat->m[8] = dst_mat->m[9] = dst_mat->m[11] = 0.0f;
    dst_mat->m[10] = -2 / world_depth;
    dst_mat->m[12] = -(right + left) / world_width;
    dst_mat->m[13] = -(top + bottom) / world_height;
    dst_mat->m[14] = -(near_z + far_z) / world_depth;
    dst_mat->m[15] = 1.0f;
}
void sgg_inverse_quaternion(SGG_Quaternion * dst_quat){
    dst_quat->x = -dst_quat->x;
    dst_quat->y = -dst_quat->y;
    dst_quat->z = -dst_quat->z;
}

void sgg_inverse(SGG_Mat4 * dst_mat4){
    float a[4][8];
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            a[row][col] = dst_mat4->m[col * 4 + row];
        }
        for (int col = 0; col < 4; ++col) {
            a[row][4 + col] = (row == col) ? 1.0f : 0.0f;
        }
    }
    for (int i = 0; i < 4; ++i) {
        int pivot_row = i;
        float max_abs = fabs(a[i][i]);
        for (int r = i + 1; r < 4; ++r) {
            float v = fabs(a[r][i]);
            if (v > max_abs) { max_abs = v; pivot_row = r; }
        }
        if (max_abs < EPS) return;

        if (pivot_row != i)
            for (int c = 0; c < 8; ++c) sgg_swap(&a[i][c], &a[pivot_row][c]);

        float pivot = a[i][i];
        for (int c = 0; c < 8; ++c) a[i][c] /= pivot;

        for (int r = 0; r < 4; ++r) {
            if (r == i) continue;
            float f = a[r][i];
            for (int c = 0; c < 8; ++c) a[r][c] -= f * a[i][c];
        }
    }
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col)
            dst_mat4->m[col * 4 + row] = a[row][4 + col];
    
}


void sgg_quads_matrix(float * q_mat, float left, float right, float bottom, float top){
    q_mat[0] = left;
    q_mat[1] = top;
    q_mat[2] = q_mat[6] = left;
    q_mat[3] = q_mat[7] = bottom;
    q_mat[4] = q_mat[10] = right;
    q_mat[5] = q_mat[11] = top;
    q_mat[8] = right;
    q_mat[9] = bottom;
}
void sgg_quads_strip_tex_coords_pos_matrix(float* q_mat, float left, float right, float bottom, float top) {
    q_mat[0] = left; q_mat[1] = top; q_mat[2] = 0.0f; q_mat[3] = 0.0f; q_mat[4] = 1.0f;
    q_mat[5] = left; q_mat[6] = bottom; q_mat[7] = 0.0f; q_mat[8] = 0.0f; q_mat[9] = 0.0f;
    q_mat[10] = right; q_mat[11] = top; q_mat[12] = 0.0f; q_mat[13] = 1.0f; q_mat[14] = 1.0f;
    q_mat[15] = right; q_mat[16] = bottom; q_mat[17] = 0.0f; q_mat[18] = 1.0f; q_mat[19] = 0.0f;
}

void sgg_cube_matrix(float * c_mat, float left, float right, float bottom, float top, float near, float far, int next_pos){
    
    set_f_array_to_vec3(&c_mat, left, top, near, next_pos);
    set_f_array_to_vec3(&c_mat, left, bottom,  near, next_pos);
    set_f_array_to_vec3(&c_mat, right, top,  near, next_pos);

    set_f_array_to_vec3(&c_mat, right, top, near, next_pos);
    set_f_array_to_vec3(&c_mat, left, bottom,  near, next_pos);
    set_f_array_to_vec3(&c_mat, right, bottom,  near, next_pos);

    set_f_array_to_vec3(&c_mat, left,  bottom, far, next_pos);
    set_f_array_to_vec3(&c_mat, left,  top,    far, next_pos);
    set_f_array_to_vec3(&c_mat, right, top,    far, next_pos);

    set_f_array_to_vec3(&c_mat, right, top,    far, next_pos);
    set_f_array_to_vec3(&c_mat, right, bottom, far, next_pos);
    set_f_array_to_vec3(&c_mat, left,  bottom, far, next_pos);

    set_f_array_to_vec3(&c_mat, left,  bottom, far, next_pos);
    set_f_array_to_vec3(&c_mat, left,  bottom, near, next_pos);
    set_f_array_to_vec3(&c_mat, left,  top,    near, next_pos);

    set_f_array_to_vec3(&c_mat, left,  top,    near, next_pos);
    set_f_array_to_vec3(&c_mat, left,  top,    far, next_pos);
    set_f_array_to_vec3(&c_mat, left,  bottom, far, next_pos);

    set_f_array_to_vec3(&c_mat, right, bottom, near, next_pos);
    set_f_array_to_vec3(&c_mat, right, bottom, far, next_pos);
    set_f_array_to_vec3(&c_mat, right, top,    far, next_pos);

    set_f_array_to_vec3(&c_mat, right, top,    far, next_pos);
    set_f_array_to_vec3(&c_mat, right, top,    near, next_pos);
    set_f_array_to_vec3(&c_mat, right, bottom, near, next_pos);

    set_f_array_to_vec3(&c_mat, left,  top,    near, next_pos);
    set_f_array_to_vec3(&c_mat, right, top,    near, next_pos);
    set_f_array_to_vec3(&c_mat, right, top,    far, next_pos);

    set_f_array_to_vec3(&c_mat, right, top,    far, next_pos);
    set_f_array_to_vec3(&c_mat, left,  top,    far, next_pos);
    set_f_array_to_vec3(&c_mat, left,  top,    near, next_pos);

    set_f_array_to_vec3(&c_mat, left,  bottom, near, next_pos);
    set_f_array_to_vec3(&c_mat, left,  bottom, far, next_pos);
    set_f_array_to_vec3(&c_mat, right, bottom, near, next_pos);

    set_f_array_to_vec3(&c_mat, right, bottom, near, next_pos);
    set_f_array_to_vec3(&c_mat, left,  bottom, far, next_pos);
    set_f_array_to_vec3(&c_mat, right, bottom, far, next_pos);
}

void sgg_sphere_matrix(float * sph_mat, float stack_count, float sector_count, float radius, uint32_t attr_num){
    SGG_Vector3 pos;

    float length_inv = 1.0f / radius;
    float sector_step = 2.0f * M_PI / sector_count;
    float stack_step = M_PI / stack_count;
    uint32_t stride = attr_num == 1 ? 6 : 3;
    for(uint32_t i = 0, index = 0; i <= stack_count; ++i){
        float stack_angle = M_PI / 2.0f - i * stack_step;
        float xy = radius * cos(stack_angle);
        pos.z = radius * sin(stack_angle);    
        for(uint32_t j = 0; j <= sector_count; ++j){
            float sector_angle = j * sector_step;
            pos.x = xy * cos(sector_angle);
            pos.y = xy * sin(sector_angle);
            SGG_Vector3 a_pos = pos;
            *(sph_mat + index) = a_pos.x;
            *(sph_mat + index + 1) = a_pos.y;
            *(sph_mat + index + 2) = a_pos.z;
            if(attr_num == 1){
                SGG_Vector3 norm;
                assignment_vector3_vector3(&norm, &pos);
                mul_vector3(&norm, length_inv);
                *(sph_mat + index + 3) = norm.x;
                *(sph_mat + index + 4) = norm.y;
                *(sph_mat + index + 5) = norm.z;
            }
            index += stride;
        }
    }
}

bool look_rotation_vec3_quat(SGG_Quaternion * rot_quat, const SGG_Vector3 * a_pos, const SGG_Vector3 * b_pos, SGG_Vector3 * a_dir){
    SGG_Vector3 ba_;
    assignment_vector3_vector3(&ba_, b_pos);
    sub_vector3_vector3(&ba_, a_pos);
    normalize_vector3(&ba_);
    normalize_vector3(a_dir);
    float ba_dot = sgg_dot_vector3(a_dir, &ba_);
    ba_dot = sgg_clampf(ba_dot, -1.0f, 1.0f);
    
    float r = acosf(ba_dot);
    SGG_Vector3 axis;
    cross_sgg_vector3(&axis, a_dir, &ba_);
    //normalize_vector3(&axis);

    if(length_sgg_vector3(&axis) < EPS) {
        if(ba_dot > 0.0f) return false;
        SGG_Vector3 b_axis = {1.0f, 0.0f, 0.0f};
        if(sgg_absf(a_dir->x) > 0.9f) b_axis = (SGG_Vector3){0.0f, 1.0f, 0.0f};
        cross_sgg_vector3(&axis, a_dir, &b_axis);
        *rot_quat = quat_axis_angle_vec3_rad(M_PI, axis);
        
        return true;
    }
    *rot_quat = quat_axis_angle_vec3_rad(r, axis);
    return true;
}
/**
 * bool look_rotation_vec3_quat(SGG_Quaternion * rot_quat, SGG_Vector3 * a_pos, SGG_Vector3 * b_pos, SGG_Vector3 * a_dir){
    SGG_Vector3 ba_;
    assignment_vector3_vector3(&ba_, b_pos);
    sub_vector3_vector3(&ba_, a_pos);
    normalize_vector3(&ba_);
    normalize_vector3(a_dir);
    float r = acosf(sgg_dot_vector3(a_dir, &ba_));
    SGG_Vector3 axis;
    cross_sgg_vector3(&axis, a_dir, &ba_);
    if(length_sgg_vector3(&axis) < EPS) return false;
    *rot_quat = quat_axis_angle_vec3_rad(r, axis);
    return true;
}
 * 
*/
SGG_Vector3 look_rotation_vec3(SGG_Vector3 a_pos, SGG_Vector3 b_pos, SGG_Vector3 a_dir){
    SGG_Quaternion rot_quat;
    if(!look_rotation_vec3_quat(&rot_quat, &a_pos, &b_pos, &a_dir)) return a_dir;
    normalize_quaternion(&rot_quat);
    return rotation_vec3(rot_quat, a_dir);
}
