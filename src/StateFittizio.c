#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "BankManager.h"
#include "Palette.h"
#include "Sound.h"
#include "Music.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Fade.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "sgb_palette.h"
#include "Dialogs.h"

#define CAMERA_DELTA_RIGHT 40
#define CAMERA_DELTA_LEFT 32

DECLARE_MUSIC(bgm_intro);
DECLARE_MUSIC(bgm_ow);
DECLARE_MUSIC(bgm_exzoo);
DECLARE_MUSIC(bgm_cemetery);
DECLARE_MUSIC(bgm_blackiecave);
DECLARE_MUSIC(bgm_mine);
DECLARE_MUSIC(bgm_hood);
DECLARE_MUSIC(bgm_gameover);
DECLARE_MUSIC(bgm_reward);

DECLARE_MUSIC(_03_ow2_noch2);
DECLARE_MUSIC(_03_ow2);
DECLARE_MUSIC(_04);
DECLARE_MUSIC(_04_noch2);

extern struct InvItem itemEquipped;
extern struct MISSION help_cemetery_woman;
extern INT8 motherpl_hp;
extern INT8 motherpl_surf_dx;
extern MOTHERPL_STATE motherpl_state;
extern Sprite* s_surf;
extern struct InvItem itemMoney;
extern UINT8 arrows_onscreen;
extern UINT8 motherpl_blocked_cooldown;
extern UINT8 spawnitem_random;
extern UINT8 enemy_random_30_100;
extern UINT8 motherpl_hit_cooldown;
extern WHOSTALKING whostalking;
extern Sprite* s_motherow;
extern unsigned char log0[];
extern UINT8 current_map;
extern UINT8 teleporting;

UINT8 mine_powderspawned = 3u;
UINT8 npc_spawned_zone = 0u;
Sprite* s_motherpl = 0;
UINT8 init_enemy = 0u;
INT8 hud_motherpl_hp = 0;
UINT8 camera_ok = 0u;
MirroMode motherpl_mirror = NO_MIRROR; 
UINT16 motherpl_pos_x = 0u;
UINT16 motherpl_pos_y = 0u;
UINT16 motherow_pos_x = 0u;
UINT16 motherow_pos_y = 0u;
struct EtoReload e_to_reload[3];
UINT8 enemy_counter = 0u;
UINT8 mapwidth;
UINT8 mapheight;
UINT8 previous_state;
UINT8 item_spawned_cooldown = 255u;
INT8 sfx_cooldown = 0u;
UINT8 dialog_bg_cooldown = 32u;
UINT8 dialog_bg_activated = 0u;
UINT8 dialog_bg_charcounter = 0u;
UINT8 generic_counter = 0u;
unsigned char dbg1[50];
UINT8 just_started = 1u;
UINT8 logtimeout = 10u;
UINT8 ow_pusha_hp = 0u;

void UpdateHUD() BANKED;
void Log(NPCNAME npcname) BANKED;
void update_camera_position() BANKED;
void update_camera_position_ow() BANKED;
void camera_tramble() BANKED;
void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
void spawn_npc(UINT8 type, UINT16 posx, UINT16 posy, NPCTYPE head, NPCTYPE body, MirroMode mirror, WHOSTALKING whos, NPCNAME npcname) BANKED;
void spawn_item(INVITEMTYPE itemtype, UINT16 x, UINT16 y) BANKED;
void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;
void manage_bgm(UINT8 new_state, UINT8 previous_state, INT8 next_map) BANKED;
void trigger_dialog_bg(UINT8 on_off, UINT8 x, UINT8 y, UINT8 nchar) BANKED;
void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;
void save_mother_pos(UINT8 sprite_type, UINT16 x, UINT16 y) BANKED;
void check_sgb_palette(UINT8 new_state) BANKED;
void play_music_reward() BANKED;
void restartFromHospital() BANKED;

extern void ChangeStateThroughBetween(UINT8 new_state, UINT8 previous_state) BANKED;

void restartFromHospital() BANKED{
    current_map = 0;
    motherpl_hp = 5;
    motherow_pos_x = (UINT16) 35u << 3;
    motherow_pos_y = (UINT16) 20u << 3;
    previous_state = StateOverworld;
    ChangeStateThroughBetween(StateHospital, StateOverworld);
}

void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED{
    whostalking = whost;
    ChangeState(StateDialog, s_mother, -1);
}

