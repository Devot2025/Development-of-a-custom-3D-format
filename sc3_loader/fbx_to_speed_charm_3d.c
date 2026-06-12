#include "fbx_to_speed_charm_3d.h"
#define SC3_ARM_TYPE_AO 0
#define SC3_ARM_TYPE_ROUGHNESS 1
#define SC3_ARM_TYPE_METALLIC 2

typedef struct SC3_FBX_ARM_Cache {
    sc_size ao_idx;
    sc_size roughness_idx;
    sc_size metallic_idx;
    sc_size arm_idx;
}SC3_FBX_ARM_Cache;

static float sc3_fbx_material_value_to_float(Speed_Charm_3D_Material_Buffers* mat) {
    if (!mat) return 1.0f;

    if (mat->sc_has_material == SC3_HAS_VALUES_SCALER) {
        return mat->material_value;
    }
    if (mat->sc_has_material == SC3_HAS_VALUES_VECTOR) {
        return mat->material_value3.x;
    }
    if (mat->sc_has_material == SC3_HAS_VALUES_VECTOR_ALPHA) {
        return mat->material_value4.x;
    }

    return 1.0f;
}

static float sc3_fbx_arm_default_value_float(sc_size type) {
    switch (type) {
    case SC3_ARM_TYPE_AO:
        return 1.0f;

    case SC3_ARM_TYPE_ROUGHNESS:
        return 1.0f;

    case SC3_ARM_TYPE_METALLIC:
        return 0.0f;
    }

    return 1.0f;
}

static sc_ubyte sc3_fbx_arm_default_value(sc_size type) {
    switch (type) {
    case SC3_ARM_TYPE_AO:
        return 255;

    case SC3_ARM_TYPE_ROUGHNESS:
        return 255;

    case SC3_ARM_TYPE_METALLIC:
        return 0;
    }

    return 255;
}

static sc_ubyte sc3_fbx_float_to_ubyte(float v) {
    if (v < 0.0f) v = 0.0f;
    if (v > 1.0f) v = 1.0f;

    return (sc_ubyte)(v * 255.0f);
}

static bool sc3_fbx_material_is_texture(
    Speed_Charm_3D_Main* sc3_main,
    sc_size idx
) {
    if (!idx) return false;

    Speed_Charm_3D_Material_Buffers* mat =
        &sc3_main->sc_materials[idx - 1];

    return mat->sc_has_material == SC3_HAS_TEXTURES &&
        mat->material_map.img_data;
}

static sc_ubyte sc3_fbx_material_sample(
    Speed_Charm_3D_Main* sc3_main,
    sc_size idx,
    sc_size type,
    sc_size x,
    sc_size y,
    sc_size out_x,
    sc_size out_y
) {
    if (!idx) {
        return sc3_fbx_arm_default_value(type);
    }

    Speed_Charm_3D_Material_Buffers* mat =
        &sc3_main->sc_materials[idx - 1];

    if (mat->sc_has_material == SC3_HAS_TEXTURES && mat->material_map.img_data) {
        sc_size src_x = 0;
        sc_size src_y = 0;

        if (out_x > 1) src_x = x * (mat->material_map.x - 1) / (out_x - 1);
        if (out_y > 1) src_y = y * (mat->material_map.y - 1) / (out_y - 1);

        sc_size src_idx =
            (src_y * mat->material_map.x + src_x) * 4;

        return mat->material_map.img_data[src_idx];
    }

    return sc3_fbx_float_to_ubyte(
        sc3_fbx_material_value_to_float(mat)
    );
}

static sc_size sc3_fbx_push_arm_value_material(
    Speed_Charm_3D_Main* sc3_main,
    sc_size ao_idx,
    sc_size roughness_idx,
    sc_size metallic_idx
) {
    Speed_Charm_3D_Material_Buffers* n =
        smart_realloc(
            Speed_Charm_3D_Material_Buffers,
            sc3_main->sc_materials,
            sc3_main->materials_num + 1
        );

    if (!n) return 0;

    sc3_main->sc_materials = n;

    sc_size arm_idx = sc3_main->materials_num + 1;

    sc3_main->sc_materials[sc3_main->materials_num].sc_has_material =
        SC3_HAS_VALUES_VECTOR;

    sc3_main->sc_materials[sc3_main->materials_num].material_value3 =
        (SGG_Vector3){
            ao_idx ?
                sc3_fbx_material_value_to_float(&sc3_main->sc_materials[ao_idx - 1]) :
                sc3_fbx_arm_default_value_float(SC3_ARM_TYPE_AO),

            roughness_idx ?
                sc3_fbx_material_value_to_float(&sc3_main->sc_materials[roughness_idx - 1]) :
                sc3_fbx_arm_default_value_float(SC3_ARM_TYPE_ROUGHNESS),

            metallic_idx ?
                sc3_fbx_material_value_to_float(&sc3_main->sc_materials[metallic_idx - 1]) :
                sc3_fbx_arm_default_value_float(SC3_ARM_TYPE_METALLIC)
    };

    sc3_main->materials_num++;

    return arm_idx;
}

static sc_size sc3_fbx_push_arm_texture_material(
    Speed_Charm_3D_Main* sc3_main,
    sc_size ao_idx,
    sc_size roughness_idx,
    sc_size metallic_idx
) {
    sc_size width = 1;
    sc_size height = 1;

    sc_size ids[3] = {
        ao_idx,
        roughness_idx,
        metallic_idx
    };

    for (sc_size i = 0; i < 3; ++i) {
        if (!ids[i]) continue;

        Speed_Charm_3D_Material_Buffers* mat =
            &sc3_main->sc_materials[ids[i] - 1];

        if (mat->sc_has_material == SC3_HAS_TEXTURES && mat->material_map.img_data) {
            if ((sc_size)mat->material_map.x > width) {
                width = mat->material_map.x;
            }

            if ((sc_size)mat->material_map.y > height) {
                height = mat->material_map.y;
            }
        }
    }

    sc_ubyte* arm_buffer =
        smart_malloc(sc_ubyte, width * height * 4);

    if (!arm_buffer) return 0;

    for (sc_size y = 0; y < height; ++y) {
        for (sc_size x = 0; x < width; ++x) {
            sc_size dst_idx = (y * width + x) * 4;

            arm_buffer[dst_idx + 0] =
                sc3_fbx_material_sample(
                    sc3_main,
                    ao_idx,
                    SC3_ARM_TYPE_AO,
                    x,
                    y,
                    width,
                    height
                );

            arm_buffer[dst_idx + 1] =
                sc3_fbx_material_sample(
                    sc3_main,
                    roughness_idx,
                    SC3_ARM_TYPE_ROUGHNESS,
                    x,
                    y,
                    width,
                    height
                );

            arm_buffer[dst_idx + 2] =
                sc3_fbx_material_sample(
                    sc3_main,
                    metallic_idx,
                    SC3_ARM_TYPE_METALLIC,
                    x,
                    y,
                    width,
                    height
                );

            arm_buffer[dst_idx + 3] = 255;
        }
    }

    Speed_Charm_3D_Material_Buffers* n =
        smart_realloc(
            Speed_Charm_3D_Material_Buffers,
            sc3_main->sc_materials,
            sc3_main->materials_num + 1
        );

    if (!n) {
        s_free(arm_buffer);
        return 0;
    }

    sc3_main->sc_materials = n;

    sc_size arm_idx = sc3_main->materials_num + 1;

    sc3_main->sc_materials[sc3_main->materials_num].sc_has_material =
        SC3_HAS_TEXTURES;

    sc3_main->sc_materials[sc3_main->materials_num].material_map =
        (Speed_Charm_Image_Data){
            width,
            height,
            arm_buffer
    };

    sc3_main->materials_num++;

    return arm_idx;
}

static sc_size sc3_fbx_create_arm_material(
    Speed_Charm_3D_Main* sc3_main,
    sc_size ao_idx,
    sc_size roughness_idx,
    sc_size metallic_idx
) {
    bool has_texture = false;

    if (sc3_fbx_material_is_texture(sc3_main, ao_idx)) {
        has_texture = true;
    }

    if (sc3_fbx_material_is_texture(sc3_main, roughness_idx)) {
        has_texture = true;
    }

    if (sc3_fbx_material_is_texture(sc3_main, metallic_idx)) {
        has_texture = true;
    }

    if (!has_texture) {
        return sc3_fbx_push_arm_value_material(
            sc3_main,
            ao_idx,
            roughness_idx,
            metallic_idx
        );
    }

    return sc3_fbx_push_arm_texture_material(
        sc3_main,
        ao_idx,
        roughness_idx,
        metallic_idx
    );
}

