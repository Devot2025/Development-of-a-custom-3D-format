#ifndef _SGG_VEC_H_
#define _SGG_VEC_H_
#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "c_header_helper.hint"
#if defined(_WIN64) || defined(_LINUX)
#define SGG_MULTI_MAT

#include <xmmintrin.h>
#define mf128t __m128
typedef struct MF_128_T {
    mf128t a0;
    mf128t a1;
    mf128t a2;
    mf128t a3;
}MT_128_T;
#define set_mat4_broad1_mtf(smat4_, offset, idx) _mm_set1_ps((smat4_).m[offset * 4 + idx])
#define mul_mft128(a, b) _mm_mul_ps(a, b)
#define add_mft128(a, b) _mm_add_ps(a, b)
#define mov_mft128_to_mat4(smat4_, offset, a) _mm_storeu_ps(&(smat4_).m[offset * 4], a);
#define GEN_MT_128_T(smat4_) \
_mm_loadu_ps(&(smat4_).m[0]), _mm_loadu_ps(&(smat4_).m[4]), _mm_loadu_ps(&(smat4_).m[8]), _mm_loadu_ps(&(smat4_).m[12])
#else
#endif // SGG_MULTI_MAT

#define sgg_vec_si_strap static inline

#define EPS 1e-8f

#define phi_degree 180.0
#define phi2_degree 360.0

#define CONVERT_GL_NDC_WIDTH(xx_, wwidth_) ((2.0 * xx_) / wwidth_ - 1.0f)
#define CONVERT_GL_NDC_HEIGHT(yy_, hheight_) (1.0f - (2.0 * yy_) / hheight_)
#define INSERT_ROW_MAT4(mat4, idx, m1, m2, m3, m4)\
     mat4->m[idx * 4 + 0] = m1,\
     mat4->m[idx * 4 + 1] = m2, \
     mat4->m[idx * 4 + 2] = m3, \
     mat4->m[idx * 4 + 3] = m4 
#define INSERT_COL_MAT4(mat4, idx, m1, m2, m3, m4)\
    mat4->m[idx + 0 * 4] = m1,\
    mat4->m[idx + 1 * 4] = m2,\
    mat4->m[idx + 2 * 4] = m3,\
    mat4->m[idx + 3 * 4] = m4
#define set_x_up(vec) assignment_vector3_xyz(&vec, 1.0f, 0.0, 0.0f)
#define set_y_up(vec) assignment_vector3_xyz(&vec, 0.0f, 1.0, 0.0f)
#define set_z_up(vec) assignment_vector3_xyz(&vec, 0.0f, 0.0, 1.0f)
#define IS_X_UP 0
#define IS_Y_UP 1
#define IS_Z_UP 2
#define convert_degree_rad(d_degree) (M_PI * (double)d_degree) / phi_degree  
#define convert_rad_degree(d_rad) (float)((phi_degree * d_rad) / M_PI)  

typedef struct SGG_Vector2{ float x; float y;}SGG_Vector2;
typedef struct SGG_Vector3{ float x; float y; float z;}SGG_Vector3;
typedef struct SGG_Vector4{ float x; float y; float z; float w;}SGG_Vector4;
typedef struct SGG_Quaternion{ float x; float y; float z; float w;}SGG_Quaternion;
typedef struct SGG_Mat3{float m[3 * 3];}SGG_Mat3;
typedef struct SGG_Mat4{float m[4 * 4];}SGG_Mat4;

_EXTERN_C_BEGIN_

sgg_vec_si_strap float get_maxf(float a, float b){return a > b ? a : b;}
sgg_vec_si_strap float get_minf(float a, float b){return a < b ? a : b;}

sgg_vec_si_strap size_t get_maxu(size_t a, size_t b){return a > b ? a : b;}
sgg_vec_si_strap size_t get_minu(size_t a, size_t b){return a < b ? a : b;}


