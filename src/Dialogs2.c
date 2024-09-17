#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"

#include "Dialogs.h"
#include "Dialogs2.h"
#include "Dialogs3.h"
#include "custom_datas.h"

extern unsigned char log0[21];
extern unsigned char m0[17];
extern unsigned char m1[17];
extern unsigned char m2[17];
extern unsigned char m3[17];
extern unsigned char d0[22];
extern unsigned char d1[22];
extern unsigned char d2[22];
extern unsigned char d3[22];
extern unsigned char d4[22];
extern unsigned char d5[22];
extern unsigned char d6[22];
extern unsigned char d7[22];
extern unsigned char d8[22];
extern unsigned char d9[22];
extern unsigned char d10[22];
extern unsigned char d11[22];
extern unsigned char d12[22];
extern unsigned char d13[22];
extern unsigned char d14[22];
extern unsigned char d15[22];
extern unsigned char d16[22];
extern unsigned char d17[22];
extern unsigned char d18[22];
extern unsigned char d19[22];
extern WHOSTALKING whostalking;
extern unsigned char EMPTY_STRING_21[22];
extern unsigned char D0[22];
extern UINT8 choice;
extern UINT8 choice_left;
extern UINT8 choice_right;
extern INT8 outwalker_info_step;
extern INT8 outwalker_info_given;
extern struct MISSION outwalker_chief;
extern struct MISSION outwalker_glass;
extern struct MISSION outwalker_smith;
extern struct MISSION find_antidote;
extern struct MISSION hungry_people;
extern struct MISSION golden_armor;
extern struct MISSION fix_bridge;

extern void pickup(struct ItemSpawned* pickedup_data) BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;
extern UINT8 get_quantity(INVITEMTYPE itemtype) BANKED;


void GetLocalizedLogName2_EN(NPCNAME npcname) BANKED{
	switch(npcname){
		case SMEE:
			memcpy(log0, "MR SMEE             ", 20);
		break;
		case ONE_EYED_JACK:
			memcpy(log0, "ONE EYED JACK       ", 20);
		break;
		case MARTIN:
			memcpy(log0, "MARTIN              ", 20);
		break;
		case BOB:
			memcpy(log0, "BOB                 ", 20);
		break;
		case MAURICE:
			memcpy(log0, "MAURICE             ", 20);
		break;
		case RICK:
			memcpy(log0, "HEADLESS RICK       ", 20);
		break;
	}
}

