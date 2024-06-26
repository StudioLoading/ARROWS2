#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Palette.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Sound.h"

#include "custom_datas.h"

#define E_GRAVITY 1
#define E_VX 1
#define E_FRAMSKIP_SNAKE 3
#define E_FRAMSKIP_RAT 1
#define E_FRAMSKIP_PINE 3
#define E_FRAMSKIP_COBRA 1
#define E_FRAMSKIP_SPIDER 0
#define MAX_ENEMY 3


const UINT8 e_anim_hidden[] = {1, 0};

UINT8 enemy_random_30_100 = 30u;

extern UINT8 enemy_counter;
extern Sprite* s_motherpl;
extern struct MotherplData* d_motherpl;
extern MOTHERPL_STATE motherpl_state;
extern struct EtoReload e_to_reload[3];
extern UINT8 mapwidth;

void Estart() BANKED;
void configure() BANKED;
void ETurn(UINT8 e_vx);
void changeEstate(Sprite* s_enemy, ENEMY_STATE new_e_state) BANKED;
UINT8 getEmaxFrameskip() BANKED;
void Econfiguration() BANKED;
void Emanagement() BANKED;
void Edestroy() BANKED;
void EspawnItem() BANKED;
UINT8 is_item_equippable(INVITEMTYPE itemtype) BANKED;

extern void EsimpleSnakeAnim(ENEMY_STATE estate) BANKED;
extern void EsimpleRatAnim(ENEMY_STATE estate) BANKED;
extern void EattackerPineAnim(ENEMY_STATE estate) BANKED;
extern void EattackerCobraAnim(ENEMY_STATE estate) BANKED;
extern void EthrowerSpiderAnim(ENEMY_STATE estate) BANKED;
extern void EthrowerTarantulaAnim(ENEMY_STATE estate) BANKED;
extern void EthrowerScorpionAnim(ENEMY_STATE estate) BANKED;
extern void EthrowWeb(ENEMY_STATE estate) BANKED;
extern void EthrowAcid(ENEMY_STATE estate) BANKED;
extern void EthrowProjectile(ENEMY_STATE estate) BANKED;
extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;
extern void motherpl_hitted(Sprite* s_enemy) BANKED;

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
            Econfiguration();
            return;
        }
    //CHECK DEATH
        if(eu_info->hp <= 0){changeEstate(THIS, ENEMY_DEAD);}
    //MANAGEMENT
        Emanagement();
}