static void sc3_fbx_build_arm_materials(Speed_Charm_3D_Main* sc3_main) {
    SC3_FBX_ARM_Cache* arm_cache =
        smart_calloc(SC3_FBX_ARM_Cache, sc3_main->meshes_num);

    if (!arm_cache) return;

    sc_size arm_cache_num = 0;

    tmp_printf("[FBX ARM] build start\n");

    for (sc_size i = 0; i < sc3_main->meshes_num; ++i) {
        Speed_Charm_3D_Mesh* mesh =
            &sc3_main->sc_meshes[i];

        if (mesh->material_point.arm_idx) {
            tmp_printf(
                "[FBX ARM] mesh %zu already has ARM %zu\n",
                i,
                mesh->material_point.arm_idx
            );
            continue;
        }

        sc_size ao_idx = mesh->material_point.ao_idx;
        sc_size roughness_idx = mesh->material_point.roughness_idx;
        sc_size metallic_idx = mesh->material_point.metallic_idx;

        sc_size arm_idx = 0;

        for (sc_size j = 0; j < arm_cache_num; ++j) {
            if (
                arm_cache[j].ao_idx == ao_idx &&
                arm_cache[j].roughness_idx == roughness_idx &&
                arm_cache[j].metallic_idx == metallic_idx
                ) {
                arm_idx = arm_cache[j].arm_idx;

                tmp_printf(
                    "[FBX ARM] mesh %zu reuse ARM %zu  AO=%zu R=%zu M=%zu\n",
                    i,
                    arm_idx,
                    ao_idx,
                    roughness_idx,
                    metallic_idx
                );

                break;
            }
        }

        if (!arm_idx) {
            arm_idx = sc3_fbx_create_arm_material(
                sc3_main,
                ao_idx,
                roughness_idx,
                metallic_idx
            );

            if (arm_idx) {
                arm_cache[arm_cache_num].ao_idx = ao_idx;
                arm_cache[arm_cache_num].roughness_idx = roughness_idx;
                arm_cache[arm_cache_num].metallic_idx = metallic_idx;
                arm_cache[arm_cache_num].arm_idx = arm_idx;
                arm_cache_num++;

                tmp_printf(
                    "[FBX ARM] mesh %zu create ARM %zu  AO=%zu R=%zu M=%zu\n",
                    i,
                    arm_idx,
                    ao_idx,
                    roughness_idx,
                    metallic_idx
                );
            }
            else {
                tmp_printf(
                    "[FBX ARM] mesh %zu failed create ARM  AO=%zu R=%zu M=%zu\n",
                    i,
                    ao_idx,
                    roughness_idx,
                    metallic_idx
                );
            }
        }

        mesh->material_point.arm_idx = arm_idx;
    }

    tmp_printf("[FBX ARM] build end  cache=%zu\n", arm_cache_num);

    s_free(arm_cache);
}

static void sc3_fbx_clear_arm_source_materials(Speed_Charm_3D_Main* sc3_main) {
    tmp_printf("[FBX ARM] clear AO/Roughness/Metallic start\n");

    for (sc_size i = 0; i < sc3_main->meshes_num; ++i) {
        sc3_main->sc_meshes[i].material_point.ao_idx = 0;
        sc3_main->sc_meshes[i].material_point.roughness_idx = 0;
        sc3_main->sc_meshes[i].material_point.metallic_idx = 0;
    }

    tmp_printf("[FBX ARM] clear AO/Roughness/Metallic end\n");
}

static void sc3_fbx_mark_material_idx(
    bool* used,
    sc_size materials_num,
    sc_size idx
) {
    if (!idx) return;
    if (idx > materials_num) return;

    used[idx - 1] = true;
}

static void sc3_fbx_remap_material_idx(
    sc_size* idx,
    sc_size* remap,
    sc_size materials_num
) {
    if (!*idx) return;

    if (*idx > materials_num) {
        *idx = 0;
        return;
    }

    *idx = remap[*idx - 1];
}

static void sc3_fbx_reduce_unused_materials(Speed_Charm_3D_Main* sc3_main) {
    bool* used =
        smart_calloc(bool, sc3_main->materials_num);

    sc_size* remap =
        smart_calloc(sc_size, sc3_main->materials_num);

    if (!used || !remap) {
        s_free(used);
        s_free(remap);
        return;
    }

    sc_size old_materials_num = sc3_main->materials_num;

    for (sc_size i = 0; i < sc3_main->meshes_num; ++i) {
        Speed_Charm_3D_Mesh_Material_Point* material_point =
            &sc3_main->sc_meshes[i].material_point;

        sc3_fbx_mark_material_idx(
            used,
            sc3_main->materials_num,
            material_point->albedo_idx
        );

        sc3_fbx_mark_material_idx(
            used,
            sc3_main->materials_num,
            material_point->normal_idx
        );

        sc3_fbx_mark_material_idx(
            used,
            sc3_main->materials_num,
            material_point->ao_idx
        );

        sc3_fbx_mark_material_idx(
            used,
            sc3_main->materials_num,
            material_point->roughness_idx
        );

        sc3_fbx_mark_material_idx(
            used,
            sc3_main->materials_num,
            material_point->metallic_idx
        );

        sc3_fbx_mark_material_idx(
            used,
            sc3_main->materials_num,
            material_point->arm_idx
        );

        sc3_fbx_mark_material_idx(
            used,
            sc3_main->materials_num,
            material_point->alpha_idx
        );
    }

    sc_size new_materials_num = 0;

    for (sc_size i = 0; i < sc3_main->materials_num; ++i) {
        if (used[i]) {
            remap[i] = new_materials_num + 1;

            if (i != new_materials_num) {
                sc3_main->sc_materials[new_materials_num] =
                    sc3_main->sc_materials[i];
            }

            new_materials_num++;
        }
    }

    for (sc_size i = 0; i < sc3_main->meshes_num; ++i) {
        Speed_Charm_3D_Mesh_Material_Point* material_point =
            &sc3_main->sc_meshes[i].material_point;

        sc3_fbx_remap_material_idx(
            &material_point->albedo_idx,
            remap,
            sc3_main->materials_num
        );

        sc3_fbx_remap_material_idx(
            &material_point->normal_idx,
            remap,
            sc3_main->materials_num
        );

        sc3_fbx_remap_material_idx(
            &material_point->ao_idx,
            remap,
            sc3_main->materials_num
        );

        sc3_fbx_remap_material_idx(
            &material_point->roughness_idx,
            remap,
            sc3_main->materials_num
        );

        sc3_fbx_remap_material_idx(
            &material_point->metallic_idx,
            remap,
            sc3_main->materials_num
        );

        sc3_fbx_remap_material_idx(
            &material_point->arm_idx,
            remap,
            sc3_main->materials_num
        );

        sc3_fbx_remap_material_idx(
            &material_point->alpha_idx,
            remap,
            sc3_main->materials_num
        );
    }

    Speed_Charm_3D_Material_Buffers* n =
        smart_realloc(
            Speed_Charm_3D_Material_Buffers,
            sc3_main->sc_materials,
            new_materials_num
        );

    if (n || !new_materials_num) {
        sc3_main->sc_materials = n;
    }

    sc3_main->materials_num = new_materials_num;

    tmp_printf(
        "[FBX ARM] reduce materials %zu -> %zu\n",
        old_materials_num,
        new_materials_num
    );

    s_free(used);
    s_free(remap);
}
ufbx_scene * loading_simple_fbx(const char * fbx_file_path, float default_size){
    ufbx_load_opts __opts = {0};
    __opts.target_unit_meters = default_size;
    ufbx_error load_err;
    ufbx_scene * __scene = ufbx_load_file(fbx_file_path, &__opts, &load_err);
    return __scene;
}

