#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 diary_cursor_anim[] = {5, 0,0,0,1,1}; //The first number indicates the number of frames

void START(){
    SetSpriteAnim(THIS, diary_cursor_anim, 16u);
}

void UPDATE(){
    
}

void DESTROY(){

}