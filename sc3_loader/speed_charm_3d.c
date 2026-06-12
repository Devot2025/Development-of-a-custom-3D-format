#include "speed_charm_3d.h"

static sc_err_t delete_speed_charm_3d_meshes(Speed_Charm_3D_Mesh * sc3_meshes, sc_size mesh_size){
    if(!sc3_meshes) return SC3_FILE_OUT_CODE;
    for(sc_size i = 0; i < mesh_size; ++i){
        s_free((*(sc3_meshes + i)).indices);
        s_free((*(sc3_meshes + i)).mesh_attr);
        s_free((*(sc3_meshes + i)).skin_attr);
        //s_free(__sc3_main->sc_meshes[i].mesh_bone_map);
    }
    return SC3_FILE_OUT_CODE;
}
static sc_err_t delete_speed_charm_3d_instance_group(Speed_Charm_3D_Instance_Group* sc3_ins_group, sc_size group_size) {
    if (!sc3_ins_group) return SC3_FILE_OUT_CODE;
    for (sc_size i = 0; i < group_size; ++i) {
        s_free((*(sc3_ins_group + i)).trs);
    }
    return SC3_FILE_OUT_CODE;
}

static sc_err_t delete_speed_charm_3d_instance(Speed_Charm_3D_Instance* sc3_ins, sc_size ins_size) {
    if (!sc3_ins) return SC3_FILE_OUT_CODE;
    for (sc_size i = 0; i < ins_size; ++i) {
        delete_speed_charm_3d_instance_group((*(sc3_ins + i)).group, (*(sc3_ins + i)).group_num);
        s_free((*(sc3_ins + i)).group);
    }

    return SC3_FILE_OUT_CODE;
}
static sc_err_t delete_speed_charm_3d_anim_deform(Speed_Charm_3D_Anim_Deform  * sc3_anim_def, sc_size skeletons_size){
    if(!sc3_anim_def) return SC3_FILE_OUT_CODE;
    for(sc_size i = 0; i < skeletons_size; ++i){
        s_free((*(sc3_anim_def + i)).pos);
        s_free((*(sc3_anim_def + i)).rot);
        s_free((*(sc3_anim_def + i)).scale);
        
    }
    return SC3_FILE_OUT_CODE;
}
static sc_err_t delete_speed_charm_3d_anim(Speed_Charm_3D_Anim * sc3_anim, sc_size anim_size, sc_size skeletons_size){
    if(!sc3_anim) return SC3_FILE_OUT_CODE;
    for(sc_size i = 0; i < anim_size; ++i){
        delete_speed_charm_3d_anim_deform((*(sc3_anim + i)).anim_deforms, skeletons_size);
        s_free((*(sc3_anim + i)).anim_deforms);
    }
    return SC3_FILE_OUT_CODE;
}
static sc_err_t delete_speed_charm_3d_material_tex(Speed_Charm_3D_Material_Buffers * sc3_tex, sc_size material_tex_size){
    if(!sc3_tex) return SC3_FILE_OUT_CODE;
    for(sc_size i = 0; i < material_tex_size; ++i){
        if(sc3_tex->sc_has_material == SC3_HAS_TEXTURES) s_free(sc3_tex->material_map.img_data);
    }
    return SC3_FILE_OUT_CODE;
}
sc_err_t delete_speed_charm_3d_main(Speed_Charm_3D_Main ** sc3_main){
    if(!sc3_main) return SC3_FILE_OUT_CODE;
    Speed_Charm_3D_Main* sc3_main__ = *sc3_main;
    if(!sc3_main__) return SC3_FILE_OUT_CODE;
    delete_speed_charm_3d_meshes(sc3_main__->sc_meshes, sc3_main__->meshes_num);
    delete_speed_charm_3d_anim(sc3_main__->sc_anim, sc3_main__->anim_num, sc3_main__->skeletons_num);
    delete_speed_charm_3d_material_tex(sc3_main__->sc_materials, sc3_main__->materials_num);
    delete_speed_charm_3d_instance(sc3_main__->sc_ins, sc3_main__->instance_num);
    s_free(sc3_main__->sc_bones);
    s_free(sc3_main__->sc_meshes);
    s_free(sc3_main__->sc_anim);
    s_free(sc3_main__->sc_ins);
    s_free(sc3_main__->sc_skeletons);
    s_free(sc3_main__->sc_materials);
    s_free(sc3_main__);
    *sc3_main = NULL;
    return SC3_FILE_OUT_CODE;
}

sc_err_t delete_speed_charm_3d_main_contents(Speed_Charm_3D_Main_Contents ** sc3_main_cont){
    if(!sc3_main_cont) return SC3_FILE_OUT_CODE;
    if(!*sc3_main_cont) return SC3_FILE_OUT_CODE;
    
    delete_speed_charm_3d_main(&(*sc3_main_cont)->sc3_main);
    s_free(*sc3_main_cont);
    return SC3_FILE_OUT_CODE;
}

sc_err_t delete_speed_charm_3d_run_main(Speed_Charm_3D_Run_Main ** sc3_run_main, free_vvp_api_funcs s_free_vvp, free_vmp_api_funcs s_free_vmp){
    /*specialty error.*/
    if(!s_free_vvp) return SC3_FREE_FUNC_API_NONE;
    if(!s_free_vmp) return SC3_FREE_FUNC_API_NONE;

    if(!sc3_run_main) return SC3_FILE_OUT_CODE;
    Speed_Charm_3D_Run_Main* sc3_run_main__ = *sc3_run_main;
    if(!sc3_run_main__) return SC3_FILE_OUT_CODE;
    delete_speed_charm_3d_anim(sc3_run_main__->sc_anim, sc3_run_main__->anim_num, sc3_run_main__->skeletons_num);
    delete_speed_charm_3d_instance(sc3_run_main__->sc_ins, sc3_run_main__->instance_num);
    s_free(sc3_run_main__->sc_bones);
    s_free(sc3_run_main__->sc_meshes);
    s_free(sc3_run_main__->sc_anim);
    s_free(sc3_run_main__->sc_skeletons);
    s_free(sc3_run_main__->sc_dynamic_def);
    s_free_vvp(sc3_run_main__->sc_vvp);
    s_free_vmp(sc3_run_main__->sc_vmp);
    s_free(sc3_run_main__);
    return SC3_FILE_OUT_CODE;
}

void set_sc3_file_format(sc_byte * sc3_path){
    if (!sc3_path) return;
    sc_size name_len__ = safety_strlen(sc3_path);
    sc_size format_offset__ = name_len__;
   const sc_byte * sc3_format__ = ".sc3";
   //const sc_size sc3_size__ = STRING_LITERAL_SIZE(".sc3");
   if(SC3_FILE_NAME_DEFAULT_SIZE <= name_len__) name_len__ = SC3_FILE_NAME_DEFAULT_SIZE - SC_POLICY_FILE_FORMAT_MAX;
   if(SC3_FILE_NAME_DEFAULT_SIZE - name_len__ < SC_POLICY_FILE_FORMAT_MAX) name_len__ = SC3_FILE_NAME_DEFAULT_SIZE - SC_POLICY_FILE_FORMAT_MAX;
   format_offset__ = name_len__;
   sc_byte* dst__ = sc3_path + name_len__;

   smart_memcpy_p(dst__, sc3_format__, sc_byte, SC_POLICY_FILE_FORMAT_MAX);
   
}
void create_auto_arm_material(Speed_Charm_3D_Main * sc3_main){
    sc_size new_materials_num = 0;
    for(sc_size i = 0; i < sc3_main->meshes_num; ++i){
        if(!sc3_main->sc_meshes[i].material_point.arm_idx){
            new_materials_num++;
        }
    }

    for(sc_size i = 0; i < sc3_main->meshes_num; ++i){
        if(!sc3_main->sc_meshes[i].material_point.arm_idx){
            sc_size ao_texture_size = 0;
            sc_size roughness_texture_size = 0;
            sc_size metallic_texture_size = 0;
            bool texture_check = false;
            SGG_Vector3 arm_vec = {1.0f, 1.0f, 1.0f};

            sc_size arm_x = 0;
            sc_size arm_y = 0;

            if(sc3_main->sc_meshes[i].material_point.ao_idx){
                Speed_Charm_3D_Material_Buffers * buf = &sc3_main->sc_materials[sc3_main->sc_meshes[i].material_point.ao_idx];
                if(buf->sc_has_material == SC3_HAS_TEXTURES){
                    texture_check = true;
                    arm_x = buf->material_map.x;
                    arm_y = buf->material_map.y;
                    ao_texture_size = buf->material_map.x * buf->material_map.y * SC3_TEXTURE_MATERIAL_CHANNEL;
                }
                else if(buf->sc_has_material == SC3_HAS_VALUES_SCALER){
                    arm_vec.x = buf->material_value;
                    ao_texture_size = sizeof(float);
                }
                else if(buf->sc_has_material == SC3_HAS_VALUES_VECTOR){
                    arm_vec.x = buf->material_value3.x;
                    ao_texture_size = sizeof(float);
                }
                else if(buf->sc_has_material == SC3_HAS_VALUES_VECTOR_ALPHA){
                    arm_vec.x = buf->material_value4.x;
                    ao_texture_size = sizeof(float);
                }
            }

            if(sc3_main->sc_meshes[i].material_point.roughness_idx){
                Speed_Charm_3D_Material_Buffers * buf = &sc3_main->sc_materials[sc3_main->sc_meshes[i].material_point.roughness_idx];
                if(buf->sc_has_material == SC3_HAS_TEXTURES){
                    texture_check = true;
                    if(!arm_x && !arm_y){
                        arm_x = buf->material_map.x;
                        arm_y = buf->material_map.y;
                    }
                    roughness_texture_size = buf->material_map.x * buf->material_map.y * SC3_TEXTURE_MATERIAL_CHANNEL;
                }
                else if(buf->sc_has_material == SC3_HAS_VALUES_SCALER){
                    arm_vec.y = buf->material_value;
                    roughness_texture_size = sizeof(float);
                }
                else if(buf->sc_has_material == SC3_HAS_VALUES_VECTOR){
                    arm_vec.y = buf->material_value3.x;
                    roughness_texture_size = sizeof(float);
                }
                else if(buf->sc_has_material == SC3_HAS_VALUES_VECTOR_ALPHA){
                    arm_vec.y = buf->material_value4.x;
                    roughness_texture_size = sizeof(float);
                }
            }

            if(sc3_main->sc_meshes[i].material_point.metallic_idx){
                Speed_Charm_3D_Material_Buffers * buf = &sc3_main->sc_materials[sc3_main->sc_meshes[i].material_point.metallic_idx];
                if(buf->sc_has_material == SC3_HAS_TEXTURES){
                    texture_check = true;
                    if(!arm_x && !arm_y){
                        arm_x = buf->material_map.x;
                        arm_y = buf->material_map.y;
                    }
                    metallic_texture_size = buf->material_map.x * buf->material_map.y * SC3_TEXTURE_MATERIAL_CHANNEL;
                }
                else if(buf->sc_has_material == SC3_HAS_VALUES_SCALER){
                    arm_vec.z = buf->material_value;
                    metallic_texture_size = sizeof(float);
                }
                else if(buf->sc_has_material == SC3_HAS_VALUES_VECTOR){
                    arm_vec.z = buf->material_value3.x;
                    metallic_texture_size = sizeof(float);
                }
                else if(buf->sc_has_material == SC3_HAS_VALUES_VECTOR_ALPHA){
                    arm_vec.z = buf->material_value4.x;
                    metallic_texture_size = sizeof(float);
                }
            }

            if(texture_check){
                sc_size size = arm_x * arm_y * SC3_TEXTURE_MATERIAL_CHANNEL;
                sc_ubyte * arm = smart_malloc(sc_ubyte, size);
                if (!arm) break;
                for(sc_size j = 0; j < arm_x * arm_y; ++j){
                    sc_ubyte ao_value = (sc_ubyte)(arm_vec.x * 255.0f);
                    sc_ubyte roughness_value = (sc_ubyte)(arm_vec.y * 255.0f);
                    sc_ubyte metallic_value = (sc_ubyte)(arm_vec.z * 255.0f);

                    sc_size arm_px = j % arm_x;/*50 % 200 = 50 x*/
                    sc_size arm_py = j / arm_x;/*50 / 200 = 200 ln*/

                    if(sc3_main->sc_meshes[i].material_point.ao_idx){
                        Speed_Charm_3D_Material_Buffers * buf = &sc3_main->sc_materials[sc3_main->sc_meshes[i].material_point.ao_idx];
                        if(buf->sc_has_material == SC3_HAS_TEXTURES){
                            /*50 * 300 / 200 -> 15000 / 200 = 75*/
                            sc_size src_x = (arm_px * buf->material_map.x) / arm_x;
                            sc_size src_y = (arm_py * buf->material_map.y) / arm_y;
                            sc_size src_idx = (src_y * buf->material_map.x + src_x) * SC3_TEXTURE_MATERIAL_CHANNEL;
                            ao_value = buf->material_map.img_data[src_idx];
                        }
                    }

                    if(sc3_main->sc_meshes[i].material_point.roughness_idx){
                        Speed_Charm_3D_Material_Buffers * buf = &sc3_main->sc_materials[sc3_main->sc_meshes[i].material_point.roughness_idx];
                        if(buf->sc_has_material == SC3_HAS_TEXTURES){
                            sc_size src_x = (arm_px * buf->material_map.x) / arm_x;
                            sc_size src_y = (arm_py * buf->material_map.y) / arm_y;
                            sc_size src_idx = (src_y * buf->material_map.x + src_x) * SC3_TEXTURE_MATERIAL_CHANNEL;
                            roughness_value = buf->material_map.img_data[src_idx];
                        }
                    }

                    if(sc3_main->sc_meshes[i].material_point.metallic_idx){
                        Speed_Charm_3D_Material_Buffers * buf = &sc3_main->sc_materials[sc3_main->sc_meshes[i].material_point.metallic_idx];
                        if(buf->sc_has_material == SC3_HAS_TEXTURES){
                            sc_size src_x = (arm_px * buf->material_map.x) / arm_x;
                            sc_size src_y = (arm_py * buf->material_map.y) / arm_y;
                            sc_size src_idx = (src_y * buf->material_map.x + src_x) * SC3_TEXTURE_MATERIAL_CHANNEL;
                            metallic_value = buf->material_map.img_data[src_idx];
                        }
                    }

                    arm[j * SC3_TEXTURE_MATERIAL_CHANNEL + 0] = ao_value;
                    arm[j * SC3_TEXTURE_MATERIAL_CHANNEL + 1] = roughness_value;
                    arm[j * SC3_TEXTURE_MATERIAL_CHANNEL + 2] = metallic_value;
                    arm[j * SC3_TEXTURE_MATERIAL_CHANNEL + 3] = 0;
                }
            }
            else{
                SGG_Vector3 arm = arm_vec;
            }
        }
    }
}
void set_pbr_external_api_tex_materials(Speed_Charm_3D_Main * sc3_main, sc_size mesh_idx, sc_ushort materials_type_id, const void * new_materials, const sc_size width, const sc_size height){
    sc_ubyte * new_sc_materials = smart_malloc(sc_ubyte, width * height * SC3_TEXTURE_MATERIAL_CHANNEL);
    if(!new_sc_materials) return;
    smart_memcpy_p(new_sc_materials, new_materials, sc_ubyte, width * height * SC3_TEXTURE_MATERIAL_CHANNEL);
    if(sc3_main->meshes_num <= mesh_idx){
        s_free(new_sc_materials);
        return;
    }
    switch(materials_type_id){
        case SC3_MESH_MATERIAL_POINT_ALBEDO:
        case SC3_MESH_MATERIAL_POINT_NORMAL:
        case SC3_MESH_MATERIAL_POINT_ROUGHNESS:
        case SC3_MESH_MATERIAL_POINT_METALLIC:
        case SC3_MESH_MATERIAL_POINT_AO:
        case SC3_MESH_MATERIAL_POINT_ARM:
        case SC3_MESH_MATERIAL_POINT_ALPHA:
        break;
        default: 
        s_free(new_sc_materials);
        return;
    }
    Speed_Charm_3D_Material_Buffers * sc3_material_buffer = smart_realloc(Speed_Charm_3D_Material_Buffers, sc3_main->sc_materials, sc3_main->materials_num + 1);
    if(!sc3_material_buffer) {
        s_free(new_sc_materials);
        return;
    }
    switch(materials_type_id){
        case SC3_MESH_MATERIAL_POINT_ALBEDO:
        
        sc3_main->sc_meshes[mesh_idx].material_point.albedo_idx = sc3_main->materials_num + 1;
        break;
        case SC3_MESH_MATERIAL_POINT_NORMAL:
        sc3_main->sc_meshes[mesh_idx].material_point.normal_idx = sc3_main->materials_num + 1;
        break;
        case SC3_MESH_MATERIAL_POINT_ROUGHNESS:
        sc3_main->sc_meshes[mesh_idx].material_point.roughness_idx = sc3_main->materials_num + 1;
        break;
        case SC3_MESH_MATERIAL_POINT_METALLIC:
        sc3_main->sc_meshes[mesh_idx].material_point.metallic_idx = sc3_main->materials_num + 1;
        break;
        case SC3_MESH_MATERIAL_POINT_AO:
        sc3_main->sc_meshes[mesh_idx].material_point.ao_idx = sc3_main->materials_num + 1;
        break;
        case SC3_MESH_MATERIAL_POINT_ARM:
        sc3_main->sc_meshes[mesh_idx].material_point.arm_idx = sc3_main->materials_num + 1;
        break;
        case SC3_MESH_MATERIAL_POINT_ALPHA:
        sc3_main->sc_meshes[mesh_idx].material_point.alpha_idx = sc3_main->materials_num + 1;
        break;
    }
    sc3_main->sc_materials = sc3_material_buffer;
    sc3_main->sc_materials[sc3_main->materials_num].sc_has_material = SC3_HAS_TEXTURES;
    sc3_main->sc_materials[sc3_main->materials_num].material_map.img_data = new_sc_materials;
    sc3_main->sc_materials[sc3_main->materials_num].material_map.x = width;
    sc3_main->sc_materials[sc3_main->materials_num++].material_map.y = height;
}
static bool is_same_animation_deform_data(Speed_Charm_3D_Anim * sc3_anim_1, Speed_Charm_3D_Anim * sc3_anim_2, sc_size skeleton_num){
    bool same_check = sc3_anim_1->frame_num == sc3_anim_2->frame_num && sc3_anim_1->framerate == sc3_anim_2->framerate && sc3_anim_1->time_beg == sc3_anim_2->time_beg && sc3_anim_1->time_end == sc3_anim_2->time_end;
    if(!same_check) return same_check;
    if(!sc3_anim_1->anim_deforms) return false;
    if(!sc3_anim_2->anim_deforms) return false;
    for(sc_size i = 0; i < skeleton_num; ++i){
        // 
        same_check = sc3_anim_1->anim_deforms[i].deform_status == sc3_anim_2->anim_deforms[i].deform_status;
        if(!same_check) return same_check;
        sc_byte status__ = sc3_anim_1->anim_deforms[i].deform_status;
        sc_size size__ = status__ & SC3_DEFORM_POS_STATIC ? 1 : sc3_anim_1->frame_num;
        same_check = smart_memsame(sc3_anim_1->anim_deforms[i].pos, sc3_anim_2->anim_deforms[i].pos, SGG_Vector3, size__);
        if(!same_check) return same_check;
        size__ = status__ & SC3_DEFORM_ROT_STATIC ? 1 : sc3_anim_1->frame_num;
        same_check = smart_memsame(sc3_anim_1->anim_deforms[i].rot, sc3_anim_2->anim_deforms[i].rot, SGG_Quaternion, size__);
        if(!same_check) return same_check;
        size__ = status__ & SC3_DEFORM_SCALE_STATIC ? 1 : sc3_anim_1->frame_num;
        same_check = smart_memsame(sc3_anim_1->anim_deforms[i].scale, sc3_anim_2->anim_deforms[i].scale, SGG_Vector3, size__);
        if(!same_check) return same_check;
    }
    return true;
}