void play_music_reward() BANKED{
    StopMusic;
    PlayMusic(bgm_reward, 0);
}

void manage_bgm(UINT8 new_state, UINT8 previous_state, INT8 next_map) BANKED{
    if(previous_state == new_state && next_map == current_map){
        return;
    }
    switch(new_state){
        case StateInventory:
            PauseMusic;
            my_play_fx(CHANNEL_1, 60, 0x36, 0x9f, 0xf6, 0x91, 0x86);//SFX_START
        break;
        case StateDialog:
            if(previous_state == StateTitlescreen){
                PauseMusic;
            }
            switch(whostalking){
                case INTRO:
                    PlayMusic(bgm_intro, 1);
                break;
                case DEATH:
                    PlayMusic(bgm_gameover, 1);
                break;
            }
        break;
        case StateOverworld:
        	StopMusic;
            if(just_started == 0){
                switch(current_map){
                    case 0u:PlayMusic(_03_ow2_noch2, 1);break;
                    case 1u:PlayMusic(bgm_ow, 1);break;
                    case 2u:break;
                }
            }            
        break;
        case StateExzoo:
            if(previous_state == StateInventory){ResumeMusic;}
            else{StopMusic;PlayMusic(bgm_exzoo, 1);}
        break;
        case StateCemetery:
            if(previous_state == StateInventory){ResumeMusic;}
            else{StopMusic;PlayMusic(bgm_cemetery, 1);}
        break;
        case StateBlackiecave:
            if(previous_state == StateInventory){ResumeMusic;}
            else{StopMusic;PlayMusic(bgm_blackiecave, 1);}
        break;
        case StateMountain:
        case StateMine:
            if(previous_state == StateInventory){ResumeMusic;}
            else{StopMusic;PlayMusic(_04_noch2, 1);}//bgm_mine
        break;
        case StateHood:
            if(previous_state == StateInventory){ResumeMusic;}
            else{StopMusic;PlayMusic(bgm_hood, 1);}
        break;
    }
}

void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED{
    if(sfx_cooldown == 0){
        PlayFx(c, mute_frames, s0, s1, s2, s3, s4);
        sfx_cooldown = 20u;
    }
}

void save_mother_pos(UINT8 sprite_type, UINT16 x, UINT16 y) BANKED{
    switch(sprite_type){
        case SpriteMotherpl:
            motherpl_pos_x = x;
            motherpl_pos_y = y;
        break;
        case SpriteMotherow:
            motherow_pos_x = x;
            motherow_pos_y = y;
        break;
    }
}

