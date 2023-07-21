#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "string.h"

#include "Dialogs.h"
#include "custom_datas.h"

unsigned char log0[21];
unsigned char m0[17];
unsigned char m1[17];
unsigned char m2[17];
unsigned char m3[17];
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

extern UINT16 motherow_pos_x;
extern UINT16 motherow_pos_y;
extern struct MISSION find_blackie;
extern struct MISSION help_cemetery_woman;
extern UINT16 spawn_child_cooldown;
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
			memcpy(d2, " ING ELSE HERE       \0", 22);
			memcpy(d3, " BEFORE LEAVING      \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_DARK_FOREST:
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "  THE DARKEST FOREST \0", 22);
			memcpy(d3, EMPTY_STRING_21 , 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_LABIRYNTH:
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "      NORTHERN       \0", 22);
			memcpy(d3, "      LABIRYNTH      \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_GROTTO:
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "       GROTTO       \0", 22);
			memcpy(d3, EMPTY_STRING_21 , 22);
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
			*n_lines = 16u;
			memcpy(d1, "'DESSA YOU WITCH!    \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "DO YOU THINK HELPING \0", 22);
			memcpy(d4, "WILD ANIMALS WILL    \0", 22);
			memcpy(d5, "GUARANTEE OUR SURVI  \0", 22);
			memcpy(d6, "VAL IN THIS WORLD?   \0", 22);
			memcpy(d7, EMPTY_STRING_21, 22);
			memcpy(d8, "DRAGONS ARE COMING! \0", 22);
			memcpy(d9, "AND WE MUST BE READY\0", 22);
			memcpy(d10, EMPTY_STRING_21, 22);
			memcpy(d11, "I'M TEACHING SOME   \0", 22);
			memcpy(d12, "TRICKS TO LIAM WITH \0", 22);
			memcpy(d13, "THAT BOW. WE ARE IN \0", 22);
			memcpy(d14, "THE NORTHERN ISLE.  \0", 22);
			memcpy(d15, EMPTY_STRING_21, 22);
			memcpy(d16, "GET OVER HERE!      \0", 22);
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
			*n_lines = 10u;
			memcpy(d1, "HI, I AM JOHN.      \0", 22);
			memcpy(d2, "I AM THE SMITH AND  \0", 22);
			memcpy(d3, "I'D LIKE TO HELP YOU\0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "BRING TO THE FORGE  \0", 22);
			memcpy(d6, "10 OF WOOD AND 10 OF\0", 22);
			memcpy(d7, "METAL. GO CHECK THE \0", 22);
			memcpy(d8, "OLD MINE.           \0", 22);
			memcpy(d9, EMPTY_STRING_21, 22);
			memcpy(d10, "SEE YOU TO THE FORGE\0", 22);
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
			switch(find_blackie.current_step){
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
			switch(find_blackie.current_step){
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
					*n_lines = 9u;
					memcpy(d1, "THANK YOU HEALER.   \0", 22);
					memcpy(d2, EMPTY_STRING_21, 22);	
					memcpy(d3, "FIRST TAKE THIS...  \0", 22);	
					memcpy(d4, "IT'S A SPECIAL METAL\0", 22);	
					memcpy(d5, "THE HOSPITAL IS LOOK\0", 22);	
					memcpy(d6, "ING FOR.            \0", 22);	
					memcpy(d7, EMPTY_STRING_21, 22);	
					memcpy(d8, "NOW LET'S GO OUTSIDE\0", 22);
					memcpy(d9, "WE NEED TO TALK.    \0", 22);					
					struct ItemSpawned metal_special_data={.itemtype = INVITEM_METAL_SPECIAL, .quantity = 1, .equippable = 0u};
					pickup(&metal_special_data);
					find_blackie.current_step = 3u;
				break;
            	case 3u://ow orde sconfitte
					*n_lines = 19u;
					memcpy(d1, "SALUTE HEALER, I AM \0", 22);
					memcpy(d2, "SORRY THE ARCHER HAS\0", 22);	
					memcpy(d3, "BEEN CAUGHT. LET ME \0", 22);
					memcpy(d4, "THINK...            \0", 22);	
					memcpy(d5, "GOT AN IDEA!   GIVE \0", 22);	
					memcpy(d6, "ME THE LETTER. LET  \0", 22);	
					memcpy(d7, "ME GET TO THE MOUNTA\0", 22);	
					memcpy(d8, "INS AND ASK THE LORD\0", 22);	
					memcpy(d9, "OF THE THUNDERS TO  \0", 22);	
					memcpy(d10, "HELP US.           \0", 22);
					memcpy(d11, EMPTY_STRING_21, 22);	
					memcpy(d12, "I'VE HEARD A CRYING\0", 22);
					memcpy(d13, "LADY, MARGARET, AT \0", 22);
					memcpy(d14, "THE CEMETERY:      \0", 22);
					memcpy(d15, "PLEASE GO HELP HER.\0", 22);
					memcpy(d16, "THEN MEET ME UP TO \0", 22);
					memcpy(d17, EMPTY_STRING_21, 22);	
					memcpy(d18, EMPTY_STRING_21, 22);
					memcpy(d19, "THE MOUNTAINS      \0", 22);
                	find_blackie.current_step = 4u;
					help_cemetery_woman.mission_state = MISSION_STATE_ENABLED;
                	find_blackie.mission_state = MISSION_STATE_STARTED;
					{
						change_quantity(INVITEM_LETTER, -1);
					}
				break;
            }
        break;
		case DEATH:
			*n_lines = 6u;
			if(find_blackie.current_step < 4){
				memcpy(d1, EMPTY_STRING_21, 22);
				memcpy(d2, EMPTY_STRING_21, 22);
				memcpy(d3, "DESSA IS DEAD.      \0", 22);
				memcpy(d4, EMPTY_STRING_21, 22);
				memcpy(d5, EMPTY_STRING_21, 22);
				memcpy(d6, "     GAME  OVER     \0", 22);
			}else{
				memcpy(d1, "DESSA PASSED OUT.   \0", 22);
				memcpy(d2, EMPTY_STRING_21, 22);
				memcpy(d3, "SHE WAS TAKEN TO THE\0", 22);
				memcpy(d4, "CLOSEST HUMAN       \0", 22);
				memcpy(d5, "HOSPITAL.           \0", 22);
				memcpy(d6, EMPTY_STRING_21, 22);
			}
		break;
		case HOSPITAL_DISABLED:
			*n_lines = 7u;
			memcpy(d1, "WE KNOW YOU ARE THE \0", 22);
			memcpy(d2, "HEALER BUT YOU CAN'T\0", 22);
			memcpy(d3, "HEAL YOURSELF.      \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "BRING US THE SPECIAL\0", 22);
			memcpy(d6, "METAL AND WE CAN    \0", 22);
			memcpy(d7, "START DOING OUR JOB.\0", 22);
		break;
		case HOSPITAL_ENABLING:
			*n_lines = 5u;
			memcpy(d1, "HERE IT IS!         \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "THANK YOU, FROM NOW \0", 22);
			memcpy(d4, "ON WE ARE ABLE TO   \0", 22);
			memcpy(d5, "CURE YOUR WOUNDS.   \0", 22);
		break;
		case HOSPITAL_CURE:
			*n_lines = 5u;
			memcpy(d1, "HELLO DESSA         \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "NOW REST, WE CAN    \0", 22);
			memcpy(d4, "TAKE CARE OF YOUR   \0", 22);
			memcpy(d5, "WOUNDS.             \0", 22);
		break;
		case CRYING_MOTHER:
			*n_lines = 13u;
			memcpy(d1, "SIGH!... HI...      \0", 22);
			memcpy(d2, "MY NAME IS MARGARET.\0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "SIGHT! OH HEALER I  \0", 22);
			memcpy(d5, "KNOW YOU CAN FEEL ME\0", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "MY CHILD HAS BEEN LO\0", 22);
			memcpy(d8, "ST IN THE HOODS...! \0", 22);
			memcpy(d9, "I AM SO SCARED!     \0", 22);
			memcpy(d10, EMPTY_STRING_21, 22);
			memcpy(d11, "PLEASE HELP ME FIND \0", 22);
			memcpy(d12, "HIM. I'LL GO WEST   \0", 22);
			memcpy(d13, "YOU GO NORTH.      \0", 22);
			help_cemetery_woman.mission_state = MISSION_STATE_STARTED;
		break;
		case CHILD:
			*n_lines = 5u;
			memcpy(d1, "HE-HELLO            \0", 22);
			memcpy(d2, "I RUN OFF A BEAR... \0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "CAN YOU BRING ME TO \0", 22);
			memcpy(d5, "MOM?                \0", 22);
		break;
		case CHILDS_SAVED:
			*n_lines = 4u;
			motherow_pos_x = ((UINT16) 18u << 3);
			motherow_pos_y = ((UINT16) 3u << 3);
			memcpy(d1, "THANK YOU HEALER!   \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "IS SAFE HERE. I CAN \0", 22);
			memcpy(d4, "NOW FIND MY WAY HOME\0", 22);
		break;
    }
}


void GetLocalizedLogName_EN(NPCNAME npcname) BANKED{
	switch(npcname){
		case JOHN:
			memcpy(log0, "JOHN                ", 20);
		break;
		case WOMAN:
			memcpy(log0, "WOMAN               ", 20);
		break;
		case LUKE:
			memcpy(log0, "LUKE                ", 20);
		break;
		case LEGO:
			memcpy(log0, "LEGO                ", 20);
		break;
		case WOLF_BLACKIE:
			memcpy(log0, "BLACKIE             ", 20);
		break;
	}
}

void GetLocalizedLog_EN() BANKED{
	switch(current_state){
		case StateExzoo:
			memcpy(log0, "EXZOO VILLAGE       ", 20);
		break;
		case StateBlackiecave:
			memcpy(log0, "CAVE OF THE WOLVES  ", 20);
		break;
		case StateBlackieroom:
			memcpy(log0, "SLAY THE HORDES!    ", 20);
		break;
		case StateCemetery:
			memcpy(log0, "HUMAN CEMETERY      ", 20);
		break;
		case StateHood:
			if(spawn_child_cooldown < 100u && spawn_child_cooldown > 0u){
				memcpy(log0, "HEY! WHO'S THERE?   ", 20);
			}else{
				memcpy(log0, "THE HOOD            ", 20);
			}
		break;
		case StateMine:
			memcpy(log0, "THE OLD MINE        ", 20);
		break;
	}
}