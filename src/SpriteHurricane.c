#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 anim_hurr[] = {1, 0}; //The first number indicates the number of frames

void START() {
	THIS->lim_x = 1u;
	THIS->lim_y = 8u;
	SetSpriteAnim(THIS, anim_hurr, 8u);
	struct PlatformInfo* data_platform = (struct PlatformInfo*)THIS->custom_data;
	data_platform->type = 0;
	data_platform->distance = 200;
	data_platform->step = 0;
    data_platform->vx = 0;
}

void UPDATE() {
	struct PlatformInfo* data_platform = (struct PlatformInfo*)THIS->custom_data;
	UINT8 r = 0x1;//2;
	if (data_platform->step < (data_platform->distance/6) 
	|| data_platform->step > (data_platform->distance-(data_platform->distance/6))){
		r=0x7;//8;
		//TURBINE
			if(data_platform->step == 10){THIS->y--;}
			if(data_platform->step == (data_platform->distance - 20)){THIS->y--;}
            if(data_platform->step == 0){THIS->y--;}
            if(data_platform->step 
			== (data_platform->distance - (data_platform->distance/20))){THIS->y--;}
	}else if(data_platform->step > ((data_platform->distance>>1)-(data_platform->distance/6) ) 
		&& data_platform->step < ((data_platform->distance>>1)+(data_platform->distance/6) ) ){
		r=0x7;//8;		
		if(data_platform->step == ((data_platform->distance>>1) - 20)
            || data_platform->step == ((data_platform->distance>>1) - 10)
            || data_platform->step == (data_platform->distance>>1)
            || data_platform->step == ((data_platform->distance>>1) + 10)
        ){
			THIS->y++;
		}
	}
	if ((data_platform->step & r) == 0 && data_platform->step != 0){
		if (data_platform->step < (data_platform->distance >> 1)){
			data_platform->vx = 1;
		}else{		
			data_platform->vx = -1;
		}
	}else{
		data_platform->vx = 0;
    }
	//THIS->x += data_platform->vx;
	//THIS->y += data_platform->vy;
	UINT8 coll = TranslateSprite(THIS, data_platform->vx, 0);
	if(coll){
		SpriteManagerRemoveSprite(THIS);
	}
	data_platform->step += 1;
	if (data_platform->step >= data_platform->distance){
		data_platform->step = 0;
		//THIS->x = data_platform->initx;		
	}
}

void DESTROY() {
}
