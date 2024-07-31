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
#include "DialogTips.h"

#define OW_NORMAL_FRAMESKIP 1
#define OW_PATH_FRAMESKIP 3

const UINT8 motherow_anim_down[] = {4, 9, 0, 9, 1}; //The first number indicates the number of frames
const UINT8 motherow_anim_up[] = {4, 4,3,4,2}; //The first number indicates the number of frames
const UINT8 motherow_anim_h[] = {4, 5, 7, 6, 7}; //The first number indicates the number of frames
const UINT8 motherow_anim_idle_h[] = {1, 7}; //The first number indicates the number of frames
const UINT8 motherow_anim_idle_up[] = {1, 4}; //The first number indicates the number of frames
const UINT8 motherow_anim_idle_down[] = {6, 8,8,8,8,9,10}; //The first number indicates the number of frames

extern UINT8 hudow_opened;
extern UINT8 show_tip;
extern UINT8 showed_tip;
extern UINT8 showed_tip_goback;
extern TIP_TO_BE_LOCALIZED tip_to_show;
extern INT8 sfx_cooldown;
extern UINT8 just_started;
extern INT8 current_map;
extern UINT16 motherow_pos_x;
extern UINT16 motherow_pos_y;
extern struct MISSION find_blackie;
extern struct MISSION engage_smith;
extern struct MISSION enable_hospital;
extern struct MISSION outwalker_chief;
extern struct MISSION outwalker_glass;
extern struct MISSION defeat_scorpions;
extern struct MISSION find_antidote;
extern struct MISSION hungry_people;
extern struct MISSION fix_bridge;
extern INT8 show_tip_movingscroll;
extern UINT8 scorpion_mission_goal;
extern SHOP current_shop;
extern UINT8 hidden_items_flags;
extern CHAPTERS chapter;
extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

Sprite* s_push_sign = 0;
struct PushASignData d_push_sign = {.collided_tile = 0u};
struct OwSpriteInfo* motherow_info = 0;
UINT8 frameskip = 0u;
UINT8 frameskip_max = 1u;// same as OW_NORMAL_FRAMESKIP
FA2OW_SPRITE_STATES new_state = 0;
UINT8 step_counter = 0u;
UINT8 teleporting = 0u;
UINT8 ow_pusha_hp = 0;
UINT8 ow_chitchat_counter = 0u;
UINT8 colliding_owpeople = 0u;

void owChangeState(FA2OW_SPRITE_STATES new_state);
void ow_tips(TIP_TO_BE_LOCALIZED forced_tip) BANKED;
void update_position_motherow() BANKED;
void ow_check_place() BANKED;
TIP_TO_BE_LOCALIZED ow_show_pusha_sign() BANKED;
UINT8 ow_manage_chitchat() BANKED;

extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void ShowTipOW() BANKED;
extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;
extern void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;
extern void pickup(struct ItemSpawned* pickedup_data) BANKED;
extern void spawn_step(UINT16 stepx, UINT16 stepy) BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;
extern UINT8 get_quantity(INVITEMTYPE itemtype) BANKED;

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
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void update_position_motherow() BANKED{
    if(show_tip == 0u){        
        if(frameskip < frameskip_max){frameskip++;}else{frameskip = 0u;}
        if(frameskip == 0u && (motherow_info->vx != 0 || motherow_info->vy != 0)){
            motherow_info->tile_collision = TranslateSprite(THIS, motherow_info->vx << delta_time, motherow_info->vy << delta_time);
        }
        UINT8 scroll_tile = GetScrollTile((THIS->x >> 3), (THIS->y >> 3));
        if(current_map != MAP_MAZE){//NOT IN MAZE
            switch(scroll_tile){
                case 40u: 
                case 99u: case 102u: case 103u: case 104u:
                    //SFX
                        if(THIS->anim_frame == 1){
                            my_play_fx(CHANNEL_2, 60, 0x43, 0x32, 0x29, 0x85, 0x00);//SFX_OW_STEP
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
    if(just_started == 1u){
        new_state = WALK_UP;
        update_position_motherow();
        owChangeState(new_state);
        if(motherow_info->tile_collision == 62u || motherow_info->tile_collision == 64u){
            ChangeState(StateTutorial, THIS, 0);
            just_started = 2;
        }
        return; 
    }
    if(sfx_cooldown > 0){sfx_cooldown--;}
    if(hudow_opened == 1u){return;}
    if(KEY_PRESSED(J_DOWN)){new_state = WALK_DOWN;}
    else if(KEY_PRESSED(J_UP)){new_state = WALK_UP;}
    else if(KEY_PRESSED(J_LEFT)){new_state = WALK_LEFT;}
    else if(KEY_PRESSED(J_RIGHT)){new_state = WALK_RIGHT;}
    else if(KEY_RELEASED(J_DOWN)){new_state = IDLE_DOWN;}
    else if(KEY_RELEASED(J_UP)){new_state = IDLE_UP;}
    else if(KEY_RELEASED(J_LEFT)){new_state = IDLE_LEFT;}
    else if(KEY_RELEASED(J_RIGHT)){new_state = IDLE_RIGHT;}  
    
    //INTERACT WITH SPRITES
        UINT8 mow_a_tile;
        Sprite* imowspr;
        SPRITEMANAGER_ITERATE(mow_a_tile, imowspr) {
            if(CheckCollision(THIS, imowspr)) {
                switch(imowspr->type){
                    case SpriteBlackieow:
                        if(find_blackie.current_step < 5u){
                            trigger_dialog(BLACKIE, THIS);
                        }
                    break;
                    case SpriteBottle:
                        {
                        struct ItemSpawned pass_data={.itemtype = INVITEM_MAP, .quantity = 1, .equippable = 0u};
                        pickup(&pass_data);
                        hungry_people.mission_state = MISSION_STATE_REWARDED;
                        SpriteManagerRemoveSprite(imowspr);
                        }
                    break;
                    case SpriteItemspawned:
                        {
                            struct ItemSpawned* spawned_data = (struct ItemSpawned*) imowspr->custom_data;
                            if(spawned_data->configured == 4u){//means hidden
                                //if(KEY_TICKED(J_FIRE) || KEY_TICKED(J_JUMP)){
                                    pickup(spawned_data);
                                    hidden_items_flags = hidden_items_flags | spawned_data->frmskip;
                                    switch(spawned_data->itemtype){
                                        case INVITEM_ARROW_BASTARD:
                                        case INVITEM_ARROW_NORMAL:
                                        case INVITEM_ARROW_PERFO:
                                            ow_tips(TIP_HIDDEN_ARROWS);            
                                        break;
                                        case INVITEM_METAL:
                                            ow_tips(TIP_HIDDEN_METAL);
                                        break;
                                        case INVITEM_MONEY:
                                            ow_tips(TIP_HIDDEN_MONEY);
                                        break;
                                        case INVITEM_WOOD:
                                            ow_tips(TIP_HIDDEN_WOOD);
                                        break; 
                                    }
                                    SpriteManagerRemoveSprite(imowspr);
                                //}
                            }
                        }
                    break;
                    case SpriteTeleport://teleport!
                        {
                            struct TeleportInfo* tport_data = (struct TeleportInfo*) imowspr->custom_data;
                            motherow_pos_x = tport_data->dest_x;
                            motherow_pos_y = tport_data->dest_y;
                            teleporting = 1u;
                            ChangeState(StateOverworld, THIS, -1);
                        }
                    break;
                    case SpriteOwcrab:
                        if(outwalker_glass.mission_state < MISSION_STATE_ACCOMPLISHED){
                            THIS->x = imowspr->x + 24u;
                            THIS->y = imowspr->y;
                            trigger_dialog(BOSS_CRAB_FIGHT, THIS);
                        }
                    break;
                    case SpriteOwscorpion:
                        {
                            struct EnemyData* scorp_data = (struct EnemyData*) imowspr->custom_data;
                            scorpion_mission_goal = scorp_data->configured;
                            ChangeState(StateScorpions, THIS, -1);
                        }
                    break;
                    case SpriteHerb:
                        {
                            struct ItemSpawned* herb_data = (struct ItemSpawned*) imowspr->custom_data;
                            find_antidote.current_step = find_antidote.current_step | herb_data->hp;
                            pickup(herb_data);
                            SpriteManagerRemoveSprite(imowspr);
                        }
                    break;
                    case SpriteOwfisherman:
                        THIS->x -= 4;
                        if(hungry_people.mission_state < MISSION_STATE_ACCOMPLISHED &&
                            hungry_people.mission_state > MISSION_STATE_DISABLED){
                            trigger_dialog(FISHERMAN_LETSGO, THIS);
                        }else{
                            trigger_dialog(FISHERMAN_THERESFISH, THIS);
                        }
                    break;
                    case SpriteOwpeople:
                            if(show_tip == 0u && showed_tip == 0u){
                                colliding_owpeople = 1u;
                            }
                            if(imowspr->x < THIS->x && new_state == WALK_LEFT){
                                motherow_info->vx = 0;}
                            if(imowspr->x > THIS->x && new_state == WALK_RIGHT){
                                motherow_info->vx = 0;}
                            if(imowspr->y > THIS->y && new_state == WALK_DOWN){
                                motherow_info->vy = 0;}
                            if(imowspr->y < THIS->y && new_state == WALK_UP){
                                motherow_info->vy = 0;}
                    break;
                }
            }
        };
    if(motherow_info->ow_state != new_state){ 
        owChangeState(new_state);
    }
    update_position_motherow();
    //INTERACT WITH MAP
        if(motherow_info->tile_collision && motherow_info->tile_collision != d_push_sign.collided_tile){
           d_push_sign.collided_tile = motherow_info->tile_collision;
           tip_to_show = ow_show_pusha_sign();
        }
        if(colliding_owpeople && tip_to_show == 0){
            tip_to_show = TIP_CHITCHAT;
            colliding_owpeople = 0; 
        }
        if((ow_pusha_hp || colliding_owpeople) && (KEY_TICKED(J_FIRE) || KEY_TICKED(J_JUMP))){
             ow_tips(tip_to_show);            
        }
    //CHECK COLLIDED PLACE
        ow_check_place();
}

TIP_TO_BE_LOCALIZED ow_show_pusha_sign() BANKED{
    TIP_TO_BE_LOCALIZED result = TIP_NOTHING;
    switch(d_push_sign.collided_tile){
        case 66u:
        case 65u://HOSPITAL
        case 83u:
        case 85u://SMITH, POLICE
        case 46u:
        case 47u://GREEN CARTEL
        case 86u:
        case 87u://YELLOW CARTEL
            result = TIP_SOMETHING;            
        break;
        case 162u:
        case 167u://HOUSE DOORS
            result = TIP_CHITCHAT;
        break;
    }
    if(result != TIP_NOTHING && ow_pusha_hp == 0u){
        ow_pusha_hp = 80u;
        s_push_sign = SpriteManagerAdd(SpriteOwpusha, THIS->x - 1u, THIS->y - 26u);
    }
    return result;
}

void ow_check_place() BANKED{//tile collision
    switch(d_push_sign.collided_tile){
        case 16u:
        case 50u:
        case 51u:
            switch(current_map){
                case MAP_SOUTHWEST:
                    ChangeState(StateMine, THIS, -1);
                break;
                case MAP_NORTHWEST:
                    if((outwalker_chief.mission_state <= MISSION_STATE_ENABLED
                        && get_quantity(INVITEM_PASS) == 1)
                        || outwalker_chief.mission_state >= MISSION_STATE_STARTED
                    ){
                        if(outwalker_chief.mission_state <= MISSION_STATE_ENABLED
                            && get_quantity(INVITEM_PASS) == 1){
                            change_quantity(INVITEM_PASS, -1);
                            outwalker_chief.mission_state = MISSION_STATE_STARTED;
                        }
                        ChangeState(StateOutwalkers, THIS, -1);
                    }else{
                        trigger_dialog(OUTWALKER_NO_ENTER, THIS);
                    }
                break;
            }
        break;
        case 62u:
        case 64u://EXZOO
            switch(current_map){
                case MAP_SOUTHWEST:
                    ChangeState(StateExzoo, THIS, -1);
                break;
            }
        break;
        case 70u:
        case 72u://CEMETERY
            ChangeState(StateCemetery, THIS, -1);
        break;
        case 90u:
        case 18u:
        case 91u://CAVE
            switch(current_map){
                case MAP_SOUTHWEST:
                    //ChangeState(StateBlackiecave, THIS);
                break;
                case MAP_NORTHWEST:
                    ow_tips(TIP_STILL_SOMETHING);
                break;
                case MAP_SOUTHEAST://to Scorpion Boss fight
                    if(find_antidote.mission_state < MISSION_STATE_STARTED){
                        ow_tips(TIP_STILL_SOMETHING);
                    }else{
                        ChangeState(StateBossscorpion, THIS, -1);
                    }
                break;
                case MAP_EAST://to Silver Cave
                    ChangeState(StateSilvercave, THIS, -1);
                break;
            }
        break;
        case 17u:
        case 95u:
        case 96u:
            switch(current_map){
                case MAP_SOUTHWEST:
                    if(engage_smith.mission_state != MISSION_STATE_REWARDED
                    && enable_hospital.mission_state != MISSION_STATE_REWARDED){
                        ow_tips(TIP_STILL_SOMETHING);
                    }else{
                        ChangeState(StateBlackiecave, THIS, -1);
                    }
                break;
                case MAP_NORTHWEST:
                    if(outwalker_chief.mission_state == MISSION_STATE_DISABLED){
                        trigger_dialog(MAZE_CANT_GO, THIS);
                    }else{ChangeState(StateOverworld, THIS, 2);}
                break;
                case MAP_SOUTHEAST://to scorpioncav
                    if(chapter < CHAPTER_4_SHIP){
                        ow_tips(TIP_STILL_SOMETHING);
                    }else{
                        ChangeState(StateBatcave, THIS, -1);
                    }
                break;
            }
        break;
        case 149u://BRIDGE
            if(fix_bridge.mission_state >= MISSION_STATE_ACCOMPLISHED){
                ChangeState(StateBridge, THIS, -1);
            }else{trigger_dialog(BRIDGE_BROKEN, THIS);}
        break;
    }
    //motherow_info->tile_collision = 0;
}

void ow_tips(TIP_TO_BE_LOCALIZED forced_tip) BANKED{
    UINT8 trigger_tip = 0u;
    switch(forced_tip){
        case TIP_NOTHING://donothing
        break;
        case TIP_SOMETHING:
            UINT16 t_coll = 0u;
            if(d_push_sign){
                t_coll = d_push_sign.collided_tile;
            }else if(motherow_info->tile_collision){
                t_coll = motherow_info->tile_collision;
            } 
            switch(t_coll){
                case 65u:
                case 66u://HOSPITAL
                    ChangeState(StateHospital, THIS, -1);
                break;
                case 83u:
                case 85u://SMITH, POLICE
                {
                    switch(current_map){
                        case MAP_SOUTHWEST:
                            current_shop = SHOP_SMITH;
                            ChangeState(StateShop, THIS, -1);
                        break;
                        case MAP_NORTHWEST:
                            if(outwalker_chief.mission_state == MISSION_STATE_DISABLED){
                                trigger_dialog(POLICE_0_GET_PASS, THIS);
                            }else if(outwalker_chief.mission_state == MISSION_STATE_ENABLED){
                                if(outwalker_chief.current_step <= 3){
                                    trigger_dialog(POLICE_0_STILL_NOT_FOUND, THIS);
                                }else{
                                    trigger_dialog(POLICE_0_WONT_TALK, THIS);
                                }
                            }else if(defeat_scorpions.mission_state >= MISSION_STATE_ENABLED &&
                                defeat_scorpions.mission_state < MISSION_STATE_ACCOMPLISHED){
                                trigger_dialog(POLICE_0_FIGHTING,THIS);
                            }else{
                                trigger_dialog(POLICE_0_NOGUARDS, THIS);
                            }
                        break;
                        case MAP_SOUTHEAST://carpenter
                            current_shop = SHOP_CARPENTER;
                            ChangeState(StateShop, THIS, -1);
                        break;
                    }
                }
                break;
                case 46u:
                case 47u://GREEN CARTEL
                    switch(current_map){
                        case MAP_SOUTHWEST://sw map = BLACKIE CAVE
                            tip_to_show = TIP_BLACKIE_CAVE;
                        break;
                        case MAP_NORTHWEST:
                            tip_to_show = TIP_DARK_FOREST;
                        break;
                    }
                    trigger_tip = 1u;
                break;
                case 86u:
                case 87u:// YELLOW CARTEL
                    switch(current_map){
                        case MAP_SOUTHWEST://sw map = MINE CAVE
                            tip_to_show = TIP_MINE_CAVE;
                        break;
                        case MAP_NORTHWEST:
                            if(THIS->x > ((UINT16) 60u << 3)){tip_to_show = TIP_LABIRYNTH;}
                            else{tip_to_show = TIP_GROTTO;}
                        break;
                        case 3u:
                            tip_to_show = TIP_FISHERMAN;
                        break;
                        case 4u://EAST
                            tip_to_show = TIP_SILVER_MINE;
                        break;
                    }
                    trigger_tip = 1u;
                break;
                case 16u:
                case 18u://SEA SOUTH
                    tip_to_show = TIP_OWLIMIT_SOUTH;
                    trigger_tip = 1u;
                break;
            }
        break;
        case TIP_CHITCHAT:
            trigger_tip = ow_manage_chitchat();
        break;
        default:
            tip_to_show = forced_tip;
            trigger_tip = 1u;
    }
    if(trigger_tip){
        if(show_tip == 0u && showed_tip == 0u){
            UINT8 rest = scroll_target->x % 8;
            if(rest > 0){
                scroll_target->x+=(8-rest); 
            }
	        show_tip_movingscroll = 0u;
            ShowTipOW();
        }
    }
}

UINT8 ow_manage_chitchat() BANKED{
    //o trigger_dialog(POLICE_0_GET_PASS, THIS);
    //o tip_to_show = TIP_OWLIMIT_SOUTH; trigger_tip = 1u;
    UINT8 trigger_tip = 0u;
    if(hungry_people.mission_state > MISSION_STATE_DISABLED &&
            hungry_people.mission_state < MISSION_STATE_ACCOMPLISHED){
        tip_to_show = TIP_CHITCHAT_HUNGRY; 
        trigger_tip = 1u;
    }else{
        switch(chapter){
            case CHAPTER_0_BLACKIE:
                switch(ow_chitchat_counter){
                    case 0u:tip_to_show = TIP_CHITCHAT_SMITH;trigger_tip = 1u;break;
                    case 1u:tip_to_show = TIP_CHITCHAT_HOSP;trigger_tip = 1u;break;
                    case 2u:tip_to_show = TIP_CHITCHAT_ITEMS;trigger_tip = 1u;break;
                    case 3u:tip_to_show = TIP_CHITCHAT_SALUTE;trigger_tip = 1u;break;
                    case 4u:tip_to_show = TIP_CHITCHAT_HIDDEN_00;trigger_tip = 1u;break;
                    case 5u:tip_to_show = TIP_CHITCHAT_HIDDEN_01;trigger_tip = 1u;break;
                    case 6u:tip_to_show = TIP_CHITCHAT_HIDDEN_02;trigger_tip = 1u;break;
                }
            break;
            case CHAPTER_1_BANDITS:
                switch(ow_chitchat_counter){
                    case 0u:tip_to_show = TIP_CHITCHAT_LABYRINTH;trigger_tip = 1u;break;
                    case 1u:tip_to_show = TIP_CHITCHAT_GLASS;trigger_tip = 1u;break;
                    case 2u:tip_to_show = TIP_CHITCHAT_MARGARET;trigger_tip = 1u;break;
                    case 3u:tip_to_show = TIP_CHITCHAT_MOUNTAINS;trigger_tip = 1u;break;
                    case 4u:tip_to_show = TIP_CHITCHAT_LIAM;trigger_tip = 1u;break;
                    case 5u:tip_to_show = TIP_CHITCHAT_HIDDEN_10;trigger_tip = 1u;break;
                    case 6u:tip_to_show = TIP_CHITCHAT_HIDDEN_11;trigger_tip = 1u;break;
                }
            break;
        }
        ow_chitchat_counter++;
        if(ow_chitchat_counter >= 7){
            ow_chitchat_counter = 0u;
        }
    }
    return trigger_tip;              
}

void owChangeState(FA2OW_SPRITE_STATES new_state){
    if(showed_tip != 0) return;
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