sgg_vec_si_strap float sgg_dot_vector3(const SGG_Vector3 * a, const SGG_Vector3 * b){
    return a->x * b->x + a->y * b->y + a->z * b->z;
}
sgg_vec_si_strap float sgg_dot_vector4(const SGG_Vector4 * a, const SGG_Vector4 * b){
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}
sgg_vec_si_strap float sgg_dot_quat(const SGG_Quaternion * a, const SGG_Quaternion * b){
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

sgg_vec_si_strap size_t sgg_clampu(size_t v, size_t min, size_t max){
    return get_minu(get_maxu(v, min), max);
}
sgg_vec_si_strap float sgg_clampf(float v, float min, float max){
    return get_minf(get_maxf(v, min), max);
}
sgg_vec_si_strap float sgg_absf(float v){
    if(v < 0.0f) return -v;
    return v;
}

sgg_vec_si_strap void set_f_array_to_vec3(float ** vec3, float x, float y, float z, int next_pos){
    (*vec3)[0] = x; (*vec3)[1] = y; (*vec3)[2] = z;
    (*vec3) += 3 + next_pos;

}


float length_sgg_vector2(const SGG_Vector2 * src_vec2);
float length_sgg_vector3(const SGG_Vector3 * src_vec3);
float length_sgg_vector4(const SGG_Vector4 * src_vec4);
float length_sgg_quaternion(const SGG_Quaternion * src_quat);
bool sgg_equal_vector1(float a, float b);
bool sgg_equal_vector2(const SGG_Vector2 * a, const SGG_Vector2 * b);
bool sgg_equal_vector3(const SGG_Vector3 * a, const SGG_Vector3 * b);
bool sgg_equal_vector4(const SGG_Vector4 * a, const SGG_Vector4 * b);
bool sgg_equal_quaternion(const SGG_Quaternion * a, const SGG_Quaternion * b);
void sgg_neg_vector4(SGG_Vector4 * a);
void sgg_neg_quaternion(SGG_Quaternion * a);
void sgg_swap(float *a, float *b);
void sgg_identity(SGG_Mat4* dst);
void normalize_vector3(SGG_Vector3 * dst_vec3);
void normalize_vector4(SGG_Vector4 * dst_vec4);
void normalize_quaternion(SGG_Quaternion * dst_quat);
void assignment_vector2_vector2(SGG_Vector2 * vec2, const SGG_Vector2 * a_vec2);
void assignment_vector2_xyz(SGG_Vector2 * vec2, float ax, float ay);
void assignment_vector2(SGG_Vector2 * vec2, float ass_vec);

void add_vector2_vector2(SGG_Vector2 * vec2, const SGG_Vector2 * a_vec2);
void add_vector2_xyz(SGG_Vector2 * vec2, float ax, float ay);
void add_vector2(SGG_Vector2 * vec2, float add_vec);

void sub_vector2_vector2(SGG_Vector2 * vec2, const SGG_Vector2 * a_vec2);
void sub_vector2_xyz(SGG_Vector2 * vec2, float ax, float ay);
void sub_vector2(SGG_Vector2 * vec2, float sub_vec);

void mul_vector2_vector2(SGG_Vector2 * vec2, const SGG_Vector2 * a_vec2);
void mul_vector2_xyz(SGG_Vector2 * vec2, float ax, float ay);
void mul_vector2(SGG_Vector2 * vec2, float mul_vec);

void div_vector2_vector2(SGG_Vector2 * vec2, const SGG_Vector2 * a_vec2);
void div_vector2_xyz(SGG_Vector2 * vec2, float ax, float ay);
void div_vector2(SGG_Vector2 * vec2, float div_vec);


void assignment_vector3_vector3(SGG_Vector3 * vec3, const SGG_Vector3 * a_vec3);
void assignment_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az);
void assignment_vector3(SGG_Vector3 * vec3, float ass_vec);

void add_vector3_vector3(SGG_Vector3 * vec3, const SGG_Vector3 * a_vec3);
void add_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az);
void add_vector3(SGG_Vector3 * vec3, float add_vec);

void sub_vector3_vector3(SGG_Vector3 * vec3, const SGG_Vector3 * a_vec3);
void sub_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az);
void sub_vector3(SGG_Vector3 * vec3, float sub_vec);

void mul_vector3_vector3(SGG_Vector3 * vec3, const SGG_Vector3 * a_vec3);
void mul_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az);
void mul_vector3(SGG_Vector3 * vec3, float mul_vec);

void div_vector3_vector3(SGG_Vector3 * vec3, const SGG_Vector3 * a_vec3);
void div_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az);
void div_vector3(SGG_Vector3 * vec3, float div_vec);

void assignment_vector4(SGG_Vector4 * vec4, float ass_vec);

