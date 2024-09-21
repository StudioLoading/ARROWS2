#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "BankManager.h"
#include "Palette.h"
#include "Sound.h"
#include "Music.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Fade.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "sgb_palette.h"
#include "Dialogs.h"

extern CHAPTERS chapter;
extern UINT8 current_map;
extern struct MISSION find_blackie;
extern struct MISSION enable_hospital;
extern struct MISSION help_cemetery_woman;
extern struct MISSION outwalker_chief;
extern struct MISSION outwalker_glass;
extern struct MISSION defeat_scorpions;
extern struct MISSION find_antidote;
extern struct MISSION hungry_people;
extern struct MISSION fix_bridge;
extern UINT16 motherow_pos_x;
extern UINT16 motherow_pos_y;
extern UINT16 hidden_items_flags;
extern struct EnemyData* blackieow_data;

UINT8 ow_is_beach = 0u;

void maze_teleport() BANKED;
void initial_sprite_spawning() BANKED;
void initial_ow_npc() BANKED;
void initial_ow_items() BANKED;
void spawn_hidden_item(INVITEMTYPE type, INT8 q, INT16 x, INT16 y, UINT16 flags) BANKED;
void spawn_ow_npc(OWPEOPLETYPE type, UINT16 posx, UINT16 posy, UINT8 max_wait, UINT8 wait, INT8 init_vx, INT8 init_vy) BANKED;

void START(){}
void UPDATE(){}

void spawn_ow_npc(OWPEOPLETYPE type, UINT16 posx, UINT16 posy, UINT8 max_wait, UINT8 wait, INT8 init_vx, INT8 init_vy) BANKED{
    Sprite* s_ownpc = SpriteManagerAdd(SpriteOwpeople, posx, posy);
    struct OwPeopleData* d_ownpc = (struct OwPeopleData*) s_ownpc->custom_data;
    d_ownpc->vy = init_vy;
    d_ownpc->vx = init_vx;
    d_ownpc->wait = wait;
    d_ownpc->type = type;
    d_ownpc->et_collision = 0;
    d_ownpc->max_wait = max_wait;
}

