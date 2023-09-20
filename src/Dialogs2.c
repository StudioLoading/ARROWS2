#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "string.h"

#include "Dialogs.h"
#include "Dialogs2.h"
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

extern void pickup(struct ItemSpawned* pickedup_data) BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;
extern UINT8 get_quantity(INVITEMTYPE itemtype) BANKED;

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
							struct ItemSpawned pass_data={.itemtype = INVITEM_ARROW_PERFO, .quantity = 20, .equippable = 1u};
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
						if(get_quantity(INVITEM_ARROW_PERFO) < 5){
							*n_lines = 9u;
							memcpy(d6, "TAKE THESE PEARCING\0", 22);
							memcpy(d7, "ARROWS: GUESS YOU  \0", 22);
							memcpy(d8, "GONNA NEED EM FOR  \0", 22);
							memcpy(d9, "THE FIGHTING...    \0", 22);
							{
								struct ItemSpawned pass_data={.itemtype = INVITEM_ARROW_PERFO, .quantity = 20, .equippable = 1u};
								pickup(&pass_data);
								outwalker_glass.mission_state = MISSION_STATE_STARTED;
								outwalker_glass.current_step = 1;
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
			memcpy(d7, "HIM SO, AND YOU     \0", 22);
			memcpy(d8, "FINALLY PASS.       \0", 22);
			outwalker_smith.mission_state = MISSION_STATE_ENABLED;
		break;
		case OUTWALKER_GUARD_OK:
				*n_lines = 6u;
				memcpy(d0, "SIMON:              \0", 22);
				memcpy(d1, "YES, THIS I THE WAY \0", 22);
				memcpy(d2, "TO THE MOUNTAINS.   \0", 22);
				memcpy(d3, EMPTY_STRING_21, 22);
				memcpy(d4, "YOU DID A LOT FOR US\0", 22);
				memcpy(d5, "AND WE THANK YOU    \0", 22);
				memcpy(d6, "NOW YOU CAN PASS!   \0", 22);
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
			memcpy(d0, "DESSA:              \0", 22);
			memcpy(d1, "UMH... IT'S NOT SAFE\0", 22);
			memcpy(d2, "TO ENTER HERE YET.  \0", 22);
		break;
		case BOSS_CRAB_FIGHT:
			*n_lines = 11u;
			memcpy(d0, "BIG BOSS CRAB:      \0", 22);
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
		case IBEX_GIVE_MISSION:
			*n_lines = 17u;
			memcpy(d0, "LORD OF THUNDER:    \0", 22);
			memcpy(d1, "*COUGH!* *COUGH!*   \0", 22);
			memcpy(d2, "SALUTE *COUGH!* HEAL\0", 22);
			memcpy(d3, "ER. BLACKIE ALREADY \0", 22);
			memcpy(d4, "EXPLAINED ME WHAT   \0", 22);
			memcpy(d5, "HAPPENED TO YOUR SON\0", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "OUT OF NOWHRE A COLO\0", 22);
			memcpy(d8, "NY OF SCORPIONS APPE\0", 22);
			memcpy(d9, "ARED AND IS MARCHING\0", 22);
			memcpy(d10, "DOWN TO THE *COUGH!*\0", 22);
			memcpy(d11, "VILLAGE!            \0", 22);
			memcpy(d12, EMPTY_STRING_21, 22);
			memcpy(d13, "AS YOU CAN IMAGINE, \0", 22);
			memcpy(d14, "THOSE BASTARDS HAVE \0", 22);
			memcpy(d15, "A LETHAL POISON IN  \0", 22);
			memcpy(d16, "THOSE STINGS. HERE  \0", 22);
			memcpy(d17, EMPTY_STRING_21, 22);
		break;
		case IBEX_GIVE_HERBS:
			*n_lines = 17u;
			memcpy(d0, "LORD OF THUNDER:    \0", 22);
			memcpy(d1, "*COUGH!* *COUGH!*   \0", 22);
			memcpy(d2, "TAKE THESE HERBS.   \0", 22);
			memcpy(d3, "I ALREADY ATE A     \0", 22);
			memcpy(d4, "BUNCH. I AM SLOWLY  \0", 22);
			memcpy(d5, "REVOVERING.         \0", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "TAKE THESE TO THE   \0", 22);
			memcpy(d8, "HUMANS, YOU KNOW    \0", 22);
			memcpy(d9, "IT CAN HEAL THEM.   \0", 22);
			memcpy(d10, EMPTY_STRING_21, 22);
			memcpy(d11, "NOW LEAVE! THROUGH \0", 22);
			memcpy(d12, "THAT CAVE YOU'LL   \0", 22);
			memcpy(d13, "GET DOWN THE MOUNTA\0", 22);
			memcpy(d14, "IN FASTER!         \0", 22);
			memcpy(d15, EMPTY_STRING_21, 22);
			memcpy(d16, "I'LL SEE YOU, AGAIN\0", 22);
			memcpy(d17, EMPTY_STRING_21, 22);
		break;
    }
}