#include "Banks/SetAutoBank.h"

#include "Palette.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "TilesAnimations0.h"
#include "custom_datas.h"

#define MAX_WAIT_CHAR 4

IMPORT_TILES(font);
IMPORT_MAP(dmapbackup);
IMPORT_TILES(dialogtilesbase);
IMPORT_MAP(dialogmapbase);
IMPORT_MAP(dialogmapintro);
IMPORT_MAP(dialogmapexzoo);
IMPORT_MAP(dialogmapcemetery);
IMPORT_MAP(dialogmapmine);
IMPORT_MAP(dmapblackiecave);
IMPORT_MAP(dmapshop);
IMPORT_MAP(dmapblackie);
IMPORT_MAP(dmaphood);
IMPORT_MAP(dmappolice);
IMPORT_MAP(dmapriverside);
IMPORT_MAP(dmapminotaur);
IMPORT_MAP(dmapminodef);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern Sprite* s_motherpl;
extern Sprite* s_motherow;
extern UINT8 previous_state;
extern unsigned char EMPTY_STRING_21[];
extern unsigned char d0[];
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
extern UINT8 choice;
extern UINT8 choice_left;
extern UINT8 choice_right;
extern struct MISSION find_blackie;
extern struct MISSION engage_smith;
extern struct MISSION golden_armor;
extern INT8 current_map;
extern INT8 chapter;
extern INT8 outwalker_info_step;
extern INT8 outwalker_info_given;
extern uint8_t sgb_running;

UINT8 dialog_ready = 0u;
UINT8 counter_char = 0u;
UINT8 wait_char = MAX_WAIT_CHAR;
UINT8 writing_line = 1u;
UINT8 n_lines = 0u;
Sprite* dialog_cursor = 0;
UINT8 next_page = 0u;
UINT8 give_new_password = 0;
UINT8 diary_spawned = 0;

void move_on() BANKED;
void shift_text_one_line_up() BANKED;
void show_next_character() BANKED;

extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void GetLocalizedDialog_EN(UINT8* n_lines) BANKED;
extern void position_init() BANKED;
extern UINT8 get_quantity(INVITEMTYPE itemtype) BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;
extern void restartFromHospital() BANKED;
extern void check_sgb_palette(UINT8 new_state) BANKED;
extern void load_chapter() BANKED;