void delete_duplicate_animation(Speed_Charm_3D_Main * sc3_main){
    if(!sc3_main) return;
    if(!sc3_main->sc_anim) return;
    sc_size anim_idx__ = 0;
    for(sc_size i = 0; i < sc3_main->anim_num; ++i){
        anim_idx__ = i + 1;
        sc_size tmp_anim_idx__ = sc3_main->anim_num;
        for(sc_size j = i + 1; j < tmp_anim_idx__; ++j){
            bool same_check = is_same_animation_deform_data(sc3_main->sc_anim + i, sc3_main->sc_anim + j, sc3_main->skeletons_num);
            if(same_check){
                (void)delete_speed_charm_3d_anim_deform((*(sc3_main->sc_anim + j)).anim_deforms, sc3_main->skeletons_num);
                s_free((*(sc3_main->sc_anim + j)).anim_deforms);
            }
            else{
                sc3_main->sc_anim[anim_idx__++] = sc3_main->sc_anim[j];
            }
        }
        sc3_main->anim_num = anim_idx__;
    }
}
void delete_ptr_animation_data(Speed_Charm_3D_Main * sc3_main, sc_size anim_idx){
    if(!sc3_main) return;
    if(!sc3_main->sc_anim) return;
    sc_size anim_idx__ = 0;
    for(sc_size i = 0; i < sc3_main->anim_num; ++i){
        if(anim_idx == i){
            if(sc3_main->sc_anim[i].anim_deforms){
                for(sc_size j = 0; j < sc3_main->skeletons_num; ++j){

                    s_free(sc3_main->sc_anim[i].anim_deforms[j].pos);
                    s_free(sc3_main->sc_anim[i].anim_deforms[j].rot);
                    s_free(sc3_main->sc_anim[i].anim_deforms[j].scale);
                }
            }
            s_free(sc3_main->sc_anim[i].anim_deforms);
        }
        else{
            sc3_main->sc_anim[anim_idx__++] = sc3_main->sc_anim[i];
        }
    }
    sc3_main->anim_num = anim_idx__;
}
sc_err_t delete_invalid_data(Speed_Charm_3D_Main * sc3_main){
    if(!sc3_main) return SC_POLICY_ERROR_ALLOC;
    //if(!sc3_main) return SC_POLICY_ERROR_ALLOC;
    //if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC_POLICY_ERROR_INVALID_VERSION;
    sc_size anim_idx__ = 0;
    for(sc_size i = 0; i < sc3_main->anim_num; ++i){
        sc_size frame_num__ = sc3_main->sc_anim[i].frame_num;
        float framerate__ = sc3_main->sc_anim[i].framerate;
        if((frame_num__ >= SC3_MAX_FRAME_NUM || framerate__ >= SC3_FRAMERATE_60) && sc3_main->sc_anim[i].anim_deforms){
            for(sc_size j = 0; j < sc3_main->skeletons_num; ++j){
                s_free(sc3_main->sc_anim[i].anim_deforms[j].pos);
                s_free(sc3_main->sc_anim[i].anim_deforms[j].rot);
                s_free(sc3_main->sc_anim[i].anim_deforms[j].scale);
            }
            s_free(sc3_main->sc_anim[i].anim_deforms);
        }
        else{
            sc3_main->sc_anim[anim_idx__++] = sc3_main->sc_anim[i];
        }
    }
    sc3_main->anim_num = anim_idx__;
    return SC3_FILE_OUT_CODE;
}
static sc_err_t solve_sc3_skeleton_sort(Speed_Charm_3D_Main_Contents * sc3_main_cont){
    if(!sc3_main_cont) return SC_POLICY_ERROR_ALLOC;
    if(!sc3_main_cont->sc3_main) return SC_POLICY_ERROR_ALLOC;

    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC_POLICY_ERROR_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;

    /**
     * 0 1 2 3 4 5 6 7
     *   0 1 2 1 7 2 3
     * 5 > 7
    */
    bool check___ = true;
    for (sc_size j = 0; j < sc3_main_cont->sc3_main->skeletons_num; ++j) {
        sc_int p = sc3_main_cont->sc3_main->sc_skeletons[j].parent;
        if (p >= 0 && (sc_size)p > j) {
            check___ = false;
        }
    }
    if(check___) return SC3_FILE_OUT_CODE;
    sc_int * none_root_mapping__ = smart_calloc(sc_int, sc3_main_cont->sc3_main->skeletons_num);
    sc_int * bones_order__ = smart_malloc(sc_int, sc3_main_cont->sc3_main->skeletons_num);
    Speed_Charm_3D_Skeleton * tmp__ = smart_malloc(Speed_Charm_3D_Skeleton, sc3_main_cont->sc3_main->skeletons_num);
    sc_int * new_index__ = smart_malloc(sc_int, sc3_main_cont->sc3_main->skeletons_num);
    if(!none_root_mapping__) return SC_POLICY_ERROR_ALLOC;
    if(!tmp__) return SC_POLICY_ERROR_ALLOC;
    if(!bones_order__) return SC_POLICY_ERROR_ALLOC;
    if(!new_index__) return SC_POLICY_ERROR_ALLOC;
    

    for(sc_size i = 0; i < sc3_main_cont->sc3_main->skeletons_num; ++i){
        sc_int parent = sc3_main_cont->sc3_main->sc_skeletons[i].parent;
        if(parent >= 0 && parent < sc3_main_cont->sc3_main->skeletons_num) none_root_mapping__[i]++;
    }
    
    sc_uint r_idx = 0;
    for(sc_size i = 0; i < sc3_main_cont->sc3_main->skeletons_num; ++i){
        /*collect root bones*/
        if(none_root_mapping__[i] == 0) bones_order__[r_idx++] = (sc_int)i;
    }
    
    /**
     * all skeleton 10
     * 
    */
    for (sc_size i = 0; i < r_idx; ++i) {
        sc_int v = bones_order__[i];
        for (sc_size j = 0; j < sc3_main_cont->sc3_main->skeletons_num; ++j) {
            if (sc3_main_cont->sc3_main->sc_skeletons[j].parent == v) {
                if (--none_root_mapping__[j] == 0) {
                    bones_order__[r_idx++] = (sc_int)j;
                }
            }
        }
    }
    /**
     * 0  1 2 3 4 5 6 7
     * -1 0 1 2 3 7 1 2
     * 
    */
    if(r_idx != sc3_main_cont->sc3_main->skeletons_num){
        s_free(new_index__);
        s_free(tmp__);
        s_free(bones_order__);
        s_free(none_root_mapping__);
        return SC3_FILE_OUT_CODE;
    }
     for (sc_size i = 0; i < sc3_main_cont->sc3_main->skeletons_num; ++i) {
        new_index__[bones_order__[i]] = (sc_int)i;
        tmp__[i] = sc3_main_cont->sc3_main->sc_skeletons[bones_order__[i]];
    }

    for (sc_size i = 0; i < sc3_main_cont->sc3_main->skeletons_num; ++i) {
        sc_int p = tmp__[i].parent;
        if (p >= 0 && (sc_size)p < sc3_main_cont->sc3_main->skeletons_num) {
            tmp__[i].parent = new_index__[p];
        } else {
            tmp__[i].parent = -1;
        }
    }
    smart_memcpy_p(sc3_main_cont->sc3_main->sc_skeletons, tmp__, Speed_Charm_3D_Skeleton, sc3_main_cont->sc3_main->skeletons_num);
    s_free(new_index__);
    s_free(tmp__);
    s_free(bones_order__);
    s_free(none_root_mapping__);

    return SC3_FILE_OUT_CODE;
    
}
static sc_err_t pack_sc3_mains(Speed_Charm_3D_Main_Contents * sc3_main_cont){
    if(!sc3_main_cont) return SC_POLICY_ERROR_ALLOC;
    if(!sc3_main_cont->sc3_main) return SC_POLICY_ERROR_ALLOC;
    Speed_Charm_3D_Main * sc3_main = sc3_main_cont->sc3_main;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC_POLICY_ERROR_INVALID_VERSION;
    if(!sc3_main) return SC3_FILE_ERROR_VAR_NULL;
    sc_size num__ = 0;
    for(sc_size i = 0; i < sc3_main->meshes_num; ++i){
        //printf("debubg out\n");
        if(sc3_main->sc_meshes[i].attr_num && sc3_main->sc_meshes[i].ind_num && sc3_main->sc_meshes[i].mesh_attr && sc3_main->sc_meshes[i].indices){
            
            sc3_main->sc_meshes[num__] = sc3_main->sc_meshes[i];
            num__++;

        }
        else{
            s_free(sc3_main->sc_meshes[i].mesh_attr);
            s_free(sc3_main->sc_meshes[i].indices);
            //s_free(sc3_main->sc_meshes[i].mesh_bone_map);
        }
    }
    Speed_Charm_3D_Mesh * sc3_meshes__ = smart_realloc(Speed_Charm_3D_Mesh, sc3_main->sc_meshes, num__);
    if (!num__) { sc3_main->sc_meshes = sc3_meshes__; return SC3_FILE_OUT_CODE; }


    if(!sc3_meshes__) return SC_POLICY_ERROR_ALLOC;
    if(num__ > sc3_main->meshes_num)
    memset(&sc3_meshes__[sc3_main->meshes_num],
        0,
        sizeof(Speed_Charm_3D_Mesh) * (num__ - sc3_main->meshes_num));

    for(sc_size i = 0; i < num__; ++i){
        sc_size offset__ = i % SC_POLICY_SC_UINT_8_BITS;
        sc_size idx__ = i / SC_POLICY_SC_UINT_8_BITS;
        sc_ubyte inf = sc3_meshes__[i].mesh_inf << offset__;
        sc3_meshes__[i].mesh_inf = (sc_ubyte)0;
        sc3_meshes__[idx__].mesh_inf |= inf;
    }
    sc3_main->meshes_num = num__;
    sc3_main->sc_meshes = sc3_meshes__;
    /*
    num__ = 0;
    for (sc_size i = 0; i < sc3_main->blend_meshes_num; ++i) {
        if (sc3_main->sc_blend_meshes[i].sc3_mesh.attr_num && 
            sc3_main->sc_blend_meshes[i].sc3_mesh.ind_num &&
            sc3_main->sc_blend_meshes[i].sc3_mesh.mesh_attr &&
            sc3_main->sc_blend_meshes[i].sc3_mesh.indices
            ) {

            sc3_main->sc_blend_meshes[num__] = sc3_main->sc_blend_meshes[i];
            num__++;

        }
        else {
            s_free(sc3_main->sc_blend_meshes[i].sc3_mesh.mesh_attr);
            s_free(sc3_main->sc_blend_meshes[i].sc3_mesh.indices);
        }
    }
    Speed_Charm_3D_Blend_Mesh* sc3_blend_meshes__ = smart_realloc(Speed_Charm_3D_Blend_Mesh, sc3_main->sc_blend_meshes, num__);
    if (!num__) { sc3_main->sc_blend_meshes = sc3_blend_meshes__; return SC3_FILE_OUT_CODE; }


    if (!sc3_blend_meshes__) return SC_POLICY_ERROR_ALLOC;
    if (num__ > sc3_main->blend_meshes_num)
        memset(&sc3_blend_meshes__[sc3_main->blend_meshes_num],
            0,
            sizeof(Speed_Charm_3D_Mesh) * (num__ - sc3_main->blend_meshes_num));

    for (sc_size i = 0; i < num__; ++i) {
        sc_size offset__ = i % SC_POLICY_SC_UINT_8_BITS;
        sc_size idx__ = i / SC_POLICY_SC_UINT_8_BITS;
        sc_ubyte inf = sc3_blend_meshes__[i].sc3_mesh.mesh_inf << offset__;
        sc3_blend_meshes__[i].sc3_mesh.mesh_inf = (sc_ubyte)0;
        sc3_blend_meshes__[idx__].sc3_mesh.mesh_inf |= inf;
    }
    sc3_main->blend_meshes_num = num__;
    sc3_main->sc_blend_meshes = sc3_blend_meshes__;
    */
    for(sc_size i = 0; i < sc3_main->anim_num; ++i){
        for(sc_size j = 0; j < sc3_main->skeletons_num; ++j){
            Speed_Charm_3D_Anim_Deform * anim_deform__ = &sc3_main->sc_anim[i].anim_deforms[j];
            if((anim_deform__->deform_status &
                ((sc_ubyte)(~0u) << SC3_HALF_UINT_8_BITS)) && j == 0) {
                
                break;
            }
            sc_ubyte sts = ((sc_ubyte)~0u >> 4u) << ((j % 2) * SC3_HALF_UINT_8_BITS);
            sc_ubyte sts__ = ((sc_ubyte)~0u >> 4u) << ((1 - j % 2) * SC3_HALF_UINT_8_BITS);
            sc_ubyte top__ =  ((anim_deform__->deform_status) << ((j % 2) * 4)) & sts;
            
            sc_size idx__ = j / 2;
            sc3_main->sc_anim[i].anim_deforms[idx__].deform_status &= sts__;
            sc3_main->sc_anim[i].anim_deforms[idx__].deform_status |= top__;
        }
    }

    sc_size opt_deform_num__ = (sc3_main->bones_num + 1) / 2;
    return SC3_FILE_OUT_CODE;
}
static sc_err_t unpack_sc3_mains(Speed_Charm_3D_Main_Contents* sc3_main_cont) {
    if (!sc3_main_cont) return SC_POLICY_ERROR_ALLOC;
    if (!sc3_main_cont->sc3_main) return SC_POLICY_ERROR_ALLOC;

    Speed_Charm_3D_Main* sc3_main = sc3_main_cont->sc3_main;

    if (!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC_POLICY_ERROR_INVALID_VERSION;
    if (!sc3_main) return SC3_FILE_ERROR_VAR_NULL;
    for (sc_size i = sc3_main_cont->sc3_main->meshes_num; i > 0; --i) {

        sc_size idx__ = (i - 1) / SC_POLICY_SC_UINT_8_BITS;
        sc_size offset__ = (i - 1) % SC_POLICY_SC_UINT_8_BITS;

        sc_ubyte packed__ = sc3_main->sc_meshes[idx__].mesh_inf;

        sc3_main->sc_meshes[i - 1].mesh_inf =
            (packed__ >> offset__) & (sc_ubyte)0x1;

    }
    /*
    for (sc_size i = sc3_main_cont->sc3_main->blend_meshes_num; i > 0; --i) {

        sc_size idx__ = (i - 1) / SC_POLICY_SC_UINT_8_BITS;
        sc_size offset__ = (i - 1) % SC_POLICY_SC_UINT_8_BITS;

        sc_ubyte packed__ = sc3_main->sc_blend_meshes[idx__].sc3_mesh.mesh_inf;

        sc3_main->sc_blend_meshes[i - 1].sc3_mesh.mesh_inf =
            (packed__ >> offset__) & (sc_ubyte)0x1;
    }
    */
    for (sc_size i = 0; i < sc3_main->anim_num; ++i) {
        if (!sc3_main->sc_anim[i].anim_deforms) {
            if (sc3_main->skeletons_num) return SC3_FILE_ERROR_MATCH_ANIM_DEF_DATA;
            continue;
        }

        for (sc_size j = sc3_main->skeletons_num; j > 0; --j) {
            sc_size def_idx__ = j - 1;
            sc_size idx__ = def_idx__ / 2;

            sc_ubyte dfs__ = sc3_main->sc_anim[i].anim_deforms[idx__].deform_status;

            if (def_idx__ % 2) {
                sc3_main->sc_anim[i].anim_deforms[def_idx__].deform_status =
                    (dfs__ >> SC3_HALF_UINT_8_BITS) &
                    ((sc_ubyte)~0u >> SC3_HALF_UINT_8_BITS);
            }
            else {
                sc3_main->sc_anim[i].anim_deforms[def_idx__].deform_status =
                    dfs__ &
                    ((sc_ubyte)~0u >> SC3_HALF_UINT_8_BITS);
            }
        }
    }

    return SC3_FILE_OUT_CODE;
}
Speed_Charm_3D_Run_Main* share_sc3_main_sc3_run_main(Speed_Charm_3D_Main** sc3_main, struct Speed_Charm_3D_Virtual_Vertex_Package* vvp, struct Speed_Charm_3D_Virtual_Material_Package* vmp) {
    if (!sc3_main) return NULL;
    if (!vvp) return NULL;
    Speed_Charm_3D_Main* sc3_main__ = *sc3_main;
    if (!sc3_main__) return NULL;
    Speed_Charm_3D_Run_Main* sc3_run_main__ = smart_calloc(Speed_Charm_3D_Run_Main, 1);
    if (!sc3_run_main__) return NULL;
    sc3_run_main__->sc_meshes = smart_calloc(Speed_Charm_3D_Run_Mesh, sc3_main__->meshes_num);
    if (!sc3_run_main__->sc_meshes) {
        s_free(sc3_run_main__);
        return NULL;
    }
    sc3_run_main__->meshes_num = sc3_main__->meshes_num;
    for (sc_size i = 0; i < sc3_main__->meshes_num; ++i) {
        sc3_run_main__->sc_meshes[i].mesh_inf = sc3_main__->sc_meshes[i].mesh_inf;
        smart_memcpy_p(&sc3_run_main__->sc_meshes[i].material_point, &sc3_main__->sc_meshes[i].material_point, Speed_Charm_3D_Mesh_Material_Point, 1);
    }
    sc3_run_main__->sc_vvp = vvp;
    sc3_run_main__->sc_vmp = vmp;
    sc3_run_main__->sc_skeletons = sc3_main__->sc_skeletons;
    sc3_run_main__->skeletons_num = sc3_main__->skeletons_num;
    sc3_run_main__->sc_bones = sc3_main__->sc_bones;
    sc3_run_main__->bones_num = sc3_main__->bones_num;
    sc3_run_main__->sc_anim = sc3_main__->sc_anim;
    sc3_run_main__->anim_num = sc3_main__->anim_num;
    sc3_run_main__->instance_num = sc3_main__->instance_num;
    sc3_run_main__->sc_ins = sc3_main__->sc_ins;
    return sc3_run_main__;
}
Speed_Charm_3D_Run_Main * move_sc3_main_to_sc3_run_main(Speed_Charm_3D_Main ** sc3_main, struct Speed_Charm_3D_Virtual_Vertex_Package * vvp, struct Speed_Charm_3D_Virtual_Material_Package * vmp){
    if(!sc3_main) return NULL;
    if(!vvp) return NULL;
    Speed_Charm_3D_Main * sc3_main__ = *sc3_main;
    if(!sc3_main__) return NULL;
    Speed_Charm_3D_Run_Main * sc3_run_main__ = smart_calloc(Speed_Charm_3D_Run_Main, 1);
    if(!sc3_run_main__) return NULL;
    sc3_run_main__->sc_meshes = smart_calloc(Speed_Charm_3D_Run_Mesh, sc3_main__->meshes_num);
    if(!sc3_run_main__->sc_meshes){
        /*delete implement*/
        s_free(sc3_run_main__);
        //delete_speed_charm_3d_run_main(sc_run_main__, sc_main__);
        return NULL;
    }
    sc3_run_main__->meshes_num = sc3_main__->meshes_num;
    for(sc_size i = 0; i < sc3_main__->meshes_num; ++i) {    
        sc3_run_main__->sc_meshes[i].mesh_inf = sc3_main__->sc_meshes[i].mesh_inf;
        smart_memcpy_p(&sc3_run_main__->sc_meshes[i].material_point, &sc3_main__->sc_meshes[i].material_point, Speed_Charm_3D_Mesh_Material_Point, 1);
    }
    sc3_run_main__->sc_vvp = vvp;
    sc3_run_main__->sc_vmp = vmp;
    sc3_run_main__->sc_skeletons = sc3_main__->sc_skeletons;
    sc3_main__->sc_skeletons = NULL;
    sc3_run_main__->skeletons_num = sc3_main__->skeletons_num;
    sc3_run_main__->bones_num = sc3_main__->bones_num;
    sc3_main__->bones_num = 0;
    sc3_run_main__->sc_bones = sc3_main__->sc_bones;
    sc3_main__->sc_bones = NULL;
    sc3_run_main__->sc_anim = sc3_main__->sc_anim;
    sc3_main__->sc_anim = NULL;
    sc3_run_main__->anim_num = sc3_main__->anim_num;
    sc3_main__->anim_num = 0;

    sc3_run_main__->instance_num = sc3_main__->instance_num;
    sc3_main__->instance_num = 0;
    sc3_run_main__->sc_ins = sc3_main__->sc_ins;
    sc3_main__->sc_ins = NULL;
    //sc_run_main__->sc_vvp = vvp;
    delete_speed_charm_3d_main(sc3_main);

    return sc3_run_main__;
}

Speed_Charm_3D_Main_Contents * init_speed_charm_3d_main_contents(Speed_Charm_3D_Main * sc3_main){
    Speed_Charm_3D_Main_Contents * sc3_main_cont = smart_calloc(Speed_Charm_3D_Main_Contents, 1);
    if(!sc3_main_cont) return NULL;
    if(sc3_main){
        sc3_main_cont->sc3_main = sc3_main;
        return sc3_main_cont;
    }
    sc3_main_cont->sc3_main = smart_calloc(Speed_Charm_3D_Main, 1);
    if(!sc3_main_cont->sc3_main){
        s_free(sc3_main_cont);
        return NULL;
    }
    return sc3_main_cont;
}
sc_err_t write_speed_charm_3d_file_entry(Speed_Charm_3D_Main_Contents * sc3_main_cont, const sc_byte * sc3_file_path, sc_ushort version){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;

    if(!sc3_file_path) sc3_file_path = SC3_DEFAULT_FILE_NAME;

    sc_byte sc3_file_path__[SC3_FILE_NAME_DEFAULT_SIZE] = {0};
    
    if(!ext_str_cpy(sc3_file_path__, sc3_file_path, SC3_FILE_NAME_DEFAULT_SIZE, (safety_strlen(sc3_file_path)) + 1)) return SC3_FILE_ERROR_VAR_NULL;

    set_sc3_file_format(sc3_file_path__);
    
    if(scl_fopen(sc3_main_cont->sc3_f__, sc3_file_path__, "wb")) return SC3_FILE_ERROR_CODE;
    sc3_main_cont->sc3_version = version;

    scl_fwrite(sc3_main_cont->sc3_f__, SC3_FILE_ENTRY_MAGIC_NUMBER, sc_byte, len_of_str(SC3_FILE_ENTRY_MAGIC_NUMBER));

    scl_fwrite(sc3_main_cont->sc3_f__, &version, sc_ushort, 1);
    
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;

    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MAIN_BEG, sc_byte, len_of_str(SC3_MAIN_BEG));
        
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MAIN_TABLE, sc_byte, len_of_str(SC3_MAIN_TABLE));
        
    return SC3_FILE_OUT_CODE;
}