void Emanagement() BANKED{
    //map screen limit
        if(THIS->x < 10u){
            THIS->x = 10u;
        }
        if(THIS->x > ((mapwidth << 3) - 16u)){
            THIS->x = (mapwidth << 3) - 16u;
        }
    struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
    //ErandomManagement();
        enemy_random_30_100++;
        if(enemy_random_30_100 >= 100u){enemy_random_30_100 = 30u;}
    //Egravity();
        UINT8 e_v_coll = TranslateSprite(THIS, 0, E_GRAVITY << delta_time);
    //EhorizontalTileCollision(eu_info);
        if(eu_info->x_frameskip == 0 && 
            (eu_info->e_state == ENEMY_HIT_1 ||
            eu_info->e_state == ENEMY_HIT_2 ||
            eu_info->e_state == ENEMY_DEAD)
            && THIS->type != SpriteEnemyThrowerScorpion){
                INT8 hit_vx = -1;
                if(THIS->mirror != NO_MIRROR){
                    hit_vx = 1;
                }
                eu_info->et_collision = TranslateSprite(THIS, hit_vx << delta_time, 0);
        }
        if(eu_info->x_frameskip == 0 && 
            (eu_info->e_state == ENEMY_WALK ||
                (eu_info->e_state == ENEMY_ATTACK 
                && THIS->type != SpriteEnemyThrowerSpider 
                && THIS->type != SpriteEnemyThrowerTarantula
                && THIS->type != SpriteEnemyThrowerTarantula
                )
            )
        ){//x_frameskip used            
            eu_info->et_collision = TranslateSprite(THIS, eu_info->vx << delta_time, 0);
            if(eu_info->et_collision){
                switch(eu_info->et_collision){
                    case 8u:
                    case 9u:
                        ETurn(E_VX);
                    break;
                }
            }
            //if(THIS->type != SpriteEnemysimplesnake){
                eu_info->x_frameskip = getEmaxFrameskip();//1u;
            //}
        }else{
            UINT8 max_frameskip = getEmaxFrameskip();
            if(eu_info->x_frameskip < max_frameskip){
                eu_info->x_frameskip++;
            }else{
                eu_info->x_frameskip = 0u;
            }
        }

    //EspriteCollision(eu_info); 
        UINT8 scroll_e_tile;
        Sprite* iespr;
        SPRITEMANAGER_ITERATE(scroll_e_tile, iespr) {
            if(CheckCollision(THIS, iespr)) {
                switch(iespr->type){
                    case SpriteMotherpl://io enemy ho colpito motherpl
                        if(motherpl_state == MOTHERPL_DASH){
                            switch(THIS->type){
                                case SpriteEnemysimplesnake:
                                case SpriteEnemysimplerat:
                                    changeEstate(THIS, ENEMY_HIT_1);
                                    return;
                                break;
                            }
                        } else if(eu_info->hp > 0 && eu_info->e_state != ENEMY_UPSIDEDOWN 
                            && eu_info->e_state != ENEMY_HIT_1 
                            && eu_info->e_state != ENEMY_HIT_2
                            && eu_info->e_state != ENEMY_DEAD){
                            motherpl_hitted(THIS);
                            changeEstate(THIS, ENEMY_WAIT);
                        }
                    break;
                }
            }
        };
    //EstateBehavior(eu_info);
        switch(eu_info->e_state){
            case ENEMY_DEAD:
                eu_info->wait--;
                //THIS->y--;
                if(e_v_coll != 0){
                    if(eu_info->wait % 8 == 0){
                        THIS->y+=2;
                    }
                }
                if(eu_info->wait == 0u){
                    Edestroy();
                }
                return;
            break;
            case ENEMY_HIT_1:
            case ENEMY_HIT_2:
            case ENEMY_WAIT:
                if(eu_info->wait){eu_info->wait--;}
                else if(eu_info->hp > 0){changeEstate(THIS, ENEMY_WALK);}
                else{changeEstate(THIS, ENEMY_DEAD);}
                return;
            break;
            case ENEMY_WALK:
                eu_info->wait--;
                {
                    INT16 distance = THIS->x - s_motherpl->x;
                    if(THIS->mirror == NO_MIRROR && distance > 80 && eu_info->wait < 10u){
                        ETurn(eu_info->vx);
                        return;
                    }else if( THIS->mirror == V_MIRROR && distance < -80 && eu_info->wait < 10u){
                        ETurn(eu_info->vx);
                        return;
                    }
                }
                if(eu_info->wait == 0u){
                    switch(THIS->type){
                        case SpriteEnemyAttackerCobra:
                        case SpriteEnemyAttackerPine:
                        case SpriteEnemyThrowerSpider:
                        case SpriteEnemyThrowerTarantula:
                        case SpriteEnemyThrowerScorpion:
                            changeEstate(THIS, ENEMY_PREATTACK);
                        break;
                    }
                }
            break;
            case ENEMY_TREMBLING:
                {
                    INT8 tremble = 0;
                    if(eu_info->wait % 4 == 0){
                        tremble = -2;
                    }else if(eu_info->wait % 4 == 0){
                        tremble = 2;
                    }
                    THIS->y += tremble;
                }
            case ENEMY_PREATTACK:
                eu_info->wait--;
                if(eu_info->wait == 0u){
                    switch(THIS->type){
                        case SpriteEnemyAttackerPine:
                        case SpriteEnemyAttackerCobra:
                            changeEstate(THIS, ENEMY_ATTACK);
                        break;
                        case SpriteEnemyThrowerSpider:
                        case SpriteEnemyThrowerTarantula:
                        case SpriteEnemyThrowerScorpion:
                            changeEstate(THIS, ENEMY_THROW);
                        break;
                    }
                }
            break;
            case ENEMY_ATTACK:
                eu_info->wait--;
                if(eu_info->wait == 0u){changeEstate(THIS, ENEMY_WALK);}
            break;
            case ENEMY_THROW:
                eu_info->wait--;
                if(eu_info->wait == 0u){changeEstate(THIS, ENEMY_WAIT);}
            break;
            case ENEMY_UPSIDEDOWN:
                eu_info->wait--;
                if(eu_info->wait == 0u){
                    if(eu_info->vx >= 0){
                        THIS->mirror = NO_MIRROR;
                    }else{
                        THIS->mirror = V_MIRROR;
                    }
                    //changeEstate(ENEMY_WAIT);
                }
                return;
            break;
        }
}

