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
unsigned char dd9[21];
unsigned char dd10[21];
unsigned char dd11[21];
unsigned char dd12[21];
unsigned char dd13[21];

void GetLocalizedMLabel_EN(DD_TO_BE_LOCALIZED label, char* d) BANKED{
	switch(label){
		case EMPTY_STRING:
			memcpy(d, "                ", 16);
		break;
	}
}

void GetLocalizedDDLabel_EN(DD_TO_BE_LOCALIZED label, char* d) BANKED{
	switch(label){
		case EMPTY_STRING:memcpy(d, "                  ", 18);break;
		case MISSIONI_IN_CORSO:memcpy(d, "IN PROGRESS", 18);break;
		case MISSIONI_COMPLETATE:memcpy(d, "COMPLETED  ", 18);break;
		case FIND_BLACKIE_TITLE:memcpy(d, "FIND BLACKIE   ", 16);break;
		case FIND_BLACKIE_D0:memcpy(d, "I NEED THE HELP OF ", 20);break;
		case FIND_BLACKIE_D1:memcpy(d, "THE BLACK WOLF.    ", 20);break;
		case FIND_BLACKIE_D2:memcpy(d, "SHOULD START GOING ", 20);break;
		case FIND_BLACKIE_D3:memcpy(d, "TO SOUTHWEST CAVE  ", 20);break;
		case FIND_BLACKIE_D4:memcpy(d, "BLACKIE NEEDS HELP ", 20);break;
		case FIND_BLACKIE_D5:memcpy(d, "TO CLEAR THE AREA  ", 20);break;
		case FIND_BLACKIE_D6:memcpy(d, "BLACKIE IS GOING   ", 20);break;
		case FIND_BLACKIE_D7:memcpy(d, "UP TO THE MOUNTAIN ", 20);break;
		case ENGAGE_SMITH_TITLE:memcpy(d, "ENGAGE THE SMITH", 16);break;
		case ENGAGE_SMITH_D0:memcpy(d, "BRING METAL AND    ", 20);break;
		case ENGAGE_SMITH_D1:memcpy(d, "WOOD TO THE FORGE  ", 20);break;
		case ENGAGE_SMITH_D2:memcpy(d, "THIS SMITH FORGES  ", 20);break;
		case ENGAGE_SMITH_D3:memcpy(d, "NORMAL ARROWS      ", 20);break;
		case ENABLE_HOSPITAL_TITLE:memcpy(d, "ENABLE HOSPITAL ", 16);break;
		case ENABLE_HOSPITAL_D0:memcpy(d, "THE HOSPITAL NEEDS ", 20);break;
		case ENABLE_HOSPITAL_D1:memcpy(d, "SOME SPECIAL METAL ", 20);break;
		case ENABLE_HOSPITAL_D2:memcpy(d, "BLACKIE GAVE ME    ", 20);break;
		case ENABLE_HOSPITAL_D3:memcpy(d, "THE METAL NEEDED   ", 20);break;
		case ENABLE_HOSPITAL_D4:memcpy(d, "NOW I CAN GET      ", 20);break;
		case ENABLE_HOSPITAL_D5:memcpy(d, "HEALED EVERYTIME   ", 20);break;
		case HELP_DESPARATE_WIDOW_TITLE:memcpy(d, "HELP THE WIDOW  ", 16);break;
	}	
}
