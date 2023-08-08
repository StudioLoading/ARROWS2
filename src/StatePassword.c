#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Sound.h"

#include "custom_datas.h"
#include "sgb_palette.h"


IMPORT_TILES(fontbw);
IMPORT_MAP(password);
IMPORT_MAP(borderdiary);

extern Sprite* inv_cursor;
extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern WHOSTALKING whostalking;
extern UINT8 previous_state;
extern unsigned char EMPTY_STRING_21[];
extern INT8 chapter;
extern UINT8 just_started;
extern UINT8 generic_counter;
extern INT8 sfx_cooldown;

const UINT8 coll_tiles_password[] = {1,0};
Sprite* pcode_0;
Sprite* pcode_1;
Sprite* pcode_2;
Sprite* pcode_3;
struct TetradadoInfo* pcode0_info;
struct TetradadoInfo* pcode1_info;
struct TetradadoInfo* pcode2_info;
struct TetradadoInfo* pcode3_info;
UINT16 cur_posx[4];
UINT16 cur_posy[4];
UINT8 cur_posi = 0u;
UINT8 generic_counter2 = 0u;

void update_curpos(INT8 move) BANKED;
void update_pcode(INT8 move) BANKED;
void update_pcode_face(struct TetradadoInfo* pcode_data, INT8 move) BANKED;
INT8 check_password() BANKED;
void password_reset() BANKED;
void load_chapter() BANKED;

extern void ChangeStateThroughBetween(UINT8 new_state, UINT8 previous_state) BANKED;
extern void missions_init() BANKED;
extern void inventory_init() BANKED;
extern void position_init() BANKED;
extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;


void START(){
    //SGB PALETTE
        if(sgb_check()){
            set_sgb_palette01_WOLF();
        }  
    //SPRITES SPAWNING & SETTINGS
        HIDE_WIN;
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 10u << 3, (UINT16) 8u << 3);
        pcode_0 = SpriteManagerAdd(SpriteTetradado, ((UINT16) 3u << 3), ((UINT16) 7u << 3));
        pcode0_info = (struct TetradadoInfo*) pcode_0->custom_data;
        pcode0_info->tetradado_state = DADO_WAITING;
        pcode_1 = SpriteManagerAdd(SpriteTetradado, ((UINT16) 7u << 3), ((UINT16) 11u << 3));
        pcode1_info = (struct TetradadoInfo*) pcode_1->custom_data;
        pcode1_info->tetradado_state = DADO_WAITING;
        pcode_2 = SpriteManagerAdd(SpriteTetradado, ((UINT16) 11u << 3), ((UINT16) 7u << 3));
        pcode2_info = (struct TetradadoInfo*) pcode_2->custom_data;
        pcode2_info->tetradado_state = DADO_WAITING;
        pcode_3 = SpriteManagerAdd(SpriteTetradado, ((UINT16) 15u << 3), ((UINT16) 11u << 3));
        pcode3_info = (struct TetradadoInfo*) pcode_3->custom_data;
        pcode3_info->tetradado_state = DADO_WAITING;
        inv_cursor = SpriteManagerAdd(SpriteInvcursor, pcode_0->x, pcode_0->y-16u);
        NR52_REG = 0x80; //Enables sound, you should always setup this first
        NR51_REG = 0xFF; //Enables all channels (left and right)
        //NR50_REG = 0x44; //Max volume 0x77

    //MAP
        InitScroll(BANK(password), &password, coll_tiles_password, 0);
        INIT_FONT(fontbw, PRINT_BKG);
    //VAR INIT
        cur_posi = 0u;
        cur_posx[0] = 24u;
        cur_posy[0] = 40u;
        cur_posx[1] = 56u;
        cur_posy[1] = 72u;
        cur_posx[2] = 88u;
        cur_posy[2] = 40u;
        cur_posx[3] = 120u;
        cur_posy[3] = 72u;
        generic_counter = 0u;
        generic_counter2 = 0u;
    //SHOW
        SHOW_BKG;
        SHOW_SPRITES;
    //WRITE TEXT    
        PRINT(0, 1, "WHAT'S THE LAST CODE");
        PRINT(0, 2, "YOU REMEMBER TO BE  ");
        PRINT(0, 3, "TOLD?               ");
        PRINT(0, 15, "START  TO LOAD     ");
        PRINT(0, 16, "SELECT TO RESET    ");
}

void UPDATE(){
    if(sfx_cooldown > 0){sfx_cooldown--;}
    generic_counter++;
    if(generic_counter == 0u){
        generic_counter2++;
        if(generic_counter2 == 5u){
            ChangeStateThroughBetween(StateCredit, StatePassword);
        }
    }
    if(KEY_TICKED(J_RIGHT)){ update_curpos(1); }
    if(KEY_TICKED(J_LEFT)){ update_curpos(-1); }
    if(KEY_TICKED(J_UP)){ update_pcode(1);}
    if(KEY_TICKED(J_DOWN)){ update_pcode(-1);}
    if(KEY_TICKED(J_A) || KEY_TICKED(J_B)){ update_pcode(0);}
    if(KEY_TICKED(J_SELECT)){ password_reset();}
    if(KEY_TICKED(J_START)){
        chapter = check_password();
        if(chapter == -1){//invalid code inserted, reset
            my_play_fx(CHANNEL_1, 50, 0x29, 0x81, 0x43, 0x01, 0x85);//SFX WRONG PWD
            password_reset();
            return;
        }
        load_chapter();
    }
}