Speed_Charm_3D_Main * convert_fbx_speed_charm_3d(const char * fbx_file_path, float default_size){

    if(default_size != 0.0f) default_size = 1.0f / default_size;
    else default_size = 1.0f;
    if(!fbx_file_path) return NULL;
    ufbx_scene * f_scene = loading_simple_fbx(fbx_file_path, default_size);
    tmp_printf(
        "LOADING %s\n", fbx_file_path
    );
    if(!f_scene) return NULL;
    tmp_printf(
        "CONVERT FBX TO SC3\n"
    );
    Speed_Charm_3D_Main * sc3_main = smart_calloc(Speed_Charm_3D_Main, 1);
    if(!sc3_main) return NULL;
    sc3_main->meshes_num = f_scene->meshes.count;
    sc3_main->sc_meshes = smart_calloc(Speed_Charm_3D_Mesh, f_scene->meshes.count);
    Speed_Charm_3D_Bone_Map_Cache * bone_list = smart_calloc(Speed_Charm_3D_Bone_Map_Cache , sc3_main->meshes_num);
    Ufbx_Materials_Extends_List ufbx_extends_lists;
    ufbx_extends_lists.materials_idx = 0;
    ufbx_extends_lists.materials_num = 10;
    ufbx_extends_lists.materials = smart_malloc(Ufbx_Material_Extends_List_, ufbx_extends_lists.materials_num);
    if(!ufbx_extends_lists.materials) return NULL;
    tmp_printf("CONVERT MESHES\n");
    for(sc_size i = 0; i < sc3_main->meshes_num; ++i) convert_fbx_speed_charm_3d_mesh(&sc3_main->sc_meshes[i], &ufbx_extends_lists, &bone_list[i], f_scene->meshes.data[i]);
    sc3_main->materials_num = ufbx_extends_lists.materials_idx;
    sc3_main->sc_materials = smart_calloc(Speed_Charm_3D_Material_Buffers, sc3_main->materials_num);
    if(!sc3_main->sc_materials){
        //return NULL;
    }
    
    const char * folders = strrchr(fbx_file_path, '/');
    if(!folders) folders = strrchr(fbx_file_path, '\\');
    for(sc_size i = 0; i < sc3_main->materials_num; ++i){
        stbi_set_flip_vertically_on_load(false);
        tmp_printf("LOADING IMG\n");
        switch(ufbx_extends_lists.materials[i].status){
            case 0:
            stbi_set_flip_vertically_on_load(true);
            sc3_main->sc_materials[i].sc_has_material = SC3_HAS_TEXTURES;
            if(ufbx_extends_lists.materials[i].texs->content.size){
                int x;
                int y;
                int channel;
                void * buffer = 
                stbi_load_from_memory(
                ufbx_extends_lists.materials[i].texs->content.data, 
                ufbx_extends_lists.materials[i].texs->content.size,
                &x,
                &y, 
                &channel,
                4
                 );
                tmp_printf("LAODING ROW DATA\n");

               if(buffer){

                    sc_ubyte * a_buffer = smart_malloc(sc_ubyte ,x * y * 4);
                    smart_memcpy_p(a_buffer, buffer, sc_ubyte, x * y * 4);
                    sc3_main->sc_materials[i].material_map = (Speed_Charm_Image_Data){
                        x, y, a_buffer
                    };
                    stbi_image_free(buffer);

                    continue;
               }
            }
            if(ufbx_extends_lists.materials[i].texs->absolute_filename.length > 0){
                
                int x;
                int y;
                int channel;
                char * tfilename = strrchr(&ufbx_extends_lists.materials[i].texs->absolute_filename.data[0], '.');
                sc_size path_len = ufbx_extends_lists.materials[i].texs->absolute_filename.length;

                sc_size dot_pos = path_len;
                if(tfilename){
                    dot_pos = (sc_size)(tfilename - &ufbx_extends_lists.materials[i].texs->absolute_filename.data[0]);
                }

                sc_size alloc_size = path_len + 5;
                if(dot_pos < path_len){
                    if((path_len - dot_pos) < 4){
                        alloc_size = dot_pos + 5;
                    }
                }

                char * filename = smart_malloc(char, alloc_size);
                if(!filename) continue;
                smart_memcpy_p(
                    filename,
                    &ufbx_extends_lists.materials[i].texs->absolute_filename.data[0],
                    char,
                    path_len
                );
                filename[path_len] = '\0';

                if(tfilename){
                    bool is_png = false;
                    bool is_jpg = false;

                    if(tfilename[1] && tfilename[2] && tfilename[3] && !tfilename[4]){
                        if(tfilename[1] == 'p' && tfilename[2] == 'n' && tfilename[3] == 'g'){
                            is_png = true;
                        }
                        else if(tfilename[1] == 'j' && tfilename[2] == 'p' && tfilename[3] == 'g'){
                            is_jpg = true;
                        }
                    }

                    if(!(is_png || is_jpg)){
                        filename[dot_pos + 0] = '.';
                        filename[dot_pos + 1] = 'p';
                        filename[dot_pos + 2] = 'n';
                        filename[dot_pos + 3] = 'g';
                        filename[dot_pos + 4] = '\0';
                    }
                }
                else{
                    filename[path_len + 0] = '.';
                    filename[path_len + 1] = 'p';
                    filename[path_len + 2] = 'n';
                    filename[path_len + 3] = 'g';
                    filename[path_len + 4] = '\0';
                }
                void * buffer = 
                 stbi_load(
                filename,
                &x,
                &y, 
                &channel,
                4
                 );
                tmp_printf("LAODING %lu %s FILE DATA\n", i, filename);

                 s_free(filename);
                 /*
                if(!buffer){
                    const char * stack_str =  &ufbx_extends_lists.materials[i].texs->absolute_filename.data[ufbx_extends_lists.materials[i].texs->absolute_filename.length - 1];
                    while(stack_str != ufbx_extends_lists.materials[i].texs->absolute_filename.data){
                        if(*stack_str == '\\' || *stack_str == '/'){              
                            size_t folders_len = 0;
                            if(folders) folders_len = (size_t)(folders - fbx_file_path) + 1;
                            
                            size_t tmp_size = strlen(stack_str) + folders_len + 1;
                            char * tmp = smart_malloc(char, tmp_size);

                            smart_memcpy_p(tmp, fbx_file_path, char, folders_len);

                            tmp[folders_len] = '\0';

                            strcat(tmp, stack_str + 1);

                            buffer = 
                             stbi_load(
                            tmp,
                            &x,
                            &y, 
                            &channel,
                            4
                             );

                             s_free(tmp);
                             if(buffer)break;
                        }
                        stack_str--;
                    }
                    if(!buffer)continue;
                }
                */
               if(buffer){

                    sc_ubyte * a_buffer = smart_malloc(sc_ubyte ,x * y * 4);
                    smart_memcpy_p(a_buffer, buffer, sc_ubyte, x * y * 4);
                    sc3_main->sc_materials[i].material_map = (Speed_Charm_Image_Data){
                        x, y, a_buffer
                    };
                    stbi_image_free(buffer);

                    continue;
               }
            }

            if(ufbx_extends_lists.materials[i].texs->filename.length > 0){
                int x;
                int y;
                int channel;
                char * tfilename = strrchr(&ufbx_extends_lists.materials[i].texs->filename.data[0], '.');
                sc_size path_len = ufbx_extends_lists.materials[i].texs->filename.length;
                sc_size dot_pos = path_len;
                if(tfilename){
                    dot_pos = (sc_size)(tfilename - &ufbx_extends_lists.materials[i].texs->filename.data[0]);
                }
                sc_size alloc_size = path_len + 5;
                if(dot_pos < path_len){
                    if((path_len - dot_pos) < 4){
                        alloc_size = dot_pos + 5;
                    }
                }
                char * filename = smart_malloc(char, alloc_size);
                if(!filename) continue;
                smart_memcpy_p(
                    filename,
                    &ufbx_extends_lists.materials[i].texs->filename.data[0],
                    char,
                    path_len
                );
                filename[path_len] = '\0';
                if(tfilename){
                    bool is_png = false;
                    bool is_jpg = false;
                    if(tfilename[1] && tfilename[2] && tfilename[3] && !tfilename[4]){
                        if(tfilename[1] == 'p' && tfilename[2] == 'n' && tfilename[3] == 'g'){
                            is_png = true;
                        }
                        else if(tfilename[1] == 'j' && tfilename[2] == 'p' && tfilename[3] == 'g'){
                            is_jpg = true;
                        }
                    }
                    if(!(is_png || is_jpg)){
                        filename[dot_pos + 0] = '.';
                        filename[dot_pos + 1] = 'p';
                        filename[dot_pos + 2] = 'n';
                        filename[dot_pos + 3] = 'g';
                        filename[dot_pos + 4] = '\0';
                    }
                }
                else{
                    filename[path_len + 0] = '.';
                    filename[path_len + 1] = 'p';
                    filename[path_len + 2] = 'n';
                    filename[path_len + 3] = 'g';
                    filename[path_len + 4] = '\0';
                }
                void * buffer = 
                
                stbi_load(
                filename,
                &x,
                &y, 
                &channel,
                4
                 );
                tmp_printf("LOADING %lu %s FILE DATA\n", i, filename);

               //printf("%s\n", filename);
                 
                s_free(filename);

               if(buffer){

                    //printf("%s %u %u %u\n",  ufbx_extends_lists.materials[i].texs->filename.data, x, y, channel);
                    sc_ubyte * a_buffer = smart_malloc(sc_ubyte ,x * y * 4);
                    smart_memcpy_p(a_buffer, buffer, sc_ubyte, x * y * 4);
                    sc3_main->sc_materials[i].material_map = (Speed_Charm_Image_Data){
                        x, y, a_buffer
                    };
                    stbi_image_free(buffer);

                    continue;
               }
            }
            if(ufbx_extends_lists.materials[i].texs->relative_filename.length > 0){
                int x;
                int y;
                int channel;
                char * tfilename = strrchr(&ufbx_extends_lists.materials[i].texs->relative_filename.data[0], '.');
                sc_size path_len = ufbx_extends_lists.materials[i].texs->relative_filename.length;
                sc_size dot_pos = path_len;
                if(tfilename){
                    dot_pos = (sc_size)(tfilename - &ufbx_extends_lists.materials[i].texs->relative_filename.data[0]);
                }
                sc_size alloc_size = path_len + 5;
                if(dot_pos < path_len){
                    if((path_len - dot_pos) < 4){
                        alloc_size = dot_pos + 5;
                    }
                }
                char * filename = smart_malloc(char, alloc_size);
                if(!filename) continue;
                smart_memcpy_p(
                    filename,
                    &ufbx_extends_lists.materials[i].texs->relative_filename.data[0],
                    char,
                    path_len
                );
                filename[path_len] = '\0';
                if(tfilename){
                    bool is_png = false;
                    bool is_jpg = false;
                    if(tfilename[1] && tfilename[2] && tfilename[3] && !tfilename[4]){
                        if(tfilename[1] == 'p' && tfilename[2] == 'n' && tfilename[3] == 'g'){
                            is_png = true;
                        }
                        else if(tfilename[1] == 'j' && tfilename[2] == 'p' && tfilename[3] == 'g'){
                            is_jpg = true;
                        }
                    }
                    if(!(is_png || is_jpg)){
                        filename[dot_pos + 0] = '.';
                        filename[dot_pos + 1] = 'p';
                        filename[dot_pos + 2] = 'n';
                        filename[dot_pos + 3] = 'g';
                        filename[dot_pos + 4] = '\0';
                    }
                }
                else{
                    filename[path_len + 0] = '.';
                    filename[path_len + 1] = 'p';
                    filename[path_len + 2] = 'n';
                    filename[path_len + 3] = 'g';
                    filename[path_len + 4] = '\0';
                }
                void * buffer = 
                 stbi_load(
                filename,
                &x,
                &y, 
                &channel,
                4
                 );
                tmp_printf("LAODING %lu %s FILE DATA\n", i, filename);

                 s_free(filename);
               if(buffer){

                    sc_ubyte * a_buffer = smart_malloc(sc_ubyte ,x * y * 4);
                    smart_memcpy_p(a_buffer, buffer, sc_ubyte, x * y * 4);
                    sc3_main->sc_materials[i].material_map = (Speed_Charm_Image_Data){
                        x, y, a_buffer
                    };
                    stbi_image_free(buffer);
                    
                    continue;

               }
            }
            tmp_printf("FIALED FILE DATA\n");

            break;
            case 1:
                sc3_main->sc_materials[i].sc_has_material = SC3_HAS_VALUES_SCALER;
            
                sc3_main->sc_materials[i].material_value = ufbx_extends_lists.materials[i].v;
                break;
            case 2:
                sc3_main->sc_materials[i].sc_has_material = SC3_HAS_VALUES_VECTOR;
                sc3_main->sc_materials[i].material_value3.x = ufbx_extends_lists.materials[i].v2.x;
                sc3_main->sc_materials[i].material_value3.y = ufbx_extends_lists.materials[i].v2.y;
                sc3_main->sc_materials[i].material_value3.z = 1.0f;
                break;
            case 3:
                sc3_main->sc_materials[i].sc_has_material = SC3_HAS_VALUES_VECTOR;
                sc3_main->sc_materials[i].material_value3.x = ufbx_extends_lists.materials[i].v3.x;
                sc3_main->sc_materials[i].material_value3.y = ufbx_extends_lists.materials[i].v3.y;
                sc3_main->sc_materials[i].material_value3.z = ufbx_extends_lists.materials[i].v3.z;
                break;
            case 4:
                sc3_main->sc_materials[i].sc_has_material = SC3_HAS_VALUES_VECTOR_ALPHA;
                sc3_main->sc_materials[i].material_value4.x = ufbx_extends_lists.materials[i].v4.x;
                sc3_main->sc_materials[i].material_value4.y = ufbx_extends_lists.materials[i].v4.y;
                sc3_main->sc_materials[i].material_value4.z = ufbx_extends_lists.materials[i].v4.z;
                sc3_main->sc_materials[i].material_value4.w = ufbx_extends_lists.materials[i].v4.w;
                break;

        }
    }
    sc3_fbx_build_arm_materials(sc3_main);

    sc3_fbx_clear_arm_source_materials(sc3_main);

    sc3_fbx_reduce_unused_materials(sc3_main);
    s_free(ufbx_extends_lists.materials);
    sc3_main->skeletons_num = f_scene->nodes.count;
    sc3_main->sc_skeletons = smart_calloc(Speed_Charm_3D_Skeleton, sc3_main->skeletons_num);
    tmp_printf("LOADING SKELETON\n");

    for(sc_size i = 0; i < sc3_main->skeletons_num; ++i){
        sc3_main->sc_skeletons[i].parent = f_scene->nodes.data[i]->parent ? f_scene->nodes.data[i]->parent->typed_id : -1;
        convert_fbx_mat_to_sc3_mesh_mat(&sc3_main->sc_skeletons[i].node.to_world , &f_scene->nodes.data[i]->node_to_world);
        convert_fbx_mat_to_sc3_mesh_mat(&sc3_main->sc_skeletons[i].node.to_parent , &f_scene->nodes.data[i]->node_to_parent);
        sc3_main->sc_skeletons[i].dynamic_point = NOT_DYNAMIC_POINT;
        //convert_fbx_mat_to_sc3_mesh_mat(&sc3_main->sc_bones[i].skl_inv , &f_scene->nodes.data[i]->node_to_parent);
    }

    sc3_main->anim_num = f_scene->anim_stacks.count;
    //Ufbx_Materials_Extends_List ufbx_list;
    sc3_main->sc_anim = smart_calloc(Speed_Charm_3D_Anim, sc3_main->anim_num);
    tmp_printf("LOADING ANIM\n");

    for(sc_size i = 0; i < sc3_main->anim_num; ++i) convert_fbx_speed_charm_3d_anim(&sc3_main->sc_anim[i], f_scene->nodes.data, f_scene->nodes.count, f_scene->anim_stacks.data[i]);
    
    Speed_Charm_3D_Bone_Map * sc3_bone_map__;
    
    sc_size total_bone_map__ = 0;
    
    for(sc_size i = 0; i < sc3_main->meshes_num; ++i) total_bone_map__ += bone_list[i].bone_map_num;
    
    sc3_bone_map__ = smart_calloc(Speed_Charm_3D_Bone_Map, total_bone_map__);
    
    sc_size idx_bone_map__ = 0;
    
    if(!sc3_bone_map__) return sc3_main;
    //tmp_printf_e("%d\n", total_bone_map__);
    tmp_printf("LOADING BONE MAP\n");

    for(sc_size i = 0; i < sc3_main->meshes_num; ++i) {
        for(sc_size j = 0; j < bone_list[i].bone_map_num; ++j){
            Speed_Charm_3D_Bone_Map sc3_mesh_bone__ = bone_list[i].bones[j];
            if(!idx_bone_map__) sc3_bone_map__[idx_bone_map__++] = sc3_mesh_bone__;
            else {
                bool check__ = false;
                for(sc_size k = 0; k < idx_bone_map__; ++k){
                    Speed_Charm_3D_Bone_Map sc3_mesh_bone = sc3_bone_map__[k];
                    if(memcmp(&sc3_mesh_bone__.inverse_mat, &sc3_mesh_bone.inverse_mat, sizeof(SGG_Mat4)) == 0){
                        if(sc3_mesh_bone__.ind == sc3_mesh_bone.ind){
                            check__ = true;
                            break;
                        }
                    }
                }
                if(!check__) sc3_bone_map__[idx_bone_map__++] = sc3_mesh_bone__;
            }        
        
        
        
            //printf("[%d]", sc3_mesh_bone__.bone_ind);
        } 
    }
    for(sc_size i = 0; i < sc3_main->meshes_num; ++i){
    Speed_Charm_3D_Mesh *ms = &sc3_main->sc_meshes[i];
    Speed_Charm_3D_Bone_Map_Cache *sk = &bone_list[i];

    if(!(ms->mesh_inf & SC3_MAIN_HAVE_SKIN) || !ms->skin_attr) continue;

    for(sc_size v = 0; v < ms->attr_num; ++v){
        Speed_Charm_3D_Skin_Attr *sv = &ms->skin_attr[v];

        for(sc_size t = 0; t < 4; ++t){
            if(!sv->skin_weight[t]) continue;

            sc_ushort local = sv->skin_indices[t];
            if(local >= sk->bone_map_num){
                sv->skin_weight[t] = 0;
                sv->skin_indices[t] = 0;
                continue;
            }
            Speed_Charm_3D_Bone_Map bone_map__ = sk->bones[local];

            for(sc_size h = 0; h < idx_bone_map__; ++h){
                Speed_Charm_3D_Bone_Map global__ = sc3_bone_map__[h];

                    if(memcmp(&global__.inverse_mat, &bone_map__.inverse_mat, sizeof(SGG_Mat4)) == 0){
                        if(global__.ind == bone_map__.ind){
                            sv->skin_indices[t] = (sc_ushort)h;
                            break;
                        }
                    }
                }
            }
        }
    }

    sc3_main->bones_num = idx_bone_map__;
    sc3_main->sc_bones = sc3_bone_map__;
    for(sc_size i = 0; i < sc3_main->meshes_num; ++i){
        s_free(bone_list[i].bones);
    }

    s_free(bone_list);
    ufbx_free_scene(f_scene);

    return sc3_main;
}

