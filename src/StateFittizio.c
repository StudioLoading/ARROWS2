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
#define CAMERA_DELTA_LEFT 24

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

Sprite* s_motherpl = 0;
UINT8 init_enemy = 0u;
INT8 hud_motherpl_hp = 0;
INT8 hud_motherpl_ups = 0;
struct InvItem* itemEquipped = &itemMoney;
UINT8 camera_ok = 0u;
UINT16 motherpl_pos_x = 0u;
UINT16 motherpl_pos_y = 0u;
UINT16 motherow_pos_x = 0u;
UINT16 motherow_pos_y = 0u;
struct EtoReload e_to_reload[3];
UINT8 enemy_counter = 0u;

void UpdateHUD() BANKED;
void Log() BANKED;
void update_camera_position() BANKED;
void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;
void spawnItem(UINT16 x, UINT16 y, UINT8 spawner_type) BANKED;

extern void ChangeStateThroughBetween(UINT8 new_state) BANKED;

void spawnItem(UINT16 x, UINT16 y, UINT8 spawner_type) BANKED{
    //SPAWN ITEM
    INVITEMTYPE itemtype = INVITEM_MONEY;
    if(enemy_random_30_100 < 40){
        itemtype = INVITEM_CROSSBOW;
    }else if (enemy_random_30_100 < 50){
        itemtype = INVITEM_METAL;
    }else if (enemy_random_30_100 < 60){
        itemtype = INVITEM_HEART;
    }else if (enemy_random_30_100 < 70){
        itemtype = INVITEM_WOOD;
    }
    UINT16 quantity = 1u;        
    Sprite* reward = SpriteManagerAdd(SpriteItemspawned, x + 4u, y - 8u);
    struct ItemSpawned* reward_data = (struct ItemSpawned*) reward->custom_data;
    reward_data->itemtype = itemtype;
    reward_data->quantity = quantity;
    reward_data->equippable = 1u;
    reward_data->configured = 1u;
}

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
	//HIDE_WIN;
    switch(s_mother->type){
        case SpriteMotherow:
            motherow_pos_x = s_mother->x;
            motherow_pos_y = s_mother->y;
        break;
        case SpriteMotherpl:
            motherpl_pos_x = s_mother->x;
            motherpl_pos_y = s_mother->y;
        break;
    }
	//SetWindowY(160);
	ChangeStateThroughBetween(new_state);
}

void UpdateHUD() BANKED{
    UINT8 idx_leftheart = 6;
    UINT8 idx_rightheart = 6;
    INT8 tmp_hp = motherpl_hp;
    //EQUIPPED ITEM
        switch(itemEquipped->itemtype){
            case INVITEM_MONEY:
                UPDATE_HUD_TILE(16,0,0);
                UPDATE_HUD_TILE(17,0,21);
                UPDATE_HUD_TILE(18,0,0);
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
        if(itemEquipped->quantity < 10){ PRINT(16,1,"00%i", itemEquipped->quantity); }
        else if(itemEquipped->quantity < 100){ PRINT(16,1,"0%i", itemEquipped->quantity);}
        else {PRINT(16,1,"%i", itemEquipped->quantity);}
    //HP
        hud_motherpl_hp = motherpl_hp;
        for(idx_leftheart=6; idx_leftheart<14 ;idx_leftheart+=2){
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
    //UPS
        hud_motherpl_ups = motherpl_ups;
        print_target = PRINT_WIN;
        if(hud_motherpl_ups < 10){PRINT(2,1,"0%i", hud_motherpl_ups);}
        else{PRINT(2,1,"%i", hud_motherpl_ups);}
}

void Log() BANKED{    
    if(print_target != PRINT_WIN){
        print_target = PRINT_WIN;
    }
    switch(motherpl_state){
        case MOTHERPL_IDLE: PRINT(0, 2, "IDLE"); break;
        case MOTHERPL_JUMP: PRINT(0, 2, "JUMP"); break;
        case MOTHERPL_WALK: PRINT(0, 2, "WALK"); break;
        case MOTHERPL_HIT: PRINT(0, 2, " HIT"); break;
        case MOTHERPL_DEAD: PRINT(0, 2, "DEAD"); break;
        case MOTHERPL_CRAWL: PRINT(0, 2, "CRAW"); break;
        case MOTHERPL_DASH: PRINT(0, 2, "DASH"); break;
    }
    if(s_surf){
        PRINT(5, 2, "SURF%i",motherpl_surf_dx);
    }else{
        PRINT(5, 2, "     ");
    }
    //PRINT(10, 2, "AR:%u%u", arrows_onscreen, 5u);
    PRINT(16, 2, "!");
    PRINT(17, 2, "LOG");
}

void update_camera_position() BANKED{
    if(scroll_target->y != (s_motherpl->y + 16u)){
        scroll_target->y = s_motherpl->y + 16u;
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
    }else{
        if(KEY_PRESSED(J_DOWN) || motherpl_state == MOTHERPL_PICKUP){camera_ok = 0u;}
        if(motherpl_state == MOTHERPL_BLOCKED ){return;}
        if(motherpl_blocked_cooldown > 0u){motherpl_blocked_cooldown--;return;}
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
                            scroll_target->x+=3;
                        }else if (!KEY_PRESSED(J_LEFT) 
                            && !KEY_PRESSED(J_DOWN)){
                            camera_ok = 1u;
                        }
                    break;
                    case V_MIRROR:
                        if(scroll_target->x > (s_motherpl->x - CAMERA_DELTA_LEFT)){
                            scroll_target->x-=3;
                        }else if (!KEY_PRESSED(J_RIGHT) 
                            && !KEY_PRESSED(J_DOWN)){
                            camera_ok = 1u;
                        }
                    break;
                }
            }
        }
    }
}

void START(){}

void UPDATE(){}