void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED{
    manage_bgm(new_state, previous_state, next_map);
    UINT8 mfit_a_tile;
    Sprite* mfitspr;
    SPRITEMANAGER_ITERATE(mfit_a_tile, mfitspr) {
        UINT8 i = 0u;
        for(i = 0u; i < 3; ++i){
            if(e_to_reload[i].alive == 1 && mfitspr->type == e_to_reload[i].type){
                e_to_reload[i].x = mfitspr->x;
                e_to_reload[i].y = mfitspr->y;
            }
        }
    };
    npc_spawned_zone = 0u;
    if(s_mother == 0){
        if(previous_state == StateOverworld){s_mother = s_motherow;}
        else{s_mother = s_motherpl;}
    }
    if(new_state != current_state && next_map != current_map){
        save_mother_pos(s_mother->type, s_mother->x, s_mother->y);
    }
    //MOTHERPL POS X Y, HIDE_WIN
        if(current_state != StateInventory && current_state != StateDiary
            && current_state != StateDialog && current_state != StatePassword
            && current_state != StateHospital
            && teleporting == 0){
            switch(s_mother->type){
                case SpriteMotherow:
                    if(new_state == StateOverworld){
                        if(current_map == 2 && next_map == 1){
                            motherow_pos_x = (UINT16) 56u << 3;
                            motherow_pos_y = (UINT16) 42u << 3;
                        }else if(next_map == 2){
                            motherow_pos_x = (UINT16) 6u << 3;
                            motherow_pos_y = (UINT16) 3u << 3;
                        }
                    }
                break;
                case SpriteMotherpl:
                    switch(current_state){
                        case StateBlackieroom:
                            if(new_state == StateBlackiecave){
                                motherpl_pos_x = (UINT16)60u << 3;
                                motherpl_pos_y = (UINT16)8u << 3;
                            }
                        break;
                        case StateHood:
                            if(new_state == StateOverworld){
                                switch(next_map){
                                    case 0:
                                        motherow_pos_x = (UINT16)18u << 3;
                                        motherow_pos_y = (UINT16)4u << 3;
                                    break;
                                    case 1:
                                        motherow_pos_x = (UINT16) 14u << 3;
                                        motherow_pos_y = (UINT16) 46u << 3;
                                    break;
                                }
                            }
                        break;
                        case StateExzoo:
                            if(new_state == StateOverworld){
                                //gotta do it cause of the maze
                                motherow_pos_x = ((UINT16) 14u << 3) + 4u;
                                motherow_pos_y = (UINT16) 21u << 3;
                            }
                        break;
                        case StateSky:
                            if(new_state == StateMountain){//falling from the skyss
                                motherpl_pos_x = (UINT16)47u << 3;
                                motherpl_pos_y = (UINT16)5u << 3;
                            }
                        break;
                        case StateMountain:
                            if(new_state == StateOutwalkers){
                                motherpl_pos_x = (UINT16)93u << 3;
                                motherpl_pos_y = (UINT16)9u << 3;
                            }
                        break;
                    }
                    motherpl_mirror = s_mother->mirror;
                break;        
            }
            HIDE_WIN;
            SetWindowY(160);
        }
        if(just_started == 1){
            motherow_pos_x = ((UINT16) 14u << 3) + 4u;
            motherow_pos_y = (UINT16) 23u << 3;
        }
    //CURRENT_MAP vs NEXT_MAP
        if(next_map != -1){
            current_map = next_map;
        }
    //SGB PALETTE CHECK
        if(sgb_check()){
            check_sgb_palette(new_state);
        }
    if(previous_state != current_state){
        previous_state = current_state;
    }
    /*if(motherpl_state == MOTHERPL_DEAD){
        ChangeStateThroughBetween(new_state, previous_state);
    }else 
    */
   if(new_state != StateDialog && current_state != StateDialog){
	    ChangeStateThroughBetween(new_state, previous_state);
    }else{
        SetState(new_state);
    }
}

void check_sgb_palette(UINT8 new_state) BANKED{
    switch(new_state){
        case StateCart:
            set_sgb_palette01_cart();
            set_sgb_palette_statusbar();
        break;
        case StateBosscrab:
            set_sgb_crab();
            set_sgb_palette_statusbar();
        break;
        case StateOutwalkers:
        case StateHood:
            set_sgb_palette01_HOOD();
            set_sgb_palette_statusbar();
        break;
        case StateExzoo:
            set_sgb_palette01_ZOO();
            set_sgb_palette_statusbar();
        break;
        case StateSky:
            set_sgb_SKY();
            set_sgb_palette_statusbar();
        break;
        case StateSmith:
            set_sgb_palette01_WOLF();
            set_sgb_palette_statusbar();
        break;
        case StateBlackiecave:
            set_sgb_palette01_BLACKIECAVE();
            set_sgb_palette_statusbar();
        break;
        case StateCemetery:
            set_sgb_palette01_CEMATERYCRYPT();
            set_sgb_palette_statusbar();
        break;
        case StateMine:
            set_sgb_palette01_MINE();
            set_sgb_palette_statusbar();
        break;
        case StateMountain:
            set_sgb_palette01_MINE();
            set_sgb_palette_statusbar();
        break;
        case StateInventory:
            set_sgb_palette_inventory();
            reset_sgb_palette_statusbar();
        break;
        case StateDiary:
        case StateDialog:
        case StateOverworld:
            if(new_state == StateOverworld){
                switch(current_map){
                    case 0:set_sgb_palette01_worldmap();break;//sw
                    case 1:set_sgb_worldmap_nw();break;//nw
                    case 2:set_sgb_palette01_worldmap_maze();break;//maze
                }
            }
            reset_sgb_palette_statusbar();
        break;
    }
}