void Estart() BANKED{
    THIS->lim_x = 88u;
    THIS->lim_y = 8u;
    UINT8 i = 0u;    
    for(i = 0u; i < 3u; ++i){
        UINT8 j = 0u;    
        for(j = 0u; j < 3u; ++j){
            if(e_to_reload[j].type == THIS->type && e_to_reload[j].alive == 1u){
                j = 4u;
            }
        }
        if(e_to_reload[i].alive == 0u && j == 3u){
            e_to_reload[i].x = THIS->x;
            e_to_reload[i].y = THIS->y;
            e_to_reload[i].type = THIS->type;
            e_to_reload[i].alive = 1u;
        }
    }
    enemy_counter++;
    struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
    eu_info->configured = 1u;
    eu_info->wait = 40u;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(THIS,1);
    }
}

void Econfiguration() BANKED{
    struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
    if(eu_info->configured == 1u){
        configure();
        return;
    }
}

UINT8 getEmaxFrameskip() BANKED{
    UINT8 result = 0u;
    switch(THIS->type){
        case SpriteEnemysimplesnake:
        case SpriteEnemysimplerat:
            result = E_FRAMSKIP_SNAKE;
        break;
        case SpriteEnemyAttackerCobra:
            result = E_FRAMSKIP_COBRA;
        break;
        case SpriteEnemyAttackerPine:
            result = E_FRAMSKIP_PINE;
        break;
        case SpriteEnemyThrowerSpider:
        case SpriteEnemyThrowerTarantula:
        case SpriteEnemyThrowerScorpion:
            result = E_FRAMSKIP_SPIDER;
        break;
    }
    return result;
}

void ETurn(UINT8 e_vx){
    struct EnemyData* e_info = (struct EnemyData*) THIS->custom_data;
    if(e_info->vx > 0){
        THIS->mirror = V_MIRROR;
        THIS->x--;
    }else{
        THIS->mirror = NO_MIRROR;
        THIS->x++;
    }
    e_info->vx = -e_vx;
    if(THIS->type != SpriteEnemyThrowerScorpion && 
        THIS->type != SpriteEnemyThrowerTarantula &&
        THIS->type != SpriteEnemyThrowerScorpion){
        e_info->wait += 32u;
    }
    if(e_info->e_state != ENEMY_PREATTACK || e_info->e_state != ENEMY_THROW){
        changeEstate(THIS, ENEMY_WAIT);
    }
}

void configure() BANKED{
    struct EnemyData* e_info = (struct EnemyData*) THIS->custom_data;
    e_info->x_frameskip = 1u;
    switch(THIS->type){
        case SpriteEnemysimplesnake:
            e_info->x_frameskip = 0u;
            e_info->hp = 1;
        break;
        case SpriteEnemysimplerat:
        case SpriteEnemyAttackerCobra:
        case SpriteEnemyAttackerPine:
            e_info->hp = 2;
        break;
        case SpriteEnemyThrowerSpider:
        case SpriteEnemyThrowerTarantula:
            e_info->hp = 3;
        break;
        case SpriteEnemyThrowerScorpion:
            e_info->hp = 5;
        break;
    }
    e_info->vx = E_VX;
    e_info->configured = 2u;
    e_info->e_state =  ENEMY_IDLE;
    changeEstate(THIS, ENEMY_WALK);
    if(s_motherpl->x < THIS->x){
        ETurn(E_VX);
    }
}