void load_chapter() BANKED{
    my_play_fx(CHANNEL_1, 50, 0x56, 0x86, 0x76, 0xDE, 0x86);//SFX OK PWD
    missions_init();
    inventory_init();
    position_init();
    just_started = 1u;
    switch(chapter){
        case 0:
            just_started = 0;
            previous_state = StateOverworld;
            whostalking = INTRO;
            //LOAD_SGB_BORDER(borderdiary);
            ChangeStateThroughBetween(StateDialog, StateTitlescreen);
        break;
        case 1:
            ChangeStateThroughBetween(StateOverworld, StatePassword);
        break;
    }
}

void update_curpos(INT8 move) BANKED{
    generic_counter = 0;
    generic_counter2 = 0;
    my_play_fx(CHANNEL_2, 50, 0xBF, 0xF1, 0x27, 0x87, 0x00);
    cur_posi += move;
    cur_posi = cur_posi % 4;
    inv_cursor->x = cur_posx[cur_posi];
    inv_cursor->y = cur_posy[cur_posi];
}

void update_pcode(INT8 move) BANKED{
    generic_counter = 0;
    generic_counter2 = 0;
    Sprite* current_pcode = 0;
    switch(cur_posi){
        case 0u:
            update_pcode_face(pcode0_info, move);
        break;
        case 1u:
            update_pcode_face(pcode1_info, move);
        break;
        case 2u:
            update_pcode_face(pcode2_info, move);
        break;
        case 3u:
            update_pcode_face(pcode3_info, move);
        break;
    }
}

void update_pcode_face(struct TetradadoInfo* pcode_data, INT8 move) BANKED{
    if(move == 0){
        pcode_data->tetradado_state = DADO_WAITING;
        return;
    }else if(pcode_data->tetradado_state != PASSWORD){
        pcode_data->tetradado_state = PASSWORD;
        if(move == 1){ pcode_data->tetradado_faccia = FACCIA_1; }
        else if(move == -1){ pcode_data->tetradado_faccia = FACCIA_4; }
    }else{
        switch(pcode_data->tetradado_faccia){
            case FACCIA_1:
                    if(move == 1){pcode_data->tetradado_faccia = FACCIA_2;}
                    else if(move == -1){pcode_data->tetradado_faccia = FACCIA_4;}
            break; 
            case FACCIA_2:
                    if(move == 1){pcode_data->tetradado_faccia = FACCIA_3;}
                    else if(move == -1){pcode_data->tetradado_faccia = FACCIA_1;}
            break; 
            case FACCIA_3:
                    if(move == 1){pcode_data->tetradado_faccia = FACCIA_4;}
                    else if(move == -1){pcode_data->tetradado_faccia = FACCIA_2;}
            break; 
            case FACCIA_4:
                    if(move == 1){pcode_data->tetradado_faccia = FACCIA_1;}
                    else if(move == -1){pcode_data->tetradado_faccia = FACCIA_3;}
            break; 
        }
    }
}

INT8 check_password() BANKED{
    INT8 result = -1u;
    //IF ALL PCODE IS STILL WAITING (MEANS WITH ? SHOWN)
        if(pcode0_info->tetradado_state != PASSWORD &&
        pcode1_info->tetradado_state != PASSWORD &&
        pcode2_info->tetradado_state != PASSWORD &&
        pcode3_info->tetradado_state != PASSWORD){
            result = 0u;
        } 
    //THE CODE IS NOT VALID, RESET
        else if(pcode0_info->tetradado_state != PASSWORD ||
        pcode1_info->tetradado_state != PASSWORD ||
        pcode2_info->tetradado_state != PASSWORD ||
        pcode3_info->tetradado_state != PASSWORD){
            result = -1u;
        }
    //CHECK IF ALL PCODE HAS PASSWORD STATUS
        else if(pcode0_info->tetradado_state == PASSWORD &&
        pcode1_info->tetradado_state == PASSWORD &&
        pcode2_info->tetradado_state == PASSWORD &&
        pcode3_info->tetradado_state == PASSWORD){
            //then check combinations
            //cpt2
                if(pcode0_info->tetradado_faccia == FACCIA_4 &&
                pcode1_info->tetradado_faccia == FACCIA_1 &&
                pcode2_info->tetradado_faccia == FACCIA_4 &&
                pcode3_info->tetradado_faccia == FACCIA_1){
                    result = 1u;
                }
        }
    return result;
}

void password_reset() BANKED{
    cur_posi = 0;
    inv_cursor->x = cur_posx[cur_posi];
    inv_cursor->y = cur_posy[cur_posi];
    pcode0_info->tetradado_state = DADO_WAITING;
    pcode1_info->tetradado_state = DADO_WAITING;
    pcode2_info->tetradado_state = DADO_WAITING;
    pcode3_info->tetradado_state = DADO_WAITING;
}