void convert_fbx_speed_charm_3d_bones_anim(Speed_Charm_3D_Anim_Deform * sc3_anim_def, Speed_Charm_3D_Anim * sc3_anim, ufbx_anim_stack * fbx_anim_stack, ufbx_node * fbx_node){
    sc3_anim_def->rot = smart_calloc(SGG_Quaternion, sc3_anim->frame_num);
    sc3_anim_def->pos = smart_calloc(SGG_Vector3, sc3_anim->frame_num);
    sc3_anim_def->scale = smart_calloc(SGG_Vector3, sc3_anim->frame_num);
    sc3_anim_def->deform_status = SC3_DEFORM_ROT_STATIC | SC3_DEFORM_POS_STATIC | SC3_DEFORM_SCALE_STATIC; 
    //sc3_anim_def->cpos = sc3_anim_def->crot = sc3_anim_def->cscale = true;
    //bool rot_check = true;
	for (size_t i = 0; i < sc3_anim->frame_num; i++) {
		double time = fbx_anim_stack->time_begin + (double)i / sc3_anim->framerate;
		ufbx_transform transform = ufbx_evaluate_transform(fbx_anim_stack->anim, fbx_node, time);
        convert_fbx_quat_to_sc3_quat(&sc3_anim_def->rot[i], &transform.rotation);
        convert_fbx_vec3_to_sc3_mesh_vec3(&sc3_anim_def->pos[i], &transform.translation);
        convert_fbx_vec3_to_sc3_mesh_vec3(&sc3_anim_def->scale[i], &transform.scale);
        
		if (i > 0) {
			float dot = sgg_dot_quat(&sc3_anim_def->rot[i], &sc3_anim_def->rot[i - 1]);
            if (dot < 0.0f) sgg_neg_quaternion(&sc3_anim_def->rot[i]);
            
            bool qual_check = sgg_equal_quaternion(&sc3_anim_def->rot[i], &sc3_anim_def->rot[i - 1]);
			if (!qual_check) sc3_anim_def->deform_status &= ~(SC3_DEFORM_ROT_STATIC);
            qual_check =  sgg_equal_vector3(&sc3_anim_def->pos[i], &sc3_anim_def->pos[i - 1]);
			if (!qual_check) sc3_anim_def->deform_status &= ~(SC3_DEFORM_POS_STATIC);
            qual_check =  sgg_equal_vector3(&sc3_anim_def->scale[i], &sc3_anim_def->scale[i - 1]);
			if (!qual_check) sc3_anim_def->deform_status &= ~(SC3_DEFORM_SCALE_STATIC);
		}
	}
    
	if (sc3_anim_def->deform_status & SC3_DEFORM_ROT_STATIC) { 
        SGG_Quaternion * static_rot = smart_realloc(SGG_Quaternion, sc3_anim_def->rot, 1);
        if(static_rot) sc3_anim_def->rot = static_rot;
    }
	if (sc3_anim_def->deform_status & SC3_DEFORM_POS_STATIC) { 
        SGG_Vector3 * static_pos = smart_realloc(SGG_Vector3, sc3_anim_def->pos, 1);
        if(static_pos) sc3_anim_def->pos = static_pos;
    }
	if (sc3_anim_def->deform_status & SC3_DEFORM_SCALE_STATIC) { 
        SGG_Vector3 * static_scale = smart_realloc(SGG_Vector3, sc3_anim_def->scale, 1);
        if(static_scale) sc3_anim_def->scale = static_scale;
    }
}

