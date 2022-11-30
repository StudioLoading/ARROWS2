#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"
#include "Dialogs.h"

#define GRAVITY 8
#define JUMP_MIN_POWER 0
#define JUMP_MAX_POWER 5
#define INERTIA_MAX 6

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 motherpl_anim_idle[] = {2, 1, 2}; //The first number indicates the number of frames
struct MotherplData* motherpl_data = 0;
INT8 motherpl_vx = 0;
UINT8 motherpl_coll = 0u;
UINT8 motherpl_jpower = 0u;
UINT8 motherpl_inertiax = 0u;
UINT8 motherpl_inertia_down = 0u;
UINT8 gravity_frame_skip = 0u;

void START(){
    SetSpriteAnim(THIS, motherpl_anim_idle, 8u);
    motherpl_data = (struct MotherplData*) THIS->custom_data;
    motherpl_data->accel_y = 1;
    motherpl_data->mpl_state = MOTHERPL_IDLE;
}

void UPDATE(){
    //INPUTS
    if(KEY_TICKED(J_JUMP) && motherpl_data->mpl_state != MOTHERPL_JUMP){
        motherpl_jpower = JUMP_MIN_POWER;
        motherpl_data->accel_y = -motherpl_jpower;
        motherpl_data->mpl_state = MOTHERPL_JUMP;
    }
    if(KEY_PRESSED(J_JUMP) && motherpl_jpower < JUMP_MAX_POWER && 
        motherpl_data->mpl_state == MOTHERPL_JUMP && gravity_frame_skip == 0){
        motherpl_jpower++;
        motherpl_data->accel_y = -motherpl_jpower;
    }
    if(KEY_PRESSED(J_RIGHT)){
        motherpl_inertia_down = 0u;
        if(motherpl_inertiax < INERTIA_MAX){
            motherpl_inertiax++;
        }
        motherpl_vx = 1;
        THIS->mirror = NO_MIRROR;
    }
    if(KEY_RELEASED(J_RIGHT) || KEY_RELEASED(J_LEFT)){
        motherpl_inertia_down = 1u;
    }
    if(KEY_PRESSED(J_LEFT)){
        motherpl_inertia_down = 0u;
        if(motherpl_inertiax < INERTIA_MAX){
            motherpl_inertiax++;
        }
        motherpl_vx = -1;
        THIS->mirror = V_MIRROR;
    }
    if(motherpl_inertia_down == 1u){
        if(motherpl_inertiax > 0){
            motherpl_inertiax--;
        }
    }
    //GRAVITY ALWAYS
    
    if(gravity_frame_skip == 0u){
        gravity_frame_skip = 1u;
        if(motherpl_data->accel_y < GRAVITY){
        //if(motherpl_data->mpl_state == MOTHERPL_JUMP){
            motherpl_data->accel_y++;
        }
    }else{
        gravity_frame_skip--;
    }
        //}else{
            //motherpl_data->accel_y++;
        //}
    //ACTUAL MOVEMENT
    if(motherpl_inertiax > 2){
        motherpl_coll = TranslateSprite(THIS, motherpl_vx << delta_time, motherpl_data->accel_y << delta_time);
    }else if(motherpl_inertiax > 0 && !KEY_PRESSED(J_LEFT) && !KEY_PRESSED(J_RIGHT)){
        motherpl_inertiax--;
        motherpl_coll = TranslateSprite(THIS, motherpl_vx << delta_time, motherpl_data->accel_y << delta_time);
    }else{
        motherpl_coll = TranslateSprite(THIS, 0, motherpl_data->accel_y << delta_time);
    }
    if(motherpl_data->mpl_state != MOTHERPL_IDLE && motherpl_coll){//IF ON SURFACE, NO MORE JUMP
        motherpl_data->mpl_state = MOTHERPL_IDLE;
        motherpl_jpower = 0;
        //gravity_frame_skip = 0;
    }
}

void DESTROY(){}