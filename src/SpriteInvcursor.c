#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 invcur_square_anim[] = {2,0,1}; //The first number indicates the number of frames
const UINT8 invcur_arrow_anim[] = {2,2,3}; //The first number indicates the number of frames

void START(){
	THIS->lim_x = 255u;
	THIS->lim_y = 255u;
    SetSpriteAnim(THIS, invcur_square_anim, 32u);
	struct InvcursorInfo* cur_data = (struct InvcursorInfo*) THIS->custom_data;
	cur_data->switch_animation = 0u;
	cur_data->square_or_arrow = 0u;
}

void UPDATE(){
	struct InvcursorInfo* cur_data = (struct InvcursorInfo*) THIS->custom_data;
	if(cur_data->switch_animation){
		switch(cur_data->square_or_arrow){
			case 0u://to arrow
				SetSpriteAnim(THIS, invcur_arrow_anim, 24u);
				cur_data->square_or_arrow = 1u;
			break;
			case 1u:
				SetSpriteAnim(THIS, invcur_square_anim, 24u);
				cur_data->square_or_arrow = 0u;
			break;
		}
		cur_data->switch_animation = 0u;
	}
}

void DESTROY(){

}