sc_err_t write_speed_charm_3d_table_helper(Speed_Charm_3D_Main_Contents * sc3_main_cont, const sc_byte * table_str, size_t table_str_len){
    const sc_size booking_table_pos__ = 0;
    scl_fwrite(sc3_main_cont->sc3_f__, table_str, sc_byte, table_str_len);
    scl_fwrite(sc3_main_cont->sc3_f__, &booking_table_pos__, sc_size, 1);
    return SC3_FILE_OUT_CODE;
}
sc_err_t write_speed_charm_3d_main_table(Speed_Charm_3D_Main_Contents * sc3_main_cont){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;

    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC_POLICY_ERROR_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;
    
    write_speed_charm_3d_table_helper(sc3_main_cont, SC3_MAIN_TABLE_MESH, len_of_str(SC3_MAIN_TABLE_MESH));
    write_speed_charm_3d_table_helper(sc3_main_cont, SC3_MAIN_TABLE_BONE, len_of_str(SC3_MAIN_TABLE_BONE));
    write_speed_charm_3d_table_helper(sc3_main_cont, SC3_MAIN_TABLE_ANIM, len_of_str(SC3_MAIN_TABLE_ANIM));
    if(sc3_main_cont->sc3_version >= SC3_TEXTURE_VALID_VERSION) write_speed_charm_3d_table_helper(sc3_main_cont, SC3_MAIN_TABLE_TEXTURE, len_of_str(SC3_MAIN_TABLE_TEXTURE));
    if(sc3_main_cont->sc3_version >= SC3_INSTANCE_VALID_VERSION) write_speed_charm_3d_table_helper(sc3_main_cont, SC3_MAIN_TABLE_INSTANCE, len_of_str(SC3_MAIN_TABLE_INSTANCE));
    return SC3_FILE_OUT_CODE;
}

sc_err_t write_speed_charm_3d_user(Speed_Charm_3D_Main_Contents * sc3_main_cont, const sc_byte * comment_str){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC_POLICY_ERROR_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;

    //if(!SC_POLICY_CHECK_NONE_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC_POLICY_ERROR_INVALID_VERSION;
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MAIN_USER, sc_byte, len_of_str(SC3_MAIN_USER));
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MAIN_COMMENT, sc_byte, len_of_str(SC3_MAIN_COMMENT));
    sc_byte sc3_comment__[SC3_COMMENT_SIZE_MAX] = {0};
    
    set_os_string(sc3_comment__);
    
    if(!comment_str) smart_memcpy_p(sc3_comment__ + SC_POLICY_OS_STR_SIZE, SC3_DEFAULT_COMMENT, sc_byte, SC3_DEFAULT_COMMENT_LEN);
    else{
        sc_size len__ = strlen(comment_str);
        len__ = SC3_DEFAULT_COMMENT_LEN <= len__ ? SC3_DEFAULT_COMMENT_LEN : len__;
        smart_memcpy_p(sc3_comment__ + SC_POLICY_OS_STR_SIZE, comment_str, sc_byte, len__);
    }

    scl_fwrite(sc3_main_cont->sc3_f__, sc3_comment__, sc_byte, SC3_COMMENT_SIZE_MAX);
    
    sc_ubyte fd_data__[SC3_FREE_DATA_SIZE] = {0};
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_FREE_DATA_CHUNK, sc_byte, len_of_str(SC3_FREE_DATA_CHUNK));
    scl_fwrite(sc3_main_cont->sc3_f__, fd_data__, sc_ubyte, SC3_FREE_DATA_SIZE);
    
    return SC3_FILE_OUT_CODE;
}

sc_err_t write_speed_charm_3d_close(Speed_Charm_3D_Main_Contents * sc3_main_cont){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) {
        scl_close(sc3_main_cont->sc3_f__);
        return SC_POLICY_ERROR_INVALID_VERSION;
    }
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) {
        scl_close(sc3_main_cont->sc3_f__);
        return SC3_INVALID_VERSION;
    }
    scl_fgoto_e(sc3_main_cont->sc3_f__);
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MAIN_END, sc_byte, len_of_str(SC3_MAIN_END));
    scl_close(sc3_main_cont->sc3_f__);
    return SC3_FILE_OUT_CODE;
}
void goto_sc3f_pos(SC_FILE __sc3f, sc_size pos){
    /*now : 10
    * pos : 5
    * now : 5
    * now : 10
    */
    scl_fgoto(__sc3f, pos, SEEK_SET);
}

