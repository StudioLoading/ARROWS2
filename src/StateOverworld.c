#include "Banks/SetAutoBank.h"

#include "SGB.h"
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
#include "sgb_palette.h"
#include "Dialogs.h"

IMPORT_MAP(border2);

IMPORT_TILES(fontbw);
IMPORT_TILES(tilesowsouthwest);

IMPORT_MAP(owsouthwest);
IMPORT_MAP(ownorthwest);
IMPORT_MAP(owmaze);
IMPORT_MAP(hudow);


extern UINT8 scroll_top_movement_limit;
extern UINT8 scroll_bottom_movement_limit;
extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 collision_tiles_ow_sw[] = {16, 17, 18, 23, 24, 25, 26, 28, 29, 32, 
33, 34, 39, 41, 44, 45, 46, 47, 50, 51, 53, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 
68, 70, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 89, 90, 91, 95, 96, 112, 113, 
114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 0};
const UINT8 collision_tiles_maze[] = {1, 2, 3, 4, 5, 8, 9, 10, 11, 12, 13, 14, 15,
18, 19, 20, 0};
UINT8 border_set_ow = 0u;
UINT8 border_set_diary = 0u;
UINT8 border_set_exzoo = 0u;
UINT8 current_map = 0u;//0=south-west, 1=south-east, 2=north-west, 3=north-east
UINT8 hudow_opened = 0;
UINT8 show_tip = 0u;
INT8 show_tip_movingscroll = 0u;
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
TIP_TO_BE_LOCALIZED tip_to_show = TIP_SMITH_NO;
UINT8 delay_spawning = 0;
UINT8 anim_counter = 0u;

extern struct OwSpriteInfo* motherow_info;
extern UINT16 motherow_pos_x;
extern UINT16 motherow_pos_y;
extern unsigned char EMPTY_STRING_21[];
extern unsigned char D0[];
extern unsigned char d1[];
extern unsigned char d2[];
extern unsigned char d3[];
extern unsigned char d4[];
extern struct MISSION find_blackie;
extern struct MISSION help_cemetery_woman;
extern struct EnemyData* blackieow_data;
extern MOTHERPL_STATE motherpl_state;
extern WHOSTALKING whostalking;
extern UINT8 child_hooked;
extern INT8 chapter;
extern UINT8 previous_state;
extern FA2OW_SPRITE_STATES new_state;

void PauseGameOW();
void UnpauseGameOW();
void UpdateHUDOW();
void DrawHUD(HUD_OPTION opt);
void ShowTipOW() BANKED;
void initial_sprite_spawning() BANKED;
void spawn_hidden_item(INVITEMTYPE type, INT8 q, INT16 x, INT16 y) BANKED;
void spawn_step(UINT16 stepx, UINT16 stepy) BANKED;
extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;
extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;
extern void update_position_motherow() BANKED;
extern void owTips(TIP_TO_BE_LOCALIZED forced_tip) BANKED;
extern void trigger_dialog(WHOSTALKING whost, Sprite* s_mother) BANKED;

