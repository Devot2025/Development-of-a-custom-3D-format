#ifndef _SC3_H_
#define _SC3_H_
#include <ggmath/sgg_vec.h>
#include <prog_c/std_ext/ext_alloc.h>
#include <prog_c/std_ext/ext_os_strs.h>
#include "prog_c/std_ext/ext_rand.h"
#include <c_header_helper.hint>
#include <prog_c/sc_data_policy/speed_charm_policy.h>

#include <prog_c/std_ext/ext_strs.h>
#include <prog_c/sci_loader/speed_charm_image.h>
#define DEBUG_VERSION                             1
#define RELEASE_VERSION                           0

#define speed_charm_3d_si_strap                   sc_policy_si_strap

#define SC3_FILE_FORMAT                           SC_POLICY_FILE_FORMAT_NAME(3)
#define SC3_DEFAULT_FILE_NAME                     "a" SC3_FILE_FORMAT
#define SC3_FILE_NAME_DEFAULT_SIZE                SC_POLICY_FILE_NAME_MAX

#define SC3_FILE_ENTRY_MAGIC_NUMBER               SC_POLICY_REQUIRE_SC_ENTRY_MAGIC_NUM("SC3")

#define SC3_MAIN_BEG                              SC_POLICY_REQUIRE_BEG_CHUNK
#define SC3_MAIN_END                              SC_POLICY_REQUIRE_END_CHUNK
#define SC3_MAIN_USER                             "USR"
#define SC3_MAIN_COMMENT                          "COM"
#define SC3_DEFAULT_COMMENT                       "Game-only:no blends/cam/lights"

#define SC3_MAIN_TABLE                            "TBL"
#define SC3_MAIN_TABLE_MESH                       "MES"
#define SC3_MAIN_TABLE_BONE                       "BON"
#define SC3_MAIN_TABLE_ANIM                       "ANI"
#define SC3_MAIN_TABLE_TEXTURE                    "TEX"
#define SC3_MAIN_TABLE_INSTANCE                   "INS"

#define SC3_MESH_CHUNK                            "MSC"
#define SC3_MESH_INFORMATION_CHUNK                "MIC"
#define SC3_MESH_DATA_CHUNK                       "MDC"
#define SC3_BLEND_MESH_INFORMATION_CHUNK          "BIC"
#define SC3_BLEND_MESH_DATA_CHUNK                 "BDC"

#define SC3_BONE_CHUNK                            "BNC"

#define SC3_ANIM_CHUNK                            "ANC"
#define SC3_ANIM_INFORMATION_CHUNK                "AIC"
#define SC3_ANIM_DATA_CHUNK                       "ADC"

#define SC3_TEXTURE_CHUNK                         "TXC"
#define SC3_MATERIAL_TEXTURE_CHUNK                "SMT"
#define SC3_SCI_IMAGE_CHUNK                       "STC"
#define SC3_SCI_STATIC_IMAGE_CHUNK                "STC"
#define SC3_SCI_DYNAMIC_IMAGE_CHUNK               "DTC"

#define SC3_INSTANCE_CHUNK                        "INC"
#define SC3_INSTANCE_INFORMATION_CHUNK            "IIC"
#define SC3_INSTANCE_DATA_CHUNK                   "IDC"

#define SC3_FREE_DATA_CHUNK                       "SFD"

#define SC3_MAIN_TABLE_POS                        11
#define SC3_TABLE_META_LEN                        3
#define SC3_CHUNK_LEN                             3

#define CALC_SC3_TABLE_POS(NUM__)                 SC3_MAIN_TABLE_POS + sizeof(sc_size) * (NUM__) + SC3_TABLE_META_LEN * (NUM__ + 1)

#define SC3_MESH_TABLE_POS                        CALC_SC3_TABLE_POS(0)
#define SC3_BONE_TABLE_POS                        CALC_SC3_TABLE_POS(1)
#define SC3_ANIM_TABLE_POS                        CALC_SC3_TABLE_POS(2)
#define SC3_TEX_TABLE_POS                         CALC_SC3_TABLE_POS(3)
#define SC3_INS_TABLE_POS                         CALC_SC3_TABLE_POS(4)

#define NOT_HAS_MATERIALS_POINT                   0
#define TOP_PARENT                                -1
#define NOT_DYNAMIC_POINT                         TOP_PARENT
#define DIMENSION_XYZ                             3

#define SC3_TEXTURE_MATERIAL_CHANNEL              4

#define SC3_FRAMERATE__                           30.0f
#define SC3_FRAMERATE_60                          SC3_FRAMERATE__ * 2.0f
#define SC3_MAX_FRAME_NUM                         4096

