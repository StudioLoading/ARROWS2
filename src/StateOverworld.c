#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Print.h"
#include "Sound.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "Dialogs.h"
#include "DialogTips.h"

IMPORT_TILES(font);
IMPORT_TILES(tilesowsouthwest);

IMPORT_MAP(owsouthwest);
IMPORT_MAP(ownorthwest);
IMPORT_MAP(owmaze);
IMPORT_MAP(owsoutheast);
IMPORT_MAP(oweast);
IMPORT_MAP(hudow);

extern UINT8 scroll_top_movement_limit;
extern UINT8 scroll_bottom_movement_limit;
extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 collision_tiles_ow_sw[] = {3, 4, 16, 17, 18, 23, 24, 25, 26, 28, 29, 32, 
33, 34, 39, 41, 42, 43, 44, 45, 46, 47, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
62, 63, 64, 65, 66, 67, 68, 70, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 83,
84, 85, 86, 87, 88, 90, 91, 95, 96, 112, 113, 114, 115, 116, 117, 118, 119, 120,
121, 122, 123, 124, 125, 126, 127, 134, 135, 136, 140, 141, 142, 140, 148, 149, 
150, 151, 152, 153, 154, 155, 156, 157, 159, 160, 161, 162, 
164, 165, 166, 167, 168, 169, 170, 171, 193,
0};
const UINT8 collision_tiles_maze[] = {1, 2, 3, 4, 5, 8, 9, 10, 11, 12, 13, 14, 15,
18, 19, 20, 0};
UINT8 current_map = MAP_SOUTHWEST;//0=south-west, 1=north-west, 2=labyrinth, 3=south-east, 4=east
UINT8 hudow_opened = 0;
UINT8 show_tip = 0u;
INT8 show_tip_movingscroll_x = 0u;
INT8 show_tip_movingscroll_y = 0u;
UINT8 showed_tip = 0u;
UINT8 showed_tip_goback = 0u;
UINT8 showing_tip_line = 0u;
UINT8 showing_tip_delay = 8u;
UINT16 lim_up_y = ((UINT16) 9u << 3);
UINT16 lim_down_y = ((UINT16) 46u << 3);
UINT16 lim_east_x = ((UINT16) 46u << 3);
UINT16 lim_west_x = ((UINT16) 46u << 3);
HUD_OPTION owhudopt = OW_DIARY;
Sprite* s_motherow = 0;
TIP_TO_BE_LOCALIZED tip_to_show = TIP_NOTHING;
UINT8 delay_spawning = 8u;
UINT8 anim_counter = 0u;
HOOD_TYPE hood_type = NORTH_SOUTH;  

extern UINT16 motherow_pos_x;
extern UINT16 motherow_pos_y;
extern unsigned char EMPTY_STRING_21[];
extern unsigned char D0[];
extern unsigned char d1[];
extern unsigned char d2[];
extern unsigned char d3[];
extern unsigned char d4[];
extern struct MISSION find_blackie;
extern struct MISSION enable_hospital;
extern struct MISSION help_cemetery_woman;
extern struct MISSION outwalker_chief;
extern struct MISSION outwalker_glass;
extern struct MISSION defeat_scorpions;
extern struct MISSION find_antidote;
extern struct MISSION hungry_people;
extern struct MISSION fix_bridge;
extern MOTHERPL_STATE motherpl_state;
extern WHOSTALKING whostalking;
extern UINT8 child_hooked;
extern CHAPTERS chapter;
extern UINT8 previous_state;
extern FA2OW_SPRITE_STATES motherow_new_state;
extern UINT8 just_started;
extern UINT16 motherpl_pos_x;
extern UINT16 motherpl_pos_y;
extern UINT8 mapwidth;
extern UINT8 mapheight;
extern UINT8 ow_is_beach;

void PauseGameOW();
void UnpauseGameOW();
void UpdateHUDOW();
void DrawHUD(HUD_OPTION opt);
void ShowTipOW() BANKED;
void spawn_step(UINT16 stepx, UINT16 stepy) BANKED;
void showing_tip(); 

extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;
extern void update_position_motherow() BANKED;
extern void ow_tips(Sprite* s_motherow_arg, TIP_TO_BE_LOCALIZED forced_tip) BANKED;
extern void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;

extern void initial_ow_npc() BANKED;
extern void initial_ow_items() BANKED;
extern void initial_sprite_spawning() BANKED;
extern void maze_teleport() BANKED;

