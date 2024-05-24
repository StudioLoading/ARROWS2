#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 animright_leaf[] = {1, 0}; //The first number indicates the number of frames
const UINT8 animmid_leaf[] = {1, 1}; //The first number indicates the number of frames

void START() {
	THIS->lim_x = 1u;
	THIS->lim_y = 8u;
	SetSpriteAnim(THIS, animmid_leaf, 8u);
	struct PlatformInfo* data_platform = (struct PlatformInfo*)THIS->custom_data;
	data_platform->configured = 0;
	data_platform->distance = 200;
	data_platform->step = 0;
}

void UPDATE() {
	struct PlatformInfo* data_platform = (struct PlatformInfo*)THIS->custom_data;
	UINT8 r = 0x1;//2;
	if (data_platform->step < (data_platform->distance/6) 
	|| data_platform->step > (data_platform->distance-(data_platform->distance/6))){
		r=0x7;//8;
		//TURBINE
			//if(data_platform->step == 10){THIS->y--;}
			//if(data_platform->step == (data_platform->distance - 20)){THIS->y--;}
		if(data_platform->step == (data_platform->distance/7)){THIS->y--;}
		if(data_platform->step 
			== (data_platform->distance - (data_platform->distance/20))){THIS->y--;}
		SetSpriteAnim(THIS, animright_leaf, 8u);
	}else if(data_platform->step > ((data_platform->distance>>1)-(data_platform->distance/6) ) 
		&& data_platform->step < ((data_platform->distance>>1)+(data_platform->distance/6) ) ){
		r=0x7;//8;		
		SetSpriteAnim(THIS, animmid_leaf, 8u);
		switch(data_platform->step){
			case 80:
			case 90:
			case 95:
			case 100:
			case 105:
			case 110:
				THIS->y++;
			break;
		}
	}
	if ((data_platform->step & r) == 0 && data_platform->step != 0){
		data_platform->vy = 1;
		if (data_platform->step < (data_platform->distance >> 1)){
			data_platform->vx = 1;
			THIS->mirror = NO_MIRROR;
			//TURBINE
				//data_platform->vy = 1;
		}else{		
			data_platform->vx = -1;
			THIS->mirror = V_MIRROR;
			//TURBINE
				//data_platform->vy = -1;
		}
	}else{
		data_platform->vx = 0;
		data_platform->vy = 0;
	}
	//TURBINE
		//data_platform->vy = 0;
	//THIS->x += data_platform->vx;
	//THIS->y += data_platform->vy;
	UINT8 coll = TranslateSprite(THIS, data_platform->vx, data_platform->vy);
	if(coll){
		SpriteManagerRemoveSprite(THIS);
	}
	data_platform->step += 1;
	if (data_platform->step >= data_platform->distance){
		data_platform->step = 0;
	}
}

void DESTROY() {
}
