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

const UINT8 a_diary[] = {2, 0, 1}; //The first number indicates the number of frames

void START() {
	THIS->lim_x = 650u;
	THIS->lim_y = 650u;
	struct EnemyData* diary_data = (struct EnemyData*) THIS->custom_data;
    diary_data->hp = 100;
	SetSpriteAnim(THIS, a_diary, 24u);
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE() {
	struct EnemyData* diary_data = (struct EnemyData*) THIS->custom_data;
    diary_data->hp--;
	if(scroll_target != 0 &&
		(scroll_target->x != THIS->x || scroll_target->y != THIS->y)){
		THIS->x = scroll_target->x;
		THIS->y = scroll_target->y;
	}
	if(diary_data->hp < 0){
		SpriteManagerRemoveSprite(THIS);
	}	
}

void DESTROY(){
	UINT8 delta = THIS->x - scroll_target->x;
	THIS->x++;
}