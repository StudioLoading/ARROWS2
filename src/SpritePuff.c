#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "SpriteManager.h"
#include "Scroll.h"

#include "custom_datas.h"


const UINT8 anim_puff[] = {5, 0, 1, 2, 3, 3}; //The first number indicates the number of frames

void START() {
	SetSpriteAnim(THIS, anim_puff, 12u);
}

void UPDATE() {
	if(THIS->anim_frame == 4){
		SpriteManagerRemoveSprite(THIS);
	}	
}

void DESTROY(){
	
}