void maze_teleport() BANKED{	
	Sprite* s_teleport10 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 12u << 3) + 6, ((UINT16) 5u << 3) + 8);
	Sprite* s_teleport11 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 17u << 3) + 6, ((UINT16) 14u << 3) + 8);
	struct TeleportInfo* teleport10_data = (struct TeleportInfo*) s_teleport10->custom_data;
	teleport10_data->dest_x = (UINT16) 19u << 3;
	teleport10_data->dest_y = (UINT16) 14u << 3;
	struct TeleportInfo* teleport11_data = (struct TeleportInfo*) s_teleport11->custom_data;
	teleport11_data->dest_x = (UINT16) 14u << 3;
	teleport11_data->dest_y = (UINT16) 5u << 3;
	Sprite* s_teleport20 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 2u << 3) + 6, ((UINT16) 18u << 3) + 8);
	Sprite* s_teleport21 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 29u << 3) + 6, ((UINT16) 24u << 3) + 8);
	struct TeleportInfo* teleport20_data = (struct TeleportInfo*) s_teleport20->custom_data;
	teleport20_data->dest_x = (UINT16) 32u << 3;
	teleport20_data->dest_y = (UINT16) 24u << 3;
	struct TeleportInfo* teleport21_data = (struct TeleportInfo*) s_teleport21->custom_data;
	teleport21_data->dest_x = (UINT16) 4u << 3;
	teleport21_data->dest_y = (UINT16) 18u << 3;

	Sprite* s_teleport30 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 41u << 3) + 6, ((UINT16) 24u << 3) + 8);
	Sprite* s_teleport31 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 40u << 3) + 6, ((UINT16) 9u << 3) + 8);
	struct TeleportInfo* teleport30_data = (struct TeleportInfo*) s_teleport30->custom_data;
	teleport30_data->dest_x = (UINT16) 42u << 3;
	teleport30_data->dest_y = (UINT16) 9u << 3;
	struct TeleportInfo* teleport31_data = (struct TeleportInfo*) s_teleport31->custom_data;
	teleport31_data->dest_x = (UINT16) 40u << 3;
	teleport31_data->dest_y = (UINT16) 24u << 3;

	Sprite* s_teleport40 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 64u << 3) + 6, ((UINT16) 6u << 3) + 8);
	Sprite* s_teleport41 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 40u << 3) + 6, ((UINT16) 34u << 3) + 8);
	struct TeleportInfo* teleport40_data = (struct TeleportInfo*) s_teleport40->custom_data;
	teleport40_data->dest_x = (UINT16) 42u << 3;
	teleport40_data->dest_y = (UINT16) 34u << 3;
	struct TeleportInfo* teleport41_data = (struct TeleportInfo*) s_teleport41->custom_data;
	teleport41_data->dest_x = (UINT16) 63u << 3;
	teleport41_data->dest_y = (UINT16) 6u << 3;

	Sprite* s_teleport50 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 29u << 3) + 6, ((UINT16) 34u << 3) + 8);
	Sprite* s_teleport51 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 68u << 3) + 6, ((UINT16) 23u << 3) + 8);
	struct TeleportInfo* teleport50_data = (struct TeleportInfo*) s_teleport50->custom_data;
	teleport50_data->dest_x = (UINT16) 68u << 3;
	teleport50_data->dest_y = (UINT16) 25u << 3;
	struct TeleportInfo* teleport51_data = (struct TeleportInfo*) s_teleport51->custom_data;
	teleport51_data->dest_x = (UINT16) 31u << 3;
	teleport51_data->dest_y = (UINT16) 34u << 3;

	Sprite* s_teleport60 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 19u << 3) + 6, ((UINT16) 5u << 3) + 8);
	Sprite* s_teleport61 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 2u << 3) + 6, ((UINT16) 26u << 3) + 8);
	struct TeleportInfo* teleport60_data = (struct TeleportInfo*) s_teleport60->custom_data;
	teleport60_data->dest_x = (UINT16) 4u << 3;
	teleport60_data->dest_y = (UINT16) 26u << 3;
	struct TeleportInfo* teleport61_data = (struct TeleportInfo*) s_teleport61->custom_data;
	teleport61_data->dest_x = (UINT16) 18u << 3;
	teleport61_data->dest_y = (UINT16) 5u << 3;

	Sprite* s_teleport70 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 3u << 3) + 6, ((UINT16) 31u << 3) + 8);
	Sprite* s_teleport71 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 14u << 3) + 6, ((UINT16) 30u << 3) + 8);
	struct TeleportInfo* teleport70_data = (struct TeleportInfo*) s_teleport70->custom_data;
	teleport70_data->dest_x = (UINT16) 16u << 3;
	teleport70_data->dest_y = (UINT16) 30u << 3;
	struct TeleportInfo* teleport71_data = (struct TeleportInfo*) s_teleport71->custom_data;
	teleport71_data->dest_x = (UINT16) 2u << 3;
	teleport71_data->dest_y = (UINT16) 31u << 3;

	Sprite* s_teleport80 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 19u << 3) + 6, ((UINT16) 3u << 3) + 8);
	Sprite* s_teleport81 = SpriteManagerAdd(SpriteTeleport, ((UINT16) 47u << 3) + 6, ((UINT16) 24u << 3) + 8);
	struct TeleportInfo* teleport80_data = (struct TeleportInfo*) s_teleport80->custom_data;
	teleport80_data->dest_x = (UINT16) 49u << 3;
	teleport80_data->dest_y = (UINT16) 24u << 3;
	struct TeleportInfo* teleport81_data = (struct TeleportInfo*) s_teleport81->custom_data;
	teleport81_data->dest_x = (UINT16) 18u << 3;
	teleport81_data->dest_y = (UINT16) 3u << 3;
}

