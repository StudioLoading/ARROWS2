#include "Banks/SetAutoBank.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "string.h"
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
			memcpy(d, "                  ", 18);
		break;
		case MISSIONI_IN_CORSO:
			memcpy(d, "IN PROGRESS", 18);
		break;
		case FIND_BLACKIE_TITLE:
			memcpy(d, "FIND BLACKIE", 18);
		break;
		case FIND_BLACKIE_D0:
			memcpy(d, "I NEED THE HELP OF", 20);
		break;
		case FIND_BLACKIE_D1:
			memcpy(d, "THE BLACK WOLF.", 20);
		break;
		case ENGAGE_SMITH_TITLE:
			memcpy(d, "ENGAGE THE SMITH", 18);
		break;
		case ENGAGE_SMITH_D0:
			memcpy(d, "BRING METAL AND", 20);
		break;
		case ENGAGE_SMITH_D1:
			memcpy(d, "WOOD TO THE FORGE", 20);
		break;
		case HELP_DESPARATE_WIDOW_TITLE:
			memcpy(d, "HELP THE WIDOW", 18);
		break;
	}	
}
