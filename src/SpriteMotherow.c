#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Sound.h"

#include "custom_datas.h"
#include "Dialogs.h"

#define OW_NORMAL_FRAMESKIP 1
#define OW_PATH_FRAMESKIP 3

const UINT8 motherow_anim_down[] = {4, 0, 8, 1, 8}; //The first number indicates the number of frames
const UINT8 motherow_anim_up[] = {2, 2, 3}; //The first number indicates the number of frames
const UINT8 motherow_anim_h[] = {4, 4, 7, 6, 7}; //The first number indicates the number of frames
const UINT8 motherow_anim_idle_h[] = {1, 5}; //The first number indicates the number of frames
const UINT8 motherow_anim_idle_up[] = {1, 2}; //The first number indicates the number of frames
const UINT8 motherow_anim_idle_down[] = {1, 1}; //The first number indicates the number of frames

extern UINT8 hudow_opened;
extern UINT8 show_tip;
extern UINT8 showed_tip;
extern UINT8 showed_tip_goback;
extern TIP_TO_BE_LOCALIZED tip_to_show;
extern INT8 sfx_cooldown;
extern UINT8 just_started;
extern UINT8 current_map;
extern UINT16 motherow_pos_x;
extern UINT16 motherow_pos_y;
extern struct MISSION outwalker_chief;

struct OwSpriteInfo* motherow_info = 0;
UINT8 frameskip = 0u;
UINT8 frameskip_max = 1u;// same as OW_NORMAL_FRAMESKIP
FA2OW_SPRITE_STATES new_state = 0;
UINT8 step_counter = 0u;
UINT8 teleporting = 0u;

void owChangeState(FA2OW_SPRITE_STATES new_state);
void owTips(TIP_TO_BE_LOCALIZED forced_tip) BANKED;
void update_position_motherow() BANKED;
void ow_check_place() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ShowTipOW() BANKED;
extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;
extern void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;
extern void pickup(struct ItemSpawned* pickedup_data) BANKED;
extern void spawn_step(UINT16 stepx, UINT16 stepy) BANKED;

void START(){
    new_state = IDLE_DOWN;
    motherow_info = (struct OwSpriteInfo*) THIS->custom_data;
    motherow_info->ow_state = IDLE_DOWN;
    motherow_info->tile_collision = 0u;
    motherow_info->vx = 0u;
    motherow_info->vy = 0u;
    frameskip = 0u;
    step_counter = 0u;
    frameskip_max = OW_NORMAL_FRAMESKIP;
    teleporting = 0u;
}

void update_position_motherow() BANKED{
    //new_state = motherow_info->ow_state;
    //WALKING
        if(show_tip == 0u){        
            if(frameskip < frameskip_max){frameskip++;}else{frameskip = 0u;}
            if(frameskip == 0u){
                motherow_info->tile_collision = TranslateSprite(THIS, motherow_info->vx << delta_time, motherow_info->vy << delta_time);
            }
            UINT8 scroll_tile = GetScrollTile((THIS->x >> 3), (THIS->y >> 3));
            if(current_map != 2u){//NOT IN MAZE
                switch(scroll_tile){
                    //case 8u:
                    case 40u: case 92u:
                    case 93u: case 94u:
                    case 99u: case 102u: 
                        //SFX
                            if(THIS->anim_frame == 1){
                                my_play_fx(CHANNEL_1, 60, 0x13, 0x21, 0xf8, 0xb9, 0x82);//SFX_OW_STEP
                                //STEP
                                    if(motherow_info->ow_state == WALK_DOWN ||
                                        motherow_info->ow_state == WALK_UP ||
                                        motherow_info->ow_state == WALK_LEFT ||
                                        motherow_info->ow_state == WALK_RIGHT){
                                        step_counter++;
                                        if(step_counter == 8u){
                                            step_counter = 0u;
                                            UINT16 step_x = THIS->x;
                                            UINT16 step_y = THIS->y;
                                            switch(motherow_info->ow_state){
                                                case WALK_DOWN:
                                                    step_x += 1u;
                                                    step_y -= 12u;
                                                break;
                                                case WALK_UP:
                                                    step_x += 1u;
                                                    step_y -= 6u;
                                                break;
                                                case WALK_LEFT:
                                                    step_x += 2u;
                                                    step_y -= 8u;
                                                break;
                                                case WALK_RIGHT:
                                                    step_x -= 2u;
                                                    step_y -= 8u;
                                                break;
                                            }
                                            spawn_step(step_x, step_y);
                                        }
                                    }
                            }
                            if(frameskip_max != OW_PATH_FRAMESKIP){
                                frameskip_max = OW_PATH_FRAMESKIP;
                            }
                    break;
                    default:
                        if(frameskip_max != OW_NORMAL_FRAMESKIP){
                            frameskip_max = OW_NORMAL_FRAMESKIP;
                        }
                }
            }
        }
}

