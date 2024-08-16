#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

const UINT8 camerafocus_normal[] = {1, 0}; //The first number indicates the number of frames

extern UINT8 camera_ok;

void START() {
	THIS->lim_x = 255u;
	THIS->lim_y = 255u;
	SetSpriteAnim(THIS, camerafocus_normal, 8u);
    camera_ok = 0u;
}

void UPDATE() {
	
}

void DESTROY() {
}
