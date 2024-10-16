#include "Banks/SetAutoBank.h"

// #include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Palette.h"

#include "custom_datas_tetra.h"

extern TETRA_GAME_STATE tetra_game_state;
extern TETRA_TURN tetraturn;
extern Sprite* dado1;
extern Sprite* dado2;
extern Sprite* dado3;
extern Sprite* dado4;
extern Sprite* dado5;
extern Sprite* dado6;
extern UINT8 dadi_disponibili;
extern UINT8 player_chosen_dragons;
extern UINT8 J_FIRE;
extern UINT8 J_JUMP;
extern TETRA_CAMERA_STATE camera_state;

const UINT8 cursor_anim_hide[] = {1, 0}; //The first number indicates the number of frames
const UINT8 cursor_anim_triangle_full[] = {1, 2}; //The first number indicates the number of frames
const UINT8 cursor_anim_triangle_blink[] = {2, 2, 1}; //The first number indicates the number of frames
const UINT8 cursor_anim_triangle_empty[] = {1, 1}; //The first number indicates the number of frames
const UINT8 cursor_anim_hand_opened[] = {1, 3}; //The first number indicates the number of frames
const UINT8 cursor_anim_hand_closed[] = {1, 4}; //The first number indicates the number of frames
const UINT8 cursor_anim_buy[] = {2, 0,5}; //The first number indicates the number of frames
const UINT8 cursor_anim_cross[] = {2, 0,6}; //The first number indicates the number of frames

UINT8 cursor_dado_on = 1u;
UINT8 cursor_drago_on = 1u;

void move_cursor_hand_dice(UINT8 direction) BANKED;
void move_cursor_hand_dragon(UINT8 direction) BANKED;

extern void card_chosen(TETRA_DADO_FACCE chosen_face) BANKED;
extern UINT8 buy_dragon(UINT8 dragon_arg, UINT8 just_check) BANKED;
extern void tetra_change_game_state(TETRA_GAME_STATE new_game_state) BANKED;

void START(){
    THIS->lim_x = 800u;
    THIS->lim_y = 800u;
    SetSpriteAnim(THIS, cursor_anim_hide, 4u);
    struct TetracursorInfo* cursor_this_info = (struct TetracursorInfo*) THIS->custom_data;
    cursor_this_info->cursor_state = CURSOR_INVISIBLE;
    cursor_dado_on = 1u;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void UPDATE(){
    if(camera_state == READ_INSTRUCTIONS){return;}
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
            if(tetraturn != TURN_PLAYER){return;}
            switch(tetra_game_state){
                case TURN_PICK_DICE:
                    struct TetradadoInfo* dado6_info = (struct TetradadoInfo*) dado6->custom_data;
                    if(dado6_info->tetradado_state < DADO_FACE){
                    SetSpriteAnim(THIS, cursor_anim_hide, 4u);return;}
                    SetSpriteAnim(THIS, cursor_anim_hand_opened, 4u);
                    if(KEY_TICKED(J_RIGHT) || KEY_TICKED(J_LEFT)){
                        if(KEY_TICKED(J_RIGHT)){
                            move_cursor_hand_dice(J_RIGHT);
                        }
                        if(KEY_TICKED(J_LEFT)){
                            move_cursor_hand_dice(J_LEFT);
                        }
                    }else if(KEY_TICKED(J_A) || KEY_TICKED(J_B)){//voglio scegliere
                        switch(cursor_dado_on){
                            case 1u:
                                tetradado_sel_info = (struct TetradadoInfo*) dado1->custom_data;
                                if(tetradado_sel_info->tetradado_state == DADO_FACE){
                                    tetradado_sel_info->tetradado_state = DADO_SELECTED_PLAYER;
                                    card_chosen(tetradado_sel_info->tetradado_faccia);
                                }
                            break;
                            case 2u:
                                tetradado_sel_info = (struct TetradadoInfo*) dado2->custom_data;
                                if(tetradado_sel_info->tetradado_state == DADO_FACE){
                                    tetradado_sel_info->tetradado_state = DADO_SELECTED_PLAYER;
                                    card_chosen(tetradado_sel_info->tetradado_faccia);
                                }
                            break;
                            case 3u:
                                tetradado_sel_info = (struct TetradadoInfo*) dado3->custom_data;
                                if(tetradado_sel_info->tetradado_state == DADO_FACE){
                                    tetradado_sel_info->tetradado_state = DADO_SELECTED_PLAYER;
                                    card_chosen(tetradado_sel_info->tetradado_faccia);
                                }
                            break;
                            case 4u:
                                tetradado_sel_info = (struct TetradadoInfo*) dado4->custom_data;
                                if(tetradado_sel_info->tetradado_state == DADO_FACE){
                                    tetradado_sel_info->tetradado_state = DADO_SELECTED_PLAYER;
                                    card_chosen(tetradado_sel_info->tetradado_faccia);
                                }
                            break;
                            case 5u:
                                tetradado_sel_info = (struct TetradadoInfo*) dado5->custom_data;
                                if(tetradado_sel_info->tetradado_state == DADO_FACE){
                                    tetradado_sel_info->tetradado_state = DADO_SELECTED_PLAYER;
                                    card_chosen(tetradado_sel_info->tetradado_faccia);
                                }
                            break;
                            case 6u:
                                tetradado_sel_info = (struct TetradadoInfo*) dado6->custom_data;
                                if(tetradado_sel_info->tetradado_state == DADO_FACE){
                                    tetradado_sel_info->tetradado_state = DADO_SELECTED_PLAYER;
                                    card_chosen(tetradado_sel_info->tetradado_faccia);
                                }
                            break;
                        }
                    }
                break;
                case TURN_PICK_DRAGON:
                    if(KEY_TICKED(J_RIGHT) || KEY_TICKED(J_LEFT)){
                        if(KEY_TICKED(J_RIGHT)){
                            move_cursor_hand_dragon(J_RIGHT);
                        }
                        if(KEY_TICKED(J_LEFT)){
                            move_cursor_hand_dragon(J_LEFT);
                        }
                    }else if(KEY_TICKED(J_FIRE)){//pick dragon
                        tetra_change_game_state(TURN_BUY_DRAGON);
                    }else if(KEY_TICKED(J_JUMP)){
                        cursor_dado_on = 1;
                        SetSpriteAnim(THIS, cursor_anim_hand_opened, 4u);
                        tetra_change_game_state(TURN_MAKE_DICE);
                    }
                break;
                case TURN_BUY_DRAGON:
                    {                        
                        UINT8 can_buy = buy_dragon(cursor_drago_on, 0);
                        switch(can_buy){
                            case 1:
                                SetSpriteAnim(THIS, cursor_anim_buy, 24);
                                if(KEY_TICKED(J_FIRE)){//pick dragon
                                    buy_dragon(cursor_drago_on, 1);
                                    SetSpriteAnim(THIS, cursor_anim_hide, 4u);
                                }
                            break;
                            case 0://non posso comperarlo!
                                SetSpriteAnim(THIS, cursor_anim_cross, 24);
                                if(KEY_TICKED(J_FIRE)){
                                    tetra_change_game_state(TURN_PICK_DRAGON);
                                    SetSpriteAnim(THIS, cursor_anim_hand_opened, 4u);
                                }
                            break;
                        }
                        if(KEY_TICKED(J_JUMP)){
                            SetSpriteAnim(THIS, cursor_anim_hand_opened, 4u);
                            tetra_change_game_state(TURN_PICK_DRAGON);
                        }
                    }
                break;
            }   
        break;
	    case HAND_CLOSED:
            SetSpriteAnim(THIS, cursor_anim_hand_closed, 4u);
        break;
    }
}

