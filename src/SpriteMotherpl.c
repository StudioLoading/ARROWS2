#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"
#include "Dialogs.h"

#define GRAVITY 2
#define JUMP_MIN_POWER 4
#define JUMP_MAX_POWER GRAVITY*10
#define JUMP_TICKED_COOLDOWN 16
#define INERTIA_MAX 6

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 motherpl_anim_idle[] = {2, 1, 2}; //The first number indicates the number of frames
struct MotherplData* motherpl_data = 0;
INT8 motherpl_vx = 0;
INT8 motherpl_vy = 0;
UINT8 motherpl_coll = 0u;
UINT8 motherpl_jpower = 0u;
UINT8 motherpl_inertiax = 0u;
UINT8 motherpl_inertia_down = 0u;
UINT8 gravity_frame_skip = 0u;
UINT8 jump_frame_skip = 0u;
UINT8 jump_ticked_delay = 0u;
UINT8 jump_max_toched = 0u;

void START(){
    SetSpriteAnim(THIS, motherpl_anim_idle, 8u);
    motherpl_data = (struct MotherplData*) THIS->custom_data;
    motherpl_vy = 1;
    motherpl_data->mpl_state = MOTHERPL_IDLE;
}

void UPDATE(){
    switch(motherpl_data->mpl_state){
        case MOTHERPL_IDLE:
            motherpl_jpower = 0;
            jump_max_toched = 0u;
            motherpl_vy = GRAVITY;
        break;
        case MOTHERPL_JUMP:
            if(KEY_RELEASED(J_JUMP)){
                motherpl_jpower = 0;
                jump_max_toched = 0u;
                motherpl_vy = GRAVITY;
                jump_ticked_delay = JUMP_TICKED_COOLDOWN;
            }else if(KEY_PRESSED(J_JUMP) && jump_ticked_delay == 0){
                if(motherpl_jpower < JUMP_MAX_POWER){
                    motherpl_jpower++;
                    if(motherpl_jpower < (JUMP_MAX_POWER / 3)){
                        motherpl_vy = -3;
                    }else if(motherpl_jpower < (JUMP_MAX_POWER / 2)){
                        motherpl_vy = -2;
                    }else{
                        motherpl_vy = -1;
                    }
                }       
                if(motherpl_jpower == JUMP_MAX_POWER){
                    if(motherpl_vy < GRAVITY){
                        motherpl_vy++;
                    }
                }
            }else{
                if (motherpl_jpower > 0){
                    motherpl_jpower--;
                }
                if(motherpl_vy < GRAVITY){
                    motherpl_vy++;
                }
            }
        break;
    }
    //INPUTS
    if(jump_ticked_delay == 0 && motherpl_vy == GRAVITY && motherpl_jpower == 0){
        if(KEY_TICKED(J_JUMP)){
            if(motherpl_data->mpl_state != MOTHERPL_JUMP){
                motherpl_data->mpl_state = MOTHERPL_JUMP;
                motherpl_vy = -1;
                jump_ticked_delay = JUMP_TICKED_COOLDOWN;
            }
        }
    }
    if(jump_ticked_delay > 0){
            jump_ticked_delay--;
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
    //GRAVITY FRAME SKIP
    if(gravity_frame_skip == 0u){
        gravity_frame_skip = 1u;        
    }else{
        gravity_frame_skip = 0u;
    }
    //INERTIA ON X AXIS
    if(motherpl_inertiax > 0 && !KEY_PRESSED(J_LEFT) && !KEY_PRESSED(J_RIGHT)){
        motherpl_inertiax--;
    }
    //ACTUAL MOVEMENT
    if(motherpl_inertiax > 2){
        motherpl_coll = TranslateSprite(THIS, motherpl_vx << delta_time, motherpl_vy << delta_time);
    }else{
        motherpl_coll = TranslateSprite(THIS, 0, motherpl_vy << delta_time);
    }
    if(motherpl_data->mpl_state == MOTHERPL_JUMP && motherpl_coll && motherpl_vy > 0){//IF ON SURFACE, NO MORE JUMP
        motherpl_jpower = 0;
        jump_max_toched = 0u;
        motherpl_vy = GRAVITY;
        motherpl_data->mpl_state == MOTHERPL_IDLE;
    }
}

void DESTROY(){}