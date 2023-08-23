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
INT8 outwalker_info_step = 0;
INT8 outwalker_info_given = 0;

extern struct MISSION find_blackie;
extern struct MISSION help_cemetery_woman;
extern struct MISSION outwalker_chief;
extern struct MISSION outwalker_glass;
extern struct MISSION outwalker_smith;
extern UINT16 spawn_child_cooldown;
extern void pickup(struct ItemSpawned* pickedup_data) BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;
extern struct EnemyData* crab_data;

void GetLocalizedTip_EN(TIP_TO_BE_LOCALIZED tip) BANKED{
	switch(tip){
		case TIP_HIDDEN_ARROWS:
			memcpy(d1, " SOMETHING'S ON THE  \0", 22);
			memcpy(d2, " GROUND:             \0", 22);
			memcpy(d3, "           ARROWS!   \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
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

/*
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
*/

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
		case SMITH_FLOWERS_ASKED:
			*n_lines = 2u;
			memcpy(d1, "I ASK FOR FLOWERS   \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
		break;
		case SMITH_FLOWERS_MISSING:
			*n_lines = 2u;
			memcpy(d1, "NOT ENOUGH FLOWERS  \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
		break;
		case SMITH_FLOWERS_THANKYOU:
			*n_lines = 3u;
			memcpy(d1, "THANK YOU, TAKE THIS\0", 22);
			memcpy(d2, "BOX TO THE OUTWALKER\0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
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
			memcpy(d1, "DESSA PASSED OUT.   \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "SHE WAS TAKEN TO THE\0", 22);
			memcpy(d4, "CLOSEST HUMAN       \0", 22);
			memcpy(d5, "HOSPITAL.           \0", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
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
			memcpy(d1, "THANK YOU HEALER!   \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "IS SAFE HERE. I CAN \0", 22);
			memcpy(d4, "NOW FIND MY WAY HOME\0", 22);
		break;
		case POLICE_0_GET_PASS:
			*n_lines = 15u;
			memcpy(d1, "SALUTE HEALER,      \0", 22);
			memcpy(d2, "WE KNOW THAT TO THE \0", 22);
			memcpy(d3, "WEST THERE'S A BUNCH\0", 22);
			memcpy(d4, "OF PEOPLE LIVING BY \0", 22);
			memcpy(d5, "THEIR OWN. WE WANT  \0", 22);
			memcpy(d6, "TO KNOW WHO IS THEIR\0", 22);
			memcpy(d7, "LEADER. BECAUSE THEY\0", 22);
			memcpy(d8, "WON'T TELL US, TAKE \0", 22);
			memcpy(d9, "THIS PASS. SHOW IT  \0", 22);
			memcpy(d10, "AND THEY WILL LET  \0", 22);
			memcpy(d11, "YOU IN.            \0", 22);
			memcpy(d12, EMPTY_STRING_21, 22);
			memcpy(d13, "PLEASE COME BACK AS\0", 22);
			memcpy(d14, "SOON AS YOU FIND   \0", 22);
			memcpy(d15, "OUT. THANK YOU.    \0", 22);				
			{
				struct ItemSpawned pass_data={.itemtype = INVITEM_PASS, .quantity = 1, .equippable = 0u};
				pickup(&pass_data);
				outwalker_chief.mission_state = MISSION_STATE_ENABLED;
				outwalker_chief.current_step = 1;
			}
		break;
		case POLICE_0_STILL_NOT_FOUND:
			*n_lines = 8u;
			memcpy(d1, "SALUTE HEALER,      \0", 22);
			memcpy(d2, "WE ARE STILL WAITING\0", 22);
			memcpy(d3, "FOR YOU TO TELL US  \0", 22);
			memcpy(d4, "WHO'S THE OUTWALKER \0", 22);
			memcpy(d4, "CHIEF.              \0", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "PLEASE COME BACK AS\0", 22);
			memcpy(d7, "SOON AS YOU FIND   \0", 22);
			memcpy(d8, "OUT. THANK YOU.    \0", 22);
		break;
		case POLICE_0_WONT_TALK:
			*n_lines = 8u;
			memcpy(d1, "HOW DARE YOU NOT TO  \0", 22);
			memcpy(d2, "TELL US THIS PRECIOUS\0", 22);
			memcpy(d3, "INFORMATION!! THIS IS\0", 22);
			memcpy(d4, "A BAD DECISION...    \0", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "...AND WILL HAVE     \0", 22);
			memcpy(d7, "CONSEQUENCES!        \0", 22);
			memcpy(d8, EMPTY_STRING_21, 22);
		break;
		case OUTWALKER_NO_ENTER:
			*n_lines = 5u;
			memcpy(d1, "SORRY HEALER,       \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "YOU CAN NOT GET IN  \0", 22);
			memcpy(d4, "HERE WITHOUT A PASS.\0", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
		break;
		case OUTWALKER_MAN1:
			*n_lines = 12u;
			memcpy(d1, "WE ARE SO SORRY FOR \0", 22);
			memcpy(d2, "YOUR CHILD KIDNAP!  \0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "ARE YOU SURE NO WILD\0", 22);
			memcpy(d5, "ANIMALS ARE INVOLVED\0", 22);
			memcpy(d6, "IN THIS?            \0", 22);
			memcpy(d7, EMPTY_STRING_21, 22);
			memcpy(d8, "LIAM HAS DEFEATED   \0", 22);
			memcpy(d9, "THEIR MASTER IN THE \0", 22);
			memcpy(d10, "BATTLE OF THE COLUS \0", 22);
			memcpy(d11, "SEUM, SOME TIME AGO.\0", 22);
			memcpy(d12, EMPTY_STRING_21, 22);
		break;
		case OUTWALKER_MAN2:
			if(outwalker_info_given >= 4){outwalker_info_step = 2;}
			switch(outwalker_info_step){
				case -1://you are poor
					*n_lines = 4u;
					memcpy(d1, "I SEE YOU ARE A POOR\0", 22);
					memcpy(d2, "LADY... COME BACK   \0", 22);
					memcpy(d3, "WITH THE COINS AND  \0", 22);
					memcpy(d4, "I'LL TALK!          \0", 22);
					outwalker_info_step = 0;
				break;
				case 0://wanna pay to know?
					*n_lines = 9u;
					memcpy(d1, "SO YOU WANT TO KNOW \0", 22);
					memcpy(d2, "WHO'S OUR CHIEF?    \0", 22);
					memcpy(d3, EMPTY_STRING_21, 22);
					memcpy(d4, "WOULD YOU GIVE ME   \0", 22);
					memcpy(d5, "30 COINS? I'LL SHARE\0", 22);
					memcpy(d6, "SOME INFORMATIONS...\0", 22);
					memcpy(d7, EMPTY_STRING_21, 22);	
					memcpy(d8, EMPTY_STRING_21, 22);
					memcpy(d9, " NO          YES    \0", 22);
					choice = 1u;
					choice_left = 0u;
					choice_right = 0u;
				break;
				case 1://listen...
					switch(outwalker_info_given){
						case 1:
							*n_lines = 8u;
							memcpy(d1, "ALRIGHT, 30 COINS   \0", 22);
							memcpy(d2, "FOR ME...           \0", 22);
							memcpy(d3, EMPTY_STRING_21, 22);
							memcpy(d4, "LISTEN: AT THE END  \0", 22);
							memcpy(d5, "OF THE GROTTO MAZE  \0", 22);
							memcpy(d6, "THE CHIEF LIKES TO  \0", 22);
							memcpy(d7, "STAY.               \0", 22);
							memcpy(d8, EMPTY_STRING_21, 22);
							choice = 0u;
							choice_left = 0u;
							choice_right = 0u;
							outwalker_info_step = 0;
						break;
						case 2:
							*n_lines = 10u;
							memcpy(d1, "ALRIGHT, 30 COINS   \0", 22);
							memcpy(d2, "FOR ME...           \0", 22);
							memcpy(d3, EMPTY_STRING_21, 22);
							memcpy(d4, "LISTEN: THE ONLY WAY\0", 22);
							memcpy(d5, "FOR YOU TO BE       \0", 22);
							memcpy(d6, "NOTICED BY THE CHIEF\0", 22);
							memcpy(d7, "IS TO POP IN FRONT  \0", 22);
							memcpy(d8, "OF HIM EXITING THE  \0", 22);
							memcpy(d9, "GROTTO MAZE.        \0", 22);
							memcpy(d10, EMPTY_STRING_21, 22);
							choice = 0u;
							choice_left = 0u;
							choice_right = 0u;
							outwalker_info_step = 0;
						break;
						case 3:
							*n_lines = 8u;
							memcpy(d1, "ALRIGHT, 30 COINS   \0", 22);
							memcpy(d2, "FOR ME...           \0", 22);
							memcpy(d3, EMPTY_STRING_21, 22);
							memcpy(d4, "LISTEN: THE EXIT OF \0", 22);
							memcpy(d5, "THE MAZE IS OUR     \0", 22);
							memcpy(d6, "SECRET PASSAGE TO   \0", 22);
							memcpy(d7, "THE EXZOO VILLAGE!  \0", 22);
							memcpy(d8, EMPTY_STRING_21, 22);
							choice = 0u;
							choice_left = 0u;
							choice_right = 0u;
							outwalker_info_step = 2;
						break;
					}
				break;
				case 2://i said it all
					*n_lines = 2u;
					memcpy(d1, "I TOLD YOU ALL THAT \0", 22);
					memcpy(d2, "I KNOW...           \0", 22);
				break;
				case 3://you know already who the chief is
					*n_lines = 2u;
					memcpy(d1, "WILD ANIMALS ARE NOT\0", 22);
					memcpy(d2, "TO BE TRUSTED.      \0", 22);
				break;
			}
		break;
		case OUTWALKER_WOMAN1:		
			*n_lines = 11u;
			memcpy(d1, "WE ARE THE  OUTWAL  \0", 22);
			memcpy(d2, "KERS. WE DON'T BELIE\0", 22);
			memcpy(d3, "VE THE NOW INTELLIGE\0", 22);
			memcpy(d4, "NT ANIMALS AREN'T   \0", 22);
			memcpy(d5, "WILD NO MORE...     \0", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "THEY ARE PETTING THE\0", 22);
			memcpy(d8, "HUMANS AT THE EX ZOO\0", 22);
			memcpy(d9, "BECAUSE THEY WANT TO\0", 22);
			memcpy(d10, "RULE THEM!         \0", 22);
			memcpy(d11, EMPTY_STRING_21, 22);
		break;
		case OUTWALKER_WOMAN2:
			*n_lines = 8u;
			memcpy(d1, "WE ARE CALLED       \0", 22);
			memcpy(d2, "OUTWALKERS. WE ARE  \0", 22);
			memcpy(d3, "VERY PROUD OF WHO WE\0", 22);
			memcpy(d4, "ARE AND HOW WE LIVE.\0", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "WE ARE PEACEFUL, BUT\0", 22);
			memcpy(d7, "ALWAYS READY TO DEFE\0", 22);
			memcpy(d8, "ND OURSELVES.       \0", 22);
		break;
		case OUTWALKER_GLASS:
			if(outwalker_glass.mission_state >= MISSION_STATE_ENABLED){
				switch(outwalker_glass.current_step){
					case 0u://give mission
						*n_lines = 15u;
						memcpy(d1, "I MISS MY GLASSES BY\0", 22);
						memcpy(d2, "THE SEE, I SAW A    \0", 22);
						memcpy(d3, "GIANT CRAB ATTACKING\0", 22);
						memcpy(d4, "AND I PANICED.      \0", 22);
						memcpy(d5, EMPTY_STRING_21, 22);
						memcpy(d6, "NOW THAT YOU KNOW   \0", 22);
						memcpy(d7, "WHO IS OUR CHIEF I  \0", 22);
						memcpy(d8, "CAN TRUST YOU. CAN  \0", 22);
						memcpy(d9, "YOU PLEASE BRING MY \0", 22);
						memcpy(d10, "GLASSES BACK?      \0", 22);
						memcpy(d11, EMPTY_STRING_21, 22);
						memcpy(d12, "TAKE THESE PEARCING\0", 22);
						memcpy(d13, "ARROWS: GUESS YOU  \0", 22);
						memcpy(d14, "GONNA NEED EM FOR  \0", 22);
						memcpy(d15, "THE FIGHTING...    \0", 22);
						{
							struct ItemSpawned pass_data={.itemtype = INVITEM_ARROW_PERFO, .quantity = 8, .equippable = 1u};
							pickup(&pass_data);
							outwalker_glass.mission_state = MISSION_STATE_STARTED;
							outwalker_glass.current_step = 1;
						}
					break;
					case 1u://occhiali non ancora trovati
						*n_lines = 5u;
						memcpy(d1, "I MISS MY GLASSES BY\0", 22);
						memcpy(d2, "THE SEE... CAN YOU  \0", 22);
						memcpy(d3, "PLEASE BRING THOSE  \0", 22);
						memcpy(d4, "BACK?               \0", 22);
						memcpy(d5, EMPTY_STRING_21, 22);
					break;
					case 3u://occhiali trovati
						*n_lines = 8u;
						memcpy(d1, "THANK YOU MY DEAR!  \0", 22);
						memcpy(d2, "NOW I CAN FINALLY   \0", 22);
						memcpy(d3, "SEE AGAIN!!         \0", 22);
						memcpy(d4, EMPTY_STRING_21, 22);
						memcpy(d5, "OH, PLEASE, ACCEPT  \0", 22);
						memcpy(d6, "THESE FEW COINS AS A\0", 22);
						memcpy(d7, "REWARD!!            \0", 22);
						memcpy(d8, EMPTY_STRING_21, 22);
						{
							struct ItemSpawned pass_data={.itemtype = INVITEM_MONEY, .quantity = 40, .equippable = 1u};
							pickup(&pass_data);
							change_quantity(INVITEM_GLASSES, -1);
							outwalker_glass.mission_state = MISSION_STATE_REWARDED;
							outwalker_glass.current_step = 4;
						}
					break;
					case 4u:
						*n_lines = 4u;
						memcpy(d1, "THANK YOU MY DEAR!  \0", 22);
						memcpy(d2, "NOW I CAN FINALLY   \0", 22);
						memcpy(d3, "SEE AGAIN!!         \0", 22);
						memcpy(d4, EMPTY_STRING_21, 22);
					break;
				}
			}else{
				*n_lines = 5u;
				memcpy(d1, EMPTY_STRING_21, 22);
				memcpy(d2, "WHO ARE YOU?!       \0", 22);
				memcpy(d3, "I CAN'T SEE WITHOUT \0", 22);
				memcpy(d4, "MY GLASSES...       \0", 22);
				memcpy(d5, EMPTY_STRING_21, 22);
			}
		break;
		case OUTWALKER_GUARD_NOCHIEF_NOGLASS:
			*n_lines = 8u;
			memcpy(d1, "YES, THIS I THE WAY \0", 22);
			memcpy(d2, "TO THE MOUNTAINS.   \0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "YOU STILL DON'T KNOW\0", 22);
			memcpy(d5, "WHO IS OUR CHIEF... \0", 22);
			memcpy(d6, "SO I CAN'T TRUST YOU\0", 22);
			memcpy(d7, EMPTY_STRING_21, 22);
			memcpy(d8, "YOU SHALL NOT PASS! \0", 22);
		break;
		case OUTWALKER_GUARD_NOGLASS:
			*n_lines = 12u;
			memcpy(d1, "YES, THIS I THE WAY \0", 22);
			memcpy(d2, "TO THE MOUNTAINS.   \0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "SO YOU KNOW WHO IS  \0", 22);
			memcpy(d5, "OUR CHIEF... GOOD!  \0", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "I WILL TRUST YOU AS \0", 22);
			memcpy(d8, "SOON AS YOU HELP   \0", 22);
			memcpy(d9, "ONE OF US. JACK MAY\0", 22);
			memcpy(d10, "NEED YOU.          \0", 22);
			memcpy(d11, EMPTY_STRING_21, 22);
			memcpy(d12, "YOU SHALL NOT PASS! \0", 22);
			outwalker_glass.mission_state = MISSION_STATE_ENABLED;
		break;
		case OUTWALKER_GUARD_NOSMITH:
			*n_lines = 8u;
			memcpy(d1, "YES, THIS I THE WAY \0", 22);
			memcpy(d2, "TO THE MOUNTAINS.   \0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "WE NEED SOME WEAPONS\0", 22);
			memcpy(d5, "AND THE SMITH CAN   \0", 22);
			memcpy(d6, "FORGE EM FOR US. ASK\0", 22);
			memcpy(d7, "HIM SO, AND YOU     \0", 22);
			memcpy(d8, "FINALLY PASS.       \0", 22);
			outwalker_smith.mission_state = MISSION_STATE_ENABLED;
		break;
		case OUTWALKER_GUARD_OK:
				*n_lines = 6u;
				memcpy(d1, "YES, THIS I THE WAY \0", 22);
				memcpy(d2, "TO THE MOUNTAINS.   \0", 22);
				memcpy(d3, EMPTY_STRING_21, 22);
				memcpy(d4, "YOU DID A LOT FOR US\0", 22);
				memcpy(d5, "AND WE THANK YOU    \0", 22);
				memcpy(d6, "NOW YOU CAN PASS!   \0", 22);
		break;
		case OUTWALKER_CHIEF_FOUND:
			*n_lines = 18u;
			memcpy(d1, "WOHA! HOW DID YOU...\0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "OH WELL, NOW YOU    \0", 22);
			memcpy(d4, "KNOW...             \0", 22);
			memcpy(d5, "IT'S ME, LEGO, I AM \0", 22);
			memcpy(d6, "THE CHIEF OF THE OUT\0", 22);
			memcpy(d7, "WALKERS!!           \0", 22);
			memcpy(d8, EMPTY_STRING_21, 22);
			memcpy(d9, "I LIKE TO KEEP IT A \0", 22);
			memcpy(d10, "SECRET, SO PEOPLE   \0", 22);
			memcpy(d11, "ACT NORMALLY WHEN I \0", 22);
			memcpy(d12, "AM AROUND HERE.     \0", 22);
			memcpy(d13, EMPTY_STRING_21, 22);
			memcpy(d14, "PLEASE DON'T CHANGE \0", 22);
			memcpy(d15, "THIS, DON'T REVEAL  \0", 22);
			memcpy(d16, "THIS. HERE, TAKE:   \0", 22);
			memcpy(d17, "100 COINS FOR YOUR  \0", 22);
			memcpy(d18, "SILENCE.            \0", 22);
            outwalker_chief.current_step = 3;
			outwalker_chief.mission_state = MISSION_STATE_REWARDED;
			{
				struct ItemSpawned pass_data={.itemtype = INVITEM_MONEY, .quantity = 100, .equippable = 1u};
				pickup(&pass_data);
			}
		break;
		case MAZE_CANT_GO:
			*n_lines = 2u;
			memcpy(d1, "UMH... IT'S NOT SAFE\0", 22);
			memcpy(d2, "TO ENTER HERE YET.  \0", 22);
		break;
		case BOSS_CRAB_FIGHT:
			*n_lines = 11u;
			memcpy(d1, "*CLICK*! *CLICK*!   \0", 22);
			memcpy(d2, "WITH THESE GLASSES I\0", 22);
			memcpy(d3, "SEE A LOT BETTER... \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "WHAT? I FOUND IT!   \0", 22);
			memcpy(d6, "IT'S MINE NOW! OH   \0", 22);
			memcpy(d7, "THEY AREN'T?...\0", 22);
			memcpy(d8, EMPTY_STRING_21, 22);
			memcpy(d9, "*CLICK*! *CLICK*!   \0", 22);
			memcpy(d10, EMPTY_STRING_21, 22);
			memcpy(d11, "WE'LL SEE...      \0", 22);
		break;
		case BOSS_CRAB_END:
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
		case OUTWALKER_JERRY:
			memcpy(log0, "JERRY               ", 20);
		break;
		case OUTWALKER_JASON:
			memcpy(log0, "JASON               ", 20);
		break;
		case OUTWALKER_ANNETTE:
			memcpy(log0, "ANNETTE             ", 20);
		break;
		case OUTWALKER_JESSICA:
			memcpy(log0, "JESSICA             ", 20);
		break;
		case OUTWALKER_JACK:
			memcpy(log0, "JACK                ", 20);
		break;
		case OUTWALKER_SIMON:
			memcpy(log0, "SIMON THE GUARD     ", 20);
		break;
	}
}

void GetLocalizedLog_EN() BANKED{
	switch(current_state){
		case StateBosscrab:
			switch(crab_data->hp){
				case 0:memcpy(log0, "CRAB: BEATED!       ", 20);break;
				case 1:memcpy(log0, "CRAB: ( ) ) ) ) )   ", 20);break;
				case 2:memcpy(log0, "CRAB: ( ( ) ) ) )   ", 20);break;
				case 3:memcpy(log0, "CRAB: ( ( ( ) ) )   ", 20);break;
				case 4:memcpy(log0, "CRAB: ( ( ( ( ) )   ", 20);break;
				case 5:memcpy(log0, "CRAB: ( ( ( ( ( )   ", 20);break;
				case 6:memcpy(log0, "CRAB: ( ( ( ( ( (   ", 20);break;
			};
		break;
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
		case StateOutwalkers:
			memcpy(log0, "OUTWALKER CAMP      ", 20);
		break;
	}
}