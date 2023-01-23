#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Fade.h"
#include "Music.h"

#include "TilesAnimations0.h"
#include "custom_datas.h"
#include "sgb_palette.h"

IMPORT_MAP(border2);
IMPORT_TILES(titlescreentiles);
IMPORT_TILES(fontbw);
IMPORT_MAP(titlescreenmap);
DECLARE_MUSIC(bgm_credits);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 collision_tiles_titlescreen[] = {1,0};
UINT8 titlescreen_counter;
UINT8 titlescreen_step = 0u;
UINT8 titlescreen_wait_time;


void START() {
    LOAD_SGB_BORDER(border2);
	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	//NR50_REG = 0x44; //Max volume 0x77
	//PlayMusic(bgm_credits, 0);

	InitScroll(BANK(titlescreenmap), &titlescreenmap, collision_tiles_titlescreen, 0);
	SHOW_BKG;
}

void UPDATE() {
	if(KEY_TICKED(J_START)){
		StopMusic;
		SetState(StateExzoo);
		return;
	}else if(KEY_TICKED(J_FIRE) || KEY_TICKED(J_JUMP)){
		titlescreen_wait_time = 0u;
		titlescreen_step += 1u;
		if(titlescreen_step == 5u){
			StopMusic;
			//SetState(StateTitlescreen);
			return;
		}else{
			StopMusic;			
			SetState(StateExzoo);//StateOverworld
		}
	}
		
}
