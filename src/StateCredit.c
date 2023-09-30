#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Fade.h"
#include "Music.h"

#include "TilesAnimations0.h"
#include "custom_datas.h"
#include "sgb_palette.h"

IMPORT_MAP(border2);
IMPORT_TILES(tilescredit);
IMPORT_MAP(mapcredit0);
DECLARE_MUSIC(bgm_credits);
DECLARE_MUSIC(creditstitlescreen);
DECLARE_MUSIC(creditstitlescreen_noch2);

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

const UINT8 collision_tiles_credits[] = {1,0};
UINT8 credit_step = 0u;
UINT16 credit_wait_time;
INT8 chapter = 0;

struct MISSION find_blackie = {.mission_title = FIND_BLACKIE, 
.mission_state = MISSION_STATE_ENABLED, .current_step = 0, .reward_quantity = 1u};
struct MISSION enable_hospital = {.mission_title = ENABLE_HOSPITAL, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u};
struct MISSION engage_smith = {.mission_title = ENGAGE_SMITH, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u};
struct MISSION help_cemetery_woman = {.mission_title = HELP_CEMATERY_WOMAN, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u};
struct MISSION outwalker_chief = {.mission_title = OUTWALKER_CHIEF, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u};
struct MISSION outwalker_glass = {.mission_title = OUTWALKER_GLASSES, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u};
struct MISSION outwalker_smith = {.mission_title = OUTWALKER_SMITH, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u};
struct MISSION get_to_the_mountain = {.mission_title = GET_TO_THE_MOUNTAIN, 
.mission_state = MISSION_STATE_DISABLED, .current_step = 0, .reward_quantity = 1u};
struct MISSION* missions[9];
/* 
{find_blackie, engage_smith, help_cemetery_woman, outwalker_chief,
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
struct InvItem unequip02 = {.itemtype = INVITEM_LETTER, .quantity = 1, .equippable = 0u};
struct InvItem unequip03 = {.itemtype = INVITEM_UNASSIGNED, .quantity = 0, .equippable = 0u};
struct InvItem unequip04 = {.itemtype = INVITEM_UNASSIGNED, .quantity = 0, .equippable = 0u};
struct InvItem unequip05 = {.itemtype = INVITEM_UNASSIGNED, .quantity = 0, .equippable = 0u};
	
struct InvItem inventory[12];
struct InvItem itemEquipped = {.itemtype = INVITEM_MONEY, .quantity = 10, .equippable = 1u};

void missions_init() BANKED;
void inventory_init() BANKED;
void position_init() BANKED;

extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern Sprite* s_motherpl;
extern Sprite* s_motherow;

void missions_init() BANKED{
	switch(chapter){
		case 0u:
			current_map = 0u;
			engage_smith.mission_state = MISSION_STATE_ENABLED;
			engage_smith.current_step = 0u;
			find_blackie.mission_state = MISSION_STATE_DISABLED;
			find_blackie.current_step = 0u;
			enable_hospital.mission_state = MISSION_STATE_DISABLED;
			enable_hospital.current_step = 0u;
			help_cemetery_woman.mission_state = MISSION_STATE_DISABLED;
			help_cemetery_woman.current_step = 0u;
			outwalker_chief.mission_state = MISSION_STATE_DISABLED;
			outwalker_chief.current_step = 0u;
			outwalker_glass.mission_state = MISSION_STATE_DISABLED;
			outwalker_glass.current_step = 0u;
			outwalker_smith.mission_state = MISSION_STATE_DISABLED;
			outwalker_smith.current_step = 0u;
			get_to_the_mountain.mission_state = MISSION_STATE_DISABLED;
			get_to_the_mountain.current_step = 0u;
		break;
		case 1u:
			current_map = 1;
			find_blackie.mission_state = MISSION_STATE_REWARDED;
			find_blackie.current_step = 6u;
			enable_hospital.mission_state = MISSION_STATE_REWARDED;
			enable_hospital.current_step = 6u;
			engage_smith.mission_state = MISSION_STATE_REWARDED;
			engage_smith.current_step = 6u;
			help_cemetery_woman.mission_state = MISSION_STATE_ENABLED;
			help_cemetery_woman.current_step = 0u;
			outwalker_chief.mission_state = MISSION_STATE_DISABLED;
			outwalker_chief.current_step = 0u;//0u
			outwalker_glass.mission_state = MISSION_STATE_DISABLED;
			outwalker_glass.current_step = 0u;//0u
			outwalker_smith.mission_state = MISSION_STATE_DISABLED;
			outwalker_smith.current_step = 0u;//0b00000111;
			get_to_the_mountain.mission_state = MISSION_STATE_DISABLED;
			get_to_the_mountain.current_step = 0u;
		break;
		case 2u:
			current_map = 1;
			find_blackie.mission_state = MISSION_STATE_REWARDED;
			find_blackie.current_step = 6u;
			enable_hospital.mission_state = MISSION_STATE_REWARDED;
			enable_hospital.current_step = 6u;
			engage_smith.mission_state = MISSION_STATE_REWARDED;
			engage_smith.current_step = 6u;
			help_cemetery_woman.mission_state = MISSION_STATE_REWARDED;//ENABLED
			help_cemetery_woman.current_step = 4u;//0u
			outwalker_chief.mission_state = MISSION_STATE_REWARDED;//disabled
			outwalker_chief.current_step = 5u;//0u
			outwalker_glass.mission_state = MISSION_STATE_REWARDED;//DISABLED
			outwalker_glass.current_step = 4u;//0u
			outwalker_smith.mission_state = MISSION_STATE_REWARDED;//DISABLED
			outwalker_smith.current_step = 0b00000111;//0u
			get_to_the_mountain.mission_state = MISSION_STATE_REWARDED;//DISABLED
			get_to_the_mountain.current_step = 0u;
		break;
	}
	missions[0] = &engage_smith;
	missions[1] = &find_blackie;
	missions[2] = &enable_hospital;
	missions[3] = 0;
	missions[4] = &help_cemetery_woman;
	missions[5] = &outwalker_chief;
	missions[6] = &outwalker_glass;
	missions[7] = &outwalker_smith;
	missions[8] = &get_to_the_mountain;
}

void inventory_init() BANKED{
	switch(chapter){
		case 0:
			itemMoney.itemtype = INVITEM_MONEY;
			itemMoney.quantity = 20;
			item00.itemtype = INVITEM_ARROW_NORMAL; item00.quantity = 30; item00.equippable = 1u;
			item01.itemtype = INVITEM_ARROW_PERFO; item01.quantity = 0; item01.equippable = 1u;
			item02.itemtype = INVITEM_ARROW_BASTARD; item02.quantity = 0; item02.equippable = 1u;
			item03.itemtype = INVITEM_BOMB; item03.quantity = 0; item03.equippable = 1u;
			item04.itemtype = INVITEM_UNASSIGNED; item04.quantity = 0; item04.equippable = 1u;
			unequip00.itemtype = INVITEM_WOOD; unequip00.quantity = 26; unequip00.equippable = 0u;//2
			unequip01.itemtype = INVITEM_METAL; unequip01.quantity = 26; unequip01.equippable = 0u;//3
			unequip02.itemtype = INVITEM_LETTER; unequip02.quantity = 1; unequip02.equippable = 0u;
			unequip03.itemtype = INVITEM_UNASSIGNED; unequip03.quantity = 0; unequip03.equippable = 0u;
			unequip04.itemtype = INVITEM_UNASSIGNED; unequip04.quantity = 0; unequip04.equippable = 0u;
			unequip05.itemtype = INVITEM_UNASSIGNED; unequip05.quantity = 0; unequip05.equippable = 0u;
		break;
		case 1:
			itemMoney.itemtype = INVITEM_MONEY;
			itemMoney.quantity = 20;
			item00.itemtype = INVITEM_ARROW_NORMAL; item00.quantity = 300; item00.equippable = 1u;
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
}

void position_init() BANKED{
	switch(chapter){
		case 0u:
			current_map = 0;
			motherow_pos_x = (UINT16) 14u << 3;
			motherow_pos_y = (UINT16) 26u << 3;
		break;
		case 1u:
			current_map = 0;
			motherow_pos_x = (UINT16) 12u << 3;
			motherow_pos_y = (UINT16) 36u << 3;
			/*CRAB
			motherow_pos_x = (UINT16) 14u << 3;
			motherow_pos_y = (UINT16) 11u << 3;*/
			/*MINE
			current_map = 2;
			motherpl_pos_x = (UINT16) 4u << 3;
			motherpl_pos_y = (UINT16) 7u << 3;
			motherow_pos_x = (UINT16) 3u << 3;
			motherow_pos_y = (UINT16) 2u << 3;*/
		break;
		case 2u:
			current_map = 1;
			motherow_pos_x = (UINT16) 6u << 3;//22 MAP0
			motherow_pos_y = (UINT16) 28u << 3;//21 MAP0
			motherpl_pos_x = (UINT16) 8u << 3;
			motherpl_pos_y = (UINT16) 8u << 3;
		break;
	}
}

