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
extern INT8 outwalker_info_given;
extern struct MISSION outwalker_chief;
extern struct MISSION outwalker_glass;
extern struct MISSION outwalker_smith;
extern struct MISSION find_antidote;
extern struct MISSION visit_blackie;
extern struct MISSION fix_bridge;

extern INT8 outwalker_info_step = 0;

extern void pickup(struct ItemSpawned* pickedup_data) BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;
extern UINT8 get_quantity(INVITEMTYPE itemtype) BANKED;
extern void play_music_missionaccomplished() BANKED;


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
					memcpy(d1, "SO YOU WANT TO KNOW ", 22);
					memcpy(d2, "WHO OUR CHIEF IS?   ", 22);
					memcpy(d3, EMPTY_STRING_21, 22);
					memcpy(d4, "GIVE ME 30 COINS AND", 22);
					memcpy(d5, "I WILL GIVE YOU SOME", 22);
					memcpy(d6, "CLUES.              ", 22);
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
							*n_lines = 7u;
							memcpy(d1, "OH NICE! 30 COINS!  ", 22);
							memcpy(d2, EMPTY_STRING_21, 22);
							memcpy(d3, EMPTY_STRING_21, 22);
							memcpy(d4, "LISTEN: THE CHIEF   ", 22);
							memcpy(d5, "LIKES TO HANG AT THE", 22);
							memcpy(d6, "EXIT OF THE MAZE    ", 22);
							memcpy(d7, EMPTY_STRING_21, 22);
							choice = 0u;
							choice_left = 0u;
							choice_right = 0u;
							outwalker_info_step = 0;
							outwalker_chief.current_step = 1;
						break;
						case 2:
							*n_lines = 9u;
							memcpy(d1, "OH NICE! 30 COINS!  ", 22);
							memcpy(d2, EMPTY_STRING_21, 22);
							memcpy(d3, EMPTY_STRING_21, 22);
							memcpy(d4, "LISTEN: THE ONLY WAY", 22);
							memcpy(d5, "FOR YOU TO BE       ", 22);
							memcpy(d6, "NOTICED BY THE CHIEF", 22);
							memcpy(d7, "IS TO POP IN FRONT  ", 22);
							memcpy(d8, "OF HIM.             ", 22);
							memcpy(d9, EMPTY_STRING_21, 22);
							choice = 0u;
							choice_left = 0u;
							choice_right = 0u;
							outwalker_info_step = 1;
						break;
						case 3:
							*n_lines = 8u;
							memcpy(d1, "OH NICE! 30 COINS!  ", 22);
							memcpy(d2, EMPTY_STRING_21, 22);
							memcpy(d3, EMPTY_STRING_21, 22);
							memcpy(d4, "LISTEN: THE MAZE IS ", 22);
							memcpy(d5, "OUR SECRET PASSAGE  ", 22);
							memcpy(d6, "TO THE VILLAGE!     ", 22);
							memcpy(d7, EMPTY_STRING_21, 22);
							memcpy(d8, EMPTY_STRING_21, 22);
							memcpy(d9, "WITH THIS AMOUNT OF ", 22);
							memcpy(d10, "MONEY I AM GOING TO ", 22);
							memcpy(d11, "BUY A REAL HOUSE!   ", 22);
							memcpy(d12, "THIS PLACE AND THESE", 22);
							memcpy(d13, "PEOPLE SMELL LIKE   ", 22);
							memcpy(d14, "SHIT.               ", 22);
							memcpy(d15, EMPTY_STRING_21, 22);
							memcpy(d16, "SO LONG, CHUMP!!    ", 22);
							//con questa cifra mi compro una casa
							//c'è bisogno di più creduloni come te nel mondo!
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
			*n_lines = 15u;
			memcpy(d0, "ANNETTE:            ", 22);
			memcpy(d1, "HELLO STRANGER, WE  ", 22);
			memcpy(d2, "ARE THE OUTWALKERS. ", 22);
			memcpy(d3, "WE BELIEVE THAT,    ", 22);
			memcpy(d4, "EVEN IF THE ANIMALS ", 22);
			memcpy(d5, "ARE NOW INTELLIGENT,", 22);
			memcpy(d6, "THEY ARE STILL WILD.", 22);
			memcpy(d7, EMPTY_STRING_21, 22);
			memcpy(d8, "THEY ARE TAMING THE ", 22);
			memcpy(d9, "HUMANS AT THE VILLA ", 22);
			memcpy(d10, "GE.                 ", 22);
			memcpy(d11, EMPTY_STRING_21, 22);
			memcpy(d12, "ALL I WANT TO SAY IS", 22);
			memcpy(d13, "THAT WE SHOULD NOT  ", 22);
			memcpy(d14, "TRUST THEM.         ", 22);
			memcpy(d15, EMPTY_STRING_21, 22);
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
			memcpy(d1, "WE ARE CALLED THE   \0", 22);
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
						memcpy(d1, "I'VE LOST MY GLASSES", 22);
						memcpy(d2, "BY THE SEE, I SAW A ", 22);
						memcpy(d3, "GIANT CRAB! HE AT   ", 22);
						memcpy(d4, "TACKED ME AND I RUN ", 22);
						memcpy(d5, "AWAY.               ", 22);
						memcpy(d6, "NOW THAT YOU KNOW   ", 22);
						memcpy(d7, "WHO IS OUR CHIEF I  ", 22);
						memcpy(d8, "CAN TRUST YOU. CAN  ", 22);
						memcpy(d9, "YOU PLEASE BRING MY ", 22);
						memcpy(d10, "GLASSES BACK?      ", 22);
						memcpy(d11, EMPTY_STRING_21, 22);
						memcpy(d12, "TAKE THESE PEARCING", 22);
						memcpy(d13, "ARROWS: GUESS YOU  ", 22);
						memcpy(d14, "GONNA NEED EM TO   ", 22);
						memcpy(d15, "FIGHT THAT BEAST.  ", 22);
						if(outwalker_glass.mission_state < MISSION_STATE_STARTED){
							struct ItemSpawned perfo_data={.itemtype = INVITEM_ARROW_PERFO, .quantity = 20, .equippable = 1u};
							pickup(&perfo_data);
							outwalker_glass.mission_state = MISSION_STATE_STARTED;
							outwalker_glass.current_step = 1;
							SpriteManagerAdd(SpriteDiary, 72, 8);
						}
					break;
					case 1u://occhiali non ancora trovati
						*n_lines = 5u;
						memcpy(d1, "I'VE LOST MY GLASSES", 22);
						memcpy(d2, "BY THE SEE, I SAW A ", 22);
						memcpy(d3, "GIANT CRAB! HE AT   ", 22);
						memcpy(d4, "TACKED ME AND I RUN ", 22);
						memcpy(d5, "AWAY.               ", 22);
						if(get_quantity(INVITEM_ARROW_PERFO) < 5){
							*n_lines = 9u;
							memcpy(d6, "TAKE THESE PEARCING", 22);
							memcpy(d7, "ARROWS: GUESS YOU  ", 22);
							memcpy(d8, "GONNA NEED EM TO   ", 22);
							memcpy(d9, "FIGHT THAT BEAST.  ", 22);
							if(outwalker_glass.mission_state < MISSION_STATE_STARTED){
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
						if(outwalker_glass.mission_state < MISSION_STATE_REWARDED){
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
			memcpy(d0, "SIMON:              ", 22);
			memcpy(d1, "YES, THIS I THE WAY ", 22);
			memcpy(d2, "TO THE MOUNTAINS.   ", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "YOU STILL DON'T KNOW", 22);
			memcpy(d5, "WHO IS OUR CHIEF... ", 22);
			memcpy(d6, "SO I CAN'T TRUST YOU", 22);
			memcpy(d7, EMPTY_STRING_21, 22);
			memcpy(d8, "YOU SHALL NOT PASS! ", 22);
		break;
		case OUTWALKER_GUARD_NOGLASS:
			*n_lines = 12u;
			memcpy(d0, "SIMON:              ", 22);
			memcpy(d1, "YES, THIS IS THE WAY", 22);
			memcpy(d2, "TO THE MOUNTAINS.   ", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "NOW YOU KNOW WHO IS ", 22);
			memcpy(d5, "OUR CHIEF... GOOD!  ", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "I WILL TRUST YOU AS ", 22);
			memcpy(d8, "SOON AS YOU HELP    ", 22);
			memcpy(d9, "ONE OF US. JACK MAY ", 22);
			memcpy(d10, "NEED YOU.          ", 22);
			memcpy(d11, EMPTY_STRING_21, 22);
			memcpy(d12, "TALK TO JACK.      ", 22);
			outwalker_glass.mission_state = MISSION_STATE_ENABLED;
			SpriteManagerAdd(SpriteDiary, 72, 8);
		break;
		case OUTWALKER_GUARD_NOSMITH:
			*n_lines = 8u;
			memcpy(d0, "SIMON:              \0", 22);
			memcpy(d1, "YES, THIS I THE WAY \0", 22);
			memcpy(d2, "TO THE MOUNTAINS.   \0", 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, "WE NEED SOME WEAPONS", 22);
			memcpy(d5, "AND THE SMITH CAN   ", 22);
			memcpy(d6, "FORGE 'EM FOR US.   ", 22);
			memcpy(d7, "ASK HIM AND THEN I  ", 22);
			memcpy(d8, "WILL LET YOU PASS.  ", 22);
			outwalker_smith.mission_state = MISSION_STATE_ENABLED;
			SpriteManagerAdd(SpriteDiary, 72, 8);
		break;
		case OUTWALKER_GUARD_OK:
				*n_lines = 10u;
				memcpy(d0, "SIMON:              ", 22);
				memcpy(d1, "YES  THIS I THE WAY ", 22);
				memcpy(d2, "TO THE MOUNTAINS.   ", 22);
				memcpy(d3, EMPTY_STRING_21, 22);
				memcpy(d4, "THANK YOU: THESE WE ", 22);
				memcpy(d5, "APONS MEAN A LOT FOR", 22);
				memcpy(d6, "US!                 ", 22);
				memcpy(d7, EMPTY_STRING_21, 22);
				memcpy(d8, "NOW WE TRUST YOU.   ", 22);
				memcpy(d9, "YOU CAN PASS NOW.   ", 22);
				memcpy(d10, "GOOD LUCK!          ", 22);
		break;
		case OUTWALKER_CHIEF_FOUND:
			*n_lines = 17u;
			memcpy(d0, "PAUL:               \0", 22);
			memcpy(d1, "WOW! I WASN'T EXPECT", 22);
			memcpy(d2, "ING TO SEE YOU HERE.", 22);
			memcpy(d3, "OH WELL, NOW YOU    ", 22);
			memcpy(d4, "KNOW...             ", 22);
			memcpy(d5, "IT'S ME, LEGO, I AM \0", 22);
			memcpy(d6, "THE CHIEF OF THE OUT\0", 22);
			memcpy(d7, "WALKERS!!           \0", 22);
			memcpy(d8, EMPTY_STRING_21, 22);
			memcpy(d9, "I LIKE TO KEEP IT A \0", 22);
			memcpy(d10, "SECRET, SO VILLAGERS", 22);
			memcpy(d11, "ACT NORMALLY WHEN I \0", 22);
			memcpy(d12, "AM AROUND.          \0", 22);
			memcpy(d13, EMPTY_STRING_21, 22);
			memcpy(d14, "PLEASE DON'T TELL   ", 22);
			memcpy(d15, "ANYONE. HERE: TAKE  ", 22);
			memcpy(d16, "100 COINS FOR YOUR  ", 22);
			memcpy(d17, "SILENCE.            ", 22);
			if(outwalker_chief.mission_state < MISSION_STATE_REWARDED){
				struct ItemSpawned money_data={.itemtype = INVITEM_MONEY, .quantity = 100, .equippable = 1u};
				pickup(&money_data);
				outwalker_chief.current_step = 3;
				outwalker_info_step = 3;
				SpriteManagerAdd(SpriteDiary, 72, 8);
				outwalker_chief.mission_state = MISSION_STATE_REWARDED;
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
			*n_lines = 17u;
			memcpy(d0, "BIG BOSS CRAB:       ", 22);
			memcpy(d1, "*CLACK! CLACK!*      ", 22);
			memcpy(d2, "WITH THESE GLASSES I ", 22);
			memcpy(d3, "SEE WAY MORE BETTER. ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);

			memcpy(d5, "DESSA: THOSE ARE NOT ", 22);
			memcpy(d6, "YOURS. GIVE THEM BACK", 22);
			memcpy(d7, "NOW!                 ", 22);
			memcpy(d8, EMPTY_STRING_21, 22);

			memcpy(d9, "WHAT? I'VE FOUND EM!", 22);
			memcpy(d10, "THEY'RE MINE NOW!   ", 22);
			memcpy(d11, "AREN'T THEM?...     ", 22);
			memcpy(d12, EMPTY_STRING_21, 22);
			
			memcpy(d13, "DESSA: THOSE ARE NOT ", 22);
			memcpy(d14, "YOURS.               ", 22);
			memcpy(d15, EMPTY_STRING_21, 22);

			memcpy(d16, "'CLACK! CLACK!'     ", 22);
			memcpy(d17, "WE'LL SEE...        ", 22);
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
			struct ItemSpawned herb_data={.itemtype = INVITEM_HERB, .quantity = 1, .equippable = 0u};
			pickup(&herb_data);
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
			SpriteManagerAdd(SpriteDiary, 72, 8);
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
			SpriteManagerAdd(SpriteDiary, 72, 8);
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
			SpriteManagerAdd(SpriteDiary, 72, 8);
			find_antidote.mission_state = MISSION_STATE_ACCOMPLISHED;
            play_music_missionaccomplished();
			find_antidote.phase = 5;
			visit_blackie.mission_state = MISSION_STATE_STARTED;
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
            fix_bridge.mission_state = MISSION_STATE_REWARDED;
			SpriteManagerAdd(SpriteDiary, 72, 8);
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
		default:
			GetLocalizedDialog3_EN(n_lines);
		break;
    }
}