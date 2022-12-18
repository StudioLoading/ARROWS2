#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "ZGBMain.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Music.h"

#include "custom_datas.h"
#include "Dialogs.h"

extern INT8 motherpl_hp;
extern INT8 motherpl_ups;
extern INT8 motherpl_surf_dx;
extern MOTHERPL_STATE motherpl_state;
extern Sprite* s_surf;

Sprite* s_motherpl = 0;
UINT8 init_enemy = 0u;
INT8 hud_motherpl_hp = 0;
INT8 hud_motherpl_ups = 0;


void UpdateHUD() BANKED;
void Log() BANKED;

void UpdateHUD() BANKED{
    UINT8 idx_leftheart = 6;
    UINT8 idx_rightheart = 6;
    INT8 tmp_hp = motherpl_hp;
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
    PRINT(3,1,"%i", hud_motherpl_ups);
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
    }
    if(s_surf){
        PRINT(5, 2, "SURF%i",motherpl_surf_dx);
    }else{
        PRINT(5, 2, "     ");
    }
}

void START(){}

void UPDATE(){}