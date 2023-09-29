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
extern void manage_bgm(UINT8 new_state, UINT8 previous_state) BANKED;
extern void check_sgb_palette(UINT8 new_state) BANKED;

void ChangeStateThroughBetween(UINT8 new_state) BANKED;

void START(){
	InitScroll(BANK(inbetweenmap), &inbetweenmap, 0, 0);
}

void UPDATE(){
    countdown--;
    if(countdown == 0u){
        manage_bgm(new_state_to_go, StateInbetween);
        SetState(new_state_to_go);
    }
}

void ChangeStateThroughBetween(UINT8 new_state) BANKED{
    new_state_to_go = new_state;
    countdown = 60u;
    HIDE_WIN;
    check_sgb_palette(new_state);
    SetState(StateInbetween);
}