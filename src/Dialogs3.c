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
extern struct MISSION mr_smee;
extern struct MISSION captain;

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
		case HARBOR_TOOSOON:
			*n_lines = 4u;
			memcpy(d0, "THE HARBOR:        ", 22);
			memcpy(d1, "IT IS TOO SOON FOR ", 22);
			memcpy(d2, "ME TO LEAVE. HERE  ", 22);
			memcpy(d3, "THERE IS STILL A   ", 22);
			memcpy(d4, "LOT TO DO!         ", 22);
		break;
		case PIRATE_SPUGNA_0:
			*n_lines = 19u;
			memcpy(d0, "MR SMEE:            ", 22);
			memcpy(d1, "HELLO DESSA! WE HAVE", 22);
			memcpy(d2, "HEARD YOU NEED US TO", 22);
			memcpy(d3, "CROSS THE SEA. WELL ", 22);
			memcpy(d4, "WE CAN DO THAT, BUT ", 22);
			memcpy(d5, "... SOME OF US MAY  ", 22);
			memcpy(d6, "NEED YOUR HELP FOR  ", 22);
			memcpy(d7, "SOME BUSINESSES...  ", 22);
			memcpy(d8, EMPTY_STRING_21, 22);
			memcpy(d9, "THE SHIP IS BROKEN, ", 22);
			memcpy(d10, "MAURICE HAS LOST A ", 22);
			memcpy(d11, "FRIEND, WE ARE CUR ", 22);
			memcpy(d12, "RENTLY ON A STRIKE ", 22);
			memcpy(d13, "AND THE CAPTAIN IS ", 22);
			memcpy(d14, "DEPRESSED BECAUSE  ", 22);
			memcpy(d15, "HE HAS NO ONE TO   ", 22);
			memcpy(d16, "PLAY TETRA WITH.   ", 22);
			memcpy(d17, EMPTY_STRING_21, 22);
			memcpy(d18, "WELCOME TO THE     ", 22);
			memcpy(d19, "HARBOR!            ", 22);
            mr_smee.mission_state = MISSION_STATE_ENABLED;
		break;
		case PIRATE_SPUGNA_1:
			*n_lines = 8u;
			memcpy(d0, "MR SMEE:            ", 22);
			memcpy(d1, "ABOUT ME... MY FRIE ", 22);
			memcpy(d2, "ND RICK WAS HUNTED  ", 22);
			memcpy(d3, "SOME BATS IN A CAVE.", 22);
			memcpy(d4, "FACT IS: IT'S BEEN  ", 22);
			memcpy(d5, "THREE DAYS SINCE WE ", 22);
			memcpy(d6, "'VE SEEN HIM. CAN   ", 22);
			memcpy(d7, "YOU KINDLY HAVE A   ", 22);
			memcpy(d8, "CHECK ON THAT CAVE? ", 22);
            if(mr_smee.mission_state < MISSION_STATE_STARTED){
				mr_smee.mission_state = MISSION_STATE_STARTED;
			}
		break;
		case PIRATE_SPUGNA_2:
			*n_lines = 4u;
			memcpy(d0, "MR SMEE:            ", 22);
			memcpy(d1, "OH, I SEE... I AM   ", 22);
			memcpy(d2, "SORRY FOR HIM, BUT  ", 22);
			memcpy(d3, "THIS IS HOW THE WOR ", 22);
			memcpy(d4, "LD WORKS NOW.       ", 22);
            mr_smee.mission_state = MISSION_STATE_REWARDED;
		break;
		case PIRATE_SPUGNA_3:
			*n_lines = 4u;
			memcpy(d0, "MR SMEE:            ", 22);
			memcpy(d1, "YOU HAVE MY SUPPORT ", 22);
			memcpy(d2, "I AM DOING THE BEST ", 22);
			memcpy(d3, "I CAN TO SET THE    ", 22);
			memcpy(d4, "SAIL!               ", 22);
		break;
		case PIRATE_CAPTAIN_0:
            captain.mission_state = MISSION_STATE_ENABLED;
		break;
		case PIRATE_CAPTAIN_1:
            captain.mission_state = MISSION_STATE_STARTED;
		break;
    }
}