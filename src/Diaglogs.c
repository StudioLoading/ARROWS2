#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "string.h"
#include "Music.h"

#include "Dialogs.h"

unsigned char d1[21];
unsigned char d2[21];
unsigned char d3[21];
unsigned char d4[21];

unsigned char EMPTY_STRING_20[21] = "                    \0";
unsigned char D0[21] = "$[$[$[$[$[$[$[$[$[$[\0";

void GetLocalizedTip_EN(TIP_TO_BE_LOCALIZED tip) BANKED{
	switch(tip){
		case TIP_SMITH_NO:
			memcpy(d3, EMPTY_STRING_20 , 20);
			memcpy(d1, "THE SMITH IS YET TO ", 20);
			memcpy(d2, "COME.               ", 20);
			memcpy(d4, EMPTY_STRING_20, 20);
		break;
		case TIP_HOSPITAL_NO:
			memcpy(d1, "THIS IS THE         " , 20);
			memcpy(d2, "HOSPITAL.           ", 20);
			memcpy(d3, EMPTY_STRING_20, 20);
			memcpy(d4, EMPTY_STRING_20, 20);
		break;
		case TIP_MINE_CAVE:
			memcpy(d1, "THIS IS THE OLD     " , 20);
			memcpy(d2, "MINE. HERE YOU CAN  ", 20);
			memcpy(d3, "FIND METAL.         ", 20);
			memcpy(d4, EMPTY_STRING_20, 20);
		break;
		case TIP_BLACKIE_CAVE:
			memcpy(d3, EMPTY_STRING_20 , 20);
			memcpy(d1, "RED RIDING HOOD     ", 20);
			memcpy(d2, "WORST ENEMY' FAMILY ", 20);
			memcpy(d4, EMPTY_STRING_20, 20);
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