void START(){
	//SCROLL LIMITS
		scroll_top_movement_limit = 56u;
		scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
		show_tip = 0u;
		if(chapter == CHAPTER_4_SHIP){
			s_motherow = SpriteManagerAdd(SpriteMotherowarmor, motherow_pos_x, motherow_pos_y);
		}else{
			s_motherow = SpriteManagerAdd(SpriteMotherow, motherow_pos_x, motherow_pos_y);
		}
		switch (current_map){
			case MAP_SOUTHWEST://south-west
				scroll_target = SpriteManagerAdd(SpriteCamerafocus, motherow_pos_x, motherow_pos_y);
				InitScroll(BANK(owsouthwest), &owsouthwest, collision_tiles_ow_sw, 0);
				GetMapSize(BANK(owsouthwest), &owsouthwest, &mapwidth, &mapheight);
			break;
			case MAP_NORTHWEST://north-west
				motherow_new_state = IDLE_UP;
				scroll_target = SpriteManagerAdd(SpriteCamerafocus, motherow_pos_x, motherow_pos_y);
				InitScroll(BANK(ownorthwest), &ownorthwest, collision_tiles_ow_sw, 0);				
				//SpriteManagerAdd(SpriteArmor, (UINT16) 14u << 3, (UINT16) 40u << 3);
				GetMapSize(BANK(ownorthwest), &ownorthwest, &mapwidth, &mapheight);
			break;
			case MAP_MAZE://labyrinth
				motherow_new_state = IDLE_RIGHT;
				scroll_target = SpriteManagerAdd(SpriteCamerafocus, motherow_pos_x, motherow_pos_y);
				InitScroll(BANK(owmaze), &owmaze, collision_tiles_maze, 0);				
				GetMapSize(BANK(owmaze), &owmaze, &mapwidth, &mapheight);
			break;
			case MAP_SOUTHEAST://south-east
				motherow_new_state = IDLE_RIGHT;
				scroll_target = SpriteManagerAdd(SpriteCamerafocus, motherow_pos_x, motherow_pos_y);
				InitScroll(BANK(owsoutheast), &owsoutheast, collision_tiles_ow_sw, 0);
				GetMapSize(BANK(owsoutheast), &owsoutheast, &mapwidth, &mapheight);
			break;
			case MAP_EAST://east
				motherow_new_state = IDLE_RIGHT;
				scroll_target = SpriteManagerAdd(SpriteCamerafocus, motherow_pos_x, motherow_pos_y);
				InitScroll(BANK(oweast), &oweast, collision_tiles_ow_sw, 0);
				GetMapSize(BANK(oweast), &oweast, &mapwidth, &mapheight);
			break;
		}
		delay_spawning = 8u;
	//CUTSCENES
		if(help_cemetery_woman.current_step == 4u){
			//non capisco perché child_hooked è a zero anche quando ho portato bene
			help_cemetery_woman.mission_state = MISSION_STATE_REWARDED;
			help_cemetery_woman.current_step = 5u;
			SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
			trigger_dialog(CHILDS_SAVED, s_motherow);
		}
	INIT_FONT(font, PRINT_WIN);
	INIT_HUD(hudow); 
	HIDE_WIN;
	SHOW_SPRITES;
	hudow_opened = 0;
	//LIMITS
		switch(current_map){
			case MAP_SOUTHWEST://SOUTH WEST
				lim_up_y = ((UINT16) 9u << 3);
				lim_east_x = ((UINT16) 46u << 3);
				lim_west_x = ((UINT16) 2u << 3);
				lim_down_y = ((UINT16) 200u << 3);
			break;
			case MAP_NORTHWEST://NORTH WEST
				lim_up_y = ((UINT16) 200u << 3);
				lim_down_y = ((UINT16) 48u << 3);
				lim_west_x = ((UINT16) 2u << 3);
				lim_east_x = ((UINT16) 79u << 3);
			break;
			case MAP_MAZE://MAZE
				lim_up_y = ((UINT16) 200u << 3);
				lim_down_y = ((UINT16) 37u << 3);
				lim_west_x = ((UINT16) 2u << 3);
			break;
			case MAP_SOUTHEAST://SOUTH EAST
				lim_up_y = ((UINT16) 4u << 3);
				lim_down_y = ((UINT16) 50u << 3);
				lim_west_x = ((UINT16) 3u << 3);
			break;
			case MAP_EAST://EAST
				lim_up_y = ((UINT16) 6u << 3);
				lim_down_y = ((UINT16) 56u << 3);
				lim_west_x = ((UINT16) 6u << 3);
			break;
		}
	//INITIAL TIPS
		if(just_started == 2){//got to show "PRESS SELECT" tip
			ow_tips(s_motherow, TIP_PRESS_SELECT);
		}
}