sc_err_t write_speed_charm_3d_main(const sc_byte * sc3_file_path, Speed_Charm_3D_Main * sc3_main){
    Speed_Charm_3D_Main_Contents sc3_main_cont = {0};
    sc3_main_cont.sc3_main = sc3_main;
    //init_speed_charm_3d_main_contents(sc3_main);
    sc_err_t error_code__ = write_speed_charm_3d_file_entry(&sc3_main_cont, sc3_file_path, SC3_NOW_VERSION);
    if(error_code__ < SC3_FILE_OUT_CODE) {
        //fclose(sc3_main_cont->sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }

    error_code__ = write_speed_charm_3d_main_table(&sc3_main_cont);
    if(error_code__ < SC3_FILE_OUT_CODE) {
        scl_close(sc3_main_cont.sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }

    error_code__ = write_speed_charm_3d_user(&sc3_main_cont, NULL);
    if(error_code__ < SC3_FILE_OUT_CODE) {
        scl_close(sc3_main_cont.sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }
    error_code__ = write_speed_charm_3d_data_main(&sc3_main_cont);
    if(error_code__ < SC3_FILE_OUT_CODE) {
        scl_close(sc3_main_cont.sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }
    error_code__ = write_speed_charm_3d_close(&sc3_main_cont);
    //delete_speed_charm_3d_main_contents(&sc3_main_cont);
    //scl_close(sc3_main_cont.sc3_f__);
    return error_code__;

}

sc_err_t write_speed_charm_3d_data_main(Speed_Charm_3D_Main_Contents * sc3_main_cont){
    sc_size end_pos__;
    /*after place to calc mesh first pos macro*/
    end_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);
    
    pack_sc3_mains(sc3_main_cont);
    solve_sc3_skeleton_sort(sc3_main_cont);
    sc_err_t error_code__ = write_speed_charm_3d_mesh(sc3_main_cont, &end_pos__);

    if(error_code__ < SC3_FILE_OUT_CODE) {
        //fclose(sc3_main_cont->sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }

    error_code__ = write_speed_charm_3d_bone(sc3_main_cont, &end_pos__);
    if(error_code__ < SC3_FILE_OUT_CODE) {
        //fclose(sc3_main_cont->sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }

    error_code__ = write_speed_charm_3d_anim(sc3_main_cont, &end_pos__);

    if(error_code__ < SC3_FILE_OUT_CODE) {
        //fclose(sc3_main_cont->sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }
    error_code__ = write_speed_charm_3d_tex(sc3_main_cont, &end_pos__);
    if(error_code__ < SC3_FILE_OUT_CODE) {
        //fclose(sc3_main_cont->sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }
    error_code__ = write_speed_charm_3d_ins(sc3_main_cont, &end_pos__);
    if (error_code__ < SC3_FILE_OUT_CODE) {
        return error_code__;
    }

    unpack_sc3_mains(sc3_main_cont);

    return SC3_FILE_OUT_CODE;
}

sc_err_t write_speed_charm_3d_bone(Speed_Charm_3D_Main_Contents * sc3_main_cont, sc_size * end_size){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!end_size) return SC3_FILE_ERROR_VAR_NULL;
    //goto_sc3f_pos(sc3_main_cont->sc3_f__, *end_size);

    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;
    
    goto_sc3f_pos(sc3_main_cont->sc3_f__, *end_size);
    sc_size now_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);
    sc_size bone_size__ = 0;
    
    scl_fwrite(sc3_main_cont->sc3_f__, &bone_size__, sc_size, 1);
    bone_size__ += sizeof(sc_size) * 1;
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_BONE_CHUNK, sc_byte, len_of_str(SC3_BONE_CHUNK));
    bone_size__ += sizeof(sc_byte) * len_of_str(SC3_BONE_CHUNK);
    scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->bones_num, sc_size, 1);
    bone_size__ += sizeof(sc_size) * 1;
    scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->skeletons_num, sc_size, 1);
    bone_size__ += sizeof(sc_size) * 1;
    
    if(!sc3_main_cont->sc3_main->sc_bones && sc3_main_cont->sc3_main->bones_num) return SC3_FILE_ERROR_MATCH_BONE_DATA;
    if(!sc3_main_cont->sc3_main->sc_skeletons && sc3_main_cont->sc3_main->skeletons_num) return SC3_FILE_ERROR_MATCH_SKELETON_DATA;
    scl_fwrite(sc3_main_cont->sc3_f__, sc3_main_cont->sc3_main->sc_bones, Speed_Charm_3D_Bone_Map, sc3_main_cont->sc3_main->bones_num);
    bone_size__ += sizeof(Speed_Charm_3D_Bone_Map) * sc3_main_cont->sc3_main->bones_num;
    scl_fwrite(sc3_main_cont->sc3_f__, sc3_main_cont->sc3_main->sc_skeletons, Speed_Charm_3D_Skeleton, sc3_main_cont->sc3_main->skeletons_num);
    bone_size__ += sizeof(Speed_Charm_3D_Skeleton) * sc3_main_cont->sc3_main->skeletons_num;
    *end_size = scl_get_fpos(sc3_main_cont->sc3_f__);

    goto_sc3f_pos(sc3_main_cont->sc3_f__, now_pos__);
    scl_fwrite(sc3_main_cont->sc3_f__, &bone_size__, sc_size, 1);
    sc_size bone_table_pos__ = SC3_BONE_TABLE_POS;
    goto_sc3f_pos(sc3_main_cont->sc3_f__, bone_table_pos__);
    scl_fwrite(sc3_main_cont->sc3_f__, &now_pos__, sc_size, 1);
    return SC3_FILE_OUT_CODE;

}

sc_err_t write_speed_charm_3d_anim(Speed_Charm_3D_Main_Contents * sc3_main_cont, sc_size * end_size){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!end_size) return SC3_FILE_ERROR_VAR_NULL;

    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;

    goto_sc3f_pos(sc3_main_cont->sc3_f__, *end_size);

    sc_size now_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);
    sc_size anim_size__ = 0;
    scl_fwrite(sc3_main_cont->sc3_f__, &anim_size__, sc_size, 1);
    anim_size__ += calc_type_size(sc_size, 1);
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_ANIM_CHUNK, sc_byte, len_of_str(SC3_ANIM_CHUNK));
    anim_size__ += calc_type_size(sc_byte, len_of_str(SC3_ANIM_CHUNK));
    
    scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->anim_num, sc_size, 1);
    anim_size__ += calc_type_size(sc_size, 1);

    if(!sc3_main_cont->sc3_main->sc_anim && sc3_main_cont->sc3_main->anim_num) return SC3_FILE_ERROR_ANIM;
    
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_ANIM_INFORMATION_CHUNK, sc_byte, len_of_str(SC3_ANIM_INFORMATION_CHUNK));
    anim_size__ += calc_type_size(sc_byte, len_of_str(SC3_ANIM_INFORMATION_CHUNK));

    for(sc_size i = 0; i < sc3_main_cont->sc3_main->anim_num; ++i){
        scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_anim[i].frame_num, sc_size, 1);
        anim_size__ += calc_type_size(sc_size, 1);
        scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_anim[i].time_beg, float, 1);
        anim_size__ += calc_type_size(float, 1);
        scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_anim[i].time_end, float, 1);
        anim_size__ += calc_type_size(float, 1);
        scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_anim[i].framerate, float, 1);
        anim_size__ += calc_type_size(float, 1);
        if(!sc3_main_cont->sc3_main->sc_anim[i].anim_deforms && sc3_main_cont->sc3_main->skeletons_num) return SC3_FILE_ERROR_MATCH_ANIM_DEF_DATA;
        sc_size opt_deform_num__ = (sc3_main_cont->sc3_main->skeletons_num + 1) / 2;
        for(sc_size j = 0; j < opt_deform_num__; ++j){
            sc_ubyte sc3_dfs__ = sc3_main_cont->sc3_main->sc_anim[i].anim_deforms[j].deform_status;
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_dfs__, sc_ubyte, 1);
        }
        anim_size__ += calc_type_size(sc_ubyte, opt_deform_num__);
    }

    scl_fwrite(sc3_main_cont->sc3_f__, SC3_ANIM_DATA_CHUNK, sc_byte, len_of_str(SC3_ANIM_DATA_CHUNK));
    anim_size__ += calc_type_size(sc_byte, len_of_str(SC3_ANIM_DATA_CHUNK));

    for(sc_size i = 0; i < sc3_main_cont->sc3_main->anim_num; ++i){
        for(sc_size j = 0; j < sc3_main_cont->sc3_main->skeletons_num; ++j){

            sc_ubyte dfs__ = get_animation_deform_status_packed(j, &sc3_main_cont->sc3_main->sc_anim[i]);
            sc_size deforms_num__ = dfs__ & SC3_DEFORM_ROT_STATIC ? 1 : sc3_main_cont->sc3_main->sc_anim[i].frame_num;
            //printf("%f\n", sc3_main_cont->sc3_main->sc_anim[i].anim_deforms[j].rot[0]);
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_anim[i].anim_deforms[j].rot[0], SGG_Quaternion, deforms_num__);
            anim_size__ += calc_type_size(SGG_Quaternion, deforms_num__);
            deforms_num__ = dfs__ & SC3_DEFORM_POS_STATIC ? 1 : sc3_main_cont->sc3_main->sc_anim[i].frame_num;
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_anim[i].anim_deforms[j].pos[0], SGG_Vector3, deforms_num__);
            anim_size__ += calc_type_size(SGG_Vector3, deforms_num__);
            deforms_num__ = dfs__ & SC3_DEFORM_SCALE_STATIC ? 1 : sc3_main_cont->sc3_main->sc_anim[i].frame_num;
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_anim[i].anim_deforms[j].scale[0], SGG_Vector3, deforms_num__);
            anim_size__ += calc_type_size(SGG_Vector3, deforms_num__);
        }
    }

    *end_size = scl_get_fpos(sc3_main_cont->sc3_f__);

    goto_sc3f_pos(sc3_main_cont->sc3_f__, now_pos__);
    scl_fwrite(sc3_main_cont->sc3_f__, &anim_size__, sc_size, 1);
    const sc_size anim_table_pos__ = SC3_ANIM_TABLE_POS;
    goto_sc3f_pos(sc3_main_cont->sc3_f__, anim_table_pos__);
    scl_fwrite(sc3_main_cont->sc3_f__, &now_pos__, sc_size, 1);
    return SC3_FILE_OUT_CODE;

}

sc_err_t write_speed_charm_3d_tex(Speed_Charm_3D_Main_Contents * sc3_main_cont,  sc_size * end_size){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!end_size) return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;


    goto_sc3f_pos(sc3_main_cont->sc3_f__, *end_size);

    if(SC_POLICY_CHECK_MODULE_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_TEXTURE_VALID_VERSION)) {
        /*not implement*/

        //goto_sc3f_pos(sc3_main_cont->sc3_f__, *end_size);
        sc_size now_pos = ftell(sc3_main_cont->sc3_f__);
        sc_size tex_size = 0;
        fwrite(&tex_size, sizeof(sc_size), 1, sc3_main_cont->sc3_f__);
        tex_size += sizeof(sc_size) * 1;
        //if(sc3_main_cont->sc3_optional & SC3_SCI_TEXTURE_OPTIONAL){
        fwrite(SC3_MATERIAL_TEXTURE_CHUNK, sizeof(sc_byte), len_of_str(SC3_MATERIAL_TEXTURE_CHUNK), sc3_main_cont->sc3_f__);
        tex_size += sizeof(sc_byte) * len_of_str(SC3_MATERIAL_TEXTURE_CHUNK);
        sc_size total_size = 0;
        scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->materials_num, sc_size, 1);
        tex_size += sizeof(sc_size) * 1;

        if(!sc3_main_cont->sc3_main->materials_num){
            *end_size = scl_get_fpos(sc3_main_cont->sc3_f__);

            goto_sc3f_pos(sc3_main_cont->sc3_f__, now_pos);

            scl_fwrite(sc3_main_cont->sc3_f__, &tex_size, sc_size, 1);

            const sc_size tex_table_pos__ = SC3_TEX_TABLE_POS;

            goto_sc3f_pos(sc3_main_cont->sc3_f__, tex_table_pos__);

            scl_fwrite(sc3_main_cont->sc3_f__, &now_pos, sc_size, 1);
            return SC3_FILE_OUT_CODE;
        }
        
        for(sc_size i = 0; i < sc3_main_cont->sc3_main->materials_num; ++i){
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_materials[i].sc_has_material, sc_ushort, 1);
            tex_size += sizeof(sc_ushort) * 1;
            if(sc3_main_cont->sc3_main->sc_materials[i].sc_has_material == SC3_HAS_TEXTURES){
                total_size += sc3_main_cont->sc3_main->sc_materials[i].material_map.x * sc3_main_cont->sc3_main->sc_materials[i].material_map.y * SC3_TEXTURE_MATERIAL_CHANNEL;
                //printf("%u\n", total_size);
                scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_materials[i].material_map.x, sc_size, 1);
                scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_materials[i].material_map.y, sc_size, 1);
                tex_size += sizeof(sc_size) * 2;
            }
            else if(sc3_main_cont->sc3_main->sc_materials[i].sc_has_material == SC3_HAS_VALUES_VECTOR){
                total_size += sizeof(SGG_Vector3);
            }
            else if(sc3_main_cont->sc3_main->sc_materials[i].sc_has_material == SC3_HAS_VALUES_VECTOR_ALPHA){
                total_size += sizeof(SGG_Vector4);
            }
            else if(sc3_main_cont->sc3_main->sc_materials[i].sc_has_material == SC3_HAS_VALUES_SCALER){
                total_size += sizeof(float);
            }
        }
        sc_ubyte * total_buffer = smart_malloc(sc_ubyte, total_size);

        if(!total_buffer) return SC3_FILE_ERROR_VAR_NULL;

        for(sc_size i = 0, offset = 0; i < sc3_main_cont->sc3_main->materials_num; ++i){
            if(sc3_main_cont->sc3_main->sc_materials[i].sc_has_material == SC3_HAS_TEXTURES){
                //printf("%u %u %u %u %u\n", sc3_main_cont->sc3_main->sc_materials[i].material_map.x, sc3_main_cont->sc3_main->sc_materials[i].material_map.y, SC3_TEXTURE_MATERIAL_CHANNEL, sc3_main_cont->sc3_main->materials_num, i);
                smart_memcpy_p(total_buffer + offset, sc3_main_cont->sc3_main->sc_materials[i].material_map.img_data, sc_ubyte, sc3_main_cont->sc3_main->sc_materials[i].material_map.x * sc3_main_cont->sc3_main->sc_materials[i].material_map.y * SC3_TEXTURE_MATERIAL_CHANNEL);
                
                offset += sc3_main_cont->sc3_main->sc_materials[i].material_map.x * sc3_main_cont->sc3_main->sc_materials[i].material_map.y * SC3_TEXTURE_MATERIAL_CHANNEL;
            }
            else if(sc3_main_cont->sc3_main->sc_materials[i].sc_has_material == SC3_HAS_VALUES_VECTOR){
                smart_memcpy_p(total_buffer + offset, &sc3_main_cont->sc3_main->sc_materials[i].material_value3, SGG_Vector3, 1);
                offset += sizeof(SGG_Vector3);
            }
            else if(sc3_main_cont->sc3_main->sc_materials[i].sc_has_material == SC3_HAS_VALUES_VECTOR_ALPHA){
                smart_memcpy_p(total_buffer + offset, &sc3_main_cont->sc3_main->sc_materials[i].material_value4, SGG_Vector4, 1);
                offset += sizeof(SGG_Vector4);
            }
            else if(sc3_main_cont->sc3_main->sc_materials[i].sc_has_material == SC3_HAS_VALUES_SCALER){
                smart_memcpy_p(total_buffer + offset, &sc3_main_cont->sc3_main->sc_materials[i].material_value, float, 1);
                offset += sizeof(float);
            }
        }

        sc_ubyte * w_buffer = sc3_zstd_compress(total_buffer, &total_size);

        bool is_raw = false;
        if(!w_buffer) {
            is_raw = true;
            w_buffer = total_buffer;
            total_buffer = NULL;
        }
        scl_fwrite(sc3_main_cont->sc3_f__, &is_raw, bool, 1);
        tex_size += sizeof(bool) * 1;
        scl_fwrite(sc3_main_cont->sc3_f__, &total_size, sc_size, 1);
        tex_size += sizeof(sc_size) * 1;
        scl_fwrite(sc3_main_cont->sc3_f__, w_buffer, sc_ubyte, total_size);
        tex_size += sizeof(sc_ubyte) * total_size;
        s_free(w_buffer);
        s_free(total_buffer);

        *end_size = scl_get_fpos(sc3_main_cont->sc3_f__);

        goto_sc3f_pos(sc3_main_cont->sc3_f__, now_pos);

        scl_fwrite(sc3_main_cont->sc3_f__, &tex_size, sc_size, 1);

        const sc_size tex_table_pos__ = SC3_TEX_TABLE_POS;

        goto_sc3f_pos(sc3_main_cont->sc3_f__, tex_table_pos__);

        scl_fwrite(sc3_main_cont->sc3_f__, &now_pos, sc_size, 1);

    }
    return SC3_FILE_OUT_CODE;

}

