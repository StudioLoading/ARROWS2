#include "Banks/SetAutoBank.h"

#include "Palette.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "TilesAnimations0.h"
#include "custom_datas.h"

#define MAX_CROSSBOW_ANIM_COOLDOWN 12
#define MAX_TARGET_COUNTER 255
#define MAX_COUNTER_GROUND_ANIMATION 20
#define GATOR_MAX_KILLED 12

IMPORT_TILES(tiles00);
IMPORT_TILES(tilesdiagcrossbow);
IMPORT_TILES(font);

IMPORT_MAP(mapdiagcrossbow);
IMPORT_MAP(mapdiagcrossbowempty);
IMPORT_MAP(mapbonus);
IMPORT_MAP(fpsgatormap);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern UINT16 BONUS_PUNTA_INIT_X;
extern UINT16 BONUS_PUNTA_INIT_Y;

const UINT8 const coll_fps_gator_tiles[] = {0, 0};//numero delle tile con zero finale
const UINT8 const coll_fps_dragon_tiles[] = {0, 0};//numero delle tile con zero finale

UINT8 crossbow_anim_cooldown = 0u;
Sprite* spr_punta = 0;
Sprite* target_0 = 0;
struct PuntaInfo* punta_info = 0;
struct TargetInfo* t0_info = 0;
struct MirinoInfo* mirino_info = 0;
UINT8 target_counter = 0u;
UINT8 counter_ground_animation = 0u;
typedef enum{
	FPS_GATOR,
	FPS_DRAGON
}FPS_TYPE;
FPS_TYPE fps_level_type = FPS_GATOR;
UINT8 gator_counter = 0u;
UINT8 gator_killed = 0u;
INT8 gator_random = 0;
UINT8 scroll_wave_counter = 0;
Sprite* s_mirino = 0;
UINT16 starting_x = ((UINT16) 11u << 3);
UINT16 starting_y = ((UINT16) 11u << 3);
UINT16 delta_mirino_punta = 45u;
UINT16 fps_limit_min_x = 88u;
UINT16 fps_limit_max_x = 300u;
UINT16 fps_limit_min_y = 80u;
UINT16 fps_limit_max_y = 116u;
UINT16 strike_x = 0u;
UINT16 strike_y = 0u;
UINT16 counter_birdsky = 0u;
UINT8 counter_fish = 0u;
UINT8 fps_endgame_countdown = 160u;

extern INT8 anim_counter;
extern Sprite* s_motherow;

extern void spawn_npa(UINT8 type, UINT16 posx, UINT16 posy, UINT8 configured) BANKED;
extern void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;

void START() {
	//SCROLL LIMITS
        scroll_top_movement_limit = 56u;//56u;
        scroll_bottom_movement_limit = 56u;//80u;
	switch(fps_level_type){
		case FPS_GATOR:
			InitScroll(BANK(fpsgatormap), &fpsgatormap, 0, 0);
		break;
	}
    SHOW_BKG;	
    SHOW_SPRITES;
	//WINDOW
	INIT_FONT(font, PRINT_WIN);
	WX_REG = 7;
	WY_REG = 144 - 40;
	INIT_HUD(mapdiagcrossbow);

	SHOW_WIN;

	crossbow_anim_cooldown = 0u;
	scroll_target = SpriteManagerAdd(SpriteCamerafocus, starting_x, starting_y);
	spr_punta = SpriteManagerAdd(SpritePunta, starting_x, starting_y + delta_mirino_punta);
	punta_info = (struct PuntaInfo*) spr_punta->custom_data;
	punta_info->punta_state = IDLE;
	s_mirino = SpriteManagerAdd(SpriteMirino, scroll_target->x, scroll_target->y);
	mirino_info = (struct MirinoInfo*) s_mirino->custom_data;
	mirino_info->target = 0;
	t0_info = 0;
	counter_ground_animation = 0u;
	gator_counter = 0u;
	gator_killed = 0u;
	gator_random = 0;
	scroll_wave_counter = 0;
	counter_birdsky = 0u;
	counter_fish = 0u;
	anim_counter = 0u;
	fps_endgame_countdown = 160u;
}