void UPDATE(){
    if(just_started == 0u){
        new_state = WALK_UP;
        update_position_motherow();
        owChangeState(new_state);
        ow_check_place();
        return; 
    }
    if(sfx_cooldown > 0){sfx_cooldown--;}
    if(hudow_opened == 1u){return;}
    //new_state = motherow_info->ow_state;    
    if(KEY_PRESSED(J_DOWN)){new_state = WALK_DOWN;}
    else if(KEY_PRESSED(J_UP)){new_state = WALK_UP;}
    else if(KEY_PRESSED(J_LEFT)){new_state = WALK_LEFT;}
    else if(KEY_PRESSED(J_RIGHT)){new_state = WALK_RIGHT;}
    else if(KEY_RELEASED(J_DOWN)){new_state = IDLE_DOWN;}
    else if(KEY_RELEASED(J_UP)){new_state = IDLE_UP;}
    else if(KEY_RELEASED(J_LEFT)){new_state = IDLE_LEFT;}
    else if(KEY_RELEASED(J_RIGHT)){new_state = IDLE_RIGHT;}  
    update_position_motherow();  
    if(motherow_info->ow_state != new_state){ 
        owChangeState(new_state);
    }
    //CHECK COLLIDED PLACE
        ow_check_place();
    //INTERACT WITH MAP
        if(motherow_info->tile_collision){//diverso da zero
            if(KEY_TICKED(J_A) || KEY_TICKED(J_B)){
                owTips(TIP_NOTHING);            
            }
        }
    //INTERACT WITH SPRITES    
        UINT8 mow_a_tile;
        Sprite* imowspr;
        SPRITEMANAGER_ITERATE(mow_a_tile, imowspr) {
            if(CheckCollision(THIS, imowspr)) {
                switch(imowspr->type){
                    case SpriteBlackieow:
                        if(KEY_TICKED(J_A) || KEY_TICKED(J_B)){
                            trigger_dialog(BLACKIE, THIS);
                        }
                    break;
                    case SpriteItemspawned:
                        {
                            struct ItemSpawned* spawned_data = (struct ItemSpawned*) imowspr->custom_data;
                            if(spawned_data->configured == 4u){//means hidden
                                if(KEY_TICKED(J_A) || KEY_TICKED(J_B)){
                                    pickup(spawned_data);
                                    switch(spawned_data->itemtype){
                                        case INVITEM_ARROW_BASTARD:
                                        case INVITEM_ARROW_NORMAL:
                                        case INVITEM_ARROW_PERFO:
                                            owTips(TIP_HIDDEN_ARROWS);            
                                        break;
                                    }
                                    SpriteManagerRemoveSprite(imowspr);
                                }
                            }
                        }
                    break;
                    case SpriteTeleport:
                        //teleport!
                            {
                                struct TeleportInfo* tport_data = (struct TeleportInfo*) imowspr->custom_data;
                                motherow_pos_x = tport_data->dest_x;
                                motherow_pos_y = tport_data->dest_y;
                                teleporting = 1u;
                                ChangeState(StateOverworld, THIS, -1);
                            }
                    break;
                }
            }
        };
}

void ow_check_place() BANKED{//tile collision
    if(motherow_info->tile_collision){
        switch(motherow_info->tile_collision){
            case 16u:
            case 50u:
            case 51u:
                switch(current_map){
                    case 0u:
                        ChangeState(StateMine, THIS, -1);
                    break;
                    case 1u:
                        if(outwalker_chief.mission_state != MISSION_STATE_DISABLED){
                            ChangeState(StateOutwalkers, THIS, -1);
                        }else{
                            trigger_dialog(OUTWALKER_NO_ENTER, THIS);
                        }
                    break;
                }
            break;
            case 62u:
            case 64u:
                just_started = 1u;
                switch(current_map){
                    case 0u:
                        ChangeState(StateExzoo, THIS, -1);
                    break;
                }
            break;
            case 70u:
            case 72u:
                ChangeState(StateCemetery, THIS, -1);
            break;
            case 90u:
            case 91u:
                switch(current_map){
                    case 0u:
                        //ChangeState(StateBlackiecave, THIS);
                    break;
                    case 1u:
                        ChangeState(StateOverworld, THIS, 2);
                    break;
                }
            break;
            case 17u:
            case 95u:
            case 96u:
                switch(current_map){
                    case 0u:
                        ChangeState(StateBlackiecave, THIS, -1);
                    break;
                    case 1u:
                        if(outwalker_chief.mission_state == MISSION_STATE_DISABLED){
                            trigger_dialog(MAZE_CANT_GO, THIS);
                        }else{ChangeState(StateOverworld, THIS, 2);}
                    break;
                }
            break;
        }
    }
}

