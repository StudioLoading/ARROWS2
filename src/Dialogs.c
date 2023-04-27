#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "string.h"

#include "Dialogs.h"
#include "custom_datas.h"

unsigned char d1[22];
unsigned char d2[22];
unsigned char d3[22];
unsigned char d4[22];
unsigned char d5[22];
unsigned char d6[22];
unsigned char d7[22];
unsigned char d8[22];
unsigned char d9[22];
unsigned char d10[22];
unsigned char d11[22];
unsigned char d12[22];
unsigned char d13[22];
unsigned char d14[22];
unsigned char d15[22];
unsigned char d16[22];
unsigned char d17[22];
unsigned char d18[22];
unsigned char d19[22];
WHOSTALKING whostalking = NOBODY;
unsigned char EMPTY_STRING_21[22] = "                     \0";
unsigned char D0[22] = "$[$[$[$[$[$[$[$[$[$[$\0";
UINT8 choice = 0u;
UINT8 choice_left = 0u;
UINT8 choice_right = 0u;

extern struct MISSION missions[4];
extern void pickup(struct ItemSpawned* pickedup_data) BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;

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
			memcpy(d2, " MINE. HERE I FIND   \0", 22);
			memcpy(d3, " METAL AND WOOD.     \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_BLACKIE_CAVE:
			memcpy(d1, " RED RIDING HOOD   \0", 22);
			memcpy(d2, " WORST ENEMY CAVE  \0", 22);
			memcpy(d3, EMPTY_STRING_21 , 22);
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
		case TIP_STILL_SOMETHING:
			memcpy(d1, " I HAVE TO DO SOMETH \0", 22);
			memcpy(d2, " ING ELSE HERE,      \0", 22);
			memcpy(d3, " BEFORE EAVING       \0", 22);
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
		case INTRO:
			*n_lines = 7u;
			memcpy(d1, "'DESSA, YOU WITCH!  \0", 22);
			memcpy(d2, "I TOOK YOUR BOY TO  \0", 22);
			memcpy(d3, "SHOW HIM WHAT WE ALL\0", 22);
			memcpy(d4, "ARE GOING TO DEAL   \0", 22);
			memcpy(d5, "WITH. DRAGONS ARE   \0", 22);
			memcpy(d6, "COMING AND WE MUST  \0", 22);
			memcpy(d7, "BE READY. COME HERE!\0", 22);
		break;
		case EXZOO_WOMAN1:
			*n_lines = 3u;
			memcpy(d1, "HELLO 'DESSA. THE   \0", 22);
			memcpy(d2, "BLACK WOLF IS LOOKIN\0", 22);
			memcpy(d3, "FOR YOU.            \0" , 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case EXZOO_WOMAN2:
			*n_lines = 6u;
			memcpy(d1, "THERE'S GOING TO BE \0", 22);
			memcpy(d2, "A TOURNAMENT AT THE \0", 22);
			memcpy(d3, "INN IN FEW DAYS.    \0" , 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "WHERE'S THE CHAMP?  \0", 22);
			memcpy(d6, "I WONDER...         \0", 22);
		break;
		case EXZOO_WOMAN3:
			*n_lines = 8u;
			memcpy(d1, "THIS MAN' NAME IS   \0", 22);
			memcpy(d2, "PAUL. BUT WE CALL   \0", 22);
			memcpy(d3, "HIM 'LEGO'. GUESS   \0", 22);
			memcpy(d4, "WHY...? HIHIHI      \0", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "HE IS NOT GONNA BE  \0", 22);
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
		case CEMETERY_WOMAN1:
			*n_lines = 7u;
			memcpy(d1, "ETERNAL REST GRANT  \0", 22);
			memcpy(d2, "UNTO THEM, O LORD   \0", 22);
			memcpy(d3, "AND LET PERPETUAL   \0", 22);
			memcpy(d4, "LIGHT SHINE UPON EM.\0", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "MAY THEY REST IN   .\0", 22);
			memcpy(d7, "PEACE... AMEN.      \0", 22);
		break;
		case CEMETERY_WOMAN2:
			*n_lines = 4u;
			memcpy(d1, "WE DON'T USE WEAPONS\0", 22);
			memcpy(d2, "HERE.               \0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "LET US PRAY...      \0", 22);
		break;
		case SMITH:
			*n_lines = 7u;
			memcpy(d1, "HI, I AM THE SMITH. \0", 22);
			memcpy(d2, "I'D LIKE TO HELP YOU\0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "BRING TO THE FORGE  \0", 22);
			memcpy(d5, "10 OF WOOD AND 10 OF\0", 22);
			memcpy(d6, "METAL. GO CHECK THE \0" , 22);
			memcpy(d7, "OLD MINE.           \0", 22);
		break;
		case SUPERSTONE:
			*n_lines = 6u;
			memcpy(d1, "IT SEEMS TO ME THAT \0", 22);
			memcpy(d2, "THIS BIG STONE CAN  \0", 22);
			memcpy(d3, "BE BROKEN SOMEHOW...\0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "LET'S BRING SOME GUN\0", 22);
			memcpy(d6, "POWDER TO THE SMITH.\0", 22);
		break;
		case BOUNCINGBLOCK:
			*n_lines = 6u;
			memcpy(d1, "I SHOULD TRY TO     \0", 22);
			memcpy(d2, "SHOOT AN ARROW TO   \0", 22);
			memcpy(d3, "THIS BLOCK...       \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "IS IT GOING TO      \0", 22);
			memcpy(d6, "BOUNCE IT?          \0", 22);
		break;
		case SMITH_DISABLED:
			*n_lines = 5u;
			memcpy(d1, "THERE'S NO ONE IN   \0", 22);
			memcpy(d2, "HERE...             \0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "'SHOULD CHECK THE   \0", 22);
			memcpy(d5, "INN AND THE CEMETERY\0", 22);
		break;
		case SMITH_CHECKING_WOODANDMETAL:
			*n_lines = 3u;
			memcpy(d1, "I SEE... BROUGHT    \0", 22);
			memcpy(d2, "THE METAL AND HOOD? \0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
		break;
		case SMITH_CHECKING_NOWOODANDMETAL:
			*n_lines = 7u;
			memcpy(d1, "SORRY DESSA...      \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "'NEED AT LEAST 10   \0", 22);
			memcpy(d4, "PIECES OF METAL AND \0", 22);
			memcpy(d5, "WOOD THEN I MAY HELP\0", 22);
			memcpy(d6, "PLEASE COME BACK AS \0", 22);
			memcpy(d7, "SOON AS YOU HAVE IT.\0", 22);
		break;
		case SMITH_GIVING_ARROWS:
			*n_lines = 7u;
			memcpy(d1, "AAAAALRIIIGHT!      \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "*SPLINK! SPLINK!*   \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "HERE THEY ARE: 30   \0", 22);
			memcpy(d7, "ARROWS FOR YOU!!    \0", 22);			
		break;
		case WOLF01:
			switch(missions[0].current_step){
				case 0u:
				case 1u://non ho sconfitto le orde
					*n_lines = 9u;
					memcpy(d1, "HONORED TO SEE YOU, \0", 22);
					memcpy(d2, "HEALER. MOTHER BLAC \0", 22);
					memcpy(d3, "KIE NEEDS SOME HELP \0", 22);
					memcpy(d4, "IN THE NEXT ROOM.   \0", 22);
					memcpy(d5, "ARE YOU READY TO    \0", 22);
					memcpy(d6, "HELP HER?           \0", 22);
					memcpy(d7, EMPTY_STRING_21, 22);	
					memcpy(d8, EMPTY_STRING_21, 22);
					memcpy(d9, " NO          YES    \0", 22);
					choice = 1u;
					choice_left = 0u;
					choice_right = 0u;
				break;
			}
		break;
		case BLACKIE:
			switch(missions[0].current_step){
				case 0u:
				case 1u://non ho sconfitto le orde
					*n_lines = 6u;
					memcpy(d1, "GOOD MORNING HEALER \0", 22);
					memcpy(d2, "I AM TIRED BEATING  \0", 22);
					memcpy(d3, "UP ALL THESE LITTLE \0", 22);
					memcpy(d4, "BEASTS. PLEASE      \0", 22);
					memcpy(d5, "FINISH THE JOB AND  \0", 22);
					memcpy(d6, "COME BACK HERE.     \0", 22);
				break;
				case 2u://ho sconfitto le orde
					*n_lines = 6u;
					memcpy(d1, "THANK YOU HEALER.   \0", 22);
					memcpy(d2, EMPTY_STRING_21, 22);	
					memcpy(d3, "LET'S GO OUTSIDE.   \0", 22);
					memcpy(d4, EMPTY_STRING_21, 22);	
					memcpy(d5, EMPTY_STRING_21, 22);	
					memcpy(d6, "WE NEED TO TALK.    \0", 22);
					missions[0].current_step = 3u;
				break;
            	case 3u://ow orde sconfitte
					*n_lines = 18u;
					memcpy(d1, "I SAW A LOT OF ALLIG\0", 22);
					memcpy(d2, "ATORS IN THE EASTERN\0", 22);	
					memcpy(d3, "RN RIVER...         \0", 22);
					memcpy(d4, EMPTY_STRING_21, 22);	
					memcpy(d5, "UMH, I SEE... GIVE  \0", 22);	
					memcpy(d6, "ME THE LETTER. LET  \0", 22);	
					memcpy(d7, "ME GET TO THE MOUNTA\0", 22);	
					memcpy(d8, "INS AND ASK THE LORD\0", 22);	
					memcpy(d9, "OF THE THUNDERS TO  \0", 22);	
					memcpy(d10, "HELP US.           \0", 22);
					memcpy(d11, EMPTY_STRING_21, 22);	
					memcpy(d12, "I'VE HEARD A CRYING\0", 22);
					memcpy(d13, "LADY AT THE CEMETER\0", 22);
					memcpy(d14, "Y: PLEASE GO       \0", 22);
					memcpy(d15, "HELP HER.          \0", 22);
					memcpy(d16, EMPTY_STRING_21, 22);	
					memcpy(d17, "THEN               \0", 22);
					memcpy(d18, "MEET ME AT THE DOCK\0", 22);
                	missions[0].current_step = 4u;
					missions[2].mission_state = MISSION_STATE_ENABLED;
                	missions[0].mission_state = MISSION_STATE_REWARDED;
					{
						change_quantity(INVITEM_LETTER, -1);
						struct ItemSpawned metal_special_data={.itemtype = INVITEM_METAL_SPECIAL, .quantity = 1, .equippable = 0u};
						pickup(&metal_special_data);
					}
				break;
            }
        break;
    }
}