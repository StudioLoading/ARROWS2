#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Sound.h"

#include "custom_datas.h"

INT8 diary_hp = 0;

const UINT8 a_diary[] = {2, 0, 1}; //The first number indicates the number of frames

void START() {
    diary_hp = 100;
	SetSpriteAnim(THIS, a_diary, 24u);
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
    diary_hp--;
	if(scroll_target == 0 || scroll_target->x != THIS->x || scroll_target->y != THIS->y){
		THIS->x = scroll_target->x;
		THIS->y = scroll_target->y;
	}
	if(diary_hp < 0){
		SpriteManagerRemoveSprite(THIS);
	}	
}

void DESTROY(){
	
}