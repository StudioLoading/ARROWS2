#include "Banks/SetAutoBank.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "string.h"
#include "Music.h"
#include "Scroll.h"

#include "DiaryDialogs.h"

unsigned char dd1[21];
unsigned char dd2[21];
unsigned char dd3[21];
unsigned char dd4[21];
unsigned char dd5[21];
unsigned char dd6[21];
unsigned char dd7[21];
unsigned char dd8[21];


void GetLocalizedDDLabel_EN(DD_TO_BE_LOCALIZED label, char* d) BANKED{
	switch(label){
		case EMPTY_STRING:
			memcpy(d, "                    ", 20);
		break;
		case MISSIONI_IN_CORSO:
			memcpy(d, "IN PROGRESS", 20);
		break;
		case FIND_BLACKIE_TITLE:
			memcpy(d, "FIND BLACKIE", 20);
		break;
		case FIND_BLACKIE_D0:
			memcpy(d, "I NEED THE HELP OF", 20);
		break;
		case FIND_BLACKIE_D1:
			memcpy(d, "THE BLACK WOLF.", 20);
		break;
		case HELP_DESPARATE_WOMAN_TITLE:
			memcpy(d, "HELP THE DESPERATE", 20);
		break;
	}	
}