void UPDATE(){
	if( punta_info->punta_state == IDLE){
		if(KEY_TICKED(J_FIRE)){
			Anim_Crossbow_1();
			punta_info->punta_state = JUST_THROWN;
			crossbow_anim_cooldown = MAX_CROSSBOW_ANIM_COOLDOWN;
			strike_x = s_mirino->x + 4;
			strike_y = s_mirino->y + 4u;
		}else if(crossbow_anim_cooldown > 0u){
			crossbow_anim_cooldown = 0u;
			Anim_Crossbow_0();
		}
	}
	//MOVING
		switch(fps_level_type){
			case FPS_GATOR:{
				scroll_wave_counter++;
				if ((scroll_wave_counter % 4) == 0){
					if (scroll_wave_counter < 124u){
						if(scroll_wave_counter > 70u){
							if((scroll_wave_counter % 8) == 0){
								scroll_target->y++;
							}
						}else{
							scroll_target->y++;
						}
					}else{
						if(scroll_wave_counter > 190u){
							if((scroll_wave_counter % 8) == 0){
								scroll_target->y--;
							}
						}else{
							scroll_target->y--;
						}
					}
				}
				if (scroll_wave_counter >= 240u){
					scroll_wave_counter = 0;
					//scroll_target->y = starting_y;
				}
				s_mirino->y = scroll_target->y;
				if(punta_info->punta_state == IDLE){
					spr_punta->y = scroll_target->y + delta_mirino_punta;
				}
				INT8 scroll_vx = 0;
				INT8 scroll_vy = 0;
				if(KEY_PRESSED(J_LEFT) || KEY_PRESSED(J_RIGHT)){
					if(KEY_PRESSED(J_LEFT) && scroll_target->x > fps_limit_min_x){
						scroll_vx = -1;
					}
					if(KEY_PRESSED(J_RIGHT) && scroll_target->x < fps_limit_max_x){
						scroll_vx = 1;
					}
				}
				if(KEY_PRESSED(J_UP) || KEY_PRESSED(J_DOWN)){
					if(KEY_PRESSED(J_UP) && scroll_target->y > fps_limit_min_y){
						scroll_vy = -1;
					}
					if(KEY_PRESSED(J_DOWN) && scroll_target->y < fps_limit_max_y){
						scroll_vy = 1;
					}
				}			
				UINT8 scroll_accel = 0;
				if(KEY_PRESSED(J_JUMP)){
					scroll_vx = scroll_vx << 1;
					scroll_vy = scroll_vy << 1;
				}
				scroll_target->x += scroll_vx;
				scroll_target->y += scroll_vy;
				s_mirino->x = scroll_target->x - 2u;
				if(punta_info->punta_state == IDLE){
					spr_punta->x = scroll_target->x - 3u;
				}
			}
			break;
		}
	//SPAWNING
		switch(fps_level_type){
			case FPS_GATOR:
				gator_random++;
				if((gator_counter - gator_killed < 2) && gator_killed < GATOR_MAX_KILLED){
					if((gator_random % 80u) == 0){
						UINT16 gator_spanwn_x = 140u;
						if(gator_random < 0){gator_spanwn_x -= gator_random;}
						else{gator_spanwn_x += gator_random;}
						UINT16 gator_spawn_y = (UINT16) 11u;
						INT8 gator_random_y = gator_random % 3;
						gator_spawn_y += gator_random_y;
						gator_spawn_y = (UINT16) gator_spawn_y << 3;
						Sprite* s_gator = SpriteManagerAdd(SpriteGator, gator_spanwn_x, gator_spawn_y);
						struct PlatformInfo* gator_data = (struct PlatformInfo*)s_gator->custom_data;
						gator_data->vy = 1;
						gator_data->clockwise = 1;
						gator_data->distance = 3u;
						gator_counter++;
					}
				}
				counter_birdsky++;
				if(counter_birdsky >= 1000){
					spawn_npa(SpriteBirdsky, scroll_target->x + ((UINT16) 5 << 3), ((UINT16) 5 << 3), 1);
					spawn_npa(SpriteBirdsky, scroll_target->x + ((UINT16) 2 << 3), ((UINT16) 7 << 3), 2);
            		if(_cpu == CGB_TYPE){
						spawn_npa(SpriteBirdsky, scroll_target->x - ((UINT16) 3 << 3), ((UINT16) 4 << 3), 2);
						spawn_npa(SpriteBirdsky, scroll_target->x - ((UINT16) 10 << 3), ((UINT16) 5 << 3), 1);
						spawn_npa(SpriteBirdsky, scroll_target->x + ((UINT16) 10 << 3), ((UINT16) 4 << 3), 1);
						counter_birdsky = 0u;
					}
				}
				counter_fish++;
				switch(counter_fish){
					case 60:
					case 120:
						spawn_npa(SpriteFish, scroll_target->x + ((UINT16) 15 << 3), ((UINT16) 12 << 3), 2);
						spawn_npa(SpriteFish, scroll_target->x - ((UINT16) 13 << 3), ((UINT16) 13 << 3) +1, 3);
						if(_cpu == CGB_TYPE){
							spawn_npa(SpriteFish, scroll_target->x - ((UINT16) 11 << 3)+1, ((UINT16) 14 << 3) -2, 3);
							spawn_npa(SpriteFish, scroll_target->x + ((UINT16) 8 << 3), ((UINT16) 12 << 3) + 2, 4);
						}
					break;
				}
			break;
		}
	//TILES ANIM
		switch(fps_level_type){
			case FPS_GATOR:
				anim_counter++;
				switch(anim_counter){
					case 8:
						Anim_FPS_gator_1();
					break;
					case 16:
						Anim_FPS_gator_0();
						anim_counter = 0;
					break;
				}
			break;
		}
	//ENDGAME
		switch(fps_level_type){
			case FPS_GATOR:
				if(gator_killed >= GATOR_MAX_KILLED){
					fps_endgame_countdown--;
					if(fps_endgame_countdown < 4){
						trigger_dialog(FISHERMAN_FPSGATOR_COMPLETED, s_motherow);
					}
				}
			break;
		}		
}