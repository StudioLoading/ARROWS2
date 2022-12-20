#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define E_GRAVITY 4
#define E_VX 1
#define E_FRAMSKIP_SNAKE 3
#define E_FRAMSKIP_RAT 1


const UINT8 e_anim_hidden[] = {1, 0};
const UINT8 snake_anim_idle[] = {1, 1}; //The first number indicates the number of frames
const UINT8 snake_anim_walk[] = {4, 3, 4, 5, 6}; //The first number indicates the number of frames
const UINT8 snake_anim_hit[] = {2, 1, 3}; //The first number indicates the number of frames
const UINT8 rat_anim_idle[] = {1, 8}; //The first number indicates the number of frames
const UINT8 rat_anim_walk[] = {3, 9, 8, 7}; //The first number indicates the number of frames
const UINT8 rat_anim_hit[] = {2, 8, 0}; //The first number indicates the number of frames

extern Sprite* s_motherpl;
extern struct MotherplData* d_motherpl;
extern UINT8 motherpl_hit;
UINT8 e_frameskip = 0u;

void configure(struct EnemyData* e_info);
void ETurn(struct EnemyData* e_info);
void changeEstate(struct EnemyData* e_info, ENEMY_STATE new_e_state) BANKED;
UINT8 getEmaxFrameskip(ENEMY_TYPE etype);

void START(){
    struct EnemyData* enemy_info = (struct EnemyData*) THIS->custom_data;
    SetSpriteAnim(THIS, e_anim_hidden, 12u);
    THIS->lim_x = 255u;
}

void UPDATE(){
    struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
    if(eu_info->configured == 0){
        return;
    }
    if(eu_info->configured == 1u && eu_info->type != UNASSIGNED){
        configure(eu_info);
        return;
    }else if (eu_info->configured != 2u){
        return;
    }
    switch(eu_info->e_state){
        case ENEMY_DEAD:
            eu_info->wait--;
            THIS->y--;
            if(eu_info->wait == 0u){SpriteManagerRemoveSprite(THIS);}
            return;
        break;
        case ENEMY_HIT:
            if(eu_info->type == SNAKE){SetSpriteAnim(THIS, snake_anim_hit, 16u);}
            if(eu_info->type == RAT){SetSpriteAnim(THIS, rat_anim_hit, 16u);}
        case ENEMY_WAIT:
            if(eu_info->wait){eu_info->wait--;}
            else{changeEstate(eu_info, ENEMY_WALK);}
            return;
        break;
        case ENEMY_WALK:
            if(eu_info->hp <= 0){changeEstate(eu_info, ENEMY_DEAD);}
        break;
    }
    //GRAVITY
    UINT8 e_v_coll = TranslateSprite(THIS, 0, E_GRAVITY << delta_time);
    //TODO check vertical collision
    //HORIZONTAL MAP COLLISION: BACK & FORTH LOGIC
    if(eu_info->x_frameskip == 0 && eu_info->e_state == ENEMY_WALK){//x_frameskip used
        eu_info->et_collision = TranslateSprite(THIS, eu_info->vx << delta_time, 0);
        if(eu_info->et_collision){
            switch(eu_info->et_collision){
                case 8u:
                case 9u:
                    ETurn(eu_info);
                break;
            }
        }
        eu_info->x_frameskip = 1u;
    }else{
        UINT8 max_frameskip = getEmaxFrameskip(eu_info->type);
        if(eu_info->x_frameskip < max_frameskip){
            eu_info->x_frameskip++;
        }else{
            eu_info->x_frameskip = 0u;
        }
    }
    //SPRITE COLLISION
    
	UINT8 scroll_e_tile;
	Sprite* iespr;
	SPRITEMANAGER_ITERATE(scroll_e_tile, iespr) {
		if(CheckCollision(THIS, iespr)) {
			switch(iespr->type){
				case SpriteMotherpl://io enemy ho colpito motherpl
                    if(motherpl_hit != 1u){motherpl_hit = 1u;}
                    changeEstate(eu_info, ENEMY_WAIT);
                break;
                case SpriteArrow:
                    changeEstate(eu_info, ENEMY_HIT);
                break;
            }
        }
    };
}

UINT8 getEmaxFrameskip(ENEMY_TYPE etype){
    UINT8 result = 0u;
    switch(etype){
        case SNAKE:
            result = E_FRAMSKIP_SNAKE;
        break;
        case RAT:
            result = E_FRAMSKIP_RAT;
        break;
    }
    return result;
}

void ETurn(struct EnemyData* e_info){
    if(e_info->vx > 0){
        e_info->vx = -E_VX;
        THIS->mirror = V_MIRROR;
    }else{
        e_info->vx = E_VX;
        THIS->mirror = NO_MIRROR;
    }
}

void configure(struct EnemyData* e_info){
    switch(e_info->type){
        case SNAKE:
            e_info->hp = 4;
            e_info->x_frameskip = 1;
        break;
        case RAT:
            e_info->hp = 4;
            e_info->x_frameskip = 1;
        break;
    }
    e_info->x_frameskip = 1u;
    e_info->vx = E_VX;
    e_info->configured = 2u;
    changeEstate(e_info, ENEMY_WALK);
}

void changeEstate(struct EnemyData* e_info, ENEMY_STATE new_e_state) BANKED{
    if(e_info->e_state != new_e_state){
        switch(new_e_state){
            case ENEMY_WALK:
                if(e_info->type == SNAKE){SetSpriteAnim(THIS, snake_anim_walk, 12u);}
                if(e_info->type == RAT){SetSpriteAnim(THIS, rat_anim_walk, 12u);}
            break;
            case ENEMY_IDLE:
                if(e_info->type == SNAKE){SetSpriteAnim(THIS, snake_anim_walk, 12u);}
                if(e_info->type == RAT){SetSpriteAnim(THIS, rat_anim_walk, 12u);}
            break;
            case ENEMY_WAIT:
                if(e_info->type == SNAKE){SetSpriteAnim(THIS, snake_anim_idle, 16u);}
                if(e_info->type == RAT){SetSpriteAnim(THIS, rat_anim_idle, 16u);}
                e_info->wait = 64u;
            break;
            case ENEMY_HIT:
                if(e_info->type == SNAKE){SetSpriteAnim(THIS, snake_anim_hit, 16u);}
                if(e_info->type == RAT){SetSpriteAnim(THIS, rat_anim_hit, 16u);}
                e_info->hp--;
                e_info->wait = 100u;
            break;
            case ENEMY_DEAD:
                e_info->wait = 24u;
                if(e_info->type == SNAKE){SetSpriteAnim(THIS, snake_anim_hit, 32u);}
                if(e_info->type == RAT){SetSpriteAnim(THIS, rat_anim_hit, 32u);}
            break;
        }
        e_info->e_state = new_e_state;
    }
}

void DESTROY(){

}