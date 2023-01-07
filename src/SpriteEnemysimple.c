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
#define E_FRAMSKIP_PINE 3
#define E_FRAMSKIP_COBRA 1
#define E_FRAMSKIP_SPIDER 0


const UINT8 e_anim_hidden[] = {1, 0};
const UINT8 snake_anim_idle[] = {1, 1}; //The first number indicates the number of frames
const UINT8 snake_anim_walk[] = {4, 3, 4, 1, 2}; //The first number indicates the number of frames
const UINT8 snake_anim_hit[] = {2, 1, 3}; //The first number indicates the number of frames
const UINT8 rat_anim_idle[] = {1, 5}; //The first number indicates the number of frames
const UINT8 rat_anim_walk[] = {3, 6, 5, 7}; //The first number indicates the number of frames
const UINT8 rat_anim_hit[] = {2, 5, 0}; //The first number indicates the number of frames

UINT8 enemy_random_30_100 = 30u;

extern Sprite* s_motherpl;
extern struct MotherplData* d_motherpl;
extern UINT8 motherpl_hit;

void Estart() BANKED;
void configure(struct EnemyData* e_info) BANKED;
void ETurn(struct EnemyData* e_info, UINT8 e_vx);
void changeEstate(struct EnemyData* e_info, ENEMY_STATE new_e_state) BANKED;
UINT8 getEmaxFrameskip(ENEMY_TYPE etype);
void ErandomManagement();
void Econfiguration(struct EnemyData* eu_info) BANKED;
void Egravity();
void EhorizontalTileCollision(struct EnemyData* eu_info);
void EspriteCollision(struct EnemyData* eu_info);
void EstateBehavior(struct EnemyData* eu_info);
void Emanagement(struct EnemyData* eu_info) BANKED;

extern void spawnItem(Sprite* s_enemy, INVITEMTYPE itemtype, INT16 quantity) BANKED;
extern void EattackerAnim(struct EnemyData* eu_info, ENEMY_STATE estate) BANKED;
extern void EthrowerAnim(struct EnemyData* eu_info, ENEMY_STATE estate) BANKED;
extern void Ethrow(struct EnemyData* eu_info, ENEMY_STATE estate) BANKED;

void START(){
    Estart();
}

void UPDATE(){
    //configuration
        struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
        if(eu_info->configured == 0){
            return;
        }
        if(eu_info->configured == 1){
            Econfiguration(eu_info);
        }
    //RANDOM        
    //CHECK DEATH
        if(eu_info->hp <= 0){changeEstate(eu_info, ENEMY_DEAD);}
    //MANAGEMENT
        Emanagement(eu_info);
}

void Emanagement(struct EnemyData* eu_info) BANKED{
    ErandomManagement();
    Egravity();
    EhorizontalTileCollision(eu_info);
    EspriteCollision(eu_info); 
    EstateBehavior(eu_info);
}

void Estart() BANKED{    
    SetSpriteAnim(THIS, e_anim_hidden, 12u);
    THIS->lim_x = 255u;
}

void ErandomManagement(){
    enemy_random_30_100++;
    if(enemy_random_30_100 >= 100u){enemy_random_30_100 = 30u;}
}

void Econfiguration(struct EnemyData* eu_info) BANKED{
    if(eu_info->configured == 1u && eu_info->type != UNASSIGNED){
        configure(eu_info);
        return;
    }
}

void EhorizontalTileCollision(struct EnemyData* eu_info){
    if(eu_info->x_frameskip == 0 && (eu_info->e_state == ENEMY_WALK ||
        (eu_info->e_state == ENEMY_ATTACK && eu_info->type != SPIDER && eu_info->type != TARANTULA))){//x_frameskip used            
            eu_info->et_collision = TranslateSprite(THIS, eu_info->vx << delta_time, 0);
            if(eu_info->et_collision){
                switch(eu_info->et_collision){
                    case 8u:
                    case 9u:
                        ETurn(eu_info, E_VX);
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
}

void Egravity(){
    UINT8 e_v_coll = TranslateSprite(THIS, 0, E_GRAVITY << delta_time);
}

void EspriteCollision(struct EnemyData* eu_info){
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
                    if((s_motherpl->x < THIS->x && THIS->mirror == NO_MIRROR) || 
                        (s_motherpl->x > THIS->x && THIS->mirror == V_MIRROR)){
                            ETurn(eu_info, eu_info->vx);
                    }
                    {
                        struct ArrowData* arrow_data = (struct ArrowData*) iespr->custom_data;
                        arrow_data->hit = 1u;
                    }
                    changeEstate(eu_info, ENEMY_HIT);
                break;
            }
        }
    };
}