void initial_sprite_spawning() BANKED{
	if(current_map == MAP_MAZE){maze_teleport();}
	if(current_map == MAP_SOUTHEAST){
		SpriteManagerAdd(SpriteOwfisherman, ((UINT16) 35u << 3) + 3u, ((UINT16) 7u << 3));
	}
	switch(chapter){
		case CHAPTER_0_BLACKIE:
			switch(current_map){
				case MAP_SOUTHWEST:
					if(find_blackie.current_step > 2 && find_blackie.current_step < 5u){
						Sprite* s_blackieow = SpriteManagerAdd(SpriteBlackieow, motherow_pos_x + 12u, motherow_pos_y - 8u);
						s_blackieow->mirror = V_MIRROR;
						if(find_blackie.current_step == 4u){
							find_blackie.current_step = 5u;
							SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
							find_blackie.mission_state = MISSION_STATE_REWARDED;
							blackieow_data->wait = 60u;
							blackieow_data->vx = -2;
						}
					}
				break;
			}
		break;
		case CHAPTER_1_BANDITS:
			switch(current_map){
				case MAP_SOUTHWEST://southwest
				break;
				case MAP_NORTHWEST://northwest
					Sprite* s_crabow = 0;
					if(outwalker_glass.mission_state >= MISSION_STATE_STARTED &&  outwalker_glass.mission_state <= MISSION_STATE_ACCOMPLISHED && outwalker_glass.current_step < 3){
						s_crabow = SpriteManagerAdd(SpriteOwcrab, (UINT16) 11u << 3, (UINT16) 11u << 3);	
					}
					if(outwalker_glass.mission_state == MISSION_STATE_ACCOMPLISHED 
						&& outwalker_glass.current_step == 2){
						SpriteManagerAdd(SpriteDiary, scroll_target->x, scroll_target->y);
						outwalker_glass.current_step = 3;
						struct EnemyData* crabow_data = (struct EnemyData*)s_crabow->custom_data;
						crabow_data->vx = 1;
						crabow_data->configured = 2;
					}
				break;
			}
		break;
		case CHAPTER_2_PLAGUE:
			switch(current_map){
				case MAP_SOUTHWEST:
					if(defeat_scorpions.mission_state >= MISSION_STATE_STARTED
						&& defeat_scorpions.mission_state < MISSION_STATE_ACCOMPLISHED
						&& defeat_scorpions.phase == 0u){
						if((defeat_scorpions.current_step & 0b00000001) == 0b00000000){
							Sprite* s_scor0 = SpriteManagerAdd(SpriteOwscorpion, ((UINT16) 26u << 3), ((UINT16) 26u << 3));
							struct EnemyData* s_scor0_data = (struct EnemyData*)s_scor0->custom_data;
							s_scor0_data->hp = 1;
							s_scor0_data->configured = 0b00000001;
						}
						if((defeat_scorpions.current_step & 0b00000010) == 0b00000000){
							Sprite* s_scor1 = SpriteManagerAdd(SpriteOwscorpion, ((UINT16) 23u << 3), ((UINT16) 32u << 3));
							struct EnemyData* s_scor1_data = (struct EnemyData*)s_scor1->custom_data;
							s_scor1_data->hp = -1;
							s_scor1_data->configured = 0b00000010;						
						}
						if((defeat_scorpions.current_step & 0b00000100) == 0b00000000){
							Sprite* s_scor2 = SpriteManagerAdd(SpriteOwscorpion, ((UINT16) 15u << 3), ((UINT16) 22u << 3));
							struct EnemyData* s_scor2_data = (struct EnemyData*)s_scor2->custom_data;
							s_scor2_data->hp = 1;
							s_scor2_data->configured = 0b00000100;						
						}
					}				
					if(find_antidote.phase == 4){					
						UINT8 spawn_herb = find_antidote.current_step & 0b00100000;
						if(spawn_herb == 0){
							Sprite* s_herb1 = SpriteManagerAdd(SpriteHerb, (UINT16)12u << 3, (UINT16)46u << 3);
							struct ItemSpawned* herb1_data = (struct ItemSpawned*) s_herb1->custom_data;
							herb1_data->itemtype = INVITEM_HERB;
							herb1_data->hp = 0b00100000;
						}
						spawn_herb = find_antidote.current_step & 0b01000000;
						if(spawn_herb == 0){
							Sprite* s_herb1 = SpriteManagerAdd(SpriteHerb, (UINT16)23u << 3, (UINT16)45u << 3);
							struct ItemSpawned* herb1_data = (struct ItemSpawned*) s_herb1->custom_data;
							herb1_data->itemtype = INVITEM_HERB;
							herb1_data->hp = 0b01000000;
						}
						spawn_herb = find_antidote.current_step & 0b10000000;
						if(spawn_herb == 0){
							Sprite* s_herb1 = SpriteManagerAdd(SpriteHerb, (UINT16)40u << 3, (UINT16)33u << 3);
							struct ItemSpawned* herb1_data = (struct ItemSpawned*) s_herb1->custom_data;
							herb1_data->itemtype = INVITEM_HERB;
							herb1_data->hp = 0b10000000;
						}
					}
				break;
				case MAP_NORTHWEST:
					if(defeat_scorpions.mission_state >= MISSION_STATE_STARTED
						&& defeat_scorpions.mission_state < MISSION_STATE_ACCOMPLISHED
						&& defeat_scorpions.phase == 0u){
						if((defeat_scorpions.current_step & 0b00001000) == 0b00000000){
							Sprite* s_scor0 = SpriteManagerAdd(SpriteOwscorpion, ((UINT16) 18u << 3), ((UINT16) 10u << 3));
							struct EnemyData* s_scor0_data = (struct EnemyData*)s_scor0->custom_data;
							s_scor0_data->hp = 1;
							s_scor0_data->configured = 0b00001000;
						}
						if((defeat_scorpions.current_step & 0b00010000) == 0b00000000){
							Sprite* s_scor1 = SpriteManagerAdd(SpriteOwscorpion, ((UINT16) 14u << 3), ((UINT16) 41u << 3));
							struct EnemyData* s_scor1_data = (struct EnemyData*)s_scor1->custom_data;
							s_scor1_data->hp = 1;
							s_scor1_data->configured = 0b00010000;
						}
						if((defeat_scorpions.current_step & 0b00100000) == 0b0000000){
							Sprite* s_scor2 = SpriteManagerAdd(SpriteOwscorpion, ((UINT16) 43u << 3), ((UINT16) 29u << 3));
							struct EnemyData* s_scor2_data = (struct EnemyData*)s_scor2->custom_data;
							s_scor2_data->hp = -1;
							s_scor2_data->configured = 0b00100000;
						}
						if((defeat_scorpions.current_step & 0b01000000) == 0b00000000){
							Sprite* s_scor3 = SpriteManagerAdd(SpriteOwscorpion, ((UINT16) 52u << 3), ((UINT16) 25u << 3));
							struct EnemyData* s_scor3_data = (struct EnemyData*)s_scor3->custom_data;
							s_scor3_data->hp = 1;
							s_scor3_data->configured = 0b01000000;						
						}
						if((defeat_scorpions.current_step & 0b10000000) == 0b00000000){
							Sprite* s_scor4 = SpriteManagerAdd(SpriteOwscorpion, ((UINT16) 52u << 3), ((UINT16) 30u << 3));
							struct EnemyData* s_scor4_data = (struct EnemyData*)s_scor4->custom_data;
							s_scor4_data->hp = -1;
							s_scor4_data->configured = 0b10000000;						
						}
					}
				break;
				case MAP_SOUTHEAST:
					if(defeat_scorpions.mission_state >= MISSION_STATE_STARTED
						&& defeat_scorpions.mission_state < MISSION_STATE_ACCOMPLISHED
						&& defeat_scorpions.phase == 1u){
						if((defeat_scorpions.current_step & 0b00000001) == 0b00000000){
							Sprite* s_scor0 = SpriteManagerAdd(SpriteOwscorpion, ((UINT16) 16u << 3), ((UINT16) 12u << 3));
							struct EnemyData* s_scor0_data = (struct EnemyData*)s_scor0->custom_data;
							s_scor0_data->hp = -1;
							s_scor0_data->configured = 0b00000001;
						}
						if((defeat_scorpions.current_step & 0b00000010) == 0b00000000){
							Sprite* s_scor0 = SpriteManagerAdd(SpriteOwscorpion, ((UINT16) 14u << 3), ((UINT16) 23u << 3));
							struct EnemyData* s_scor0_data = (struct EnemyData*)s_scor0->custom_data;
							s_scor0_data->hp = 1;
							s_scor0_data->configured = 0b00000010;
						}
						if((defeat_scorpions.current_step & 0b00000100) == 0b00000000){
							Sprite* s_scor0 = SpriteManagerAdd(SpriteOwscorpion, ((UINT16) 24u << 3), ((UINT16) 32u << 3));
							struct EnemyData* s_scor0_data = (struct EnemyData*)s_scor0->custom_data;
							s_scor0_data->hp = -1;
							s_scor0_data->configured = 0b00000100;
						}
						if((defeat_scorpions.current_step & 0b00001000) == 0b00000000){
							Sprite* s_scor0 = SpriteManagerAdd(SpriteOwscorpion, ((UINT16) 15u << 3), ((UINT16) 38u << 3));
							struct EnemyData* s_scor0_data = (struct EnemyData*)s_scor0->custom_data;
							s_scor0_data->hp = 1;
							s_scor0_data->configured = 0b00001000;
						}
						if((defeat_scorpions.current_step & 0b00010000) == 0b00000000){
							Sprite* s_scor0 = SpriteManagerAdd(SpriteOwscorpion, ((UINT16) 10u << 3), ((UINT16) 9u << 3));
							struct EnemyData* s_scor0_data = (struct EnemyData*)s_scor0->custom_data;
							s_scor0_data->hp = 1;
							s_scor0_data->configured = 0b00010000;
						}
					}
					if(find_antidote.phase == 4){
						UINT8 spawn_herb = find_antidote.current_step & 0b00000010;
						if(spawn_herb == 0){
							Sprite* s_herb1 = SpriteManagerAdd(SpriteHerb, (UINT16)15u << 3, (UINT16)44u << 3);
							struct ItemSpawned* herb1_data = (struct ItemSpawned*) s_herb1->custom_data;
							herb1_data->itemtype = INVITEM_HERB;
							herb1_data->hp = 0b00000010;
						}
						spawn_herb = find_antidote.current_step & 0b00000100;
						if(spawn_herb == 0){
							Sprite* s_herb2 = SpriteManagerAdd(SpriteHerb, (UINT16)27u << 3, (UINT16)41u << 3);
							struct ItemSpawned* herb2_data = (struct ItemSpawned*) s_herb2->custom_data;
							herb2_data->itemtype = INVITEM_HERB;
							herb2_data->hp = 0b00000100;
						}
						spawn_herb = find_antidote.current_step & 0b00001000;
						if(spawn_herb == 0){
							Sprite* s_herb3 = SpriteManagerAdd(SpriteHerb, (UINT16)28u << 3, (UINT16)44u << 3);
							struct ItemSpawned* herb3_data = (struct ItemSpawned*) s_herb3->custom_data;
							herb3_data->itemtype = INVITEM_HERB;
							herb3_data->hp = 0b00001000;
						}
						spawn_herb = find_antidote.current_step & 0b00010000;
						if(spawn_herb == 0){
							Sprite* s_herb4 = SpriteManagerAdd(SpriteHerb, (UINT16)27u << 3, (UINT16)29u << 3);
							struct ItemSpawned* herb4_data = (struct ItemSpawned*) s_herb4->custom_data;
							herb4_data->itemtype = INVITEM_HERB;
							herb4_data->hp = 0b00010000;
						}
					}
				break;
			}
		break;
		case CHAPTER_3_ARMOUR:
			switch(current_map){
				case MAP_SOUTHWEST:
				break;
				case MAP_SOUTHEAST:
					if(hungry_people.mission_state < MISSION_STATE_ACCOMPLISHED){
						//gators
						Sprite* gator0 = SpriteManagerAdd(SpriteOwgator, ((UINT16) 39u << 3), ((UINT16) 33u << 3));
						struct PlatformInfo* gator0_data = (struct PlatformInfo*)gator0->custom_data;
						gator0_data->vy = 1;
						gator0_data->clockwise = 1;
						gator0_data->distance = 3u;
						Sprite* gator1 = SpriteManagerAdd(SpriteOwgator, ((UINT16) 38u << 3), ((UINT16) 33u << 3));
						struct PlatformInfo* gator1_data = (struct PlatformInfo*)gator1->custom_data;
						gator1_data->vy = 1;
						gator1_data->clockwise = 1;
						gator1_data->distance = 80u;
    					if(_cpu == CGB_TYPE){
							Sprite* gator2 = SpriteManagerAdd(SpriteOwgator, ((UINT16) 39u << 3), ((UINT16) 33u << 3) + 4u);
							struct PlatformInfo* gator2_data = (struct PlatformInfo*)gator2->custom_data;
							gator2_data->vy = -1;
							gator2_data->clockwise = 0;
							gator2_data->distance = 10u;
						}
						Sprite* gator3 = SpriteManagerAdd(SpriteOwgator, ((UINT16) 39u << 3)+3, ((UINT16) 38u << 3)  -1u);
						struct PlatformInfo* gator3_data = (struct PlatformInfo*)gator3->custom_data;
						gator3_data->vy = -1;
						gator3_data->clockwise = 1;
						gator3_data->distance = 32u;
						Sprite* gator4 = SpriteManagerAdd(SpriteOwgator, ((UINT16) 41u << 3), ((UINT16) 37u << 3)  -1u);
						struct PlatformInfo* gator4_data = (struct PlatformInfo*)gator4->custom_data;
						gator4_data->vy = -1;
						gator4_data->clockwise = 0;
						gator4_data->distance = 32u;
    					if(_cpu == CGB_TYPE){
							Sprite* gator5 = SpriteManagerAdd(SpriteOwgator, ((UINT16) 41u << 3) + 6u, ((UINT16) 38u << 3));
							struct PlatformInfo* gator5_data = (struct PlatformInfo*)gator5->custom_data;
							gator5_data->vy = 1;
							gator5_data->clockwise = 1;
							gator5_data->distance = 54u;
						}
					}else if(hungry_people.mission_state == MISSION_STATE_ACCOMPLISHED){
						SpriteManagerAdd(SpriteBottle, ((UINT16) 29u << 3), ((UINT16) 9u << 3));
					}
				break;
			}
		break;
	}
}