void ShowTipOW() BANKED{
	/*if(show_tip == 0){
        THIS->x += 2;
	}*/
	show_tip = 1u;
	if(showed_tip == 0u){
		UINT8 scrolling = 0u;
		if(scroll_target->x < 90u){
			show_tip_movingscroll_x+=2;
			scroll_target->x+=2;
			scrolling = 1;
		}
		if(scroll_target->x > ((mapwidth<<3)-80)){
			show_tip_movingscroll_x-=2;
			scroll_target->x-=2;
			scrolling = 1;
		} 
		if(show_tip_movingscroll_y < 64u){
			show_tip_movingscroll_y+=2;
			scroll_target->y+=2;
			scrolling = 1;
		}
		if(scrolling == 0){
			if(print_target != PRINT_BKG){
			//SFX
				my_play_fx(CHANNEL_2, 60, 0x8b, 0x67, 0xa5, 0x84, 0x00);//SFX_OW_TIP
				print_target = PRINT_BKG;
			}
			showing_tip();
		}
	}
	if(showed_tip_goback == 1u){//SpriteMotherow lo setta a 1
		UINT8 scrolling = 0u;
		if(scroll_target->x < 90){
			scrolling = 1;
			show_tip_movingscroll_x-=2;
			scroll_target->x-=2;
		}
		if(scroll_target->x > ((mapwidth<<3)-80)){
			scrolling = 1;
			show_tip_movingscroll_x+=2;
			scroll_target->x+=2;
		}
		if(show_tip_movingscroll_y > -24u){
			scrolling = 1;
			show_tip_movingscroll_y-=2;
			scroll_target->y-=2;
		}
		if(scrolling == 0){
			tip_to_show = TIP_NOTHING;
			show_tip_movingscroll_x = 0u;
			show_tip_movingscroll_y = 0u;
			showed_tip_goback = 0u;
			showed_tip = 0u;
			show_tip = 0u;
		}
	}
}

void spawn_step(UINT16 stepx, UINT16 stepy) BANKED{
	SpriteManagerAdd(SpriteMotherowstep, stepx, stepy);
}