void EstateBehavior(struct EnemyData* eu_info){    
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
            if(eu_info->type == PORCUPINE){EattackerAnim(eu_info, eu_info->e_state);}
            if(eu_info->type == COBRA){EattackerAnim(eu_info, eu_info->e_state);}
            if(eu_info->type == SPIDER){EthrowerAnim(eu_info, eu_info->e_state);}
            if(eu_info->type == TARANTULA){EthrowerAnim(eu_info, eu_info->e_state);}
        case ENEMY_WAIT:
            if(eu_info->wait){eu_info->wait--;}
            else{changeEstate(eu_info, ENEMY_WALK);}
            return;
        break;
        case ENEMY_WALK:
            eu_info->wait--;
            if(eu_info->wait == 0u){
                if(eu_info->type == PORCUPINE || eu_info->type == COBRA 
                    || eu_info->type == SPIDER || eu_info->type == TARANTULA){
                    changeEstate(eu_info, ENEMY_PREATTACK);
                }
            }
        break;
        case ENEMY_PREATTACK:
            eu_info->wait--;
            if(eu_info->wait == 0u){
                switch(eu_info->type){
                    case PORCUPINE:
                    case COBRA:
                        changeEstate(eu_info, ENEMY_ATTACK);
                    break;
                    case SPIDER:
                    case TARANTULA:
                        changeEstate(eu_info, ENEMY_THROW);
                    break;
                }
            }
        break;
        case ENEMY_ATTACK:
            eu_info->wait--;
            if(eu_info->wait == 0u){changeEstate(eu_info, ENEMY_WALK);}
        break;
        case ENEMY_THROW:
            eu_info->wait--;
            if(eu_info->wait == 0u){changeEstate(eu_info, ENEMY_WAIT);}
        break;

    }
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
        case PORCUPINE:
            result = E_FRAMSKIP_PINE;
        break;
        case COBRA:
            result = E_FRAMSKIP_COBRA;
        break;
        case SPIDER:
            result = E_FRAMSKIP_SPIDER;
        break;
    }
    return result;
}

void ETurn(struct EnemyData* e_info, UINT8 e_vx){
    if(e_info->vx > 0){
        e_info->vx = -e_vx;
        THIS->mirror = V_MIRROR;
        THIS->x--;
    }else{
        e_info->vx = e_vx;
        THIS->mirror = NO_MIRROR;
        THIS->x++;
    }
    e_info->wait = 24u;
    //if(e_info->e_state != ENEMY_WALK){
        changeEstate(e_info, ENEMY_WAIT);
    //}
}

void configure(struct EnemyData* e_info) BANKED{
    switch(e_info->type){
        case SNAKE:
            e_info->hp = 1;
        break;
        case RAT:
            e_info->hp = 3;
        break;
        case PORCUPINE:
            e_info->hp = 2;
        break;
        case COBRA:
            e_info->hp = 4;
        break;
        case SPIDER:
            e_info->hp = 2;
        break;
        case TARANTULA:
            e_info->hp = 2;
        break;
    }
    e_info->x_frameskip = 1u;
    e_info->vx = E_VX;
    e_info->configured = 2u;
    changeEstate(e_info, ENEMY_WALK);
}