void owTips(TIP_TO_BE_LOCALIZED forced_tip) BANKED{
    UINT8 trigger_tip = 0u;
    switch(forced_tip){
        case TIP_NOTHING:
            switch(motherow_info->tile_collision){//sul gioco completo sarà switch su mappa e poi su tile
                case 66u:
                case 68u://HOSPITAL
                    ChangeState(StateHospital, THIS, -1);
                break;
                case 83u:
                case 85u://SMITH, POLICE
                {
                    switch(current_map){
                        case 0u:
                            ChangeState(StateSmith, THIS, -1);
                        break;
                        case 1u:
                            if(outwalker_chief.mission_state == MISSION_STATE_DISABLED){
                                trigger_dialog(POLICE_0_GET_PASS, THIS);
                            }else if(outwalker_chief.mission_state == MISSION_STATE_ENABLED &&
                                outwalker_chief.current_step <= 3){
                                trigger_dialog(POLICE_0_STILL_NOT_FOUND, THIS);
                            }else{
                                trigger_dialog(POLICE_0_WONT_TALK, THIS);
                            }
                        break;
                    }
                }
                break;
                case 46u:
                case 47u:
                    switch(current_map){
                        case 0u://sw map = BLACKIE CAVE
                            tip_to_show = TIP_BLACKIE_CAVE;
                        break;
                        case 1u:
                            tip_to_show = TIP_DARK_FOREST;
                        break;
                    }
                    trigger_tip = 1u;
                break;
                case 86u:
                case 87u:
                    switch(current_map){
                        case 0u://sw map = MINE CAVE
                            tip_to_show = TIP_MINE_CAVE;
                        break;
                        case 1u:
                            if(THIS->x > ((UINT16) 60u << 3)){tip_to_show = TIP_LABIRYNTH;}
                            else{tip_to_show = TIP_GROTTO;}
                        break;
                    }
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
        break;
        default:
            tip_to_show = forced_tip;
            trigger_tip = 1u;
    }
    if(trigger_tip){
        if(show_tip == 0u && showed_tip == 0u){
            ShowTipOW();
        }
    }
}

void owChangeState(FA2OW_SPRITE_STATES new_state){
    if(new_state == GENERIC_IDLE){
        motherow_info->vy = 0;
        motherow_info->vx = 0;
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
            motherow_info->vy = 0;
            motherow_info->vx = 0;
            SetSpriteAnim(THIS, motherow_anim_idle_down, 4u);
        break;
        case IDLE_UP:
            motherow_info->vy = 0;
            motherow_info->vx = 0;
            SetSpriteAnim(THIS, motherow_anim_idle_up, 4u);
        break;
        case IDLE_LEFT:
            motherow_info->vy = 0;
            motherow_info->vx = 0;
            SetSpriteAnim(THIS, motherow_anim_idle_h, 4u);
            THIS->mirror = V_MIRROR;
        break;
        case IDLE_RIGHT:
            motherow_info->vx = 0;
            motherow_info->vy = 0;
            SetSpriteAnim(THIS, motherow_anim_idle_h, 4u);
            THIS->mirror = NO_MIRROR;
        break;
        case WALK_DOWN:
            motherow_info->vx = 0;
            motherow_info->vy = 1;
            SetSpriteAnim(THIS, motherow_anim_down, 12u);
        break;
        case WALK_UP:
            motherow_info->vx = 0;
            motherow_info->vy = -1;
            SetSpriteAnim(THIS, motherow_anim_up, 12u);
        break;
        case WALK_LEFT:
            motherow_info->vx = -1;
            motherow_info->vy = 0;
            SetSpriteAnim(THIS, motherow_anim_h, 10u);
            THIS->mirror = V_MIRROR;
        break;
        case WALK_RIGHT:
            motherow_info->vx = 1;
            motherow_info->vy = 0;
            SetSpriteAnim(THIS, motherow_anim_h, 10u);
            THIS->mirror = NO_MIRROR;
        break;
    }
    motherow_info->ow_state = new_state;
}

void DESTROY(){

}