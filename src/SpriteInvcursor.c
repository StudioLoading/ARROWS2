#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 invcur_arrow_anim[] = {2,0,1}; //The first number indicates the number of frames
const UINT8 a_invucrsor_end[] = {2,2,3}; //The first number indicates the number of frames

void START(){
	THIS->lim_x = 255u;
	THIS->lim_y = 255u;
	struct ItemSpawned* incursor_data = (struct ItemSpawned*)THIS->custom_data;
    incursor_data->configured = 0;
}

void UPDATE(){
	struct ItemSpawned* incursor_data = (struct ItemSpawned*)THIS->custom_data;
	switch(incursor_data->configured){
		case 1u:
			SetSpriteAnim(THIS, invcur_arrow_anim, 16u);
			incursor_data->configured = 3;
		break;
		case 2u:
			SetSpriteAnim(THIS, a_invucrsor_end, 8u);
			incursor_data->configured = 3;
		break;
	}

}

void DESTROY(){

}