void UPDATE(){
	//ANIMATION
		if(current_map != MAP_MAZE){// 2 means labyrinth
			anim_counter++;
			switch(anim_counter){//see animation
				case 24u:
				case 120u:
					Anim_Ow_see_1();
				break;
				case 48u:
					Anim_Ow_see_2();
				break;
				case 72u:
				case 144u:
					Anim_Ow_see_3();
				break;
				case 96u:
					Anim_Ow_see_0();
				break;
			}
			switch(anim_counter){//grass & waves on sand
				case 40u:
					Anim_Ow_1();
				break;
				case 80u:
					Anim_Ow_2();
				break;
				case 90u:
					Anim_Ow_3();
				break;
				case 120u:
				break;
				case 130u:
					Anim_Ow_4();
				break;
				case 160u:
				break;
				case 170u:
					Anim_Ow_5();
				break;
				case 180u:
					Anim_Ow_0();
				break;
			}
			switch(anim_counter){//flag
				case 1u:
				case 45u:
				case 90u:
				case 135u:
					Anim_Ow_flag_1();
				break;
				case 10u:
				case 55u:
				case 100u:
				case 145u:
					Anim_Ow_flag_2();
				break;
				case 20:
				case 65:
				case 110:
				case 155u:
					Anim_Ow_flag_3();
				break;
				case 30u:
				case 75u:
				case 120u:
				case 165u:
					Anim_Ow_flag_0();
				break;
			}
			if(anim_counter >= 180u){
				anim_counter = 0u;
			}
		}
	//MAP LIMITS
		if(s_motherow->y < lim_up_y || s_motherow->x > lim_east_x 
			|| s_motherow->y > lim_down_y || s_motherow->x < lim_west_x){
		//non diminuire, ci sono problemi col ritorno camera
		//il testo rimane sullo schermo
			UINT8 alt = 0u;
			switch(chapter){
				case CHAPTER_0_BLACKIE:
					switch(current_map){
						case MAP_SOUTHWEST:
							if(s_motherow->y < lim_up_y){
								if(help_cemetery_woman.mission_state < MISSION_STATE_STARTED){
									s_motherow->y = lim_up_y + 6u;
									alt = 1;
								}else{
									ChangeState(StateHood, s_motherow, -1);
								}
							}else if(s_motherow->x > lim_east_x){
								s_motherow->x = lim_east_x - 6u;
								alt = 1;
							}
						break;
					}
				break;
				case CHAPTER_1_BANDITS:
					switch(current_map){
						case MAP_SOUTHWEST:
							if(s_motherow->y < lim_up_y){//go north to StateHood
								ChangeState(StateHood, s_motherow, -1);
							}
							if(s_motherow->x > lim_east_x){
								s_motherow->x = lim_east_x - 6u;
								alt = 1;
							}
						break;
						case MAP_NORTHWEST:						
							if(s_motherow->y > lim_down_y){//go south to StateHood
								ChangeState(StateHood, s_motherow, -1);
							}else if(s_motherow->x > lim_east_x){
								s_motherow->x = lim_east_x - 6u;
								alt = 1;
							}
						break;
						case MAP_MAZE:
							if(s_motherow->x < lim_west_x){//go back to StateOverworld NW
								ChangeState(StateOverworld, s_motherow, 1);
							}
							if(s_motherow->y > lim_down_y){
								//TODO dove spunta quando esce dal labirinto?
								if(outwalker_chief.mission_state < MISSION_STATE_ACCOMPLISHED){
									outwalker_chief.mission_state = MISSION_STATE_ACCOMPLISHED;
									outwalker_chief.current_step = 2;
									SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
								}
								motherpl_pos_x = (UINT16) 83u << 3;
								motherpl_pos_y = (UINT16) 11u << 3;
								ChangeState(StateExzoo, s_motherow, -1);
							}
						break;
					}
				break;
				case CHAPTER_2_PLAGUE:
					switch(current_map){
						case MAP_SOUTHWEST:
							if(s_motherow->y < lim_up_y){//go north to StateHood
								if(help_cemetery_woman.mission_state >= MISSION_STATE_STARTED){
									hood_type = NORTH_SOUTH;
									ChangeState(StateHood, s_motherow, -1);
								}else{
									s_motherow->y = lim_up_y + 6u;
									alt = 1;
								}
							}
							if(s_motherow->x > lim_east_x){
								if(defeat_scorpions.phase > 0){
									hood_type = EAST_WEST;
									ChangeState(StateHood, s_motherow, -1);
								}else{
									s_motherow->x = lim_east_x - 6u;
									alt = 1;
								}
							}
						break;
						case MAP_NORTHWEST:						
							if(s_motherow->y > lim_down_y){//go south to StateHood
									hood_type = NORTH_SOUTH;
								ChangeState(StateHood, s_motherow, -1);
							}else if(s_motherow->x > lim_east_x){
								s_motherow->x = lim_east_x - 6u;
								alt = 1;
							}
						break;
						case MAP_MAZE:
							if(s_motherow->x < lim_west_x){//go back to StateOverworld NW
								ChangeState(StateOverworld, s_motherow, 1);
							}
							if(s_motherow->y > lim_down_y){
								//TODO dove spunta quando esce dal labirinto?
								if(outwalker_chief.mission_state < MISSION_STATE_ACCOMPLISHED){
									outwalker_chief.mission_state = MISSION_STATE_ACCOMPLISHED;
									outwalker_chief.current_step = 2;
									SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
								}
								motherpl_pos_x = (UINT16) 83u << 3;
								motherpl_pos_y = (UINT16) 11u << 3;
								ChangeState(StateExzoo, s_motherow, -1);
							}
						break;
						case MAP_SOUTHEAST:
							if(s_motherow->x < lim_west_x){//go back to StateOverworld NW
								hood_type = EAST_WEST;
								ChangeState(StateHood, s_motherow, -1);
							}
					}
				break;
				case CHAPTER_3_ARMOUR:				
					switch(current_map){
						case MAP_SOUTHWEST:
							if(s_motherow->y < lim_up_y){//go north to StateHood
								if(help_cemetery_woman.mission_state >= MISSION_STATE_STARTED){
									hood_type = NORTH_SOUTH;
									ChangeState(StateHood, s_motherow, -1);
								}else{
									s_motherow->y = lim_up_y + 6u;
									alt = 1;
								}
							}
							if(s_motherow->x > lim_east_x){
								hood_type = EAST_WEST;
								ChangeState(StateHood, s_motherow, -1);
							}
						break;
						case MAP_NORTHWEST:						
							if(s_motherow->y > lim_down_y){//go south to StateHood
								hood_type = NORTH_SOUTH;
								ChangeState(StateHood, s_motherow, -1);
							}else if(s_motherow->x > lim_east_x){
								s_motherow->x = lim_east_x - 6u;
								alt = 1;
							}
						break;
						case MAP_MAZE:
							if(s_motherow->x < lim_west_x){//go back to StateOverworld NW
								ChangeState(StateOverworld, s_motherow, 1);
							}
							if(s_motherow->y > lim_down_y){
								motherpl_pos_x = (UINT16) 83u << 3;
								motherpl_pos_y = (UINT16) 11u << 3;
								ChangeState(StateExzoo, s_motherow, -1);
							}
						break;
						case MAP_SOUTHEAST:
							if(s_motherow->x < lim_west_x){//go back to StateOverworld NW
								hood_type = EAST_WEST;
								ChangeState(StateHood, s_motherow, -1);
							}
						break;
					}
				break;
				case CHAPTER_4_SHIP:				
					switch(current_map){
						case MAP_SOUTHWEST:
							if(s_motherow->y < lim_up_y){//go north to StateHood
								hood_type = NORTH_SOUTH;
								ChangeState(StateHood, s_motherow, -1);
							}
							if(s_motherow->x > lim_east_x){
								hood_type = EAST_WEST;
								ChangeState(StateHood, s_motherow, -1);
							}
						break;
						case MAP_NORTHWEST:						
							if(s_motherow->y > lim_down_y){//go south to StateHood
								hood_type = NORTH_SOUTH;
								ChangeState(StateHood, s_motherow, -1);
							}else if(s_motherow->x > lim_east_x){
								s_motherow->x = lim_east_x - 6u;
								alt = 1;
							}
						break;
						case MAP_MAZE:
							if(s_motherow->x < lim_west_x){//go back to StateOverworld NW
								ChangeState(StateOverworld, s_motherow, 1);
							}
							if(s_motherow->y > lim_down_y){
								motherpl_pos_x = (UINT16) 83u << 3;
								motherpl_pos_y = (UINT16) 11u << 3;
								ChangeState(StateExzoo, s_motherow, -1);
							}
						break;
						case MAP_SOUTHEAST:
							if(s_motherow->x < lim_west_x){//go back to StateOverworld NW
								hood_type = EAST_WEST;
								ChangeState(StateHood, s_motherow, -1);
							}
						break;
					}
				break;
			}
			if(alt != 0){
				show_tip = 1;
				tip_to_show = TIP_STILL_SOMETHING;
			}
		}
	//CAMERA FOLLOW
		if(show_tip == 0u){
			scroll_target->x = s_motherow->x+4u;
			scroll_target->y = s_motherow->y+4u;
		}
	//IS ON THE BEACH
		switch(current_map){
			case MAP_NORTHWEST:
			case MAP_EAST:
				if(ow_is_beach==0 && s_motherow->y <  ((UINT16)16u <<3)){
					ow_is_beach = 1;
				}else if(ow_is_beach == 1 && s_motherow->y >= ((UINT16)16u <<3)){
					ow_is_beach = 0;
				}
			break;
			default: ow_is_beach = 0; break;
		}
	//INITIAL SPRITE SPAWNING
		if(delay_spawning > 0){
			delay_spawning--;
			if(delay_spawning == 0){
				if(chapter != CHAPTER_2_PLAGUE || (chapter == CHAPTER_2_PLAGUE && 
					defeat_scorpions.mission_state >= MISSION_STATE_ACCOMPLISHED)){
					initial_ow_npc();
				}
				initial_ow_items();
				initial_sprite_spawning();
			}
		}
	//START & SELECT
		if(KEY_RELEASED(J_SELECT)){
			if(just_started == 2){
				just_started = 0;
				HIDE_WIN;
				show_tip = 0u;
				showed_tip = 0;
				show_tip_movingscroll_x = 0u;
				show_tip_movingscroll_y = 0u;
				showed_tip_goback = 0u;
				ChangeState(StateDiary, s_motherow, -1);
			}
			if(showed_tip == 1u && showed_tip_goback == 0u){
			}else{	
				HIDE_WIN;
				show_tip = 0u;
				showed_tip = 0;
				show_tip_movingscroll_x = 0u;
				show_tip_movingscroll_y = 0u;
				showed_tip_goback = 0u;
				ChangeState(StateDiary, s_motherow, -1);
			}
		}
		if(KEY_TICKED(J_START)){
			if(just_started == 0 && show_tip == 0){	
				HIDE_WIN;
				show_tip = 0u;
				showed_tip = 0;
				show_tip_movingscroll_x = 0u;
				show_tip_movingscroll_y = 0u;
				showed_tip_goback = 0u;
				ChangeState(StateInventory, s_motherow, -1);
			}
		}
	//DISMISS TIP
		if(showed_tip == 1u && showed_tip_goback == 0u && just_started != 2){
			if(KEY_RELEASED(J_FIRE) || KEY_RELEASED(J_JUMP)){
				showed_tip_goback = 1u;
			}
		}
	//OPEN TIP
		if(show_tip == 1u){
			ShowTipOW();
			return;
		}
}