#define SC3_MAIN_MESH_CHUNK_FIRST_POS
#define SC3_MAIN_HAVE_SKIN                        1

#define SC3_LOD_MIN                               0
#define SC3_LOD_MID                               1
#define SC3_LOD_MAX                               2
#define SC3_LOD_MAX_NUM                           3

#define SC3_SCI_TEXTURE_OPTIONAL                  (1 << 0)
#define SC3_SCI_TEXTURE_DYNAMIC_OPTIONAL          (1 << 1)
#define SC3_SCI_TEXTURE_STATIC_OPTIONAL           (1 << 2)
#define SC3_BONE_MAP_OPTIONAL                     (1 << 1)

#define SC3_OFFSET_INF_NUM                        (SC_POLICY_SC_UINT_8_BITS - 1)
#define SC3_HALF_UINT_8_BITS                      (SC_POLICY_SC_UINT_8_BITS / 2)

#define SC3_DEFORM_ROT_STATIC                     1 << 0
#define SC3_DEFORM_POS_STATIC                     1 << 1
#define SC3_DEFORM_SCALE_STATIC                   1 << 2

#define SC3_CHECK_DYNAMIC_ROT(status)             !(SC3_DEFORM_ROT_STATIC & status)
#define SC3_CHECK_DYNAMIC_POS(status)             !(SC3_DEFORM_POS_STATIC & status)
#define SC3_CHECK_DYNAMIC_SCL(status)             !(SC3_DEFORM_SCALE_STATIC & status)

#define SC3_COMMENT_SIZE_MAX                      35
#define SC3_FREE_DATA_SIZE                        (1 << 8)
#define SC3_SKIN_MAX_SIZE                         4
#define SC3_DEFAULT_COMMENT_LEN                   SC_POLICY_OS_ANOTHER_COMMENT_SIZE(SC3_COMMENT_SIZE_MAX)

#define SC3_DEBUG_VERSION_STR                     #SC3_DEBUG_VERSION
#define SC3_TEXTURE_VALID_VERSION                 SC_POLICY_SET_SC_VERSIONS(3, 0)
#define SC3_DYNAMIC_POINT_VERSION                 SC_POLICY_SET_SC_VERSIONS(2, 0)
#define SC3_INSTANCE_VALID_VERSION                SC_POLICY_SET_SC_VERSIONS(4, 0)
#define SC3_BLEND_MESH_VALID_VERSION                SC_POLICY_SET_SC_VERSIONS(5, 0)
#define SC3_NOW_VERSION                           SC_POLICY_SET_NOW_VERSION(SC3_INSTANCE_VALID_VERSION)

#define SC3_EXAPMLE_MESH_ATTR_STRIDE              8
#define SC3_EXAPMLE_POS_SIZE                      3
#define SC3_EXAPMLE_NORMALIZE_SIZE                3
#define SC3_EXAPMLE_UV_SIZE                       2
#define SC3_EXAPMLE_POS_LOCATION                  0
#define SC3_EXAPMLE_NORMALIZE_LOCATION            1

#define SC3_MESH_MATERIAL_POINT_ALBEDO            0
#define SC3_MESH_MATERIAL_POINT_NORMAL            1
#define SC3_MESH_MATERIAL_POINT_METALLIC          2
#define SC3_MESH_MATERIAL_POINT_ROUGHNESS         3
#define SC3_MESH_MATERIAL_POINT_AO                4
#define SC3_MESH_MATERIAL_POINT_ARM               5
#define SC3_MESH_MATERIAL_POINT_ALPHA             6

#define GET_SC3_VMP(sc3_main, materials_point)    materials_point ? &sc3_main->sc_vmp[materials_point - 1] : NULL

#define SC3_FILE_OUT_CODE                         SC_POLICY_OUT_NORMAL

#define SC3_FILE_W_FOPEN                          SC_POLICY_ERROR_FOPEN
#define SC3_FILE_R_FOPEN                          SC_POLICY_ERROR_FOPEN

#define SC3_FILE_READ_ENTRY_MAGIC_NUM             SC_POLICY_ERROR_ENTRY_MAGIC_NUM
#define SC3_FILE_READ_USR                         SC_POLICY_ERROR_USER
#define SC3_FILE_READ_BEG                         SC_POLICY_ERROR_BEG
#define SC3_FILE_READ_END                         SC_POLICY_ERROR_END
#define SC3_FILE_READ_COM                         SC_POLICY_ERROR_COMMENT_MAIN