void START(){
    LOAD_SGB_BORDER(border2);
	//SOUND
		NR52_REG = 0x80; //Enables sound, you should always setup this first
		NR51_REG = 0xFF; //Enables all channels (left and right)
		NR50_REG = 0x77; //Max volume
	//SCROLL LIMITS
		scroll_top_movement_limit = 56u;
		scroll_bottom_movement_limit = 80u;
    //INIT GRAPHICS
		show_tip = 0u;
		switch (current_map){
			case 0u://south-west
				if(sgb_check()){
					set_sgb_palette_overworldsw();
				}
				s_motherow = SpriteManagerAdd(SpriteMotherow, motherow_pos_x, motherow_pos_y);
				scroll_target = SpriteManagerAdd(SpriteCamerafocus, motherow_pos_x, motherow_pos_y);
				InitScroll(BANK(owsouthwest), &owsouthwest, collision_tiles_ow_sw, 0);
			break;
			case 1u://north-west
				if(sgb_check()){
					set_sgb_palette_overworldsw();
				}
				s_motherow = SpriteManagerAdd(SpriteMotherow, motherow_pos_x, motherow_pos_y);
				new_state = IDLE_UP;
				scroll_target = SpriteManagerAdd(SpriteCamerafocus, motherow_pos_x, motherow_pos_y);
				InitScroll(BANK(ownorthwest), &ownorthwest, collision_tiles_ow_sw, 0);				
				//SpriteManagerAdd(SpriteArmor, (UINT16) 14u << 3, (UINT16) 40u << 3);
			break;
			case 2u://labyrinth
				if(sgb_check()){
					set_sgb_palette_overworldsw();
				}
				s_motherow = SpriteManagerAdd(SpriteMotherow, motherow_pos_x, motherow_pos_y);
				new_state = IDLE_RIGHT;
				scroll_target = SpriteManagerAdd(SpriteCamerafocus, motherow_pos_x, motherow_pos_y);
				InitScroll(BANK(owmaze), &owmaze, collision_tiles_maze, 0);				
			break;
		}
		delay_spawning = 80u;
	//CUTSCENES
		if(child_hooked == 1u && help_cemetery_woman.current_step == 3u){
			help_cemetery_woman.current_step = 4u;
			trigger_dialog(CHILDS_SAVED, s_motherow);
		}else if(help_cemetery_woman.current_step > 3u && help_cemetery_woman.mission_state == MISSION_STATE_ENABLED){
			help_cemetery_woman.current_step = 0u;
		}
	INIT_FONT(fontbw, PRINT_WIN);
	INIT_HUD(hudow); 
	HIDE_WIN;
	SHOW_SPRITES;
	hudow_opened = 0;
	//LIMITS
		switch(current_map){
			case 0u://SOUTH WEST
				lim_up_y = ((UINT16) 9u << 3);
				lim_east_x = ((UINT16) 46u << 3);
			break;
			case 1u://NORTH WEST
				lim_down_y = ((UINT16) 48u << 3);
				lim_east_x = ((UINT16) 79u << 3);
			break;
			case 2u://MAZE
				lim_down_y = ((UINT16) 36u << 3);
				lim_west_x = ((UINT16) 3u << 3);
			break;
		}
}

void ShowTipOW() BANKED{
	show_tip = 1u;
	if(showed_tip == 0u){
		if(show_tip_movingscroll < 64u){
			show_tip_movingscroll+=2;
			scroll_target->y+=2;
		}else{
			if(print_target != PRINT_BKG){
				print_target = PRINT_BKG;
			}
			showing_tip();
		}
	}
	if(showed_tip_goback == 1u){//SpriteMotherow lo setta a 1
		if(show_tip_movingscroll > -24u){
			show_tip_movingscroll-=2;
			scroll_target->y-=2;
		}else{
			show_tip_movingscroll = 0u;
			showed_tip_goback = 0u;
			showed_tip = 0u;
			show_tip = 0u;
		}
	}
}

void initial_sprite_spawning() BANKED{
	switch(current_map){
		case 0u:
			if(chapter == 0){
				if(find_blackie.current_step == 3u || find_blackie.current_step == 4u){
					Sprite* s_blackieow = SpriteManagerAdd(SpriteBlackieow, motherow_pos_x + 12u, motherow_pos_y - 8u);
					s_blackieow->mirror = V_MIRROR;
					if(find_blackie.current_step == 4u){
						find_blackie.current_step = 5u;
						find_blackie.mission_state = MISSION_STATE_ACCOMPLISHED;
						blackieow_data->wait = 60u;
						blackieow_data->vx = -2;
					}
				}
				spawn_hidden_item(INVITEM_ARROW_PERFO, 5, 38u, 29u);
				spawn_hidden_item(INVITEM_ARROW_NORMAL, 10, 40u, 13u);
				spawn_hidden_item(INVITEM_WOOD, 10, 24u, 45u);
			}
		break;
		case 1u:
			if(chapter == 1){
				spawn_hidden_item(INVITEM_ARROW_PERFO, 10, 20u, 41u);
			}
		break;
		case 2u://maze
			//configuring teleporting
			{
				Sprite* s_teleport0 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 23u << 3) + 4, (UINT16) 17u << 3);
				struct TeleportInfo* teleport0_data = (struct TeleportInfo*) s_teleport0->custom_data;
				teleport0_data->dest_x = (UINT16) 46u << 3;
				teleport0_data->dest_y = (UINT16) 9u << 3;
			}
		break;
	}
}

