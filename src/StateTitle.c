#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "Music.h"
#include "SGB.h"
#include "SpriteManager.h"
#include "string.h"
#include "Sound.h"
#include "Print.h"

#include "TAnim0.h"
#include "custom_datas.h"
#include "sgb_palette.h"

IMPORT_TILES(titlescreentiles);
IMPORT_TILES(fontbw);
IMPORT_MAP(titlescreenmap);
//DECLARE_MUSIC(bgm_titlescreen);
DECLARE_MUSIC(sloopy);

extern WHOSTALKING whostalking;
extern INT8 sfx_cooldown;
extern UINT8 generic_counter;
extern UINT8 generic_counter2;
extern uint8_t sgb_running;

const UINT8 collision_tiles_titlescreen[] = {1,0};
UINT8 titlescreen_step = 0u;
INT8 titlescreen_wait_time = 0;
UINT8 activate_titlescreen_wait_time = 0u;
INT8 counter_anim = 0u;
UINT8 cursor_spawned = 0u;
UINT8 wait_titlescreen = 30u;
Sprite* s_cursor = 0;
extern UINT8 cursor_moving;

extern void Anim_Titlescreen_0() BANKED;
extern void Anim_Titlescreen_1() BANKED;
extern void Anim_Titlescreen_2() BANKED;
extern void Anim_Titlescreen_3() BANKED;
extern void ChangeStateThroughBetween(UINT8 new_state) BANKED;
extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;

void go_to_password(UINT8 my_next_state) BANKED;

void START() {
	//SGB
		if(sgb_running){
			set_sgb_palette01_TITLESCREEN();
		}
	//SPRITES & SCROLL_TARGET
		scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 80u, (UINT16) 240u);
		InitScroll(BANK(titlescreenmap), &titlescreenmap, collision_tiles_titlescreen, 0);

	SHOW_BKG;
	INIT_FONT(fontbw, PRINT_BKG);
	
	titlescreen_step = 0u;
	generic_counter = 0;
	generic_counter2 = 0;
	//PlayMusic(sloopy, 1);
	//PlayMusic(bgm_titlescreen, 0);
	titlescreen_wait_time = 0;
	cursor_spawned = 0u;
	wait_titlescreen = 30u;
}

void UPDATE() {
    if(sfx_cooldown > 0){sfx_cooldown--;}
    generic_counter++;
    if(generic_counter == 0u && titlescreen_step == 2u){
        generic_counter2++;
        if(generic_counter2 == 4u){
            go_to_password(StateCredit);
        }
    }
	counter_anim++;
	if(counter_anim < 20u){Anim_Titlescreen_0();}
	else if(counter_anim < 40u){Anim_Titlescreen_1();}
	else if(counter_anim < 60u){Anim_Titlescreen_2();}
	else if(counter_anim < 80u){Anim_Titlescreen_3();}
	else{ counter_anim = 0;}		
	switch(titlescreen_step){
		case 0u:
			if(scroll_target->y > (UINT16) 30u){
				scroll_target->y--;
			}else{
				scroll_target->y = (UINT16) 30u;
				if(sgb_running){
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
			titlescreen_wait_time++;
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
		break;
		case 3u:
			titlescreen_wait_time++;
			if(titlescreen_wait_time == 10){
				my_play_fx(CHANNEL_1, 10, 0x36, 0x9f, 0xf6, 0x91, 0x86);//SFX_START
			}
			if(titlescreen_wait_time > 40){
				titlescreen_wait_time = 0;
				titlescreen_step = 4u;
			}
		break;
		case 4u:
			scroll_target->x += 2u;
			if(scroll_target->x >= ((UINT16) 30u << 3)){
				scroll_target->x = (UINT16) 30u << 3;
				PRINT(23u, 6u, "A JUMP  B FIRE");
				PRINT(23u, 9u, "A FIRE  B JUMP");
				s_cursor = SpriteManagerAdd(SpriteCursor, ((UINT16) 22u << 3), ((UINT16) 5u << 3));
				cursor_spawned = 1u;
				PRINT(20u, 0u, "SELECT TO CHANGE");
				PRINT(20u, 1u, "START TO CHOOSE");
				titlescreen_step = 5u;
			}
		break;
		case 5u:
			if(s_cursor == 0 && cursor_spawned == 1){
				s_cursor = SpriteManagerAdd(SpriteCursor, ((UINT16) 22u << 3), ((UINT16) 5u << 3));
			}
			if(KEY_PRESSED(J_START)){go_to_password(StatePwd);}
		break;
	}		
}

void go_to_password(UINT8 my_next_state) BANKED{
	StopMusic;
	reset_sgb_palette_title();
	ChangeStateThroughBetween(my_next_state);
}
