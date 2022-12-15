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

Sprite* s_motherpl = 0;
UINT8 init_enemy = 0u;
INT8 hud_motherpl_hp = 0;
INT8 hud_motherpl_ups = 0;


void UpdateHUD() BANKED;

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

void START(){}

void UPDATE(){}