#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"


void START(){ 
}

void UPDATE(){
    /*if(THIS->x < (UINT16) 10u << 3){
        THIS->x = (UINT16) 10u << 3;
        return;
    }
    if(THIS->x > (UINT16) 11u << 3){
        THIS->x = (UINT16) 11u << 3;
        return;
    }*/
    if(THIS->y < (UINT16) 4u << 3){
        THIS->y = (UINT16) 10u << 3;
        return;
    }
    if(THIS->y > (UINT16) 30u << 3){
        THIS->y = (UINT16) 30u << 3;
        return;
    }
    if(KEY_PRESSED(J_DOWN)){
        THIS->y++;
    }
    if(KEY_PRESSED(J_UP)){
        THIS->y--;
    }
}

void DESTROY(){
    
}