void add_vector4_vector4(SGG_Vector4* vec4, const SGG_Vector4* a_vec4);
void sub_vector4_vector4(SGG_Vector4* vec4, const SGG_Vector4* a_vec4);
void mul_vector4(SGG_Vector4 * vec4, float mul_vec);

void div_vector4_vector4(SGG_Vector4* vec4, const SGG_Vector4* a_vec4);
void div_vector4_xyz(SGG_Vector4* vec4, float ax, float ay, float az, float aw);
void div_vector4(SGG_Vector4* vec4, float div_vec);
void assignment_quaternion(SGG_Quaternion * quat, float ass_vec);
void mul_quaternion(SGG_Quaternion * quat, float ass_vec);

SGG_Quaternion mul_quaternion_quaternion(const SGG_Quaternion * quat, const SGG_Quaternion * a_quat);

void assignment_mat4(SGG_Mat4 * dst, float x);
void mul_mat4_vector4(SGG_Vector4 *dst, const SGG_Mat4 *a, const SGG_Vector4 * b);
void mul_mat4_mat4(SGG_Mat4 *dst, const SGG_Mat4 *a, const SGG_Mat4 *b);

void sgg_trs_mat4(SGG_Mat4 * dst, const SGG_Quaternion * rot, const SGG_Vector3 * pos, const SGG_Vector3 * scale);

