#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "string.h"

#include "Dialogs.h"
#include "Dialogs2.h"
#include "custom_datas.h"

extern unsigned char log0[21];
extern unsigned char m0[17];
extern unsigned char m1[17];
extern unsigned char m2[17];
extern unsigned char m3[17];
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
extern INT8 outwalker_info_step;
extern INT8 outwalker_info_given;



void GetLocalizedDialog2_EN(UINT8* n_lines) BANKED{
    switch(whostalking){
		case IBEX_GIVE_MISSION:
			*n_lines = 17u;
			memcpy(d1, "*COUGH!* *COUGH!*   \0", 22);
			memcpy(d2, "SALUTE *COUGH!* HEAL\0", 22);
			memcpy(d3, "ER. BLACKIE ALREADY \0", 22);
			memcpy(d4, "EXPLAINED ME WHAT   \0", 22);
			memcpy(d5, "HAPPENED TO YOUR SON\0", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "OUT OF NOWHRE A COLO\0", 22);
			memcpy(d8, "NY OF SCORPIONS APPE\0", 22);
			memcpy(d9, "ARED AND IS MARCHING\0", 22);
			memcpy(d10, "DOWN TO THE *COUGH!*\0", 22);
			memcpy(d11, "VILLAGE!            \0", 22);
			memcpy(d12, EMPTY_STRING_21, 22);
			memcpy(d13, "AS YOU CAN IMAGINE, \0", 22);
			memcpy(d14, "THOSE BASTARDS HAVE \0", 22);
			memcpy(d15, "A LETHAL POISON IN  \0", 22);
			memcpy(d16, "THOSE STINGS. HERE  \0", 22);
			memcpy(d17, EMPTY_STRING_21, 22);
		break;
		case IBEX_GIVE_HERBS:
			*n_lines = 17u;
			memcpy(d1, "*COUGH!* *COUGH!*   \0", 22);
			memcpy(d2, "TAKE THESE HERBS.   \0", 22);
			memcpy(d3, "I ALREADY ATE A     \0", 22);
			memcpy(d4, "BUNCH. I AM SLOWLY  \0", 22);
			memcpy(d5, "REVOVERING.         \0", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "TAKE THESE TO THE   \0", 22);
			memcpy(d8, "HUMANS, YOU KNOW    \0", 22);
			memcpy(d9, "IT CAN HEAL THEM.   \0", 22);
			memcpy(d10, EMPTY_STRING_21, 22);
			memcpy(d11, "NOW LEAVE! THROUGH \0", 22);
			memcpy(d12, "THAT CAVE YOU'LL   \0", 22);
			memcpy(d13, "GET DOWN THE MOUNTA\0", 22);
			memcpy(d14, "IN FASTER!         \0", 22);
			memcpy(d15, EMPTY_STRING_21, 22);
			memcpy(d16, "I'LL SEE YOU, AGAIN\0", 22);
			memcpy(d17, EMPTY_STRING_21, 22);
		break;
    }
}