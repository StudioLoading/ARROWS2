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
DECLARE_MUSIC(bgm_blackiecave);
DECLARE_MUSIC(bgm_mine);

extern struct InvItem itemEquipped;
extern struct MISSION find_blackie;
extern struct MISSION engage_smith;
extern struct MISSION missions[4];
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
INT8 sfx_cooldown = 127u;
UINT8 dialog_bg_cooldown = 32u;
UINT8 dialog_bg_activated = 0u;
UINT8 dialog_bg_charcounter = 0u;
UINT8 generic_counter = 0u;
unsigned char dbg1[50];
UINT8 just_started = 0u;

void UpdateHUD() BANKED;
void Log() BANKED;
void update_camera_position() BANKED;
void update_camera_position_ow() BANKED;
void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;
void spawn_npc(UINT8 type, UINT16 posx, UINT16 posy, NPCTYPE head, NPCTYPE body, MirroMode mirror, WHOSTALKING whos) BANKED;
void spawn_item(INVITEMTYPE itemtype, UINT16 x, UINT16 y) BANKED;
void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;
void manage_bgm(UINT8 new_state, UINT8 previous_state) BANKED;
void trigger_dialog_bg(UINT8 on_off, UINT8 x, UINT8 y, UINT8 nchar) BANKED;
void trigger_dialog(WHOSTALKING whost) BANKED;

extern void ChangeStateThroughBetween(UINT8 new_state, UINT8 previous_state) BANKED;

void trigger_dialog(WHOSTALKING whost) BANKED{
    
    whostalking = whost;
    ChangeState(StateDialog, s_motherpl);

}

void manage_bgm(UINT8 new_state, UINT8 previous_state) BANKED{
    if(previous_state == new_state){
        return;
    }
    switch(new_state){
        case StateInventory:
            PauseMusic;
        break;
        case StateDialog:
            if(previous_state == StateTitlescreen){
                PauseMusic;
            }
            switch(whostalking){
                case INTRO:
                    PlayMusic(bgm_intro, 1);
                break;
            }
        break;
        case StateOverworld:
        	StopMusic;
            PlayMusic(bgm_ow, 1);            
        break;
        case StateExzoo:
        case StateCemetery:
            if(previous_state == StateInventory){ResumeMusic;}
            else{StopMusic;PlayMusic(bgm_exzoo, 1);}
        break;
        case StateBlackiecave:
            if(previous_state == StateInventory){ResumeMusic;}
            else{StopMusic;PlayMusic(bgm_blackiecave, 1);}
        break;
        case StateMine:
            if(previous_state == StateInventory){ResumeMusic;}
            else{StopMusic;PlayMusic(bgm_mine, 1);}
        break;
    }
}

void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED{
    if(sfx_cooldown == 0){
        PlayFx(c, mute_frames, s0, s1, s2, s3, s4);
        sfx_cooldown = 20u;
    }
}

void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED{    
    if(new_state == StateInventory){
	    PauseMusic;
        my_play_fx(CHANNEL_1, 60, 0x36, 0x9f, 0xf6, 0x91, 0x86);//SFX_START
    }else if (current_state == StateInventory){
        ResumeMusic;
        //AUDIO REVERSE dell' entrata all' inventario, dato che ora ne sto uscendo
    }
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
    if(current_state != StateInventory && current_state != StateDiary
        && current_state != StateDialog){
        switch(s_mother->type){
            case SpriteMotherow:
                motherow_pos_x = s_mother->x;
                motherow_pos_y = s_mother->y;
            break;
            case SpriteMotherpl:
                motherpl_pos_x = s_mother->x;
                motherpl_pos_y = s_mother->y;
                switch(current_state){
                    case StateBlackieroom:
                        if(new_state == StateBlackiecave){
                            motherpl_pos_x = (UINT16)60u << 3;
                            motherpl_pos_y = (UINT16)8u << 3;
                        }
                    break;
                    case StateHood:
                        if(new_state == StateOverworld){
                            motherow_pos_x = (UINT16)18u << 3;
                            motherow_pos_y = (UINT16)4u << 3;
                        }
                    break;
                }
                motherpl_mirror = s_mother->mirror;
            break;        
        }
        HIDE_WIN;
        SetWindowY(160);
    }
    //SGB PALETTE
        if(sgb_check()){
            switch(new_state){
                case StateExzoo:
                    set_sgb_palette01_ZOO();
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
                case StateInventory:
                    set_sgb_palette_inventory();
                    reset_sgb_palette_statusbar();
                break;
                case StateDiary:
                case StateDialog:
                case StateOverworld:
                    reset_sgb_palette_statusbar();
                break;
            }
        }
    previous_state = current_state;
    manage_bgm(new_state, previous_state);
    if(motherpl_state == MOTHERPL_DEAD){
        motherpl_hp = 5;
        ChangeStateThroughBetween(new_state, previous_state);
    }else if(new_state != StateDialog && current_state != StateDialog){
	    ChangeStateThroughBetween(new_state, previous_state);
    }else{
        SetState(new_state);
    }
}