void UpdateHUD() BANKED{
    UINT8 idx_leftheart = 5;
    UINT8 idx_rightheart = 5;
    INT8 tmp_hp = motherpl_hp;
    //EQUIPPED ITEM
        switch(itemEquipped.itemtype){
            case INVITEM_MONEY:
                UPDATE_HUD_TILE(16,1,22);
                UPDATE_HUD_TILE(17,1,21);
                UPDATE_HUD_TILE(18,1,22);
            break;
	        case INVITEM_ARROW_NORMAL:
                UPDATE_HUD_TILE(16,1,6);
                UPDATE_HUD_TILE(17,1,5);
                UPDATE_HUD_TILE(18,1,17);
            break;
	        case INVITEM_ARROW_PERFO:
                UPDATE_HUD_TILE(16,1,6);
                UPDATE_HUD_TILE(17,1,5);
                UPDATE_HUD_TILE(18,1,18);
            break;
	        case INVITEM_ARROW_BASTARD:
                UPDATE_HUD_TILE(16,1,6);
                UPDATE_HUD_TILE(17,1,5);
                UPDATE_HUD_TILE(18,1,19);
            break;
	        case INVITEM_BOMB:
                UPDATE_HUD_TILE(16,1,6);
                UPDATE_HUD_TILE(17,1,5);
                UPDATE_HUD_TILE(18,1,20);
            break;
        }
        print_target = PRINT_WIN;
        if(itemEquipped.quantity < 10){ PRINT(16,2,"00%i", itemEquipped.quantity); }
        else if(itemEquipped.quantity < 100){ PRINT(16,2,"0%i", itemEquipped.quantity);}
        else {PRINT(16,2,"%i", itemEquipped.quantity);}
    //HP
        hud_motherpl_hp = motherpl_hp;
        for(idx_leftheart=5; idx_leftheart<15 ;idx_leftheart+=2){
            if(tmp_hp > 0){
                UPDATE_HUD_TILE(idx_leftheart,1,7);
                UPDATE_HUD_TILE(idx_leftheart,2,8);
                idx_rightheart++;
                UPDATE_HUD_TILE(idx_rightheart,1,9);
                UPDATE_HUD_TILE(idx_rightheart,2,10);
            }else{
                UPDATE_HUD_TILE(idx_leftheart,1,1);
                UPDATE_HUD_TILE(idx_leftheart,2,2);
                idx_rightheart++;
                UPDATE_HUD_TILE(idx_rightheart,1,3);
                UPDATE_HUD_TILE(idx_rightheart,2,4);
            }
            tmp_hp--;
            idx_rightheart++;
        }

}

void Log(NPCNAME npcname) BANKED{    
    /*switch(motherpl_state){
        case MOTHERPL_IDLE: PRINT(0, 0, "IDLE"); break;
        case MOTHERPL_JUMP: PRINT(0, 0, "JUMP"); break;
        case MOTHERPL_WALK: PRINT(0, 0, "WALK"); break;
        case MOTHERPL_HIT: PRINT(0, 0, " HIT"); break;
        case MOTHERPL_DEAD: PRINT(0, 0, "DEAD"); break;
        case MOTHERPL_CRAWL: PRINT(0, 0, "CRAW"); break;
        case MOTHERPL_CRAWL_SURF: PRINT(0, 0, "CRAS"); break;
        case MOTHERPL_DASH: PRINT(0, 0, "DASH"); break;
    }
    if(s_surf){
        PRINT(5, 0, "SURF%i",motherpl_surf_dx);
    }
    */
    /*else{
        if(hurricane_info->step < 10){
            PRINT(5, 2, "00%u",hurricane_info->step);   
        }else if(hurricane_info->step < 100){
            PRINT(5, 2, "0%u",hurricane_info->step);
        }else{
            PRINT(5, 2, "%u",hurricane_info->step);
        }
    }*/    
    //PRINT(10, 3, "AR:%u%u", arrows_onscreen, 5u);
    //PRINT(16, 0, "!LOG");
    if(logtimeout > 0){logtimeout--;}
    else{GetLocalizedLog_EN();}
    if(npcname != NONAME){
        if(logtimeout == 0u){logtimeout = 60u;}
        GetLocalizedLogName_EN(npcname);
    }
    PRINT(0,0,log0);   
}

void camera_tramble() BANKED{
    UINT8 cooldown_shifted = 1;
    if(motherpl_hit_cooldown > 50){
        cooldown_shifted = motherpl_hit_cooldown % 4;
    }
    else if(motherpl_hit_cooldown > 17){
        cooldown_shifted = motherpl_hit_cooldown % 6;
    } else {
        cooldown_shifted = motherpl_hit_cooldown % 8;
    }
    if(cooldown_shifted == 0){
        if(scroll_target->y > s_motherpl->y){
            scroll_target->y = ((UINT16)s_motherpl->y) - 6u;
        }else{
            scroll_target->y = ((UINT16)s_motherpl->y) + 20u;
        }
    }
}

