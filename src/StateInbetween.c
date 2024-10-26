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

IMPORT_MAP(inbetweenmap);

UINT8 new_state_to_go = 0u;
UINT8 countdown = 0u;

extern WHOSTALKING whostalking;
extern uint8_t sgb_running;
extern void manage_border(UINT8 my_next_state) BANKED;
extern void manage_bgm(UINT8 new_state, UINT8 previous_state, INT8 next_map) BANKED;

void ChangeStateThroughBetween(UINT8 new_state) BANKED;
void ChangeStateThroughBetween_TetraWinner() BANKED;
void ChangeStateThroughBetween_TetraLoser() BANKED;

void START(){
	InitScroll(BANK(inbetweenmap), &inbetweenmap, 0, 0);
            BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_WHITE, DMG_WHITE, DMG_WHITE);
    countdown = 2u;
}

void UPDATE(){
    switch(countdown){
        case 2u:
            if(sgb_running){
                manage_border(new_state_to_go);
            }
        break;
        case 0: 
            manage_bgm(new_state_to_go, StateInbetween, -1);
            SetState(new_state_to_go);        
        break;
    }
    countdown--;
}

void ChangeStateThroughBetween(UINT8 new_state) BANKED{
    new_state_to_go = new_state;
    HIDE_WIN;
    SetState(StateInbetween);
}
void ChangeStateThroughBetween_TetraWinner() BANKED{
    new_state_to_go = StateDialog;
    whostalking = PIRATE_CAPTAIN_3;
    HIDE_WIN;
    SetState(StateInbetween);
}
void ChangeStateThroughBetween_TetraLoser() BANKED{
    new_state_to_go = StateDialog;
    whostalking = PIRATE_CAPTAIN_2;
    HIDE_WIN;
    SetState(StateInbetween);
}