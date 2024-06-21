#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"

#include "Dialogs.h"
#include "Dialogs2.h"
#include "Dialogs3.h"
#include "custom_datas.h"

extern unsigned char log0[21];
extern unsigned char m0[17];
extern unsigned char m1[17];
extern unsigned char m2[17];
extern unsigned char m3[17];
extern unsigned char d0[22];
extern unsigned char d1[22];
extern unsigned char d2[22];
extern unsigned char d3[22];
extern unsigned char d4[22];
extern unsigned char d5[22];
extern unsigned char d6[22];
extern unsigned char d7[22];
extern unsigned char d8[22];
extern unsigned char d9[22];
extern unsigned char d10[22];
extern unsigned char d11[22];
extern unsigned char d12[22];
extern unsigned char d13[22];
extern unsigned char d14[22];
extern unsigned char d15[22];
extern unsigned char d16[22];
extern unsigned char d17[22];
extern unsigned char d18[22];
extern unsigned char d19[22];
extern WHOSTALKING whostalking;
extern unsigned char EMPTY_STRING_21[22];
extern unsigned char D0[22];
extern UINT8 choice;
extern UINT8 choice_left;
extern UINT8 choice_right;
extern INT8 outwalker_info_given;
extern struct MISSION outwalker_chief;
extern struct MISSION outwalker_glass;
extern struct MISSION outwalker_smith;
extern struct MISSION find_antidote;
extern struct MISSION hungry_people;
extern struct MISSION golden_armor;
extern struct MISSION fix_bridge;

extern void pickup(struct ItemSpawned* pickedup_data) BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;
extern UINT8 get_quantity(INVITEMTYPE itemtype) BANKED;

void GetLocalizedDialog3_EN(UINT8* n_lines) BANKED{
    switch(whostalking){		
		case MINOTAUR_ENTRANCE:
			*n_lines = 13u;
			memcpy(d0, "DESSA:              ", 22);
			memcpy(d1, "IT'S A LOT COLDER   ", 22);
			memcpy(d2, "IN HERE, MY ARROWS  ", 22);
			memcpy(d3, "MAY FREEZE...       ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);	
			memcpy(d5, "I CAN SEE A TALL AND", 22);
			memcpy(d6, "DARK FIGURE IN THE  ", 22);
			memcpy(d7, "NEXT ROOM.IT MUST BE", 22);
			memcpy(d8, "THE MINOTAUR PETER  ", 22);
			memcpy(d9, "TALKED ME ABOUT.    ", 22);
			memcpy(d10, EMPTY_STRING_21, 22);	
			memcpy(d11, "SHOULD I FACE HIM?   ", 22);
			memcpy(d12, EMPTY_STRING_21, 22);
			memcpy(d13, " NO          YES    \0", 22);
			choice = 1u;
			choice_left = 0u;
			choice_right = 0u;
		break;
		case MINOTAUR_DEFEAT:
			*n_lines = 17u;
			memcpy(d0, EMPTY_STRING_21, 22);
			memcpy(d1, "MY BROTHER TOLD ME ", 22);
			memcpy(d2, "YOUR FAMILY WAS    ", 22);
			memcpy(d3, "STRONG....         ", 22);
			memcpy(d4, "ONLY ONE HUMAN     ", 22);
			memcpy(d5, "UNMASKED ME ONCE...", 22);
			memcpy(d6, "BUT THIS IS ANOTHER", 22);
			memcpy(d7, "STORY. YOU FOUND A ", 22);
			memcpy(d8, "NEW FRIEND IN ME.  ", 22);
			memcpy(d9, "AND NOW MY SKULL   ", 22);
			memcpy(d10, "IS YOURS. DO WHAT ", 22);
			memcpy(d11, "EVER YOU WANT.    ", 22);
			memcpy(d12, EMPTY_STRING_21, 22);
			memcpy(d13, "DESSA: YOUR SKULL ", 22);
			memcpy(d14, "IS MADE OF SILVER ", 22);
			memcpy(d15, "AND GOLD: I WILL  ", 22);
			memcpy(d16, "USE IT TO FORGE A ", 22);
			memcpy(d17, "GOLDEN ARMOR.     ", 22);
		break;
    }
}