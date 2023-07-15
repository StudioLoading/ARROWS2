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
#include "Sound.h"
#include "Music.h"

#include "TilesAnimations0.h"
#include "custom_datas.h"
#include "sgb_palette.h"

IMPORT_MAP(border2);
IMPORT_TILES(titlescreentiles);
IMPORT_TILES(fontbw);
IMPORT_MAP(titlescreenmap);
//DECLARE_MUSIC(bgm_titlescreen);
DECLARE_MUSIC(sloopy);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern WHOSTALKING whostalking;
extern UINT8 previous_state;

const UINT8 collision_tiles_titlescreen[] = {1,0};
UINT8 titlescreen_step = 0u;
INT8 titlescreen_wait_time = 0;
UINT8 activate_titlescreen_wait_time = 0u;
INT8 counter_anim = 0u;

extern void Anim_Titlescreen_0() BANKED;
extern void Anim_Titlescreen_1() BANKED;
extern void Anim_Titlescreen_2() BANKED;
extern void Anim_Titlescreen_3() BANKED;
extern void ChangeStateThroughBetween(UINT8 new_state, UINT8 previous_state) BANKED;
extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;

void START() {
    LOAD_SGB_BORDER(border2);
	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	//NR50_REG = 0x44; //Max volume 0x77

	if(sgb_check()){
		set_sgb_palette01_TITLESCREEN();
	}

	scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 80u, (UINT16) 240u);
	InitScroll(BANK(titlescreenmap), &titlescreenmap, collision_tiles_titlescreen, 0);
	//scroll_target->y = (UINT16) 200u;
	SHOW_BKG;
	INIT_FONT(fontbw, PRINT_BKG);
	
	titlescreen_step = 0u;
	
	PlayMusic(sloopy, 1);
	//PlayMusic(bgm_titlescreen, 0);
	titlescreen_wait_time = 0;
}

void UPDATE() {
	if(titlescreen_step < 3u){
		counter_anim++;
		if(counter_anim < 20u){Anim_Titlescreen_0();}
		else if(counter_anim < 40u){Anim_Titlescreen_1();}
		else if(counter_anim < 60u){Anim_Titlescreen_2();}
		else if(counter_anim < 80u){Anim_Titlescreen_3();}
		else{ counter_anim = 0;}		
		if(KEY_TICKED(J_START)){
			StopMusic;
			titlescreen_wait_time = 0;
			PRINT(5, 9, " LET'S GO! ");
			titlescreen_step = 3u;
		}
	}

	switch(titlescreen_step){
		case 0u:
			if(scroll_target->y > (UINT16) 30u){
				scroll_target->y--;
			}else{
				scroll_target->y = (UINT16) 30u;
				if(sgb_check()){
					set_sgb_palette_title();
				}
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
			if(titlescreen_wait_time == 20){
				my_play_fx(CHANNEL_1, 60, 0x36, 0x9f, 0xf6, 0x91, 0x86);//SFX_START
			}
			if(titlescreen_wait_time > 60){
				reset_sgb_palette_title();
				ChangeStateThroughBetween(StatePassword, StateTitlescreen);
			}
		break;
	}
		
}
