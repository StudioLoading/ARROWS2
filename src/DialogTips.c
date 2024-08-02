#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"

#include "DialogTips.h"

extern unsigned char d0[22];
extern unsigned char d1[22];
extern unsigned char d2[22];
extern unsigned char d3[22];
extern unsigned char d4[22];
extern unsigned char EMPTY_STRING_21[22];

void GetLocalizedTip_EN(TIP_TO_BE_LOCALIZED tip) BANKED{
	switch(tip){
		case TIP_HIDDEN_ARROWS:
			memcpy(d1, " SOMETHING'S ON THE  ", 22);
			memcpy(d2, " GROUND:             ", 22);
			memcpy(d3, "           ARROWS!   ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_HIDDEN_METAL:
			memcpy(d1, " SOMETHING'S ON THE  ", 22);
			memcpy(d2, " GROUND:             ", 22);
			memcpy(d3, "          METAL!     ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_HIDDEN_WOOD:
			memcpy(d1, " SOMETHING'S ON THE  ", 22);
			memcpy(d2, " GROUND:             ", 22);
			memcpy(d3, "           WOOD!     ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_HIDDEN_MONEY:
			memcpy(d1, " SOMETHING'S ON THE  ", 22);
			memcpy(d2, " GROUND:             ", 22);
			memcpy(d3, "           COINS !   ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_PRESS_SELECT:
			memcpy(d1, " PRESS SELECT TO    ", 22);
			memcpy(d2, " VIEW MY MISSION'S  ", 22);
			memcpy(d3, " DIARY FOR THE      ", 22);
			memcpy(d4, " CURRENT CHAPTER.   ", 22);
		break;
		case TIP_SMITH_NO:
			memcpy(d3, EMPTY_STRING_21 , 22);
			memcpy(d1, " THE SMITH IS YET TO ", 22);
			memcpy(d2, " COME.               ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_HOSPITAL_NO:
			memcpy(d1, " THIS IS THE         ", 22);
			memcpy(d2, " HOSPITAL.           ", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_MINE_CAVE:
			memcpy(d1, " THIS IS THE OLD     ", 22);
			memcpy(d2, " MINE. HERE I FIND   ", 22);
			memcpy(d3, " METAL AND WOOD.     ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_BLACKIE_CAVE:
			memcpy(d1, "  RED RIDING HOOD   ", 22);
			memcpy(d2, "  WORST ENEMY CAVE  ", 22);
			memcpy(d3, EMPTY_STRING_21 , 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_OWLIMIT_EAST:
			memcpy(d1, " IT IS NOT SAFE TO   \0", 22);
			memcpy(d2, " GO THROUGH THIS     \0", 22);
			memcpy(d3, " FOREST YET.         \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_OWLIMIT_SOUTH:
			memcpy(d1, " SON- WHERE ARE YOU? \0" , 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_STILL_SOMETHING:
			memcpy(d1, " I HAVE TO DO SOMETH \0", 22);
			memcpy(d2, " ING ELSE HERE       \0", 22);
			memcpy(d3, " BEFORE LEAVING      \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_DARK_FOREST:
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "  THE DARKEST FOREST ", 22);
			memcpy(d3, EMPTY_STRING_21 , 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_LABIRYNTH:
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d1, "      NORTHERN      ", 22);
			memcpy(d2, "        MAZE        ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_GROTTO:
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "    OUTWALKER CAMP   \0", 22);
			memcpy(d3, EMPTY_STRING_21 , 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_FISHERMAN:
			memcpy(d3, EMPTY_STRING_21 , 22);
			memcpy(d1, " THE FISHERMAN       ", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_SILVER_MINE:
			memcpy(d3, EMPTY_STRING_21 , 22);
			memcpy(d1, " SILVER MINE -       ", 22);
			memcpy(d2, "BEWARE THE MINOTAUR! ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_CHITCHAT_HUNGRY:
			memcpy(d3, EMPTY_STRING_21 , 22);
			memcpy(d1, " OH PLEASE BRING US  ", 22);
			memcpy(d2, " SOME FOOD!          ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_CHITCHAT_SMITH:
			memcpy(d1, " THE FORGE IS THE BU ", 22);
			memcpy(d2, " ILDING WITH AN ANV  ", 22);
			memcpy(d3, " IL SIGN POSTED      ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_HOSP:
			memcpy(d1, " YOU CAN GET HEALED  ", 22);
			memcpy(d2, " AT THE HOSPITAL BUT ", 22);
			memcpy(d3, " IT WILL COSTS YOU   ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_ITEMS:
			memcpy(d1, " GET ITEM LOOTS GET  ", 22);
			memcpy(d2, " TING WILD ANIMALS   ", 22);
			memcpy(d3, " DOWN!               ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_SALUTE:
			memcpy(d1, " SALUTE HEALER! OUR  ", 22);
			memcpy(d2, " FAMILY WISHES YOU   ", 22);
			memcpy(d3, " A VERY GOOD LUCK!   ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_HIDDEN_00:
			memcpy(d1, " LOOK IN FRONT AT    ", 22);
			memcpy(d2, " THE CEMETERY ENTRAN ", 22);
			memcpy(d3, " CE SOMETHING'S THERE", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_HIDDEN_01:
			memcpy(d1, " LOOK TO THE RIGHT OF", 22);
			memcpy(d2, " THE HOSPITAL.       ", 22);
			memcpy(d3, " SOMETHING'S THERE.  ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_HIDDEN_02:
			memcpy(d1, " THEY SAY THE CARPE ", 22);
			memcpy(d2, " NTER HAS LEFT SOME ", 22);
			memcpy(d3, " WOOD IN THE SOUTH.	", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_LABYRINTH:
			memcpy(d1, " HAVE YOU CHECKED   ", 22);
			memcpy(d2, " THE UNDERGROUND    ", 22);
			memcpy(d3, " MAZE YET?          ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_GLASS:
			memcpy(d1, " YOU KNOW THE SMITH ", 22);
			memcpy(d2, " IS ABLE TO PRODUCE ", 22);
			memcpy(d3, " GLASS? WONDERFUL!  ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_MARGARET:
			memcpy(d1, " MARGARET LOOKS BET ", 22);
			memcpy(d2, " TER SINCE HER CHI  ", 22);
			memcpy(d3, " LD'S RETURNED.     ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_MOUNTAINS:
			memcpy(d1, " THE PATH TO THE MO ", 22);
			memcpy(d2, " UNTAINS STARTS FRO ", 22);
			memcpy(d3, " M OUTWALKER'S CAMP ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_LIAM:
			memcpy(d1, " YOUR SON LIAM      ", 22);
			memcpy(d2, " FOUGHT GLORIOUSLY! ", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_HIDDEN_10:
			memcpy(d1, " I SAW SOME READY   ", 22);
			memcpy(d2, " TO USE ARROWS AT   ", 22);
			memcpy(d3, " THE BEACH! GO GET! ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
		case TIP_CHITCHAT_HIDDEN_11:
			memcpy(d1, " SOME GUY AT THE INN", 22);
			memcpy(d2, " SAID THERE ARE SOME", 22);
			memcpy(d3, " ARROWS IN THE MAZE ", 22);
			memcpy(d4, EMPTY_STRING_21 , 22);
		break;
	}
}