void spawn_hidden_item(INVITEMTYPE type, INT8 q, INT16 x, INT16 y) BANKED{
	Sprite* hidden_0 = SpriteManagerAdd(SpriteItemspawned, (UINT16) x <<3, (UINT16) y << 3);
	struct ItemSpawned* hidden_0_data = (struct ItemSpawned*) hidden_0->custom_data;
	hidden_0_data->quantity = q;
	hidden_0_data->itemtype = type;
	hidden_0_data->hp = 5u;
	hidden_0_data->configured = 4u; 
}

void spawn_step(UINT16 stepx, UINT16 stepy) BANKED{
	SpriteManagerAdd(SpriteMotherowstep, stepx, stepy);
}

void UPDATE(){
	//ANIMATION
		if(current_map != 2){// 2 means labyrinth
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
					anim_counter = 0u;
				break;
			}
		}
	//MAP LIMITS
		if(s_motherow->y < lim_up_y || s_motherow->x > lim_east_x || s_motherow->y > lim_down_y){
		//non diminuire, ci sono problemi col ritorno camera
		//il testo rimane sullo schermo
			switch(current_map){
				case 0u://ow south west
					if(find_blackie.current_step < 5u || help_cemetery_woman.mission_state < MISSION_STATE_STARTED){
						if(s_motherow->y < lim_up_y){
							s_motherow->y = lim_up_y + 6u;
						}
						if(s_motherow->x > lim_east_x){
							s_motherow->x = lim_east_x - 6u;
						}												
						owTips(TIP_STILL_SOMETHING);
					}else{
						if(s_motherow->y < 10u){//go north to StateHood
							ChangeState(StateHood, s_motherow, -1);
						}
					}
				break;
				case 1u://ow north west
					if(s_motherow->y > lim_down_y){//go south to StateHood
						ChangeState(StateHood, s_motherow, -1);
					}else if(s_motherow->x > lim_east_x){
						if(chapter == 1){
							s_motherow->x = lim_east_x - 6u;
							owTips(TIP_STILL_SOMETHING);
						}
					}
				break;
				case 2u:
					if(s_motherow->x < lim_west_x){//go back to StateOverworld NW
                        current_map = 1u;
                        ChangeState(StateOverworld, THIS, 1);
					}
				break;

			}
		}
	//CAMERA FOLLOW
		if(show_tip == 0u){
			scroll_target->x = s_motherow->x+4u;
			scroll_target->y = s_motherow->y+4u;
		}
	//INITIAL SPRITE SPAWNING
		if(delay_spawning > 0){
			delay_spawning--;
			if(delay_spawning == 0){
				initial_sprite_spawning();
			}
		}
	//START & SELECT
		if(KEY_RELEASED(J_SELECT)){
			/*
			switch(hudow_opened){
				case 0u://vado in 
					my_play_fx(CHANNEL_1, 60, 0x36, 0x9f, 0xf6, 0x91, 0x86);//SFX_START
					PauseGameOW();
					hudow_opened = 1u;
				break;
				case 1u:
					UnpauseGameOW();
					hudow_opened = 0u;
				break;
			}
			*/
			HIDE_WIN;
			ChangeState(StateDiary, s_motherow, -1);
		}
		if(KEY_TICKED(J_START)){	
			HIDE_WIN;
			ChangeState(StateInventory, s_motherow, -1);
		}
	/*
	if(hudow_opened == 1u){
		UpdateHUDOW();
		return;
	}
	*/
	//DISMISS TIP
		if(showed_tip == 1u && showed_tip_goback == 0u){
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
			//SFX
				my_play_fx(CHANNEL_2, 60, 0x8b, 0x67, 0xa5, 0x84, 0x00);//SFX_OW_TIP	
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
	if(KEY_TICKED(J_A)){
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
/*
void PauseGameOW(){
	//TODO metti scroll_target in idle
	print_target = PRINT_WIN;
	SHOW_WIN;
	DrawHUD(owhudopt);
	GetLocalizedLabel_EN(DIARIO_MISSIONI, d1);
	PRINT(2, 1, d1);
	GetLocalizedLabel_EN(OPZIONI_PARTITA, d2);
	PRINT(2, 3, d2);
}

void UnpauseGameOW(){
	HIDE_WIN;
}
*/