void sgg_perspective(SGG_Mat4 * dst_per, float vertical_rad, float aspect, float near, float far);
void sgg_lookAt(SGG_Mat4 * dst_mat4, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
void sgg_ortho(SGG_Mat4 * dst_mat, float left, float right, float bottom, float top, float near_z, float far_z);
void sgg_inverse(SGG_Mat4 * dst_mat4);
void sgg_inverse_quaternion(SGG_Quaternion * dst_quat);
void cross_sgg_vector3(SGG_Vector3 * vec3, const SGG_Vector3 * src_a, const SGG_Vector3 * src_b);
void sgg_quads_matrix(float * q_mat, float left, float right, float bottom, float top);
void sgg_quads_strip_tex_coords_pos_matrix(float * q_mat, float left, float right, float bottom, float top);
void sgg_cube_matrix(float * c_mat, float left, float right, float bottom, float top, float near, float far, int next_pos);
void sgg_sphere_matrix(float * sph_mat, float stack_count, float sector_count, float radius, uint32_t attr_num);
bool look_rotation_vec3_quat(SGG_Quaternion * rot_quat, const SGG_Vector3 * a_pos, const SGG_Vector3 * b_pos, SGG_Vector3 * a_dir);
SGG_Vector3 look_rotation_vec3(SGG_Vector3 a_pos, SGG_Vector3 b_pos, SGG_Vector3 a_dir);
sgg_vec_si_strap float get_distance_ray(const SGG_Vector3 * ray_pos, const SGG_Vector3 * ray_dir, const SGG_Vector3 * a, const SGG_Vector3 * b) {
    float u = sgg_dot_vector3(ray_dir, ray_dir);
    SGG_Vector3 ba_ = *b;
    SGG_Vector3 ra_ = *ray_pos;
    sub_vector3_vector3(&ba_, a);
    sub_vector3_vector3(&ra_, a);
    float v = sgg_dot_vector3(ray_dir, &ba_);
    float w = sgg_dot_vector3(&ba_, &ba_);
    float x = sgg_dot_vector3(ray_dir, &ra_);
    float y = sgg_dot_vector3(&ba_, &ra_);
    /*
    *  |u||w| cos(1) = u*w
    */
    float den_ = u * w - v * v;
    float s = 0.0f;
    float t = 0.0f;
    if (den_ != 0.0f) {
        s = (v * y - w * x) / den_;
    }
    if (s < 0.0) s = 0.0f;
    t = (v * s + y) / w;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    /*
    * D・u = 0
    * D=PQ
    * PQ orthogonal u
    * P -> latest point
    * Q -> latest point
    * D => lim 0 latest
    * (1)
    * -------P
    *        | D
    * -------Q
    * (2)
    * -------P
    *      /  D
    * -------
    * (1) D > (2) D
    * R = S + DIR * T
    * S = START POS
    * T = STEP
    * 
    */

    SGG_Vector3 a_ray = *ray_dir;
    SGG_Vector3 b_ray = ba_;
    mul_vector3(&a_ray, s);
    add_vector3_vector3(&a_ray, ray_pos);
    mul_vector3(&b_ray, t);
    add_vector3_vector3(&b_ray, a);
    SGG_Vector3 sub_ = a_ray;
    sub_vector3_vector3(&sub_, &b_ray);
    return length_sgg_vector3(&sub_);
}
sgg_vec_si_strap void convert_mouse_ray(float mouse_x, float mouse_y, float width_, float height_, SGG_Mat4 * proj, SGG_Mat4 * view, SGG_Vector3 * dst_ray_pos, SGG_Vector3 * dst_ray_dir) {
    float x_ = CONVERT_GL_NDC_WIDTH(mouse_x, width_);
    float y_ = CONVERT_GL_NDC_HEIGHT(mouse_y, height_);
    SGG_Vector4 near_ = {
        x_,
        y_,
        -1.0f,
        1.0f
    };

    SGG_Vector4 far_ = {
        x_,
        y_,
        1.0f,
        1.0f
    };
    SGG_Mat4 inv_;
    mul_mat4_mat4(&inv_, proj, view);
    sgg_inverse(&inv_);
    SGG_Vector4 n_world;
    SGG_Vector4 f_world;
    mul_mat4_vector4(&n_world, &inv_, &near_);
    mul_mat4_vector4(&f_world, &inv_, &far_);
    n_world.x /= n_world.w;
    n_world.y /= n_world.w;
    n_world.z /= n_world.w;


    f_world.x /= f_world.w;
    f_world.y /= f_world.w;
    f_world.z /= f_world.w;

    dst_ray_pos->x = n_world.x;
    dst_ray_pos->y = n_world.y;
    dst_ray_pos->z = n_world.z;


    dst_ray_dir->x = f_world.x - n_world.x;
    dst_ray_dir->y = f_world.y - n_world.y;
    dst_ray_dir->z = f_world.z - n_world.z;
    normalize_vector3(dst_ray_dir);

}
sgg_vec_si_strap SGG_Mat3 mat4_to_mat3(const SGG_Mat4 * mat4){
    return
        SET_NEW_TYPE_(SGG_Mat3){
        mat4->m[0], mat4->m[1], mat4->m[2],
        mat4->m[4], mat4->m[5], mat4->m[6],
        mat4->m[8], mat4->m[9], mat4->m[10],
    };
}
sgg_vec_si_strap SGG_Mat4 mat3_to_mat4(const SGG_Mat3 * mat3){
    return SET_NEW_TYPE_(SGG_Mat4){
        mat3->m[0], mat3->m[1], mat3->m[2], 0.0f,
        mat3->m[3], mat3->m[4], mat3->m[5], 0.0f,
        mat3->m[6], mat3->m[7], mat3->m[8], 0.0f,
        0.0f      , 0.0f      , 0.0f       , 1.0f
    };
}

/**
 * MEMO:
 * 
 * A, B -> C
 * A - A, B - A, C - A
 * 0, B - A, C - A
 * ROT = [X', Y'] = [COS(S), -SIN(S); SIN(S), COS(S)][X, Y]
 * C - A = (B - A)
 * C = ROT(B - A) + A
 * 
*/

sgg_vec_si_strap SGG_Mat4 quat_rotation_matrix(const SGG_Quaternion q){
    /**
    * q = x y z w = (sinθ/2, cos�θ/2)
    * Q = 1 = X + Y + Z + W
    * 
    * sinθ = 2sin(θ/2)cos(θ/2)
    * cosθ = 2cos^2(θ/2) - 1 = 1 - 2sin^2(θ/2)
    * cosθ - 1 = -2sin^2(θ/2)
    * cosθ + nx^2(1 - cosθ)
    * 2cos^2(θ/2) - 1 + nx^2(2sin^2(θ/2))
    * 2w^2 - 1 + 2x^2
    * 2w^2 - Q + 2x^2
    * 2w^2 - X - Y - Z - W + 2x^2
    * 2w^2 - x^2 - y^2 - z^2 - w^2 + 2x^2
    * x^2 + w^2 - y^2 - z^2
    * W = 1 - X - Y - Z
    * x^2 + 1 - x^2 - y^2 - z^2 - y^2 - z^2
    * 1 - 2y^2 - 2z^2
    * [
    *   1 - 2y^2 - 2z^2  2xy + 2wz        2xz - 2wy       0
    *   2xy - 2wz        1 - 2x^2 - 2z^2  2yz + 2wx       0
    *   2xz + 2wy        2yz - 2wx        1 - 2x^2 - 2y^2 0
    *   0                0                0               1
    * ]
    */
    float x2 = q.x * q.x;
    float y2 = q.y * q.y;
    float z2 = q.z * q.z;
    return SET_NEW_TYPE_(SGG_Mat4){
        1 - 2 * y2 - 2 * z2, 2 * q.x * q.y + 2 * q.w * q.z, 2 * q.x * q.z - 2 * q.w * q.y, 0,
        2 * q.x * q.y - 2 * q.w * q.z,  1 - 2 * x2 - 2 * z2, 2 * q.y * q.z + 2 * q.w * q.x, 0,
        2 * q.x * q.z + 2 * q.w * q.y, 2 * q.y * q.z - 2 * q.w * q.x, 1 - 2 * x2 - 2 * y2, 0,
        0, 0, 0, 1,
    };
}

sgg_vec_si_strap SGG_Mat4 d_quat_rotation_matrix(const SGG_Quaternion q, const SGG_Vector3 t){
    /**
    * q = x y z w = (sin��/2, cos��/2)
    * Q = 1 = X + Y + Z + W
    * 
    * sin�� = 2sin(��/2)cos(��/2)
    * cos�� = 2cos^2(��/2) - 1 = 1 - 2sin^2(��/2)
    * cos�� - 1 = -2sin^2(��/2)
    * cos�� + nx^2(1 - cos��)
    * 2cos^2(��/2) - 1 + nx^2(2sin^2(��/2))
    * 2w^2 - 1 + 2x^2
    * 2w^2 - Q + 2x^2
    * 2w^2 - X - Y - Z - W + 2x^2
    * 2w^2 - x^2 - y^2 - z^2 - w^2 + 2x^2
    * x^2 + w^2 - y^2 - z^2
    * W = 1 - X - Y - Z
    * x^2 + 1 - x^2 - y^2 - z^2 - y^2 - z^2
    * 1 - 2y^2 - 2z^2
    * [
    *   1 - 2y^2 - 2z^2  2xy + 2wz        2xz - 2wy       0
    *   2xy - 2wz        1 - 2x^2 - 2z^2  2yz + 2wx       0
    *   2xz + 2wy        2yz - 2wx        1 - 2x^2 - 2y^2 0
    *   0                0                0               1
    * ]
    * 
    * M = [
    * R T
    * 0,1
    * ]
    */
    float x2 = q.x * q.x;
    float y2 = q.y * q.y;
    float z2 = q.z * q.z;
    return SET_NEW_TYPE_(SGG_Mat4){
        1 - 2 * y2 - 2 * z2, 2 * q.x * q.y + 2 * q.w * q.z, 2 * q.x * q.z - 2 * q.w * q.y, t.x,
        2 * q.x * q.y - 2 * q.w * q.z,  1 - 2 * x2 - 2 * z2, 2 * q.y * q.z + 2 * q.w * q.x, t.y,
        2 * q.x * q.z + 2 * q.w * q.y, 2 * q.y * q.z - 2 * q.w * q.x, 1 - 2 * x2 - 2 * y2, t.z,
        0, 0, 0, 1,
    };
}
sgg_vec_si_strap SGG_Vector2 rotation_vec2(SGG_Vector2 a, SGG_Vector2 b, int degree){
    float rad = convert_degree_rad(degree);
    float rcos = cos(rad);
    float rsin = sin(rad);
    sub_vector2_vector2(&b, &a);
    return SET_NEW_TYPE_(SGG_Vector2){rcos * b.x + -rsin * b.y + a.x, rsin * b.x + rcos * b.y + a.x};

}
sgg_vec_si_strap SGG_Vector3 rotation_vec3(SGG_Quaternion q, SGG_Vector3 v){
    /**
     * |Q|=1
     * |Q|!=1 => NORMALIZE
     * 
    * Q' = Q*P*Q^-1
    * Q' = RV
    * 
    * Q' - A = R(V - A)
    * Q' = R(V - A) + A
    * 
    */
    //normalize_vector4(&q);
    SGG_Mat4 r = quat_rotation_matrix(q);
    SGG_Vector4 d;
    SGG_Vector4 f = {v.x, v.y, v.z, 0.0f};
    mul_mat4_vector4(&d, &r, &f);
    return SET_NEW_TYPE_(SGG_Vector3){d.x, d.y, d.z};
}

sgg_vec_si_strap SGG_Vector3 rotation_deg_z_vec3(float deg, SGG_Vector3 v){
    float rad = convert_degree_rad(deg);
    SGG_Quaternion q = {
        0.0f, 0.0f, sinf(rad * 0.5f), cosf(rad * 0.5f) 
    };
    return rotation_vec3(q, v);
}

sgg_vec_si_strap SGG_Quaternion quat_rotation_xyz(float deg_x, float deg_y, float deg_z){
    float rad_x = convert_degree_rad(deg_x);
    float rad_y = convert_degree_rad(deg_y);
    float rad_z = convert_degree_rad(deg_z);
    SGG_Quaternion qx = {
        sinf(rad_x * 0.5f), 0.0f, 0.0f, cosf(rad_x * 0.5f)
    };
    SGG_Quaternion qy = {
        0.0f, sinf(rad_y * 0.5f), 0.0f, cosf(rad_y * 0.5f)
    };
    SGG_Quaternion qz = {
        0.0f, 0.0f, sinf(rad_z * 0.5f), cosf(rad_z * 0.5f) 
    };
    SGG_Quaternion t = mul_quaternion_quaternion(&qz, &qy);
    return mul_quaternion_quaternion(&t, &qx);
}
sgg_vec_si_strap void quat_to_degree_xyz(
    const SGG_Quaternion* q,
    float* deg_x,
    float* deg_y,
    float* deg_z
) {
    float sinr_cosp = 2.0f * (q->w * q->x + q->y * q->z);
    float cosr_cosp = 1.0f - 2.0f * (q->x * q->x + q->y * q->y);
    float rad_x = atan2f(sinr_cosp, cosr_cosp);

    float sinp = 2.0f * (q->w * q->y - q->z * q->x);
    float rad_y;
    if (fabsf(sinp) >= 1.0f) {
        rad_y = copysignf(M_PI / 2.0f, sinp);
    }
    else {
        rad_y = asinf(sinp);
    }

    float siny_cosp = 2.0f * (q->w * q->z + q->x * q->y);
    float cosy_cosp = 1.0f - 2.0f * (q->y * q->y + q->z * q->z);
    float rad_z = atan2f(siny_cosp, cosy_cosp);

    *deg_x = convert_rad_degree(rad_x);
    *deg_y = convert_rad_degree(rad_y);
    *deg_z = convert_rad_degree(rad_z);
}
sgg_vec_si_strap SGG_Vector3 rotation_deg_xyz_vec3(float deg_x, float deg_y, float deg_z, SGG_Vector3 v){
    return rotation_vec3(quat_rotation_xyz(deg_x, deg_y, deg_z), v);
}

sgg_vec_si_strap SGG_Quaternion quat_axis_angle_vec3(float deg, SGG_Vector3 axis){
    normalize_vector3(&axis);
    float rad = convert_degree_rad(deg);
    float sr = sin(rad * 0.5f);
    float cr = cos(rad * 0.5f);
    return SET_NEW_TYPE_(SGG_Quaternion){
        axis.x * sr, axis.y * sr, axis.z * sr, cr
    };
}

sgg_vec_si_strap SGG_Quaternion quat_axis_angle_vec3_rad(float rad, SGG_Vector3 axis){
    normalize_vector3(&axis);
    float sr = sin(rad * 0.5f);
    float cr = cos(rad * 0.5f);
    return
    SET_NEW_TYPE_(SGG_Quaternion) {
        axis.x * sr, axis.y * sr, axis.z * sr, cr
    };
}

sgg_vec_si_strap SGG_Vector3 rotation_transform_point_vec3(SGG_Quaternion q, SGG_Quaternion t, const SGG_Vector3 v){
    normalize_quaternion(&q);
    //mul_vector4_vector4(&t, &q);
    SGG_Quaternion r = mul_quaternion_quaternion(&t, &q);
    mul_quaternion(&r, 0.5f);
    SGG_Vector3 p = {
        2 * (-r.w * q.x + r.x * q.w - r.y * q.z + r.z * q.y),
        2 * (-r.w * q.y + r.x * q.z + r.y * q.w - r.z * q.x),
        2 * (-r.w * q.z - r.x * q.y + r.y * q.x + r.z * q.w),
    };
    
    SGG_Mat4 dq = d_quat_rotation_matrix(q, p);
    SGG_Vector4 d;
    SGG_Vector4 f = {v.x, v.y, v.z, 1.0f};
    mul_mat4_vector4(&d, &dq, &f);
    return SET_NEW_TYPE_(SGG_Vector3){d.x, d.y, d.z};

}
sgg_vec_si_strap SGG_Quaternion rotaion_to_dirs(SGG_Vector3 a, SGG_Vector3 b) {
    float ba_dot = sgg_dot_vector3(&a, &b);
    ba_dot = sgg_clampf(ba_dot, -1.0f, 1.0f);

    float r = acosf(ba_dot);
    SGG_Vector3 axis;
    cross_sgg_vector3(&axis, &a, &b);

    if (length_sgg_vector3(&axis) < EPS) {
        if (ba_dot > 0.0f) return SET_NEW_TYPE_(SGG_Quaternion) { 0.0f, 0.0f, 0.0f, 1.0f };
        SGG_Vector3 b_axis = { 1.0f, 0.0f, 0.0f };
        if (sgg_absf(a.x) > 0.9f) b_axis = SET_NEW_TYPE_(SGG_Vector3){ 0.0f, 1.0f, 0.0f };
        cross_sgg_vector3(&axis, &a, &b_axis);
        return quat_axis_angle_vec3_rad(M_PI, axis);
    }
    return quat_axis_angle_vec3_rad(r, axis);
}
sgg_vec_si_strap float sgg_lerp(float sx, float ex, float t){
    /* V = S * (1 - t) + E * t */
    return sx * (1.0f - t) + ex * t;
}
sgg_vec_si_strap void sgg_lerp_vector3(SGG_Vector3 * dv, const SGG_Vector3 * sv, const SGG_Vector3 * ev, float t){
    float lx = sgg_lerp(sv->x, ev->x, t);
    float ly = sgg_lerp(sv->y, ev->y, t);
    float lz = sgg_lerp(sv->z, ev->z, t);
    dv->x = lx;
    dv->y = ly;
    dv->z = lz;
}

sgg_vec_si_strap void sgg_lerp_vector4(SGG_Vector4 * dv, const SGG_Vector4 * sv, const SGG_Vector4 * ev, float t){
    float lx = sgg_lerp(sv->x, ev->x, t);
    float ly = sgg_lerp(sv->y, ev->y, t);
    float lz = sgg_lerp(sv->z, ev->z, t);
    float lw = sgg_lerp(sv->w, ev->w, t);
    dv->x = lx;
    dv->y = ly;
    dv->z = lz;
    dv->w = lw;
}

sgg_vec_si_strap void sgg_lerp_quaternion(SGG_Quaternion * dv,const SGG_Quaternion * sv,const SGG_Quaternion * ev, float t){
    float lx = sgg_lerp(sv->x, ev->x, t);
    float ly = sgg_lerp(sv->y, ev->y, t);
    float lz = sgg_lerp(sv->z, ev->z, t);
    float lw = sgg_lerp(sv->w, ev->w, t);
    dv->x = lx;
    dv->y = ly;
    dv->z = lz;
    dv->w = lw;
}

sgg_vec_si_strap void sgg_nlerp_quaternion(SGG_Quaternion * dv,const SGG_Quaternion * sv,const SGG_Quaternion * ev, float t){
    sgg_lerp_quaternion(dv, sv, ev, t);
    normalize_quaternion(dv);
}
sgg_vec_si_strap void sgg_slerp_quaternion(SGG_Quaternion * dv, const SGG_Quaternion * sv, const SGG_Quaternion * ev, float t){
    float dot_value = sgg_clampf(sgg_dot_quat(sv, ev), -1.0f, 1.0f);
    SGG_Quaternion eev = *ev;
    if(dot_value < 0.0f){
        dot_value = -dot_value;
        sgg_neg_quaternion(&eev);
    }
    if(dot_value > 0.9995f) sgg_nlerp_quaternion(dv, sv, &eev, t);
    float theta = acos(dot_value);
    float tt = sin((1 - t) * theta);
    float tu = sin(theta);
    float tv = sin(t * theta);
    dv->x = (tt / tu) * sv->x + (tv / tu) * eev.x;
    dv->y = (tt / tu) * sv->y + (tv / tu) * eev.y;
    dv->z = (tt / tu) * sv->z + (tv / tu) * eev.z;
    dv->w = (tt / tu) * sv->w + (tv / tu) * eev.w;
}
_EXTERN_C_END_
#endif