#define SC3_INVALID_VERSION                       SC_POLICY_ERROR_INVALID_VERSION

#define SC3_FILE_ERROR_CODE                       SC_POLICY_ERROR_IDENTIFY_START(1)
#define SC3_FILE_ERROR_VAR_NULL                   SC_POLICY_ERROR_IDENTIFY_START(2)

#define SC3_FILE_ERROR_MAIN                       SC_POLICY_ERROR_IDENTIFY_START(3)
#define SC3_FILE_ERROR_MAIN_TABLE                 SC_POLICY_ERROR_IDENTIFY_START(4)

#define SC3_FILE_ERROR_MESH                       SC_POLICY_ERROR_IDENTIFY_START(5)
#define SC3_FILE_ERROR_MESH_TABLE                 SC_POLICY_ERROR_IDENTIFY_START(6)
#define SC3_FILE_ERROR_MESH_INFORMATION           SC_POLICY_ERROR_IDENTIFY_START(7)
#define SC3_FILE_ERROR_MESH_DATA                  SC_POLICY_ERROR_IDENTIFY_START(8)
#define SC3_FILE_ERROR_MESH_DATA__                SC_POLICY_ERROR_IDENTIFY_START(9)
#define SC3_FILE_ERROR_MATCH_MESH_SKIN_DATA       SC_POLICY_ERROR_IDENTIFY_START(10)
#define SC3_FILE_ERROR_MATCH_MESH_VERT_DATA       SC_POLICY_ERROR_IDENTIFY_START(11)

#define SC3_FILE_ERROR_BONE                       SC_POLICY_ERROR_IDENTIFY_START(12)
#define SC3_FILE_ERROR_BONE_TABLE                 SC_POLICY_ERROR_IDENTIFY_START(13)
#define SC3_FILE_ERROR_MATCH_BONE_DATA            SC_POLICY_ERROR_IDENTIFY_START(14)
#define SC3_FILE_ERROR_MATCH_SKELETON_DATA        SC_POLICY_ERROR_IDENTIFY_START(15)

#define SC3_FILE_ERROR_ANIM                       SC_POLICY_ERROR_IDENTIFY_START(16)
#define SC3_FILE_ERROR_ANIM_TABLE                 SC_POLICY_ERROR_IDENTIFY_START(17)
#define SC3_FILE_ERROR_ANIM_INFORMATION           SC_POLICY_ERROR_IDENTIFY_START(18)
#define SC3_FILE_ERROR_ANIM_DATA                  SC_POLICY_ERROR_IDENTIFY_START(19)
#define SC3_ANIM_INFORMATION_ERROR_FRAME          SC_POLICY_ERROR_IDENTIFY_START(20)
#define SC3_ANIM_INFORMATION_ERROR_FRAMERATE      SC_POLICY_ERROR_IDENTIFY_START(21)
#define SC3_FILE_ERROR_MATCH_ANIM_DEF_DATA        SC_POLICY_ERROR_IDENTIFY_START(22)
#define SC3_FILE_ERROR_MATCH_ANIM_ROT_DATA        SC_POLICY_ERROR_IDENTIFY_START(23)
#define SC3_FILE_ERROR_MATCH_ANIM_POS_DATA        SC_POLICY_ERROR_IDENTIFY_START(24)
#define SC3_FILE_ERROR_MATCH_ANIM_SCL_DATA        SC_POLICY_ERROR_IDENTIFY_START(25)

#define SC3_FILE_ERROR_TEX                        SC_POLICY_ERROR_IDENTIFY_START(26)
#define SC3_FILE_ERROR_TEX_TABLE                  SC_POLICY_ERROR_IDENTIFY_START(27)
#define SC3_FILE_ERROR_TEX_INVALID_VERSION        SC_POLICY_ERROR_IDENTIFY_START(28)

#define SC3_FILE_ERROR_INS                        SC_POLICY_ERROR_IDENTIFY_START(29)
#define SC3_FILE_ERROR_INS_TABLE                  SC_POLICY_ERROR_IDENTIFY_START(30)
#define SC3_FILE_ERROR_INS_INFORMATION            SC_POLICY_ERROR_IDENTIFY_START(31)
#define SC3_FILE_ERROR_INS_DATA                   SC_POLICY_ERROR_IDENTIFY_START(32)
#define SC3_FILE_ERROR_INS_LOD_MAX                SC_POLICY_ERROR_IDENTIFY_START(33)

