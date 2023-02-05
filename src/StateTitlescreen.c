#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Keys.h"
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
UINT8 titlescreen_step = 0u;
INT8 titlescreen_wait_time = 0;
UINT8 activate_titlescreen_wait_time = 0u;


void START() {
    LOAD_SGB_BORDER(border2);
	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	//NR50_REG = 0x44; //Max volume 0x77
	//PlayMusic(bgm_credits, 0);

	scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 80u, (UINT16) 240u);
	InitScroll(BANK(titlescreenmap), &titlescreenmap, collision_tiles_titlescreen, 0);
	//scroll_target->y = (UINT16) 200u;
	SHOW_BKG;
	INIT_FONT(fontbw, PRINT_BKG);
	
	titlescreen_step = 0u;
	titlescreen_wait_time = 0;
}

void UPDATE() {
	switch(titlescreen_step){
		case 0u:
			if(scroll_target->y > (UINT16) 30u){
				scroll_target->y--;
			}else{
				scroll_target->y = (UINT16) 30u;
				titlescreen_step = 1u;
			}
		break;
		case 1u:
			titlescreen_wait_time++;
			if(titlescreen_wait_time == 100u){
				titlescreen_wait_time = 0;
				titlescreen_step = 2u;
			}
		break;
		case 2u:
			if(KEY_TICKED(J_START)){
				StopMusic;
				titlescreen_wait_time = 0;
				PRINT(5, 9, " LET'S GO! ");
				titlescreen_step = 3u;
			}
			if(KEY_TICKED(J_SELECT)){
				StopMusic;
				SetState(current_state);
			}
			switch(titlescreen_wait_time){
				case 1:
					PRINT(5, 9, "PUSH START");
				break;
				case 30:
					PRINT(5, 9, "          ");
				break;
				case 60:
					titlescreen_wait_time = 0;
				break;
			}
			titlescreen_wait_time++;
		break;
		case 3u:
			titlescreen_wait_time++;
			if(titlescreen_wait_time > 60){
				SetState(StateOverworld);
			}
		break;
	}
		
}