void UpdateHUD() BANKED{
    UINT8 idx_leftheart = 5;
    UINT8 idx_rightheart = 5;
    INT8 tmp_hp = motherpl_hp;
    //EQUIPPED ITEM
        switch(itemEquipped.itemtype){
            case INVITEM_MONEY:
                UPDATE_HUD_TILE(16,0,22);
                UPDATE_HUD_TILE(17,0,21);
                UPDATE_HUD_TILE(18,0,22);
            break;
	        case INVITEM_ARROW_NORMAL:
                UPDATE_HUD_TILE(16,0,6);
                UPDATE_HUD_TILE(17,0,5);
                UPDATE_HUD_TILE(18,0,17);
            break;
	        case INVITEM_ARROW_PERFO:
                UPDATE_HUD_TILE(16,0,6);
                UPDATE_HUD_TILE(17,0,5);
                UPDATE_HUD_TILE(18,0,18);
            break;
	        case INVITEM_ARROW_BASTARD:
                UPDATE_HUD_TILE(16,0,6);
                UPDATE_HUD_TILE(17,0,5);
                UPDATE_HUD_TILE(18,0,19);
            break;
	        case INVITEM_BOMB:
                UPDATE_HUD_TILE(16,0,6);
                UPDATE_HUD_TILE(17,0,5);
                UPDATE_HUD_TILE(18,0,20);
            break;
        }
        print_target = PRINT_WIN;
        if(itemEquipped.quantity < 10){ PRINT(16,1,"00%i", itemEquipped.quantity); }
        else if(itemEquipped.quantity < 100){ PRINT(16,1,"0%i", itemEquipped.quantity);}
        else {PRINT(16,1,"%i", itemEquipped.quantity);}
    //HP
        hud_motherpl_hp = motherpl_hp;
        for(idx_leftheart=5; idx_leftheart<15 ;idx_leftheart+=2){
            if(tmp_hp > 0){
                UPDATE_HUD_TILE(idx_leftheart,0,7);
                UPDATE_HUD_TILE(idx_leftheart,1,8);
                idx_rightheart++;
                UPDATE_HUD_TILE(idx_rightheart,0,9);
                UPDATE_HUD_TILE(idx_rightheart,1,10);
            }else{
                UPDATE_HUD_TILE(idx_leftheart,0,1);
                UPDATE_HUD_TILE(idx_leftheart,1,2);
                idx_rightheart++;
                UPDATE_HUD_TILE(idx_rightheart,0,3);
                UPDATE_HUD_TILE(idx_rightheart,1,4);
            }
            tmp_hp--;
            idx_rightheart++;
        }

}

void Log() BANKED{    
    /* hudpl.gbm aggiungi una linea sotto, 4 tiles height */
    switch(motherpl_state){
        case MOTHERPL_IDLE: PRINT(0, 2, "IDLE"); break;
        case MOTHERPL_JUMP: PRINT(0, 2, "JUMP"); break;
        case MOTHERPL_WALK: PRINT(0, 2, "WALK"); break;
        case MOTHERPL_HIT: PRINT(0, 2, " HIT"); break;
        case MOTHERPL_DEAD: PRINT(0, 2, "DEAD"); break;
        case MOTHERPL_CRAWL: PRINT(0, 2, "CRAW"); break;
        case MOTHERPL_CRAWL_SURF: PRINT(0, 2, "CRAS"); break;
        case MOTHERPL_DASH: PRINT(0, 2, "DASH"); break;
    }
    if(s_surf){
        PRINT(5, 2, "SURF%i",motherpl_surf_dx);
    }
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
    PRINT(16, 2, "!");
    PRINT(17, 2, "LOG");    
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
            if(current_state == StateHood && missions[3].current_step < 3u){ 
            //|| missions[3].mission_state == MISSION_STATE_STARTED)){
            }else{
                ChangeState(StateOverworld, s_motherpl);
            }
        }
        if(s_motherpl->x > (((UINT16)mapwidth) << 3) - 16u){
            s_motherpl->x = (((UINT16)mapwidth) << 3) - 16u;
            if(current_state == StateHood && missions[3].current_step < 3u){
            }else{
                ChangeState(StateOverworld, s_motherpl);
            }
        }  
        //VERTICAL
        if(s_motherpl->y > (((UINT16) mapheight) << 3)){
            s_motherpl->y = ((UINT16) mapheight) - 32u;
        }
        if(s_motherpl->y <= 0u){
            s_motherpl->y = 32u;
        }  
    //SCROLL TARGET Y
    if(scroll_target->y != (s_motherpl->y + 16u)){
        scroll_target->y = s_motherpl->y + 16u;
    }
    if(motherpl_state == MOTHERPL_BLOCKED ){return;}
    if(motherpl_blocked_cooldown > 0u){motherpl_blocked_cooldown--;return;}
    //camera fissa per certi stage
    UINT8 consider_margins = 0u;
    switch(current_state){
        case StateMine:
        case StateBlackiecave:
            scroll_target->x = s_motherpl->x + 8u;
            scroll_target->y = s_motherpl->y + 8u;
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
    if(KEY_PRESSED(J_RIGHT) || KEY_PRESSED(J_LEFT) || motherpl_state == MOTHERPL_DASH){
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

void spawn_npc(UINT8 type, UINT16 posx, UINT16 posy, NPCTYPE head, NPCTYPE body, MirroMode mirror, WHOSTALKING whos) BANKED{
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