#define SC3_FILE_ERROR_SUM_CHECK                  SC_POLICY_ERROR_IDENTIFY_START(34)
#define SC3_FILE_ERROR_VIRTUAL_DATA               SC_POLICY_ERROR_IDENTIFY_START(35)
#define SC3_FILE_ERROR_FILE_END_NEXT              SC_POLICY_ERROR_IDENTIFY_START(36)
#define SC3_FILE_ERROR_DE_COMPRESSION             SC_POLICY_ERROR_IDENTIFY_START(37)

#define SC3_FREE_FUNC_API_NONE                    SC_POLICY_ERROR_IDENTIFY_START(38)


#define SC3_HAS_TEXTURES                          1 << 0
#define SC3_HAS_VALUES_SCALER                     1 << 2
#define SC3_HAS_VALUES_VECTOR                     1 << 3
#define SC3_HAS_VALUES_VECTOR_ALPHA               1 << 4
#define OLD_API_STRUCT_NAME(api_struct)           api_struct ## __
#define HAS_SC3_SKIN(mesh_inf) mesh_inf & SC3_MAIN_HAVE_SKIN
typedef union Speed_Charm_3D_Tags {
    char sc3_entry_magic_num[len_of_str(SC3_FILE_ENTRY_MAGIC_NUMBER)];
    //char sc3_ver[STRING_LITERAL_SIZE(SC3_VER)];
    char sc3_beg[len_of_str(SC3_MAIN_BEG)];
    char sc3_end[len_of_str(SC3_MAIN_END)];
    char sc3_usr[len_of_str(SC3_MAIN_USER)];
    char sc3_com[SC3_COMMENT_SIZE_MAX];
    char sc3_tbl[SC3_TABLE_META_LEN];
    char sc3_chunks[SC3_CHUNK_LEN];
}Speed_Charm_3D_Tags;

typedef struct Speed_Charm_3D_Skin_Attr {
    sc_ubyte    skin_weight[SC3_SKIN_MAX_SIZE];
    sc_ushort   skin_indices[SC3_SKIN_MAX_SIZE];
}Speed_Charm_3D_Skin_Attr;

typedef struct Speed_Charm_3D_Mesh_Attr {
    SGG_Vector3 pos;
    SGG_Vector3 norm;
    SGG_Vector2 uv;
}Speed_Charm_3D_Mesh_Attr;

typedef struct Speed_Charm_3D_Mesh_Bone_Map {
    SGG_Mat4 bone_map;
    sc_uint bone_ind;
}Speed_Charm_3D_Mesh_Bone_Map;
struct Speed_Charm_3D_Skeleton_Node { SGG_Mat4 to_world; SGG_Mat4 to_parent; };

typedef struct OLD_API_STRUCT_NAME(Speed_Charm_3D_Skeleton) {
    union {
        struct Speed_Charm_3D_Skeleton_Node node;
    };
    //struct{SGG_Mat4 inv_default_mat;SGG_Mat4 default_mat;};
    sc_int parent;
}OLD_API_STRUCT_NAME(Speed_Charm_3D_Skeleton);
/*
*
* skeleton
* init
* pos, rot, scale
* trs[pos, rot, scale] ----> init bone deform
* parent inverse matrix(parent matrix) * children matrix =>  relatrive matrix  => children matrix
* The idea is to convert between world space and relative (local) space using inverse values.

In this explanation, matrices are intentionally avoided because they can be difficult to understand. Instead, simple scalar equations are used to illustrate the concept.

---

First, during initialization, the child position is converted into a relative position based on the parent.

If the parent world position is 5 and the child world position is 4, then the relative position of the child is calculated as:

-5 + 4 = -1

This means the child is located at -1 relative to the parent.

So the initial state stores this relative position.

---

Next, when updating the position, we convert this relative position back into world space.

If the parent world position is 5 and the relative position is -1, then:

5 + (-1) = 4

This reconstructs the childfs world position.

If the child has moved and its relative position becomes -2, then:

5 + (-2) = 3

So the new world position becomes 3.

---

After that, we consider how much the child has moved compared to its initial position.

The initial world position was 4, so to compare relatively, we take its inverse:

-4

Then we add the current world position:

-4 + 3 = -1

This represents how much the child has moved from its initial position.

---

In summary:

Relative position (local space):

* parent + child = relative
  -5 + 4 = -1

Reconstruction to world space:
parent + relative = world
5 + (-1) = 4

Difference from initial state:

* initial + current = movement
  -4 + 3 = -1

So the inverse value is reused to interpret positions relatively, either from the parent or from the initial state.

*
*/

