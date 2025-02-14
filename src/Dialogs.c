#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
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
unsigned char EMPTY_STRING_21[22] = "                     ";
unsigned char D0[22] = "$[$[$[$[$[$[$[$[$[$[$";
UINT8 choice = 0u;
UINT8 choice_left = 0u;
UINT8 choice_right = 0u;
INT8 outwalker_info_given = 0;

extern struct MISSION find_blackie;
extern struct MISSION help_cemetery_woman;
extern struct MISSION find_antidote;
extern struct MISSION fix_bridge;
extern UINT16 spawn_child_cooldown;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;
extern struct EnemyData* crab_data;
extern struct EnemyData* scorpiohead_data;
extern struct EnemyData* minotaur_data;
extern struct EnemyData* bossbat_data;
extern UINT8 horde_step;
extern INT8 scorpio_hp;
extern UINT8 child_hooked;

extern void GetLocalizedDialog2_EN(UINT8* n_lines) BANKED;
extern UINT16 get_chapter_cost() BANKED;


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
			memcpy(d14, "WITH ME TRAINING    \0", 22);
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
		case WHOST_SHOP_SMITH:
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
			memcpy(d11, "SEE YOU AT THE FORGE\0", 22);
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
			memcpy(d0, "JOHN:               ", 22);
			memcpy(d1, "SORRY DESSA...      ", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "I NEED AT LEAST 10  ", 22);
			memcpy(d4, "PIECES OF METAL AND ", 22);
			memcpy(d5, "WOOD THEN I CAN HELP", 22);
			memcpy(d6, "PLEASE COME BACK    ", 22);
			memcpy(d7, "WHEN YOU HAVE IT.   ", 22);
		break;
		case SMITH_FLOWERS_ASKED:
			*n_lines = 17u;
			memcpy(d0, "JOHN:               ", 22);
			memcpy(d1, "SO YOU WANT ME TO FO", 22);
			memcpy(d2, "RGE SOME WEAPONS FOR", 22);
			memcpy(d3, "THE OUTWALKERS. FINE", 22);
			memcpy(d4, "BUT...              ", 22);
			memcpy(d5, "   (JOHN BLUSHES)   ", 22);
			memcpy(d6, "I AM QUITE ASHAMED  ", 22);
			memcpy(d7, "TO ASK YOU THIS...  ", 22);
			memcpy(d8, "(JOHN BLUSHES AGAIN)", 22);
			memcpy(d9, EMPTY_STRING_21, 22);
			memcpy(d10, "CAN YOU PLEASE BRING", 22);
			memcpy(d11, "ME 4 FLOWERS? I WANT", 22);
			memcpy(d12, "TO ASK MARGARET!    ", 22);
			memcpy(d13, "JUST CHECK AROUND HE", 22);
			memcpy(d14, "RE, THEY ARE NOT FAR", 22);
			memcpy(d15, "LIKE THE EXZOO, THE ", 22);
			memcpy(d16, "OLD MINE, THE CEMETE", 22);
			memcpy(d17, "RY, BLACKIE CAVE... ", 22);
		break;
		case SMITH_FLOWERS_MISSING:
			*n_lines = 9u;
			memcpy(d0, "JOHN:               ", 22);
			memcpy(d1, "HI, I AM NOT READY  ", 22);
			memcpy(d2, "YET AND YOU'VE NOT  ", 22);
			memcpy(d3, "COLLECTED THE FLOW  ", 22);
			memcpy(d4, "ERS.                ", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "PLEASE, COME BACK   ", 22);
			memcpy(d7, "WHEN YOU'VE COLLECT ", 22);
			memcpy(d8, "ED ALL THE FLOWERS. ", 22);
			memcpy(d9, EMPTY_STRING_21, 22);
		break;
		case SMITH_FLOWERS_THANKYOU:
			*n_lines = 11u;
			memcpy(d0, "JOHN:               ", 22);
			memcpy(d1, "OH! YOU FOUND THEM! ", 22);
			memcpy(d2, "THANK YOU HEALER!   ", 22);
			memcpy(d3, "THEY ARE BEAUTIFUL! ", 22);
			memcpy(d4, "I'LL BRING THEM TO  ", 22);
			memcpy(d5, "MARGARET.           ", 22);
			memcpy(d6, "WISH ME LUCK!       ", 22);	
			memcpy(d7, EMPTY_STRING_21, 22);
			memcpy(d8, EMPTY_STRING_21, 22);
			memcpy(d9, "AH! SURE, THE BOX!   ", 22);
			memcpy(d10, "HERE: THE WEAPONS   ", 22);
			memcpy(d11, "ARE INSIDE THIS BOX ", 22);
		break;
		case SMITH_NEED_GOLD_AND_SILVER:
			*n_lines = 5u;
			memcpy(d0, "JOHN:               ", 22);
			memcpy(d1, "I SEE... I CAN      ", 22);
			memcpy(d2, "FORGE A GOLDEN ARMOR", 22);
			memcpy(d3, "FOR YOU. I NEED 100 ", 22);
			memcpy(d4, "PIECES OF GOLD AND A", 22);
			memcpy(d5, "BIG PIECE OF SILVER ", 22);
			if(fix_bridge.mission_state == MISSION_STATE_ENABLED){
				*n_lines = 15u;
				memcpy(d6, "THERE'S A SILVER MI ", 22);
				memcpy(d7, "NE ON THE OTHER SIDE", 22);
				memcpy(d8, "OF THE EASTERN RIVER", 22);
				memcpy(d9, "THE BRIDGE SHOULD   ", 22);
				memcpy(d10, "BRING YOU THERE, BUT", 22);
				memcpy(d11, "IT'S BROKEN AS FAR  ", 22);
				memcpy(d12, "AS I KNOW.          ", 22);
				memcpy(d13, "PETER SHOULD BE ABLE", 22);
				memcpy(d14, "TO HELP YOU.        ", 22);
				memcpy(d15, "HE'S A CARPENTER!   ", 22);
				SpriteManagerAdd(SpriteDiary, 72, 8);
			}
		break;
		case SMITH_FORGE_ARMOR:
			*n_lines = 6u;
			memcpy(d0, "JOHN:               ", 22);
			memcpy(d1, "AAAAALRIIIGHT!      ", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "'SPLINK! SPLINK!'   ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "HERE! YOUR NEW ARMOR", 22);
			memcpy(d6, "ARMOR IS READY!     ", 22);
			SpriteManagerAdd(SpriteDiary, 72, 8);
		break;
		case WOLF01:
			memcpy(d0, "GRAY WOLF:          ", 22);
			switch(find_blackie.current_step){
				case 0u:
				case 1u://non ho sconfitto le orde
					*n_lines = 8u;
					memcpy(d1, "HONORED TO SEE YOU  ", 22);
					memcpy(d2, "HEALER. MOTHER BLAC ", 22);
					memcpy(d3, "KIE NEEDS YOUR HELP ", 22);
					memcpy(d4, "IN THE NEXT ROOM.   ", 22);
					memcpy(d5, "WILL YOU HELP HER?  ", 22);
					memcpy(d6, EMPTY_STRING_21, 22);
					memcpy(d7, EMPTY_STRING_21, 22);
					memcpy(d8, " NO          YES    ", 22);
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
					*n_lines = 5u;
					memcpy(d1, "HELLO HEALER I AM TI", 22);
					memcpy(d2, "RED OF FIGHTING PLEA", 22);
					memcpy(d3, "SE FINISH THE JOB   ", 22);
					memcpy(d4, "FOR ME AND COME BACK", 22);
					memcpy(d5, "WHEN YOU'RE DONE.   ", 22);
				break;
				case 2u://ho sconfitto le orde
					*n_lines = 5u;
					memcpy(d1, EMPTY_STRING_21, 22);	
					memcpy(d2, "THANK YOU HEALER.   ", 22);
					memcpy(d3, EMPTY_STRING_21, 22);	
					memcpy(d4, "LET'S GO OUTSIDE WE ", 22);
					memcpy(d5, "NEED TO TALK.       ", 22);
					find_blackie.current_step = 3u;
					SpriteManagerAdd(SpriteDiary, 72, 8);
				break;
            	case 3u://ow orde sconfitte
					*n_lines = 22u;
					memcpy(d1, EMPTY_STRING_21, 22);
					memcpy(d2, "WHY DO I SMELL MY   ", 22);
					memcpy(d3, "OLD MASTER'S SCENT? ", 22);
					memcpy(d4, EMPTY_STRING_21, 22);		
					memcpy(d5, "DESSA:              ", 22);	
					memcpy(d6, "I HAVE RECEIVED A   ", 22);	
					memcpy(d7, "LETTER FROM HIM.    ", 22);
					memcpy(d8, EMPTY_STRING_21, 22);		
					memcpy(d9, "BLACKIE:            ", 22);
					memcpy(d10, "I AM SORRY HE GOT   ", 22);
					memcpy(d11, "COUGHT. LET ME THINK", 22);	
					memcpy(d12, "UHM, LISTEN... GIVE ", 22);						
					memcpy(d13, "ME THE LETTER AND I ", 22);	
					memcpy(d14, "WILL GO LOOK FOR    ", 22);	
					memcpy(d15, "HELP IN THE         ", 22);	
					memcpy(d16, "MOUNTAINS.          ", 22);
					memcpy(d17, "BUT BEFORE I GO...  ", 22);
					memcpy(d18, EMPTY_STRING_21, 22);	
					memcpy(d19, "I'VE MET MARGARET AT", 22);
					memcpy(d20, "THE GRAVEYARD...    ", 22);
					memcpy(d21, "PLEASE GO HELP HER  ", 22);
					memcpy(d22, "SEE YOU LATER!      ", 22);
					change_quantity(INVITEM_LETTER, -1);
                	find_blackie.current_step = 4u;
					SpriteManagerAdd(SpriteDiary, 72, 8);
                	find_blackie.mission_state = MISSION_STATE_REWARDED;
					help_cemetery_woman.mission_state = MISSION_STATE_ENABLED;
				break;
            }
        break;
		case DEATH:
			*n_lines = 5u;
			memcpy(d1, "DESSA PASSED OUT.   ", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "SHE WAS TAKEN TO THE", 22);
			memcpy(d4, "CLOSEST HOSPITAL.   ", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
		break;
		case HOSPITAL_DISABLED:
			*n_lines = 12u;
			memcpy(d0, "DOCTOR:             ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "WELCOME TO THE HOSPI", 22);
			memcpy(d3, "TAL.                ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "WE ARE NOT ABLE TO  ", 22);
			memcpy(d6, "FULLY HEAL YOU      ", 22);
			memcpy(d7, "ALL OUR TOOLS ARE   ", 22);
			memcpy(d8, "BROKEN.             ", 22);
			memcpy(d9, EMPTY_STRING_21, 22);
			memcpy(d10, "PLEASE BRING US SOME", 22);
			memcpy(d11, "STEEL SO WE CAN FIX ", 22);
			memcpy(d12, "THEM.               ", 22);
		break;
		case HOSPITAL_ENABLING:
			*n_lines = 13u;
			memcpy(d0, "DOCTOR:             ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "THANK YOU DESSA!    ", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "FROM NOW ON WE WILL ", 22);
			memcpy(d5, "BE ABLE TO CURE YOUR", 22);
			memcpy(d6, "WOUNDS COMPLETELY   ", 22);
			memcpy(d7, "AND EVERYTIME YOU'LL", 22);
			memcpy(d8, "FAINT BLACKIE WILL  ", 22);
			memcpy(d9, "BRING YOU BACK HERE.", 22);
			memcpy(d10, EMPTY_STRING_21, 22);
			memcpy(d11, "FROM THE NEXT TIME  ", 22);
			memcpy(d12, "WE WILL ASK YOU SOME", 22);
			memcpy(d13, "GOLD THE HEAL YOU.  ", 22);
		break;
		case HOSPITAL_CURE:
			*n_lines = 5u;
			memcpy(d0, "DOCTOR:             \0", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "LET US HEAL YOU SO  ", 22);
			memcpy(d3, "YOU CAN CONTINUE    ", 22);
			memcpy(d4, "YOUR JOURNEY WITH   ", 22);
			memcpy(d5, "ALL OF YOUR STRENGTH", 22);
		break;
		case HOSPITAL_GAMEOVER:
			*n_lines = 12u;
			memcpy(d0, "DOCTOR:             ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "TOO BAD DESSA, YOU  ", 22);
			memcpy(d3, "DO NOT OWN ENOUGH   ", 22);
			memcpy(d4, "GOLD WE NEED IT OR  ", 22);
			memcpy(d5, "WE WILL NOT BE ABLE ", 22);
			memcpy(d6, "TO RUN THIS HOSPITAL", 22);
			memcpy(d7, "EFFICIENTLY         ", 22);
			memcpy(d8, EMPTY_STRING_21, 22);
			memcpy(d9, "... WE ARE SORRY... ", 22);
			memcpy(d10, "... WE ARE SORRY... ", 22);
			memcpy(d11, "YOUR JOURNEY MUST   ", 22);
			memcpy(d12, "START AGAIN.        ", 22);
		break;
		case HOSPITAL_FINE:
			*n_lines = 7u;
			memcpy(d0, "DOCTOR:             ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "HEY DESSA, YOU LOOK ", 22);
			memcpy(d3, "PERFECTLY FINE.     ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "PLEASE REMEMBER WE  ", 22);
			{
				UINT16 chapter_cost = get_chapter_cost();
				switch(chapter_cost){
					case 10: memcpy(d6, "NEED 10 GOLD COINS  ", 22);break;	
					case 20: memcpy(d6, "NEED 20 GOLD COINS  ", 22);break;	
					case 30: memcpy(d6, "NEED 30 GOLD COINS  ", 22);break;	
					case 40: memcpy(d6, "NEED 40 GOLD COINS  ", 22);break;	
					case 50: memcpy(d6, "NEED 50 GOLD COINS  ", 22);break;	
				}
			}
			memcpy(d7, "TO HEAL YOU.       ", 22);
		break;
		case HOSPITAL_CURE_FROM_DEATH:
			*n_lines = 6u;
			memcpy(d0, "DOCTOR:             ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "... WAKE UP!        ", 22);
			memcpy(d3, "BLACKIE BROUGHT YOU ", 22);
			memcpy(d4, "HERE... WE NEED     ", 22);
			{
				UINT16 chapter_cost = get_chapter_cost();
				switch(chapter_cost){
					case 10: memcpy(d5, "  10 GOLD COINS TO  ", 22);break;	
					case 20: memcpy(d5, "  20 GOLD COINS TO  ", 22);break;	
					case 30: memcpy(d5, "  30 GOLD COINS TO  ", 22);break;	
					case 40: memcpy(d5, "  40 GOLD COINS TO  ", 22);break;	
					case 50: memcpy(d5, "  50 GOLD COINS TO  ", 22);break;	
				}
			}
			memcpy(d6, "HEAL YOU.           ", 22);
		break;
		case HOSPITAL_CANT_CURE:
			*n_lines = 5u;
			memcpy(d0, "DOCTOR:             \0", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "SORRY DESSA, WE HAVE\0", 22);
			memcpy(d3, "NOT ENOUGH GOLD WITH\0", 22);
			memcpy(d4, "YOU. WE CAN DO      \0", 22);
			memcpy(d5, "NOTHING FOR YOU.    \0", 22);
		break;
		case CRYING_MOTHER:
			*n_lines = 13u;
			memcpy(d0, "MARGARET:           ", 22);
			memcpy(d1, "SIGH!... HELLO...   ", 22);
			memcpy(d2, "MY NAME IS MARGARET.", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "SIGH! OH HEALER I   ", 22);
			memcpy(d5, "KNOW YOU CAN FEEL ME", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "MY CHILD HAS BEEN LO", 22);
			memcpy(d8, "ST IN THE WOODS...  ", 22);
			memcpy(d9, "I AM SO SCARED!     ", 22);
			memcpy(d10, EMPTY_STRING_21, 22);
			memcpy(d11, "PLEASE HELP ME FIND ", 22);
			memcpy(d12, "HIM. I WILL GO WEST ", 22);
			memcpy(d13, "YOU WILL GO NORTH.  ", 22);
			help_cemetery_woman.current_step = 1u;
			help_cemetery_woman.mission_state = MISSION_STATE_STARTED;
			SpriteManagerAdd(SpriteDiary, 72, 8);
		break;
		case CHILD:
			memcpy(d0, "ROBERT:             ", 22);
			*n_lines = 6u;
			memcpy(d1, "HE HELLO            ", 22);
			memcpy(d2, "I ESCAPED FROM A    ", 22);
			memcpy(d3, "BEAR...             ", 22);			
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "CAN YOU PLEASE BRING", 22);
			memcpy(d6, "ME BACK TO MY MOM?  ", 22);
			help_cemetery_woman.current_step = 2u;
			child_hooked = 1;
		break;
		case CHILD_SAVED:
			*n_lines = 4u;
			memcpy(d0, "ROBERT:             ", 22);
			memcpy(d1, "THANK YOU HEALER!   ", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "IS SAFE HERE. I CAN ", 22);
			memcpy(d4, "NOW FIND MY WAY HOME", 22);
		break;
		default:
			GetLocalizedDialog2_EN(n_lines);
		break;
    }
}

void GetLocalizedLogItem_EN(INVITEMTYPE invitemtype) BANKED{
	switch(invitemtype){
		case INVITEM_MONEY:
			memcpy(log0, "       COINS!       ", 20);
		break;
		case INVITEM_ARROW_BASTARD:
			memcpy(log0, "     BASTARDS!      ", 20);
		break;
		case INVITEM_ARROW_NORMAL:
			memcpy(log0, "      ARROWS!       ", 20);
		break;
		case INVITEM_ARROW_PERFO:
			memcpy(log0, "   PUNCTURING!      ", 20);
		break;
		case INVITEM_FLOWER:
			memcpy(log0, "       FLOWER!      ", 20);
		break;
		case INVITEM_GLASSES:
			memcpy(log0, "      GLASSES!      ", 20);
		break;
		case INVITEM_SCORPIONTAIL:
			memcpy(log0, "   SCORPION'S TAIL! ", 20);
		break;
		case INVITEM_HERB:
			memcpy(log0, " HERBS FOR ANTIDOTE ", 20);
		break;
		case INVITEM_MUSHROOM:
			memcpy(log0, "      MUSHROOM     ", 20);
		break;
		case INVITEM_HEART:
		case INVITEM_HEARTS:
			memcpy(log0, "       HEALTH!      ", 20);
		break;
		case INVITEM_METAL:
			memcpy(log0, "       METAL!       ", 20);
		break;
		case INVITEM_POWDER:
			memcpy(log0, "      POWDER!       ", 20);
		break;
		case INVITEM_WOOD:
			memcpy(log0, "       WOOD!        ", 20);
		break;
		case INVITEM_LIAM_HANDWRITTEN:
			memcpy(log0, "    LIAM MESSAGE!   ", 20);
		break;
		case INVITEM_SILVERSKULL:
			memcpy(log0, "    SILVER SKULL    ", 20);
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
		case MARGARET:
			memcpy(log0, "MARGARET            ", 20);
		break;
		case LUKE:
			memcpy(log0, "LUKE                ", 20);
		break;
		case PAUL:
			memcpy(log0, "PAUL                ", 20);
		break;
		case CHILD_TOOFAR:
			memcpy(log0, "HEY! I AM SCARED!!  ", 20);
		break;
		case WOLF_BLACKIE:
			memcpy(log0, "BLACKIE             ", 20);
		break;
		case WOLF_BLACKIE_CHILD:
			memcpy(log0, "DEAD WOLF           ", 20);
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
		default:
			GetLocalizedLogName2_EN(npcname);
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
				case 1:memcpy(log0, "CRAB  ( ) ) ) ) )   ", 20);break;
				case 2:memcpy(log0, "CRAB  ( ( ) ) ) )   ", 20);break;
				case 3:memcpy(log0, "CRAB  ( ( ( ) ) )   ", 20);break;
				case 4:memcpy(log0, "CRAB  ( ( ( ( ) )   ", 20);break;
				case 5:memcpy(log0, "CRAB  ( ( ( ( ( )   ", 20);break;
				case 6:memcpy(log0, "CRAB  ( ( ( ( ( (   ", 20);break;
			};
		break;
		case StateBossminotaur:
			switch(minotaur_data->hp){
				case 0:memcpy(log0, "MINOTAUR:  BEATED!  ", 20);break;
				case 1:memcpy(log0, "MINOTAUR ( ) ) ) ) )", 20);break;
				case 2:memcpy(log0, "MINOTAUR ( ( ) ) ) )", 20);break;
				case 3:memcpy(log0, "MINOTAUR ( ( ( ) ) )", 20);break;
				case 4:memcpy(log0, "MINOTAUR ( ( ( ( ) )", 20);break;
				case 5:memcpy(log0, "MINOTAUR ( ( ( ( ( )", 20);break;
				case 6:memcpy(log0, "MINOTAUR ( ( ( ( ( (", 20);break;
			};
		break;
		case StateBossscorpion:
			if(find_antidote.phase < 2){
				if(scorpio_hp != scorpiohead_data->hp){
					scorpio_hp = scorpiohead_data->hp;
					switch(scorpio_hp){
						case 0:memcpy(log0, "SCORPIO: BEATED!    ", 20);break;
						case 1:memcpy(log0, "SCORPIO ( ) ) ) ) ) ", 20);break;
						case 2:memcpy(log0, "SCORPIO ( ( ) ) ) ) ", 20);break;
						case 3:memcpy(log0, "SCORPIO ( ( ( ) ) ) ", 20);break;
						case 4:memcpy(log0, "SCORPIO ( ( ( ( ) ) ", 20);break;
						case 5:memcpy(log0, "SCORPIO ( ( ( ( ( ) ", 20);break;
						case 6:memcpy(log0, "SCORPIO ( ( ( ( ( ( ", 20);break;
					};
				}
			}else{memcpy(log0, "THE SOUTH EAST CAVE ", 20);}
		break;
		case StateExzoo:
			memcpy(log0, "THE EX ZOO VILLAGE  ", 20);
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
			memcpy(log0, "THE HUMAN CEMETERY  ", 20);
		break;
		case StateHood:
			if(spawn_child_cooldown < 100u && spawn_child_cooldown > 0u){
				memcpy(log0, "HEY! WHO'S THERE?   ", 20);
			}else{
				memcpy(log0, "THE WOODS           ", 20);
			}
		break;
		case StateMine:
			memcpy(log0, "THE OLD MINE        ", 20);
		break;
		case StateOutwalkers:
			memcpy(log0, "THE OUTWALKER'S CAMP", 20);
		break;
		case StateMountain:
			memcpy(log0, "ROLLING STONES!     ", 20);
		break;
		case StateSky:
			memcpy(log0, "THUN DERS!          ", 20);
		break;
		case StateScorpions:
			memcpy(log0, "THE COUNTRY         ", 20);
		break;
		case StateCart:
			memcpy(log0, "CART IS FUN!        ", 20);
		break;
		case StateBridge:
			memcpy(log0, "THE FIXED BRIDGE    ", 20);
		break;
		case StateHarbor:
			memcpy(log0, "THE HARBOR          ", 20);
		break;
		case StateSilvercave:
			memcpy(log0, "THE SILVER CAVE     ", 20);
		break;
		case StateBatcave:
			memcpy(log0, "THE BAT CAVE        ", 20);
		break;
		case StateBossbat:
			switch(bossbat_data->hp){
				case 0:memcpy(log0, "BAT: BEATED!       ", 20);break;
				case 1:memcpy(log0, "BAT   ( ) ) ) ) )   ", 20);break;
				case 2:memcpy(log0, "BAT   ( ( ) ) ) )   ", 20);break;
				case 3:memcpy(log0, "BAT   ( ( ( ) ) )   ", 20);break;
				case 4:memcpy(log0, "BAT   ( ( ( ( ) )   ", 20);break;
				case 5:memcpy(log0, "BAT   ( ( ( ( ( )   ", 20);break;
				case 6:memcpy(log0, "BAT   ( ( ( ( ( (   ", 20);break;
			};
		break;
	}
}