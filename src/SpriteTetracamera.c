#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern void refresh_bkg_tiles() BANKED;

void START(){ 
}

void UPDATE(){
    if(THIS->y < (UINT16) 7u << 3){
        THIS->y = (UINT16) 7u << 3;
        return;
    }

    if(THIS->y > (UINT16) 40u << 3){
        THIS->y = (UINT16) 40u << 3;
        return;
    }
    if(KEY_PRESSED(J_DOWN)){
        THIS->y+=4;
        if(THIS->y < ((UINT16) 25u << 3)){
            refresh_bkg_tiles();
        }
    }
    if(KEY_PRESSED(J_UP)){
        THIS->y-=4;
        if(THIS->y < ((UINT16) 25u << 3)){
            refresh_bkg_tiles();
        }
    }
}

void DESTROY(){
    
}