typedef struct Speed_Charm_3D_Skeleton {
    union {
        struct Speed_Charm_3D_Skeleton_Node node;
    };
    sc_int parent;
    sc_int dynamic_point;
}Speed_Charm_3D_Skeleton;
typedef struct OLD_API_STRUCT_NAME(Speed_Charm_3D_Mesh) {
    Speed_Charm_3D_Skin_Attr* skin_attr;
    Speed_Charm_3D_Mesh_Attr* mesh_attr;
    sc_uint* indices;
    sc_size                    attr_num;
    sc_size                    ind_num;
    sc_ubyte                    mesh_inf;
}OLD_API_STRUCT_NAME(Speed_Charm_3D_Mesh);

typedef struct Speed_Charm_3D_Material_Buffers {
    sc_ushort sc_has_material;
    union {
        float material_value;
        SGG_Vector3 material_value3;
        SGG_Vector4 material_value4;
        Speed_Charm_Image_Data material_map;
    };
}Speed_Charm_3D_Material_Buffers;

typedef struct Speed_Charm_3D_Mesh_Material_Point {
    sc_size albedo_idx;
    sc_size normal_idx;
    sc_size roughness_idx;
    sc_size metallic_idx;
    sc_size ao_idx;
    sc_size arm_idx;
    sc_size alpha_idx;
}Speed_Charm_3D_Mesh_Material_Point;

typedef struct Speed_Charm_3D_Mesh {
    Speed_Charm_3D_Skin_Attr* skin_attr;
    Speed_Charm_3D_Mesh_Attr* mesh_attr;
    sc_uint* indices;
    sc_size                    attr_num;
    sc_size                    ind_num;
    sc_ubyte                    mesh_inf;
    Speed_Charm_3D_Mesh_Material_Point material_point;
}Speed_Charm_3D_Mesh;
typedef struct Speed_Charm_3D_Blend_Mesh {
    SGG_Vector3 center;
    Speed_Charm_3D_Mesh sc3_mesh;
}Speed_Charm_3D_Blend_Mesh;
typedef struct Speed_Charm_3D_Anim_Deform {
    sc_ubyte deform_status; //
    SGG_Quaternion* rot;
    SGG_Vector3* pos;
    SGG_Vector3* scale;
}Speed_Charm_3D_Anim_Deform;
typedef struct Speed_Charm_3D_Anim {
    float time_beg;
    float time_end;
    float framerate;
    sc_size frame_num;
    //sc_ubyte * deform_status; // A
    Speed_Charm_3D_Anim_Deform* anim_deforms;
}Speed_Charm_3D_Anim;
typedef struct Speed_Charm_3D_Bone_Map {
    SGG_Mat4 inverse_mat;
    sc_uint ind;
}Speed_Charm_3D_Bone_Map;
typedef struct Speed_Charm_3D_Instance_Group {
    SGG_Mat4* trs;
    SGG_Vector3 center;
    uint32_t num;
}Speed_Charm_3D_Instance_Group;
typedef struct Speed_Charm_3D_Instance {
    sc_uint level[SC3_LOD_MAX_NUM];
    Speed_Charm_3D_Instance_Group* group;
    uint32_t group_num;
    uint32_t level_num;
}Speed_Charm_3D_Instance;
/*
* Skeleton
*  first position
*  first rotaion
*  first scale
*  init trs ->
*/
typedef struct Speed_Charm_3D_Main {
    Speed_Charm_3D_Mesh* sc_meshes;
    Speed_Charm_3D_Anim* sc_anim;
    Speed_Charm_3D_Bone_Map* sc_bones;
    Speed_Charm_3D_Skeleton* sc_skeletons;
    Speed_Charm_3D_Material_Buffers* sc_materials;
    Speed_Charm_3D_Instance* sc_ins;
    ///Speed_Charm_3D_Blend_Mesh* sc_blend_meshes;
    //sc_size               blend_meshes_num;
    sc_size               instance_num;
    sc_size               skeletons_num;
    sc_size               meshes_num;
    sc_size               bones_num;
    sc_size               anim_num;
    sc_size               materials_num;
}Speed_Charm_3D_Main;

typedef struct Speed_Charm_3D_Main_Contents {
    SC_FILE sc3_f__;
    sc_ushort sc3_version;
    uint8_t sc3_optional;

    struct {
        sc_size mesh_pos;
        sc_size bone_pos;
        sc_size anim_pos;
        sc_size tex_pos;
        sc_size ins_pos;
    };
    Speed_Charm_3D_Main* sc3_main;
}Speed_Charm_3D_Main_Contents;

/**
 * @brief Implementation-defined.
 * The structure layout and behavior are left to the implementation.
 */