void initial_ow_npc() BANKED{
	switch(current_map){
		case MAP_SOUTHWEST:
			spawn_ow_npc(OWTYPE_ORANGE_HORIZONTAL, ((UINT16) 39u << 3), ((UINT16) 32u << 3), 80u, 30u, -1,0);
			spawn_ow_npc(OWTYPE_BLUETUNIC_STAND, ((UINT16) 25u << 3), ((UINT16) 31u << 3), 100u, 20u, 0,0);					
			if(_cpu == CGB_TYPE){
				spawn_ow_npc(OWTYPE_ORANGE_HORIZONTAL, ((UINT16) 22u << 3), ((UINT16) 45u << 3)+1, 100u, 20u, -1,0);
				spawn_ow_npc(OWTYPE_RED_VERTICAL,((UINT16) 30u << 3), ((UINT16) 15 << 3), 50u, 16u, 0, 1);
			}
		break;
		case MAP_NORTHWEST://northwest
			spawn_ow_npc(OWTYPE_BLUETUNIC_STAND, ((UINT16) 19u << 3), ((UINT16) 39u << 3), 100u, 20u, 0,0);
			spawn_ow_npc(OWTYPE_ORANGE_HORIZONTAL, ((UINT16) 44u << 3), ((UINT16) 40u << 3)+1, 80u, 30u, 0,0);
			spawn_ow_npc(OWTYPE_KNIGHT_STAND, ((UINT16) 53u << 3)+1, ((UINT16) 29u << 3)+5, 100u, 30u, 0,0);
			spawn_ow_npc(OWTYPE_KNIGHT_STAND, ((UINT16) 56u << 3)-1, ((UINT16) 29u << 3)+5, 100u, 30u, 0,0);
			if(_cpu == CGB_TYPE){
				spawn_ow_npc(OWTYPE_RED_VERTICAL,((UINT16) 65u << 3), ((UINT16) 11 << 3), 40u, 8u, 0, 1);
				spawn_ow_npc(OWTYPE_BLUETUNIC_STAND, ((UINT16) 21u << 3)-3, ((UINT16) 39u << 3), 100u, 20u, 0,0);
				spawn_ow_npc(OWTYPE_RED_VERTICAL,((UINT16) 17u << 3) - 3u, ((UINT16) 33 << 3), 60u, 8u, 0, 1);
			}
		break;
		case MAP_SOUTHEAST:
			spawn_ow_npc(OWTYPE_RED_VERTICAL,((UINT16) 23u << 3), ((UINT16) 21 << 3), 40u, 16u, 0, 1);
			spawn_ow_npc(OWTYPE_ORANGE_HORIZONTAL, ((UINT16) 25u << 3), ((UINT16) 33u << 3)+1, 80u, 30u, -1,0);
			if(_cpu == CGB_TYPE){
				spawn_ow_npc(OWTYPE_BLUETUNIC_STAND, ((UINT16) 17u << 3), ((UINT16) 20u << 3), 100u, 20u, 0,0);
				spawn_ow_npc(OWTYPE_ORANGE_HORIZONTAL,((UINT16) 15u << 3), ((UINT16) 23 << 3), 80u, 16u, 1, 0);
				spawn_ow_npc(OWTYPE_BLUETUNIC_STAND, ((UINT16) 30u << 3), ((UINT16) 14u << 3), 100u, 20u, 0,0);
				spawn_ow_npc(OWTYPE_RED_VERTICAL,((UINT16) 27u << 3), ((UINT16) 19 << 3), 80u, 16u, 0, 1);
			}
		break;
		case MAP_EAST:
			spawn_ow_npc(OWTYPE_ORANGE_HORIZONTAL, ((UINT16) 40u << 3), ((UINT16) 34u << 3), 90u, 30u, -1,0);
			spawn_ow_npc(OWTYPE_RED_VERTICAL,((UINT16) 53u << 3), ((UINT16) 21 << 3), 100u, 16u, 0, 1);
			spawn_ow_npc(OWTYPE_BLUETUNIC_STAND, ((UINT16) 31u << 3)+4, ((UINT16) 20u << 3), 60u, 20u, 0,0);
			spawn_ow_npc(OWTYPE_KNIGHT_STAND, ((UINT16) 44u << 3), ((UINT16) 22u << 3), 100u, 30u, 0,0);
			if(_cpu == CGB_TYPE){
				spawn_ow_npc(OWTYPE_BLUETUNIC_STAND, ((UINT16) 49u << 3), ((UINT16) 28u << 3), 100u, 20u, 0,0);
				spawn_ow_npc(OWTYPE_ORANGE_HORIZONTAL, ((UINT16) 54u << 3), ((UINT16) 15u << 3), 120u, 30u, -1,0);
			}
		break;
	}
}