void update_camera_position() BANKED{
    switch(current_state){
        case StateBlackieroom:
            if(scroll_target->x != (UINT16) 80u || 
                scroll_target->y != (UINT16) 56u){
                scroll_target->x = (UINT16) 80u;
                scroll_target->y = (UINT16) 56u;
            }
            return;
        break;
    }
    //LIMITS
        //HORIZONTAL
            if(s_motherpl->x < (UINT16)8u){
                s_motherpl->x = 8u;
                UINT8 next_ = -1;
                switch(current_state){
                    case StateHood:
                    case StateExzoo:
                        next_ = 0u;                
                        ChangeState(StateOverworld, s_motherpl, next_);
                    break;
                    case StateSky:
                        ChangeState(StateMountain, s_motherpl, -1);
                    break;
                    case StateMountain:
                        ChangeState(StateOutwalkers, s_motherpl, -1);
                    break;
                    default:
                        ChangeState(StateOverworld, s_motherpl, next_);
                    break;
                }
                
            }
            if(s_motherpl->x > (((UINT16)mapwidth) << 3) - 16u){
                s_motherpl->x = (((UINT16)mapwidth) << 3) - 16u;
                switch(current_state){
                    case StateHood:
                        if(help_cemetery_woman.current_step < 3u){
                        }else{//exiting hoods to the north
                            motherow_pos_x = 0u;
                            motherow_pos_y = 0u;
                            ChangeState(StateOverworld, s_motherpl, 1);
                        }
                    break;
                    case StateMountain:
                        ChangeState(StateSky, s_motherpl, -1);
                    break;
                    case StateOutwalkers:
                        ChangeState(StateMountain, s_motherpl, -1);
                    break;
                    case StateSky:
                        trigger_dialog(IBEX_GIVE_MISSION, s_motherpl);
                    break;
                    default:
                        ChangeState(StateOverworld, s_motherpl, 0);
                    break;
                }
            }
        //VERTICAL
            if(s_motherpl->y > (((UINT16) mapheight) << 3) && current_state != StateCart){
                s_motherpl->y = ((UINT16) mapheight) - 32u;
            }
            if(s_motherpl->y <= 0u){
                s_motherpl->y = 32u;
            }  
    //SCROLL TARGET Y    
        if(motherpl_hit_cooldown > 0){
            //CAMERA TRAMBLE
            camera_tramble();
        }else{
            if(scroll_target->y != (s_motherpl->y + 16u)){
                scroll_target->y = s_motherpl->y + 16u;
            }
        }
    //BLOCK 
        if(motherpl_state == MOTHERPL_BLOCKED ){return;}
        if(motherpl_blocked_cooldown > 0u){motherpl_blocked_cooldown--;return;}
    //camera fissa per certi stage
        UINT8 consider_margins = 0u;
        switch(current_state){
            case StateMine:
            case StateMountain:
            case StateBlackiecave:
                scroll_target->x = s_motherpl->x + 8u;
                if(motherpl_hit_cooldown == 0){
                    scroll_target->y = s_motherpl->y + 8u;
                }
                consider_margins = 1u;
            break;
        }
        if(consider_margins){
            return;
        }
    //in ogni caso non uscire dai margini
    if(s_surf){
        switch(s_motherpl->mirror){
            case NO_MIRROR:
                scroll_target->x = s_motherpl->x + CAMERA_DELTA_RIGHT;
            break;
            case V_MIRROR:
                scroll_target->x = s_motherpl->x - CAMERA_DELTA_LEFT;
            break;
        }
        return;
    }
    if((KEY_PRESSED(J_DOWN) && motherpl_state != MOTHERPL_JUMP) 
        || motherpl_state == MOTHERPL_PICKUP){
            camera_ok = 0u;
    }
    if(KEY_PRESSED(J_RIGHT) || KEY_PRESSED(J_LEFT) || motherpl_state == MOTHERPL_DASH
        || current_state == StateCart){
        if(camera_ok == 1u){
            switch(s_motherpl->mirror){
                case NO_MIRROR:
                    scroll_target->x = s_motherpl->x + CAMERA_DELTA_RIGHT;
                break;
                case V_MIRROR:
                    scroll_target->x = s_motherpl->x - CAMERA_DELTA_LEFT;
                break;
            }
        }else{
            switch(s_motherpl->mirror){
                case NO_MIRROR://going right
                    if (scroll_target->x < (s_motherpl->x + CAMERA_DELTA_RIGHT)){
                        scroll_target->x+=2;
                    }else if (!KEY_PRESSED(J_LEFT) 
                        && !KEY_PRESSED(J_DOWN)){
                        camera_ok = 1u;
                    }
                break;
                case V_MIRROR:
                    if(scroll_target->x > (s_motherpl->x - CAMERA_DELTA_LEFT)){
                        scroll_target->x-=2;
                    }else if (!KEY_PRESSED(J_RIGHT) && !KEY_PRESSED(J_DOWN)){
                        camera_ok = 1u;
                    }
                break;
            }
        }
    }
}

