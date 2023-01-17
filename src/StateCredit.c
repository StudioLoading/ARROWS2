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

IMPORT_TILES(tilescredit);
IMPORT_MAP(mapcredit0);
DECLARE_MUSIC(bgm_credits);
IMPORT_MAP(border2);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern struct EtoReload e_to_reload[3];


const UINT8 collision_tiles_credits[] = {1,0};
UINT8 credit_counter;
UINT8 credit_step = 0u;
UINT8 credit_wait_time;
UINT8 thunder_delay;

struct MISSION find_blackie = {.mission_title = FIND_BLACKIE, .mission_state = MISSION_STATE_ENABLED, 
.current_step = 0, .reward_quantity = 1u, .goal = 1u, .sprite_goal_type = 0};
struct MISSION missions[4];//= {find_blackie, 0, 0, 0};


void START() {
	if(credit_step == 0u){
		LOAD_SGB_BORDER(border2);
	}
	missions[0] = find_blackie;
	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	//NR50_REG = 0x44; //Max volume 0x77
	//PlayMusic(bgm_credits, 0);

	//FadeIn();
	//DISPLAY_OFF;
	switch (credit_step){
		case 0u:
			if(sgb_check()){
				set_sgb_palette01_WOLF();
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

	SHOW_BKG;
}

void UPDATE() {
	credit_wait_time += 1u;
	UINT8 cb = 0;
	if(credit_step == 0){
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
	}	
	if(KEY_TICKED(J_START)){
		StopMusic;
		//SetState(StateTitlescreen);
		return;
	}else if(KEY_TICKED(J_FIRE) || KEY_TICKED(J_JUMP)){
		credit_wait_time = 0u;
		credit_step += 1u;
		if(credit_step == 5u){
			StopMusic;
			//SetState(StateTitlescreen);
			return;
		}else{
			StopMusic;			
			SetState(StateExzoo);//StateEOverworld
		}
	}
		
}
