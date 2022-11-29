#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "BankManager.h"
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

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "sgb_palette.h"
#include "Dialogs.h"

IMPORT_TILES(fontbw);

IMPORT_TILES(tilesowsouthwest);

IMPORT_MAP(owsouthwest);
DECLARE_MUSIC(bgm_credits);
IMPORT_MAP(border2);
IMPORT_MAP(hudow);

extern UINT8 scroll_top_movement_limit;
extern UINT8 scroll_bottom_movement_limit;
extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 collision_tiles_ow_sw[] = {1, 2, 14, 15, 16, 17, 18, 22, 23, 24, 25, 26, 28, 29, 32, 
33, 34, 39, 41, 44, 45, 46, 47, 50, 51, 53, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 
68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 0};
UINT8 border_set_ow = 0u;
UINT8 border_set_diary = 0u;
UINT8 current_map = 0u;//0=south-west, 1=south-east, 2=north-west, 3=north-east
UINT8 hudow_opened = 0;
UINT16 ow_mother_pos_x = 0u;
UINT16 ow_mother_pos_y = 0u;
UINT8 show_tip = 0u;
INT8 show_tip_movingscroll = 0u;
UINT8 showed_tip = 0u;
UINT8 showed_tip_goback = 0u;
UINT8 showing_tip_line = 0u;
UINT8 showing_tip_delay = 8u;
HUD_OPTION owhudopt = OW_DIARY;
Sprite* s_motherow = 0;
TIP_TO_BE_LOCALIZED tip_to_show = TIP_SMITH_NO;
extern unsigned char D0[];
extern unsigned char d1[];
extern unsigned char d2[];
extern unsigned char d3[];
extern unsigned char d4[];

void PauseGameOW();
void UnpauseGameOW();
void UpdateHUDOW();
void DrawHUD(HUD_OPTION opt);

void START(){
	if(border_set_ow == 0u){
        border_set_ow = 1u;
        switch(current_map){
		    case 0u:
                LOAD_SGB_BORDER(border2);
            break;
        }
	}
	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume
	//PlayMusic(bgm_credits, 0);
	
	//SCROLL LIMITS
	scroll_top_movement_limit = 56u;
	scroll_bottom_movement_limit = 80u;

	switch (current_map){
		case 0u:
			if(sgb_check()){
				set_sgb_palette_2();
			}
			if(ow_mother_pos_y == 0u){
				ow_mother_pos_y = (UINT16) 16u << 3;
			}
			if(ow_mother_pos_x == 0u){
				ow_mother_pos_x = (UINT16) 13u << 3;
			}
			s_motherow = SpriteManagerAdd(SpriteMotherow, ow_mother_pos_x, ow_mother_pos_y);
			scroll_target = SpriteManagerAdd(SpriteCamerafocus, ow_mother_pos_x, ow_mother_pos_y);
			InitScroll(BANK(owsouthwest), &owsouthwest, collision_tiles_ow_sw, 0);
        break;
	}

	SHOW_BKG;


	hudow_opened = 0;	
	INIT_FONT(fontbw, PRINT_WIN);
	INIT_HUD(hudow); 
	HIDE_WIN;
}

void UPDATE(){
	if(KEY_RELEASED(J_START)){
		switch(hudow_opened){
			case 0u://vado in 
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
		if(showed_tip == 0u){
			if(show_tip_movingscroll < 64u){
				show_tip_movingscroll++;
				scroll_target->y++;
			}else{
				if(print_target != PRINT_BKG){
					print_target = PRINT_BKG;
				}
				showing_tip();
			}
		}
		if(showed_tip_goback == 1u){//SpriteMotherow lo setta a 1
			if(show_tip_movingscroll > -32u){
				show_tip_movingscroll--;
				scroll_target->y--;
			}else{
				show_tip_movingscroll = 0u;
				showed_tip_goback = 0u;
				showed_tip = 0u;
				show_tip = 0u;
			}
		}
	}else{
		scroll_target->x = s_motherow->x;
		scroll_target->y = s_motherow->y;
	}
}

void showing_tip(){
	if(showed_tip == 0){
		if(showing_tip_line == 0){
			GetLocalizedTip_EN(tip_to_show);
			PRINT((scroll_target->x >> 3) - 9u, (scroll_target->y >> 3) + showing_tip_line + 3u, D0);
			showing_tip_line = 1;
		}else if (showing_tip_line < 5){
			if(showing_tip_delay > 0){showing_tip_delay--;}
			else{
				switch(showing_tip_line){
					case 1:PRINT((scroll_target->x >> 3) - 9u, (scroll_target->y >> 3) + showing_tip_line + 3u, d1);break;
					case 2:PRINT((scroll_target->x >> 3) - 9u, (scroll_target->y >> 3) + showing_tip_line + 3u, d2);break;
					case 3:PRINT((scroll_target->x >> 3) - 9u, (scroll_target->y >> 3) + showing_tip_line + 3u, d3);break;
					case 4:PRINT((scroll_target->x >> 3) - 9u, (scroll_target->y >> 3) + showing_tip_line + 3u, d4);break;
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
		ow_mother_pos_x = scroll_target->x;
		ow_mother_pos_y = scroll_target->y;
		SetWindowY(160);
		switch(owhudopt){
			case OW_DIARY://selezionato Diario Missione
    			border_set_ow = 0u;
				SetState(StateDiary);
			break;
			case OW_GAMEOPT://selezionato Menu Partita
				SetState(StateOverworld);
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

