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

IMPORT_MAP(inbetweenmap);

UINT8 new_state_to_go = 0u;
UINT8 countdown = 0u;

void ChangeStateThroughBetween(UINT8 new_state) BANKED;

void START(){
	InitScroll(BANK(inbetweenmap), &inbetweenmap, 0, 0);
}

void UPDATE(){
    countdown--;
    if(countdown == 0u){
        SetState(new_state_to_go);
    }
}

void ChangeStateThroughBetween(UINT8 new_state) BANKED{
    new_state_to_go = new_state;
    countdown = 60u;
    HIDE_WIN;
    SetState(StateInbetween);
}