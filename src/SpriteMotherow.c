#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 motherow_anim_down[] = {2, 0, 1}; //The first number indicates the number of frames
const UINT8 motherow_anim_up[] = {2, 2, 3}; //The first number indicates the number of frames
const UINT8 motherow_anim_h[] = {2, 4, 5}; //The first number indicates the number of frames
const UINT8 motherow_anim_idle_h[] = {1, 5}; //The first number indicates the number of frames
const UINT8 motherow_anim_idle_up[] = {1, 2}; //The first number indicates the number of frames
const UINT8 motherow_anim_idle_down[] = {1, 1}; //The first number indicates the number of frames

extern UINT8 hudow_opened;

struct OwSpriteInfo* motherow_info = 0;

void changeState(FA2OW_SPRITE_STATES new_state);

void START(){
    motherow_info = (struct OwSpriteInfo*) THIS->custom_data;
    motherow_info->ow_state = IDLE_DOWN;
}

void UPDATE(){
    if(hudow_opened == 1u){return;}
    FA2OW_SPRITE_STATES new_state = motherow_info->ow_state;
    
    //WALKING
    if(KEY_PRESSED(J_DOWN)){motherow_info->tile_collision = TranslateSprite(THIS, 0, 1 << delta_time);new_state = WALK_DOWN;}
    else if(KEY_PRESSED(J_UP)){motherow_info->tile_collision = TranslateSprite(THIS, 0, -1 << delta_time);new_state = WALK_UP;}
    else if(KEY_PRESSED(J_LEFT)){motherow_info->tile_collision = TranslateSprite(THIS, -1 << delta_time, 0);new_state = WALK_LEFT;}
    else if(KEY_PRESSED(J_RIGHT)){motherow_info->tile_collision = TranslateSprite(THIS, 1 << delta_time, 0);new_state = WALK_RIGHT;}
    if(KEY_RELEASED(J_DOWN)){new_state = IDLE_DOWN;}
    if(KEY_RELEASED(J_UP)){new_state = IDLE_UP;}
    if(KEY_RELEASED(J_LEFT)){new_state = IDLE_LEFT;}
    if(KEY_RELEASED(J_RIGHT)){new_state = IDLE_RIGHT;}
    
    if(motherow_info->ow_state != new_state){ 
        changeState(new_state);
    }

}

void changeState(FA2OW_SPRITE_STATES new_state){
    if(new_state == GENERIC_IDLE){
        if(motherow_info->ow_state == WALK_DOWN){
            new_state = IDLE_DOWN;
        }
        else if(motherow_info->ow_state == WALK_UP){
            new_state = IDLE_UP;
        }
        if(motherow_info->ow_state == WALK_LEFT){
            new_state = IDLE_LEFT;
        }
        if(motherow_info->ow_state == WALK_RIGHT){
            new_state = IDLE_RIGHT;
        }
    }
    switch(new_state){
        case IDLE_DOWN:
            SetSpriteAnim(THIS, motherow_anim_idle_down, 4u);
        break;
        case IDLE_UP:
            SetSpriteAnim(THIS, motherow_anim_idle_up, 4u);
        break;
        case IDLE_LEFT:
            SetSpriteAnim(THIS, motherow_anim_idle_h, 4u);
            THIS->mirror = V_MIRROR;
        break;
        case IDLE_RIGHT:
            SetSpriteAnim(THIS, motherow_anim_idle_h, 4u);
            THIS->mirror = NO_MIRROR;
        break;
        case WALK_DOWN:
            SetSpriteAnim(THIS, motherow_anim_down, 12u);
        break;
        case WALK_UP:
            SetSpriteAnim(THIS, motherow_anim_up, 12u);
        break;
        case WALK_LEFT:
            SetSpriteAnim(THIS, motherow_anim_h, 12u);
            THIS->mirror = V_MIRROR;
        break;
        case WALK_RIGHT:
            SetSpriteAnim(THIS, motherow_anim_h, 12u);
            THIS->mirror = NO_MIRROR;
        break;
    }
    motherow_info->ow_state = new_state;
}

void DESTROY(){

}