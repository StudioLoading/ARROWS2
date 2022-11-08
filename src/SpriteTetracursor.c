#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "Scroll.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern TETRA_GAME_STATE tetra_game_state;
extern TETRA_TURN tetraturn;
extern Sprite* dado1;
extern Sprite* dado2;
extern Sprite* dado3;
extern Sprite* dado4;
extern Sprite* dado5;
extern Sprite* dado6;
extern UINT8 dadi_disponibili;

const UINT8 cursor_anim_hide[] = {1, 2}; //The first number indicates the number of frames
const UINT8 cursor_anim_triangle_full[] = {1, 0}; //The first number indicates the number of frames
const UINT8 cursor_anim_triangle_blink[] = {2, 0, 1}; //The first number indicates the number of frames
const UINT8 cursor_anim_triangle_empty[] = {1, 1}; //The first number indicates the number of frames
const UINT8 cursor_anim_hand_opened[] = {1, 3}; //The first number indicates the number of frames
const UINT8 cursor_anim_hand_closed[] = {1, 4}; //The first number indicates the number of frames

UINT8 cursor_dado_on = 1u;

void START(){
    THIS->lim_x = 80u;
    THIS->lim_y = 80u;
    SetSpriteAnim(THIS, cursor_anim_hide, 4u);
    struct TetracursorInfo* cursor_this_info = (struct TetracursorInfo*) THIS->custom_data;
    cursor_this_info->cursor_state = CURSOR_INVISIBLE;
    cursor_dado_on = 1u;
}

void UPDATE(){
    struct TetracursorInfo* cursor_info = (struct TetracursorInfo*) THIS->custom_data;
    struct TetradadoInfo* tetradado_sel_info = 0;
    switch(cursor_info->cursor_state){
        case CURSOR_INVISIBLE:
            SetSpriteAnim(THIS, cursor_anim_hide, 4u);
        break;
	    case TRIANGLE_BLINK:
            SetSpriteAnim(THIS, cursor_anim_triangle_blink, 16u);
        break;
	    case TRIANGLE_FULL:
            SetSpriteAnim(THIS, cursor_anim_triangle_full, 4u);
	    break;
        case TRIANGLE_EMPTY:
            SetSpriteAnim(THIS, cursor_anim_triangle_empty, 4u);
	    break;
        case HAND_OPENED:
            SetSpriteAnim(THIS, cursor_anim_hand_opened, 4u);
            if(tetra_game_state == TURN_PICK_DICE && tetraturn == TURN_PLAYER) {     
                if(KEY_TICKED(J_RIGHT)){
                    if(cursor_dado_on == 6){
                        cursor_dado_on = 1;
                    }else{
                        cursor_dado_on++;
                    }
                    switch(cursor_dado_on){
                        case 1u:
                            THIS->x = dado1->x;
                            THIS->y = dado1->y + 16u;
                        break;
                        case 2u:
                            THIS->x = dado2->x;
                            THIS->y = dado2->y + 16u;
                        break;
                        case 3u:
                            THIS->x = dado3->x;
                            THIS->y = dado3->y + 16u;
                        break;
                        case 4u:
                            THIS->x = dado4->x;
                            THIS->y = dado4->y + 16u;
                        break;
                        case 5u:
                            THIS->x = dado5->x;
                            THIS->y = dado5->y + 16u;
                        break;
                        case 6u:
                            THIS->x = dado6->x;
                            THIS->y = dado6->y + 16u;
                        break;
                    }
                }else if(KEY_TICKED(J_A) || KEY_TICKED(J_B)){
                    //voglio scegliere
                    switch(cursor_dado_on){
                        case 1u:
                            tetradado_sel_info = (struct TetradadoInfo*) dado1->custom_data;
                            if(tetradado_sel_info->tetradado_state == DADO_FACE){
                                tetradado_sel_info->tetradado_state = DADO_SELECTED_PLAYER;
                                tetraturn = TURN_ENEMY;
                                dadi_disponibili--;
                            }
                        break;
                        case 2u:
                            tetradado_sel_info = (struct TetradadoInfo*) dado2->custom_data;
                            if(tetradado_sel_info->tetradado_state == DADO_FACE){
                                tetradado_sel_info->tetradado_state = DADO_SELECTED_PLAYER;
                                tetraturn = TURN_ENEMY;
                                dadi_disponibili--;
                            }
                        break;
                        case 3u:
                            tetradado_sel_info = (struct TetradadoInfo*) dado3->custom_data;
                            if(tetradado_sel_info->tetradado_state == DADO_FACE){
                                tetradado_sel_info->tetradado_state = DADO_SELECTED_PLAYER;
                                tetraturn = TURN_ENEMY;
                                dadi_disponibili--;
                            }
                        break;
                        case 4u:
                            tetradado_sel_info = (struct TetradadoInfo*) dado4->custom_data;
                            if(tetradado_sel_info->tetradado_state == DADO_FACE){
                                tetradado_sel_info->tetradado_state = DADO_SELECTED_PLAYER;
                                tetraturn = TURN_ENEMY;
                                dadi_disponibili--;
                            }
                        break;
                        case 5u:
                            tetradado_sel_info = (struct TetradadoInfo*) dado5->custom_data;
                            if(tetradado_sel_info->tetradado_state == DADO_FACE){
                                tetradado_sel_info->tetradado_state = DADO_SELECTED_PLAYER;
                                tetraturn = TURN_ENEMY;
                                dadi_disponibili--;
                            }
                        break;
                        case 6u:
                            tetradado_sel_info = (struct TetradadoInfo*) dado6->custom_data;
                            if(tetradado_sel_info->tetradado_state == DADO_FACE){
                                tetradado_sel_info->tetradado_state = DADO_SELECTED_PLAYER;
                                tetraturn = TURN_ENEMY;
                                dadi_disponibili--;
                            }
                        break;
                    }
                }
            }        
        break;
	    case HAND_CLOSED:
            SetSpriteAnim(THIS, cursor_anim_hand_closed, 4u);
        break;
    }

}

void DESTROY(){

}