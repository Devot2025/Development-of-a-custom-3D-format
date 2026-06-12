#ifndef _SGG_VEC_HPP_
#define _SGG_VEC_HPP_
#include "sgg_vec.h"

sgg_vec_si_strap void sgg_identity(SGG_Mat4 &a){sgg_identity(&a);}
sgg_vec_si_strap void sgg_perspective(SGG_Mat4 &a, float vertical_rad, float aspect, float near, float far){sgg_perspective(&a, vertical_rad, aspect, near, far);}
sgg_vec_si_strap void sgg_ortho(SGG_Mat4 & dst_mat, float left, float right, float bottom, float top, float near_z, float far_z){sgg_ortho(&dst_mat, left, right, bottom, top, near_z, far_z);}
sgg_vec_si_strap void sgg_ortho(SGG_Mat4 & dst_mat, float dis, float near_z, float far_z){sgg_ortho(&dst_mat, -dis, dis, -dis, dis, near_z, far_z);}
sgg_vec_si_strap void normalize_vector3(SGG_Vector3 & dst){normalize_vector3(&dst);}
sgg_vec_si_strap void normalize_vector(SGG_Vector4 & dst){normalize_vector4(&dst);}
sgg_vec_si_strap void sgg_inverse(SGG_Mat4 & dst){sgg_inverse(&dst);}

sgg_vec_si_strap void sgg_lookAt(SGG_Mat4 & a, float pos_x, float pos_y, float pos_z, float center_x, float center_y, float center_z, float up_x, float up_y, float up_z){
    sgg_lookAt(&a, pos_x, pos_y, pos_z, center_x, center_y, center_z, up_x, up_y, up_z);
}
sgg_vec_si_strap void sgg_lookAt(SGG_Mat4 & a, const SGG_Vector3 & pos, const SGG_Vector3 &center, const SGG_Vector3 & up){
    sgg_lookAt(a, pos.x, pos.y, pos.z, center.x, center.y, center.z, up.x, up.y, up.z);
}
sgg_vec_si_strap SGG_Vector2 & assignment_vector(SGG_Vector2 & a, const SGG_Vector2 &b){
    assignment_vector2_vector2(&a, &b);
    return a;
}

sgg_vec_si_strap SGG_Vector2 & assignment_vector(SGG_Vector2 & a, float x, float y){
    assignment_vector2_xyz(&a, x, y);
    return a;
}

sgg_vec_si_strap SGG_Vector2 & assignment_vector(SGG_Vector2 & a, float v){
    assignment_vector2(&a, v);
    return a;
}
sgg_vec_si_strap SGG_Vector2 operator + (const SGG_Vector2 & a, const SGG_Vector2 & b){
    SGG_Vector2 c;
    assignment_vector2_vector2(&c, &a);
    add_vector2_vector2(&c, &b);
    return c;
}


sgg_vec_si_strap SGG_Vector2 operator + (SGG_Vector2 & a, const float v){
    SGG_Vector2 c;
    assignment_vector2_vector2(&c, &a);
    add_vector2(&c, v);
    return c;
}

sgg_vec_si_strap SGG_Vector2 operator - (const SGG_Vector2 & a, const SGG_Vector2 & b){
    SGG_Vector2 c;
    assignment_vector2_vector2(&c, &a);
    sub_vector2_vector2(&c, &b);
    return c;
}

sgg_vec_si_strap SGG_Vector2 operator - (SGG_Vector2 & a, const float v){
    SGG_Vector2 c;
    assignment_vector2_vector2(&c, &a);
    sub_vector2(&c, v);
    return c;
}
sgg_vec_si_strap SGG_Vector2 operator * (const SGG_Vector2 & a, const SGG_Vector2 & b){
    SGG_Vector2 c; 
    assignment_vector2_vector2(&c, &a);
    mul_vector2_vector2(&c, &b);
    return c;
}

sgg_vec_si_strap SGG_Vector2 operator * (SGG_Vector2 & a, const float v){
    SGG_Vector2 c;
    assignment_vector2_vector2(&c, &a);
    mul_vector2(&c, v);
    return c;
}
sgg_vec_si_strap SGG_Vector2 operator / (const SGG_Vector2 & a, const SGG_Vector2 & b){
    SGG_Vector2 c; 
    assignment_vector2_vector2(&c, &a);
    div_vector2_vector2(&c, &b);
    return c;
}

sgg_vec_si_strap SGG_Vector2 operator / (SGG_Vector2 & a, const float v){
    SGG_Vector2 c;
    assignment_vector2_vector2(&c, &a);
    div_vector2(&c, v);
    return c;
}
sgg_vec_si_strap SGG_Vector2 &operator += (SGG_Vector2 & a, const SGG_Vector2 & b){
    add_vector2_vector2(&a, &b);
    return a;
}


sgg_vec_si_strap SGG_Vector2 &operator += (SGG_Vector2 & a, const float v){
    add_vector2(&a, v);
    return a;
}
sgg_vec_si_strap SGG_Vector2 &operator -= (SGG_Vector2 & a, const SGG_Vector2 & b){
    sub_vector2_vector2(&a, &b);
    return a;
}

sgg_vec_si_strap SGG_Vector2 &operator -= (SGG_Vector2 & a, const float v){
    sub_vector2(&a, v);
    return a;
}
sgg_vec_si_strap SGG_Vector2 &operator *= (SGG_Vector2 & a, const SGG_Vector2 & b){
    mul_vector2_vector2(&a, &b);
    return a;
}

