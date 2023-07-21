#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_owsign[] = {4, 1,2,3,0}; //The first number indicates the number of frames

void START() {
	THIS->lim_x = 250u;
	THIS->lim_y = 250u;
	SetSpriteAnim(THIS, a_owsign, 5u);
}

void UPDATE() {
}

void DESTROY() {
}
