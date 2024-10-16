#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Palette.h"
#include "SpriteManager.h"
#include "Scroll.h"


const UINT8 a_tetrahour[] = {9, 1, 1, 2, 3, 4, 5, 5, 6, 6}; //The first number indicates the number of frames

void START() {
    THIS->lim_x = 300;
    THIS->lim_y = 0;
	SetSpriteAnim(THIS, a_tetrahour, 12u);
}

void UPDATE() {	
}

void DESTROY(){
	
}