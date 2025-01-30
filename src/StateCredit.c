#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Music.h"

#include "TAnim0.h"
#include "custom_datas.h"

IMPORT_TILES(font);
IMPORT_TILES(tilescredit);
IMPORT_MAP(mapcredit0);
IMPORT_MAP(mapcredit2);
IMPORT_MAP(mapcredit3);
IMPORT_MAP(mapcredit4);
DECLARE_MUSIC(titlescreen);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern struct EtoReload e_to_reload[3];
extern WHOSTALKING whostalking;
extern UINT8 stop_music_on_new_state;
extern INT8 current_map;//0=south-west, 1=north-east, 2=maze, 3=north-east
extern UINT8 generic_counter;
extern UINT16 motherow_pos_x;
extern UINT16 motherow_pos_y;
extern UINT16 motherpl_pos_x;
extern UINT16 motherpl_pos_y;
extern UINT8 itemspawned_powder_counter;
extern INT8 motherpl_hp;

const UINT8 collision_tiles_credits[] = {1,0};
INT8 credit_step = -1;
UINT16 credit_wait_time;
CHAPTERS chapter = CHAPTER_0_BLACKIE;
CURRENT_BORDER current_border = BORDER_NONE;
uint8_t sgb_running = 0;

struct MISSION find_blackie = {.mission_title = FIND_BLACKIE, 
.mission_state = MISSION_STATE_ENABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION enable_hospital = {.mission_title = ENABLE_HOSPITAL, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION engage_smith = {.mission_title = ENGAGE_SMITH, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION help_cemetery_woman = {.mission_title = HELP_CEMATERY_WOMAN, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION outwalker_chief = {.mission_title = OUTWALKER_CHIEF, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION outwalker_glass = {.mission_title = OUTWALKER_GLASSES, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION outwalker_smith = {.mission_title = OUTWALKER_SMITH, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION get_to_the_mountain = {.mission_title = GET_TO_THE_MOUNTAIN, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION defeat_scorpions = {.mission_title = DEFEAT_SCORPIONS, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION find_antidote = {.mission_title = FIND_ANTIDOTE, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION visit_blackie = {.mission_title = VISIT_BLACKIE, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION hungry_people = {.mission_title = HUNGRY_PROPLE, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION fix_bridge = {.mission_title = FIX_BRIDGE, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION golden_armor = {.mission_title = GOLDEN_ARMOR, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION mr_smee = {.mission_title = MR_SMEE, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION broken_ship = {.mission_title = BROKEN_SHIP, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION pirate_strike = {.mission_title = PIRATE_STRIKE, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};
struct MISSION captain = {.mission_title = CAPTAIN, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u, .phase = 0u};

struct MISSION* missions[21];
/* 
{ engage_smith, help_cemetery_woman, find_blackie, outwalker_chief,
outwalker_glass, outwalker_smith, get_to_the_mountain, 0};
*/
struct InvItem itemMoney = {.itemtype = INVITEM_MONEY, .quantity = 0, .equippable = 1u};
struct InvItem item00 = {.itemtype = INVITEM_ARROW_NORMAL, .quantity = 100, .equippable = 1u};
struct InvItem item01 = {.itemtype = INVITEM_ARROW_PERFO, .quantity = 0, .equippable = 1u};
struct InvItem item02 = {.itemtype = INVITEM_ARROW_BASTARD, .quantity = 0, .equippable = 1u};
struct InvItem item03 = {.itemtype = INVITEM_BOMB, .quantity = 0, .equippable = 1u};
struct InvItem item04 = {.itemtype = INVITEM_UNASSIGNED, .quantity = 0, .equippable = 1u};
struct InvItem unequip00 = {.itemtype = INVITEM_WOOD, .quantity = 14, .equippable = 0u};//2
struct InvItem unequip01 = {.itemtype = INVITEM_METAL, .quantity = 15, .equippable = 0u};//3
struct InvItem unequip02 = {.itemtype = INVITEM_LETTER, .quantity = 0, .equippable = 0u};
struct InvItem unequip03 = {.itemtype = INVITEM_UNASSIGNED, .quantity = 0, .equippable = 0u};
struct InvItem unequip04 = {.itemtype = INVITEM_UNASSIGNED, .quantity = 0, .equippable = 0u};
struct InvItem unequip05 = {.itemtype = INVITEM_UNASSIGNED, .quantity = 0, .equippable = 0u};
	
struct InvItem inventory[12];
struct InvItem itemEquipped = {.itemtype = INVITEM_MONEY, .quantity = 10, .equippable = 1u};

UINT16 hidden_items_flags = 0;

void missions_init() BANKED;
void inventory_init() BANKED;
void position_init() BANKED;

extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void manage_border(UINT8 my_next_state) BANKED;

extern Sprite* s_motherpl;

void missions_init() BANKED{
	engage_smith.mission_state = MISSION_STATE_DISABLED;
	outwalker_chief.mission_state = MISSION_STATE_DISABLED;
	enable_hospital.mission_state = MISSION_STATE_DISABLED;
	find_blackie.mission_state = MISSION_STATE_DISABLED;
	help_cemetery_woman.mission_state = MISSION_STATE_DISABLED;
	outwalker_chief.mission_state = MISSION_STATE_DISABLED;
	get_to_the_mountain.mission_state = MISSION_STATE_DISABLED;
	defeat_scorpions.mission_state = MISSION_STATE_DISABLED;
	find_antidote.mission_state = MISSION_STATE_DISABLED;
	visit_blackie.mission_state = MISSION_STATE_DISABLED;
	fix_bridge.mission_state = MISSION_STATE_DISABLED;
	golden_armor.mission_state = MISSION_STATE_DISABLED;
	mr_smee.mission_state = MISSION_STATE_DISABLED;
	broken_ship.mission_state = MISSION_STATE_DISABLED;
	pirate_strike.mission_state = MISSION_STATE_DISABLED;
	captain.mission_state = MISSION_STATE_DISABLED;
	switch(chapter){
		case CHAPTER_0_BLACKIE:
			motherpl_hp = 5;
			engage_smith.mission_state = MISSION_STATE_ENABLED;
			engage_smith.current_step = 0u;
			enable_hospital.mission_state = MISSION_STATE_ENABLED;
			enable_hospital.current_step = 0u;
			/*START TEST*/
			/* engage_smith.mission_state = MISSION_STATE_REWARDED;
			engage_smith.current_step = 6u;
			enable_hospital.mission_state = MISSION_STATE_REWARDED;
			enable_hospital.current_step = 6u;
			find_blackie.mission_state = MISSION_STATE_REWARDED;
			find_blackie.current_step = 6u;
			help_cemetery_woman.mission_state = MISSION_STATE_STARTED;
			help_cemetery_woman.current_step = 1u; */
			/*END TEST*/
		break;
		case CHAPTER_1_BANDITS:
			engage_smith.mission_state = MISSION_STATE_REWARDED;
			engage_smith.current_step = 6u;
			enable_hospital.mission_state = MISSION_STATE_REWARDED;
			enable_hospital.current_step = 6u;
			find_blackie.mission_state = MISSION_STATE_REWARDED;
			find_blackie.current_step = 6u;
			help_cemetery_woman.mission_state = MISSION_STATE_REWARDED;
			help_cemetery_woman.current_step = 5u;
		break;
		case CHAPTER_2_PLAGUE:
			engage_smith.mission_state = MISSION_STATE_REWARDED;
			engage_smith.current_step = 6u;
			enable_hospital.mission_state = MISSION_STATE_REWARDED;
			enable_hospital.current_step = 6u;
			find_blackie.mission_state = MISSION_STATE_REWARDED;
			find_blackie.current_step = 6u;
			help_cemetery_woman.mission_state = MISSION_STATE_REWARDED;
			help_cemetery_woman.current_step = 5u;//0u
			outwalker_chief.mission_state = MISSION_STATE_REWARDED;
			outwalker_chief.current_step = 5u;//0u
			outwalker_glass.mission_state = MISSION_STATE_REWARDED;
			outwalker_glass.current_step = 4u;//0u
			outwalker_smith.mission_state = MISSION_STATE_REWARDED;
			outwalker_smith.current_step = 0b00001111;
			get_to_the_mountain.mission_state = MISSION_STATE_REWARDED;
			get_to_the_mountain.current_step = 0u;
			defeat_scorpions.mission_state = MISSION_STATE_STARTED;
			defeat_scorpions.current_step = 0;
			defeat_scorpions.phase = 0;
		break;
		case CHAPTER_3_ARMOUR:
			engage_smith.mission_state = MISSION_STATE_REWARDED;
			engage_smith.current_step = 6u;
			enable_hospital.mission_state = MISSION_STATE_REWARDED;
			enable_hospital.current_step = 6u;
			find_blackie.mission_state = MISSION_STATE_REWARDED;
			find_blackie.current_step = 6u;
			help_cemetery_woman.mission_state = MISSION_STATE_REWARDED;
			help_cemetery_woman.current_step = 5u;//0u
			outwalker_chief.mission_state = MISSION_STATE_REWARDED;
			outwalker_chief.current_step = 5u;//0u
			outwalker_glass.mission_state = MISSION_STATE_REWARDED;
			outwalker_glass.current_step = 4u;//0u
			outwalker_smith.mission_state = MISSION_STATE_REWARDED;
			outwalker_smith.current_step = 0b00001111;
			get_to_the_mountain.mission_state = MISSION_STATE_REWARDED;
			get_to_the_mountain.current_step = 0u;
			defeat_scorpions.mission_state = MISSION_STATE_REWARDED;
			defeat_scorpions.current_step = 0b11111111;
			defeat_scorpions.phase = 5;
			find_antidote.mission_state = MISSION_STATE_REWARDED;
			find_antidote.phase = 5;//0
			find_antidote.current_step = 1u;//0
			visit_blackie.mission_state = MISSION_STATE_REWARDED;
			hungry_people.mission_state = MISSION_STATE_ENABLED;
			hungry_people.phase = 0;
			hungry_people.current_step = 0;
		break;
		case CHAPTER_4_SHIP:
			engage_smith.mission_state = MISSION_STATE_REWARDED;
			engage_smith.current_step = 6u;
			enable_hospital.mission_state = MISSION_STATE_REWARDED;
			enable_hospital.current_step = 6u;
			find_blackie.mission_state = MISSION_STATE_REWARDED;
			find_blackie.current_step = 6u;
			help_cemetery_woman.mission_state = MISSION_STATE_REWARDED;
			help_cemetery_woman.current_step = 5u;//0u
			outwalker_chief.mission_state = MISSION_STATE_REWARDED;
			outwalker_chief.current_step = 5u;//0u
			outwalker_glass.mission_state = MISSION_STATE_REWARDED;
			outwalker_glass.current_step = 4u;//0u
			outwalker_smith.mission_state = MISSION_STATE_REWARDED;
			outwalker_smith.current_step = 0b00001111;
			get_to_the_mountain.mission_state = MISSION_STATE_REWARDED;
			get_to_the_mountain.current_step = 0u;
			defeat_scorpions.mission_state = MISSION_STATE_REWARDED;
			defeat_scorpions.current_step = 0b11111111;
			defeat_scorpions.phase = 5;
			find_antidote.mission_state = MISSION_STATE_REWARDED;
			find_antidote.phase = 5;//0
			find_antidote.current_step = 1u;//0
			visit_blackie.mission_state = MISSION_STATE_REWARDED;
			hungry_people.mission_state = MISSION_STATE_REWARDED;
			hungry_people.phase = 0;
			hungry_people.current_step = 0;
			fix_bridge.mission_state = MISSION_STATE_REWARDED;
			fix_bridge.phase = 0;
			fix_bridge.current_step = 0;
			golden_armor.mission_state = MISSION_STATE_REWARDED;
			golden_armor.phase = 0;
			/* START TEST */
			//mr_smee.mission_state = MISSION_STATE_REWARDED;
			//broken_ship.mission_state = MISSION_STATE_REWARDED;
			//pirate_strike.mission_state = MISSION_STATE_REWARDED;
			/* END TEST */
		break;
	}
	missions[0] = &engage_smith;
	missions[1] = &enable_hospital;
	missions[2] = &find_blackie;
	missions[3] = &help_cemetery_woman;
	missions[4] = &outwalker_chief;
	missions[5] = &outwalker_glass;
	missions[6] = &outwalker_smith;
	missions[7] = &get_to_the_mountain;
	missions[8] = &defeat_scorpions;
	missions[9] = &find_antidote;
	missions[10] = &visit_blackie;
	missions[11] = 0;
	missions[12] = &hungry_people;
	missions[13] = &fix_bridge;
	missions[14] = &golden_armor;
	missions[15] = 0;
	missions[16] = &mr_smee;
	missions[17] = &broken_ship;
	missions[18] = &pirate_strike;
	missions[19] = &captain;
	missions[20] = 0;
}

void inventory_init() BANKED{
	switch(chapter){
		case CHAPTER_0_BLACKIE:
			itemMoney.itemtype = INVITEM_MONEY;
			itemMoney.quantity = 30;
			item00.itemtype = INVITEM_ARROW_NORMAL; item00.quantity = 30; item00.equippable = 1u;
			item01.itemtype = INVITEM_ARROW_PERFO; item01.quantity = 0; item01.equippable = 1u;
			item02.itemtype = INVITEM_ARROW_BASTARD; item02.quantity = 0; item02.equippable = 1u;
			item03.itemtype = INVITEM_BOMB; item03.quantity = 0; item03.equippable = 1u;
			item04.itemtype = INVITEM_UNASSIGNED; item04.quantity = 0; item04.equippable = 1u;
			unequip00.itemtype = INVITEM_WOOD; unequip00.quantity = 20; unequip00.equippable = 0u;//2
			unequip01.itemtype = INVITEM_METAL; unequip01.quantity = 20; unequip01.equippable = 0u;//3
			unequip02.itemtype = INVITEM_LETTER; unequip02.quantity = 1; unequip02.equippable = 0u;
			unequip03.itemtype = INVITEM_UNASSIGNED; unequip03.quantity = 0; unequip03.equippable = 0u;
			unequip04.itemtype = INVITEM_UNASSIGNED; unequip04.quantity = 0; unequip04.equippable = 0u;
			unequip05.itemtype = INVITEM_UNASSIGNED; unequip05.quantity = 0; unequip05.equippable = 0u;
		break;
		case CHAPTER_1_BANDITS:
			itemMoney.itemtype = INVITEM_MONEY;
			itemMoney.quantity = 20;
			item00.itemtype = INVITEM_ARROW_NORMAL; item00.quantity = 30; item00.equippable = 1u;
			item01.itemtype = INVITEM_ARROW_PERFO; item01.quantity = 10; item01.equippable = 1u;
			item02.itemtype = INVITEM_ARROW_BASTARD; item02.quantity = 10; item02.equippable = 1u;
			item03.itemtype = INVITEM_BOMB; item03.quantity = 0; item03.equippable = 1u;
			item04.itemtype = INVITEM_UNASSIGNED; item04.quantity = 0; item04.equippable = 1u;
			unequip00.itemtype = INVITEM_WOOD; unequip00.quantity = 17; unequip00.equippable = 0u;//2
			unequip01.itemtype = INVITEM_METAL; unequip01.quantity = 15; unequip01.equippable = 0u;//3
			unequip02.itemtype = INVITEM_POWDER; unequip02.quantity = 1; unequip02.equippable = 0u;
			unequip03.itemtype = INVITEM_UNASSIGNED; unequip03.quantity = 0; unequip03.equippable = 0u;//UNASSIGNED
			unequip04.itemtype = INVITEM_UNASSIGNED; unequip04.quantity = 0; unequip04.equippable = 0u;
			unequip05.itemtype = INVITEM_UNASSIGNED; unequip05.quantity = 0; unequip05.equippable = 0u;
		break;
		case CHAPTER_2_PLAGUE:
			itemMoney.itemtype = INVITEM_MONEY;
			itemMoney.quantity = 50;
			item00.itemtype = INVITEM_ARROW_NORMAL; item00.quantity = 100; item00.equippable = 1u;
			item01.itemtype = INVITEM_ARROW_PERFO; item01.quantity = 10; item01.equippable = 1u;
			item02.itemtype = INVITEM_ARROW_BASTARD; item02.quantity = 10; item02.equippable = 1u;
			item03.itemtype = INVITEM_BOMB; item03.quantity = 0; item03.equippable = 1u;
			item04.itemtype = INVITEM_UNASSIGNED; item04.quantity = 0; item04.equippable = 1u;
			unequip00.itemtype = INVITEM_WOOD; unequip00.quantity = 6; unequip00.equippable = 2;
			unequip01.itemtype = INVITEM_METAL; unequip01.quantity = 6; unequip01.equippable = 3;
			unequip02.itemtype = INVITEM_POWDER; unequip02.quantity = 1; unequip02.equippable = 1u;
			unequip03.itemtype = INVITEM_HERB; unequip03.quantity = 1; unequip03.equippable = 1u;
			unequip04.itemtype = INVITEM_UNASSIGNED; unequip04.quantity = 0; unequip04.equippable = 0u;
			unequip05.itemtype = INVITEM_UNASSIGNED; unequip05.quantity = 0; unequip05.equippable = 0u;
		break;
		case CHAPTER_3_ARMOUR:
			itemMoney.itemtype = INVITEM_MONEY;
			itemMoney.quantity = 60;
			item00.itemtype = INVITEM_ARROW_NORMAL; item00.quantity = 30; item00.equippable = 1u;
			item01.itemtype = INVITEM_ARROW_PERFO; item01.quantity = 10; item01.equippable = 1u;
			item02.itemtype = INVITEM_ARROW_BASTARD; item02.quantity = 10; item02.equippable = 1u;
			item03.itemtype = INVITEM_BOMB; item03.quantity = 0; item03.equippable = 1u;
			item04.itemtype = INVITEM_UNASSIGNED; item04.quantity = 0; item04.equippable = 1u;
			unequip00.itemtype = INVITEM_WOOD; unequip00.quantity = 30; unequip00.equippable = 0u;
			unequip01.itemtype = INVITEM_METAL; unequip01.quantity = 30; unequip01.equippable = 0u;
			unequip02.itemtype = INVITEM_UNASSIGNED; unequip02.quantity = 0; unequip02.equippable = 0u;
			unequip03.itemtype = INVITEM_UNASSIGNED; unequip03.quantity = 0; unequip03.equippable = 0u;
			unequip04.itemtype = INVITEM_UNASSIGNED; unequip04.quantity = 0; unequip04.equippable = 0u;
			unequip05.itemtype = INVITEM_UNASSIGNED; unequip05.quantity = 0; unequip05.equippable = 0u;
		break;
		case CHAPTER_4_SHIP:
			itemMoney.itemtype = INVITEM_MONEY;
			itemMoney.quantity = 100;
			item00.itemtype = INVITEM_ARROW_NORMAL; item00.quantity = 30; item00.equippable = 1u;
			item01.itemtype = INVITEM_ARROW_PERFO; item01.quantity = 10; item01.equippable = 1u;
			item02.itemtype = INVITEM_ARROW_BASTARD; item02.quantity = 10; item02.equippable = 1u;
			item03.itemtype = INVITEM_BOMB; item03.quantity = 0; item03.equippable = 1u;
			item04.itemtype = INVITEM_UNASSIGNED; item04.quantity = 0; item04.equippable = 1u;
			unequip00.itemtype = INVITEM_WOOD; unequip00.quantity = 30; unequip00.equippable = 0u;
			unequip01.itemtype = INVITEM_METAL; unequip01.quantity = 30; unequip01.equippable = 0u;
			unequip02.itemtype = INVITEM_UNASSIGNED; unequip02.quantity = 0; unequip02.equippable = 0u;
			unequip03.itemtype = INVITEM_UNASSIGNED; unequip03.quantity = 0; unequip03.equippable = 0u;
			unequip04.itemtype = INVITEM_UNASSIGNED; unequip04.quantity = 0; unequip04.equippable = 0u;
			unequip05.itemtype = INVITEM_UNASSIGNED; unequip05.quantity = 0; unequip05.equippable = 0u;
		break;
	}
	inventory[0] = itemMoney;
	inventory[1] = item00;
	inventory[2] = item01;
	inventory[3] = item02;
	inventory[4] = item03;
	inventory[5] = item04;
	inventory[6] = unequip00;
	inventory[7] = unequip01;
	inventory[8] = unequip02;
	inventory[9] = unequip03;
	inventory[10] = unequip04;
	inventory[11] = unequip05;
	itemEquipped = inventory[0];
	hidden_items_flags = 0;
	itemspawned_powder_counter = 0;
}

void position_init() BANKED{
	switch(chapter){
		case CHAPTER_0_BLACKIE:
			current_map = MAP_SOUTHWEST;
			motherow_pos_x = (UINT16) 14u << 3;
			motherow_pos_y = (UINT16) 26u << 3;
		break;
		case CHAPTER_1_BANDITS:
			current_map = MAP_NORTHWEST;
			motherow_pos_x = (UINT16) 14u << 3;
			motherow_pos_y = (UINT16) 42u << 3;
			/*CRAB
			motherow_pos_x = (UINT16) 14u << 3;
			motherow_pos_y = (UINT16) 11u << 3;*/
			/*MINE
			current_map = MAP_MAZE;
			motherpl_pos_x = (UINT16) 4u << 3;
			motherpl_pos_y = (UINT16) 7u << 3;
			motherow_pos_x = (UINT16) 3u << 3;
			motherow_pos_y = (UINT16) 2u << 3;*/
		break;
		case CHAPTER_2_PLAGUE:
			current_map = MAP_NORTHWEST;
			motherow_pos_x = (UINT16) 6u << 3;//22 MAP0  6 MAP1 
			motherow_pos_y = (UINT16) 28u << 3;//21 MAP0 28 MAP1
			motherpl_pos_x = (UINT16) 8u << 3;
			motherpl_pos_y = (UINT16) 8u << 3;
		break;
		case CHAPTER_3_ARMOUR:
			current_map = MAP_SOUTHEAST;
			motherow_pos_x = (UINT16) 15u << 3; 
			motherow_pos_y = (UINT16) 22u << 3;
			motherpl_pos_x = (UINT16) 8u << 3;
			motherpl_pos_y = (UINT16) 8u << 3;
			//TEST START
			/*current_map = MAP_EAST;
			motherow_pos_x = (UINT16) 38u << 3; 
			motherow_pos_y = (UINT16) 44u << 3;
			motherpl_pos_x = (UINT16) 8u << 3;
			motherpl_pos_y = (UINT16) 8u << 3;*/
			//TEST END
		break;
		case CHAPTER_4_SHIP:
			current_map = MAP_SOUTHWEST;
			motherow_pos_x = (UINT16) 24u << 3; 
			motherow_pos_y = (UINT16) 18u << 3;
			//TEST START
			/*current_map = MAP_EAST;
			motherow_pos_x = (UINT16) 46u << 3; 
			motherow_pos_y = (UINT16) 12u << 3;*/
			//TEST END
		break;
	}
}

void empty_e_to_reload() BANKED{
    e_to_reload[0].type = 0u;
    e_to_reload[0].x = 0u;
    e_to_reload[0].y = 0u;
    e_to_reload[0].mirror = 0u;
	e_to_reload[0].e_data_hp=0;
	e_to_reload[0].e_data_vx=0;
	e_to_reload[0].e_data_wait=0;
	e_to_reload[0].e_data_configured=0;
	e_to_reload[0].e_data_type=0;
	e_to_reload[0].e_data_et_collision=0;
	e_to_reload[0].e_data_e_state=0;
	e_to_reload[0].e_data_x_frameskip=0;
    e_to_reload[1].type = 0u;
    e_to_reload[1].x = 0u;
    e_to_reload[1].y = 0u;
    e_to_reload[1].mirror = 0u;
	e_to_reload[1].e_data_hp=0;
	e_to_reload[1].e_data_vx=0;
	e_to_reload[1].e_data_wait=0;
	e_to_reload[1].e_data_configured=0;
	e_to_reload[1].e_data_type=0;
	e_to_reload[1].e_data_et_collision=0;
	e_to_reload[1].e_data_e_state=0;
	e_to_reload[1].e_data_x_frameskip=0;
    e_to_reload[2].type = 0u;
    e_to_reload[2].x = 0u;
    e_to_reload[2].y = 0u;
    e_to_reload[2].mirror = 0u;
	e_to_reload[2].e_data_hp=0;
	e_to_reload[2].e_data_vx=0;
	e_to_reload[2].e_data_wait=0;
	e_to_reload[2].e_data_configured=0;
	e_to_reload[2].e_data_type=0;
	e_to_reload[2].e_data_et_collision=0;
	e_to_reload[2].e_data_e_state=0;
	e_to_reload[2].e_data_x_frameskip=0;
}

void START() {
	if(credit_step == -1){
		credit_step = 0;
		sgb_running = sgb_check();
		if(sgb_running){
			BGP_REG = DMG_PALETTE(DMG_BLACK, DMG_BLACK, DMG_BLACK, DMG_BLACK);
		}
	}
	credit_step++;
	if(sgb_running){
		manage_border(StateCredit);
	}
	//SOUND
		stop_music_on_new_state = 0;
        NR52_REG = 0x80; //Enables sound, you should always setup this first
        NR51_REG = 0xFF; //Enables all channels (left and right)
        NR50_REG = 0x77; //Max volume
	//CREDIT STEP MAP
		switch (credit_step){
			case 1://STUDIOLOADING & BN
				InitScroll(BANK(mapcredit0), &mapcredit0, collision_tiles_credits, 0);
			break;
			case 2://SLOOPYGOOP
				InitScroll(BANK(mapcredit2), &mapcredit2, collision_tiles_credits, 0);
			break;
			case 3://BACKERS
				InitScroll(BANK(mapcredit3), &mapcredit3, collision_tiles_credits, 0);
			break;
			case 4://COMUNEKO
				InitScroll(BANK(mapcredit4), &mapcredit4, collision_tiles_credits, 0);
			break;
		}

	whostalking = NOBODY;	
	PlayMusic(titlescreen, 1);
	credit_wait_time = 0u;
	generic_counter = 0u;
	INIT_FONT(font, PRINT_BKG);
	SHOW_BKG;
	
	switch (credit_step){
		case 1:
			SpriteManagerAdd(SpriteFlame, (UINT16) 6u << 3, (UINT16) 15u << 3);
			Sprite* s_flame = SpriteManagerAdd(SpriteFlame, (UINT16) 14u << 3, (UINT16) 15u << 3);
			s_flame->anim_frame = 4;
		break;
		case 3:
			PRINT(1,1, "SPECIAL THANKS TO   ");
			PRINT(1,2, "KICKSTARTER BACKERS:");
			
			PRINT(1,4, "D. C. FRAGA         ");
			PRINT(1,5, "HAYDEN CRIST        ");
			PRINT(1,6, "MARTIN GAUER        ");
			
			SpriteManagerAdd(SpriteFlame, (UINT16) 17u << 3, (UINT16) 5u << 3);
			
			PRINT(1,8, "   54&O PRODUCTION  ");
			PRINT(1,9, "   ABBY BRAUNSDORF  ");
			PRINT(1,10, "   TY LAUGHLIN      ");
			Sprite* s_flame2 = SpriteManagerAdd(SpriteFlame, (UINT16) 1u << 3, (UINT16) 9u << 3);
			s_flame->anim_frame = 4;
			
			PRINT(1,12, "      ANDREW AJELLO ");
			PRINT(1,13, "      TRAVIS YATES  ");
			PRINT(1,14, "    OLIVER HASLINGER");	
			Sprite* s_flame3 = SpriteManagerAdd(SpriteFlame, (UINT16) 3u << 3, (UINT16) 13u << 3);
			s_flame3->anim_frame = 2;		
		break;
	}
	
	SHOW_SPRITES;
}

void UPDATE() {
	credit_wait_time += 1u;
	if(credit_step == 1){
		switch(generic_counter){
			case 10u:
				Anim_StudioLoading_1();
			break;		
			case 20u:
				Anim_StudioLoading_2();
			break;
			case 30u:
				Anim_StudioLoading_0();
				generic_counter = 0;
			break;
		}
		generic_counter++;
	}
	if(credit_wait_time >= 240u || KEY_TICKED(J_START)){
		switch(credit_step){
			case 1:
			case 2:
			case 3:
				SetState(StateCredit);
			break;
			case 4:
				credit_step = 0;
				ChangeState(StateTitle, s_motherpl, -1);
			break;
		}
	}
		
}
