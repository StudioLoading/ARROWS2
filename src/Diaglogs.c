#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "string.h"
#include "Music.h"

#include "Dialogs.h"

unsigned char d1[22];
unsigned char d2[22];
unsigned char d3[22];
unsigned char d4[22];

unsigned char EMPTY_STRING_21[22] = "                     \0";
unsigned char D0[22] = "$[$[$[$[$[$[$[$[$[$[$\0";

void GetLocalizedTip_EN(TIP_TO_BE_LOCALIZED tip) BANKED{
	switch(tip){
		case TIP_SMITH_NO:
			memcpy(d3, EMPTY_STRING_21 , 22);
			memcpy(d1, " THE SMITH IS YET TO \0", 22);
			memcpy(d2, " COME.               \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_HOSPITAL_NO:
			memcpy(d1, " THIS IS THE         \0", 22);
			memcpy(d2, " HOSPITAL.           \0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_MINE_CAVE:
			memcpy(d1, " THIS IS THE OLD     \0", 22);
			memcpy(d2, " MINE. HERE YOU CAN  \0", 22);
			memcpy(d3, " FIND METAL.         \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_BLACKIE_CAVE:
			memcpy(d3, EMPTY_STRING_21 , 22);
			memcpy(d1, " RED RIDING HOOD      \0", 22);
			memcpy(d2, " WORST ENEMY' FAMILY  \0", 22);
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
	}
}
void GetLocalizedLabel_EN(TO_BE_LOCALIZED label, char* d) BANKED{
	switch(label){
		case DIARIO_MISSIONI:
			memcpy(d, "MISSION DIARY", 20);
		break;
		case OPZIONI_PARTITA:
			memcpy(d, "GAME OPTIONS", 20);
		break;		
	}	
}
