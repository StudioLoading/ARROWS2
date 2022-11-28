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
#include "DiaryDialogs.h"

IMPORT_TILES(fontbw);
DECLARE_MUSIC(bgm_credits);
IMPORT_MAP(border);
IMPORT_MAP(mapdiary);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 collision_tiles_diary[] = {1, 2, 0};
extern UINT8 border_set_diary;
extern UINT8 border_set_ow;
extern UINT8 current_map;//0=south-west, 1=south-east, 2=north-west, 3=north-east
extern unsigned char dd1[];
extern unsigned char dd2[];
extern unsigned char dd3[];
extern unsigned char dd4[];

extern struct MISSION missions[4];
extern const UINT8 TOTAL_MISSIONS;


UINT8 cursor_posx[] = {12u, 12u, 12u, 12u, 12u, 132u};
UINT8 cursor_posy[] = {12u, 36u, 60u, 84u, 116u, 116u};
INT8 cursor_posi = 0u;
UINT8 cursor_old_posi = 0u;
const INT8 cursor_posimax = 6;
Sprite* diary_cursor = 0;
// cursor_posi
// TOTAL_MISSIONS / 4 = numero di pagine
UINT8 idx_mission = 0u;///TOTAL_MISSIONS

void show_missions();

void START(){
	if(border_set_diary == 0u){
        border_set_diary = 1u;
        LOAD_SGB_BORDER(border);        
	}
    //HIDE_WIN;

	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume
	//PlayMusic(bgm_credits, 0);
    
    //SGB palette
    if(sgb_check()){
        set_sgb_palette_2();
    }
    //scroll_target = 
    diary_cursor = SpriteManagerAdd(SpriteDiarycursor, 24u, 24u);
	InitScroll(BANK(mapdiary), &mapdiary, collision_tiles_diary, 0);

	INIT_FONT(fontbw, PRINT_BKG);
    SHOW_BKG;

    cursor_old_posi = cursor_posi;
    diary_cursor->x = cursor_posx[cursor_old_posi];
    diary_cursor->y = cursor_posy[cursor_old_posi];

    GetLocalizedDDLabel_EN(MISSIONI_IN_CORSO, dd1);
	PRINT(8, 0, dd1);
    show_missions();
}

void show_missions(){
    if(missions[0].mission_state == MISSION_STATE_ENABLED){
        GetLocalizedDDLabel_EN(FIND_BLACKIE_TITLE, dd2);
        PRINT(3, 2, dd2);
    }
}

void UPDATE(){
    if(KEY_RELEASED(J_START)){
        border_set_diary = 0u;
        SetState(StateOverworld);
    }
    if(KEY_RELEASED(J_RIGHT) || KEY_RELEASED(J_DOWN)){
        cursor_posi++;
    }
    if(KEY_RELEASED(J_LEFT) || KEY_RELEASED(J_UP)){
        cursor_posi--;
    }
    if(cursor_posi < 0){
        cursor_posi = cursor_posimax - 1;
    }
    if(cursor_posi >= cursor_posimax){
        cursor_posi = 0;
    }
    if(cursor_old_posi != cursor_posi){//muovo cursor verso prossima posizione
        cursor_old_posi = cursor_posi;
        diary_cursor->x = cursor_posx[cursor_posi];
        diary_cursor->y = cursor_posy[cursor_posi];
    }

}