sc_err_t write_speed_charm_3d_ins(Speed_Charm_3D_Main_Contents* sc3_main_cont, sc_size* end_size) {
    if (!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if (!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if (!end_size) return SC3_FILE_ERROR_VAR_NULL;
    if (!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if (!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;


    goto_sc3f_pos(sc3_main_cont->sc3_f__, *end_size);

    if (SC_POLICY_CHECK_MODULE_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_INSTANCE_VALID_VERSION)) {
        sc_size ins_size__ = 0;
        sc_size now_pos__ = ftell(sc3_main_cont->sc3_f__);

        scl_fwrite(sc3_main_cont->sc3_f__, &ins_size__, sc_size, 1);
        ins_size__ += sizeof(sc_size) * 1;
        scl_fwrite(sc3_main_cont->sc3_f__, SC3_INSTANCE_CHUNK, sc_byte, len_of_str(SC3_INSTANCE_CHUNK));
        ins_size__ += sizeof(sc_byte) * len_of_str(SC3_INSTANCE_CHUNK);
        scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->instance_num, sc_size, 1);
        ins_size__ += sizeof(sc_size) * 1;
        if (!sc3_main_cont->sc3_main->sc_ins && sc3_main_cont->sc3_main->instance_num) return SC3_FILE_ERROR_INS;

        scl_fwrite(sc3_main_cont->sc3_f__, SC3_INSTANCE_INFORMATION_CHUNK, sc_byte, len_of_str(SC3_INSTANCE_INFORMATION_CHUNK));
        ins_size__ += sizeof(sc_byte) * len_of_str(SC3_INSTANCE_INFORMATION_CHUNK);
        for (sc_size i = 0; i < sc3_main_cont->sc3_main->instance_num; ++i) {
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_ins[i].group_num, sc_uint, 1);
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_ins[i].level_num, sc_uint, 1);
            ins_size__ += sizeof(sc_uint) * 2;
            if (sc3_main_cont->sc3_main->sc_ins[i].level_num > SC3_LOD_MAX_NUM) return SC3_FILE_ERROR_INS_LOD_MAX;
            scl_fwrite(sc3_main_cont->sc3_f__, sc3_main_cont->sc3_main->sc_ins[i].level, sc_uint, sc3_main_cont->sc3_main->sc_ins[i].level_num);
            ins_size__ += sizeof(sc_uint) * sc3_main_cont->sc3_main->sc_ins[i].level_num;
            for (sc_size j = 0; j < sc3_main_cont->sc3_main->sc_ins[i].group_num; ++j) {
                scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_ins[i].group[j].num, sc_uint, 1);
                scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_ins[i].group[j].center, SGG_Vector3, 1);
                ins_size__ += sizeof(sc_uint) * 1;
                ins_size__ += sizeof(SGG_Vector3) * 1;
            }
        }

        scl_fwrite(sc3_main_cont->sc3_f__, SC3_INSTANCE_DATA_CHUNK, sc_byte, len_of_str(SC3_INSTANCE_DATA_CHUNK));
        ins_size__ += sizeof(sc_byte) * len_of_str(SC3_INSTANCE_DATA_CHUNK);
        for (sc_size i = 0; i < sc3_main_cont->sc3_main->instance_num; ++i) {
            for (sc_size j = 0; j < sc3_main_cont->sc3_main->sc_ins[i].group_num; ++j) {
                scl_fwrite(sc3_main_cont->sc3_f__, sc3_main_cont->sc3_main->sc_ins[i].group[j].trs, SGG_Mat4, sc3_main_cont->sc3_main->sc_ins[i].group[j].num);
                ins_size__ += sizeof(SGG_Mat4) * sc3_main_cont->sc3_main->sc_ins[i].group[j].num;
            }
        }

        *end_size = ftell(sc3_main_cont->sc3_f__);

        goto_sc3f_pos(sc3_main_cont->sc3_f__, now_pos__);
        scl_fwrite(sc3_main_cont->sc3_f__, &ins_size__, sc_size, 1);
        sc_size size = SC3_INS_TABLE_POS;

        goto_sc3f_pos(sc3_main_cont->sc3_f__, size);
        scl_fwrite(sc3_main_cont->sc3_f__, &now_pos__, sc_size, 1);


    }
    return SC3_FILE_OUT_CODE;
}
sc_err_t write_speed_charm_3d_mesh(Speed_Charm_3D_Main_Contents * sc3_main_cont, sc_size * end_size){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!end_size) return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;

    /**
     * [size]
     * [chunk]
     * [raw data num]
     * [raw data inf chunk]
     * [raw data inf]
     * [raw data chunk]
     * [raw data]
    */
   
    goto_sc3f_pos(sc3_main_cont->sc3_f__, *end_size);
    sc_size now_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);
    sc_size mesh_size__ = 0;
    
    scl_fwrite(sc3_main_cont->sc3_f__, &mesh_size__, sc_size, 1);
    
    mesh_size__ += sizeof(sc_size) * 1;
    
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MESH_CHUNK, sc_byte, len_of_str(SC3_MESH_CHUNK));
    mesh_size__ += sizeof(sc_byte) * len_of_str(SC3_MESH_CHUNK);

    scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->meshes_num, sc_size, 1);
    mesh_size__ += sizeof(sc_size) * 1;

    if(!sc3_main_cont->sc3_main->sc_meshes && sc3_main_cont->sc3_main->meshes_num) return SC3_FILE_ERROR_MESH;
    
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MESH_INFORMATION_CHUNK, sc_byte, len_of_str(SC3_MESH_INFORMATION_CHUNK));

    mesh_size__ += sizeof(sc_byte) * len_of_str(SC3_MESH_INFORMATION_CHUNK);
    for(sc_size i__ = 0; i__ < sc3_main_cont->sc3_main->meshes_num; ++i__){
        Speed_Charm_3D_Mesh * sc3_mesh = sc3_main_cont->sc3_main->sc_meshes + i__;           
        scl_fwrite(sc3_main_cont->sc3_f__, &sc3_mesh->attr_num, sc_size, 1);
        mesh_size__ += sizeof(sc_size) * 1;
        scl_fwrite(sc3_main_cont->sc3_f__, &sc3_mesh->ind_num, sc_size, 1);
        mesh_size__ += sizeof(sc_size) * 1;
        if(SC_POLICY_CHECK_MODULE_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_TEXTURE_VALID_VERSION)){
            
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_mesh->material_point, Speed_Charm_3D_Mesh_Material_Point, 1);
            mesh_size__ += sizeof(Speed_Charm_3D_Mesh_Material_Point) * 1;
        }
        if(i__ < (sc3_main_cont->sc3_main->meshes_num  + SC3_OFFSET_INF_NUM) / SC_POLICY_SC_UINT_8_BITS){
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_meshes[i__].mesh_inf, sc_ubyte, 1);
            mesh_size__ += sizeof(sc_ubyte) * 1;
        }
    }
    
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MESH_DATA_CHUNK, sc_byte, len_of_str(SC3_MESH_DATA_CHUNK));
    mesh_size__ += sizeof(sc_byte) * len_of_str(SC3_MESH_DATA_CHUNK);
    for(sc_size i = 0; i < sc3_main_cont->sc3_main->meshes_num; ++i){
        Speed_Charm_3D_Mesh * sc3_mesh__ = sc3_main_cont->sc3_main->sc_meshes + i;
        if(!sc3_mesh__->mesh_attr && sc3_mesh__->attr_num) return SC3_FILE_ERROR_MATCH_MESH_VERT_DATA;
        scl_fwrite(sc3_main_cont->sc3_f__, sc3_mesh__->mesh_attr, Speed_Charm_3D_Mesh_Attr, sc3_mesh__->attr_num);
        mesh_size__ += sizeof(Speed_Charm_3D_Mesh_Attr) * sc3_mesh__->attr_num;
        sc_ubyte idx__ = i / SC_POLICY_SC_UINT_8_BITS;
        sc_ubyte inf__ = (*(sc3_main_cont->sc3_main->sc_meshes + idx__)).mesh_inf >> (i % SC_POLICY_SC_UINT_8_BITS);
        if(HAS_SC3_SKIN(inf__, SC3_MAIN_HAVE_SKIN)) {
            if (!sc3_mesh__->skin_attr && sc3_mesh__->attr_num) return SC3_FILE_ERROR_MATCH_MESH_SKIN_DATA;
            scl_fwrite(sc3_main_cont->sc3_f__, sc3_mesh__->skin_attr, Speed_Charm_3D_Skin_Attr, sc3_mesh__->attr_num);
            mesh_size__ += sizeof(Speed_Charm_3D_Skin_Attr) * sc3_mesh__->attr_num;
        }

        scl_fwrite(sc3_main_cont->sc3_f__, sc3_mesh__->indices, sc_uint, sc3_mesh__->ind_num);
        mesh_size__ += sizeof(sc_uint) * sc3_mesh__->ind_num;

    }
    /*
    if (SC_POLICY_CHECK_MODULE_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_BLEND_MESH_VALID_VERSION)) {

        scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->blend_meshes_num, sc_size, 1);
        mesh_size__ += sizeof(sc_size) * 1;

        if (!sc3_main_cont->sc3_main->sc_blend_meshes && sc3_main_cont->sc3_main->blend_meshes_num) return SC3_FILE_ERROR_MESH;
        scl_fwrite(sc3_main_cont->sc3_f__, SC3_BLEND_MESH_INFORMATION_CHUNK, sc_byte, len_of_str(SC3_BLEND_MESH_INFORMATION_CHUNK));

        mesh_size__ += sizeof(sc_byte) * len_of_str(SC3_BLEND_MESH_INFORMATION_CHUNK);
        for (sc_size i__ = 0; i__ < sc3_main_cont->sc3_main->blend_meshes_num; ++i__) {
            Speed_Charm_3D_Blend_Mesh* sc3_blend_mesh = sc3_main_cont->sc3_main->sc_blend_meshes + i__;
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_blend_mesh->center, SGG_Vector3, 1);
            mesh_size__ += sizeof(SGG_Vector3) * 1;

            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_blend_mesh->sc3_mesh.attr_num, sc_size, 1);
            mesh_size__ += sizeof(sc_size) * 1;
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_blend_mesh->sc3_mesh.ind_num, sc_size, 1);
            mesh_size__ += sizeof(sc_size) * 1;

            if (SC_POLICY_CHECK_MODULE_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_TEXTURE_VALID_VERSION)) {
                scl_fwrite(sc3_main_cont->sc3_f__, &sc3_blend_mesh->sc3_mesh.material_point, Speed_Charm_3D_Mesh_Material_Point, 1);
                mesh_size__ += sizeof(Speed_Charm_3D_Mesh_Material_Point) * 1;
            }
            if (i__ < (sc3_main_cont->sc3_main->blend_meshes_num + SC3_OFFSET_INF_NUM) / SC_POLICY_SC_UINT_8_BITS) {
                scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_blend_meshes[i__].sc3_mesh.mesh_inf, sc_ubyte, 1);
                mesh_size__ += sizeof(sc_ubyte) * 1;
            }
        }

        scl_fwrite(sc3_main_cont->sc3_f__, SC3_BLEND_MESH_DATA_CHUNK, sc_byte, len_of_str(SC3_BLEND_MESH_DATA_CHUNK));
        mesh_size__ += sizeof(sc_byte) * len_of_str(SC3_BLEND_MESH_DATA_CHUNK);
        for (sc_size i = 0; i < sc3_main_cont->sc3_main->blend_meshes_num; ++i) {
            Speed_Charm_3D_Blend_Mesh* sc3_blend_mesh__ = sc3_main_cont->sc3_main->sc_blend_meshes + i;
            if (!sc3_blend_mesh__->sc3_mesh.mesh_attr)
                if (sc3_blend_mesh__->sc3_mesh.attr_num) return SC3_FILE_ERROR_MATCH_MESH_VERT_DATA;
            scl_fwrite(sc3_main_cont->sc3_f__, sc3_blend_mesh__->sc3_mesh.mesh_attr, Speed_Charm_3D_Mesh_Attr, sc3_blend_mesh__->sc3_mesh.attr_num);
            mesh_size__ += sizeof(Speed_Charm_3D_Mesh_Attr) * sc3_blend_mesh__->sc3_mesh.attr_num;
            sc_ubyte idx__ = i / SC_POLICY_SC_UINT_8_BITS;
            sc_ubyte inf__ = (*(sc3_main_cont->sc3_main->sc_blend_meshes + idx__)).sc3_mesh.mesh_inf >> (i % SC_POLICY_SC_UINT_8_BITS);
            if (inf__ & SC3_MAIN_HAVE_SKIN) {
                if (!sc3_blend_mesh__->sc3_mesh.skin_attr)
                    if (sc3_blend_mesh__->sc3_mesh.attr_num) return SC3_FILE_ERROR_MATCH_MESH_SKIN_DATA;

                scl_fwrite(sc3_main_cont->sc3_f__, sc3_blend_mesh__->sc3_mesh.skin_attr, Speed_Charm_3D_Skin_Attr, sc3_blend_mesh__->sc3_mesh.attr_num);
                mesh_size__ += sizeof(Speed_Charm_3D_Skin_Attr) * sc3_blend_mesh__->sc3_mesh.attr_num;
            }

            scl_fwrite(sc3_main_cont->sc3_f__, sc3_blend_mesh__->sc3_mesh.indices, sc_uint, sc3_blend_mesh__->sc3_mesh.ind_num);
            mesh_size__ += sizeof(sc_uint) * sc3_blend_mesh__->sc3_mesh.ind_num;

        }
    }
    */
    *end_size = scl_get_fpos(sc3_main_cont->sc3_f__);
    const sc_size mesh_table_pos__ = SC3_MESH_TABLE_POS;
    goto_sc3f_pos(sc3_main_cont->sc3_f__, mesh_table_pos__);
    scl_fwrite(sc3_main_cont->sc3_f__, &now_pos__, sc_size, 1);
    goto_sc3f_pos(sc3_main_cont->sc3_f__, now_pos__);

    scl_fwrite(sc3_main_cont->sc3_f__, &mesh_size__, sc_size, 1);

    return SC3_FILE_OUT_CODE;
}

sc_err_t read_speed_charm_3d_main(Speed_Charm_3D_Main ** sc3_main, const sc_byte * sc3_path){
    if(!sc3_main) return SC3_FILE_ERROR_VAR_NULL;
    Speed_Charm_3D_Main_Contents * sc3_main_cont__;
    sc3_main_cont__ = init_speed_charm_3d_main_contents(NULL);
    Speed_Charm_3D_Tags sc3_tags;
    sc_err_t error_code__ = read_speed_charm_3d_entry_data(sc3_main_cont__, &sc3_tags, sc3_path);
    if(SC_POLICY_ERROR_CHECK(error_code__)) {
        //scl_close(sc3_main_cont->sc3_f__);
        delete_speed_charm_3d_main_contents(&sc3_main_cont__);
        return error_code__;
    }

    error_code__ = read_speed_charm_3d_table(sc3_main_cont__, &sc3_tags);
    if(SC_POLICY_ERROR_CHECK(error_code__)) {
        scl_close(sc3_main_cont__->sc3_f__);
        delete_speed_charm_3d_main_contents(&sc3_main_cont__);
        return error_code__;
    }
    error_code__ = read_speed_charm_3d_user(sc3_main_cont__, &sc3_tags);
    if(SC_POLICY_ERROR_CHECK(error_code__)) {
        scl_close(sc3_main_cont__->sc3_f__);
        delete_speed_charm_3d_main_contents(&sc3_main_cont__);
        return error_code__;
    }

    error_code__ = read_speed_charm_3d_data(sc3_main_cont__, &sc3_tags);

    if(SC_POLICY_ERROR_CHECK(error_code__)) {
        
        scl_close(sc3_main_cont__->sc3_f__);
        delete_speed_charm_3d_main_contents(&sc3_main_cont__);
        return error_code__;
    }
    error_code__ = read_speed_charm_3d_close(sc3_main_cont__, &sc3_tags);
    if(SC_POLICY_ERROR_CHECK(error_code__)) {
        delete_speed_charm_3d_main_contents(&sc3_main_cont__);
        return error_code__;
    }
    *sc3_main = sc3_main_cont__->sc3_main;
    sc3_main_cont__->sc3_main = NULL;
    delete_speed_charm_3d_main_contents(&sc3_main_cont__);

    return SC3_FILE_OUT_CODE;
}

