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

const UINT8 collision_tiles_ow_sw[] = {1, 2, 14, 15, 16, 17, 18, 22, 23, 24, 25, 26, 28, 29, 32, 33, 34, 39, 41, 44, 45, 46, 47, 50, 51, 53, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 0};
UINT8 border_set = 0u;
UINT8 current_map = 0u;//0=south-west, 1=south-east, 2=north-west, 3=north-east
UINT8 hudow_opened = 0;

void PauseGameOW();
void UnpauseGameOW();
void UpdateHUDOW();

void START(){
	if(border_set == 0u){
        border_set = 1u;
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
	PlayMusic(bgm_credits, 0);

	switch (current_map){
		case 0u:
			if(sgb_check()){
				set_sgb_palette_2();
			}
			scroll_target = SpriteManagerAdd(SpriteMotherow, (UINT16) 13u << 3, (UINT16) 16u << 3);
			InitScroll(BANK(owsouthwest), &owsouthwest, collision_tiles_ow_sw, 0);
        break;
	}

	SHOW_BKG;
	
	scroll_top_movement_limit = 56u;
	scroll_bottom_movement_limit = 88u;

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
}

void UpdateHUDOW(){
	if(KEY_RELEASED(J_DOWN)){
		UPDATE_HUD_TILE(0,1,0);
		UPDATE_HUD_TILE(0,3,3);//show cursor in position 0,1
		INT8 tx = 0;
		for(tx=1;tx<19;tx++){
			UPDATE_HUD_TILE(tx,2,0);
			UPDATE_HUD_TILE(tx,4,4);//show underline
		}
		UPDATE_HUD_TILE(19,1,0);//show cursor in position 19,1
		UPDATE_HUD_TILE(19,3,3);//show cursor in position 19,1
	}else if(KEY_RELEASED(J_UP)){
		UPDATE_HUD_TILE(0,1,3);//show cursor in position 0,1
		UPDATE_HUD_TILE(0,3,0);
		INT8 tx = 0;
		for(tx=1;tx<19;tx++){
			UPDATE_HUD_TILE(tx,2,4);//show underline
			UPDATE_HUD_TILE(tx,4,0);//show underline
		}
		UPDATE_HUD_TILE(19,1,3);//show cursor in position 19,
		UPDATE_HUD_TILE(19,3,0);//show cursor in position 19,1
	}
}

void PauseGameOW(){
	//TODO metti scroll_target in idle
	SHOW_WIN;
	UPDATE_HUD_TILE(0,1,3);//show cursor in position 0,1
	INT8 tx = 0;
	for(tx=1;tx<19;tx++){
		UPDATE_HUD_TILE(tx,2,4);//show underline
	}
	UPDATE_HUD_TILE(19,1,3);//show cursor in position 19,1
	PRINT(2, 1, "DIARIO MISSIONI");
	PRINT(2, 3, "OPZIONI PARTITA");
}

void UnpauseGameOW(){
	HIDE_WIN;
}