void START() {
    HIDE_WIN;
    switch(whostalking){
        case INTRO: case ITEMDETAIL_MAP:
        InitScroll(BANK(dialogmapintro), &dialogmapintro, 0, 0);break;
        case DEATH:InitScroll(BANK(dialogmapcemetery), &dialogmapcemetery, 0, 0);break;
        case BLACKIE:
        case BLACKIE_DEAD_CHILD:
            InitScroll(BANK(dmapblackie), &dmapblackie, 0, 0);
        break;
        case CHILD: case CHILDS_SAVED:
            InitScroll(BANK(dmaphood), &dmaphood, 0, 0);
        break;
        case POLICE_0_GET_PASS: case POLICE_0_STILL_NOT_FOUND:
	    case POLICE_0_WONT_TALK: case POLICE_0_FIGHTING: case POLICE_0_NOGUARDS:
            InitScroll(BANK(dmappolice), &dmappolice, 0, 0);
        break;
        case FISHERMAN_LETSGO: case FISHERMAN_THERESFISH:
        case FISHERMAN_FPSGATOR_COMPLETED: case BRIDGE_BROKEN:
            InitScroll(BANK(dmapriverside), &dmapriverside, 0, 0);
        break;
        case MINOTAUR_ENTRANCE:
            InitScroll(BANK(dmapminotaur), &dmapminotaur, 0, 0);
        break;
        case MINOTAUR_DEFEAT:
            InitScroll(BANK(dmapminodef), &dmapminodef, 0, 0);
        break;
        default:
            switch(previous_state){
                case StateExzoo:InitScroll(BANK(dialogmapexzoo), &dialogmapexzoo, 0, 0);break;
                case StateMine:InitScroll(BANK(dialogmapmine), &dialogmapmine, 0, 0);break; 
                case StateCemetery:InitScroll(BANK(dialogmapcemetery), &dialogmapcemetery, 0, 0);break;
                case StateShop:InitScroll(BANK(dmapshop), &dmapshop, 0, 0);break;
                case StateBlackieroom:
                case StateBlackiecave:InitScroll(BANK(dmapblackiecave), &dmapblackiecave, 0, 0);break;
                case StateOutwalkers:
                case StateHood:InitScroll(BANK(dmaphood), &dmaphood, 0, 0);break;
                default:InitScroll(BANK(dmapbackup), &dmapbackup, 0, 0);break;
            }
        break;
    }
    scroll_target->x = 80u;
    scroll_target->x = 72u;
    INIT_FONT(font, PRINT_BKG);
    SHOW_BKG;
    wait_char = MAX_WAIT_CHAR;
    writing_line = 1u;
    counter_char = 0u;
    n_lines = 0u;
    dialog_ready = 0u;
	next_page = 0u;
	SHOW_SPRITES;
    choice = 0u;
    choice_left = 0u;
    choice_right = 0u;
    diary_spawned = 0;
    //SGB PALETTE CHECK
        if(sgb_running){
            check_sgb_palette(StateDialog);
        }    
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
        GetLocalizedDialog_EN(&n_lines);
        wait_char = MAX_WAIT_CHAR;
        writing_line = 1u;
        dialog_ready = 1u;
        PRINT(0, 7, d0);
    }else if(dialog_ready == 1u){
        if(KEY_PRESSED(J_A) || KEY_PRESSED(J_B) || KEY_PRESSED(J_DOWN)){
            wait_char = 1u;
        }
        wait_char--;
        if(wait_char == 0u){//mostra lettera successiva
            show_next_character();
        }
    }else if(dialog_ready == 2u){
        if(whostalking == INTRO){
            dialog_cursor = SpriteManagerAdd(SpriteStartbtn, (INT16)136u, (UINT16)120u);
        }else{
            dialog_cursor = SpriteManagerAdd(SpriteInvcursor,(UINT16)144u, (UINT16)120u);
            struct ItemSpawned* dialog_cursor_data = (struct ItemSpawned*)dialog_cursor->custom_data;
            if(choice == 1u){
                dialog_cursor_data->configured = 1;
                dialog_cursor->x = 8u;
                dialog_cursor->y = 96u;
            }else{
                dialog_cursor_data->configured = 2;
            }
        }
        dialog_ready = 3u;
    } else if(dialog_ready == 3u){
        if(choice == 0u){//NO CHOICE TO DO
            if(KEY_RELEASED(J_START) || KEY_TICKED(J_JUMP) || KEY_TICKED(J_FIRE)){
                move_on();
            }
        }else{//CHOICE TO DO
            if(KEY_RELEASED(J_SELECT) || KEY_RELEASED(J_LEFT) || KEY_RELEASED(J_RIGHT)){
                if(dialog_cursor->x == 8u){dialog_cursor->x = 104u;}
                else{dialog_cursor->x = 8u;}
            }
            if(KEY_RELEASED(J_START) || KEY_TICKED(J_JUMP) || KEY_TICKED(J_FIRE)){
                if(dialog_cursor->x == 8u){choice_left = 1u;}
                else{choice_right = 1u;}
                move_on();
            }            
        }
        switch(previous_state){
            case StateCemetery:
                if(whostalking == WHOST_SHOP_SMITH && diary_spawned == 0
                    && engage_smith.current_step < 1){
                    engage_smith.current_step = 1;
                    diary_spawned = 1;
					SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
                }
            break;
        }
    }
}

void show_next_character() BANKED{
    unsigned char to_print[2] = " ";
    switch(writing_line){
        case 1u:to_print[0] = d1[counter_char];break;
        case 2u:to_print[0] = d2[counter_char];break;
        case 3u:to_print[0] = d3[counter_char];break;
        case 4u:to_print[0] = d4[counter_char];break;
        case 5u:to_print[0] = d5[counter_char];break;
        case 6u:to_print[0] = d6[counter_char];break;
        case 7u:to_print[0] = d7[counter_char];break;
        case 8u:to_print[0] = d8[counter_char];break;
        case 9u:to_print[0] = d9[counter_char];break;
        case 10u:to_print[0] = d10[counter_char];break;
        case 11u:to_print[0] = d11[counter_char];break;
        case 12u:to_print[0] = d12[counter_char];break;
        case 13u:to_print[0] = d13[counter_char];break;
        case 14u:to_print[0] = d14[counter_char];break;
        case 15u:to_print[0] = d15[counter_char];break;
        case 16u:to_print[0] = d16[counter_char];break;
        case 17u:to_print[0] = d17[counter_char];break;
        case 18u:to_print[0] = d18[counter_char];break;
        case 19u:to_print[0] = d19[counter_char];break;
        case 20u:to_print[0] = d20[counter_char];break;
        case 21u:to_print[0] = d21[counter_char];break;
        case 22u:to_print[0] = d22[counter_char];break;
    }
    UINT8 writing_line_on_video = writing_line;
    if(writing_line > 7){writing_line_on_video = 7;}
    PRINT(counter_char, 7+writing_line_on_video, to_print);
    wait_char = MAX_WAIT_CHAR;
    counter_char++;
    if(counter_char == 21u){
        counter_char = 0u;
        writing_line++;
        if(writing_line > 7 && writing_line <= n_lines){
            shift_text_one_line_up();
        }
        if(writing_line > n_lines){
            dialog_ready = 2u;
        }
    }
}

