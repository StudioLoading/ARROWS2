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
		case ENGAGE_SMITH_D0:memcpy(d, "LOOK FOR JOHN AT   ", 20);break;
		case ENGAGE_SMITH_D1:memcpy(d, "THE CEMETERY       ", 20);break;
		case ENGAGE_SMITH_D2:memcpy(d, "BRING METAL AND    ", 20);break;
		case ENGAGE_SMITH_D3:memcpy(d, "WOOD TO THE FORGE  ", 20);break;
		case ENGAGE_SMITH_D4:memcpy(d, "SEARCH IN THE OLD  ", 20);break;
		case ENGAGE_SMITH_D5:memcpy(d, "MINE FOR MATERIALS ", 20);break;
		case ENGAGE_SMITH_D6:memcpy(d, "THIS SMITH FORGES  ", 20);break;
		case ENGAGE_SMITH_D7:memcpy(d, "NORMAL ARROWS      ", 20);break;
		case ENABLE_HOSPITAL_TITLE:memcpy(d, "ENABLE HOSPITAL ", 16);break;
		case ENABLE_HOSPITAL_D0:memcpy(d, "THE HOSPITAL NEEDS ", 20);break;
		case ENABLE_HOSPITAL_D1:memcpy(d, "SOME SPECIAL METAL ", 20);break;
		case ENABLE_HOSPITAL_D2:memcpy(d, "I OBRAINED THE     ", 20);break;
		case ENABLE_HOSPITAL_D3:memcpy(d, "STEEL METAL NEEDED ", 20);break;
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
		case GET_TO_THE_MOUNTAIN_TITLE:memcpy(d, "LORD OF THUNDERS", 16);break;
		case MOUNTAIN_D0:memcpy(d, "GET UP TO THE MOUN ", 20);break;
		case MOUNTAIN_D1:memcpy(d, "TAINS              ", 20);break;
		case MOUNTAIN_D2:memcpy(d, "                   ", 20);break;
		case MOUNTAIN_D3:memcpy(d, "                   ", 20);break;
		case DEFEAT_SCORPIONS_TITLE:memcpy(d, "SCORPION'S HORDE ", 16);break;
		case SCORPIONS_D0:memcpy(d, "DEFEAT ALL THE     ", 20);break;
		case SCORPIONS_D1:memcpy(d, "SCORPIONS AROUND   ", 20);break;
		case SCORPIONS_D2:memcpy(d, "I SHOULD ALSO CLEAN", 20);break;
		case SCORPIONS_D3:memcpy(d, "THE EASTERN SIDE   ", 20);break;
		case FIND_ANTIDOTE_TITLE:memcpy(d, "FIND ANTIDOTE  ", 16);break;
		case ANTIDOTE_D0:memcpy(d, "TALK TO THE DOCTORS", 20);break;
		case ANTIDOTE_D1:memcpy(d, "ABOUT THE ANTIDOTE ", 20);break;
		case ANTIDOTE_D2:memcpy(d, "FIND THE SCORPION' ", 20);break;
		case ANTIDOTE_D3:memcpy(d, "MOTHER             ", 20);break;
		case ANTIDOTE_D4:memcpy(d, "BRING MOTHER'S TAIL", 20);break;
		case ANTIDOTE_D5:memcpy(d, "TO THE HOSPITAL!   ", 20);break;
		case ANTIDOTE_D6:memcpy(d, "GO TALK TO JESSICA ", 20);break;
		case ANTIDOTE_D7:memcpy(d, "BRING HERBS TO HOSP", 20);break;
		case HUNGRY_TITLE:memcpy(d, "HUNGRY PEOPLE  ", 16);break;
		case HUNGRY_D0:memcpy(d, "KILL ALL THE ALLIGA", 20);break;
		case HUNGRY_D1:memcpy(d, "TORS FROM THE BOAT ", 20);break;
		case HUNGRY_D2:memcpy(d, "READ THE MESSAGE IN", 20);break;
		case HUNGRY_D3:memcpy(d, "THE BOTTLE         ", 20);break;
		case FIX_BRIDGE_TITLE:memcpy(d, "FIX THE BRIDGE ", 16);break;
		case FIX_BRIDGE_D0:memcpy(d, "ASK THE CARPENTER ", 20);break;
		case FIX_BRIDGE_D1:memcpy(d, "TO FIX THE BRIDGE ", 20);break;
		case FIX_BRIDGE_D2:memcpy(d, "THE CARPENTER NEED", 20);break;
		case FIX_BRIDGE_D3:memcpy(d, "50 WOOD, 50 METAL ", 20);break;
		case FIX_BRIDGE_D4:memcpy(d, "BRIDGE IS FIXED!  ", 20);break;
		case GOLDEN_ARMOR_TITLE:memcpy(d, "A NEW ARMOR    ", 16);break;
		case GOLDEN_D0:memcpy(d, "ASK THE SMITH TO   ", 20);break;
		case GOLDEN_D1:memcpy(d, "FORGE A GOLDEN ARM ", 20);break;
		case GOLDEN_D2:memcpy(d, "THE SMITH WANTS    ", 20);break;
		case GOLDEN_D3:memcpy(d, "GOLD AND SILVER:   ", 20);break;
		case GOLDEN_D4:memcpy(d, "THE MINOTAUR SHOULD", 20);break;
		case GOLDEN_D5:memcpy(d, "HAVE ALL THAT.     ", 20);break;
		case GOLDEN_D6:memcpy(d, "LET'S GET TO THE   ", 20);break;
		case GOLDEN_D7:memcpy(d, "SMITH.             ", 20);break;
		case GOLDEN_D8:memcpy(d, "I OBTAINED THE     ", 20);break;
		case GOLDEN_D9:memcpy(d, "GOLDEN ARMOR       ", 20);break;
		case MRSMEE_TITLE:memcpy(d, "A LOST FRIEND  ", 16);break;
		case MRSMEE_D0:memcpy(d, "CHECK THE BAT CAVE ", 20); break;
		case MRSMEE_D1:memcpy(d, "                   ", 20); break;
		case MRSMEE_D2:memcpy(d, "REPORT THE BAD NEW ", 20); break;
		case MRSMEE_D3:memcpy(d, "TO MR SMEE         ", 20); break;
		case MRSMEE_D4:memcpy(d, "MR SMEE HAS THANKED", 20); break;
		case MRSMEE_D5:memcpy(d, "ME", 20); break;
		case BROKENSHIP_TITLE:memcpy(d, "BROKEN SHIP    ", 16);break;
		case BROKENSHIP_D0:memcpy(d, "CHECK THE BAT CAVE ", 20); break;
		case BROKENSHIP_D1:memcpy(d, "                   ", 20); break;
		case BROKENSHIP_D2:memcpy(d, "REPORT THE BAD NEW ", 20); break;
		case BROKENSHIP_D3:memcpy(d, "TO MR SMEE         ", 20); break;
		case BROKENSHIP_D4:memcpy(d, "MR SMEE HAS THANKED", 20); break;
		case BROKENSHIP_D5:memcpy(d, "ME", 20); break;
		case STRIKE_TITLE:memcpy(d, "THE STRIKE     ", 16);break;
		case STRIKE_D0:memcpy(d, "CHECK THE BAT CAVE ", 20); break;
		case STRIKE_D1:memcpy(d, "                   ", 20); break;
		case STRIKE_D2:memcpy(d, "REPORT THE BAD NEW ", 20); break;
		case STRIKE_D3:memcpy(d, "TO MR SMEE         ", 20); break;
		case STRIKE_D4:memcpy(d, "MR SMEE HAS THANKED", 20); break;
		case STRIKE_D5:memcpy(d, "ME", 20); break;
		case CAPTAIN_TITLE:memcpy(d, "PLAY TETRA     ", 16);break;
		case CAPTAIN_D0:memcpy(d, "CHECK THE BAT CAVE ", 20); break;
		case CAPTAIN_D1:memcpy(d, "                   ", 20); break;
		case CAPTAIN_D2:memcpy(d, "REPORT THE BAD NEW ", 20); break;
		case CAPTAIN_D3:memcpy(d, "TO MR SMEE         ", 20); break;
		case CAPTAIN_D4:memcpy(d, "MR SMEE HAS THANKED", 20); break;
		case CAPTAIN_D5:memcpy(d, "ME", 20); break;
	}	
}
