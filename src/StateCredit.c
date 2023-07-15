#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "Palette.h"
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
IMPORT_MAP(creditmap);
DECLARE_MUSIC(bgm_credits);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern struct EtoReload e_to_reload[3];
extern WHOSTALKING whostalking;
extern UINT8 stop_music_on_new_state;

const UINT8 collision_tiles_credits[] = {1,0};
UINT8 credit_step = 0u;
UINT16 credit_wait_time;
UINT8 thunder_delay;
INT8 chapter = 0;

struct MISSION find_blackie = {.mission_title = FIND_BLACKIE, 
.mission_state = MISSION_STATE_ENABLED, 
.current_step = 0, .reward_quantity = 1u, .goal = 1u, .sprite_goal_type = 0};
struct MISSION enable_hospital = {.mission_title = ENABLE_HOSPITAL, 
.mission_state = MISSION_STATE_DISABLED, 
.current_step = 0, .reward_quantity = 1u, .goal = 1u, .sprite_goal_type = 0};
struct MISSION engage_smith = {.mission_title = ENGAGE_SMITH, 
.mission_state = MISSION_STATE_DISABLED, 
.current_step = 0, .reward_quantity = 1u, .goal = 1u, .sprite_goal_type = SpritePgceme};
struct MISSION help_cemetery_woman = {.mission_title = HELP_CEMATERY_WOMAN, 
.mission_state = MISSION_STATE_DISABLED, 
.current_step = 0, .reward_quantity = 1u, .goal = 1u, .sprite_goal_type = SpritePgceme};
struct MISSION* missions[4];//= {find_blackie, engage_smith, help_cemetery_woman, 0};

struct InvItem itemMoney = {.itemtype = INVITEM_MONEY, .quantity = 10, .equippable = 1u};
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

extern void ChangeState(UINT8 new_state, Sprite* s_mother) BANKED;
extern Sprite* s_motherpl;
extern Sprite* s_motherow;

void missions_init() BANKED{
	switch(chapter){
		case 0u:
			find_blackie.mission_state = MISSION_STATE_ENABLED;
			find_blackie.current_step = 0u;
			enable_hospital.mission_state = MISSION_STATE_DISABLED;
			enable_hospital.current_step = 0u;
			engage_smith.mission_state = MISSION_STATE_DISABLED;
			engage_smith.current_step = 0u;
			help_cemetery_woman.mission_state = MISSION_STATE_DISABLED;
			help_cemetery_woman.current_step = 0u;
		break;
		case 1u:
			find_blackie.mission_state = MISSION_STATE_ACCOMPLISHED;
			find_blackie.current_step = 6u;
			enable_hospital.mission_state = MISSION_STATE_REWARDED;
			enable_hospital.current_step = 6u;
			engage_smith.mission_state = MISSION_STATE_ACCOMPLISHED;
			engage_smith.current_step = 6u;
			help_cemetery_woman.mission_state = MISSION_STATE_ENABLED;
			help_cemetery_woman.current_step = 0u;
		break;
	}
	missions[0] = &find_blackie;
	missions[1] = &engage_smith;
	missions[2] = &enable_hospital;
	missions[3] = &help_cemetery_woman;
}

void inventory_init() BANKED{
	switch(chapter){
		case 0:
			item00.itemtype = INVITEM_ARROW_NORMAL; item00.quantity = 10; item00.equippable = 1u;
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
			item00.itemtype = INVITEM_ARROW_NORMAL; item00.quantity = 30; item00.equippable = 1u;
			item01.itemtype = INVITEM_ARROW_PERFO; item01.quantity = 0; item01.equippable = 1u;
			item02.itemtype = INVITEM_ARROW_BASTARD; item02.quantity = 0; item02.equippable = 1u;
			item03.itemtype = INVITEM_BOMB; item03.quantity = 0; item03.equippable = 1u;
			item04.itemtype = INVITEM_UNASSIGNED; item04.quantity = 0; item04.equippable = 1u;
			unequip00.itemtype = INVITEM_WOOD; unequip00.quantity = 17; unequip00.equippable = 0u;//2
			unequip01.itemtype = INVITEM_METAL; unequip01.quantity = 15; unequip01.equippable = 0u;//3
			unequip02.itemtype = INVITEM_POWDER; unequip02.quantity = 1; unequip02.equippable = 0u;
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

	//FadeIn();
	//DISPLAY_OFF;
	switch (credit_step){
		case 0u:
			if(sgb_check()){
				set_sgb_palette01_WOLF();
			}
			InitScroll(BANK(creditmap), &creditmap, collision_tiles_credits, 0);
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
	
	PlayMusic(bgm_credits, 0);
	credit_wait_time = 0u;
}

void UPDATE() {
	credit_wait_time += 1u;
	/*if(credit_step == 0){
		switch(thunder_delay){
			case 20u:
				Anim_StudioLoading_1();
			break;		
			case 40u:
				Anim_StudioLoading_0();
				thunder_delay = 0;
			break;
		}
		thunder_delay++;
	}	*/
	if(credit_wait_time == 511u || KEY_TICKED(J_START) 
		|| KEY_TICKED(J_FIRE) || KEY_TICKED(J_JUMP)){
		StopMusic;
		ChangeState(StateTitlescreen, s_motherpl);// StateTitlescreen
	}
	/*if(KEY_TICKED(J_START) || KEY_TICKED(J_FIRE) || KEY_TICKED(J_JUMP)){
		credit_wait_time = 0u;
		credit_step += 1u;
		StopMusic;
		if(credit_step == 5u){
			//SetState(StateTitlescreen);
			return;
		}else{
			ChangeState(StateTitlescreen, s_motherpl);// StateTitlescreen
		}
	}*/
		
}
