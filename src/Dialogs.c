#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "string.h"

#include "Dialogs.h"
#include "Dialogs2.h"
#include "custom_datas.h"

unsigned char log0[21];
unsigned char m0[17];
unsigned char m1[17];
unsigned char m2[17];
unsigned char m3[17];
unsigned char d0[22];
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
unsigned char d20[22];
unsigned char d21[22];
unsigned char d22[22];
unsigned char d23[22];
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
extern UINT8 horde_step;
extern void GetLocalizedDialog2_EN(UINT8* n_lines) BANKED;

void GetLocalizedTip_EN(TIP_TO_BE_LOCALIZED tip) BANKED{
	switch(tip){
		case TIP_HIDDEN_ARROWS:
			memcpy(d1, " SOMETHING'S ON THE  \0", 22);
			memcpy(d2, " GROUND:             \0", 22);
			memcpy(d3, "           ARROWS!   \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case TIP_PRESS_SELECT:
			memcpy(d1, " PRESS SELECT TO VIEW\0", 22);
			memcpy(d2, " MY MISSION'S DIARY  \0", 22);
			memcpy(d3, " FOR THE CURRENT     \0", 22);
			memcpy(d4, " CHAPTER.            \0", 22);
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
	memcpy(d0, EMPTY_STRING_21, 22);//backup
	switch(whostalking){
		case INTRO:
			*n_lines = 18u;
			memcpy(d0, "MASTER:             \0", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "'DESSA YOU WITCH!   \0", 22);
			memcpy(d3, "DO YOU THINK HELPING\0", 22);
			memcpy(d4, "WILD ANIMALS WILL   \0", 22);
			memcpy(d5, "GUARANTEE OUR SURVI \0", 22);
			memcpy(d6, "VAL IN THIS WORLD?  \0", 22);
			memcpy(d7, EMPTY_STRING_21, 22);
			memcpy(d8, "DRAGONS ARE COMING! \0", 22);
			memcpy(d9, "THEY WILL PREVAIL   \0", 22);
			memcpy(d10, "UPON HUMANS UNLESS  \0", 22);
			memcpy(d11, "WE JOIN OUR FORCES! \0", 22);
			memcpy(d12, EMPTY_STRING_21, 22);
			memcpy(d13, "YOUR SON LIAM IS    \0", 22 );
			memcpy(d14, "WITH ME, TRAINING   \0", 22);
			memcpy(d15, "FOR THE BATTLE.     \0", 22);
			memcpy(d16, EMPTY_STRING_21, 22);
			memcpy(d17, "GET YOUR ASS TO THE \0", 22);
			memcpy(d18, "NORTHERN ISLE!      \0", 22);
		break;
		case EXZOO_WOMAN1:
			*n_lines = 3u;
			memcpy(d0, "HELEN:              \0", 22);
			memcpy(d1, "HELLO 'DESSA. THE   \0", 22);
			memcpy(d2, "BLACK WOLF IS LOOKIN\0", 22);
			memcpy(d3, "FOR YOU.            \0" , 22);
			memcpy(d4, EMPTY_STRING_21, 22);
		break;
		case EXZOO_WOMAN2:
			*n_lines = 6u;
			memcpy(d0, "PHOEBE:             \0", 22);
			memcpy(d1, "THERE'S GOING TO BE \0", 22);
			memcpy(d2, "A TOURNAMENT AT THE \0", 22);
			memcpy(d3, "INN IN A FEW DAYS.  \0" , 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "WHERE'S THE CHAMP?  \0", 22);
			memcpy(d6, "I WONDER...         \0", 22);
		break;
		case EXZOO_WOMAN3:
			*n_lines = 11u;
			memcpy(d0, "LISA:               \0", 22);
			memcpy(d1, "THIS MAN'S NAME IS  \0", 22);
			memcpy(d2, "PAUL. BUT WE CALL   \0", 22);
			memcpy(d3, "HIM 'LEGO'. GUESS   \0", 22);
			memcpy(d4, "WHY...? HIHIHI      \0", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "HE THINKS HE'S GOING\0", 22);
			memcpy(d7, "TO BE THE NEXT TETRA\0", 22);
			memcpy(d8, "CHAMP...            \0", 22);
			memcpy(d9, EMPTY_STRING_21, 22);
			memcpy(d10, "I WILL BE THE CHAM \0", 22);
			memcpy(d11, "PION!!!            \0", 22);
		break;
		case EXZOO_MAN1:
			*n_lines = 6u;
			memcpy(d0, "PAUL:               \0", 22);
			memcpy(d1, "HI! I'M PAUL.       \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "I'VE HEARD THE TETRA\0", 22);
			memcpy(d4, "CHAMP IS NOT COMING \0", 22);
			memcpy(d5, "MAYBE I WILL HAVE A \0" , 22);
			memcpy(d6, "CHANCE THIS TIME!   \0", 22);
		break;
		case EXZOO_MAN2:
			*n_lines =8u;
			memcpy(d0, "LUKE:               \0", 22);
			memcpy(d1, "MY DAD IS THE SMITH.\0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "I KNOW HE CAN FORGE \0", 22);
			memcpy(d4, "ARROWS AND OTHER OBJ\0", 22);
			memcpy(d5, "ECTS.               \0", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "HE IS GOING TO      \0", 22);
			memcpy(d8, "TEACH ME VERY SOON. \0", 22);
		break;		
		case CEMETERY_WOMAN1:
			*n_lines = 7u;
			memcpy(d0, "NUN:                \0", 22);
			memcpy(d1, "ETERNAL REST GRANT  \0", 22);
			memcpy(d2, "UNTO THEM, O LORD   \0", 22);
			memcpy(d3, "AND LET PERPETUAL   \0", 22);
			memcpy(d4, "LIGHT SHINE UPON EM.\0", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "MAY THEY REST IN    \0", 22);
			memcpy(d7, "PEACE... AMEN.      \0", 22);
		break;
		case CEMETERY_WOMAN2:
			*n_lines = 4u;
			memcpy(d0, "NUN:                \0", 22);
			memcpy(d1, "WE DON'T USE WEAPONS\0", 22);
			memcpy(d2, "HERE.               \0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "LET US PRAY...      \0", 22);
		break;
		case SMITH:
			*n_lines = 11u;
			memcpy(d0, "JOHN:               \0", 22);
			memcpy(d1, "HI, I AM JOHN.      \0", 22);
			memcpy(d2, "I AM THE SMITH AND  \0", 22);
			memcpy(d3, "I'D LIKE TO HELP YOU\0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "PLEASE BRING TO THE \0", 22);
			memcpy(d6, "FORGE 10 LOGS OF    \0", 22);
			memcpy(d7, "WOOD AND 10 PIECES  \0", 22);
			memcpy(d8, "OF METAL.           \0", 22);
			memcpy(d9, "CHECK THE OLD MINE. \0", 22);
			memcpy(d10, EMPTY_STRING_21, 22);
			memcpy(d11, "SEE YOU TO THE FORGE\0", 22);
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
			*n_lines = 7u;
			memcpy(d0, "DESSA:              \0", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "  THERE'S NO ONE    \0", 22);
			memcpy(d3, "IN HERE...          \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "I SHOULD CHECK THE  \0", 22);
			memcpy(d6, "INN AND THE         \0", 22);
			memcpy(d7, "CEMETERY            \0", 22);
		break;
		case SMITH_CHECKING_WOODANDMETAL:
			*n_lines = 3u;
			memcpy(d0, "JOHN:               \0", 22);
			memcpy(d1, "I SEE... BROUGHT    \0", 22);
			memcpy(d2, "THE METAL AND HOOD? \0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
		break;
		case SMITH_CHECKING_NOWOODANDMETAL:
			*n_lines = 7u;
			memcpy(d0, "JOHN:               \0", 22);
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
			memcpy(d0, "JOHN:               \0", 22);
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
			memcpy(d0, "JOHN:               \0", 22);
			memcpy(d1, "I ASK FOR FLOWERS   \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
		break;
		case SMITH_FLOWERS_MISSING:
			*n_lines = 2u;
			memcpy(d0, "JOHN:               \0", 22);
			memcpy(d1, "NOT ENOUGH FLOWERS  \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
		break;
		case SMITH_FLOWERS_THANKYOU:
			*n_lines = 3u;
			memcpy(d0, "JOHN:               \0", 22);
			memcpy(d1, "THANK YOU, TAKE THIS\0", 22);
			memcpy(d2, "BOX TO THE OUTWALKER\0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
		break;
		case WOLF01:
			memcpy(d0, "GRAY WOLF:         \0", 22);
			switch(find_blackie.current_step){
				case 0u:
				case 1u://non ho sconfitto le orde
					*n_lines = 9u;
					memcpy(d1, "HONORED TO SEE YOU, \0", 22);
					memcpy(d2, "HEALER. MOTHER BLAC \0", 22);
					memcpy(d3, "BLACKIE NEEDS HELP  \0", 22);
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
			memcpy(d0, "BLACKIE:            \0", 22);
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
					memcpy(d4, "IT'S THE STEEL THE  \0", 22);	
					memcpy(d5, "HOSPITAL IS LOOKING \0", 22);	
					memcpy(d6, "FOR.                \0", 22);	
					memcpy(d7, EMPTY_STRING_21, 22);	
					memcpy(d8, "NOW LET'S GO OUTSIDE\0", 22);
					memcpy(d9, "WE NEED TO TALK.    \0", 22);					
					struct ItemSpawned metal_special_data={.itemtype = INVITEM_METAL_SPECIAL, .quantity = 1, .equippable = 0u};
					pickup(&metal_special_data);
					find_blackie.current_step = 3u;
				break;
            	case 3u://ow orde sconfitte
					*n_lines = 17u;
					memcpy(d1, EMPTY_STRING_21, 22);
					memcpy(d2, "WHY DO I SMELL MY   \0", 22);
					memcpy(d3, "OLD MASTER'S SCENT? \0", 22);
					memcpy(d4, EMPTY_STRING_21, 22);		
					memcpy(d5, "DESSA:              \0", 22);	
					memcpy(d6, "  I HAVE RECEIVED A \0", 22);	
					memcpy(d7, "LETTER FROM HIM.    \0", 22);
					memcpy(d8, EMPTY_STRING_21, 22);		
					memcpy(d9, "BLACKIE:            \0", 22);
					memcpy(d10, "I AM SORRY HE COUGHT\0", 22);	
					memcpy(d11, "HIM. LET ME THINK.. \0", 22);	
					memcpy(d12, "AH! LISTEN UP: GIVE \0", 22);						
					memcpy(d13, "ME THE LETTER AND I \0", 22);	
					memcpy(d14, "WILL GO FIND SOME   \0", 22);	
					memcpy(d15, "HELP IN THE         \0", 22);	
					memcpy(d16, "MOUNTAINS.          \0", 22);
					memcpy(d17, "BUT FIRST...        \0", 22);
					memcpy(d18, EMPTY_STRING_21, 22);	
					memcpy(d19, "I'VE HEARD A CRYING\0", 22);
					memcpy(d20, "LADY, MARGARET, AT \0", 22);
					memcpy(d21, "THE CEMETERY:      \0", 22);
					memcpy(d22, "PLEASE GO HELP HER.\0", 22);
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
			*n_lines = 12u;
			memcpy(d0, "DOCTOR:             \0", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "  WELCOME TO THE HOS\0", 22);
			memcpy(d3, "PITAL.              \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "WE ARE NOT ABLE TO  \0", 22);
			memcpy(d6, "HEAL YOU COMPLETELY \0", 22);
			memcpy(d7, "BECAUSE OUR TOOLS   \0", 22);
			memcpy(d8, "ARE BROKEN.         \0", 22);
			memcpy(d9, EMPTY_STRING_21, 22);
			memcpy(d10, "PLEASE BRING US     \0", 22);
			memcpy(d11, "THE STEEL SO WE CAN \0", 22);
			memcpy(d12, "FIX THEM.           \0", 22);
		break;
		case HOSPITAL_ENABLING:
			*n_lines = 9u;
			memcpy(d0, "DOCTOR:             \0", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "  THANK YOU DESSA!  \0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "FROM NOW ON WE ARE  \0", 22);
			memcpy(d5, "ABLE TO CURE YOUR   \0", 22);
			memcpy(d6, "WOUNDS AND EVERYTIME\0", 22);
			memcpy(d7, "YOU'LL FAINT BLACKIE\0", 22);
			memcpy(d8, "WILL BRING YOU HERE.\0", 22);
			memcpy(d9, EMPTY_STRING_21, 22);
		break;
		case HOSPITAL_CURE:
			*n_lines = 4u;
			memcpy(d0, "DOCTOR:             \0", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "  ... WAKE UP!      \0", 22);
			memcpy(d3, "BLACKIE BROUGHT YOU \0", 22);
			memcpy(d4, "HERE...             \0", 22);
		break;
		case CRYING_MOTHER:
			*n_lines = 13u;
			memcpy(d0, "MARGARET:           \0", 22);
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
			memcpy(d0, "ROBERT:             \0", 22);
			*n_lines = 5u;
			memcpy(d1, "HE-HELLO            \0", 22);
			memcpy(d2, "I RUN OFF A BEAR... \0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "CAN YOU BRING ME TO \0", 22);
			memcpy(d5, "MOM?                \0", 22);
		break;
		case CHILDS_SAVED:
			*n_lines = 4u;
			memcpy(d0, "ROBERT:             \0", 22);
			memcpy(d1, "THANK YOU HEALER!   \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "IS SAFE HERE. I CAN \0", 22);
			memcpy(d4, "NOW FIND MY WAY HOME\0", 22);
		break;
		case POLICE_0_GET_PASS:
			*n_lines = 15u;
			memcpy(d0, "GUARD:              \0", 22);
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
			memcpy(d0, "GUARD:              \0", 22);
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
			memcpy(d0, "GUARD:              \0", 22);
			memcpy(d1, "HOW DARE YOU NOT TO  \0", 22);
			memcpy(d2, "TELL US THIS PRECIOUS\0", 22);
			memcpy(d3, "INFORMATION!! THIS IS\0", 22);
			memcpy(d4, "A BAD DECISION...    \0", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "...AND WILL HAVE     \0", 22);
			memcpy(d7, "CONSEQUENCES!        \0", 22);
			memcpy(d8, EMPTY_STRING_21, 22);
		break;
		default:
			GetLocalizedDialog2_EN(n_lines);
		break;
    }
}

void GetLocalizedLogItem_EN(INVITEMTYPE invitemtype) BANKED{
	switch(invitemtype){
		case INVITEM_MONEY:
			memcpy(log0, "PICKED UP: COIN     ", 20);
		break;
		case INVITEM_ARROW_BASTARD:
			memcpy(log0, "PICKED UP: BASTARDS ", 20);
		break;
		case INVITEM_ARROW_NORMAL:
			memcpy(log0, "PICKED UP: ARROWS   ", 20);
		break;
		case INVITEM_ARROW_PERFO:
			memcpy(log0, "PICKED UP: PEARCED  ", 20);
		break;
		case INVITEM_FLOWER:
			memcpy(log0, "PICKED UP: A FLOWER ", 20);
		break;
		case INVITEM_GLASSES:
			memcpy(log0, "PICKED UP: GLASSES  ", 20);
		break;
		case INVITEM_HEART:
		case INVITEM_HEARTS:
			memcpy(log0, "PICKED UP: HEALTH   ", 20);
		break;
		case INVITEM_METAL:
			memcpy(log0, "PICKED UP: METAL    ", 20);
		break;
		case INVITEM_POWDER:
			memcpy(log0, "PICKED UP: POWDER   ", 20);
		break;
		case INVITEM_WOOD:
			memcpy(log0, "PICKED UP: WOOD     ", 20);
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
		case StateTutorial:
			memcpy(log0, "TUTORIAL            ", 20);
		break;
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
			switch(horde_step){
				case 2u:memcpy(log0, "SLAY THE SNAKES!    ", 20);break;
				case 3u:memcpy(log0, "SLAY THE RATS!      ", 20);break;
				case 4u:memcpy(log0, "SLAY THE COBRAS!    ", 20);break;
				case 5u:memcpy(log0, "SLAY THE SPIDER!    ", 20);break;
				default:memcpy(log0, "BLACKIE:COME UP HERE", 20);break;
			}
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
		case StateMountain:
			memcpy(log0, "ROLLING STONES!     ", 20);
		break;
		case StateSky:
			memcpy(log0, "THUNDERS!           ", 20);
		break;
	}
}