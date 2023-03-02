#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "BankManager.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Fade.h"
#include "Music.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "sgb_palette.h"
#include "Dialogs.h"

#define CAMERA_DELTA_RIGHT 40
#define CAMERA_DELTA_LEFT 32

IMPORT_MAP(exzoomap0);

extern struct InvItem itemEquipped;
extern struct MISSION find_blackie;
extern struct MISSION engage_smith;
extern struct MISSION missions[4];
extern INT8 motherpl_hp;
extern INT8 motherpl_ups;
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
UINT8 mine_powderspawned = 3u;

UINT8 npc_spawned_zone = 0u;
Sprite* s_motherpl = 0;
UINT8 init_enemy = 0u;
INT8 hud_motherpl_hp = 0;
INT8 hud_motherpl_ups = 0;
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
UINT8 test_countdown = 255u;
UINT8 item_spawned_cooldown = 255u;

void UpdateHUD() BANKED;
void Log() BANKED;
void update_camera_position() BANKED;
void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;
void spawn_npc(UINT8 type, UINT16 posx, UINT16 posy, NPCTYPE head, NPCTYPE body, MirroMode mirror, WHOSTALKING whos) BANKED;
void spawn_item(INVITEMTYPE itemtype, UINT16 x, UINT16 y) BANKED;

extern void ChangeStateThroughBetween(UINT8 new_state) BANKED;

void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED{    
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
                motherpl_mirror = s_mother->mirror;
            break;        
        }
        HIDE_WIN;
        SetWindowY(160);
    }
    //SGB PALETTE
        if(sgb_check()){
            switch(new_state){
                case StateOverworld:
                    reset_sgb_palette_statusbar();
                break;
                case StateInventory:
                    reset_sgb_palette_statusbar();
                break;
                case StateExzoo:
                    set_sgb_palette01_ZOO();
                    set_sgb_palette_statusbar();
                break;
                case StateSmith:
                    set_sgb_palette01_WOLF();
                    reset_sgb_palette_statusbar();
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
                    set_sgb_palette01_TREES();
                    set_sgb_palette_statusbar();
                break;
            }
        }
    previous_state = current_state;
	if(new_state != StateDialog && current_state != StateDialog){
	    ChangeStateThroughBetween(new_state);
    }else{
        SetState(new_state);
    }
}

void UpdateHUD() BANKED{
    UINT8 idx_leftheart = 6;
    UINT8 idx_rightheart = 6;
    INT8 tmp_hp = motherpl_hp;
    //EQUIPPED ITEM
        switch(itemEquipped.itemtype){
            case INVITEM_MONEY:
                UPDATE_HUD_TILE(16,1,0);
                UPDATE_HUD_TILE(17,1,21);
                UPDATE_HUD_TILE(18,1,0);
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
        for(idx_leftheart=6; idx_leftheart<14 ;idx_leftheart+=2){
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
    //UPS
        hud_motherpl_ups = motherpl_ups;
        print_target = PRINT_WIN;
        if(hud_motherpl_ups < 10){PRINT(2,2,"0%i", hud_motherpl_ups);}
        else{PRINT(2,2,"%i", hud_motherpl_ups);}
}

void Log() BANKED{    
    switch(motherpl_state){
        case MOTHERPL_IDLE: PRINT(0, 3, "IDLE"); break;
        case MOTHERPL_JUMP: PRINT(0, 3, "JUMP"); break;
        case MOTHERPL_WALK: PRINT(0, 3, "WALK"); break;
        case MOTHERPL_HIT: PRINT(0, 3, " HIT"); break;
        case MOTHERPL_DEAD: PRINT(0, 3, "DEAD"); break;
        case MOTHERPL_CRAWL: PRINT(0, 3, "CRAW"); break;
        case MOTHERPL_DASH: PRINT(0, 3, "DASH"); break;
    }
    if(s_surf){
        PRINT(5, 3, "SURF%i",motherpl_surf_dx);
    }
    //PRINT(10, 3, "AR:%u%u", arrows_onscreen, 5u);
    PRINT(16, 3, "!");
    PRINT(17, 3, "LOG");
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
            scroll_target->y = s_motherpl->y - 13u;
        }else{
            scroll_target->y = s_motherpl->y + 14u;
        }
    }
}

void update_camera_position() BANKED{
    //LIMITS
        //HORIZONTAL
        if(s_motherpl->x < (UINT16)8u){
            s_motherpl->x = 8u;
            ChangeState(StateOverworld, s_motherpl);
        }
        if(s_motherpl->x > (((UINT16)mapwidth) << 3) - 16u){
            s_motherpl->x = (((UINT16)mapwidth) << 3) - 16u;
            ChangeState(StateOverworld, s_motherpl);
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
    test_countdown = 255u;
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

void START(){}

void UPDATE(){}