void START() {
    LOAD_SGB_BORDER(border2);
	missions_init();
	inventory_init();
	//SOUND
	stop_music_on_new_state = 0;
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	//NR50_REG = 0x44; //Max volume 0x77
	switch (credit_step){
		case 0u:
			if(sgb_check()){
				set_sbg_credit0();
			}
			InitScroll(BANK(mapcredit0), &mapcredit0, collision_tiles_credits, 0);
		break;
	}
	
    e_to_reload[0].type = 0u;
    e_to_reload[0].x = 0u;
    e_to_reload[0].y = 0u;
	e_to_reload[0].alive = 0u;
    e_to_reload[1].type = 0u;
    e_to_reload[1].x = 0u;
    e_to_reload[1].y = 0u;
	e_to_reload[1].alive = 0u;
    e_to_reload[2].type = 0u;
    e_to_reload[2].x = 0u;
    e_to_reload[2].y = 0u;
	e_to_reload[2].alive = 0u;

	whostalking = NOBODY;

	SHOW_BKG;
	SHOW_SPRITES;
	
	//PlayMusic(bgm_credits, 0);
	PlayMusic(creditstitlescreen_noch2, 0);
	credit_wait_time = 0u;
	generic_counter = 0u;

	SpriteManagerAdd(SpriteFlame, (UINT16) 2u << 3, (UINT16) 2u << 3);
	Sprite* s_flame = SpriteManagerAdd(SpriteFlame, (UINT16) 16u << 3, (UINT16) 2u << 3);
	s_flame->anim_frame = 4;
}

void UPDATE() {
	credit_wait_time += 1u;
	/*if(credit_step == 0){*/
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
	//}
	if(credit_wait_time == 240u || KEY_TICKED(J_START)){
		/*) 
		|| KEY_TICKED(J_FIRE) || KEY_TICKED(J_JUMP)){
		StopMusic;*/
		ChangeState(StateTitlescreen, s_motherpl, -1);// StateTitlescreen
	}
	/*if(KEY_TICKED(J_START) || KEY_TICKED(J_FIRE) || KEY_TICKED(J_JUMP)){
		credit_wait_time = 0u;
		credit_step += 1u;
		StopMusic;
		if(credit_step == 5u){
			//SetState(StateTitlescreen);
			return;
		}else{
			ChangeState(StateTitlescreen, s_motherpl, -1);// StateTitlescreen
		}
	}*/
		
}