struct Speed_Charm_3D_Virtual_Vertex_Package;
struct Speed_Charm_3D_Virtual_Material_Package;

typedef struct Speed_Charm_3D_Run_Mesh {
    //Speed_Charm_3D_Mesh_Bone_Map * mesh_bone_map;
    //sc_size mesh_bone_map_num;
    sc_ubyte mesh_inf;
    Speed_Charm_3D_Mesh_Material_Point material_point;
}Speed_Charm_3D_Run_Mesh;

typedef struct Speed_Charm_3D_Run_Game_Data {
    SGG_Vector3 pos;
    SGG_Vector3 scale;
}Speed_Charm_3D_Run_Game_Data;
typedef struct Speed_Charm_3D_Dynamic_Deform {
    SGG_Quaternion dynamic_rot;
}Speed_Charm_3D_Dynamic_Deform;
typedef struct Speed_Charm_3D_Run_Main {
    sc_size meshes_num;
    sc_size bones_num;
    sc_size skeletons_num;
    sc_size anim_num;
    sc_size dynamic_deform_num;
    sc_size instance_num;
    struct Speed_Charm_3D_Virtual_Vertex_Package* sc_vvp;
    struct Speed_Charm_3D_Virtual_Material_Package* sc_vmp;

    Speed_Charm_3D_Dynamic_Deform* sc_dynamic_def;
    Speed_Charm_3D_Run_Mesh* sc_meshes;
    Speed_Charm_3D_Anim* sc_anim;
    Speed_Charm_3D_Bone_Map* sc_bones;
    Speed_Charm_3D_Skeleton* sc_skeletons;
    Speed_Charm_3D_Instance* sc_ins;

    float anim_times;
}Speed_Charm_3D_Run_Main;


typedef struct Speed_Charm_3D_Virtual_Vertex_Package* virtual_setter_vvp_func(Speed_Charm_3D_Main* sc3_main);

typedef virtual_setter_vvp_func* create_sc3_vvp_api_funcs;

typedef struct Speed_Charm_3D_Virtual_Vertex_Package* virtual_free_vvp_func(struct Speed_Charm_3D_Virtual_Vertex_Package* sc3_vvp);

typedef virtual_free_vvp_func* free_vvp_api_funcs;


typedef struct Speed_Charm_3D_Virtual_Material_Package* virtual_setter_vmp_func(Speed_Charm_3D_Main* sc3_main);

typedef virtual_setter_vmp_func* create_sc3_vmp_api_funcs;

typedef struct Speed_Charm_3D_Virtual_Material_Package* virtual_free_vmp_func(struct Speed_Charm_3D_Virtual_Material_Package* sc3_vvp);

typedef virtual_free_vmp_func* free_vmp_api_funcs;

_EXTERN_C_BEGIN_

#ifdef ANIM_DEFORM_STATUS_DEBUG
speed_charm_3d_si_strap sc_ubyte get_animation_deform_status(sc_size d_idx, Speed_Charm_3D_Anim* sc3_anim) {

    const sc_size idx__ = d_idx / 2;
    const sc_ubyte dfs__ = sc3_anim->deform_status[idx__];
    return d_idx % 2 ? dfs__ >> 4 : ((dfs__ << 4) >> 4);
}

#else

speed_charm_3d_si_strap sc_ubyte get_animation_deform_status(sc_size d_idx, const Speed_Charm_3D_Anim* sc3_anim) {
    return sc3_anim->anim_deforms[d_idx].deform_status;
}

speed_charm_3d_si_strap sc_ubyte get_animation_deform_status_packed(sc_size d_idx, Speed_Charm_3D_Anim* sc3_anim) {
    const sc_size idx__ = d_idx / 2;
    const sc_ubyte dfs__ = sc3_anim->anim_deforms[idx__].deform_status;
    return d_idx % 2 ? dfs__ >> SC3_HALF_UINT_8_BITS : ((dfs__ << SC3_HALF_UINT_8_BITS) >> SC3_HALF_UINT_8_BITS);

}

speed_charm_3d_si_strap sc_ubyte* sc3_zstd_compress(const sc_ubyte* buf, sc_size* buf_size) {
    size_t t_size = *buf_size;
    size_t max_compressed_size = ZSTD_compressBound(
        t_size
    );

    void* dst = e_malloc(max_compressed_size);
    if (!dst) return NULL;
    size_t d_size = ZSTD_compress(
        dst,
        max_compressed_size,
        buf,
        t_size,
        7
    );
    if (ZSTD_isError(d_size)) {
        s_free(dst);
        return NULL;
    }
    *buf_size = d_size;
    return (sc_ubyte*)dst;
}

