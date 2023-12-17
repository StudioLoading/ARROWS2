#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_owgator[] = {4, 0,0,1,0}; //The first number indicates the number of frames

void START() {
    THIS->lim_x = 6000u;
    THIS->lim_y = 6000u;
	SetSpriteAnim(THIS, a_owgator, 24u);
	struct PlatformInfo* gator_platform = (struct PlatformInfo*)THIS->custom_data;
	gator_platform->configured = 0;
	gator_platform->step = 0;
    gator_platform->vx = 0;
}

void UPDATE() {
	struct PlatformInfo* gator_platform = (struct PlatformInfo*)THIS->custom_data;
    if(gator_platform->configured == 0){
        gator_platform->distance--;
        if(gator_platform->distance == 0){gator_platform->configured = 1;}
        return;
    }
    gator_platform->distance = 200;
	UINT8 r = 0x1;//2;
	if (gator_platform->step < (gator_platform->distance/6) 
	|| gator_platform->step > (gator_platform->distance-(gator_platform->distance/6))){
		r=0x7;//8;
		//TURBINE
			if(gator_platform->step == 10){THIS->y -= gator_platform->vy;}
			if(gator_platform->step == (gator_platform->distance - 20)){THIS->y-=gator_platform->vy;}
            if(gator_platform->step == 0){THIS->y-=gator_platform->vy;}
            if(gator_platform->step 
			== (gator_platform->distance - (gator_platform->distance/20))){THIS->y-=gator_platform->vy;}
	}else if(gator_platform->step > ((gator_platform->distance>>1)-(gator_platform->distance/6) ) 
		&& gator_platform->step < ((gator_platform->distance>>1)+(gator_platform->distance/6) ) ){
		r=0x7;//8;		
		if(gator_platform->step == ((gator_platform->distance>>1) - 20)
            || gator_platform->step == ((gator_platform->distance>>1) - 10)
            || gator_platform->step == (gator_platform->distance>>1)
            || gator_platform->step == ((gator_platform->distance>>1) + 10)
        ){
			THIS->y+=gator_platform->vy;
		}
	}
	if ((gator_platform->step & r) == 0 && gator_platform->step != 0){
		if (gator_platform->step < (gator_platform->distance >> 1)){
            if(gator_platform->clockwise == 0){
			    gator_platform->vx = 1;
            }else{
                gator_platform->vx = -1;
            }
		}else{		
            if(gator_platform->clockwise == 0){
			    gator_platform->vx = -1;
            }else{
                gator_platform->vx = 1;
            }
		}
	}else{
		gator_platform->vx = 0;
    }
	//THIS->x += gator_platform->vx;
	//THIS->y += gator_platform->vy;
	THIS->x += gator_platform->vx;
    /*
    UINT8 coll = TranslateSprite(THIS, gator_platform->vx, 0);
    if(coll){
		SpriteManagerRemoveSprite(THIS);
	}
    */
	gator_platform->step += 1;
	if (gator_platform->step >= gator_platform->distance){
		gator_platform->step = 0;
		//THIS->x = gator_platform->initx;		
	}
}

void DESTROY() {
}
