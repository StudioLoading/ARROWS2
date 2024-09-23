#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"


const UINT8 invheart_empty_anim[] = {1,0}; //The first number indicates the number of frames
const UINT8 invheart_full_anim[] = {1,1}; //The first number indicates the number of frames

void START(){
	THIS->lim_x = 255u;
	THIS->lim_y = 255u;
	struct ItemSpawned* invheart_data = (struct ItemSpawned*)THIS->custom_data;
    invheart_data->configured = 0;
}

void UPDATE(){
	struct ItemSpawned* invheart_data = (struct ItemSpawned*)THIS->custom_data;
	switch(invheart_data->configured){
		case 1u:
			SetSpriteAnim(THIS, invheart_empty_anim, 1u);
			invheart_data->configured = 3;
		break;
		case 2u:
			SetSpriteAnim(THIS, invheart_full_anim, 1u);
			invheart_data->configured = 3;
		break;
	}
}

void DESTROY(){
}