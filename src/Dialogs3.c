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
extern struct MISSION broken_ship;
extern struct MISSION pirate_strike;
extern struct MISSION captain;

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
			broken_ship.mission_state = MISSION_STATE_ENABLED;
		break;
		case PIRATE_SPUGNA_3:
			*n_lines = 4u;
			memcpy(d0, "MR SMEE:            ", 22);
			memcpy(d1, "YOU HAVE MY SUPPORT ", 22);
			memcpy(d2, "I AM DOING THE BEST ", 22);
			memcpy(d3, "I CAN TO SET THE    ", 22);
			memcpy(d4, "SAIL!               ", 22);
		break;
		case PIRATE_SPUGNA_STRIKE:
			*n_lines = 2u;
			memcpy(d0, "MR SMEE:            ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "WE ARE ON STRIKE!   ", 22);
		break;
		case PIRATE_PANZONE_0:
			*n_lines = 5u;
			memcpy(d0, "BOB:                ", 22);
			memcpy(d1, "THE SHIP IS DAMAGED!", 22);
			memcpy(d2, "PLEASE PROVIDE ME OF", 22);
			memcpy(d3, "50 WOOD PIECES.     ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "THANK YOU.          ", 22);
            if(broken_ship.mission_state < MISSION_STATE_STARTED){
				broken_ship.mission_state = MISSION_STATE_STARTED;
			}
		break;
		case PIRATE_PANZONE_1:
			*n_lines = 4u;
			memcpy(d0, "BOB:                ", 22);
			memcpy(d1, "50 PIECES OF WOOD!  ", 22);
			memcpy(d2, "THANK YOU HEALER! WE", 22);
			memcpy(d3, "ARE NOW ABLE TO FIX ", 22);
			memcpy(d4, "THAT SHIP.          ", 22);
            if(broken_ship.mission_state == MISSION_STATE_STARTED){
				broken_ship.mission_state = MISSION_STATE_REWARDED;
				pirate_strike.mission_state = MISSION_STATE_ENABLED;
			}
		break;
		case PIRATE_PANZONE_2:
			*n_lines = 4u;
			memcpy(d0, "BOB:                ", 22);
			memcpy(d1, "AS SOON AS THE SHIP ", 22);
			memcpy(d2, "IS FIXED I THINK WE ", 22);
			memcpy(d3, "WILL BE READY TO    ", 22);
			memcpy(d4, "SAIL.               ", 22);
		break;
		case PIRATE_PANZONE_STRIKE:
			*n_lines = 2u;
			memcpy(d0, "BOB:                ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "WE ARE ON STRIKE!   ", 22);
		break;
		case PIRATE_MARTIN_0:
			*n_lines = 7u;
			memcpy(d0, "MARTIN:             ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "BEING HIGH HELPS A  ", 22);
			memcpy(d3, "LOT! LIVING THIS    ", 22);
			memcpy(d4, "WORLD IS TOUGH...   ", 22);
			memcpy(d5, "SOME OF US NEED AN  ", 22);
			memcpy(d6, "ESCAPE... SOMETIME  ", 22);
			memcpy(d7, "... YOU KNOW...     ", 22);
		break;
		case PIRATE_MARTIN_1:
			*n_lines = 15u;
			memcpy(d0, "MARTIN:             ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "WE ARE ON STRIKE!   ", 22);
			memcpy(d3, "WE WANNA GET HIGH!  ", 22);
			memcpy(d4, "THE DOPE IS FINISHED", 22);
			memcpy(d5, "AND WE WANT MORE!   ", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "OF COURSE CAPTAIN IS", 22);
			memcpy(d8, "AGAINST IT, BUT WE  ", 22);
			memcpy(d9, "WILL NOT SAIL UNTIL ", 22);
			memcpy(d10, "WE HAVE WHAT WE WANT", 22);
			memcpy(d11, EMPTY_STRING_21, 22);
			memcpy(d12, "CAN YOU CHECK THE   ", 22);
			memcpy(d13, "DARKEST FOREST? SOME", 22);
			memcpy(d14, "SAY INGREDIENTS MAY ", 22);
			memcpy(d15, "BE FOUND THERE      ", 22);
		break;
		case PIRATE_MARTIN_2:
			*n_lines = 7u;
			memcpy(d0, "MARTIN:             ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "THANKS MADAME. WE   ", 22);
			memcpy(d3, "WILL NOT TALK, BE   ", 22);
			memcpy(d4, "SURE OF THAT.       ", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "IF CAPTAIN ORDER US ", 22);
			memcpy(d7, "TO SAIL, WE SAIL.   ", 22);
		break;
		case PIRATE_MARTIN_STRIKE:
			*n_lines = 2u;
			memcpy(d0, "MARTIN:             ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "WE ARE ON STRIKE!   ", 22);
		break;
		case PIRATE_CAPTAIN_0:
			*n_lines = 11u;
			memcpy(d0, "ONE EYED JACK:      ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "ARRRRRR! NAME IS    ", 22);
			memcpy(d3, "JACK! I AM THE CAP  ", 22);
			memcpy(d4, "TAIN OF SIREN, MY   ", 22);
			memcpy(d5, "GALLEON!            ", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "THINK MY MEN HAVE   ", 22);
			memcpy(d8, "SOME BUSINESS FOR   ", 22);
			memcpy(d9, "YA. FIX THEM, THEN  ", 22);
			memcpy(d10, "WE CAN TALK ABOUT  ", 22);
			memcpy(d11, "SAILING.           ", 22);
		break;
		case PIRATE_CAPTAIN_1:
			*n_lines = 11u;
			memcpy(d0, "ONE EYED JACK:      ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "LOOKS LIKE YOU ARE  ", 22);
			memcpy(d3, "READY FOR AN ADVENTU", 22);
			memcpy(d4, "RE. NEED HELP UH?   ", 22);
			memcpy(d4, "THE ISLAND IS NOT   ", 22);
			memcpy(d4, "TOO FAR FROM HERE.  ", 22);
			memcpy(d4, "BUT THE TEMPEST...  ", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "WE SHOULD WAIT FOR  ", 22);
			memcpy(d7, "IT TO CALM DOWN...  ", 22);
			memcpy(d8, "MEANWHILE, I DARE   ", 22);
			memcpy(d9, "YOU BEAT ME ON TETRA", 22);
			memcpy(d10, ", IT IS MY FAV!     ", 22);
			memcpy(d11, "ARE YOU READY?      ", 22);
            captain.mission_state = MISSION_STATE_ENABLED;
		break;
		case PIRATE_CAPTAIN_2:
			*n_lines = 4u;
			memcpy(d0, "ONE EYED JACK:      ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "COME BACK WHEN YOU  ", 22);
			memcpy(d3, "FEEL READY TO TRY   ", 22);
			memcpy(d4, "AGAIN!              ", 22);
		break;
		case PIRATE_CAPTAIN_3:
			*n_lines = 10u;
			memcpy(d0, "ONE EYED JACK:      ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "AH! YOU FINALLY BEAT", 22);
			memcpy(d3, "ME! ALRIGHT ALRIGHT.", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "DEFEATED BY A WOMAN!", 22);
			memcpy(d6, "WHAT I'VE BECOME?!  ", 22);
			memcpy(d7, EMPTY_STRING_21, 22);
			memcpy(d8, "THE STORM HAS GONE  ", 22);
			memcpy(d9, "NOW, WE CAN SAIL.   ", 22);
			memcpy(d10, "MEN!!!! LET'S GO!   ", 22);
            captain.mission_state = MISSION_STATE_REWARDED;
		break;
		case CADAVER://se devo ancora fare bossfight oppure no
			if(mr_smee.mission_state == MISSION_STATE_STARTED){
				*n_lines = 12u;
				memcpy(d0, "DESSA  :            ", 22);
				memcpy(d1, "IT'S A HEADLESS     ", 22);
				memcpy(d2, "BODY! WHAT AN END   ", 22);
				memcpy(d3, "FOR A LIFE...       ", 22);
				memcpy(d4, EMPTY_STRING_21, 22);
				memcpy(d5, "WHAT COULD HAVE     ", 22);
				memcpy(d6, "POSSIBLY CAUSED...  ", 22);
				memcpy(d7, EMPTY_STRING_21, 22);
				memcpy(d8, EMPTY_STRING_21, 22);
				memcpy(d9, EMPTY_STRING_21, 22);
				memcpy(d10, "*FLAP! FLAP! FLAP!* ", 22);
				memcpy(d11, EMPTY_STRING_21, 22);
				memcpy(d12, "YYAAAAAAAAAAAAAAAH! ", 22);
				mr_smee.current_step = 1;//means start boss fight!
			}else{
				*n_lines = 5u;
				memcpy(d0, "DESSA  :            ", 22);
				memcpy(d1, "HERE LIES RICK,     ", 22);
				memcpy(d2, "HEADLESS RICK. NOW  ", 22);
				memcpy(d3, "HE'S FOOD FOR BEASTS", 22);
				memcpy(d4, EMPTY_STRING_21, 22);
				memcpy(d5, "POOR RICK!          ", 22);
			}
		break;
    }
}