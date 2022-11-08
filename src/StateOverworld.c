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

IMPORT_TILES(tilesowsouthwest);

IMPORT_MAP(mapowsouthwest);

DECLARE_MUSIC(bgm_credits);
IMPORT_MAP(border2);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 collision_tiles_ow_sw[] = {1, 0};
UINT8 border_set = 0u;
UINT8 current_map = 0u;//0=south-west, 1=south-east, 2=north-west, 3=north-east

void START(){
	if(border_set == 0u){
        border_set = 1u;
        switch(current_map){
		    case 0u:
                LOAD_SGB_BORDER(border2);
            break;
        }
	}
	//SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	//NR50_REG = 0x44; //Max volume 0x77
	PlayMusic(bgm_credits, 0);

	//FadeIn();
	//DISPLAY_OFF;
	switch (current_map){
		case 0u:
			if(sgb_check()){
				set_sgb_palette_7();
			}
			InitScroll(BANK(mapowsouthwest), &mapowsouthwest, collision_tiles_ow_sw, 0);
            scroll_target = SpriteManagerAdd(SpriteMotherow, 10u, 10u);
		break;
	}

	SHOW_BKG;
}

void UPDATE(){

}