void move_cursor_hand_dragon(UINT8 direction) BANKED{
    switch(direction){
        case J_RIGHT:
            if(cursor_drago_on == 3){
                cursor_drago_on = 1;
            }else{
                cursor_drago_on++;
            }
        break;
        case J_LEFT:
            if(cursor_drago_on == 1){
                cursor_drago_on = 3;
            }else{
                cursor_drago_on--;
            }
        break;
    }
    switch(cursor_drago_on){
        case 1u:
            THIS->x = ((UINT16) 3u << 3);
            THIS->y = ((UINT16) 6u << 3);
        break;
        case 2u:
            THIS->x = ((UINT16) 9u << 3);
            THIS->y = ((UINT16) 6u << 3);
        break;
        case 3u:
            THIS->x = ((UINT16) 15u << 3);
            THIS->y = ((UINT16) 6u << 3);
        break;
    }
}

void move_cursor_hand_dice(UINT8 direction) BANKED{
    switch(direction){
        case J_RIGHT:
            if(cursor_dado_on == 6){
                cursor_dado_on = 1;
            }else{
                cursor_dado_on++;
            }
        break;
        case J_LEFT:
            if(cursor_dado_on == 1){
                cursor_dado_on = 6;
            }else{
                cursor_dado_on--;
            }
        break;
    }
    UINT8 dado_taken = 0u;
    struct TetradadoInfo* tetradado_sel_info = 0;
    switch(cursor_dado_on){
        case 1u:tetradado_sel_info = (struct TetradadoInfo*) dado1->custom_data;break;
        case 2u:tetradado_sel_info = (struct TetradadoInfo*) dado2->custom_data;break;
        case 3u:tetradado_sel_info = (struct TetradadoInfo*) dado3->custom_data;break;
        case 4u:tetradado_sel_info = (struct TetradadoInfo*) dado4->custom_data;break;
        case 5u:tetradado_sel_info = (struct TetradadoInfo*) dado5->custom_data;break;
        case 6u:tetradado_sel_info = (struct TetradadoInfo*) dado6->custom_data;break;
    }
    if(tetradado_sel_info->tetradado_state != DADO_FACE){dado_taken = 1;}
    if(dado_taken == 1){move_cursor_hand_dice(direction);return;}
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
}
void DESTROY(){

}