#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern Sprite* s_motherpl;
extern struct MISSION missions[4];

struct EnemyData* child_info;
const UINT8 child_anim_idle[] = {8,1,2,1,2,0,2,0,2};
const UINT8 child_anim_walk[] = {4,3,4,3,2};
UINT8 child_hooked = 0u;
void child_behavior() BANKED;
void change_child_state(ENEMY_STATE new_state) BANKED;

void START(){
    THIS->lim_x = 200u;
    child_info = (struct EnemyData*)THIS->custom_data;
    SetSpriteAnim(THIS, child_anim_idle, 4u);
    child_info->e_state = ENEMY_IDLE;
    child_info->vx = 0;
    child_info->x_frameskip = 6u;
    child_hooked = 0u;
    missions[2].mission_state = MISSION_STATE_STARTED;
}

void UPDATE(){
    if(missions[2].current_step == 3u){
        child_info->x_frameskip--;
        if(child_info->x_frameskip == 0u){
            child_behavior();
            TranslateSprite(THIS, child_info->vx << delta_time, 0);
            child_info->x_frameskip = 6u;
        }
    }
}

void child_behavior() BANKED{
    if(child_info->vx > 0){THIS->mirror = NO_MIRROR;}
    if(child_info->vx < 0){THIS->mirror = V_MIRROR;}
    UINT16 distx = 0u;
    if(THIS->x < s_motherpl->x){
        distx = s_motherpl->x - THIS->x;
    }else if (THIS->x > s_motherpl->x){
        distx = THIS->x - s_motherpl->x;
    }
    if (distx > 120u){
        child_hooked = 0u;
        SpriteManagerRemoveSprite(THIS);
        missions[2].current_step = 0u;
    }else if(distx > 40u){
        child_hooked = 0u;
        change_child_state(ENEMY_IDLE);
    }else{
        child_hooked = 1u;
        switch(child_info->e_state){
            case ENEMY_WALK:
                switch(s_motherpl->mirror){
                    case NO_MIRROR:
                        if(THIS->x < (s_motherpl->x - 16u)){
                            child_info->vx = 1;
                        }else if(THIS->x > (s_motherpl->x - 16u)){
                            child_info->vx = -1;
                        }else{
                            change_child_state(ENEMY_IDLE);
                        }
                    break;
                    case V_MIRROR:
                        if(THIS->x < (s_motherpl->x + 12u)){
                            child_info->vx = 1;
                        }else if(THIS->x > (s_motherpl->x + 12u)){
                            child_info->vx = -1;
                        }else{
                            change_child_state(ENEMY_IDLE);
                        }

                    break;
                }
            break;
            case ENEMY_IDLE:
                {
                    UINT8 final_x = 16;
                    if(s_motherpl->mirror != NO_MIRROR){
                        final_x = 12u;
                    }
                    if(s_motherpl->mirror != THIS->mirror 
                        || s_motherpl->x > (THIS->x + final_x)
                        || s_motherpl->x < (THIS->x - final_x)
                    ){
                        change_child_state(ENEMY_WALK);
                    }
                }
            break;
        }
    }
}

void change_child_state(ENEMY_STATE new_state) BANKED{
    if(child_info->e_state != new_state){
        switch(new_state){
            case ENEMY_IDLE:
                child_info->vx = 0u;
                SetSpriteAnim(THIS, child_anim_idle, 4u);
                THIS->mirror = s_motherpl->mirror;
            break;
            case ENEMY_WALK:
                SetSpriteAnim(THIS, child_anim_walk, 12u);
            break;
        }
        child_info->e_state = new_state;
    }
}

void DESTROY(){
}