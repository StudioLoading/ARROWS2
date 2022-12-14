#include "Banks/SetAutoBank.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "string.h"
#include "Music.h"
#include "Scroll.h"

#include "InventoryDialogs.h"

unsigned char ddinv1[12];
unsigned char ddinv2[12];
unsigned char ddinv3[12];
unsigned char ddinv4[12];
unsigned char ddinv5[12];
unsigned char ddinv6[12];
unsigned char ddinv7[12];
unsigned char ddinv8[12];


void GetLocalizedINVLabel_EN(INV_TO_BE_LOCALIZED label, char* d) BANKED{
	switch(label){
		case INV_EMPTY_STRING:
			memcpy(d, "           ", 12);
		break;
		case CROSSBOW_NAME:
			memcpy(d, "CROSSBOW:  ", 12);
		break;
		case CROSSBOW_DETAIL1:
			memcpy(d, "SHORT-     ", 12);
		break;
		case CROSSBOW_DETAIL2:
			memcpy(d, "LIGHWEIGHT ", 12);
		break;
		case CROSSBOW_DETAIL3:
			memcpy(d, "WOODEN HAND", 12);
		break;
		case CROSSBOW_DETAIL4:
			memcpy(d, "MADE.     ", 12);
		break;
		case MONEY_NAME:
			memcpy(d, " COINS:    ", 12);
		break;
		case MONEY_DETAIL1:
			memcpy(d, "SOME GOLDEN", 12);
		break;
		case MONEY_DETAIL2:
			memcpy(d, "COINS ARE  ", 12);
		break;
		case MONEY_DETAIL3:
			memcpy(d, "USEFUL IN  ", 12);
		break;
		case MONEY_DETAIL4:
			memcpy(d, "THIS WORLD.", 12);
		break;
		case BOMB_NAME:
			memcpy(d, " BOMB:     ", 12);
		break;
		case BOMB_DETAIL1:
			memcpy(d, "AS SOON AS ", 12);
		break;
		case BOMB_DETAIL2:
			memcpy(d, "IT CRASHES ", 12);
		break;
		case BOMB_DETAIL3:
			memcpy(d, "THE FLOOR- ", 12);
		break;
		case BOMB_DETAIL4:
			memcpy(d, "IT EXPLODES", 12);
		break;
		case ARROWNORMAL_NAME:
			memcpy(d, "  NORMAL:  ", 12);
		break;
		case ARROWNORMAL_DETAIL1:
			memcpy(d, "MOST COMMON", 12);
		break;
		case ARROWNORMAL_DETAIL2:
			memcpy(d, "ARROWS I   ", 12);
		break;
		case ARROWNORMAL_DETAIL3:
			memcpy(d, "CAN FIND.  ", 12);
		break;
		case ARROWNORMAL_DETAIL4:
			memcpy(d, "           ", 12);
		break;
		case ARROWPERFO_NAME:
			memcpy(d, "  PIERCING:", 12);
		break;
		case ARROWPERFO_DETAIL1:
			memcpy(d, "NO LIVING  ", 12);
		break;
		case ARROWPERFO_DETAIL2:
			memcpy(d, "CREATURE   ", 12);
		break;
		case ARROWPERFO_DETAIL3:
			memcpy(d, "CAN STOP   ", 12);
		break;
		case ARROWPERFO_DETAIL4:
			memcpy(d, "THESE.     ", 12);
		break;
		case ARROWBASTARD_NAME:
			memcpy(d, "  BASTARD: ", 12);
		break;
		case ARROWBASTARD_DETAIL1:
			memcpy(d, "MOST POWER", 12);
		break;
		case ARROWBASTARD_DETAIL2:
			memcpy(d, "FUL ARROWS", 12);
		break;
		case ARROWBASTARD_DETAIL3:
			memcpy(d, "I CAN FIND", 12);
		break;
		case ARROWBASTARD_DETAIL4:
			memcpy(d, "           ", 12);
		break;
		/*
		,
		ARROWBASTARD_DETAIL*/
	}	
}
