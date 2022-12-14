#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define SLOW_FRAMESKIPX 1
#define FAST_FRAMESKIPX 0
#define NORMAL_FRAMESKIPX 1

#define MAX_ARROWS_ONSCREEN 4

#define TILE_ARROW_SLOW 14
#define TILE_ARROW_FAST 19
#define TILE_ARROW_LEFT 17
#define TILE_ARROW_RIGHT 18

const UINT8 arrow_anim_normal[] = {1,0};
const UINT8 arrow_anim_perfo[] = {1,1};
const UINT8 arrow_anim_bastard[] = {1,2};

UINT8 arrows_onscreen = 0u;

extern void changeEstate(Sprite* s_enemy, ENEMY_STATE new_e_state) BANKED;

void START(){
    THIS->lim_x = 120u;
    if(arrows_onscreen >= MAX_ARROWS_ONSCREEN){SpriteManagerRemoveSprite(THIS);return;}
    arrows_onscreen++;
    struct ArrowData* arrow_data = (struct ArrowData*) THIS->custom_data;
    arrow_data->hit = 0u;
    arrow_data->configured = 0u;
    arrow_data->arrow_type = ARROW_UNASSIGNED;
}

void UPDATE(){
    struct ArrowData* arrow_data = (struct ArrowData*) THIS->custom_data;
    if(arrow_data->hit > 0 && arrow_data->arrow_type != ARROW_PERF){
        SpriteManagerRemoveSprite(THIS);
    }
    //CONFIGURATION
        if(arrow_data->configured == 0u || arrow_data->arrow_type == ARROW_UNASSIGNED){
            return;
        }
        if(arrow_data->configured == 1u){
            arrow_data->configured = 2u;
            switch(arrow_data->arrow_type){
                case ARROW_NORMAL:
                    SetSpriteAnim(THIS, arrow_anim_normal, 8u);
                    arrow_data->arrow_fskipx_max = 1;
                break;
                case ARROW_PERF:
                    SetSpriteAnim(THIS, arrow_anim_perfo, 8u);
                    arrow_data->arrow_fskipx_max = 0;
                break;
                case ARROW_BASTARD:
                    SetSpriteAnim(THIS, arrow_anim_bastard, 8u);
                    arrow_data->arrow_fskipx_max = 0;
                break;
            }
            arrow_data->arrow_fskipx = NORMAL_FRAMESKIPX;
            return;
        }
    //MOVEMENT
    if(arrow_data->vx < 0){
        THIS->mirror = V_MIRROR;
    }else if (arrow_data->vx != 0 && THIS->mirror != NO_MIRROR){
        THIS->mirror = NO_MIRROR;
    }
    UINT8 arrow_t_coll =0u;
    if(arrow_data->arrow_fskipx){
        arrow_data->arrow_fskipx--;
    }
    if(arrow_data->arrow_fskipx == 0){
        arrow_t_coll = TranslateSprite(THIS, arrow_data->vx << delta_time, 0);
        arrow_data->arrow_fskipx = arrow_data->arrow_fskipx_max;
        if(arrow_t_coll == TILE_ARROW_SLOW){
            if(arrow_data->arrow_fskipx_max != SLOW_FRAMESKIPX){
                arrow_data->vx = arrow_data->vx >> 1;
                arrow_data->arrow_fskipx_max = SLOW_FRAMESKIPX;
            }
            if(arrow_data->vx < 0){THIS->x -= 8u;}
            else{THIS->x += 16u;}
        }
        else if(arrow_t_coll == TILE_ARROW_FAST){
            if(arrow_data->arrow_fskipx_max != FAST_FRAMESKIPX){
                arrow_data->vx = arrow_data->vx << 1;
                arrow_data->arrow_fskipx_max = FAST_FRAMESKIPX;
            }
            if(arrow_data->vx < 0){THIS->x -= 8u;}
            else{THIS->x += 8u;}
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
				case SpriteEnemysimple:
				case SpriteEnemyattacker:
				case SpriteEnemythrower://io freccia ho colpito enemy
                    changeEstate(iarrspr, ENEMY_HIT);
                    if(arrow_data->arrow_type == ARROW_NORMAL){arrow_data->hit = 1u;}
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