void shift_text_one_line_up() BANKED{
    PRINT(0, 14, EMPTY_STRING_21);
    switch(writing_line){
        case 8u:
            {
                Sprite* up_cursor = SpriteManagerAdd(SpriteInvcursor, 18u << 3, 7u << 3);
               	struct ItemSpawned* up_cursor_data = (struct ItemSpawned*)up_cursor->custom_data;
                up_cursor_data->configured = 1;
                up_cursor->mirror = H_MIRROR;
            }
            //PRINT(0, 7, d1);
            PRINT(0, 8, d2);
            PRINT(0, 9, d3);
            PRINT(0, 10, d4);
            PRINT(0, 11, d5);
            PRINT(0, 12, d6);
            PRINT(0, 13, d7);
        break;
        case 9u:
            //PRINT(0, 7, d2);
            PRINT(0, 8, d3);
            PRINT(0, 9, d4);
            PRINT(0, 10, d5);
            PRINT(0, 11, d6);
            PRINT(0, 12, d7);
            PRINT(0, 13, d8);
        break;
        case 10u:
            //PRINT(0, 7, d3);
            PRINT(0, 8, d4);
            PRINT(0, 9, d5);
            PRINT(0, 10, d6);
            PRINT(0, 11, d7);
            PRINT(0, 12, d8);
            PRINT(0, 13, d9);
        break;
        case 11u:
            //PRINT(0, 7, d4);
            PRINT(0, 8, d5);
            PRINT(0, 9, d6);
            PRINT(0, 10, d7);
            PRINT(0, 11, d8);
            PRINT(0, 12, d9);
            PRINT(0, 13, d10);
        break;
        case 12u:
            //PRINT(0, 7, d5);
            PRINT(0, 8, d6);
            PRINT(0, 9, d7);
            PRINT(0, 10, d8);
            PRINT(0, 11, d9);
            PRINT(0, 12, d10);
            PRINT(0, 13, d11);
        break;
        case 13u:
            //PRINT(0, 7, d6);
            PRINT(0, 8, d7);
            PRINT(0, 9, d8);
            PRINT(0, 10, d9);
            PRINT(0, 11, d10);
            PRINT(0, 12, d11);
            PRINT(0, 13, d12);
        break;
        case 14u:
            //PRINT(0, 7, d7);
            PRINT(0, 8, d8);
            PRINT(0, 9, d9);
            PRINT(0, 10, d10);
            PRINT(0, 11, d11);
            PRINT(0, 12, d12);
            PRINT(0, 13, d13);
        break;
        case 15u:
            //PRINT(0, 7, d8);
            PRINT(0, 8, d9);
            PRINT(0, 9, d10);
            PRINT(0, 10, d11);
            PRINT(0, 11, d12);
            PRINT(0, 12, d13);
            PRINT(0, 13, d14);
        break;
        case 16u:
            //PRINT(0, 7, d9);
            PRINT(0, 8, d10);
            PRINT(0, 9, d11);
            PRINT(0, 10, d12);
            PRINT(0, 11, d13);
            PRINT(0, 12, d14);
            PRINT(0, 13, d15);
        break;
        case 17u:
            //PRINT(0, 7, d10);
            PRINT(0, 8, d11);
            PRINT(0, 9, d12);
            PRINT(0, 10, d13);
            PRINT(0, 11, d14);
            PRINT(0, 12, d15);
            PRINT(0, 13, d16);
        break;
        case 18u:
            //PRINT(0, 7, d11);
            PRINT(0, 8, d12);
            PRINT(0, 9, d13);
            PRINT(0, 10, d14);
            PRINT(0, 11, d15);
            PRINT(0, 12, d16);
            PRINT(0, 13, d17);
        break;
        case 19u:
            //PRINT(0, 7, d12);
            PRINT(0, 8, d13);
            PRINT(0, 9, d14);
            PRINT(0, 10, d15);
            PRINT(0, 11, d16);
            PRINT(0, 12, d17);
            PRINT(0, 13, d18);
        break;
        case 20u:
            //PRINT(0, 7, d13);
            PRINT(0, 8, d14);
            PRINT(0, 9, d15);
            PRINT(0, 10, d16);
            PRINT(0, 11, d17);
            PRINT(0, 12, d18);
            PRINT(0, 13, d19);
        break;
        case 21u:
            //PRINT(0, 7, d14);
            PRINT(0, 8, d15);
            PRINT(0, 9, d16);
            PRINT(0, 10, d17);
            PRINT(0, 11, d18);
            PRINT(0, 12, d19);
            PRINT(0, 13, d20);
        break;
        case 22u:
            //PRINT(0, 7, d15);
            PRINT(0, 8, d16);
            PRINT(0, 9, d17);
            PRINT(0, 10, d18);
            PRINT(0, 11, d19);
            PRINT(0, 12, d20);
            PRINT(0, 13, d21);
        break;
    }
}

