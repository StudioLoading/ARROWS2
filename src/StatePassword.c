#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Sound.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "InventoryDialogs.h"


IMPORT_TILES(fontbw);
IMPORT_MAP(password);
IMPORT_MAP(borderdiary);

extern Sprite* inv_cursor;
extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern WHOSTALKING whostalking;
extern UINT8 previous_state;

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

void update_curpos(INT8 move) BANKED;
void update_pcode(INT8 move) BANKED;
void update_pcode_face(struct TetradadoInfo* pcode_data, INT8 move) BANKED;

extern void ChangeStateThroughBetween(UINT8 new_state, UINT8 previous_state) BANKED;

void START(){    
    //SPRITES SPAWNING & SETTINGS
        HIDE_WIN;
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 10u << 3, (UINT16) 9u << 3);
        pcode_0 = SpriteManagerAdd(SpriteTetradado, ((UINT16) 3u << 3), ((UINT16) 8u << 3));
        pcode0_info = (struct TetradadoInfo*) pcode_0->custom_data;
        pcode0_info->tetradado_state = DADO_WAITING;
        pcode_1 = SpriteManagerAdd(SpriteTetradado, ((UINT16) 7u << 3), ((UINT16) 12u << 3));
        pcode1_info = (struct TetradadoInfo*) pcode_1->custom_data;
        pcode1_info->tetradado_state = DADO_WAITING;
        pcode_2 = SpriteManagerAdd(SpriteTetradado, ((UINT16) 11u << 3), ((UINT16) 8u << 3));
        pcode2_info = (struct TetradadoInfo*) pcode_2->custom_data;
        pcode2_info->tetradado_state = DADO_WAITING;
        pcode_3 = SpriteManagerAdd(SpriteTetradado, ((UINT16) 15u << 3), ((UINT16) 12u << 3));
        pcode3_info = (struct TetradadoInfo*) pcode_3->custom_data;
        pcode3_info->tetradado_state = DADO_WAITING;
        inv_cursor = SpriteManagerAdd(SpriteInvcursor, pcode_0->x, pcode_0->y-16u);
    //MAP
        INIT_FONT(fontbw, PRINT_BKG);
        InitScroll(BANK(password), &password, coll_tiles_password, 0);
    //VAR INIT
        cur_posi = 0u;
        cur_posx[0] = 24u;
        cur_posy[0] = 48u;
        cur_posx[1] = 56u;
        cur_posy[1] = 80u;
        cur_posx[2] = 88u;
        cur_posy[2] = 48u;
        cur_posx[3] = 120u;
        cur_posy[3] = 80u;
    //SHOW
        SHOW_BKG;
        SHOW_SPRITES;
}

void UPDATE(){
    if(KEY_TICKED(J_START)){        
        previous_state = StateOverworld;
        whostalking = INTRO;
        //LOAD_SGB_BORDER(borderdiary);
        ChangeStateThroughBetween(StateDialog, StateTitlescreen);
    }
    if(KEY_TICKED(J_RIGHT)){ update_curpos(1); }
    if(KEY_TICKED(J_LEFT)){ update_curpos(-1); }
    if(KEY_TICKED(J_UP)){ update_pcode(1);}
    if(KEY_TICKED(J_DOWN)){ update_pcode(-1);}
}

void update_curpos(INT8 move) BANKED{
    cur_posi += move;
    cur_posi = cur_posi % 4;
    inv_cursor->x = cur_posx[cur_posi];
    inv_cursor->y = cur_posy[cur_posi];
}

void update_pcode(INT8 move) BANKED{
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
    if(pcode_data->tetradado_state != PASSWORD){
        pcode_data->tetradado_state = PASSWORD;
        pcode_data->tetradado_faccia = FACCIA_1;
    }
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