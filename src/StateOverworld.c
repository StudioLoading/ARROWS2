#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "BankManager.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Print.h"
#include "Sound.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "sgb_palette.h"
#include "Dialogs.h"

IMPORT_MAP(border2);

IMPORT_TILES(fontbw);
IMPORT_TILES(tilesowsouthwest);

IMPORT_MAP(owsouthwest);
IMPORT_MAP(hudow);

extern UINT8 scroll_top_movement_limit;
extern UINT8 scroll_bottom_movement_limit;
extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 collision_tiles_ow_sw[] = {1, 2, 14, 15, 16, 17, 18, 22, 23, 24, 25, 26, 28, 29, 32, 
33, 34, 39, 41, 44, 45, 46, 47, 50, 51, 53, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 
68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 95, 96, 0};
UINT8 border_set_ow = 0u;
UINT8 border_set_diary = 0u;
UINT8 border_set_exzoo = 0u;
UINT8 current_map = 0u;//0=south-west, 1=south-east, 2=north-west, 3=north-east
UINT8 hudow_opened = 0;
UINT8 show_tip = 0u;
INT8 show_tip_movingscroll = 0u;
UINT8 showed_tip = 0u;
UINT8 showed_tip_goback = 0u;
UINT8 showing_tip_line = 0u;
UINT8 showing_tip_delay = 8u;
HUD_OPTION owhudopt = OW_DIARY;
Sprite* s_motherow = 0;
TIP_TO_BE_LOCALIZED tip_to_show = TIP_SMITH_NO;

extern struct OwSpriteInfo* motherow_info;
extern UINT16 motherow_pos_x;
extern UINT16 motherow_pos_y;
extern unsigned char EMPTY_STRING_21[];
extern unsigned char D0[];
extern unsigned char d1[];
extern unsigned char d2[];
extern unsigned char d3[];
extern unsigned char d4[];
extern struct MISSION missions[4];

void PauseGameOW();
void UnpauseGameOW();
void UpdateHUDOW();
void DrawHUD(HUD_OPTION opt);
void ShowTipOW() BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;
extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;
extern void update_position_motherow() BANKED;

void START(){
    LOAD_SGB_BORDER(border2);
	//SOUND
		NR52_REG = 0x80; //Enables sound, you should always setup this first
		NR51_REG = 0xFF; //Enables all channels (left and right)
		NR50_REG = 0x77; //Max volume
	//SCROLL LIMITS
		scroll_top_movement_limit = 56u;
		scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
		show_tip = 0u;
		switch (current_map){
			case 0u:
				if(sgb_check()){
					set_sgb_palette_overworldsw();
				}
				if(motherow_pos_x == 0u){
					motherow_pos_x = (UINT16) 19u << 3;
				}
				if(motherow_pos_y == 0u){
					motherow_pos_y = (UINT16) 24u << 3;
				}
				s_motherow = SpriteManagerAdd(SpriteMotherow, motherow_pos_x, motherow_pos_y);
				scroll_target = SpriteManagerAdd(SpriteCamerafocus, motherow_pos_x, motherow_pos_y);
				InitScroll(BANK(owsouthwest), &owsouthwest, collision_tiles_ow_sw, 0);
				if(missions[0].current_step == 3u){
					SpriteManagerAdd(SpriteBlackieow, motherow_pos_x + 8u, motherow_pos_y - 16u);
					missions[0].current_step = 4u;
				}
			break;
		}
	INIT_FONT(fontbw, PRINT_WIN);
	INIT_HUD(hudow); 
	HIDE_WIN;
	SHOW_SPRITES;
	hudow_opened = 0;	
}

void ShowTipOW() BANKED{
	show_tip = 1u;
	if(showed_tip == 0u){
		if(show_tip_movingscroll < 64u){
			show_tip_movingscroll+=2;
			scroll_target->y+=2;
		}else{
			if(print_target != PRINT_BKG){
				print_target = PRINT_BKG;
			}
			showing_tip();
		}
	}
	if(showed_tip_goback == 1u){//SpriteMotherow lo setta a 1
		if(show_tip_movingscroll > -24u){
			show_tip_movingscroll-=2;
			scroll_target->y-=2;
		}else{
			show_tip_movingscroll = 0u;
			showed_tip_goback = 0u;
			showed_tip = 0u;
			show_tip = 0u;
		}
	}
}

