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
#include "sgb_palette.h"
#include "custom_datas.h"

#define MAX_CROSSBOW_ANIM_COOLDOWN 12
#define MAX_TARGET_COUNTER 100
#define MAX_COUNTER_GROUND_ANIMATION 20

IMPORT_TILES(tiles00);
IMPORT_TILES(tilesdiagcrossbow);
IMPORT_TILES(font);

IMPORT_MAP(mapdiagcrossbow);
IMPORT_MAP(mapdiagcrossbowempty);
IMPORT_MAP(mapbonus);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern UINT16 BONUS_PUNTA_INIT_X;
extern UINT16 BONUS_PUNTA_INIT_Y;

UINT8 bank_tilesdiagcrossbow = BANK(tilesdiagcrossbow);
UINT8 bank_tiles00 = BANK(tiles00);
UINT8 bank_mapbonus = BANK(mapbonus);
UINT8 bank_mapdiagcrossbow = BANK(mapdiagcrossbow);
UINT8 bank_mapdiagcrossbowempty = BANK(mapdiagcrossbowempty);

const UINT8 const collision_bonus_tiles[] = {0, 0};//numero delle tile con zero finale

UINT8 crossbow_anim_cooldown = 0u;
Sprite* spr_punta = 0;
Sprite* target_0 = 0;
struct PuntaInfo* punta_info = 0;
struct TargetInfo* t0_info = 0;
struct MirinoInfo* mirino_info = 0;
UINT8 target_counter = 0u;
UINT8 counter_ground_animation = 0u;

void START() {
	//INIT SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0xFF; //Max volume 0x77

    //SGB COLORS
    if(sgb_check()){
		set_sgb_palette01_worldmap();
    }
	InitScroll(bank_mapbonus, &mapbonus, collision_bonus_tiles, 0);
	
    SHOW_BKG;
	
    SHOW_SPRITES;

	//WINDOW
	INIT_FONT(font, PRINT_WIN);
	WX_REG = 7;
	WY_REG = 144 - 40;
	INIT_HUD(mapdiagcrossbow);

	SHOW_WIN;

	crossbow_anim_cooldown = 0u;

	spr_punta = SpriteManagerAdd(SpritePunta, BONUS_PUNTA_INIT_X, BONUS_PUNTA_INIT_Y);
	spr_punta->x -= 3u;
	punta_info = (struct PuntaInfo*) spr_punta->custom_data;
	punta_info->punta_state = IDLE;
	scroll_target = SpriteManagerAdd(SpriteMirino, spr_punta->x, spr_punta->y - 40u);
	mirino_info = (struct MirinoInfo*) scroll_target->custom_data;
	t0_info = 0;
	counter_ground_animation = 0u;

}

void UPDATE(){
	if(KEY_PRESSED(J_DOWN) && KEY_PRESSED(J_START)){
		SetWindowY(160);
		SetState(StateExzoo);
	}
	if(KEY_TICKED(J_FIRE) && punta_info->punta_state == IDLE){
		Anim_Crossbow_1();
		punta_info->punta_state = JUST_THROWN;
		crossbow_anim_cooldown = MAX_CROSSBOW_ANIM_COOLDOWN;		
	}
	if(punta_info->punta_state == IDLE && crossbow_anim_cooldown > 0u){
		crossbow_anim_cooldown = 0u;
		Anim_Crossbow_0();
	}

	if(KEY_PRESSED(J_UP)){
		counter_ground_animation++;
		if(counter_ground_animation == MAX_COUNTER_GROUND_ANIMATION >> 1){
			Anim_Ground_Straight_1();
		}
		if(counter_ground_animation == MAX_COUNTER_GROUND_ANIMATION){
			Anim_Ground_Straight_0();
			counter_ground_animation = 0;
		}
	}

	//SPAWNING
	/*target_counter++;
	if(target_counter == (MAX_TARGET_COUNTER >> 1)){
		target_0 = SpriteManagerAdd(SpriteTarget, (UINT16) 16u << 3, (UINT16) 9u << 3);
		t0_info = (struct TargetInfo*) target_0->custom_data;
		t0_info->target_state = TARGET_INIT_AXE;
	}
	if(target_counter == MAX_TARGET_COUNTER){
		target_counter = 0u;
		target_0 = SpriteManagerAdd(SpriteTarget, (UINT16) 10u << 3, (UINT16) 20u << 3);
		t0_info = (struct TargetInfo*) target_0->custom_data;
		t0_info->target_state = TARGET_INIT_PALLONCINO;
		t0_info->vy = -1;
	}
	*/
		
}