void showing_tip(){
	if(showed_tip == 0){
		UINT8 r = scroll_target->x % 8u;
		UINT8 xpos = (scroll_target->x >> 3) - 10u;
		if(showing_tip_line == 0){
			GetLocalizedTip_EN(tip_to_show);
			PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 3u, D0);
			PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 4u, EMPTY_STRING_21);
			PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 5u, EMPTY_STRING_21);
			PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 6u, EMPTY_STRING_21);
			PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 7u, EMPTY_STRING_21);
			showing_tip_line = 1;
		}else if (showing_tip_line < 5){
			if(showing_tip_delay > 0){showing_tip_delay--;}
			else{
				switch(showing_tip_line){
					case 1:PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 3u, d1);break;
					case 2:PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 3u, d2);break;
					case 3:PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 3u, d3);break;
					case 4:PRINT(xpos, (scroll_target->y >> 3) + showing_tip_line + 3u, d4);break;
				}
				showing_tip_delay = 24u;
				showing_tip_line++;
			}
		}else{
			showing_tip_delay = 8u;
			showing_tip_line = 0;
			showed_tip = 1u;		
		}
	}
}

void UpdateHUDOW(){
	HUD_OPTION old_owhudopt = owhudopt;
	if(KEY_RELEASED(J_DOWN)){
		owhudopt = OW_GAMEOPT;
	}else if(KEY_RELEASED(J_UP)){
		owhudopt = OW_DIARY;
	}
	if(old_owhudopt != owhudopt){
		DrawHUD(owhudopt);
		old_owhudopt = owhudopt;
	}
	if(KEY_TICKED(J_FIRE) || KEY_TICKED(J_JUMP)){
		HIDE_WIN;
		switch(owhudopt){
			case OW_DIARY://selezionato Diario Missione
				ChangeState(StateDiary, s_motherow, -1);
			break;
			case OW_GAMEOPT://selezionato Menu Partita
				ChangeState(StateOverworld, s_motherow, -1);
			break;
		}
	}
}

void DrawHUD(HUD_OPTION opt){
		INT8 tx = 0;
		switch(opt){
			case OW_GAMEOPT:			
				UPDATE_HUD_TILE(0,1,0);
				UPDATE_HUD_TILE(0,3,3);//show cursor in position 0,1
				for(tx=1;tx<19;tx++){
					UPDATE_HUD_TILE(tx,2,0);
					UPDATE_HUD_TILE(tx,4,4);//show underline
				}
				UPDATE_HUD_TILE(19,1,0);//show cursor in position 19,1
				UPDATE_HUD_TILE(19,3,3);//show cursor in position 19,1
			break;
			case OW_DIARY:
				UPDATE_HUD_TILE(0,1,3);//show cursor in position 0,1
				UPDATE_HUD_TILE(0,3,0);
				for(tx=1;tx<19;tx++){
					UPDATE_HUD_TILE(tx,2,4);//show underline
					UPDATE_HUD_TILE(tx,4,0);//show underline
				}
				UPDATE_HUD_TILE(19,1,3);//show cursor in position 19,
				UPDATE_HUD_TILE(19,3,0);//show cursor in position 19,1
			break;
		}
}
