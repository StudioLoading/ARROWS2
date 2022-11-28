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


void GetLocalizedDDLabel_EN(DD_TO_BE_LOCALIZED label, char* d) BANKED{
	switch(label){
		case MISSIONI_IN_CORSO:
			memcpy(d, "IN PROGRESS", 20);
		break;
		case FIND_BLACKIE_TITLE:
			memcpy(d, "FIND BLACKIE", 20);
		break;		
	}	
}