void convert_fbx_speed_charm_3d_anim(Speed_Charm_3D_Anim * sc3_anim, ufbx_node ** fbx_node, sc_size node_size, ufbx_anim_stack * fbx_anim){
    const float target_framerate = FBX_NORMAL_FRAMERATE;
    const size_t max_frames = FBX_MAX_FRAMERATE;
	float duration = (float)fbx_anim->time_end - (float)fbx_anim->time_begin;
    size_t frames_num = sgg_clampu((size_t)(duration * target_framerate), 2, max_frames);
    float framerate = (float)(frames_num - 1) / duration;
    sc3_anim->time_beg = (float)fbx_anim->time_begin;
    sc3_anim->time_end = (float)fbx_anim->time_end;
    sc3_anim->framerate = framerate;
    sc3_anim->frame_num = frames_num;
    sc3_anim->anim_deforms = smart_calloc(Speed_Charm_3D_Anim_Deform, node_size);
    for(sc_size i = 0; i < node_size; ++i){
        convert_fbx_speed_charm_3d_bones_anim(&sc3_anim->anim_deforms[i], sc3_anim, fbx_anim, fbx_node[i]);
    }
}

void convert_fbx_speed_charm_3d_mesh(Speed_Charm_3D_Mesh * sc3_mesh, Ufbx_Materials_Extends_List * ufbx_extends_lists, Speed_Charm_3D_Bone_Map_Cache * bone_list, ufbx_mesh * fbx_mesh){
    sc_uint f_tri_num = 0;
    sc_uint f_parts_num = 0;
    count_fbx_mesh_part(fbx_mesh, &f_tri_num, &f_parts_num);
    Speed_Charm_3D_Skin_Attr * mesh_skin_attribute = smart_calloc(Speed_Charm_3D_Skin_Attr, fbx_mesh->num_vertices);
    ufbx_skin_deformer *skin_deformer = NULL;
    if(fbx_mesh->skin_deformers.count > 0){
        sc3_mesh->mesh_inf |= SC3_MAIN_HAVE_SKIN;

        assert(fbx_mesh->skin_deformers.count <= 1);
        skin_deformer = fbx_mesh->skin_deformers.data[0];
        for(sc_size v_idx = 0; v_idx < fbx_mesh->num_vertices; ++v_idx){
            sc_size weight_size = 0;
            float total_weight = 0.0f;
            float weights[4] = {0.0f};
            sc_ushort clusters[4] = {0};
            ufbx_skin_vertex mesh_skin_vertex = skin_deformer->vertices.data[v_idx];
            for(size_t w_idx = 0; w_idx < mesh_skin_vertex.num_weights; ++w_idx){
                if(weight_size >= 4){
                    break;
                }
                ufbx_skin_weight skin_weight = skin_deformer->weights.data[mesh_skin_vertex.weight_begin + w_idx];
                total_weight += (float)skin_weight.weight;
                clusters[weight_size] = (sc_ushort)skin_weight.cluster_index;
                weights[weight_size] = (float)skin_weight.weight;
                weight_size++;
            }
            if(total_weight > 0.0f){
                Speed_Charm_3D_Skin_Attr * now_skin_attribute = &mesh_skin_attribute[v_idx];
                sc_uint quantized_sum = 0;
                for(sc_uint i = 0; i < 4; ++i){
                    sc_ubyte quantized_weight = (sc_ubyte)((float)weights[i] / total_weight * 255.0f);
                    quantized_sum += quantized_weight;
                    now_skin_attribute->skin_indices[i] = clusters[i];
                    now_skin_attribute->skin_weight[i] = quantized_weight;
                }
                if (quantized_sum > 255) quantized_sum = 255;
                now_skin_attribute->skin_weight[0] += (sc_ubyte)(255 - quantized_sum);

            }
        }
    }
    convert_fbx_attribute_to_sc3_attribute(sc3_mesh, ufbx_extends_lists, bone_list, fbx_mesh, skin_deformer, f_tri_num, mesh_skin_attribute);
    /*
    if(skin_deformer){
        sc_size node_size = skin_deformer->clusters.count;
        for(sc_size i = 0; i < node_size; ++i){
            ufbx_node * nodes = skin_deformer->clusters.data[i]->bone_node;
            bool d_check = false;
            for(sc_size j = 0; j < *bone_num; ++j){
                if(nodes == (*node_list)[j]){
                    d_check = true;
                    break;
                }
            }
            if(d_check) continue;
            if(*bone_num >= *bone_size){
                ufbx_node ** __node = smart_realloc(ufbx_node * , *node_list, (*bone_num+ 10));
                if(!__node) {
                    s_free(mesh_skin_attribute);
                    return;    
                }
                *node_list = __node;
                *bone_size = *bone_num + 10;
            }
            (*node_list)[*bone_num] = nodes;
            (*bone_num)++;
        }
    }
    */
    s_free(mesh_skin_attribute);
}
static uint32_t append_ufbx_extends_textures(Ufbx_Materials_Extends_List * ufbx_extends_lists, ufbx_texture * new_textures){
    if(ufbx_extends_lists->materials_idx >= ufbx_extends_lists->materials_num){
        Ufbx_Material_Extends_List_ * n = smart_realloc(Ufbx_Material_Extends_List_, ufbx_extends_lists->materials, ufbx_extends_lists->materials_num * 2);
        if(!n) return 0;
        ufbx_extends_lists->materials = n;
        ufbx_extends_lists->materials_num *= 2;
    }
    for(uint32_t i = 0; i < ufbx_extends_lists->materials_idx; ++i){
        if(ufbx_extends_lists->materials[i].status != 0) continue;
        if(ufbx_extends_lists->materials[i].texs == new_textures) return i + 1;
        if (ufbx_extends_lists->materials[i].texs &&
            new_textures &&
            ufbx_extends_lists->materials[i].texs->filename.data &&
            new_textures->filename.data &&
            strcmp(ufbx_extends_lists->materials[i].texs->filename.data,
                new_textures->filename.data) == 0) return i + 1;
    }

    ufbx_extends_lists->materials[ufbx_extends_lists->materials_idx].texs = new_textures;
    ufbx_extends_lists->materials[ufbx_extends_lists->materials_idx].status = 0;
    uint32_t r = ufbx_extends_lists->materials_idx++;
    return r + 1;
}
static uint32_t append_ufbx_extends_vec(Ufbx_Materials_Extends_List * ufbx_extends_lists, ufbx_real new_vec){
    if(ufbx_extends_lists->materials_idx >= ufbx_extends_lists->materials_num){
        Ufbx_Material_Extends_List_ * n = smart_realloc(Ufbx_Material_Extends_List_, ufbx_extends_lists->materials, ufbx_extends_lists->materials_num * 2);
        if(!n) return 0;
        ufbx_extends_lists->materials = n;
        ufbx_extends_lists->materials_num *= 2;
    }
    for(uint32_t i = 0; i < ufbx_extends_lists->materials_idx; ++i){
        if(ufbx_extends_lists->materials[i].status != 1) continue;
        if(
            ufbx_extends_lists->materials[i].v == new_vec
        ) return i + 1;
    }

    ufbx_extends_lists->materials[ufbx_extends_lists->materials_idx].v = new_vec;
    ufbx_extends_lists->materials[ufbx_extends_lists->materials_idx].status = 1;
    uint32_t r = ufbx_extends_lists->materials_idx++;
    return r + 1;
}
static uint32_t append_ufbx_extends_vec2(Ufbx_Materials_Extends_List * ufbx_extends_lists, ufbx_vec2 new_vec){
    if(ufbx_extends_lists->materials_idx >= ufbx_extends_lists->materials_num){
        Ufbx_Material_Extends_List_ * n = smart_realloc(Ufbx_Material_Extends_List_, ufbx_extends_lists->materials, ufbx_extends_lists->materials_num * 2);
        if(!n) return 0;
        ufbx_extends_lists->materials = n;
        ufbx_extends_lists->materials_num *= 2;
    }
    for(uint32_t i = 0; i < ufbx_extends_lists->materials_idx; ++i){
        if(ufbx_extends_lists->materials[i].status != 2) continue;
        if(
            ufbx_extends_lists->materials[i].v2.x == new_vec.x &&
            ufbx_extends_lists->materials[i].v2.y == new_vec.y
        ) return i + 1;
    }

    ufbx_extends_lists->materials[ufbx_extends_lists->materials_idx].v2 = new_vec;
    ufbx_extends_lists->materials[ufbx_extends_lists->materials_idx].status = 2;
    uint32_t r = ufbx_extends_lists->materials_idx++;
    return r + 1;
}
static uint32_t append_ufbx_extends_vec3(Ufbx_Materials_Extends_List * ufbx_extends_lists, ufbx_vec3 new_vec){
    if(ufbx_extends_lists->materials_idx >= ufbx_extends_lists->materials_num){
        Ufbx_Material_Extends_List_ * n = smart_realloc(Ufbx_Material_Extends_List_, ufbx_extends_lists->materials, ufbx_extends_lists->materials_num * 2);
        if(!n) return 0;
        ufbx_extends_lists->materials = n;
        ufbx_extends_lists->materials_num *= 2;
    }
    for(uint32_t i = 0; i < ufbx_extends_lists->materials_idx; ++i){
        if(ufbx_extends_lists->materials[i].status != 3) continue;
        if(ufbx_extends_lists->materials[i].v3.x == new_vec.x &&
        ufbx_extends_lists->materials[i].v3.y == new_vec.y &&
        ufbx_extends_lists->materials[i].v3.z == new_vec.z 
        ) return i + 1;
    }

    ufbx_extends_lists->materials[ufbx_extends_lists->materials_idx].v3 = new_vec;
    ufbx_extends_lists->materials[ufbx_extends_lists->materials_idx].status = 3;
    uint32_t r = ufbx_extends_lists->materials_idx++;
    return r + 1;
}
static uint32_t append_ufbx_extends_vec4(Ufbx_Materials_Extends_List * ufbx_extends_lists, ufbx_vec4 new_vec){
    if(ufbx_extends_lists->materials_idx >= ufbx_extends_lists->materials_num){
        Ufbx_Material_Extends_List_ * n = smart_realloc(Ufbx_Material_Extends_List_, ufbx_extends_lists->materials, ufbx_extends_lists->materials_num * 2);
        if(!n) return 0;
        ufbx_extends_lists->materials = n;
        ufbx_extends_lists->materials_num *= 2;
        
    }
    for(uint32_t i = 0; i < ufbx_extends_lists->materials_idx; ++i){
        if(ufbx_extends_lists->materials[i].status != 4) continue;
        if(ufbx_extends_lists->materials[i].v4.x == new_vec.x &&
        ufbx_extends_lists->materials[i].v4.y == new_vec.y &&
        ufbx_extends_lists->materials[i].v4.z == new_vec.z &&
        ufbx_extends_lists->materials[i].v4.w == new_vec.w
        ) return i + 1;
    }

    ufbx_extends_lists->materials[ufbx_extends_lists->materials_idx].v4 = new_vec;
    ufbx_extends_lists->materials[ufbx_extends_lists->materials_idx].status = 4;
    uint32_t r = ufbx_extends_lists->materials_idx++;
    return r + 1;
}