sc_err_t read_speed_charm_3d_entry_data(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags, const sc_byte * sc3_path){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_tags)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_path)return SC3_FILE_ERROR_VAR_NULL;
    sc_byte sc3_file_path__[SC3_FILE_NAME_DEFAULT_SIZE] = {0};
    if(!ext_str_cpy(sc3_file_path__, sc3_path, SC3_FILE_NAME_DEFAULT_SIZE, safety_strlen(sc3_path) + 1)) return SC3_FILE_ERROR_VAR_NULL;
    set_sc3_file_format(sc3_file_path__);
    if(scl_fopen(sc3_main_cont->sc3_f__, sc3_file_path__, "rb")) return SC3_FILE_R_FOPEN;

    if(!smart_sc3_read_tags_match(sc3_tags->sc3_entry_magic_num, SC3_FILE_ENTRY_MAGIC_NUMBER, len_of_str(SC3_FILE_ENTRY_MAGIC_NUMBER), sc3_main_cont->sc3_f__)) return SC3_FILE_READ_ENTRY_MAGIC_NUM;

    scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_version, sc_ushort, 1);   
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;
    if(!smart_sc3_read_tags_match(sc3_tags->sc3_beg, SC_POLICY_REQUIRE_BEG_CHUNK, len_of_str(SC_POLICY_REQUIRE_BEG_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_READ_BEG;
    if(!smart_sc3_read_tags_match(sc3_tags->sc3_tbl, SC3_MAIN_TABLE, len_of_str(SC3_MAIN_TABLE), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_MAIN_TABLE;
        //fwrite(SC3_MAIN_BEG, sizeof(sc_byte), STRING_LITERAL_SIZE(SC3_MAIN_BEG), sc3_main_cont->sc3_f__);
        //fwrite(SC3_MAIN_TABLE, sizeof(sc_byte), STRING_LITERAL_SIZE(SC3_MAIN_TABLE), sc3_main_cont->sc3_f__);
    return SC3_FILE_OUT_CODE;
    
}

sc_err_t read_speed_charm_3d_table(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){
    if(!sc3_main_cont || !sc3_tags)return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC_POLICY_ERROR_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;
    if(!smart_sc3_read_tags_match(sc3_tags->sc3_tbl, SC3_MAIN_TABLE_MESH, len_of_str(SC3_MAIN_TABLE_MESH), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_MESH_TABLE;
    scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->mesh_pos, sizeof(sc_size), 1);
    //fwrite(SC3_MAIN_TABLE_MESH, sizeof(sc_byte), STRING_LITERAL_SIZE(SC3_MAIN_TABLE_MESH), sc3_main_cont->sc3_f__);
    //fwrite(&booking_table_pos, sizeof(sc_size), 1, sc3_main_cont->sc3_f__);
    if(!smart_sc3_read_tags_match(sc3_tags->sc3_tbl, SC3_MAIN_TABLE_BONE, len_of_str(SC3_MAIN_TABLE_BONE), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_BONE_TABLE;
    scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->bone_pos, sizeof(sc_size), 1);
    if(!smart_sc3_read_tags_match(sc3_tags->sc3_tbl, SC3_MAIN_TABLE_ANIM, len_of_str(SC3_MAIN_TABLE_ANIM), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_BONE_TABLE;
    scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->anim_pos, sizeof(sc_size), 1);

    //if(smart_sc3_tags_reads(sc3_tags->sc3_tbl, SC3_MAIN_TABLE_ANIM, STRING_LITERAL_SIZE(SC3_MAIN_TABLE_TEXTURE), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_BONE_TABLE;
    //scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->anim_pos, sizeof(sc_byte), 1);
    if(sc3_main_cont->sc3_version >= SC3_TEXTURE_VALID_VERSION){
        if(!smart_sc3_read_tags_match(sc3_tags->sc3_tbl, SC3_MAIN_TABLE_TEXTURE, len_of_str(SC3_MAIN_TABLE_TEXTURE), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_TEX_TABLE;
        scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->tex_pos, sizeof(sc_size), 1);
    }

    if (sc3_main_cont->sc3_version >= SC3_INSTANCE_VALID_VERSION) {
        if (!smart_sc3_read_tags_match(sc3_tags->sc3_tbl, SC3_MAIN_TABLE_INSTANCE, len_of_str(SC3_MAIN_TABLE_INSTANCE), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_INS_TABLE;
        scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->ins_pos, sizeof(sc_size), 1);
    }
    return SC3_FILE_OUT_CODE;
}

sc_err_t read_speed_charm_3d_user(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_tags)return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC_POLICY_ERROR_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;
    if(!smart_sc3_read_tags_match(sc3_tags->sc3_usr, SC3_MAIN_USER, len_of_str(SC3_MAIN_USER), sc3_main_cont->sc3_f__)) return SC3_FILE_READ_USR;
    if(!smart_sc3_read_tags_match(sc3_tags->sc3_usr, SC3_MAIN_COMMENT, STRING_LITERAL_SIZE(SC3_MAIN_COMMENT), sc3_main_cont->sc3_f__)) return SC3_FILE_READ_COM;
    scl_fread(sc3_main_cont->sc3_f__, sc3_tags->sc3_com, sc_byte, SC3_COMMENT_SIZE_MAX);
    sc_byte vd_data__[SC3_FREE_DATA_SIZE] = {0};
    if(!smart_sc3_read_tags_match(sc3_tags->sc3_usr, SC3_FREE_DATA_CHUNK, len_of_str(SC3_FREE_DATA_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_VIRTUAL_DATA;
    scl_fread(sc3_main_cont->sc3_f__, vd_data__, sc_byte, SC3_FREE_DATA_SIZE);
    return SC3_FILE_OUT_CODE;
}

sc_err_t read_speed_charm_3d_close(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main){
        scl_close(sc3_main_cont->sc3_f__);
        return SC3_FILE_ERROR_VAR_NULL;
    }
    if(!sc3_tags) {
        scl_close(sc3_main_cont->sc3_f__);
        return SC3_FILE_ERROR_VAR_NULL;
    }
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) {
        scl_close(sc3_main_cont->sc3_f__);
        return SC3_INVALID_VERSION;
    }
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) {
        scl_close(sc3_main_cont->sc3_f__);
        return SC3_INVALID_VERSION;
    }
    fseek(sc3_main_cont->sc3_f__, 0, SEEK_END);
    fseek(sc3_main_cont->sc3_f__, -len_of_str(SC3_MAIN_END), SEEK_CUR);

    if(!smart_sc3_read_tags_match(sc3_tags->sc3_end, SC3_MAIN_END, len_of_str(SC3_MAIN_END), sc3_main_cont->sc3_f__)) return SC3_FILE_READ_END;
    if(!SC_POLICY_REQUIRE_CHECK_END_NEXT(fgetc(sc3_main_cont->sc3_f__))) {
        scl_close(sc3_main_cont->sc3_f__);
        return SC3_FILE_ERROR_FILE_END_NEXT;
    }
    scl_close(sc3_main_cont->sc3_f__);
    return SC3_FILE_OUT_CODE;
}
sc_err_t read_speed_charm_3d_data(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){ 
    sc_err_t error_code__ = read_speed_charm_3d_mesh(sc3_main_cont, sc3_tags);
    if(SC_POLICY_ERROR_CHECK(error_code__)) return error_code__;

    error_code__ = read_speed_charm_3d_bone(sc3_main_cont, sc3_tags);
    //printf("DEBUFG%d\n", error_code__);
                //printf("debug out %i %u\n", 1, error_code);    
    if(SC_POLICY_ERROR_CHECK(error_code__)) return error_code__;


    error_code__ = read_speed_charm_3d_anim(sc3_main_cont, sc3_tags);

    if(SC_POLICY_ERROR_CHECK(error_code__)) return error_code__;

    error_code__ = read_speed_charm_3d_tex(sc3_main_cont, sc3_tags);
    if (SC_POLICY_ERROR_CHECK(error_code__)) return error_code__;
    error_code__ = read_speed_charm_3d_ins(sc3_main_cont, sc3_tags);
    
    return SC3_FILE_OUT_CODE;
}
sc_err_t read_speed_charm_3d_ins(Speed_Charm_3D_Main_Contents* sc3_main_cont, Speed_Charm_3D_Tags* sc3_tags) {
    if (!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if (!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if (!sc3_tags) return SC3_FILE_ERROR_VAR_NULL;

    if (!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;

    if (!SC_POLICY_CHECK_MODULE_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_INSTANCE_VALID_VERSION)) return SC3_INVALID_VERSION;

    goto_sc3f_pos(sc3_main_cont->sc3_f__, sc3_main_cont->ins_pos);


    sc_size sum_check_size__;
    scl_fread(sc3_main_cont->sc3_f__, &sum_check_size__, sc_size, 1);

    if (!smart_sc3_read_tags_match(sc3_tags->sc3_chunks, SC3_INSTANCE_CHUNK, len_of_str(SC3_INSTANCE_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_INS;

    sc_size ins_num_;
    scl_fread(sc3_main_cont->sc3_f__, &ins_num_, sc_size, 1);

    sc3_main_cont->sc3_main->instance_num = ins_num_;

    Speed_Charm_3D_Instance* sc_ins =
        smart_calloc(Speed_Charm_3D_Instance, ins_num_);

    if (!sc_ins && ins_num_) return SC_POLICY_ERROR_ALLOC;

    sc3_main_cont->sc3_main->sc_ins = sc_ins;

    if (!smart_sc3_read_tags_match(sc3_tags->sc3_chunks, SC3_INSTANCE_INFORMATION_CHUNK, len_of_str(SC3_INSTANCE_INFORMATION_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_INS_INFORMATION;

    for (sc_size i = 0; i < ins_num_; ++i) {
        sc_uint group_;
        scl_fread(sc3_main_cont->sc3_f__, &group_, sc_uint, 1);

        sc_ins[i].group_num = group_;

        Speed_Charm_3D_Instance_Group* groups =
            smart_calloc(Speed_Charm_3D_Instance_Group, group_);

        if (!groups && group_) return SC_POLICY_ERROR_ALLOC;

        sc_ins[i].group = groups;

        scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_ins[i].level_num, sc_uint, 1);
        if (sc3_main_cont->sc3_main->sc_ins[i].level_num > SC3_LOD_MAX_NUM) return SC3_FILE_ERROR_INS_LOD_MAX;
        scl_fread(sc3_main_cont->sc3_f__, sc3_main_cont->sc3_main->sc_ins[i].level, sc_uint, sc3_main_cont->sc3_main->sc_ins[i].level_num);
        for (sc_size j = 0; j < group_; ++j) {

            sc_uint num_;
            scl_fread(sc3_main_cont->sc3_f__, &num_, sc_uint, 1);
            scl_fread(sc3_main_cont->sc3_f__, &groups[j].center, SGG_Vector3, 1);


            groups[j].num = num_;

            SGG_Mat4* trs =
                smart_calloc(SGG_Mat4, num_);

            if (!trs && num_) return SC_POLICY_ERROR_ALLOC;

            groups[j].trs = trs;
        }
    }

    if (!smart_sc3_read_tags_match(sc3_tags->sc3_chunks, SC3_INSTANCE_DATA_CHUNK, len_of_str(SC3_INSTANCE_DATA_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_INS_DATA;

    for (sc_size i = 0; i < ins_num_; ++i) {
        Speed_Charm_3D_Instance* lod_ins = &sc_ins[i];
        for (sc_size j = 0; j < lod_ins->group_num; ++j) {
            Speed_Charm_3D_Instance_Group* group = &lod_ins->group[j];
            scl_fread(sc3_main_cont->sc3_f__, group->trs, SGG_Mat4, group->num);
        }
    }


    sc_size end_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);
    if (sum_check_size__ != end_pos__ - sc3_main_cont->ins_pos) return SC3_FILE_ERROR_SUM_CHECK;

    return SC3_FILE_OUT_CODE;
}
sc_err_t read_speed_charm_3d_tex(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_tags) return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;

    if(!SC_POLICY_CHECK_MODULE_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_TEXTURE_VALID_VERSION)) return SC3_INVALID_VERSION;
    
    goto_sc3f_pos(sc3_main_cont->sc3_f__, sc3_main_cont->tex_pos);


    sc_size sum_check_size__;
    scl_fread(sc3_main_cont->sc3_f__, &sum_check_size__, sc_size, 1);
    
    if(!smart_sc3_read_tags_match(sc3_tags->sc3_chunks, SC3_MATERIAL_TEXTURE_CHUNK, len_of_str(SC3_MATERIAL_TEXTURE_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_TEX;
    
    scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->materials_num, sc_size, 1);
    if(!sc3_main_cont->sc3_main->materials_num){

        sc_size end_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);
        if(sum_check_size__ != end_pos__ - sc3_main_cont->tex_pos) return SC3_FILE_ERROR_SUM_CHECK;
        return SC3_FILE_OUT_CODE;

    }
    sc3_main_cont->sc3_main->sc_materials = smart_calloc(Speed_Charm_3D_Material_Buffers, sc3_main_cont->sc3_main->materials_num);
    if(!sc3_main_cont->sc3_main->sc_materials) return SC3_FILE_ERROR_VAR_NULL;
    for(sc_size i = 0; i < sc3_main_cont->sc3_main->materials_num; ++i){
        scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_materials[i].sc_has_material, sc_ushort, 1);
        if(sc3_main_cont->sc3_main->sc_materials[i].sc_has_material == SC3_HAS_TEXTURES){
            scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_materials[i].material_map.x, sc_size, 1);
            scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_materials[i].material_map.y, sc_size, 1);
            sc3_main_cont->sc3_main->sc_materials[i].material_map.img_data = smart_malloc(sc_ubyte, sc3_main_cont->sc3_main->sc_materials[i].material_map.x * sc3_main_cont->sc3_main->sc_materials[i].material_map.y * SC3_TEXTURE_MATERIAL_CHANNEL);
        }
    }

    bool is_raw;
    sc_size total_size;
    scl_fread(sc3_main_cont->sc3_f__, &is_raw, bool, 1);
    scl_fread(sc3_main_cont->sc3_f__, &total_size, sc_size, 1);
    sc_ubyte * r_buffer = smart_malloc(sc_ubyte, total_size);
    if(!r_buffer) return SC3_FILE_ERROR_VAR_NULL;
    scl_fread(sc3_main_cont->sc3_f__, r_buffer, sc_ubyte, total_size);
    if(!is_raw){
        sc_ubyte * rd_buffer = sc3_de_zstd_compress(
            r_buffer, total_size
        );
        s_free(r_buffer);
        if(!rd_buffer){
            return SC3_FILE_ERROR_DE_COMPRESSION;
        }
        r_buffer = rd_buffer;
    }

    for(sc_size i = 0, offset = 0; i < sc3_main_cont->sc3_main->materials_num; ++i){

        if(sc3_main_cont->sc3_main->sc_materials[i].sc_has_material == SC3_HAS_TEXTURES){

            smart_memcpy_p(sc3_main_cont->sc3_main->sc_materials[i].material_map.img_data, r_buffer + offset, sc_ubyte, sc3_main_cont->sc3_main->sc_materials[i].material_map.x * sc3_main_cont->sc3_main->sc_materials[i].material_map.y * SC3_TEXTURE_MATERIAL_CHANNEL);
            offset += sizeof(sc_ubyte) * sc3_main_cont->sc3_main->sc_materials[i].material_map.x * sc3_main_cont->sc3_main->sc_materials[i].material_map.y * SC3_TEXTURE_MATERIAL_CHANNEL;
        }
        else if(sc3_main_cont->sc3_main->sc_materials[i].sc_has_material == SC3_HAS_VALUES_VECTOR){
            smart_memcpy_p(&sc3_main_cont->sc3_main->sc_materials[i].material_value3, r_buffer + offset, SGG_Vector3, 1);
            offset += sizeof(SGG_Vector3) * 1;
        }
        else if(sc3_main_cont->sc3_main->sc_materials[i].sc_has_material == SC3_HAS_VALUES_VECTOR_ALPHA){
            smart_memcpy_p(&sc3_main_cont->sc3_main->sc_materials[i].material_value4, r_buffer + offset, SGG_Vector4, 1);
            offset += sizeof(SGG_Vector4) * 1;
        }
        else if(sc3_main_cont->sc3_main->sc_materials[i].sc_has_material == SC3_HAS_VALUES_SCALER){
            smart_memcpy_p(&sc3_main_cont->sc3_main->sc_materials[i].material_value, r_buffer + offset, float, 1);
            offset += sizeof(float) * 1;
        }

    }
    s_free(r_buffer);
    sc_size end_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);
    if(sum_check_size__ != end_pos__ - sc3_main_cont->tex_pos) return SC3_FILE_ERROR_SUM_CHECK;

    return SC3_FILE_OUT_CODE;
}

sc_err_t read_speed_charm_3d_bone(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){
    
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_tags) return SC3_FILE_ERROR_VAR_NULL;

    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;

    goto_sc3f_pos(sc3_main_cont->sc3_f__, sc3_main_cont->bone_pos);

    sc_size sum_check_size__ = 0;
    scl_fread(sc3_main_cont->sc3_f__, &sum_check_size__, sc_size, 1);

    if(!smart_sc3_read_tags_match(sc3_tags->sc3_chunks, SC3_BONE_CHUNK, len_of_str(SC3_BONE_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_BONE;
    sc_size bones_num = 0;
    scl_fread(sc3_main_cont->sc3_f__, &bones_num, sc_size, 1);
    sc3_main_cont->sc3_main->bones_num = bones_num;
    sc3_main_cont->sc3_main->sc_bones = smart_malloc(Speed_Charm_3D_Bone_Map, sc3_main_cont->sc3_main->bones_num);
    sc_size skeletons_num = 0;
    scl_fread(sc3_main_cont->sc3_f__, &skeletons_num, sc_size, 1);
    sc3_main_cont->sc3_main->skeletons_num = skeletons_num;
    sc3_main_cont->sc3_main->sc_skeletons = smart_malloc(Speed_Charm_3D_Skeleton, sc3_main_cont->sc3_main->skeletons_num);

    /*num is 0 => OK*/
    if (!sc3_main_cont->sc3_main->sc_bones && sc3_main_cont->sc3_main->bones_num) return SC_POLICY_ERROR_ALLOC;
    if (!sc3_main_cont->sc3_main->sc_skeletons && sc3_main_cont->sc3_main->skeletons_num) return SC_POLICY_ERROR_ALLOC;
       
    scl_fread(sc3_main_cont->sc3_f__, sc3_main_cont->sc3_main->sc_bones, Speed_Charm_3D_Bone_Map, sc3_main_cont->sc3_main->bones_num);
    if(!SC_POLICY_CHECK_MODULE_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_DYNAMIC_POINT_VERSION)){
        Speed_Charm_3D_Skeleton__ * old_skeleton_struct = smart_malloc(Speed_Charm_3D_Skeleton__, sc3_main_cont->sc3_main->skeletons_num);
        if(!old_skeleton_struct) return SC_POLICY_ERROR_ALLOC;

        scl_fread(sc3_main_cont->sc3_f__, old_skeleton_struct, Speed_Charm_3D_Skeleton__, sc3_main_cont->sc3_main->skeletons_num);
        for(sc_size i = 0; i < sc3_main_cont->sc3_main->skeletons_num; ++i){
            sc3_main_cont->sc3_main->sc_skeletons[i].node = old_skeleton_struct[i].node;
            sc3_main_cont->sc3_main->sc_skeletons[i].parent = old_skeleton_struct[i].parent;
            sc3_main_cont->sc3_main->sc_skeletons[i].dynamic_point = NOT_DYNAMIC_POINT;
        }
        s_free(old_skeleton_struct);
        
    }
    else{
        scl_fread(sc3_main_cont->sc3_f__, sc3_main_cont->sc3_main->sc_skeletons, Speed_Charm_3D_Skeleton, sc3_main_cont->sc3_main->skeletons_num);
    }
    
    sc_size end_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);
                //printf("debug out %i %u\n", 1, 1);    
    //printf("debug out %i %u %u\n", end_pos, sum_check_size__, sc3_main_cont->bone_pos);    
    if(sum_check_size__ != end_pos__ - sc3_main_cont->bone_pos) return SC3_FILE_ERROR_SUM_CHECK;
    
    return SC3_FILE_OUT_CODE;

}

sc_err_t read_speed_charm_3d_anim(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_tags) return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;
    goto_sc3f_pos(sc3_main_cont->sc3_f__, sc3_main_cont->anim_pos);

    sc_size sum_check_size__ = 0;
    
    scl_fread(sc3_main_cont->sc3_f__, &sum_check_size__, sc_size, 1);
    
    if(!smart_sc3_read_tags_match(sc3_tags->sc3_chunks, SC3_ANIM_CHUNK, len_of_str(SC3_ANIM_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_ANIM;
    
    scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->anim_num, sc_size, 1);
    
    Speed_Charm_3D_Anim * new_sc3_anim__ = smart_malloc(Speed_Charm_3D_Anim, sc3_main_cont->sc3_main->anim_num);

    if(!new_sc3_anim__ && sc3_main_cont->sc3_main->anim_num) return SC_POLICY_ERROR_ALLOC;
    
    for(sc_size i = 0; i < sc3_main_cont->sc3_main->anim_num; ++i) new_sc3_anim__[i].anim_deforms = NULL;

    sc3_main_cont->sc3_main->sc_anim = new_sc3_anim__;
    
    if(!smart_sc3_read_tags_match(sc3_tags->sc3_chunks, SC3_ANIM_INFORMATION_CHUNK, len_of_str(SC3_ANIM_INFORMATION_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_ANIM_INFORMATION;
    for(sc_size i = 0; i < sc3_main_cont->sc3_main->anim_num; ++i){
        Speed_Charm_3D_Anim * sc3_anim__ = sc3_main_cont->sc3_main->sc_anim + i;
        scl_fread(sc3_main_cont->sc3_f__, &sc3_anim__->frame_num, sc_size, 1);

        if(sc3_anim__->frame_num >= SC3_MAX_FRAME_NUM) return SC3_ANIM_INFORMATION_ERROR_FRAME;
        scl_fread(sc3_main_cont->sc3_f__, &sc3_anim__->time_beg, float, 1);
        
        scl_fread(sc3_main_cont->sc3_f__, &sc3_anim__->time_end, float, 1);
        
        scl_fread(sc3_main_cont->sc3_f__, &sc3_anim__->framerate, float, 1);
        if(sc3_anim__->framerate >= SC3_FRAMERATE_60) return SC3_ANIM_INFORMATION_ERROR_FRAMERATE;

        sc3_anim__->anim_deforms = smart_calloc(Speed_Charm_3D_Anim_Deform, sc3_main_cont->sc3_main->skeletons_num);
        
        if(!sc3_anim__->anim_deforms && sc3_main_cont->sc3_main->skeletons_num) return SC_POLICY_ERROR_ALLOC;
        
        sc_size opt_size__ = (sc3_main_cont->sc3_main->skeletons_num + 1) / 2;

        sc_ubyte * deform_cache__ = smart_malloc(sc_ubyte, opt_size__);
        if(!deform_cache__) return SC_POLICY_ERROR_ALLOC;

        scl_fread(sc3_main_cont->sc3_f__, deform_cache__, sc_ubyte, opt_size__);
        for(sc_size j = 0; j < opt_size__; ++j){
            sc_size dst__ = j * 2;
            Speed_Charm_3D_Anim_Deform * sc3_anim_def__ = sc3_anim__->anim_deforms + dst__;

            /**
            * 01110111
            * 11111111
            * 00001111
            * 01101111
            */
            sc_ubyte tmp_l = ((sc_ubyte)~0u >> SC3_HALF_UINT_8_BITS);
            sc3_anim_def__->deform_status = deform_cache__[j] & ((sc_ubyte)~0u >> SC3_HALF_UINT_8_BITS);

            if(dst__ + 1 >= sc3_main_cont->sc3_main->skeletons_num) break;
            sc3_anim_def__ += 1;
            sc3_anim_def__->deform_status = (deform_cache__[j] & ((sc_ubyte)~0u << SC3_HALF_UINT_8_BITS)) >> SC3_HALF_UINT_8_BITS;
        }
        s_free(deform_cache__);
    }
    
    if(!smart_sc3_read_tags_match(sc3_tags->sc3_chunks, SC3_ANIM_DATA_CHUNK, len_of_str(SC3_ANIM_DATA_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_ANIM_DATA;
    for(sc_size i = 0; i < sc3_main_cont->sc3_main->anim_num; ++i){
        Speed_Charm_3D_Anim * sc3_anim__ = sc3_main_cont->sc3_main->sc_anim + i;
        for(sc_size j = 0; j < sc3_main_cont->sc3_main->skeletons_num; ++j){
            Speed_Charm_3D_Anim_Deform * sc3_anim_def__ = sc3_anim__->anim_deforms + j;
            //scl_fread(sc3_main_cont->sc3_f__,&sc3_main_cont->sc3_main->sc_anim[i].anim_deforms[j].deform_status, sc_ubyte, 1);
            sc_size deforms_num__ = sc3_anim_def__->deform_status & SC3_DEFORM_ROT_STATIC ? 1 : sc3_anim__->frame_num;
            sc3_anim_def__->rot = smart_malloc(SGG_Quaternion, deforms_num__);
            if(!sc3_anim_def__->rot) return SC_POLICY_ERROR_ALLOC;
            scl_fread(sc3_main_cont->sc3_f__, sc3_anim_def__->rot, SGG_Quaternion, deforms_num__);
            
            deforms_num__ = sc3_anim_def__->deform_status & SC3_DEFORM_POS_STATIC ? 1 : sc3_anim__->frame_num;
            sc3_anim_def__->pos = smart_malloc(SGG_Vector3, deforms_num__);
            if(!sc3_anim_def__->pos) return SC_POLICY_ERROR_ALLOC;
            
            scl_fread(sc3_main_cont->sc3_f__, sc3_anim_def__->pos, SGG_Vector3, deforms_num__);
            deforms_num__ = sc3_anim_def__->deform_status & SC3_DEFORM_SCALE_STATIC ? 1 : sc3_anim__->frame_num;
            sc3_anim_def__->scale = smart_malloc(SGG_Vector3, deforms_num__);
            if(!sc3_anim_def__->scale) return SC_POLICY_ERROR_ALLOC;
            scl_fread(sc3_main_cont->sc3_f__, sc3_anim_def__->scale, SGG_Vector3, deforms_num__);
        }
    }
    sc_size end_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);

    if(sum_check_size__ != end_pos__ - sc3_main_cont->anim_pos) return SC3_FILE_ERROR_SUM_CHECK;
    return SC3_FILE_OUT_CODE;

}

sc_err_t read_speed_charm_3d_mesh(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_tags) return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;
    
    /**
     * Search for table pos.
    */
    goto_sc3f_pos(sc3_main_cont->sc3_f__, sc3_main_cont->mesh_pos);
    sc_size sum_check_mesh_size__;
    scl_fread(sc3_main_cont->sc3_f__, &sum_check_mesh_size__, sc_size, 1);

    if(!smart_sc3_read_tags_match(sc3_tags->sc3_chunks, SC3_MESH_CHUNK, len_of_str(SC3_MESH_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_MESH;

    scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->meshes_num, sc_size, 1);

    sc3_main_cont->sc3_main->sc_meshes = smart_calloc(Speed_Charm_3D_Mesh, sc3_main_cont->sc3_main->meshes_num);

    if(!sc3_main_cont->sc3_main->sc_meshes && sc3_main_cont->sc3_main->meshes_num) return SC3_FILE_ERROR_MESH;

    if(!smart_sc3_read_tags_match(sc3_tags->sc3_chunks, SC3_MESH_INFORMATION_CHUNK, len_of_str(SC3_MESH_INFORMATION_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_MESH_INFORMATION;
    for(sc_size i = 0; i < sc3_main_cont->sc3_main->meshes_num; ++i){
        Speed_Charm_3D_Mesh * sc3_mesh__ = sc3_main_cont->sc3_main->sc_meshes + i;
        scl_fread(sc3_main_cont->sc3_f__, &sc3_mesh__->attr_num, sc_size, 1);
        scl_fread(sc3_main_cont->sc3_f__, &sc3_mesh__->ind_num, sc_size, 1);
        if(SC_POLICY_CHECK_MODULE_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_TEXTURE_VALID_VERSION)){
    
            scl_fread(sc3_main_cont->sc3_f__, &sc3_mesh__->material_point, Speed_Charm_3D_Mesh_Material_Point, 1);
        }
        if(i < (sc3_main_cont->sc3_main->meshes_num + SC3_OFFSET_INF_NUM) / SC_POLICY_SC_UINT_8_BITS) {
            sc_ubyte tmp_inf__;
            scl_fread(sc3_main_cont->sc3_f__, &tmp_inf__, sc_ubyte, 1);
            for(sc_size j = 0; j < SC_POLICY_SC_UINT_8_BITS; ++j){
                sc_ubyte emp__ = 1 << j;
                sc_ubyte inf__ = (emp__ & tmp_inf__) >> j;
                if(i * SC_POLICY_SC_UINT_8_BITS + j >= sc3_main_cont->sc3_main->meshes_num) break;
                (*(sc3_main_cont->sc3_main->sc_meshes + i * SC_POLICY_SC_UINT_8_BITS + j)).mesh_inf = inf__;
            }

        }
    }
    if(!smart_sc3_read_tags_match(sc3_tags->sc3_chunks, SC3_MESH_DATA_CHUNK, len_of_str(SC3_MESH_DATA_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_MESH_DATA;

    for(sc_size i = 0; i < sc3_main_cont->sc3_main->meshes_num; ++i){
        
        Speed_Charm_3D_Mesh * sc3_mesh__ = sc3_main_cont->sc3_main->sc_meshes + i;
        Speed_Charm_3D_Mesh_Attr * mesh_attr__ = smart_malloc(Speed_Charm_3D_Mesh_Attr, sc3_mesh__->attr_num);
        if(!mesh_attr__) return SC_POLICY_ERROR_ALLOC;

        sc3_mesh__->mesh_attr = mesh_attr__;
        scl_fread(sc3_main_cont->sc3_f__, sc3_mesh__->mesh_attr, Speed_Charm_3D_Mesh_Attr, sc3_mesh__->attr_num);

        if(HAS_SC3_SKIN(sc3_mesh__->mesh_inf)) {
            Speed_Charm_3D_Skin_Attr * skin_attr__ =  smart_malloc(Speed_Charm_3D_Skin_Attr, sc3_mesh__->attr_num);
            
            if(!skin_attr__) return SC_POLICY_ERROR_ALLOC;
            sc3_mesh__->skin_attr = skin_attr__;
            scl_fread(sc3_main_cont->sc3_f__, sc3_mesh__->skin_attr, Speed_Charm_3D_Skin_Attr, sc3_mesh__->attr_num);
            


        }
        sc_uint * indices__ = smart_malloc(sc_uint, sc3_mesh__->ind_num);
        if(!indices__) return SC_POLICY_ERROR_ALLOC;
        sc3_mesh__->indices = indices__;
        scl_fread(sc3_main_cont->sc3_f__, sc3_mesh__->indices, sc_uint, sc3_mesh__->ind_num);
    }
    /*
    if (SC_POLICY_CHECK_MODULE_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_BLEND_MESH_VALID_VERSION)) {
        scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->blend_meshes_num, sc_size, 1);

        sc3_main_cont->sc3_main->sc_blend_meshes = smart_calloc(Speed_Charm_3D_Blend_Mesh, sc3_main_cont->sc3_main->blend_meshes_num);

        if (!sc3_main_cont->sc3_main->sc_blend_meshes && sc3_main_cont->sc3_main->blend_meshes_num) return SC3_FILE_ERROR_MESH;

        if (!smart_sc3_tags_reads(sc3_tags->sc3_chunks, SC3_BLEND_MESH_INFORMATION_CHUNK, len_of_str(SC3_BLEND_MESH_INFORMATION_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_MESH_INFORMATION;
        for (sc_size i = 0; i < sc3_main_cont->sc3_main->blend_meshes_num; ++i) {

            Speed_Charm_3D_Blend_Mesh* sc3_blend_mesh__ = sc3_main_cont->sc3_main->sc_blend_meshes + i;
            scl_fread(sc3_main_cont->sc3_f__, &sc3_blend_mesh__->center, SGG_Vector3, 1);

            scl_fread(sc3_main_cont->sc3_f__, &sc3_blend_mesh__->sc3_mesh.attr_num, sc_size, 1);
            scl_fread(sc3_main_cont->sc3_f__, &sc3_blend_mesh__->sc3_mesh.ind_num, sc_size, 1);
            if (SC_POLICY_CHECK_MODULE_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_TEXTURE_VALID_VERSION)) {

                scl_fread(sc3_main_cont->sc3_f__, &sc3_blend_mesh__->sc3_mesh.material_point, Speed_Charm_3D_Mesh_Material_Point, 1);
            }
            if (i < (sc3_main_cont->sc3_main->blend_meshes_num + SC3_OFFSET_INF_NUM) / SC_POLICY_SC_UINT_8_BITS) {
                sc_ubyte tmp_inf__;
                scl_fread(sc3_main_cont->sc3_f__, &tmp_inf__, sc_ubyte, 1);
                for (sc_size j = 0; j < SC_POLICY_SC_UINT_8_BITS; ++j) {
                    sc_ubyte emp__ = 1 << j;
                    sc_ubyte inf__ = (emp__ & tmp_inf__) >> j;
                    if (i * SC_POLICY_SC_UINT_8_BITS + j >= sc3_main_cont->sc3_main->blend_meshes_num) break;
                    (*(sc3_main_cont->sc3_main->sc_blend_meshes + i * SC_POLICY_SC_UINT_8_BITS + j)).sc3_mesh.mesh_inf = inf__;
                }

            }
        }
        if (!smart_sc3_tags_reads(sc3_tags->sc3_chunks, SC3_BLEND_MESH_DATA_CHUNK, len_of_str(SC3_BLEND_MESH_DATA_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_MESH_DATA;

        for (sc_size i = 0; i < sc3_main_cont->sc3_main->blend_meshes_num; ++i) {

            Speed_Charm_3D_Blend_Mesh* sc3_blend_mesh__ = sc3_main_cont->sc3_main->sc_blend_meshes + i;
            Speed_Charm_3D_Mesh_Attr* mesh_attr__ = smart_malloc(Speed_Charm_3D_Mesh_Attr, sc3_blend_mesh__->sc3_mesh.attr_num);
            if (!mesh_attr__) return SC_POLICY_ERROR_ALLOC;

            sc3_blend_mesh__->sc3_mesh.mesh_attr = mesh_attr__;
            scl_fread(sc3_main_cont->sc3_f__, sc3_blend_mesh__->sc3_mesh.mesh_attr, Speed_Charm_3D_Mesh_Attr, sc3_blend_mesh__->sc3_mesh.attr_num);

            if (sc3_blend_mesh__->sc3_mesh.mesh_inf & SC3_MAIN_HAVE_SKIN) {
                Speed_Charm_3D_Skin_Attr* skin_attr__ = smart_malloc(Speed_Charm_3D_Skin_Attr, sc3_blend_mesh__->sc3_mesh.attr_num);

                if (!skin_attr__) return SC_POLICY_ERROR_ALLOC;
                sc3_blend_mesh__->sc3_mesh.skin_attr = skin_attr__;
                scl_fread(sc3_main_cont->sc3_f__, sc3_blend_mesh__->sc3_mesh.skin_attr, Speed_Charm_3D_Skin_Attr, sc3_blend_mesh__->sc3_mesh.attr_num);

                //scl_fread(sc3_main_cont->sc3_f__, &sc3_mesh->mesh_bone_map_num, sc_size, 1);
                //sc3_mesh->mesh_bone_map = smart_malloc(Speed_Charm_3D_Mesh_Bone_Map, sc3_mesh->mesh_bone_map_num);
                //if(!sc3_mesh->mesh_bone_map) return SC_POLICY_ERROR_ALLOC;

                //scl_fread(sc3_main_cont->sc3_f__, sc3_mesh->mesh_bone_map, Speed_Charm_3D_Mesh_Bone_Map, sc3_me

            }
            sc_uint* indices__ = smart_malloc(sc_uint, sc3_blend_mesh__->sc3_mesh.ind_num);
            if (!indices__) return SC_POLICY_ERROR_ALLOC;
            sc3_blend_mesh__->sc3_mesh.indices = indices__;
            scl_fread(sc3_main_cont->sc3_f__, sc3_blend_mesh__->sc3_mesh.indices, sc_uint, sc3_blend_mesh__->sc3_mesh.ind_num);
        }

    }
    */
    sc_size end_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);

    if(sum_check_mesh_size__ != end_pos__ - sc3_main_cont->mesh_pos) return SC3_FILE_ERROR_SUM_CHECK;
    return SC3_FILE_OUT_CODE;
}

void update_animation_speed_charm_3d_bone(Speed_Charm_3D_Run_Main * sc3_main, sc_uint *anim_num, float delta_time){
    if(!sc3_main->sc_anim) return;
    *anim_num = *anim_num >= sc3_main->anim_num ? 0 : *anim_num;
    const Speed_Charm_3D_Anim *sc3_anim__ = &sc3_main->sc_anim[*anim_num];
    sc3_main->anim_times += delta_time;

    if (sc3_main->anim_times >= sc3_anim__->time_end) sc3_main->anim_times -= sc3_anim__->time_end - sc3_anim__->time_beg;
   	float frame_time__ = (sc3_main->anim_times - sc3_anim__->time_beg) * sc3_anim__->framerate;
	sc_size f0__ = get_minu((sc_size)frame_time__ + 0, sc3_anim__->frame_num - 1);
	sc_size f1__ = get_minu((sc_size)frame_time__ + 1, sc3_anim__->frame_num - 1);
	float t__ = get_minf(frame_time__ - (float)f0__, 1.0f);

    for (sc_size i = 0; i < sc3_main->skeletons_num; ++i) {

        const Speed_Charm_3D_Anim_Deform *anim_deform = sc3_anim__->anim_deforms + i;
        sc_byte dfs__ = get_animation_deform_status(i, sc3_anim__);
        SGG_Quaternion rot__;

        if(SC3_CHECK_DYNAMIC_ROT(dfs__)) sgg_lerp_quaternion(&rot__, anim_deform->rot + f0__, anim_deform->rot + f1__, t__);
        else rot__ = *anim_deform->rot;
        rot__ = get_anim_dynamic_rot(rot__, &sc3_main->sc_skeletons[i], sc3_main->sc_dynamic_def);
        SGG_Vector3 pos__;
        
        if(SC3_CHECK_DYNAMIC_POS(dfs__)) sgg_lerp_vector3(&pos__, anim_deform->pos + f0__, anim_deform->pos + f1__, t__);
        else pos__ = *anim_deform->pos;
        
        SGG_Vector3 scale__;

        if(SC3_CHECK_DYNAMIC_SCL(dfs__)) sgg_lerp_vector3(&scale__, anim_deform->scale + f0__, anim_deform->scale + f1__, t__);
        else scale__ = *anim_deform->scale;
        sgg_trs_mat4(&(*(sc3_main->sc_skeletons + i)).node.to_parent, &rot__, &pos__, &scale__);
    }
}
SGG_Quaternion get_anim_dynamic_rot(SGG_Quaternion anim_rot, Speed_Charm_3D_Skeleton * sc3_skeleton, Speed_Charm_3D_Dynamic_Deform * sc3_dynamic_deform){
    //printf("%d\n", sc3_skeleton->dynamic_point);
    if(sc3_skeleton->dynamic_point != NOT_DYNAMIC_POINT)
        return mul_quaternion_quaternion(&anim_rot, &sc3_dynamic_deform[sc3_skeleton->dynamic_point].dynamic_rot);
    return anim_rot;
}

void alloc_dynamic_rotation(Speed_Charm_3D_Run_Main * sc3_main, sc_size size_){
    if(!sc3_main) return;
    if(!sc3_main->sc_dynamic_def) sc3_main->sc_dynamic_def = smart_calloc(Speed_Charm_3D_Dynamic_Deform, size_);
    else{
        Speed_Charm_3D_Dynamic_Deform * tmp__ = smart_realloc(Speed_Charm_3D_Dynamic_Deform, sc3_main->sc_dynamic_def, size_);
        if(!tmp__) return;
        sc3_main->sc_dynamic_def = tmp__;
    }
    sc3_main->dynamic_deform_num = size_;
}

SGG_Quaternion * get_dynamic_point_rotation(sc_size skeleton_index, Speed_Charm_3D_Run_Main * sc3_main){
    if(sc3_main->skeletons_num <= skeleton_index) return NULL;
    sc_int dynamic_point = sc3_main->sc_skeletons[skeleton_index].dynamic_point;
    if(sc3_main->dynamic_deform_num <= dynamic_point) return NULL;
    return &sc3_main->sc_dynamic_def[dynamic_point].dynamic_rot;
}

void update_dynamic_rotation_def(float deg_x, float deg_y, float deg_z, sc_int dynamic_point_idx, Speed_Charm_3D_Run_Main * sc3_main){
    if(sc3_main->dynamic_deform_num <= dynamic_point_idx) return;
    if(dynamic_point_idx < 0) return;
    SGG_Quaternion q = quat_rotation_xyz(deg_x, deg_y, deg_z);
    sc3_main->sc_dynamic_def[dynamic_point_idx].dynamic_rot = q;
}
void reset_skeleton_dynamic_point(Speed_Charm_3D_Run_Main * sc3_main, sc_size skeleton_index){
    sc3_main->sc_skeletons[skeleton_index].dynamic_point = NOT_DYNAMIC_POINT;
}
void update_dynamic_point(sc_size skeleton_index, sc_int dynamic_point_idx, Speed_Charm_3D_Run_Main * sc3_main){
    if(sc3_main->skeletons_num <= skeleton_index) return;
    if(sc3_main->dynamic_deform_num <= dynamic_point_idx) return;
    if(dynamic_point_idx < 0) return;
    sc3_main->sc_skeletons[skeleton_index].dynamic_point = dynamic_point_idx;
}
void set_dynamic_point_rotation(float deg_x, float deg_y, float deg_z, sc_size skeleton_index, sc_int dynamic_point_idx, Speed_Charm_3D_Run_Main * sc3_main){
    if(dynamic_point_idx < 0) return;
    if(sc3_main->dynamic_deform_num <= dynamic_point_idx) return;
    if(sc3_main->skeletons_num <= skeleton_index) return;
    SGG_Quaternion q = quat_rotation_xyz(deg_x, deg_y, deg_z);
    sc3_main->sc_dynamic_def[dynamic_point_idx].dynamic_rot = q;
    sc3_main->sc_skeletons[skeleton_index].dynamic_point = dynamic_point_idx;
    //printf("DEBUG STREAM %d\n", sc3_main->sc_skeletons[skeleton_index].parent);
    /*
    printf("%f %f %f %f\n", q.x, q.y, q.z, q.w);
    sc_int * stack_point_id = smart_malloc(sc_int, sc3_main->skeletons_num);
    if(!stack_point_id)return;
    sc_size stack_index = 0;
    sc_size stack_max_size = 0;
    stack_point_id[stack_index++] = dynamic_point;
    stack_max_size++;
    while(stack_index != 0){
        sc_int t = stack_point_id[--stack_index];
        if(t == -1)continue;
        for(sc_size i = 0; i < sc3_main->skeletons_num; ++i){
            if(t == sc3_main->sc_skeletons[i].parent){
                stack_point_id[stack_index++] = i;
                sc3_main->sc_skeletons[i].dynamic_point = 1;
                sc3_main->sc_skeletons[i].dynamic_rot = q;
            }
        }
    }
    s_free(stack_point_id);
    */
}

void update_speed_charm_3d_bones_hierarchy(Speed_Charm_3D_Run_Main * sc3_main){
    for (size_t i = 0; i < sc3_main->skeletons_num; i++) {
	    Speed_Charm_3D_Skeleton *sc3_skl_map__ = &sc3_main->sc_skeletons[i];

	    if (sc3_skl_map__->parent >= 0) mul_mat4_mat4(&sc3_skl_map__->node.to_world, &(*(sc3_main->sc_skeletons + sc3_skl_map__->parent)).node.to_world, &sc3_skl_map__->node.to_parent);
        else sc3_skl_map__->node.to_world = sc3_skl_map__->node.to_parent;
    }
}

void build_bones_speed_charm_3d(Speed_Charm_3D_Run_Main * sc3_main, Speed_Charm_3D_Bone_Map * sc3_mesh_bone_map, SGG_Mat4 * sc3_mat4){
    Speed_Charm_3D_Skeleton * bone_node__ = sc3_main->sc_skeletons + sc3_mesh_bone_map->ind;
    mul_mat4_mat4(sc3_mat4, &bone_node__->node.to_world, &sc3_mesh_bone_map->inverse_mat);
}

float get_instance_groups_distance(Speed_Charm_3D_Instance* sc3_ins, SGG_Vector3 pos, sc_uint group_idx) {
    SGG_Vector3 tmp_ = pos;
    sub_vector3_vector3(&tmp_, &sc3_ins->group[group_idx].center);
    return length_sgg_vector3(&tmp_);
}
sc_uint get_instance_group_meshes_idx(Speed_Charm_3D_Run_Main* sc3_main, SGG_Vector3 pos, sc_uint group_idx, sc_uint instance_idx, float dist_min, float scaler) {
    float dis = get_instance_groups_distance(sc3_main->sc_ins + instance_idx, pos, group_idx);
    float dist_mid = dist_min * scaler;
    float dist_max = dist_mid * scaler;
    if (dis <= dist_min) return (*(sc3_main->sc_ins + instance_idx)).level[SC3_LOD_MIN];
    else if (dis <= dist_mid) return (*(sc3_main->sc_ins + instance_idx)).level[SC3_LOD_MID];
    else return (*(sc3_main->sc_ins + instance_idx)).level[SC3_LOD_MAX];
}

struct Translate_Data {
    SGG_Vector4 local;
    SGG_Mat4 trs;
};

struct Instance_Cluster {
    struct Translate_Data* trs;
    SGG_Vector4 centroid;
    uint32_t cap;
    uint32_t idx;
};

bool create_k_means_translation_cluster(struct Instance_Cluster cluster[10], const SGG_Mat4* trs, const sc_uint num, const sc_uint group_num) {
    if (group_num < 3 || group_num > 10) return false;

    SGG_Vector4 ctr = { 0.0f, 0.0f, 0.0f, 1.0f };
    Xo_Shiro_256 sample_cluster;
    xos_256_init(&sample_cluster, (uint64_t)time(NULL));
    uint32_t* cluster__ = smart_malloc(uint32_t, num);
    if (!cluster__)return false;
    for (uint32_t i = 0; i < num; ++i) {
        uint64_t n = xos_256_next(&sample_cluster);
        n = fix_rand_range_uin64_t(n, 0, group_num - 1);
        cluster[n].cap++;
        cluster__[i] = n;
    }

    for (uint32_t i = 0; i < group_num; ++i) {

        cluster[i].trs = smart_malloc(struct Translate_Data, cluster[i].cap);
        if (!cluster[i].trs) {
            if (cluster[i].cap) {
                for (uint32_t j = 0; j < i; ++j) {
                    s_free(cluster[j].trs);
                }
                s_free(cluster__);

                return false;
            }
            else continue;
        }
    }
    for (uint32_t i = 0; i < num; ++i) {
        uint32_t n = cluster__[i];

        cluster[n].trs[cluster[n].idx].trs = *(trs + i);
        mul_mat4_vector4(&cluster[n].trs[cluster[n].idx].local, trs + i, &ctr);
        cluster[n].idx++;
    }
    s_free(cluster__);
    bool move_cluster;
    do {
        move_cluster = false;
        for (uint32_t i = 0; i < group_num; ++i) {
            assignment_vector4(&cluster[i].centroid, 0.0f);
            for (uint32_t j = 0; j < cluster[i].cap; ++j) {
                add_vector4_vector4(&cluster[i].centroid, &cluster[i].trs[j].local);
            }
            if (cluster[i].cap == 0) continue;
            div_vector4(&cluster[i].centroid, cluster[i].cap);
        }

        for (uint32_t i = 0; i < group_num; ++i) {
            for (uint32_t j = 0; j < cluster[i].cap;) {

                float dist = 0.0f;
                uint32_t new_cluster = i;

                SGG_Vector4 tmp = cluster[i].centroid;
                sub_vector4_vector4(&tmp, &cluster[i].trs[j].local);
                dist = length_sgg_vector4(&tmp);
                for (uint32_t k = 0; k < group_num; ++k) {
                    if (i == k) continue;
                    if (cluster[k].cap == 0) continue;

                    tmp = cluster[k].centroid;
                    sub_vector4_vector4(&tmp, &cluster[i].trs[j].local);
                    float tmp_dist = length_sgg_vector4(&tmp);
                    if (tmp_dist < dist) {
                        new_cluster = k;
                        dist = tmp_dist;
                    }
                }
                if (new_cluster != i) {
                    move_cluster = true;
                    struct Translate_Data* new_trs = smart_realloc(struct Translate_Data, cluster[new_cluster].trs, cluster[new_cluster].cap + 1);
                    if (!new_trs) {

                        for (uint32_t i = 0; i < group_num; ++i) {
                            s_free(cluster[i].trs);
                        }
                        return false;
                    }
                    new_trs[cluster[new_cluster].cap] = cluster[i].trs[j];
                    cluster[new_cluster].trs = new_trs;
                    cluster[new_cluster].cap++;
                    cluster[i].trs[j] = cluster[i].trs[cluster[i].cap - 1];
                    new_trs = smart_realloc(struct Translate_Data, cluster[i].trs, cluster[i].cap - 1);
                    if (!new_trs && cluster[i].cap - 1) {

                        for (uint32_t k = 0; k < group_num; ++k) {
                            s_free(cluster[k].trs);
                        }
                        return false;
                    }
                    cluster[i].trs = new_trs;
                    cluster[i].cap--;
                    continue;
                }
                ++j;
            }
        }
    } while (move_cluster);
    return true;
}

void create_instance_groups_kmodel(Speed_Charm_3D_Main* sc3_main, const sc_uint * meshes_idx, sc_uint meshes_idx_num, const SGG_Mat4* trs, const sc_uint num, const sc_uint group_num) {
    if (!trs) return;
    if (!meshes_idx) return;

    struct Instance_Cluster cluster[10] = { 0 };
    if (!create_k_means_translation_cluster(cluster, trs, num, group_num)) return;
    Speed_Charm_3D_Instance * new_ins = smart_realloc(Speed_Charm_3D_Instance, sc3_main->sc_ins, sc3_main->instance_num + 1);
    if (!new_ins) {
        for (uint32_t i = 0; i < group_num; ++i) s_free(cluster[i].trs);
        return;
    }
    sc3_main->sc_ins = new_ins;
    if (meshes_idx_num > SC3_LOD_MAX_NUM) meshes_idx_num = SC3_LOD_MAX_NUM;
    smart_memcpy_p(sc3_main->sc_ins[sc3_main->instance_num].level, meshes_idx, sc_uint, meshes_idx_num);
    sc3_main->sc_ins[sc3_main->instance_num].level_num = meshes_idx_num;
    sc3_main->sc_ins[sc3_main->instance_num].group = smart_malloc(Speed_Charm_3D_Instance_Group, group_num);
    if (!sc3_main->sc_ins[sc3_main->instance_num].group) {
        for (uint32_t i = 0; i < group_num; ++i) s_free(cluster[i].trs);
        return;
    }
    sc3_main->sc_ins[sc3_main->instance_num].group_num = group_num;
    for (sc_uint i = 0; i < group_num; ++i) {
        sc3_main->sc_ins[sc3_main->instance_num].group[i].center.x = cluster[i].centroid.x;
        sc3_main->sc_ins[sc3_main->instance_num].group[i].center.y = cluster[i].centroid.y;
        sc3_main->sc_ins[sc3_main->instance_num].group[i].center.z = cluster[i].centroid.z;
        sc3_main->sc_ins[sc3_main->instance_num].group[i].num = cluster[i].cap;
        sc3_main->sc_ins[sc3_main->instance_num].group[i].trs = smart_malloc(SGG_Mat4, sc3_main->sc_ins[sc3_main->instance_num].group[i].num);
        if (!sc3_main->sc_ins[sc3_main->instance_num].group[i].trs) {
            sc3_main->sc_ins[sc3_main->instance_num].group[i].num = i;
            break;
        }
        for (sc_uint j = 0; j < cluster[i].cap; ++j) {
            sc3_main->sc_ins[sc3_main->instance_num].group[i].trs[j] = cluster[i].trs[j].trs;
        }
    }
    sc3_main->instance_num++;
    for (uint32_t i = 0; i < group_num; ++i) s_free(cluster[i].trs);
}
void test_create_k_means_translate_cluster(void) {

    SGG_Mat4 trs[12];

    for (uint32_t i = 0; i < 12; ++i) {
        sgg_identity(&trs[i]);
    }

    trs[0].m[12] = 0.0f;
    trs[1].m[12] = 1.0f;
    trs[2].m[12] = 2.0f;
    trs[3].m[12] = 3.0f;

    trs[4].m[12] = 100.0f;
    trs[5].m[12] = 101.0f;
    trs[6].m[12] = 102.0f;
    trs[7].m[12] = 103.0f;

    trs[8].m[12] = 200.0f;
    trs[9].m[12] = 201.0f;
    trs[10].m[12] = 202.0f;
    trs[11].m[12] = 203.0f;

    struct Instance_Cluster cluster[10] = { 0 };

    if (!create_k_means_translation_cluster(
        cluster,
        trs,
        12,
        3
    )) {
        printf("create_k_means_translate_cluster failed\n");
        return;
    }

    for (uint32_t i = 0; i < 3; ++i) {

        printf(
            "cluster[%u] cap=%u centroid=(%f, %f, %f)\n",
            i,
            cluster[i].cap,
            cluster[i].centroid.x,
            cluster[i].centroid.y,
            cluster[i].centroid.z
        );

        for (uint32_t j = 0; j < cluster[i].cap; ++j) {
            printf(
                "  local=(%f, %f, %f)\n",
                cluster[i].trs[j].local.x,
                cluster[i].trs[j].local.y,
                cluster[i].trs[j].local.z
            );
        }
    }

    for (uint32_t i = 0; i < 3; ++i) {
        s_free(cluster[i].trs);
    }
}