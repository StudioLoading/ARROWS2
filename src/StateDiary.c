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

IMPORT_MAP(borderdiary);
IMPORT_TILES(fontbw);
DECLARE_MUSIC(bgm_credits);
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
extern unsigned char dd5[];
extern unsigned char dd6[];
extern unsigned char dd7[];
extern unsigned char dd8[];

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
INT8 idx_page = 0u;
UINT8 showing_detail = 0u;

void empty_dds();
void show_missions();
void show_detail();
void change_page(INT8 inc);

extern void ChangeStateThroughBetween(UINT8 new_state) BANKED;

void START(){
    //HIDE_WIN;
    LOAD_SGB_BORDER(borderdiary);

	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume
	//PlayMusic(bgm_credits, 0);
    
    diary_cursor = SpriteManagerAdd(SpriteDiarycursor, 24u, 24u);
	InitScroll(BANK(mapdiary), &mapdiary, collision_tiles_diary, 0);
    scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 10u << 3, (UINT16) 9u << 3);
	INIT_FONT(fontbw, PRINT_BKG);
    SHOW_BKG;

    cursor_old_posi = cursor_posi;
    diary_cursor->x = cursor_posx[cursor_old_posi];
    diary_cursor->y = cursor_posy[cursor_old_posi];

    idx_page = 0u;
    show_missions();
    showing_detail = 0u;
    
	SHOW_SPRITES;
}

void empty_dds(){
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd1);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd2);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd3);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd4);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd5);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd6);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd7);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd8);
}

void show_detail(){
    empty_dds();
    showing_detail = 1u;
    switch(idx_page){
        case 0u:
            switch(cursor_posi){
                case 0u:
                    GetLocalizedDDLabel_EN(FIND_BLACKIE_D0, dd2);    
                    GetLocalizedDDLabel_EN(FIND_BLACKIE_D1, dd3);                    
                break;
            }
        break;
    }
}

void show_missions(){
    empty_dds();
    GetLocalizedDDLabel_EN(MISSIONI_IN_CORSO, dd1);
	PRINT(8, 0, dd1);
    switch(idx_page){
        case 0u:
            if(missions[0].mission_state == MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(FIND_BLACKIE_TITLE, dd2);
            }
            if(missions[1].mission_state == MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(HELP_DESPARATE_WIDOW_TITLE, dd4);
            }
        break;
        case 1u:
            if(missions[4].mission_state == MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(FIND_BLACKIE_TITLE, dd2);
            }
        break;
    }
    PRINT(3, 2, dd2);
    PRINT(3, 4, dd4);
    
	PRINT(7, 16, "%i:%u", idx_page+1, TOTAL_MISSIONS<<2);
}

void change_page(INT8 inc){
    idx_page+=inc;
    if(showing_detail == 0u){//sto mostrando elenco missioni
        if(idx_page<0){idx_page = (TOTAL_MISSIONS<<2)-1;}
        else{idx_page %= TOTAL_MISSIONS<<2;}
    }
}

void UPDATE(){
    if(KEY_RELEASED(J_START)){
        border_set_diary = 0u;
        ChangeStateThroughBetween(StateOverworld);
    }
    if(showing_detail == 0u){
        if (scroll_target->x > (UINT16) 10u << 3){
            scroll_target->x-=3;
        }else{
            show_missions();
        }
        if(KEY_TICKED(J_A) || KEY_TICKED(J_B)){
            if(cursor_posi < 4){
                show_detail();
            }else if (cursor_posi == 4){//Left cursor selected
                change_page(-1);
            }else if (cursor_posi == 5){//Left cursor selected
                change_page(1);
            }
        }
        if(KEY_TICKED(J_LEFT)){
            change_page(-1);
        }
        if(KEY_TICKED(J_RIGHT)){
            change_page(1);
        }
        if(KEY_RELEASED(J_DOWN)){
            cursor_posi++;
        }
        if(KEY_RELEASED(J_UP)){
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
    }else{
        if(scroll_target->x < (UINT16) 29u << 3){
            scroll_target->x+=3;
        }else{            
            PRINT(20, 2, dd2);
            PRINT(20, 3, dd3);
            PRINT(20, 4, dd4);
            PRINT(20, 5, dd5);
        }
        if(KEY_TICKED(J_A) || KEY_TICKED(J_B)){
            showing_detail = 0u;
        }
    }
}