void convert_fbx_attribute_to_sc3_attribute(Speed_Charm_3D_Mesh * sc3_mesh, Ufbx_Materials_Extends_List * ufbx_extends_list, Speed_Charm_3D_Bone_Map_Cache * bone_map, ufbx_mesh * fbx_mesh, ufbx_skin_deformer * fbx_skin, sc_uint fbx_triangle_num, Speed_Charm_3D_Skin_Attr * skin_attribute){
    sc_uint * mesh_indices = smart_calloc(sc_uint, fbx_mesh->max_face_triangles * DIMENSION_XYZ);
    sc_uint * mesh_triangle_indices = smart_calloc(sc_uint, fbx_triangle_num * DIMENSION_XYZ);
    if(!mesh_indices)return;
    if(!mesh_triangle_indices)return;
    Speed_Charm_3D_Skin_Attr * sc3_skin_attr = smart_calloc(Speed_Charm_3D_Skin_Attr, fbx_triangle_num * fbx_mesh->material_parts.count * DIMENSION_XYZ);
    Speed_Charm_3D_Mesh_Attr * sc3_mesh_attr = smart_calloc(Speed_Charm_3D_Mesh_Attr, fbx_triangle_num * fbx_mesh->material_parts.count * DIMENSION_XYZ);
    printf("debug name %s\n", fbx_mesh->name.data);
    if(fbx_mesh->materials.count){
        ufbx_material_pbr_maps fbx_pbr = fbx_mesh->materials.data[0]->pbr;
        
        if(fbx_pbr.base_color.texture_enabled){

            uint32_t index = append_ufbx_extends_textures(ufbx_extends_list, fbx_pbr.base_color.texture);
            sc3_mesh->material_point.albedo_idx = index;
        }
        else if(fbx_pbr.base_color.has_value){

            uint32_t index = 0;
            switch(fbx_pbr.base_color.value_components){
                case 0:
                sc3_mesh->material_point.albedo_idx = 0; 
                break;
                case 1: 
                index = append_ufbx_extends_vec(ufbx_extends_list, fbx_pbr.base_color.value_real);
                sc3_mesh->material_point.albedo_idx = index;
                break;
                case 2:
                index = append_ufbx_extends_vec2(ufbx_extends_list, fbx_pbr.base_color.value_vec2);
                sc3_mesh->material_point.albedo_idx = index;
                break;
                case 3:
                index = append_ufbx_extends_vec3(ufbx_extends_list, fbx_pbr.base_color.value_vec3);
                sc3_mesh->material_point.albedo_idx = index;
                break;
                case 4:
                index = append_ufbx_extends_vec4(ufbx_extends_list, fbx_pbr.base_color.value_vec4);
                sc3_mesh->material_point.albedo_idx = index;
                break;
            }

        }
        if(fbx_pbr.normal_map.texture_enabled){
            
            uint32_t index = append_ufbx_extends_textures(ufbx_extends_list, fbx_pbr.normal_map.texture);
            sc3_mesh->material_point.normal_idx = index;
        }
        else sc3_mesh->material_point.normal_idx = 0;
        if(fbx_pbr.roughness.texture_enabled){
            printf("ROUGHNESS TEX\n");

            uint32_t index = append_ufbx_extends_textures(ufbx_extends_list, fbx_pbr.roughness.texture);
            sc3_mesh->material_point.roughness_idx = index;
        }
        else if(fbx_pbr.roughness.has_value){
            uint32_t index;
            switch(fbx_pbr.roughness.value_components){
                case 0:
                sc3_mesh->material_point.roughness_idx = 0; 
                break;
                case 1: 
                index = append_ufbx_extends_vec(ufbx_extends_list, fbx_pbr.roughness.value_real);
                printf("ROUGHNESS %lf\n", fbx_pbr.roughness.value_real);
                sc3_mesh->material_point.roughness_idx = index;
                break;
                case 2:
                index = append_ufbx_extends_vec(ufbx_extends_list, fbx_pbr.roughness.value_vec2.x);
                sc3_mesh->material_point.roughness_idx = index;
                break;
                case 3:
                index = append_ufbx_extends_vec(ufbx_extends_list, fbx_pbr.roughness.value_vec3.x);
                sc3_mesh->material_point.roughness_idx = index;
                break;
                case 4:
                index = append_ufbx_extends_vec(ufbx_extends_list, fbx_pbr.roughness.value_vec4.x);
                sc3_mesh->material_point.roughness_idx = index;
                break;
            }

        }
        if(fbx_pbr.ambient_occlusion.texture_enabled){
            printf("AO TEX\n");

            uint32_t index = append_ufbx_extends_textures(ufbx_extends_list, fbx_pbr.ambient_occlusion.texture);
            sc3_mesh->material_point.ao_idx = index;
        }
        else if(fbx_pbr.ambient_occlusion.has_value){
            uint32_t index;

            switch(fbx_pbr.ambient_occlusion.value_components){
                case 0:
                sc3_mesh->material_point.ao_idx = 0; 
                break;
                case 1: 
                index = append_ufbx_extends_vec(ufbx_extends_list, fbx_pbr.ambient_occlusion.value_real);
                printf("AO %lf\n", fbx_pbr.roughness.value_real);

                sc3_mesh->material_point.ao_idx = index;
                break;
                case 2:
                index = append_ufbx_extends_vec(ufbx_extends_list, fbx_pbr.ambient_occlusion.value_vec2.x);
                sc3_mesh->material_point.ao_idx = index;
                break;
                case 3:
                index = append_ufbx_extends_vec(ufbx_extends_list, fbx_pbr.ambient_occlusion.value_vec3.x);
                sc3_mesh->material_point.ao_idx = index;
                break;
                case 4:
                index = append_ufbx_extends_vec(ufbx_extends_list, fbx_pbr.ambient_occlusion.value_vec4.x);
                sc3_mesh->material_point.ao_idx = index;
                break;
            }

        }
        if(fbx_pbr.metalness.texture_enabled){
            printf("METALLIC TEX\n");

            uint32_t index = append_ufbx_extends_textures(ufbx_extends_list, fbx_pbr.metalness.texture);
            sc3_mesh->material_point.metallic_idx = index;
        }
        else if(fbx_pbr.metalness.has_value){
            printf("HAS METALLIC\n");
            uint32_t index;
            switch(fbx_pbr.metalness.value_components){
                case 0:
                sc3_mesh->material_point.metallic_idx = 0; 
                break;
                case 1: 
                    printf("METALLIC %lf\n", fbx_pbr.metalness.value_real);

                index = append_ufbx_extends_vec(ufbx_extends_list, fbx_pbr.metalness.value_real);
                sc3_mesh->material_point.metallic_idx = index;
                break;
                case 2:
                index = append_ufbx_extends_vec(ufbx_extends_list, fbx_pbr.metalness.value_vec2.x);
                sc3_mesh->material_point.metallic_idx = index;
                break;
                case 3:
                index = append_ufbx_extends_vec(ufbx_extends_list, fbx_pbr.metalness.value_vec3.x);
                sc3_mesh->material_point.metallic_idx = index;
                break;
                case 4:
                index = append_ufbx_extends_vec(ufbx_extends_list, fbx_pbr.metalness.value_vec4.x);
                sc3_mesh->material_point.metallic_idx = index;
                break;
            }

        }

        else {
            tmp_printf("NOT HAS METALLLIC\n");
        }
    }
    for(sc_size mp_idx = 0; mp_idx < fbx_mesh->material_parts.count; ++mp_idx){

        ufbx_mesh_part * mesh_part = &fbx_mesh->material_parts.data[mp_idx];
        if(mesh_part->num_triangles == 0)continue;
        sc_uint atr_idx = 0;
        for(sc_size f_idx = 0; f_idx < mesh_part->num_faces; ++f_idx){
            
            ufbx_face mesh_face = fbx_mesh->faces.data[mesh_part->face_indices.data[f_idx]];
            sc_uint triangle_face_num = ufbx_triangulate_face(mesh_indices, fbx_mesh->max_face_triangles * DIMENSION_XYZ, fbx_mesh, mesh_face) * DIMENSION_XYZ;
            
            ufbx_vec2 deform_tri_uv = {0.0f};
            
            for(sc_uint ta_idx = 0; ta_idx < triangle_face_num; ++ta_idx){

                sc_uint tmp_indices_num = mesh_indices[ta_idx];
                ufbx_vec3 tri_vert = ufbx_get_vertex_vec3(&fbx_mesh->vertex_position, tmp_indices_num);
                ufbx_vec3 tri_norm = ufbx_get_vertex_vec3(&fbx_mesh->vertex_normal, tmp_indices_num);
                ufbx_vec2 tri_uv = fbx_mesh->vertex_uv.exists ? ufbx_get_vertex_vec2(&fbx_mesh->vertex_uv, tmp_indices_num) : deform_tri_uv;
                sc_size atr_idx_t = sc3_mesh->attr_num + atr_idx;
                sc3_mesh_attr[atr_idx_t].pos.x = tri_vert.x;
                sc3_mesh_attr[atr_idx_t].pos.y = tri_vert.y;
                sc3_mesh_attr[atr_idx_t].pos.z = tri_vert.z;
                sc3_mesh_attr[atr_idx_t].norm.x = tri_norm.x;
                sc3_mesh_attr[atr_idx_t].norm.y = tri_norm.y;   
                sc3_mesh_attr[atr_idx_t].norm.z = tri_norm.z;
                sc3_mesh_attr[atr_idx_t].uv.x = tri_uv.x;
                sc3_mesh_attr[atr_idx_t].uv.y = tri_uv.y;
                if(fbx_skin) sc3_skin_attr[atr_idx_t] = skin_attribute[fbx_mesh->vertex_indices.data[tmp_indices_num]];
                atr_idx++;
            }   
        }
        sc3_mesh->attr_num += atr_idx;
    }

    ufbx_vertex_stream v_stream[2];        
    size_t v_stream_num = 1;
    v_stream[0].data = sc3_mesh_attr;
    v_stream[0].vertex_count = sc3_mesh->attr_num;
    v_stream[0].vertex_size = sizeof(float) * 8;
    if(fbx_skin){
        v_stream_num = 2;
        v_stream[1].data = sc3_skin_attr;
        v_stream[1].vertex_count = sc3_mesh->attr_num;
        v_stream[1].vertex_size = sizeof(Speed_Charm_3D_Skin_Attr);
    }
    ufbx_error fbx_error;
    sc3_mesh->ind_num = sc3_mesh->attr_num;
    sc3_mesh->attr_num = ufbx_generate_indices(v_stream, v_stream_num, mesh_triangle_indices, sc3_mesh->ind_num, NULL, &fbx_error);
    if(fbx_error.type != UFBX_ERROR_NONE) return;
    sc3_mesh->mesh_attr = smart_calloc(Speed_Charm_3D_Mesh_Attr, sc3_mesh->attr_num);
    sc3_mesh->indices   = smart_calloc(sc_uint, sc3_mesh->ind_num);
    if(fbx_skin) sc3_mesh->skin_attr = smart_calloc(Speed_Charm_3D_Skin_Attr, sc3_mesh->attr_num);
    for(sc_size i = 0; i < sc3_mesh->attr_num; ++i){
        sc3_mesh->mesh_attr[i]              = sc3_mesh_attr[i];
        if(fbx_skin) sc3_mesh->skin_attr[i] = sc3_skin_attr[i];
    }
    for(sc_size i = 0; i < sc3_mesh->ind_num; ++i){
        sc3_mesh->indices[i] = mesh_triangle_indices[i];
    }
    
    if(fbx_skin){ 

        const sc_uint num_clusters = (sc_uint)fbx_skin->clusters.count;

        bool *weight_have_map = smart_calloc(bool, num_clusters);
        if(!weight_have_map)return;
        for (sc_uint i = 0; i < sc3_mesh->attr_num; ++i) {
            const Speed_Charm_3D_Skin_Attr *sv = &sc3_mesh->skin_attr[i];
            for (int j = 0; j < 4; ++j) {
                if (!sv->skin_weight[j]) continue;
                sc_ushort g = sv->skin_indices[j];                 
                if (g < num_clusters) weight_have_map[g] = true;
            }
        }
        sc_ushort *g2l = smart_calloc(sc_ushort, num_clusters);
        if(!g2l){
            free(weight_have_map);
        }
        /*init max uint16_t*/
        for (sc_uint g = 0; g < num_clusters; ++g) g2l[g] = 0xFFFF;
        bone_map->bones = smart_calloc(Speed_Charm_3D_Bone_Map ,num_clusters);
        if(!bone_map->bones){
            free(weight_have_map);
            free(g2l);
            return;
        }
        /*ps is fix map size and index*/
        sc_ushort ps = 0;
        /*marking is_valid weight cluster*/
        for (sc_uint g = 0; g < num_clusters; ++g) {
            if (!weight_have_map[g]) continue;
            g2l[g] = ps;
            ufbx_skin_cluster *cl = fbx_skin->clusters.data[g];
            bone_map->bones[ps].ind = (sc_uint)cl->bone_node->typed_id;
            
            convert_fbx_mat_to_sc3_mesh_mat(&bone_map->bones[ps].inverse_mat, &cl->geometry_to_bone);
            ++ps;
        }

        bone_map->bone_map_num = ps;

        Speed_Charm_3D_Bone_Map * tmp_map = smart_realloc(Speed_Charm_3D_Bone_Map, bone_map->bones, ps);
        if(tmp_map) bone_map->bones = tmp_map;
        for (sc_err_t i = 0; i < sc3_mesh->attr_num; ++i) {
            Speed_Charm_3D_Skin_Attr * sv = &sc3_mesh->skin_attr[i];

            float    wf[4] = {0.0f};
            sc_err_t li[4] = {0};
            int      n = 0;
            float    sum = 0.0f;

            for (int j = 0; j < 4; ++j) {
                if (!sv->skin_weight[j]) continue;
                sc_short g = sv->skin_indices[j];                 
                if (g >= num_clusters) continue;
                sc_short l = g2l[g];                       
                if (l == 0xFFFF) continue;                 
                li[n] = l;
                wf[n] = sv->skin_weight[j] / 255.0f;
                sum  += wf[n];
                ++n; 
            }
            if (sum > 0.0f) {
                for (int t = 0; t < n; ++t) wf[t] /= sum;
            }
            sc_uint qsum = 0;
            for (int t = 0; t < 4; ++t) {
                sc_ubyte q = (t < n) ? (sc_ubyte)(wf[t] * 255.0f) : 0;
                sv->skin_indices[t]  = (t < n) ? li[t] : 0;
                sv->skin_weight[t] = q;
                qsum += q;
            }
            if(n > 0)
            sv->skin_weight[0] += (sc_ubyte)(255 - (qsum & 0xFF));
        }
    }
    
    s_free(sc3_mesh_attr);
    s_free(sc3_skin_attr);
    s_free(mesh_triangle_indices);
    s_free(mesh_indices);
}