void changeEstate(struct EnemyData* e_info, ENEMY_STATE new_e_state) BANKED{
    if(new_e_state == ENEMY_HIT && e_info->e_state == ENEMY_ATTACK){
        return;//Invulnerabilità durante l' attacco!
    }
    if(e_info->e_state != new_e_state && e_info->e_state != ENEMY_DEAD){
        switch(new_e_state){
            case ENEMY_WALK:
                if(e_info->type == SNAKE){SetSpriteAnim(THIS, snake_anim_walk, 12u);}
                if(e_info->type == RAT){SetSpriteAnim(THIS, rat_anim_walk, 12u);}
                if(e_info->type == PORCUPINE){
                    e_info->x_frameskip = E_FRAMSKIP_PINE;
                    e_info->wait = enemy_random_30_100 + 60u;
                    EattackerAnim(e_info, new_e_state);
                }
                if(e_info->type == COBRA){
                    e_info->x_frameskip = E_FRAMSKIP_COBRA;
                    e_info->wait = enemy_random_30_100 + 100u;
                    EattackerAnim(e_info, new_e_state);
                }
                if(e_info->type == SPIDER){
                    e_info->x_frameskip = E_FRAMSKIP_SPIDER;
                    e_info->wait = enemy_random_30_100 + 100u;
                    EthrowerAnim(e_info, new_e_state);
                }
                if(e_info->type == TARANTULA){
                    e_info->x_frameskip = E_FRAMSKIP_SPIDER;
                    e_info->wait = enemy_random_30_100 + 100u;
                    EthrowerAnim(e_info, new_e_state);
                }
            break;
            case ENEMY_IDLE:
                if(e_info->type == SNAKE){SetSpriteAnim(THIS, snake_anim_walk, 12u);}
                if(e_info->type == RAT){SetSpriteAnim(THIS, rat_anim_walk, 12u);}
                if(e_info->type == PORCUPINE){EattackerAnim(e_info, new_e_state);}
                if(e_info->type == COBRA){EattackerAnim(e_info, new_e_state);}
                if(e_info->type == SPIDER){EthrowerAnim(e_info, new_e_state);}
                if(e_info->type == TARANTULA){EthrowerAnim(e_info, new_e_state);}
            break;
            case ENEMY_WAIT:
                if(e_info->type == SNAKE){SetSpriteAnim(THIS, snake_anim_idle, 16u);}
                if(e_info->type == RAT){SetSpriteAnim(THIS, rat_anim_idle, 16u);}
                if(e_info->type == PORCUPINE){EattackerAnim(e_info, new_e_state);}
                if(e_info->type == COBRA){EattackerAnim(e_info, new_e_state);}
                if(e_info->type == SPIDER){EthrowerAnim(e_info, new_e_state);}
                if(e_info->type == TARANTULA){EthrowerAnim(e_info, new_e_state);}
                e_info->wait = 40u;
            break;
            case ENEMY_HIT:
                if(e_info->type == SNAKE){SetSpriteAnim(THIS, snake_anim_hit, 16u);}
                if(e_info->type == RAT){SetSpriteAnim(THIS, rat_anim_hit, 16u);}
                if(e_info->type == PORCUPINE){EattackerAnim(e_info, new_e_state);}
                if(e_info->type == COBRA){EattackerAnim(e_info, new_e_state);}
                if(e_info->type == SPIDER){EthrowerAnim(e_info, new_e_state);}
                if(e_info->type == TARANTULA){EthrowerAnim(e_info, new_e_state);}
                e_info->hp--;
                e_info->wait = 56u;
                if(e_info->hp <= 0u){
                    changeEstate(e_info, ENEMY_DEAD);
                    return;
                }
            break;
            case ENEMY_DEAD:
                spawnItem(THIS, INVITEM_CROSSBOW, 20u);
                e_info->wait = 24u;
                if(e_info->type == SNAKE){SetSpriteAnim(THIS, snake_anim_hit, 32u);}
                if(e_info->type == RAT){SetSpriteAnim(THIS, rat_anim_hit, 32u);}
                if(e_info->type == PORCUPINE){EattackerAnim(e_info, new_e_state);}
                if(e_info->type == COBRA){EattackerAnim(e_info, new_e_state);}
                if(e_info->type == SPIDER){EthrowerAnim(e_info, new_e_state);}
                if(e_info->type == TARANTULA){EthrowerAnim(e_info, new_e_state);}
            break;
            case ENEMY_PREATTACK:
                e_info->wait = 40u;
                if(e_info->type == PORCUPINE){EattackerAnim(e_info, new_e_state);}
                if(e_info->type == COBRA){EattackerAnim(e_info, new_e_state);}
                if(e_info->type == SPIDER){EthrowerAnim(e_info, new_e_state);}
                if(e_info->type == TARANTULA){EthrowerAnim(e_info, new_e_state);}
            break;
            case ENEMY_ATTACK:
                if(THIS->mirror == NO_MIRROR){e_info->vx = 2*E_VX;}
                else{e_info->vx = 2* (-E_VX);}
                e_info->wait = enemy_random_30_100;
                if(e_info->type == PORCUPINE){EattackerAnim(e_info, new_e_state);}
                if(e_info->type == COBRA){EattackerAnim(e_info, new_e_state);}
                e_info->x_frameskip = 0u;
            break;
            case ENEMY_THROW:
                Ethrow(e_info, e_info->e_state);
            break;
        }
        e_info->e_state = new_e_state;
    }
}


void DESTROY(){
}