void initial_ow_items() BANKED{
	UINT8 multiply = chapter+1;
	switch(current_map){
		case MAP_SOUTHWEST:
			//di fronte al cimitero
			spawn_hidden_item(INVITEM_ARROW_PERFO, 5 * multiply, 38u, 29u, 0b0000000000000001);
			//a destra dell'ospedale
			spawn_hidden_item(INVITEM_ARROW_NORMAL, 10 * multiply, 40u, 13u,0b0000000000000010);
			//a sud
			spawn_hidden_item(INVITEM_WOOD, 10 * multiply, 24u, 45u, 0b0000000000000100);
		break;
		case MAP_NORTHWEST:
			//spiaggia, in basso a sinistra
			spawn_hidden_item(INVITEM_ARROW_PERFO, 10 * multiply, 4u, 14u, 0b0000000000001000);
			//spiaggia, dietro prima casa
			spawn_hidden_item(INVITEM_ARROW_BASTARD, 10 * multiply, 61u, 7u, 0b0000000000010000);
			//per terra, prima del hood a sinistra
			spawn_hidden_item(INVITEM_MONEY, 30 * multiply, 7u, 42u, 0b0000000000100000);
		break;
		/* non ce la sto facendo perché per DMG è toomuch
		case MAP_MAZE:
			//appena entri dritto, inevitabile premio
			spawn_hidden_item(INVITEM_MONEY, 20, 8u, 2u, 0b00000001);
			//corridoio stretto
			spawn_hidden_item(INVITEM_ARROW_BASTARD, 15, 47u, 5u, 0b00000010);
			//per terra, prima del hood a sinistra
			spawn_hidden_item(INVITEM_HEARTS, 1, 59u, 24u, 0b00000100);
		break;
		*/
		case MAP_SOUTHEAST:
			//in basso, fra i due alberi
			spawn_hidden_item(INVITEM_MONEY, 10 * multiply, 23u, 38u, 0b0000000001000000);
			//dietro al carpentiere
			spawn_hidden_item(INVITEM_METAL, 10 * multiply, 20u, 14u, 0b0000000010000000);
			//nei massi a nord
			spawn_hidden_item(INVITEM_WOOD, 10 * multiply, 19u, 6u, 0b0000000100000000);
		break;
		case MAP_EAST:
			//spiaggia, in basso a destra
			spawn_hidden_item(INVITEM_MONEY, 10 * multiply, 54u, 15u, 0b0000001000000000);
			//angolo in basso a destra calpestabile
			spawn_hidden_item(INVITEM_METAL, 3 * multiply, 53u, 42u, 0b0000010000000000);
			//a sud, nel corridoio verso la caverna
			spawn_hidden_item(INVITEM_WOOD, 10 * multiply, 47u, 46u, 0b0001000000000000);
		break;
	}
}

void spawn_hidden_item(INVITEMTYPE type, INT8 q, INT16 x, INT16 y, UINT16 flags) BANKED{
	if((hidden_items_flags & flags) != flags){
		Sprite* hidden_0 = SpriteManagerAdd(SpriteItemspawned, (UINT16) x <<3, (UINT16) y << 3);
		struct ItemSpawned* hidden_0_data = (struct ItemSpawned*) hidden_0->custom_data;
		hidden_0_data->quantity = q;
		hidden_0_data->itemtype = type;
		hidden_0_data->hp = 5u;
		hidden_0_data->configured = 4u;
		hidden_0_data->frmskip = flags;
	}
}


