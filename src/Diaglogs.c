#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "string.h"
#include "Music.h"

#include "Dialogs.h"
#include "custom_datas.h"

extern WHOSTALKING whostalking;

unsigned char d1[22];
unsigned char d2[22];
unsigned char d3[22];
unsigned char d4[22];
unsigned char d5[22];
unsigned char d6[22];
unsigned char d7[22];

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

void GetLocalizedDialog_EN(UINT8* n_lines) BANKED{
	switch(whostalking){
		case EXZOO_WOMAN1:
			*n_lines = 3u;
			memcpy(d1, "HELLO 'DESSA. THE   \0", 22);
			memcpy(d2, "BLACK WOLF IS LOOKIN\0", 22);
			memcpy(d3, "FOR YOU.            \0" , 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case EXZOO_WOMAN2:
			*n_lines = 4u;
			memcpy(d1, "THERE'S GOING TO BE \0", 22);
			memcpy(d2, "A TOURNAMENT AT THE \0", 22);
			memcpy(d3, "INN IN FEW DAYS.    \0" , 22);
			memcpy(d4, "WHERE'S THE CHAMP?  \0", 22);
		break;
		case EXZOO_WOMAN3:
			*n_lines = 7u;
			memcpy(d1, "THIS MAN' NAME IS   \0", 22);
			memcpy(d2, "PAUL. BUT WE CALL   \0", 22);
			memcpy(d3, "HIM 'LEGO'. GUESS   \0", 22);
			memcpy(d4, "WHY...? HIHIHI      \0", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "ANYWAY, IT'S NOT HIM\0", 22);
			memcpy(d7, "THE NEXT TETRA CHAMP\0", 22);
		break;
		case EXZOO_MAN1:
			*n_lines = 6u;
			memcpy(d1, "HI! I'M PAUL.       \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "I'VE HEARD TETRA    \0", 22);
			memcpy(d4, "CHAMP IS NOT GONNA  \0", 22);
			memcpy(d5, "COME. MAYBE I HAVE A\0" , 22);
			memcpy(d6, "CHANCE THIS TIME!   \0", 22);
		break;
		case EXZOO_MAN2:
			*n_lines =7u;
			memcpy(d1, "MY DAD IS THE SMITH.\0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "I KNOW HE CAN FORGE \0", 22);
			memcpy(d4, "ARROWS AND STUFF.   \0" , 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "HE IS GOING TO      \0", 22);
			memcpy(d7, "TEACH ME VERY SOON. \0", 22);
		break;
	}
}