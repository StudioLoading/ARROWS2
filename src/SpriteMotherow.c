#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"
#include "Dialogs.h"

const UINT8 motherow_anim_down[] = {2, 0, 1}; //The first number indicates the number of frames
const UINT8 motherow_anim_up[] = {2, 2, 3}; //The first number indicates the number of frames
const UINT8 motherow_anim_h[] = {2, 4, 5}; //The first number indicates the number of frames
const UINT8 motherow_anim_idle_h[] = {1, 5}; //The first number indicates the number of frames
const UINT8 motherow_anim_idle_up[] = {1, 2}; //The first number indicates the number of frames
const UINT8 motherow_anim_idle_down[] = {1, 1}; //The first number indicates the number of frames

extern UINT8 hudow_opened;
extern UINT8 show_tip;
extern UINT8 showed_tip;
extern UINT8 showed_tip_goback;
extern TIP_TO_BE_LOCALIZED tip_to_show;

struct OwSpriteInfo* motherow_info = 0;
UINT8 frameskip = 0u;

void changeState(FA2OW_SPRITE_STATES new_state);
void owCollision();
extern void ChangeState(UINT8 new_state) BANKED;


void START(){
    motherow_info = (struct OwSpriteInfo*) THIS->custom_data;
    motherow_info->ow_state = IDLE_DOWN;
    motherow_info->tile_collision = 0u;
    frameskip = 0u;
}

void UPDATE(){
    if(hudow_opened == 1u){return;}
    FA2OW_SPRITE_STATES new_state = motherow_info->ow_state;
    
    //WALKING
    if(show_tip == 0u){
        if(frameskip < 2u){
            if(KEY_PRESSED(J_DOWN)){motherow_info->tile_collision = TranslateSprite(THIS, 0, 1 << delta_time);new_state = WALK_DOWN;}
            else if(KEY_PRESSED(J_UP)){motherow_info->tile_collision = TranslateSprite(THIS, 0, -1 << delta_time);new_state = WALK_UP;}
            else if(KEY_PRESSED(J_LEFT)){motherow_info->tile_collision = TranslateSprite(THIS, -1 << delta_time, 0);new_state = WALK_LEFT;}
            else if(KEY_PRESSED(J_RIGHT)){motherow_info->tile_collision = TranslateSprite(THIS, 1 << delta_time, 0);new_state = WALK_RIGHT;}
            frameskip++;
        }else{
            frameskip = 0u;
        }
    }
    if(KEY_RELEASED(J_DOWN)){new_state = IDLE_DOWN;}
    if(KEY_RELEASED(J_UP)){new_state = IDLE_UP;}
    if(KEY_RELEASED(J_LEFT)){new_state = IDLE_LEFT;}
    if(KEY_RELEASED(J_RIGHT)){new_state = IDLE_RIGHT;}
    
    if(motherow_info->ow_state != new_state){ 
        changeState(new_state);
    }

    //CHECK COLLIDED PLACE
    if(motherow_info->tile_collision){
        switch(motherow_info->tile_collision){
            case 50u:
            case 51u:
                ChangeState(StateCave);
            break;
            case 62u:
            case 64u:
                ChangeState(StateExzoo);
            break;
            case 70u:
            case 72u:
                ChangeState(StateCemetery);
            break;
            case 95u:
            case 96u:
                ChangeState(StateBlackiecave);
            break;
        }
    }

    //INTERACT
    if(motherow_info->tile_collision){//diverso da zero
        if(KEY_TICKED(J_A) || KEY_TICKED(J_B)){
            owCollision();            
        }
    }

}

void owCollision(){
    UINT8 trigger_tip = 0u;
    switch(motherow_info->tile_collision){//sul gioco completo sarà switch su mappa e poi su tile
        case 66u:
        case 68u://HOSPITAL
            tip_to_show = TIP_HOSPITAL_NO;
            trigger_tip = 1u;
        break;
        case 83u:
        case 85u://SMITH
            tip_to_show = TIP_SMITH_NO;
            trigger_tip = 1u;
        break;
        case 46u:
        case 47u://BLACKIE CAVESMITH
            tip_to_show = TIP_BLACKIE_CAVE;
            trigger_tip = 1u;
        break;
        case 86u:
        case 87u://MINE
            tip_to_show = TIP_MINE_CAVE;
            trigger_tip = 1u;
        break;
        case 56u:
        case 57u://EAST LIMIT
            tip_to_show = TIP_OWLIMIT_EAST;
            trigger_tip = 1u;
        break;
        case 16u:
        case 18u://SEA SOUTH
            tip_to_show = TIP_OWLIMIT_SOUTH;
            trigger_tip = 1u;
        break;
    }
    if(trigger_tip){
        if(show_tip == 0u && showed_tip == 0u){
            show_tip = 1u;
        }
        if(showed_tip == 1u){
            showed_tip_goback = 1u;
        }
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