sgg_vec_si_strap SGG_Vector2 &operator *= (SGG_Vector2 & a, const float v){
    mul_vector2(&a, v);
    return a;
}
sgg_vec_si_strap SGG_Vector2 operator /= (SGG_Vector2 & a, const SGG_Vector2 & b){
    div_vector2_vector2(&a, &b);
    return a;
}

sgg_vec_si_strap SGG_Vector2 &operator /= (SGG_Vector2 & a, const float v){
    div_vector2(&a, v);
    return a;
}

sgg_vec_si_strap SGG_Vector3 operator + (const SGG_Vector3 & a, const SGG_Vector3 & b){
    SGG_Vector3 c;
    assignment_vector3_vector3(&c, &a);
    add_vector3_vector3(&c, &b);
    return c;
}


sgg_vec_si_strap SGG_Vector3 operator + (SGG_Vector3 & a, const float v){
    SGG_Vector3 c;
    assignment_vector3_vector3(&c, &a);
    add_vector3(&c, v);
    return c;
}

sgg_vec_si_strap SGG_Vector3 operator - (const SGG_Vector3 & a, const SGG_Vector3 & b){
    SGG_Vector3 c;
    assignment_vector3_vector3(&c, &a);
    sub_vector3_vector3(&c, &b);
    return c;
}

sgg_vec_si_strap SGG_Vector3 operator - (SGG_Vector3 & a, const float v){
    SGG_Vector3 c;
    assignment_vector3_vector3(&c, &a);
    sub_vector3(&c, v);
    return c;
}
sgg_vec_si_strap SGG_Vector3 operator * (const SGG_Vector3 & a, const SGG_Vector3 & b){
    SGG_Vector3 c; 
    assignment_vector3_vector3(&c, &a);
    mul_vector3_vector3(&c, &b);
    return c;
}

sgg_vec_si_strap SGG_Vector3 operator * (SGG_Vector3 & a, const float v){
    SGG_Vector3 c;
    assignment_vector3_vector3(&c, &a);
    mul_vector3(&c, v);
    return c;
}
sgg_vec_si_strap SGG_Vector3 operator / (const SGG_Vector3 & a, const SGG_Vector3 & b){
    SGG_Vector3 c; 
    assignment_vector3_vector3(&c, &a);
    div_vector3_vector3(&c, &b);
    return c;
}

sgg_vec_si_strap SGG_Vector3 operator / (SGG_Vector3 & a, const float v){
    SGG_Vector3 c;
    assignment_vector3_vector3(&c, &a);
    div_vector3(&c, v);
    return c;
}
sgg_vec_si_strap SGG_Vector3 &operator += (SGG_Vector3 & a, const SGG_Vector3 & b){
    add_vector3_vector3(&a, &b);
    return a;
}


sgg_vec_si_strap SGG_Vector3 &operator += (SGG_Vector3 & a, const float v){
    add_vector3(&a, v);
    return a;
}
sgg_vec_si_strap SGG_Vector3 &operator -= (SGG_Vector3 & a, const SGG_Vector3 & b){
    sub_vector3_vector3(&a, &b);
    return a;
}

sgg_vec_si_strap SGG_Vector3 &operator -= (SGG_Vector3 & a, const float v){
    sub_vector3(&a, v);
    return a;
}
sgg_vec_si_strap SGG_Vector3 &operator *= (SGG_Vector3 & a, const SGG_Vector3 & b){
    mul_vector3_vector3(&a, &b);
    return a;
}

sgg_vec_si_strap SGG_Vector3 &operator *= (SGG_Vector3 & a, const float v){
    mul_vector3(&a, v);
    return a;
}
sgg_vec_si_strap SGG_Vector3 operator /= (SGG_Vector3 & a, const SGG_Vector3 & b){
    div_vector3_vector3(&a, &b);
    return a;
}

sgg_vec_si_strap SGG_Vector3 &operator /= (SGG_Vector3 & a, const float v){
    div_vector3(&a, v);
    return a;
}

sgg_vec_si_strap SGG_Vector3 & assignment_vector(SGG_Vector3 & a, const SGG_Vector3 &b){
    assignment_vector3_vector3(&a, &b);
    return a;
}

sgg_vec_si_strap SGG_Vector3 & assignment_vector(SGG_Vector3 & a, float x, float y, float z){
    assignment_vector3_xyz(&a, x, y, z);
    return a;
}

sgg_vec_si_strap SGG_Vector3 & assignment_vector(SGG_Vector3 & a, float v){
    assignment_vector3(&a, v);
    return a;
}
sgg_vec_si_strap SGG_Vector4 operator * (SGG_Mat4 & a, SGG_Vector4 &b){
    SGG_Vector4 dst;
    mul_mat4_vector4(&dst, &a, &b);
    return dst;
}

sgg_vec_si_strap SGG_Mat4 operator * (SGG_Mat4 & a, SGG_Mat4 & b){
    SGG_Mat4 c;
    mul_mat4_mat4(&c, &a, &b);
    return c;
}
sgg_vec_si_strap SGG_Mat4 &operator *= (SGG_Mat4 & a, SGG_Mat4 & b){
    mul_mat4_mat4(&a, &a, &b);
    return a;
}
#endif
