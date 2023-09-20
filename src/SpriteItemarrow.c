#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_itemarrow_normal[] = {2,0,1}; //The first number indicates the number of frames
const UINT8 a_itemarrow_perfo[] = {2,0,2}; //The first number indicates the number of frames
const UINT8 a_itemarrow_pearced[] = {2,0,3}; //The first number indicates the number of frames

void START(){
	THIS->lim_x = 255u;
	THIS->lim_y = 255u;
	struct ArrowData* arrow_data = (struct ArrowData*) THIS->custom_data;
	arrow_data->configured = 0u;
	arrow_data->arrow_type = ARROW_UNASSIGNED;    
}

void UPDATE(){
	struct ArrowData* arrow_data = (struct ArrowData*) THIS->custom_data;
	if(arrow_data->configured == 1){
		switch(arrow_data->arrow_type){
			case ARROW_NORMAL:
				SetSpriteAnim(THIS, a_itemarrow_normal, 16u);
			break;
			case ARROW_PERF:
				SetSpriteAnim(THIS, a_itemarrow_perfo, 16u);
			break;
			case ARROW_BASTARD:
				SetSpriteAnim(THIS, a_itemarrow_pearced, 16u);
			break;
		}
		arrow_data->configured = 2;
	}
}

void DESTROY(){

}