void UPDATE(){
	if(show_tip == 0u){
		scroll_target->x = s_motherow->x+4u;
		scroll_target->y = s_motherow->y+4u;
	}
	if(KEY_RELEASED(J_START)){
		switch(hudow_opened){
			case 0u://vado in 
				my_play_fx(CHANNEL_1, 60, 0x36, 0x9f, 0xf6, 0x91, 0x86);//SFX_START
				PauseGameOW();
				hudow_opened = 1u;
			break;
			case 1u:
				UnpauseGameOW();
				hudow_opened = 0u;
			break;
		}
	}
	if(hudow_opened == 1u){
		UpdateHUDOW();
		return;
	}
	if(show_tip == 1u){
		ShowTipOW();
		return;
	}	
}

void showing_tip(){
	if(showed_tip == 0){
		UINT8 r = scroll_target->x % 8u;
		UINT8 xpos = (scroll_target->x >> 3) - 10u;
		if(showing_tip_line == 0){
			//SFX
				my_play_fx(CHANNEL_2, 60, 0x8b, 0x67, 0xa5, 0x84, 0x00);//SFX_OW_TIP	
			GetLocalizedTip_EN(tip_to_show);
			PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 3u, D0);
			PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 4u, EMPTY_STRING_21);
			PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 5u, EMPTY_STRING_21);
			PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 6u, EMPTY_STRING_21);
			PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 7u, EMPTY_STRING_21);
			showing_tip_line = 1;
		}else if (showing_tip_line < 5){
			if(showing_tip_delay > 0){showing_tip_delay--;}
			else{
				switch(showing_tip_line){
					case 1:PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 3u, d1);break;
					case 2:PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 3u, d2);break;
					case 3:PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 3u, d3);break;
					case 4:PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 3u, d4);break;
				}
				showing_tip_delay = 24u;
				showing_tip_line++;
			}
		}else{
			showing_tip_delay = 8u;
			showing_tip_line = 0;
			showed_tip = 1u;		
		}
	}
}

void UpdateHUDOW(){
	HUD_OPTION old_owhudopt = owhudopt;
	if(KEY_RELEASED(J_DOWN)){
		owhudopt = OW_GAMEOPT;
	}else if(KEY_RELEASED(J_UP)){
		owhudopt = OW_DIARY;
	}
	if(old_owhudopt != owhudopt){
		DrawHUD(owhudopt);
		old_owhudopt = owhudopt;
	}
	if(KEY_TICKED(J_A)){
		HIDE_WIN;
		switch(owhudopt){
			case OW_DIARY://selezionato Diario Missione
				ChangeState(StateDiary, s_motherow);
			break;
			case OW_GAMEOPT://selezionato Menu Partita
				ChangeState(StateOverworld, s_motherow);
			break;
		}
	}
}

void DrawHUD(HUD_OPTION opt){
		INT8 tx = 0;
		switch(opt){
			case OW_GAMEOPT:			
				UPDATE_HUD_TILE(0,1,0);
				UPDATE_HUD_TILE(0,3,3);//show cursor in position 0,1
				for(tx=1;tx<19;tx++){
					UPDATE_HUD_TILE(tx,2,0);
					UPDATE_HUD_TILE(tx,4,4);//show underline
				}
				UPDATE_HUD_TILE(19,1,0);//show cursor in position 19,1
				UPDATE_HUD_TILE(19,3,3);//show cursor in position 19,1
			break;
			case OW_DIARY:
				UPDATE_HUD_TILE(0,1,3);//show cursor in position 0,1
				UPDATE_HUD_TILE(0,3,0);
				for(tx=1;tx<19;tx++){
					UPDATE_HUD_TILE(tx,2,4);//show underline
					UPDATE_HUD_TILE(tx,4,0);//show underline
				}
				UPDATE_HUD_TILE(19,1,3);//show cursor in position 19,
				UPDATE_HUD_TILE(19,3,0);//show cursor in position 19,1
			break;
		}
}

void PauseGameOW(){
	//TODO metti scroll_target in idle
	print_target = PRINT_WIN;
	SHOW_WIN;
	DrawHUD(owhudopt);
	GetLocalizedLabel_EN(DIARIO_MISSIONI, d1);
	PRINT(2, 1, d1);
	GetLocalizedLabel_EN(OPZIONI_PARTITA, d2);
	PRINT(2, 3, d2);
}

void UnpauseGameOW(){
	HIDE_WIN;
}