void move_on() BANKED{
    SpriteManagerRemoveSprite(dialog_cursor);
    if(whostalking == BOSS_CRAB_FIGHT){previous_state = StateBosscrab;}
    if(previous_state == StateBlackiecave && whostalking == WOLF01){
        //choice_left == NO
        //choice_right == YES
        if(choice_right == 1u){
            previous_state = StateBlackieroom;
            find_blackie.current_step = 1u;
            find_blackie.mission_state = MISSION_STATE_STARTED;
			SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
        }
        //return;//DEMO ENDS HERE
    }else if(previous_state == StateBlackieroom && whostalking == BLACKIE){
        if(find_blackie.current_step == 3u){//to OW out of cave
            previous_state = StateOverworld;
        }
    }else if(whostalking == OUTWALKER_MAN2){
        if(choice_right == 1u && outwalker_info_given < 3){//SPEND 30 to learn some more info
            UINT8 current_coins = get_quantity(INVITEM_MONEY);
            if(current_coins > 30){//GOT THE MONEY
                change_quantity(INVITEM_MONEY, -30);
                outwalker_info_step = 1;
                outwalker_info_given++;
                SetState(StateDialog);
                return;
            }else if(outwalker_info_step < 2){//NOT ENOUGH MONEY
                outwalker_info_step = -1;
                SetState(StateDialog);
                return;
            }
        }
        /*else if(choice_right == 1u && outwalker_info_given >= 3){
            outwalker_info_step = 2;
            SetState(StateDialog);
            return;
        }*/
    }else if(whostalking == MINOTAUR_ENTRANCE){
        if(choice_right == 1u){//proceed to StateBossminotaur
            ChangeState(StateBossminotaur, s_motherpl, -1);
        }else{
            ChangeState(StateSilvercave, s_motherpl, -1);
        }
        return;
    }else if(whostalking == HOSPITAL_GAMEOVER){
        SetState(StateCredit);
        return;
    }
	if(next_page){
		next_page = 0u;
		dialog_ready = 0u;
		return;
	}
    //CHECK NEW PASSWORD TO GIVE?
        switch(whostalking){
            case BLACKIE:
                if(find_blackie.current_step == 4u){
                    give_new_password = 1;
                }
            break;
            case IBEX_GIVE_HERBS:
                give_new_password = 1;
            break;
            case BLACKIE_DEAD_CHILD:
                give_new_password = 1;
            break;
            case SMITH_FORGE_ARMOR:
                give_new_password = 1;
                golden_armor.current_step = 4;
            break;
        }
        if(give_new_password == 1){
            SetState(StatePassword);
            return;
        }
    if(whostalking == DEATH){
        restartFromHospital();
    }else if(whostalking == IBEX_GIVE_MISSION){
        whostalking = IBEX_GIVE_HERBS;
        SetState(StateDialog);
    }else if(whostalking == IBEX_GIVE_HERBS){
        ChangeState(StateCart, s_motherpl, -1);
    }else if(whostalking == FISHERMAN_LETSGO){
        ChangeState(StateFps, s_motherpl, -1);
    }else if(whostalking == FISHERMAN_FPSGATOR_COMPLETED){
        ChangeState(StateOverworld, s_motherow, 3);
    }else if(previous_state == StateOverworld || previous_state == StatePassword){
        ChangeState(previous_state, s_motherow, current_map);
    }else{
        ChangeState(previous_state, s_motherpl, -1);
    }
}