speed_charm_3d_si_strap sc_ubyte* sc3_de_zstd_compress(const sc_ubyte* buf, sc_size buf_size) {
    size_t tmp_comp_size = buf_size;
    size_t tmp_de_comp_size = ZSTD_getFrameContentSize(buf, buf_size);
    void* compressed = e_malloc(tmp_de_comp_size);
    size_t compressed_size = ZSTD_decompress(
        compressed,
        tmp_de_comp_size,
        buf,
        tmp_comp_size
    );
    if (ZSTD_isError(compressed_size)) {
        s_free(compressed);
        return NULL;
    }
    return (sc_ubyte*)compressed;
}
#endif

speed_charm_3d_si_strap bool smart_sc3_read_tags_match(char* dst, char* src, sc_size s_size, SC_FILE sc3_f) {
    scl_fread(sc3_f, dst, sc_byte, s_size);
    return equal_string(dst, src, s_size);
}

void delete_duplicate_animation(Speed_Charm_3D_Main* sc3_main);
sc_err_t delete_speed_charm_3d_main(Speed_Charm_3D_Main** sc3_main);
sc_err_t delete_speed_charm_3d_main_contents(Speed_Charm_3D_Main_Contents** sc3_main_cont);
sc_err_t delete_speed_charm_3d_run_main(Speed_Charm_3D_Run_Main** sc3_run_main, free_vvp_api_funcs s_free_vvp, free_vmp_api_funcs s_free_vmp);

sc_err_t delete_invalid_data(Speed_Charm_3D_Main* sc3_main_cont);
Speed_Charm_3D_Run_Main* share_sc3_main_sc3_run_main(Speed_Charm_3D_Main** sc3_main, struct Speed_Charm_3D_Virtual_Vertex_Package* vvp, struct Speed_Charm_3D_Virtual_Material_Package* vmp);
Speed_Charm_3D_Run_Main* move_sc3_main_to_sc3_run_main(Speed_Charm_3D_Main** sc3_main, struct Speed_Charm_3D_Virtual_Vertex_Package* vvp, struct Speed_Charm_3D_Virtual_Material_Package* vmp);

void set_sc3_file_format(sc_byte* sc3_path);

void goto_sc3f_pos(SC_FILE __sc3f, sc_size pos);

void set_pbr_external_api_tex_materials(Speed_Charm_3D_Main* sc3_main, sc_size mesh_idx, sc_ushort materials_type_id, const void* new_materials, const sc_size width, const sc_size height);
Speed_Charm_3D_Main_Contents* init_speed_charm_3d_main_contents(Speed_Charm_3D_Main* sc3_main);

