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
extern struct MISSION visit_blackie;
extern struct MISSION hungry_people;
extern struct MISSION golden_armor;
extern struct MISSION fix_bridge;
extern struct MISSION mr_smee;
extern struct MISSION broken_ship;
extern struct MISSION pirate_strike;
extern struct MISSION captain;

extern void pickup(struct ItemSpawned* pickedup_data) BANKED;
extern INT16 change_quantity(INVITEMTYPE itemtype, INT8 l) BANKED;
extern UINT8 get_quantity(INVITEMTYPE itemtype) BANKED;
extern void play_music_missionaccomplished() BANKED;


void GetLocalizedDialog3_EN(UINT8* n_lines) BANKED{
    switch(whostalking){		
		
		case POLICE_0_GET_PASS:
			*n_lines = 15u;
			memcpy(d0, "GUARD:              ", 22);
			memcpy(d1, "SALUTE HEALER,      ", 22);
			memcpy(d2, "WE KNOW TO THE WEST ", 22);
			memcpy(d3, "THERE ARE SOME      ", 22);
			memcpy(d4, "PEOPLE LIVING ON    ", 22);
			memcpy(d5, "THEIR OWN. WE WANT  ", 22);
			memcpy(d6, "TO KNOW WHO IS THEIR", 22);
			memcpy(d7, "LEADER. BECAUSE THEY", 22);
			memcpy(d8, "DON'T WANT TO TELL  ", 22);
			memcpy(d9, "US. TAKE THIS PASS, ", 22);
			memcpy(d10, "SHOW IT TO THEM AND ", 22);
			memcpy(d11, "THEY WILL LET YOU IN", 22);
			memcpy(d12, EMPTY_STRING_21, 22);
			memcpy(d13, "PLEASE COME BACK AS ", 22);
			memcpy(d14, "SOON AS YOU FIND IT ", 22);
			memcpy(d15, "OUT. THANK YOU.     ", 22);				
			{
				if(get_quantity(INVITEM_PASS) < 1){
					struct ItemSpawned pass_data={.itemtype = INVITEM_PASS, .quantity = 1, .equippable = 0u};
					pickup(&pass_data);
				}
				outwalker_chief.mission_state = MISSION_STATE_ENABLED;
				outwalker_chief.current_step = 0;
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
			visit_blackie.mission_state = MISSION_STATE_REWARDED;
			SpriteManagerAdd(SpriteDiary, 72, 8);
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
			SpriteManagerAdd(SpriteDiary, 72, 8);
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
			SpriteManagerAdd(SpriteDiary, 72, 8);
			hungry_people.mission_state = MISSION_STATE_ACCOMPLISHED;
            play_music_missionaccomplished();
		break;
		case ITEMDETAIL_LIAM_HANDWRITTEN:
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
			SpriteManagerAdd(SpriteDiary, 72, 8);
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
		case MINOTAUR_ENTRANCE:
			*n_lines = 13u;
			memcpy(d0, "DESSA:              ", 22);
			memcpy(d1, "IT'S A LOT COLDER   ", 22);
			memcpy(d2, "IN HERE, MY ARROWS  ", 22);
			memcpy(d3, "MAY FREEZE...       ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);	
			memcpy(d5, "I CAN SEE A TALL AND", 22);
			memcpy(d6, "DARK FIGURE IN THE  ", 22);
			memcpy(d7, "NEXT ROOM.IT MUST BE", 22);
			memcpy(d8, "THE MINOTAUR PETER  ", 22);
			memcpy(d9, "TALKED ME ABOUT.    ", 22);
			memcpy(d10, EMPTY_STRING_21, 22);	
			memcpy(d11, "SHOULD I FACE HIM?   ", 22);
			memcpy(d12, EMPTY_STRING_21, 22);
			memcpy(d13, " NO          YES    \0", 22);
			choice = 1u;
			choice_left = 0u;
			choice_right = 0u;
		break;
		case MINOTAUR_DEFEAT:
			*n_lines = 17u;
			memcpy(d0, EMPTY_STRING_21, 22);
			memcpy(d1, "MY BROTHER TOLD ME ", 22);
			memcpy(d2, "YOUR FAMILY WAS    ", 22);
			memcpy(d3, "STRONG....         ", 22);
			memcpy(d4, "ONLY ONE HUMAN     ", 22);
			memcpy(d5, "UNMASKED ME ONCE...", 22);
			memcpy(d6, "BUT THIS IS ANOTHER", 22);
			memcpy(d7, "STORY. YOU FOUND A ", 22);
			memcpy(d8, "NEW FRIEND IN ME.  ", 22);
			memcpy(d9, "AND NOW MY SKULL   ", 22);
			memcpy(d10, "IS YOURS. DO WHAT ", 22);
			memcpy(d11, "EVER YOU WANT.    ", 22);
			memcpy(d12, EMPTY_STRING_21, 22);
			memcpy(d13, "DESSA: YOUR SKULL ", 22);
			memcpy(d14, "IS MADE OF SILVER ", 22);
			memcpy(d15, "AND GOLD: I WILL  ", 22);
			memcpy(d16, "USE IT TO FORGE A ", 22);
			memcpy(d17, "GOLDEN ARMOR.     ", 22);
		break;
		case HARBOR_TOOSOON:
			*n_lines = 4u;
			memcpy(d0, "THE HARBOR:        ", 22);
			memcpy(d1, "IT IS TOO SOON FOR ", 22);
			memcpy(d2, "ME TO LEAVE. HERE  ", 22);
			memcpy(d3, "THERE IS STILL A   ", 22);
			memcpy(d4, "LOT TO DO!         ", 22);
		break;
		case PIRATE_SPUGNA_0:
			*n_lines = 19u;
			memcpy(d0, "MR SMEE:            ", 22);
			memcpy(d1, "HELLO DESSA! WE HAVE", 22);
			memcpy(d2, "HEARD YOU NEED US TO", 22);
			memcpy(d3, "CROSS THE SEA. WELL ", 22);
			memcpy(d4, "WE CAN DO THAT, BUT ", 22);
			memcpy(d5, "... SOME OF US MAY  ", 22);
			memcpy(d6, "NEED YOUR HELP FOR  ", 22);
			memcpy(d7, "SOME BUSINESSES...  ", 22);
			memcpy(d8, EMPTY_STRING_21, 22);
			memcpy(d9, "THE SHIP IS BROKEN, ", 22);
			memcpy(d10, "MAURICE HAS LOST A ", 22);
			memcpy(d11, "FRIEND, WE ARE CUR ", 22);
			memcpy(d12, "RENTLY ON A STRIKE ", 22);
			memcpy(d13, "AND THE CAPTAIN IS ", 22);
			memcpy(d14, "DEPRESSED BECAUSE  ", 22);
			memcpy(d15, "HE HAS NO ONE TO   ", 22);
			memcpy(d16, "PLAY TETRA WITH.   ", 22);
			memcpy(d17, EMPTY_STRING_21, 22);
			memcpy(d18, "WELCOME TO THE     ", 22);
			memcpy(d19, "HARBOR!            ", 22);
            mr_smee.mission_state = MISSION_STATE_ENABLED;
			SpriteManagerAdd(SpriteDiary, 72, 8);
		break;
		case PIRATE_SPUGNA_1:
			*n_lines = 8u;
			memcpy(d0, "MR SMEE:            ", 22);
			memcpy(d1, "ABOUT ME... MY FRIE ", 22);
			memcpy(d2, "ND RICK WAS HUNTED  ", 22);
			memcpy(d3, "SOME BATS IN A CAVE.", 22);
			memcpy(d4, "FACT IS: IT'S BEEN  ", 22);
			memcpy(d5, "THREE DAYS SINCE WE ", 22);
			memcpy(d6, "'VE SEEN HIM. CAN   ", 22);
			memcpy(d7, "YOU KINDLY HAVE A   ", 22);
			memcpy(d8, "CHECK ON THAT CAVE? ", 22);
            if(mr_smee.mission_state < MISSION_STATE_STARTED){
				mr_smee.mission_state = MISSION_STATE_STARTED;
				SpriteManagerAdd(SpriteDiary, 72, 8);
			}
		break;
		case PIRATE_SPUGNA_2:
			*n_lines = 4u;
			memcpy(d0, "MR SMEE:            ", 22);
			memcpy(d1, "OH, I SEE... I AM   ", 22);
			memcpy(d2, "SORRY FOR HIM, BUT  ", 22);
			memcpy(d3, "THIS IS HOW THE WOR ", 22);
			memcpy(d4, "LD WORKS NOW.       ", 22);
            mr_smee.mission_state = MISSION_STATE_REWARDED;
			broken_ship.mission_state = MISSION_STATE_ENABLED;
			SpriteManagerAdd(SpriteDiary, 72, 8);
		break;
		case PIRATE_SPUGNA_3:
			*n_lines = 4u;
			memcpy(d0, "MR SMEE:            ", 22);
			memcpy(d1, "YOU HAVE MY SUPPORT ", 22);
			memcpy(d2, "I AM DOING THE BEST ", 22);
			memcpy(d3, "I CAN TO SET THE    ", 22);
			memcpy(d4, "SAIL!               ", 22);
		break;
		case PIRATE_SPUGNA_STRIKE:
			*n_lines = 2u;
			memcpy(d0, "MR SMEE:            ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "WE ARE ON STRIKE!   ", 22);
		break;
		case PIRATE_PANZONE_0:
			*n_lines = 5u;
			memcpy(d0, "BOB:                ", 22);
			memcpy(d1, "THE SHIP IS DAMAGED!", 22);
			memcpy(d2, "PLEASE PROVIDE ME OF", 22);
			memcpy(d3, "50 WOOD PIECES.     ", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "THANK YOU.          ", 22);
            if(broken_ship.mission_state < MISSION_STATE_STARTED){
				broken_ship.mission_state = MISSION_STATE_STARTED;
				SpriteManagerAdd(SpriteDiary, 72, 8);
			}
		break;
		case PIRATE_PANZONE_1:
			*n_lines = 4u;
			memcpy(d0, "BOB:                ", 22);
			memcpy(d1, "50 PIECES OF WOOD!  ", 22);
			memcpy(d2, "THANK YOU HEALER! WE", 22);
			memcpy(d3, "ARE NOW ABLE TO FIX ", 22);
			memcpy(d4, "THAT SHIP.          ", 22);
            if(broken_ship.mission_state < MISSION_STATE_REWARDED){
				broken_ship.mission_state = MISSION_STATE_REWARDED;
				SpriteManagerAdd(SpriteDiary, 72, 8);
				pirate_strike.mission_state = MISSION_STATE_ENABLED;
                change_quantity(INVITEM_WOOD, -50);
			}
		break;
		case PIRATE_PANZONE_2:
			*n_lines = 4u;
			memcpy(d0, "BOB:                ", 22);
			memcpy(d1, "AS SOON AS THE SHIP ", 22);
			memcpy(d2, "IS FIXED I THINK WE ", 22);
			memcpy(d3, "WILL BE READY TO    ", 22);
			memcpy(d4, "SAIL.               ", 22);
		break;
		case PIRATE_PANZONE_STRIKE:
			*n_lines = 2u;
			memcpy(d0, "BOB:                ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "WE ARE ON STRIKE!   ", 22);
		break;
		case PIRATE_MARTIN_0:
			*n_lines = 7u;
			memcpy(d0, "MARTIN:             ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "BEING HIGH HELPS A  ", 22);
			memcpy(d3, "LOT! LIVING THIS    ", 22);
			memcpy(d4, "WORLD IS TOUGH...   ", 22);
			memcpy(d5, "SOME OF US NEED AN  ", 22);
			memcpy(d6, "ESCAPE... SOMETIME  ", 22);
			memcpy(d7, "... YOU KNOW...     ", 22);
		break;
		case PIRATE_MARTIN_1:
			*n_lines = 15u;
			memcpy(d0, "MARTIN:             ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "WE ARE ON STRIKE!   ", 22);
			memcpy(d3, "WE WANNA GET HIGH!  ", 22);
			memcpy(d4, "THE DOPE IS FINISHED", 22);
			memcpy(d5, "AND WE WANT MORE!   ", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "OF COURSE CAPTAIN IS", 22);
			memcpy(d8, "AGAINST IT, BUT WE  ", 22);
			memcpy(d9, "WILL NOT SAIL UNTIL ", 22);
			memcpy(d10, "WE HAVE WHAT WE WANT", 22);
			memcpy(d11, EMPTY_STRING_21, 22);
			memcpy(d12, "WE NEED 6 MUSHROOMS ", 22);
			memcpy(d13, "THEY SHOULD BE IN   ", 22);
			memcpy(d14, "DARK PLACES... MAYBE", 22);
			memcpy(d15, "ON THE UNDERGROUND? ", 22);
			SpriteManagerAdd(SpriteDiary, 72, 8);
			pirate_strike.mission_state = MISSION_STATE_STARTED;
		break;
		case PIRATE_MARTIN_2:
			*n_lines = 7u;
			memcpy(d0, "MARTIN:             ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "THANKS MADAME. WE   ", 22);
			memcpy(d3, "WILL NOT TALK, BE   ", 22);
			memcpy(d4, "SURE OF THAT.       ", 22);
			change_quantity(INVITEM_MUSHROOM, -6);
			SpriteManagerAdd(SpriteDiary, 72, 8);
			pirate_strike.mission_state = MISSION_STATE_REWARDED;
			captain.mission_state = MISSION_STATE_ENABLED;
		break;
		case PIRATE_MARTIN_3:
			*n_lines = 3u;
			memcpy(d0, "MARTIN:             ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "IF CAPTAIN ORDERS US", 22);
			memcpy(d3, "TO SAIL, WE SAIL.   ", 22);
		break;
		break;
		case PIRATE_MARTIN_STRIKE:
			*n_lines = 2u;
			memcpy(d0, "MARTIN:             ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "WE ARE ON STRIKE!   ", 22);
		break;
		case PIRATE_MAURICE_0:
			*n_lines = 11u;
			memcpy(d0, "MAURICE:            ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "CAPTAIN IS JUST     ", 22);
			memcpy(d3, "UNBEATABLE! HE IS A ", 22);
			memcpy(d4, "REAL CHAMP ON TETRA.", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "I HAVE NEVER MET A  ", 22);
			memcpy(d7, "SINGLE PERSON BEING ", 22);
			memcpy(d8, "VICTORIOUS ON HIM.  ", 22);
			memcpy(d9, EMPTY_STRING_21, 22);
			memcpy(d10, "I THINK I JUST GIVE ", 22);
			memcpy(d11, "UP!                 ", 22);
		break;
		case PIRATE_MAURICE_STRIKE:
			*n_lines = 2u;
			memcpy(d0, "MAURICE:            ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "WE ARE ON STRIKE!   ", 22);
		break;
		case PIRATE_CAPTAIN_0:
			*n_lines = 11u;
			memcpy(d0, "ONE EYED JACK:      ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "ARRRRRR! NAME IS    ", 22);
			memcpy(d3, "JACK! I AM THE CAP  ", 22);
			memcpy(d4, "TAIN OF SIREN, MY   ", 22);
			memcpy(d5, "GALLEON!            ", 22);
			memcpy(d6, EMPTY_STRING_21, 22);
			memcpy(d7, "THINK MY MEN HAVE   ", 22);
			memcpy(d8, "SOME BUSINESS FOR   ", 22);
			memcpy(d9, "YA. FIX THEM, THEN  ", 22);
			memcpy(d10, "WE CAN TALK ABOUT  ", 22);
			memcpy(d11, "SAILING.           ", 22);
		break;
		case PIRATE_CAPTAIN_1:
			*n_lines = 14u;
			memcpy(d0, "ONE EYED JACK:      ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "LOOKS LIKE YOU ARRR ", 22);
			memcpy(d3, "READY FOR AN ADVENTU", 22);
			memcpy(d4, "RE. NEED HELP UH?   ", 22);
			memcpy(d4, "THE ISLAND IS NOT   ", 22);
			memcpy(d4, "TOO FARRR FROM HERE.", 22);
			memcpy(d4, "BUT THE TEMPEST...  ", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "WE SHOULD WAIT FOR  ", 22);
			memcpy(d7, "IT TO CALM DOWN...  ", 22);
			memcpy(d8, "MEANWHILE, I DARE   ", 22);
			memcpy(d9, "YOU BEAT ME ON TETRA", 22);
			memcpy(d10, ", MY FAV BOARDGAME. ", 22);
			memcpy(d11, EMPTY_STRING_21, 22);
			memcpy(d12, "ARE YOU READY?      ", 22);
			SpriteManagerAdd(SpriteDiary, 72, 8);
			memcpy(d13, EMPTY_STRING_21, 22);
			memcpy(d14, " NO          YES    \0", 22);
			choice = 1u;
			choice_left = 0u;
			choice_right = 0u;
		break;
		case PIRATE_CAPTAIN_2:
			*n_lines = 4u;
			memcpy(d0, "ONE EYED JACK:      ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "COME BACK WHEN YOU  ", 22);
			memcpy(d3, "FEEL READY TO TRY   ", 22);
			memcpy(d4, "AGAIN!              ", 22);
		break;
		case PIRATE_CAPTAIN_3:
			*n_lines = 10u;
			memcpy(d0, "ONE EYED JACK:      ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, "AH! YOU FINALLY BEAT", 22);
			memcpy(d3, "ME! ALRIGHT ALRIGHT.", 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "DEFEATED BY A WOMAN!", 22);
			memcpy(d6, "WHAT I'VE BECOME?!  ", 22);
			memcpy(d7, EMPTY_STRING_21, 22);
			memcpy(d8, "THE STORM HAS GONE  ", 22);
			memcpy(d9, "NOW, WE CAN SAIL.   ", 22);
			memcpy(d10, "MEN!!!! LET'S GO!   ", 22);
            captain.mission_state = MISSION_STATE_REWARDED;
		break;
		case CADAVER://se devo ancora fare bossfight oppure no
			if(mr_smee.mission_state == MISSION_STATE_STARTED){
				*n_lines = 12u;
				memcpy(d0, "DESSA  :            ", 22);
				memcpy(d1, "IT'S A HEADLESS     ", 22);
				memcpy(d2, "BODY! WHAT AN END   ", 22);
				memcpy(d3, "FOR A LIFE...       ", 22);
				memcpy(d4, EMPTY_STRING_21, 22);
				memcpy(d5, "WHAT COULD HAVE     ", 22);
				memcpy(d6, "POSSIBLY CAUSED...  ", 22);
				memcpy(d7, EMPTY_STRING_21, 22);
				memcpy(d8, EMPTY_STRING_21, 22);
				memcpy(d9, EMPTY_STRING_21, 22);
				memcpy(d10, "*FLAP! FLAP! FLAP!* ", 22);
				memcpy(d11, EMPTY_STRING_21, 22);
				memcpy(d12, "YYAAAAAAAAAAAAAAAH! ", 22);
				mr_smee.current_step = 1;//means start boss fight!
			}else{
				*n_lines = 5u;
				memcpy(d0, "DESSA  :            ", 22);
				memcpy(d1, "HERE LIES RICK,     ", 22);
				memcpy(d2, "HEADLESS RICK. NOW  ", 22);
				memcpy(d3, "HE'S FOOD FOR BEASTS", 22);
				memcpy(d4, EMPTY_STRING_21, 22);
				memcpy(d5, "POOR RICK!          ", 22);
			}
		break;
		case MEANWHILE:
			*n_lines = 5u;
			memcpy(d0, "MEANWHILE           ", 22);
			memcpy(d1, EMPTY_STRING_21, 22);
			memcpy(d2, EMPTY_STRING_21, 22);
			memcpy(d3, EMPTY_STRING_21, 22);
			memcpy(d4, EMPTY_STRING_21, 22);
			memcpy(d5, "IN THE NORTHERN ISLE", 22);
		break;
		case FINAL:
			*n_lines = 19u;
			memcpy(d0, "DEAR SON,           ", 22);
			memcpy(d1, "THAT MAN IS NOT TO  ", 22);
			memcpy(d2, "BE TRUSTED! WATCH   ", 22);
			memcpy(d3, "FOR YOURSELF, I AM  ", 22);
			memcpy(d4, "COMING.             ", 22);
			memcpy(d5, EMPTY_STRING_21, 22);
			memcpy(d6, "AT DAWN ON THE      ", 22);
			memcpy(d7, "SECOND DAY FROM THIS", 22);
			memcpy(d8, "MOMENT, LOOK SOUTH  ", 22);
			memcpy(d9, "FOR A PIRATE SHIP.  ", 22);
			memcpy(d10, EMPTY_STRING_21, 22);
			memcpy(d11, "TOGETHER WE WILL WIN", 22);
			memcpy(d12, "AGAIN: NO MASTER, NO", 22);
			memcpy(d13, "DRAGONS WILL BEAT US", 22);
			memcpy(d14, EMPTY_STRING_21, 22);
			memcpy(d15, EMPTY_STRING_21, 22);
			memcpy(d16, "LOVE,               ", 22);
			memcpy(d17, "XOXO                ", 22);
			memcpy(d18, "               MOM  ", 22);
			memcpy(d19, EMPTY_STRING_21, 22);
		break;
    }
}