void changeEstate(Sprite* s_enemy, ENEMY_STATE new_e_state) BANKED{
    struct EnemyData* e_info = (struct EnemyData*) s_enemy->custom_data;
    if((new_e_state == ENEMY_HIT_1 || new_e_state == ENEMY_HIT_2)
         && e_info->e_state == ENEMY_ATTACK){
        return;//Invulnerabilità durante l' attacco!
    }
    if(e_info->e_state != new_e_state && e_info->e_state != ENEMY_DEAD){
        switch(new_e_state){
            case ENEMY_WALK:
                switch(THIS->type){
                    case SpriteEnemyAttackerCobra:
                        e_info->x_frameskip = E_FRAMSKIP_COBRA;
                        e_info->wait = enemy_random_30_100 + 100u;
                    break;
                    case SpriteEnemyAttackerPine:                    
                        e_info->x_frameskip = E_FRAMSKIP_PINE;
                        e_info->wait = enemy_random_30_100 + 60u;
                    break;
                    case SpriteEnemyThrowerSpider:
                    case SpriteEnemyThrowerTarantula:
                        e_info->x_frameskip = E_FRAMSKIP_SPIDER;
                        e_info->wait = enemy_random_30_100 + 60u;
                    break;
                    case SpriteEnemyThrowerScorpion:
                        e_info->x_frameskip = E_FRAMSKIP_SPIDER;
                        e_info->wait = enemy_random_30_100;
                    break;
                }
            break;
            case ENEMY_IDLE:
            break;
            case ENEMY_WAIT:
                e_info->wait = 40u;
            break;
            case ENEMY_HIT_2:
                my_play_fx(CHANNEL_2, 60, 0x09, 0x52, 0x36, 0x87, 0x00);//SFX_ENEMY_HIT
                e_info->hp-=2;
                if(e_info->hp <= 0u){
                    changeEstate(THIS, ENEMY_DEAD);
                    return;
                }else{
                    e_info->wait = 32u;
                    TranslateSprite(THIS, 0, -6 << delta_time);
                }
            break;
            case ENEMY_HIT_1:
                my_play_fx(CHANNEL_2, 60, 0x09, 0x52, 0x36, 0x87, 0x00);//SFX_ENEMY_HIT
                e_info->hp--;
                if(e_info->hp <= 0u){
                    changeEstate(THIS, ENEMY_DEAD);
                    return;
                }else{
                    if(THIS->type == SpriteEnemyAttackerCobra){
                        e_info->wait = 16u;
                    }else{
                        e_info->wait = 24u;
                    }
                    TranslateSprite(THIS, 0, -6 << delta_time);
                }
            break;
            case ENEMY_DEAD:
                if(e_info->vx < 0){
                    THIS->mirror = HV_MIRROR;
                }else{
                    THIS->mirror = H_MIRROR;
                }
                TranslateSprite(THIS, 0, -10 << delta_time);
                if(e_info->configured == 2u){
                    EspawnItem();
                }
                e_info->wait = 60u;
            break;
            case ENEMY_TREMBLING:
                e_info->wait = 80u;
            break;
            case ENEMY_PREATTACK:
                if((THIS->x < s_motherpl->x && e_info->vx < 0) ||
                    (THIS->x > s_motherpl->x && e_info->vx > 0)){
                    ETurn(e_info->vx);
                }
                e_info->wait = 56u;
            break;
            case ENEMY_ATTACK:
                if(THIS->mirror == NO_MIRROR){e_info->vx = 2*E_VX;}
                else{e_info->vx = 2* (-E_VX);}
                e_info->wait = enemy_random_30_100;
                e_info->x_frameskip = 0u;
            break;
            case ENEMY_THROW:
                e_info->wait = 40u;
                if((THIS->x < s_motherpl->x && THIS->mirror == V_MIRROR)
                     || (THIS->x > s_motherpl->x && THIS->mirror == NO_MIRROR)){
                    ETurn(e_info->vx);
                }
                switch(THIS->type){
                    case SpriteEnemyThrowerSpider: EthrowWeb(e_info->e_state); break;
                    case SpriteEnemyThrowerTarantula: EthrowAcid(e_info->e_state); break;
                    case SpriteEnemyThrowerScorpion: EthrowProjectile(e_info->e_state); break;
                }
            break;
            case ENEMY_UPSIDEDOWN:
                e_info->wait = 72u;
                if(e_info->vx < 0){
                    THIS->mirror = HV_MIRROR;
                }else{
                    THIS->mirror = H_MIRROR;
                }
                if(e_info->configured == 2u){
                    EspawnItem();
                }
                TranslateSprite(THIS, 0, -10 << delta_time);
                if(e_info->hp <= 1){
                    e_info->wait = 0u;
                    changeEstate(THIS, ENEMY_DEAD);
                }
            break;
        }
        //UPDATE ANIMATION
        switch(THIS->type){
            case SpriteEnemysimplesnake: EsimpleSnakeAnim(new_e_state);  break;
            case SpriteEnemysimplerat: EsimpleRatAnim(new_e_state);  break;
            case SpriteEnemyAttackerCobra: EattackerCobraAnim(new_e_state); break;
            case SpriteEnemyAttackerPine: EattackerPineAnim(new_e_state); break;
            case SpriteEnemyThrowerSpider: EthrowerSpiderAnim(new_e_state); break;
            case SpriteEnemyThrowerTarantula: EthrowerTarantulaAnim(new_e_state); break;
            case SpriteEnemyThrowerScorpion: EthrowerScorpionAnim(new_e_state); break;
        }
        e_info->e_state = new_e_state;
    }
}