#if TEST


if(fbx_skin){ 

    const sc_uint num_clusters = (sc_uint)fbx_skin->clusters.count;

    bool *weight_have_map = smart_calloc(bool, num_clusters);
    if(!weight_have_map)return;
    for (sc_uint i = 0; i < sc3_mesh->attr_num; ++i) {
        const Speed_Charm_3D_Skin_Attr *sv = &sc3_mesh->skin_attr[i];
        for (int j = 0; j < 4; ++j) {
            if (!sv->skin_weight[j]) continue;
            sc_ushort g = sv->skin_indices[j];                 
            if (g < num_clusters) weight_have_map[g] = true;
        }
    }
    sc_ushort *g2l = smart_calloc(sc_ushort, num_clusters);
    if(!g2l){
        free(weight_have_map);
    }
    /*init max uint16_t*/
    for (sc_uint g = 0; g < num_clusters; ++g) g2l[g] = 0xFFFF;
    sc3_mesh->tttt = smart_calloc(Speed_Charm_3D_Bone_Map_TEST ,num_clusters);
    if(!sc3_mesh->tttt){
        free(weight_have_map);
        free(g2l);
        return;
    }
    /*ps is fix map size and index*/
    sc_ushort ps = 0;
    /*marking is_valid weight cluster*/
    for (sc_uint g = 0; g < num_clusters; ++g) {
        if (!weight_have_map[g]) continue;
        g2l[g] = ps;
        ufbx_skin_cluster *cl = fbx_skin->clusters.data[g];
        sc3_mesh->tttt[ps].bone_ind = (sc_uint)cl->bone_node->typed_id;
        convert_fbx_mat_to_sc3_mesh_mat(&sc3_mesh->tttt[ps].bone_map, &cl->geometry_to_bone);
        ++ps;
    }
    
    sc3_mesh->ttttSIZE = ps;

    Speed_Charm_3D_Bone_Map_TEST * tmp_map_indices = smart_realloc(Speed_Charm_3D_Bone_Map_TEST, sc3_mesh->tttt, ps);
    if(tmp_map_indices) sc3_mesh->tttt = tmp_map_indices;
    for (sc_err_t i = 0; i < sc3_mesh->attr_num; ++i) {
        Speed_Charm_3D_Skin_Attr * sv = &sc3_mesh->skin_attr[i];

        float    wf[4] = {0.0f};
        sc_err_t li[4] = {0};
        int      n = 0;
        float    sum = 0.0f;

        for (int j = 0; j < 4; ++j) {
            if (!sv->skin_weight[j]) continue;
            sc_short g = sv->skin_indices[j];                 
            if (g >= num_clusters) continue;
            sc_short l = g2l[g];                       
            if (l == 0xFFFF) continue;                 
            li[n] = l;
            wf[n] = sv->skin_weight[j] / 255.0f;
            sum  += wf[n];
            ++n; 
        }
        if (sum > 0.0f) {
            for (int t = 0; t < n; ++t) wf[t] /= sum;
        }
        sc_uint qsum = 0;
        for (int t = 0; t < 4; ++t) {
            sc_ubyte q = (t < n) ? (sc_ubyte)(wf[t] * 255.0f) : 0;
            sv->skin_indices[t]  = (t < n) ? li[t] : 0;
            sv->skin_weight[t] = q;
            qsum += q;
        }
        if(n > 0)
        sv->skin_weight[0] += (sc_ubyte)(255 - (qsum & 0xFF));
    }
    }
    
    #endif