#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define SLOW_FRAMESKIPX 1
#define FAST_FRAMESKIPX 0
#define NORMAL_FRAMESKIPX 1
#define NORMAL_FRAMESKIPY 4

#define MAX_ARROWS_ONSCREEN 4

#define TILE_ARROW_SLOW 14
#define TILE_ARROW_FAST 19
#define TILE_ARROW_LEFT 17
#define TILE_ARROW_RIGHT 18

UINT8 arrows_onscreen = 0u;

void changeEstate(struct EnemyData* e_info, ENEMY_STATE new_e_state) BANKED;//SpriteEnemysimple

void START(){
    if(arrows_onscreen >= MAX_ARROWS_ONSCREEN){SpriteManagerRemoveSprite(THIS);return;}
    arrows_onscreen++;
    struct ArrowData* arrow_data = (struct ArrowData*) THIS->custom_data;
    arrow_data->arrow_fskipx = NORMAL_FRAMESKIPX;
    arrow_data->arrow_fskipy = NORMAL_FRAMESKIPY;
}

void UPDATE(){
    struct ArrowData* arrow_data = (struct ArrowData*) THIS->custom_data;
    if(arrow_data->vx < 0){
        THIS->mirror = V_MIRROR;
    }else if (arrow_data->vx != 0 && THIS->mirror != NO_MIRROR){
        THIS->mirror = NO_MIRROR;
    }
    UINT8 arrow_t_coll =0u;
    if(arrow_data->arrow_fskipy){
        arrow_data->arrow_fskipy--;
        if(arrow_data->arrow_fskipy == 0){
            arrow_t_coll = TranslateSprite(THIS, 0, arrow_data->vy << delta_time);
            arrow_data->arrow_fskipy = NORMAL_FRAMESKIPY;
            if(arrow_t_coll != 0){
                SpriteManagerRemoveSprite(THIS);
            }
        }
    }
    if(arrow_data->arrow_fskipx){
        arrow_data->arrow_fskipx--;
    }
    if(arrow_data->arrow_fskipx == 0){
        arrow_t_coll = TranslateSprite(THIS, arrow_data->vx << delta_time, 0);
        arrow_data->arrow_fskipx = arrow_data->arrow_fskipx_max;
        if(arrow_t_coll == TILE_ARROW_SLOW && arrow_data->arrow_fskipx_max != SLOW_FRAMESKIPX){
            arrow_data->vx = arrow_data->vx >> 1;
            if(arrow_data->vx < 0){THIS->x -= 8u;}
            else{THIS->x += 8u;}
            arrow_data->arrow_fskipx_max = SLOW_FRAMESKIPX;
        }
        else if(arrow_t_coll == TILE_ARROW_FAST && arrow_data->arrow_fskipx_max != FAST_FRAMESKIPX){
            arrow_data->vx = arrow_data->vx << 1;
            if(arrow_data->vx < 0){THIS->x -= 8u;}
            else{THIS->x += 8u;}
            arrow_data->arrow_fskipx_max = FAST_FRAMESKIPX;
        }
        else if(arrow_t_coll == TILE_ARROW_LEFT){
            if(arrow_data->vx > 0){arrow_data->vx = -arrow_data->vx;}
            THIS->x -= 8u;
        }
        else if(arrow_t_coll == TILE_ARROW_RIGHT){
            if(arrow_data->vx < 0){arrow_data->vx = -arrow_data->vx;}
            THIS->x += 8u;
        }else if(arrow_t_coll != 0){
            SpriteManagerRemoveSprite(THIS);
        }
    }

    
	UINT8 scroll_arr_tile;
	Sprite* iarrspr;
    struct EnemyData* enemysimple_info = 0u;
	SPRITEMANAGER_ITERATE(scroll_arr_tile, iarrspr) {
		if(CheckCollision(THIS, iarrspr)) {
			switch(iarrspr->type){
				case SpriteEnemysimple://io freccia ho colpito enemy
                    enemysimple_info = (struct EnemyData*) iarrspr->custom_data;
                    changeEstate(enemysimple_info, ENEMY_HIT);
                    if(arrow_data->arrow_type == ARROW_NORMAL){SpriteManagerRemoveSprite(THIS);}
                break;
            }
        }
    };
}

void DESTROY(){
    struct ArrowData* arrow_data = (struct ArrowData*) THIS->custom_data;
    arrow_data->arrow_type = ARROW_DESTROYED;
    arrows_onscreen--;
}