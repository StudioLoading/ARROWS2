#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"
#include "Dialogs.h"

#define GRAVITY 2
#define JUMP_MIN_POWER 0
#define JUMP_MAX_POWER GRAVITY*10
#define JUMP_TICKED_COOLDOWN 16
#define INERTIA_MAX 6

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 motherpl_anim_idle[] = {4, 1, 1, 1, 2}; //The first number indicates the number of frames
const UINT8 motherpl_anim_walk[] = {4, 3, 4, 3, 5};

struct MotherplData* motherpl_data = 0;
INT8 motherpl_vx = 0;
INT8 motherpl_vy = 0;
MOTHERPL_STATE motherpl_state = MOTHERPL_IDLE;
UINT8 motherpl_coll = 0u;
UINT8 motherpl_jpower = 0u;
UINT8 motherpl_inertiax = 0u;
UINT8 motherpl_inertia_down = 0u;
UINT8 gravity_frame_skip = 0u;
UINT8 jump_frame_skip = 0u;
UINT8 jump_ticked_delay = 0u;
UINT8 jump_max_toched = 0u;

void changeMotherplState(MOTHERPL_STATE new_state);

void START(){
    SetSpriteAnim(THIS, motherpl_anim_idle, 8u);
    motherpl_data = (struct MotherplData*) THIS->custom_data;
    //motherpl_state = MOTHERPL_IDLE;
    //changeMotherplState(MOTHERPL_JUMP);
    changeMotherplState(MOTHERPL_IDLE);
}

void UPDATE(){
    switch(motherpl_state){
        case MOTHERPL_IDLE:
            motherpl_jpower = 0;
            jump_max_toched = 0u;
            motherpl_vy = GRAVITY;
        break;
        case MOTHERPL_JUMP:        
            if(motherpl_coll && motherpl_vy > 0 && motherpl_inertiax == 0
                && jump_ticked_delay == 0u ){//IF ON SURFACE, NO MORE JUMP
                changeMotherplState(MOTHERPL_IDLE);
            }
            if(KEY_RELEASED(J_JUMP)){
                motherpl_jpower = JUMP_MIN_POWER;
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
                if (motherpl_jpower > JUMP_MIN_POWER){
                    motherpl_jpower--;
                }
                if(motherpl_vy < GRAVITY){
                    motherpl_vy++;
                }
            }
        break;
        case MOTHERPL_WALK:
            if(motherpl_inertiax == 0 || (KEY_RELEASED(J_RIGHT) || KEY_RELEASED(J_LEFT))){
                changeMotherplState(MOTHERPL_IDLE);
            }
        break;
    }
    //INPUTS
    if(jump_ticked_delay == 0 && motherpl_vy == GRAVITY && motherpl_jpower == JUMP_MIN_POWER){
        if(KEY_TICKED(J_JUMP) || KEY_PRESSED(J_JUMP)){
            changeMotherplState(MOTHERPL_JUMP);
        }else if((KEY_TICKED(J_RIGHT) || KEY_TICKED(J_LEFT))){ //motherpl_state != MOTHERPL_JUMP && 
            changeMotherplState(MOTHERPL_WALK);
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
}

void changeMotherplState(MOTHERPL_STATE new_state){
    if(motherpl_state != new_state){
        switch(new_state){
            case MOTHERPL_IDLE:
                SetSpriteAnim(THIS, motherpl_anim_idle, 8u);
                motherpl_jpower = 0;
                jump_max_toched = 0u;
                motherpl_vy = GRAVITY;
            break;
            case MOTHERPL_JUMP:
                motherpl_vy = -1;
                //jump_ticked_delay = JUMP_TICKED_COOLDOWN;
            break;
            case MOTHERPL_WALK:
                SetSpriteAnim(THIS, motherpl_anim_walk, 8u);
                motherpl_jpower = 0;
                jump_max_toched = 0u;
            break;
        }
        motherpl_state = new_state;
    }
}

void DESTROY(){}