void GetLocalizedDialog2_EN(UINT8* n_lines) BANKED{
    switch(whostalking){		
		case OUTWALKER_NO_ENTER:
			*n_lines = 5u;
			memcpy(d0, "OUTWALKER:          \0", 22);
			memcpy(d1, "SORRY HEALER,       \0", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "YOU CAN NOT GET IN  \0", 22);
			memcpy(d4, "HERE WITHOUT A PASS.\0", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
		break;
		case OUTWALKER_MAN1:
			*n_lines = 12u;
			memcpy(d0, "JASON:              \0", 22);
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
			memcpy(d0, "JERRY:              \0", 22);
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
			memcpy(d0, "ANNETTE:            \0", 22);
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
		case JESSICA_PLANTS:
			*n_lines = 5u;
			memcpy(d0, "JESSICA:            ", 22);
			memcpy(d1, "TO COMPLETE THE ANTI", 22);
			memcpy(d2, "DOTE YOU NEED TO    ", 22);
			memcpy(d3, "FIND FEW MORE HERBS ", 22);
			memcpy(d4, "LIKE THIS ONE. IT   ", 22);
			memcpy(d5, "GROWS SOUTH.        ", 22);
			if(find_antidote.current_step == 0){
				memcpy(d6, "HERE, I FOUND ONE OF", 22);
				memcpy(d7, "THEM, I THINK IT CAN", 22);
				memcpy(d8, "BE HELPFUL!         ", 22);
				*n_lines = 8u;				
				struct ItemSpawned herb_data={.itemtype = INVITEM_HERB, .quantity = 1, .equippable = 0u};
				pickup(&herb_data);
				find_antidote.phase = 4;
				find_antidote.current_step = 1;
				SpriteManagerAdd(SpriteDiary, 72, 8);
			
			}
		break;
		case OUTWALKER_WOMAN2:
			*n_lines = 8u;
			memcpy(d0, "JESSICA:            \0", 22);
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
				memcpy(d0, "JACK:               \0", 22);
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
							struct ItemSpawned perfo_data={.itemtype = INVITEM_ARROW_PERFO, .quantity = 20, .equippable = 1u};
							pickup(&perfo_data);
							outwalker_glass.mission_state = MISSION_STATE_STARTED;
							outwalker_glass.current_step = 1;
							SpriteManagerAdd(SpriteDiary, 72, 8);
						}
					break;
					case 1u://occhiali non ancora trovati
						*n_lines = 5u;
						memcpy(d1, "I MISS MY GLASSES BY\0", 22);
						memcpy(d2, "THE SEE... CAN YOU  \0", 22);
						memcpy(d3, "PLEASE BRING THOSE  \0", 22);
						memcpy(d4, "BACK?               \0", 22);
						memcpy(d5, EMPTY_STRING_21, 22);
						if(get_quantity(INVITEM_ARROW_PERFO) < 5){
							*n_lines = 9u;
							memcpy(d6, "TAKE THESE PEARCING\0", 22);
							memcpy(d7, "ARROWS: GUESS YOU  \0", 22);
							memcpy(d8, "GONNA NEED EM FOR  \0", 22);
							memcpy(d9, "THE FIGHTING...    \0", 22);
							{
								struct ItemSpawned pearc_data={.itemtype = INVITEM_ARROW_PERFO, .quantity = 20, .equippable = 1u};
								pickup(&pearc_data);
								outwalker_glass.mission_state = MISSION_STATE_STARTED;
								outwalker_glass.current_step = 1;
								SpriteManagerAdd(SpriteDiary,  72, 8);
							}
						}
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
							struct ItemSpawned money_data={.itemtype = INVITEM_MONEY, .quantity = 40, .equippable = 1u};
							pickup(&money_data);
							change_quantity(INVITEM_GLASSES, -1);
							outwalker_glass.mission_state = MISSION_STATE_REWARDED;
							outwalker_glass.current_step = 4;
							SpriteManagerAdd(SpriteDiary, 72, 8);
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
			memcpy(d0, "SIMON:              \0", 22);
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
			memcpy(d0, "SIMON:              \0", 22);
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
			SpriteManagerAdd(SpriteDiary, 72, 8);
		break;
		case OUTWALKER_GUARD_NOSMITH:
			*n_lines = 8u;
			memcpy(d0, "SIMON:              \0", 22);
			memcpy(d1, "YES, THIS I THE WAY \0", 22);
			memcpy(d2, "TO THE MOUNTAINS.   \0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "WE NEED SOME WEAPONS\0", 22);
			memcpy(d5, "AND THE SMITH CAN   \0", 22);
			memcpy(d6, "FORGE EM FOR US. ASK\0", 22);
			memcpy(d7, "HIM SO AND YOU      \0", 22);
			memcpy(d8, "FINALLY PASS.       \0", 22);
			outwalker_smith.mission_state = MISSION_STATE_ENABLED;
		break;
		case OUTWALKER_GUARD_OK:
				*n_lines = 9u;
				memcpy(d0, "SIMON:              \0", 22);
				memcpy(d1, "YES  THIS I THE WAY \0", 22);
				memcpy(d2, "TO THE MOUNTAINS.   \0", 22);
				memcpy(d3, EMPTY_STRING_21, 22);
				memcpy(d4, "THANK YOU, THIS BOX \0", 22);
				memcpy(d5, "MEANS A LOT FOR US! \0", 22);
				memcpy(d6, EMPTY_STRING_21, 22);
				memcpy(d7, "YOU DID A LOT FOR US\0", 22);
				memcpy(d8, "AND WE THANK YOU    \0", 22);
				memcpy(d9, "NOW YOU CAN PASS!   \0", 22);
		break;
		case OUTWALKER_CHIEF_FOUND:
			*n_lines = 18u;
			memcpy(d0, "PAUL:               \0", 22);
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
			memcpy(d12, "AM AROUND.          \0", 22);
			memcpy(d13, EMPTY_STRING_21, 22);
			memcpy(d14, "PLEASE DON'T CHANGE \0", 22);
			memcpy(d15, "THIS, DON'T REVEAL  \0", 22);
			memcpy(d16, "THIS. HERE, TAKE:   \0", 22);
			memcpy(d17, "100 COINS FOR YOUR  \0", 22);
			memcpy(d18, "SILENCE.            \0", 22);
            outwalker_chief.current_step = 3;
			outwalker_info_step = 3;
			SpriteManagerAdd(SpriteDiary, 72, 8);
			outwalker_chief.mission_state = MISSION_STATE_REWARDED;
			{
				struct ItemSpawned money_data={.itemtype = INVITEM_MONEY, .quantity = 100, .equippable = 1u};
				pickup(&money_data);
			}
		break;
		case OUTWALKER_GUARD_LANDSLIDE:
			*n_lines = 7u;
			memcpy(d0, "SIMON:              \0", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "SINCE THE LAST TIME \0", 22);
			memcpy(d3, "WE SAW YOU A LANDSLI\0", 22);
			memcpy(d4, "DE HAS FALLEN: UNFOR\0", 22);
			memcpy(d5, "TUNATELY WE CAN NO  \0", 22);
			memcpy(d6, "LONGER PASS THROUGH \0", 22);
			memcpy(d7, "HERE!               \0", 22);
		break;
		case MAZE_CANT_GO:
			*n_lines = 2u;
			memcpy(d0, "DESSA:              \0", 22);
			memcpy(d1, "UMH... IT'S NOT SAFE\0", 22);
			memcpy(d2, "TO ENTER HERE YET.  \0", 22);
		break;
		case BOSS_CRAB_FIGHT:
			*n_lines = 11u;
			memcpy(d0, "BIG BOSS CRAB:       ", 22);
			memcpy(d1, "'CLICK! CLICK!'      ", 22);
			memcpy(d2, "WITH THESE GLASSES I ", 22);
			memcpy(d3, "SEE A LOT BETTER...  ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "WHAT? I FOUND IT!    ", 22);
			memcpy(d6, "IT'S MINE NOW! OH    ", 22);
			memcpy(d7, "THEY AREN'T?...\0", 22);
			memcpy(d8, EMPTY_STRING_21, 22);
			memcpy(d9, "'CLICK! CLICK!'      ", 22);
			memcpy(d10, EMPTY_STRING_21, 22);
			memcpy(d11, "WE'LL SEE...         ", 22);
		break;
		case IBEX_GIVE_MISSION:
			*n_lines = 17u;
			memcpy(d0, "LORD OF THUNDER:     ", 22);
			memcpy(d1, "'COUGH! COUGH!'      ", 22);
			memcpy(d2, "SALUTE *COUGH!* HEAL ", 22);
			memcpy(d3, "ER. BLACKIE ALREADY  ", 22);
			memcpy(d4, "EXPLAINED ME WHAT    ", 22);
			memcpy(d5, "HAPPENED TO YOUR SON ", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "OUT OF NOWHRE A COLO ", 22);
			memcpy(d8, "NY OF SCORPIONS APPE ", 22);
			memcpy(d9, "ARED AND IS MARCHING ", 22);
			memcpy(d10, "DOWN TO THE 'COUGH!' ", 22);
			memcpy(d11, "VILLAGE!             ", 22);
			memcpy(d12, EMPTY_STRING_21, 22);
			memcpy(d13, "AS YOU CAN IMAGINE,  ", 22);
			memcpy(d14, "THOSE BASTARDS HAVE  ", 22);
			memcpy(d15, "A LETHAL POISON IN   ", 22);
			memcpy(d16, "THEIR STINGS. HERE   ", 22);
			memcpy(d17, EMPTY_STRING_21, 22);
		break;
		case IBEX_GIVE_HERBS:
			*n_lines = 14u;
			memcpy(d0, "LORD OF THUNDER:    ", 22);
			memcpy(d1, "'COUGH! COUGH!'     ", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "I ALREADY ATE A     ", 22);
			memcpy(d4, "BUNCH OF HERBS. I AM", 22);
			memcpy(d5, "SLOWLY REVOVERING   ", 22);
			memcpy(d6, "ALREADY...          ", 22);
			memcpy(d7, EMPTY_STRING_21, 22);
			memcpy(d8, "NOW LEAVE! THROUGH \0", 22);
			memcpy(d9, "THAT CAVE YOU'LL   \0", 22);
			memcpy(d10, "GET DOWN THE MOUNTA\0", 22);
			memcpy(d11, "IN FASTER!         \0", 22);
			memcpy(d12, EMPTY_STRING_21, 22);
			memcpy(d13, "I'LL SEE YOU, AGAIN\0", 22);
			memcpy(d14, EMPTY_STRING_21, 22);
		break;
		case HOSPITAL_HEAL_1:
			*n_lines = 6u;
			memcpy(d0, "DOCTOR:             \0", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "SCORPIONS ARE POISON\0", 22);
			memcpy(d3, "ING EVERBODY HERE!  \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "WE ARE SORRY! WE CAN\0", 22);
			memcpy(d6, "NOT FULLY HEAL YOU! \0", 22);
		break;
		case HOSPITAL_GET_ANTIDOTE:
			*n_lines = 10u;
			memcpy(d0, "DOCTOR:             \0", 22);
			memcpy(d1, "YOU DEFEAT THE HORDE\0", 22);
			memcpy(d2, "THANK YOU HEALER!   \0", 22);
			memcpy(d3, "NOW WE NEED THE ANTI\0", 22);
			memcpy(d4, "DOTE TO HEAL ALL THE\0", 22);
			memcpy(d5, "POISONED HUMANS...  \0", 22);
			memcpy(d6, "AND BEASTS. PROVIDE \0", 22);
			memcpy(d7, "US THE BIGGEST QUAN \0", 22);
			memcpy(d8, "TITY OF POISON YOU  \0", 22);
			memcpy(d9, "CAN GET.            \0", 22);
			find_antidote.mission_state = MISSION_STATE_STARTED;
			find_antidote.current_step = 0;
			find_antidote.phase = 1;
		break;
		case HOSPITAL_THANKS_FOR_TAIL:
			*n_lines = 11u;
			memcpy(d0, "DOCTOR:             \0", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "YOU DID IT! THIS IS \0", 22);
			memcpy(d3, "THE FIRST INGREDIENT\0", 22);
			memcpy(d4, "WE NEED TO PREPARE  \0", 22);
			memcpy(d5, "THE ANTIDOTE. THE SE\0", 22);
			memcpy(d6, "COND INGREDIENT ARE \0", 22);
			memcpy(d7, "SOME HERBS. PLEASE  \0", 22);
			memcpy(d8, "GO TALK TO JESSICA  \0", 22);
			memcpy(d9, "THE OUTWALKER: SHE  \0", 22);
			memcpy(d10, "KNOWS WHAT AND WHERE\0", 22);
			memcpy(d11, "TO COLLECT THEM.    \0", 22);
		break;
		case HOSPITAL_GO_FOR_HERBS:
			*n_lines = 7u;
			memcpy(d0, "DOCTOR:             \0", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "BRING US THE HERBS  \0", 22);
			memcpy(d3, "AND WE WILL HEAL    \0", 22);
			memcpy(d4, "EVERYONE! EIGHT     \0", 22);
			memcpy(d5, "ROOTS SHOULD BE     \0", 22);
			memcpy(d6, "ENOUGH TO COVER ALL \0", 22);
			memcpy(d7, "OF US.              \0", 22);
		break;
		case HOSPITAL_ANTIDOTE_BUILT:
			*n_lines = 11u;
			memcpy(d0, "DOCTOR:             \0", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "YOU DID IT! NOW WE  \0", 22);
			memcpy(d3, "CAN CREATE THE      \0", 22);
			memcpy(d4, "ANTIDOTE! YOU SAVED \0", 22);
			memcpy(d5, "US! OH, THE MOST OF \0", 22);
			memcpy(d6, "US. I AM AFRAID     \0", 22);
			memcpy(d7, "BLACKIE IS IN DANGER\0", 22);
			memcpy(d8, "...                 \0", 22);
			memcpy(d9, EMPTY_STRING_21, 22);
			memcpy(d10, "SHE NEEDS YOU NOW  \0", 22);
			memcpy(d11, "GO VISIT HER.      \0", 22);
			find_antidote.mission_state = MISSION_STATE_ACCOMPLISHED;
			find_antidote.phase = 5;
		break;
		case CARPENTER_DISABLED:
			*n_lines = 7u;
			memcpy(d0, "DESSA:              \0", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "  THERE'S NO ONE    \0", 22);
			memcpy(d3, "IN HERE...          \0", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "I SHOULD FIRSTLY    \0", 22);
			memcpy(d6, "CLEAR THE AREA OF   \0", 22);
			memcpy(d7, "SCORPIONS.          \0", 22);
		break;
		case CARPENTER_CHECKING_NOWOODANDMETAL:
			*n_lines = 9u;
			memcpy(d0, "PETER:              \0", 22);
			memcpy(d1, "HI THERE, CAN I HELP\0", 22);
			memcpy(d2, "YOU? I CAN CRAFT PEA\0", 22);
			memcpy(d3, "RCING ARROWS, BUT I ", 22);
			memcpy(d4, "NEED 15 PEACES OF WO", 22);
			memcpy(d5, "OD AND OF METAL.    ", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "PLEASE COME BACK WI ", 22);
			memcpy(d8, "TH THOSE AND I CAN  ", 22);
			memcpy(d9, "GIVE YOU 30 ARROWS! ", 22);
		break;
		case CARPENTER_GIVING_ARROWS:
			*n_lines = 6u;
			memcpy(d0, "PETER:               ", 22);
			memcpy(d1, "'TZIN! TZIN!'        ", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "HERE, 30 PEARCING    ", 22);
			memcpy(d4, "ARROWS FOR YOU!      ", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "HAVE A NICE DAY!     ", 22);
		break;
		case CARPENTER_FIX_BRIDGE:
			*n_lines = 9u;
			memcpy(d0, "PETER:               ", 22);
			memcpy(d1, "ENOUGH! I CAN NOW    ", 22);
			memcpy(d2, "FIX THE BRIDGE, THANK", 22);
			memcpy(d3, "YOU VERY MUCH!       ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "I THINK NOW YOU CAN  ", 22);
			memcpy(d6, "GO TO THE OTHER SIDE ", 22);
			memcpy(d7, "OF IT. BUT BE CAREFUL", 22);
			memcpy(d8, "I'VE HEARD THERE'S A ", 22);
			memcpy(d9, "MINOTAUR THERE...!   ", 22);
		break;
		case SMITH_GIVING_ARROWS:
			*n_lines = 8u;
			memcpy(d0, "JOHN:               ", 22);
			memcpy(d1, "AAAAALRIIIGHT!      ", 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, "'SPLINK! SPLINK!'   ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "HERE THEY ARE: 30   ", 22);
			memcpy(d7, "NORMAL ARROWS       ", 22);	
			memcpy(d8, "FOR YOU.            ", 22);			
		break;
		case CARPENTER_NEED_MATERIALS:
			*n_lines = 8u;
			memcpy(d0, "PETER:              ", 22);
			memcpy(d1, "YES, I WOULD BE ABLE", 22);
			memcpy(d2, "TO FIX THAT BRIDGE. ", 22);
			memcpy(d3, "OF COURSE I NEED MAT", 22);
			memcpy(d4, "ERIALS. 20 PIECES OF", 22);
			memcpy(d5, "WOOD AND 20 OF METAL", 22);
			memcpy(d6, "TO FIX THAT BRIDGE. ", 22);
			memcpy(d7, EMPTY_STRING_21, 22);
			memcpy(d8, "JUST BRING IT. OK?  ", 22);
		break;
		case POLICE_0_GET_PASS:
			*n_lines = 15u;
			memcpy(d0, "GUARD:              ", 22);
			memcpy(d1, "SALUTE HEALER,      ", 22);
			memcpy(d2, "WE KNOW THAT TO THE ", 22);
			memcpy(d3, "WEST THERE'S A BUNCH", 22);
			memcpy(d4, "OF PEOPLE LIVING BY ", 22);
			memcpy(d5, "THEIR OWN. WE WANT  ", 22);
			memcpy(d6, "TO KNOW WHO IS THEIR", 22);
			memcpy(d7, "LEADER. BECAUSE THEY", 22);
			memcpy(d8, "WON'T TELL US. TAKE ", 22);
			memcpy(d9, "THIS PASS. SHOW IT  ", 22);
			memcpy(d10, "AND THEY WILL LET  ", 22);
			memcpy(d11, "YOU IN.            ", 22);
			memcpy(d12, EMPTY_STRING_21, 22);
			memcpy(d13, "PLEASE COME BACK AS", 22);
			memcpy(d14, "SOON AS YOU FIND   ", 22);
			memcpy(d15, "OUT. THANK YOU.    ", 22);				
			{
				if(get_quantity(INVITEM_PASS) < 1){
					struct ItemSpawned pass_data={.itemtype = INVITEM_PASS, .quantity = 1, .equippable = 0u};
					pickup(&pass_data);
				}
				outwalker_chief.mission_state = MISSION_STATE_ENABLED;
				outwalker_chief.current_step = 1;
				SpriteManagerAdd(SpriteDiary, 72, 8);
			}
		break;
		case POLICE_0_STILL_NOT_FOUND:
			*n_lines = 8u;
			memcpy(d0, "GUARD:              ", 22);
			memcpy(d1, "SALUTE HEALER,      ", 22);
			memcpy(d2, "WE ARE STILL WAITING", 22);
			memcpy(d3, "FOR YOU TO TELL US  ", 22);
			memcpy(d4, "WHO'S THE OUTWALKER ", 22);
			memcpy(d4, "CHIEF.              ", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "PLEASE COME BACK AS ", 22);
			memcpy(d7, "SOON AS YOU FIND    ", 22);
			memcpy(d8, "OUT. THANK YOU.     ", 22);
		break;
		case POLICE_0_WONT_TALK:
			*n_lines = 8u;
			memcpy(d0, "GUARD:              ", 22);
			memcpy(d1, "HOW DARE YOU NOT TO ", 22);
			memcpy(d2, "TELL US THIS PRECIO ", 22);
			memcpy(d3, "US INFORMATION!!    ", 22);
			memcpy(d4, "THIS IS A BAD DECI  ", 22);
			memcpy(d5, "SION ...            ", 22);
			memcpy(d6, "...AND WILL HAVE    ", 22);
			memcpy(d7, "CONSEQUENCES!       ", 22);
			memcpy(d8, EMPTY_STRING_21, 22);
		break;
		case POLICE_0_FIGHTING:
			*n_lines = 5u;
			memcpy(d0, "GUARD:              ", 22);
			memcpy(d1, "SCORPIONS ARE ALL   ", 22);
			memcpy(d2, "AROUND US!          ", 22);
			memcpy(d4, "A BAD DECISION...   ", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
		break;
		case POLICE_0_NOGUARDS:
			*n_lines = 3u;
			memcpy(d0, "DESSA:              ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "NO GUARDS HERE AT   ", 22);
			memcpy(d3, "THE MOMENT...       ", 22);
		break;
		case BLACKIE_DEAD_CHILD:
			*n_lines = 19u;
			memcpy(d0, "BLACKIE:            ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "OH... MY CHILD! HE'S", 22);
			memcpy(d3, "BEEN ONE OF THE FIR ", 22);
			memcpy(d4, "ST POISONED...      ", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "            ...SIGH!", 22);
			memcpy(d7, ".. AND IT KILLED HIM", 22);
			memcpy(d8, "... YOU SEE DESSA!? ", 22);
			memcpy(d9, "BET THEY CAME TO    ", 22);
			memcpy(d10, "KILL US! AND WE    ", 22);
			memcpy(d11, "MUST FIGHT BACK!!  ", 22);
			memcpy(d12, "NO MORE CHILD HAVE ", 22);
			memcpy(d13, "TO DIE!      DESSA:", 22);
			memcpy(d14, "I SWEAR I'LL DEFEAT", 22);
			memcpy(d15, "WHO CAUSED THIS!   ", 22);
			memcpy(d16, "YOUR SON WILL BE   ", 22);
			memcpy(d17, "REVENGED WITH THE  ", 22);
			memcpy(d18, "STRENGTH OF MY     ", 22);
			memcpy(d19, "ARROWS.            ", 22);
		break;
		case IMHUNGRY:
			*n_lines = 5u;
			memcpy(d0, "'TOUCHING STOMACH': ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "I AM SORRY I CAN'T  ", 22);
			memcpy(d3, "HELP YOU NOW... I AM", 22);
			memcpy(d4, "TOO HUNGRY FOR DOING", 22);
			memcpy(d5, "ANYTHING...         ", 22);
		break;
		case FISHERMAN_THERESFISH:
			*n_lines = 8u;
			memcpy(d0, "FISHERMAN           ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "I LIKE TO GO FISHING", 22);
			memcpy(d3, "RIGHT THERE, NEXT TO", 22);
			memcpy(d4, "THE BROKEN BRIDGE.  ", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "I PROVIDE SOME FOOD ", 22);
			memcpy(d7, "FOR ALL OF US!      ", 22);
			memcpy(d8, EMPTY_STRING_21, 22);
		break;
		case FISHERMAN_LETSGO:
			*n_lines = 19u;
			memcpy(d0, "FISHERMAN:          ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "HELLO HEALER! PEOPLE", 22);
			memcpy(d3, "ARE STARVING! THERE ", 22);
			memcpy(d4, "IS AN AREA WITH A   ", 22);
			memcpy(d5, "LOT OF FISH TO GET  ", 22);
			memcpy(d6, "BUT NOW...          ", 22);
			memcpy(d7, "IT IS TOO DANGEROUS ", 22);
			memcpy(d8, "TO GO ALONE!        ", 22);
			memcpy(d9, EMPTY_STRING_21, 22);
			memcpy(d10, "I COUNTED 12 ALLIGA ", 22);
			memcpy(d11, "TORS THERE AND THEY ", 22);
			memcpy(d12, "ARE GOING TO RIP ME ", 22);
			memcpy(d13, "UP!                 ", 22);
			memcpy(d14, EMPTY_STRING_21, 22);
			memcpy(d15, "I RIDE THE BOAT  YOU", 22);
			memcpy(d16, "SLAY THE ALLIGATORS.", 22);
			memcpy(d17, EMPTY_STRING_21, 22);
			memcpy(d18, "WHAT ABOUT THAT?    ", 22);
			memcpy(d19, "LET US GO TOGETHER! ", 22);
			hungry_people.mission_state = MISSION_STATE_STARTED;
		break;
		case FISHERMAN_FPSGATOR_COMPLETED:
			*n_lines = 9u;
			memcpy(d0, "FISHERMAN:          ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "GOOD JOB! NOW I CAN ", 22);
			memcpy(d3, "GO BACK FISHING!    ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "WE HAVE PUT AN END  ", 22);
			memcpy(d6, "TO THE STARVATION!  ", 22);
			memcpy(d7, EMPTY_STRING_21, 22);
			memcpy(d8, "LET'S GO BACK TO THE", 22);
			memcpy(d9, "BEACH.              ", 22);
			hungry_people.mission_state = MISSION_STATE_ACCOMPLISHED;
		break;
		case ITEMDETAIL_MAP:
			*n_lines = 19u;
			memcpy(d0, "LIAM HANDWRITTEN    ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "HELLO MOTHER! I HOPE", 22);
			memcpy(d3, "THIS LETTER FINDS   ", 22);
			memcpy(d4, "YOU WELL. THE MASTER", 22);
			memcpy(d5, "IS TRAINING ME FOR  ", 22);
			memcpy(d6, "THE FINAL BATTLE.   ", 22);
			memcpy(d7, "DON'T WORRY ABOUT ME", 22);
			memcpy(d8, "BECAUSE I AM FINE   ", 22);
			memcpy(d9, "AND STRONGER THAN BE", 22);
			memcpy(d10, "FORE.               ", 22);
			memcpy(d11, EMPTY_STRING_21, 22);
			memcpy(d12, "HERE WE FOUND AN OLD", 22);
			memcpy(d13, "BOOK ABOUT LEGENDARY", 22);
			memcpy(d14, "BEASTS AND WE DISCO ", 22);
			memcpy(d15, "VERED GOLDEN ARMOR  ", 22);
			memcpy(d16, "WILL PROTECT US FROM", 22);
			memcpy(d17, "DRAGON BREATHS. THE ", 22);
			memcpy(d18, "SMITH SHOULD BE ABLE", 22);
			memcpy(d19, "FORGE IT. LOVE, LIAM", 22);
			golden_armor.phase = 0;
			golden_armor.mission_state = MISSION_STATE_STARTED;
		break;
		case BRIDGE_BROKEN:
			*n_lines = 5u;
			memcpy(d0, "DESSA:              ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "THE BRIDGE IS BROKEN", 22);
			if(fix_bridge.mission_state < MISSION_STATE_STARTED){
				memcpy(d3, "I WONDER IF THE     ", 22);
				memcpy(d4, "CARPENTER WOULD BE  ", 22);
				memcpy(d5, "ABLE TO FIX IT...   ", 22);
			}else{
				memcpy(d3, "I MUST HELP THE     ", 22);
				memcpy(d4, "CARPENTER AND BRING ", 22);
				memcpy(d5, "HIM THE NEEDED      ", 22);
				memcpy(d6, "MATERIALS.          ", 22);
				*n_lines = 6u;
			}
		break;
		default:
			GetLocalizedDialog3_EN(n_lines);
		break;
    }
}