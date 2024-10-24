#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Sound.h"

#include "custom_datas.h"
#include "Dialogs.h"

const UINT8 motherow_anim_down[] = {4, 9, 0, 9, 1}; //The first number indicates the number of frames
const UINT8 motherow_anim_up[] = {4, 4,3,4,2}; //The first number indicates the number of frames
const UINT8 motherow_anim_h[] = {4, 5, 7, 6, 7}; //The first number indicates the number of frames
const UINT8 motherow_anim_idle_h[] = {1, 7}; //The first number indicates the number of frames
const UINT8 motherow_anim_idle_up[] = {1, 4}; //The first number indicates the number of frames
const UINT8 motherow_anim_idle_down[] = {6, 8,8,8,8,9,10}; //The first number indicates the number of frames

extern INT8 sfx_cooldown;
extern UINT8 just_started;
extern UINT8 hudow_opened;
extern struct OwSpriteInfo* motherow_info;
extern FA2OW_SPRITE_STATES motherow_new_state;
extern UINT8 show_tip;

extern void owChangeState(Sprite* s_motherow_arg, FA2OW_SPRITE_STATES new_state) BANKED;
extern void motherow_interact_with_map(Sprite* s_motherow_arg) BANKED;
extern void motherow_interact_with_sprites(Sprite* s_motherow_arg) BANKED;

void motherownormal_setanim_walkh() BANKED;
void motherownormal_setanim_walkup() BANKED;
void motherownormal_setanim_walkdown() BANKED;
void motherownormal_setanim_idleh() BANKED;
void motherownormal_setanim_idledown() BANKED;
void motherownormal_setanim_idleup() BANKED;

extern void motherow_start(Sprite* s_motherow_arg) BANKED;
extern void update_position_motherow(Sprite* s_motherow_arg) BANKED;
extern void ow_check_place(Sprite* s_motherow_arg) BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;

void START(){
    motherow_start(THIS);
}

void UPDATE(){
    if(just_started == 1u){
        motherow_new_state = WALK_UP;
        update_position_motherow(THIS);
        owChangeState(THIS, motherow_new_state);
        if(motherow_info->tile_collision == 62u || motherow_info->tile_collision == 64u){
            ChangeState(StateTutorial, THIS, MAP_SOUTHWEST);
            just_started = 2;
        }
        return; 
    }
    //SFX & HUD
        if(sfx_cooldown > 0){sfx_cooldown--;}
        if(hudow_opened == 1u){return;}
    //CONTROLS
        if(KEY_PRESSED(J_DOWN)){motherow_new_state = WALK_DOWN;}
        else if(KEY_PRESSED(J_UP)){motherow_new_state = WALK_UP;}
        else if(KEY_PRESSED(J_LEFT)){motherow_new_state = WALK_LEFT;}
        else if(KEY_PRESSED(J_RIGHT)){motherow_new_state = WALK_RIGHT;}
        else if(KEY_RELEASED(J_DOWN)){motherow_new_state = IDLE_DOWN;}
        else if(KEY_RELEASED(J_UP)){motherow_new_state = IDLE_UP;}
        else if(KEY_RELEASED(J_LEFT)){motherow_new_state = IDLE_LEFT;}
        else if(KEY_RELEASED(J_RIGHT)){motherow_new_state = IDLE_RIGHT;}  
    //INTERACT WITH SPRITES
        motherow_interact_with_sprites(THIS);
    //CHANGE STATE
        if(motherow_info->ow_state != motherow_new_state){ 
            owChangeState(THIS, motherow_new_state);
        }
    //UPDATE POSITION
        if(show_tip == 0u){
            update_position_motherow(THIS);
        }
    //INTERACT WITH MAP
        motherow_interact_with_map(THIS);
    //CHECK COLLIDED PLACE
        ow_check_place(THIS);
}

void motherownormal_setanim_walkh() BANKED{
    SetSpriteAnim(THIS, motherow_anim_h, 10u);
}
void motherownormal_setanim_walkup() BANKED{
    SetSpriteAnim(THIS, motherow_anim_up, 12u);
}
void motherownormal_setanim_walkdown() BANKED{
    SetSpriteAnim(THIS, motherow_anim_down, 12u);
}
void motherownormal_setanim_idleh() BANKED{
    SetSpriteAnim(THIS, motherow_anim_idle_h, 4u);
}
void motherownormal_setanim_idledown() BANKED{
    SetSpriteAnim(THIS, motherow_anim_idle_down, 4u);
}
void motherownormal_setanim_idleup() BANKED{
    SetSpriteAnim(THIS, motherow_anim_idle_up, 4u);
}

void DESTROY(){

}