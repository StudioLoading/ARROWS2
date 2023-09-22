#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "TilesAnimations0.h"
#include "custom_datas.h"
#include "sgb_palette.h"

#define MAX_WAIT_CHAR 4

IMPORT_TILES(font);
IMPORT_TILES(dialogtilesbase);
IMPORT_MAP(dialogmapbase);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern Sprite* s_motherpl;
extern Sprite* s_motherow;
extern UINT8 previous_state;
extern unsigned char EMPTY_STRING_21[];
extern unsigned char d1[];
extern unsigned char d2[];
extern unsigned char d3[];
extern unsigned char d4[];
extern unsigned char d5[];
extern unsigned char d6[];
extern unsigned char d7[];
extern unsigned char d8[];
extern unsigned char d9[];
extern unsigned char d10[];
extern unsigned char d11[];
extern unsigned char d12[];
extern unsigned char d13[];
extern unsigned char d14[];
extern unsigned char d15[];
extern unsigned char d16[];
extern unsigned char d17[];
extern unsigned char d18[];
extern unsigned char d19[];
extern unsigned char d20[];
extern unsigned char d21[];
extern unsigned char d22[];
extern WHOSTALKING whostalking;
extern struct MISSION enable_hospital;

extern UINT8 dialog_ready;
extern UINT8 counter_char;
extern UINT8 wait_char;
extern UINT8 writing_line;
extern UINT8 n_lines;
extern Sprite* dialog_cursor;
extern UINT8 next_page;
extern INT8 motherpl_hp;

extern void move_on() BANKED;

extern void GetLocalizedDialog_EN(UINT8* n_lines) BANKED;
extern void dialog_map() BANKED;
extern UINT8 get_quantity(INVITEMTYPE itemtype) BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;
extern void shift_text_one_line_up() BANKED;
extern void show_next_character() BANKED;

void START() {
	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume 0x77
    SpriteManagerLoad(SpriteInvcursor);
    //SGB PALETTE
        if(sgb_check()){
            set_sgb_palette01_WOLF();
        }
	//PlayMusic(bgm_credits, 0);
    HIDE_WIN;
    InitScroll(BANK(dialogmapbase), &dialogmapbase, 0, 0);
    dialog_map();
    INIT_FONT(font, PRINT_BKG);
    SHOW_BKG;
    wait_char = MAX_WAIT_CHAR;
    writing_line = 1u;
    counter_char = 0u;
    n_lines = 0u;
    dialog_ready = 0u;
	next_page = 0u;
}

void UPDATE() {
    if(KEY_RELEASED(J_UP)){
        counter_char = 0u;
        dialog_ready = 0u; 
    }
    if(dialog_ready == 0u){
        PRINT(0, 7, EMPTY_STRING_21);
        PRINT(0, 8, EMPTY_STRING_21);
        PRINT(0, 9, EMPTY_STRING_21);
        PRINT(0, 10, EMPTY_STRING_21);
        PRINT(0, 11, EMPTY_STRING_21);
        PRINT(0, 12, EMPTY_STRING_21);
        PRINT(0, 13, EMPTY_STRING_21);
        PRINT(0, 14, EMPTY_STRING_21);
        SpriteManagerRemoveSprite(dialog_cursor);
        n_lines = 0u;
		switch(enable_hospital.mission_state){            
            case MISSION_STATE_ACCOMPLISHED:
            case MISSION_STATE_REWARDED:// la curo e la rispedisco in overworld
				enable_hospital.mission_state = MISSION_STATE_REWARDED;
                whostalking = HOSPITAL_CURE;
                motherpl_hp = 5;
            break;
            case MISSION_STATE_DISABLED:
			case MISSION_STATE_ENABLED://ho bisogno di metallo speciale
                motherpl_hp = 2;
				whostalking = HOSPITAL_DISABLED; 
                enable_hospital.mission_state = MISSION_STATE_ENABLED;
                if(get_quantity(INVITEM_METAL_SPECIAL) > 0){//se in inventario ho il metallo specialo
                    enable_hospital.mission_state = MISSION_STATE_ACCOMPLISHED;
                    whostalking = HOSPITAL_ENABLING;
                    motherpl_hp = 5;
                }
			break;
		}
        GetLocalizedDialog_EN(&n_lines);
        wait_char = MAX_WAIT_CHAR;
        writing_line = 1u;
        dialog_ready = 1u;
    }
    if(dialog_ready == 1u){
        if(KEY_PRESSED(J_A) || KEY_PRESSED(J_B) || KEY_PRESSED(J_DOWN)){
            wait_char = 1u;
        }    
        wait_char--;
        if(wait_char == 0u){//mostra lettera successiva
            show_next_character();
        }
    }
    if(dialog_ready == 2u){
        dialog_cursor = SpriteManagerAdd(SpriteInvcursor,(UINT16)144u, (UINT16)120u);
        struct ItemSpawned* dialog_cursor_data = (struct ItemSpawned*)dialog_cursor->custom_data;
        dialog_cursor_data->configured = 2;
        dialog_ready = 3u;
    }
    if(dialog_ready == 3u && (KEY_RELEASED(J_A) || KEY_RELEASED(J_B))){
        if(next_page){
            next_page = 0u;
            dialog_ready = 0u;
            return;
        }
        switch(previous_state){
            case StateOverworld:
            break;
            default:
            break;
        }
        dialog_ready = 0;
        move_on();
    }
}