void ReloadEnemiesPL() BANKED{    
    init_enemy = 0u;
    enemy_counter = 0u;
    UINT8 i = 0u;
    for(i = 0u; i < 3u; ++i){
        if(e_to_reload[i].alive == 1u){
            SpriteManagerAdd(e_to_reload[i].type, e_to_reload[i].x, e_to_reload[i].y);
            enemy_counter++;
        }
    }
}

void spawn_npc(UINT8 type, UINT16 posx, UINT16 posy, NPCTYPE head, NPCTYPE body, MirroMode mirror, WHOSTALKING whos, NPCNAME npcname) BANKED{
    Sprite* s_head = SpriteManagerAdd(type, posx, posy);
    s_head->mirror = mirror;
    struct NpcInfo* head_data = (struct NpcInfo*) s_head->custom_data;
    head_data->type = head;
    head_data->whotalks = whos;
    Sprite* s_body = SpriteManagerAdd(type, posx, posy+16u);
    s_body->mirror = mirror;
    struct NpcInfo* body_data = (struct NpcInfo*) s_body->custom_data;
    body_data->type = body;
    body_data->whotalks = whos;
    body_data->npcname = npcname;
    head_data->configured = 1u;
    body_data->configured = 1u;
}

void spawn_item(INVITEMTYPE itemtype, UINT16 x, UINT16 y) BANKED{
    //SPAWN ITEM
    if(item_spawned_cooldown == 0u && mine_powderspawned){
        mine_powderspawned--;
        Sprite* reward = SpriteManagerAdd(SpriteItemspawned, x, y);
        struct ItemSpawned* reward_data = (struct ItemSpawned*) reward->custom_data;
        reward_data->itemtype = itemtype;
        reward_data->quantity = 1;
        reward_data->equippable = 0u;
        reward_data->configured = 1u;
        item_spawned_cooldown = 255u;
    }
}


void trigger_dialog_bg(UINT8 on_off, UINT8 x, UINT8 y, UINT8 nchar) BANKED{
/*	if(on_off == 0u){//spegni
        UINT8 i = 0u;
        for(i = 0u; i<nchar; i++){
            PRINT(x-i, y, " ");
        }
        dialog_bg_charcounter = 0u;
        dialog_bg_cooldown = 32u;
        dialog_bg_activated = 0u;
        return;
    }else if(on_off == 1 && dialog_bg_activated == 0u){
        dialog_bg_activated = 1u;
    }
    if(dialog_bg_activated == 1u){
        if(dialog_bg_cooldown > 0){
            dialog_bg_cooldown--;
        }else{
            memcpy(dbg1, "HORDE!A12\0", 10);
            print_target = PRINT_BKG;
            if(dialog_bg_charcounter == 0u){
                UINT8 i = 0u;
                for(i = 0u; i<nchar; i++){
                    PRINT(x-i, y, dbg1[dialog_bg_charcounter+(nchar-i)]);
                }
            }else if(dialog_bg_charcounter >= nchar){//SHIFTING
                dialog_bg_cooldown = 32u;
                UINT8 i = 0u;
                for(i = 0u; i<nchar; i++){
                    UINT8 yx = 0u;
                        for(yx = 0u; i<nchar; i++){
                    
                    PRINT(x-i, y, " ");
                }
                PRINT(x, y, dbg1[dialog_bg_charcounter]);
            }
            dialog_bg_charcounter++;
        }

    }*/
}

void START(){}

void UPDATE(){}