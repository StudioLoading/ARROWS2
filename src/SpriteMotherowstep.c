#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 anim_step[] = {1,0}; //The first number indicates the number of frames

void START(){
	THIS->lim_x = 1u;
	THIS->lim_y = 1u;
    SetSpriteAnim(THIS, anim_step, 16u);
	struct EnemyData* step_data = (struct EnemyData*) THIS->custom_data;
	step_data->hp = 80u;
}

void UPDATE(){
	struct EnemyData* step_data = (struct EnemyData*) THIS->custom_data;
	step_data->hp--;
	if(step_data->hp == 0){
		SpriteManagerRemoveSprite(THIS);
	}
}

void DESTROY(){

}