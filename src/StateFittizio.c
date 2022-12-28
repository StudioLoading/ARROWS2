#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Music.h"

#include "custom_datas.h"
#include "Dialogs.h"

#define CAMERA_DELTA_RIGHT 32
#define CAMERA_DELTA_LEFT 24

extern INT8 motherpl_hp;
extern INT8 motherpl_ups;
extern INT8 motherpl_surf_dx;
extern MOTHERPL_STATE motherpl_state;
extern Sprite* s_surf;
extern struct InvItem itemMoney;
extern UINT8 arrows_onscreen;

Sprite* s_motherpl = 0;
UINT8 init_enemy = 0u;
INT8 hud_motherpl_hp = 0;
INT8 hud_motherpl_ups = 0;
struct InvItem* itemEquipped = &itemMoney;
UINT8 camera_ok = 0u;


void UpdateHUD() BANKED;
void Log() BANKED;
void update_camera_position() BANKED;

void UpdateHUD() BANKED{
    UINT8 idx_leftheart = 6;
    UINT8 idx_rightheart = 6;
    INT8 tmp_hp = motherpl_hp;
    //EQUIPPED ITEM
        switch(itemEquipped->itemtype){
            case INVITEM_MONEY:
                UPDATE_HUD_TILE(16,0,21);
                UPDATE_HUD_TILE(16,1,22);
            break;
	        case INVITEM_ARROW_NORMAL:
                UPDATE_HUD_TILE(16,0,5);
                UPDATE_HUD_TILE(16,1,6);
            break;
	        case INVITEM_ARROW_PERFO:
                UPDATE_HUD_TILE(16,0,17);
                UPDATE_HUD_TILE(16,1,18);
            break;
	        case INVITEM_ARROW_BASTARD:
                UPDATE_HUD_TILE(16,0,23);
                UPDATE_HUD_TILE(16,1,24);
            break;
	        case INVITEM_BOMB:
                UPDATE_HUD_TILE(16,0,19);
                UPDATE_HUD_TILE(16,1,20);
            break;
        }
        print_target = PRINT_WIN;
        if(itemEquipped->quantity < 10){ PRINT(17,1,"00%i", itemEquipped->quantity); }
        else if(itemEquipped->quantity < 100){ PRINT(17,1,"0%i", itemEquipped->quantity);}
        else {PRINT(17,1,"%i", itemEquipped->quantity);}
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
        if(hud_motherpl_ups < 10){PRINT(2,0,"0%i", hud_motherpl_ups);}
        else{PRINT(2,0,"%i", hud_motherpl_ups);}
}

void Log() BANKED{    
    if(print_target != PRINT_WIN){
        print_target = PRINT_WIN;
    }
    switch(motherpl_state){
        case MOTHERPL_IDLE:
            PRINT(0, 2, "IDLE");
        break;
        case MOTHERPL_JUMP:
            PRINT(0, 2, "JUMP");
        break;
        case MOTHERPL_WALK:
            PRINT(0, 2, "WALK");
        break;
        case MOTHERPL_HIT:
            PRINT(0, 2, " HIT");
        break;
        case MOTHERPL_DEAD:
            PRINT(0, 2, "DEAD");
        break;
        case MOTHERPL_CRAWL:
            PRINT(0, 2, "CRAW");
        break;
    }
    if(s_surf){
        PRINT(5, 2, "SURF%i",motherpl_surf_dx);
    }else{
        PRINT(5, 2, "     ");
    }
    PRINT(10, 2, "AR:%u%u", arrows_onscreen, 5u);
    PRINT(16, 2, "!");
    PRINT(17, 2, "LOG");
}

void update_camera_position() BANKED{
    scroll_target->y = s_motherpl->y + 16u;
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
        if(KEY_PRESSED(J_RIGHT) || KEY_PRESSED(J_LEFT)){
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
                        }else{camera_ok = 1u;}
                    break;
                    case V_MIRROR:
                        if(scroll_target->x > (s_motherpl->x - CAMERA_DELTA_LEFT)){
                            scroll_target->x-=2;
                        }else{camera_ok = 1u;}
                    break;
                }
            }
        }
    }
}

void START(){}

void UPDATE(){}