sc_err_t write_speed_charm_3d_main(const sc_byte* sc3_file_path, Speed_Charm_3D_Main* sc3_main);
sc_err_t write_speed_charm_3d_data_main(Speed_Charm_3D_Main_Contents* sc3_main_cont);
sc_err_t write_speed_charm_3d_file_entry(Speed_Charm_3D_Main_Contents* sc3_main_cont, const sc_byte* sc3_file_path, sc_ushort version);
sc_err_t write_speed_charm_3d_main_table(Speed_Charm_3D_Main_Contents* sc3_main_cont);
sc_err_t write_speed_charm_3d_user(Speed_Charm_3D_Main_Contents* sc3_main_cont, const sc_byte* comment_str);
sc_err_t write_speed_charm_3d_anim(Speed_Charm_3D_Main_Contents* sc3_main_cont, sc_size* end_size);
sc_err_t write_speed_charm_3d_bone(Speed_Charm_3D_Main_Contents* sc3_main_cont, sc_size* end_size);
sc_err_t write_speed_charm_3d_ins(Speed_Charm_3D_Main_Contents* sc3_main_cont, sc_size* end_size);
sc_err_t write_speed_charm_3d_mesh(Speed_Charm_3D_Main_Contents* sc3_main_cont, sc_size* end_size);
sc_err_t write_speed_charm_3d_tex(Speed_Charm_3D_Main_Contents* sc3_main_cont, sc_size* end_size);
sc_err_t read_speed_charm_3d_main(Speed_Charm_3D_Main** sc3_main, const sc_byte* sc3_path);
sc_err_t read_speed_charm_3d_entry_data(Speed_Charm_3D_Main_Contents* sc3_main_cont, Speed_Charm_3D_Tags* sc3_tags, const sc_byte* sc3_path);
sc_err_t read_speed_charm_3d_table(Speed_Charm_3D_Main_Contents* sc3_main_cont, Speed_Charm_3D_Tags* sc3_tags);
sc_err_t read_speed_charm_3d_user(Speed_Charm_3D_Main_Contents* sc3_main_cont, Speed_Charm_3D_Tags* sc3_tags);
sc_err_t read_speed_charm_3d_close(Speed_Charm_3D_Main_Contents* sc3_main_cont, Speed_Charm_3D_Tags* sc3_tags);
sc_err_t read_speed_charm_3d_data(Speed_Charm_3D_Main_Contents* sc3_main_cont, Speed_Charm_3D_Tags* sc3_tags);
sc_err_t read_speed_charm_3d_mesh(Speed_Charm_3D_Main_Contents* sc3_main_cont, Speed_Charm_3D_Tags* sc3_tags);
sc_err_t read_speed_charm_3d_anim(Speed_Charm_3D_Main_Contents* sc3_main_cont, Speed_Charm_3D_Tags* sc3_tags);
sc_err_t read_speed_charm_3d_bone(Speed_Charm_3D_Main_Contents* sc3_main_cont, Speed_Charm_3D_Tags* sc3_tags);
sc_err_t read_speed_charm_3d_tex(Speed_Charm_3D_Main_Contents* sc3_main_cont, Speed_Charm_3D_Tags* sc3_tags);
sc_err_t read_speed_charm_3d_ins(Speed_Charm_3D_Main_Contents* sc3_main_cont, Speed_Charm_3D_Tags* sc3_tags);
/*implement to animation*/
speed_charm_3d_si_strap bool has_skin_runtime_mesh(Speed_Charm_3D_Run_Mesh* sc3_mesh) { return HAS_SC3_SKIN(sc3_mesh->mesh_inf); }
speed_charm_3d_si_strap bool has_skin_mesh(Speed_Charm_3D_Mesh* sc3_mesh) { return HAS_SC3_SKIN(sc3_mesh->mesh_inf); }
speed_charm_3d_si_strap Speed_Charm_3D_Skeleton* get_skelton_node(Speed_Charm_3D_Run_Main* sc3_main, sc_size idx) { return &sc3_main->sc_skeletons[sc3_main->sc_bones[idx].ind]; }
SGG_Quaternion get_anim_dynamic_rot(SGG_Quaternion anim_rot, Speed_Charm_3D_Skeleton* sc3_skeleton, Speed_Charm_3D_Dynamic_Deform* sc3_dynamic_deform);
void update_dynamic_point(sc_size skeleton_index, sc_int dynamic_point_idx, Speed_Charm_3D_Run_Main* sc3_main);
void update_dynamic_rotation_def(float deg_x, float deg_y, float deg_z, sc_int dynamic_point_idx, Speed_Charm_3D_Run_Main* sc3_main);
SGG_Quaternion* get_dynamic_point_rotation(sc_size skeleton_index, Speed_Charm_3D_Run_Main* sc3_main);
void set_dynamic_point_rotation(float deg_x, float deg_y, float deg_z, sc_size skeleton_index, sc_int dynamic_point_idx, Speed_Charm_3D_Run_Main* sc3_main);
void alloc_dynamic_rotation(Speed_Charm_3D_Run_Main* sc3_main, sc_size size_);
void update_animation_speed_charm_3d_bone(Speed_Charm_3D_Run_Main* sc3_main, sc_uint* anim_num, float delta_time);
void reset_skeleton_dynamic_point(Speed_Charm_3D_Run_Main* sc3_main, sc_size skeleton_index);
void update_speed_charm_3d_bones_hierarchy(Speed_Charm_3D_Run_Main* sc3_main);
void build_bones_speed_charm_3d(Speed_Charm_3D_Run_Main* sc3_main, Speed_Charm_3D_Bone_Map* sc3_mesh_bone_map, SGG_Mat4* sc3_mat4);
float get_instance_groups_distance(Speed_Charm_3D_Instance* sc3_ins, SGG_Vector3 pos, sc_uint group_idx);
sc_uint get_instance_group_meshes_idx(Speed_Charm_3D_Run_Main* sc3_main, SGG_Vector3 pos, sc_uint group_idx, sc_uint instance_idx, float dist_min, float scaler);
void create_instance_groups_kmodel(Speed_Charm_3D_Main* sc3_main, const sc_uint* meshes_idx, sc_uint meshes_idx_num, const SGG_Mat4* trs, const sc_uint num, const sc_uint group_num);
void test_create_k_means_translate_cluster(void);
_EXTERN_C_END_
#endif //_SC3_H_