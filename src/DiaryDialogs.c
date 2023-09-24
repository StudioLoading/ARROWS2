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
		case FIND_BLACKIE_TITLE:memcpy(d, "FIND BLACKIE    ", 16);break;
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
		case WIDOW_D0:memcpy(d, "TALK TO THE WIDOW  ", 20);break;
		case WIDOW_D1:memcpy(d, "AT THE CEMETERY    ", 20);break;
		case WIDOW_D2:memcpy(d, "I SHOULD CHECK THE ", 20);break;
		case WIDOW_D3:memcpy(d, "WESTERN HOODS      ", 20);break;
		case WIDOW_D4:memcpy(d, "GOTTA BRING THE KID", 20);break;
		case WIDOW_D5:memcpy(d, "BACK TO THE VILLAGE", 20);break;
		case WIDOW_D6:memcpy(d, "MARGARET' CHILD IS ", 20);break;
		case WIDOW_D7:memcpy(d, "SAFE NOW!          ", 20);break;
		case OUTWALKER_CHIEF_TITLE:memcpy(d, "WHO'S IN CHARGE?     ", 16);break;
		case CHIEF_D0:memcpy(d, "THE ANSWER'S AT THE", 20);break;
		case CHIEF_D1:memcpy(d, "EXIT OF THE MAZE   ", 20);break;
		case CHIEF_D2:memcpy(d, "I SHOULD CHECK THE ", 20);break;
		case CHIEF_D3:memcpy(d, "WESTERN HOODS      ", 20);break;
		case CHIEF_D4:memcpy(d, "LEGO IS THE CHIEF! ", 20);break;
		case CHIEF_D5:memcpy(d, "                   ", 20);break;
		case OUTWALKER_GLASS_TITLE:memcpy(d, "GLASSES FOR JACK     ", 16);break;
		case GLASS_D0:memcpy(d, "FIGHT THE CRAB AT  ", 20);break;
		case GLASS_D1:memcpy(d, "THE NORTHERN BEACH ", 20);break;
		case GLASS_D2:memcpy(d, "BRING THE GLASS TO ", 20);break;
		case GLASS_D3:memcpy(d, "JACK THE OUTWALKER ", 20);break;
		case GLASS_D4:memcpy(d, "JACK IS NOW ABLE TO", 20);break;
		case GLASS_D5:memcpy(d, "SEE WELL AGAIN     ", 20);break;
		case OUTWALKER_SMITH_TITLE:memcpy(d, "FLOWERS FOR JOHN     ", 16);break;
		case SMITH_D0:memcpy(d, "SEARCH FLOWERS FOR ", 20);break;
		case SMITH_D1:memcpy(d, "JOHN               ", 20);break;
		case SMITH_D2:memcpy(d, "BRING THE FLOWERS  ", 20);break;
		case SMITH_D3:memcpy(d, "TO JOHN            ", 20);break;
		case GET_TO_THE_MOUNTAIN_TITLE:memcpy(d, "LORD OF THUNDERS ", 16);break;
		case MOUNTAIN_D0:memcpy(d, "TALK TO THE WIDOW  ", 20);break;
		case MOUNTAIN_D1:memcpy(d, "AT THE CEMETERY    ", 20);break;
		case MOUNTAIN_D2:memcpy(d, "I SHOULD CHECK THE ", 20);break;
		case MOUNTAIN_D3:memcpy(d, "WESTERN HOODS      ", 20);break;
	}	
}