void EspawnItem() BANKED{
    struct EnemyData* e_info = (struct EnemyData*) THIS->custom_data;
    e_info->configured = 3u;
    //SPAWN ITEM
    INVITEMTYPE itemtype = INVITEM_MONEY;
    if(current_state == StateMine){
        if (enemy_random_30_100 < 50){
            itemtype = INVITEM_METAL;
        }else if (enemy_random_30_100 < 90){
            itemtype = INVITEM_WOOD;        
        }else{
            itemtype = INVITEM_HEART;        
        }
    }else if(current_state == StateScorpions){
        itemtype = INVITEM_HEART;
    }else{
        if(enemy_random_30_100 < 35){
            itemtype = INVITEM_HEART;
        }else if (enemy_random_30_100 < 50){
            itemtype = INVITEM_METAL;
        }else if (enemy_random_30_100 < 58){
            //itemtype = INVITEM_HEART;
        }else if (enemy_random_30_100 < 70){
            itemtype = INVITEM_WOOD;
        }else if (enemy_random_30_100 < 80){
            return;
        }
    }
    UINT16 quantity = 1u;        
    Sprite* reward = SpriteManagerAdd(SpriteItemspawned, THIS->x + 4u, THIS->y - 12u);
    struct ItemSpawned* reward_data = (struct ItemSpawned*) reward->custom_data;
    reward_data->itemtype = itemtype;
    reward_data->quantity = quantity;
    reward_data->equippable = is_item_equippable(itemtype);
    reward_data->configured = 1u;
}

void Edestroy() {
    if(enemy_counter > 0){
        enemy_counter--;
    }
    UINT8 i = 0u;
    for(i = 0; i < 3; ++i){
        if(e_to_reload[i].type == THIS->type){
            e_to_reload[i].alive = 0u;
            e_to_reload[i].type = 0u;
            e_to_reload[i].x = 0u;
            e_to_reload[i].y = 0u;
            i = 3u;
        }
    }
    SpriteManagerRemoveSprite(THIS);
}

void DESTROY(){
    struct EnemyData* e_info = (struct EnemyData*) THIS->custom_data;
    if(e_info->e_state != ENEMY_DEAD){
        e_info->e_state = ENEMY_DEAD;
        Edestroy();
    }
}