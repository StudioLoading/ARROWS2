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

const UINT8 e_anim_hidden[] = {1, 0};

UINT8 enemy_random_30_100 = 30u;

extern UINT8 enemy_counter;
extern INT8 motherpl_hp;
extern Sprite* s_motherpl;
extern struct MotherplData* d_motherpl;
extern MOTHERPL_STATE motherpl_state;
extern UINT8 mapwidth;

void Estart(Sprite* s_enemy) BANKED;
void configure(Sprite* s_enemy) BANKED;
void ETurn(Sprite* s_enemy, INT8 e_vx) BANKED;
void changeEstate(Sprite* s_enemy, ENEMY_STATE new_e_state) BANKED;
UINT8 getEmaxFrameskip(Sprite* s_enemy) BANKED;
void Econfiguration(Sprite* s_enemy) BANKED;
void Emanagement(Sprite* s_enemy) BANKED;
void Edestroy(Sprite* s_enemy) BANKED;
void EspawnItem(Sprite* s_enemy) BANKED;
UINT8 is_item_equippable(INVITEMTYPE itemtype) BANKED;
void EUpdateAnimation(Sprite* s_enemy,  ENEMY_STATE new_e_state) BANKED;

extern void EsimpleSnakeAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED;
extern void EsimpleRatAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED;
extern void EattackerPineAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED;
extern void EattackerCobraAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED;
extern void EthrowerSpiderAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED;
extern void EthrowerTarantulaAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED;
extern void EthrowerScorpionAnim(Sprite* s_enemy, ENEMY_STATE estate) BANKED;
extern void EthrowWeb(ENEMY_STATE estate) BANKED;
extern void EthrowAcid(ENEMY_STATE estate) BANKED;
extern void EthrowProjectile(Sprite* s_enemy, ENEMY_STATE estate) BANKED;
extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;
extern void motherpl_hitted(Sprite* s_enemy) BANKED;
extern void enemy_death() BANKED;

void START(){
    Estart(THIS);
}

void UPDATE(){
    //configuration
        struct EnemyData* eu_info = (struct EnemyData*) THIS->custom_data;
        if(eu_info->configured == 0){
            return;
        }
        if(eu_info->configured == 1){
            Econfiguration(THIS);
            return;
        }
    //CHECK DEATH
        if(eu_info->hp <= 0){changeEstate(THIS, ENEMY_DEAD);}
    //MANAGEMENT
        Emanagement(THIS);
}

void Emanagement(Sprite* s_enemy) BANKED{
    //map screen limit
        if(s_enemy->x < 10u){
            s_enemy->x = 10u;
        }
        if(s_enemy->x > ((mapwidth << 3) - 16u)){
            s_enemy->x = (mapwidth << 3) - 16u;
        }
    struct EnemyData* eu_info = (struct EnemyData*) s_enemy->custom_data;
    //Force Animation if configured == 4, which means reloaded enemy
        if(eu_info->configured == 4){
            EUpdateAnimation(s_enemy, eu_info->e_state);
            eu_info->configured = 2;
        }
    //ErandomManagement();
        enemy_random_30_100++;
        if(enemy_random_30_100 >= 100u){enemy_random_30_100 = 30u;}
    //Egravity();
        UINT8 e_v_coll = TranslateSprite(s_enemy, 0, E_GRAVITY << delta_time);
    //EhorizontalTileCollision(eu_info);
        if(eu_info->x_frameskip == 0 && 
            (eu_info->e_state == ENEMY_HIT_1 ||
            eu_info->e_state == ENEMY_HIT_2 ||
            eu_info->e_state == ENEMY_DEAD)
            && s_enemy->type != SpriteEThrowerScorpion){
                INT8 hit_vx = -1;
                if(s_enemy->mirror != NO_MIRROR){
                    hit_vx = 1;
                }
                eu_info->et_collision = TranslateSprite(s_enemy, hit_vx << delta_time, 0);
        }
        if(eu_info->x_frameskip == 0 && 
            (eu_info->e_state == ENEMY_WALK ||
                (eu_info->e_state == ENEMY_ATTACK 
                && s_enemy->type != SpriteEThrowerSpider 
                && s_enemy->type != SpriteEThrowerTarantula
                && s_enemy->type != SpriteEThrowerTarantula
                )
            )
        ){//x_frameskip used            
            eu_info->et_collision = TranslateSprite(s_enemy, eu_info->vx << delta_time, 0);
            if(eu_info->et_collision){
                switch(eu_info->et_collision){
                    case 8u:
                    case 9u:
                        ETurn(s_enemy, E_VX);
                    break;
                }
            }
            //if(s_enemy->type != SpriteEsimplesnake){
                eu_info->x_frameskip = getEmaxFrameskip(s_enemy);//1u;
            //}
        }else{
            UINT8 max_frameskip = getEmaxFrameskip(s_enemy);
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
            if(CheckCollision(s_enemy, iespr)) {
                switch(iespr->type){
                    case SpriteMotherplarmor:
                    case SpriteMotherpl://io enemy ho colpito motherpl
                        if(motherpl_state == MOTHERPL_DASH || iespr->type == SpriteMotherplarmor){
                            switch(s_enemy->type){
                                case SpriteEsimplesnake:
                                case SpriteEsimplerat:
                                    changeEstate(s_enemy, ENEMY_HIT_1);
                                    return;
                                break;
                                case SpriteEThrowerScorpion:
                                    motherpl_hitted(s_enemy);
                                    changeEstate(s_enemy, ENEMY_WAIT);
                                break;
                            }
                        } else if(eu_info->hp > 0 && eu_info->e_state != ENEMY_UPSIDEDOWN 
                            && eu_info->e_state != ENEMY_HIT_1 
                            && eu_info->e_state != ENEMY_HIT_2
                            && eu_info->e_state != ENEMY_DEAD){
                            motherpl_hitted(s_enemy);
                            changeEstate(s_enemy, ENEMY_WAIT);
                        }
                    break;
                }
            }
        };
    //EstateBehavior(eu_info);
        switch(eu_info->e_state){
            case ENEMY_DEAD:
                eu_info->wait--;
                //s_enemy->y--;
                if(e_v_coll != 0){
                    if(eu_info->wait % 8 == 0){
                        s_enemy->y+=2;
                    }
                }
                if(eu_info->wait == 0u){
                    Edestroy(s_enemy);
                }
                return;
            break;
            case ENEMY_WAIT:
                if(eu_info->wait){eu_info->wait--;}
                else if(eu_info->hp > 0){
                    changeEstate(s_enemy, ENEMY_WALK);
                }else{changeEstate(s_enemy, ENEMY_DEAD);}
                return;
            break;
            case ENEMY_HIT_1:
            case ENEMY_HIT_2:
                if(eu_info->wait){eu_info->wait--;}
                else if(eu_info->hp > 0){
                    changeEstate(s_enemy, ENEMY_WALK);
                }else{changeEstate(s_enemy, ENEMY_DEAD);}
                return;
            break;
            case ENEMY_WALK:
                eu_info->wait--;
                {
                    INT16 distance = s_enemy->x - s_motherpl->x;
                    if(s_enemy->mirror == NO_MIRROR && distance > 80 && eu_info->wait < 10u){
                        ETurn(s_enemy, eu_info->vx);
                        return;
                    }else if( s_enemy->mirror == V_MIRROR && distance < -80 && eu_info->wait < 10u){
                        ETurn(s_enemy, eu_info->vx);
                        return;
                    }
                }
                if(eu_info->wait == 0u){
                    switch(s_enemy->type){
                        case SpriteEAttackerCobra:
                        case SpriteEAttackerPine:
                        case SpriteEThrowerSpider:
                        case SpriteEThrowerTarantula:
                        case SpriteEThrowerScorpion:
                            changeEstate(s_enemy, ENEMY_PREATTACK);
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
                    s_enemy->y += tremble;
                }
            case ENEMY_PREATTACK:
                eu_info->wait--;
                if(eu_info->wait == 0u){
                    switch(s_enemy->type){
                        case SpriteEAttackerPine:
                        case SpriteEAttackerCobra:
                            changeEstate(s_enemy, ENEMY_ATTACK);
                        break;
                        case SpriteEThrowerSpider:
                        case SpriteEThrowerTarantula:
                        case SpriteEThrowerScorpion:
                            changeEstate(s_enemy, ENEMY_THROW);
                        break;
                    }
                }
            break;
            case ENEMY_ATTACK:
                eu_info->wait--;
                if(eu_info->wait == 0u){changeEstate(s_enemy, ENEMY_WALK);}
            break;
            case ENEMY_THROW:
                eu_info->wait--;
                if(eu_info->wait == 0u){changeEstate(s_enemy, ENEMY_WAIT);}
            break;
            case ENEMY_UPSIDEDOWN:
                eu_info->wait--;
                if(eu_info->wait == 0u){
                    if(eu_info->vx >= 0){
                        s_enemy->mirror = NO_MIRROR;
                    }else{
                        s_enemy->mirror = V_MIRROR;
                    }
                    //changeEstate(ENEMY_WAIT);
                }
                return;
            break;
        }
}

void Estart(Sprite* s_enemy) BANKED{
    s_enemy->lim_x = 88u;
    s_enemy->lim_y = 8u;
    enemy_counter++;
    struct EnemyData* eu_info = (struct EnemyData*) s_enemy->custom_data;
    eu_info->configured = 1u;
    eu_info->wait = 40u;
    if(_cpu != CGB_TYPE){
        OBP1_REG = PAL_DEF(0, 0, 1, 3);
        SPRITE_SET_PALETTE(s_enemy,1);
    }
}

void Econfiguration(Sprite* s_enemy) BANKED{
    struct EnemyData* eu_info = (struct EnemyData*) s_enemy->custom_data;
    if(eu_info->configured == 1u){
        configure(s_enemy);
        return;
    }
}

UINT8 getEmaxFrameskip(Sprite* s_enemy) BANKED{
    UINT8 result = 0u;
    switch(s_enemy->type){
        case SpriteEsimplesnake:
        case SpriteEsimplerat:
            result = E_FRAMSKIP_SNAKE;
        break;
        case SpriteEAttackerCobra:
            result = E_FRAMSKIP_COBRA;
        break;
        case SpriteEAttackerPine:
            result = E_FRAMSKIP_PINE;
        break;
        case SpriteEThrowerSpider:
        case SpriteEThrowerTarantula:
        case SpriteEThrowerScorpion:
            result = E_FRAMSKIP_SPIDER;
        break;
    }
    return result;
}

void ETurn(Sprite* s_enemy, INT8 e_vx) BANKED{
    struct EnemyData* e_info = (struct EnemyData*) s_enemy->custom_data;
    if(e_info->vx > 0){
        s_enemy->mirror = V_MIRROR;
        s_enemy->x--;
    }else{
        s_enemy->mirror = NO_MIRROR;
        s_enemy->x++;
    }
    e_info->vx = -e_vx;
    if(s_enemy->type != SpriteEThrowerScorpion && 
        s_enemy->type != SpriteEThrowerTarantula &&
        s_enemy->type != SpriteEThrowerScorpion){
        e_info->wait += 32u;
    }
    if(e_info->e_state != ENEMY_PREATTACK || e_info->e_state != ENEMY_THROW){
        changeEstate(s_enemy, ENEMY_WAIT);
    }
}

void configure(Sprite* s_enemy) BANKED{
    struct EnemyData* e_info = (struct EnemyData*) s_enemy->custom_data;
    e_info->x_frameskip = 1u;
    switch(s_enemy->type){
        case SpriteEsimplesnake:
            e_info->x_frameskip = 0u;
            e_info->hp = 1;
        break;
        case SpriteEsimplerat:
        case SpriteEAttackerCobra:
        case SpriteEAttackerPine:
            e_info->hp = 2;
        break;
        case SpriteEThrowerSpider:
        case SpriteEThrowerTarantula:
            e_info->hp = 3;
        break;
        case SpriteEThrowerScorpion:
            e_info->hp = 5;
        break;
    }
    e_info->vx = E_VX;
    e_info->configured = 2u;
    e_info->e_state =  ENEMY_IDLE;
    changeEstate(s_enemy, ENEMY_WALK);
    if(s_motherpl->x < s_enemy->x){
        ETurn(s_enemy, E_VX);
    }
}

void changeEstate(Sprite* s_enemy, ENEMY_STATE new_e_state) BANKED{
    struct EnemyData* e_info = (struct EnemyData*) s_enemy->custom_data;
    //se è stato colpito e (lo è stato già oppure sta attancando)
    if((new_e_state == ENEMY_HIT_1 || new_e_state == ENEMY_HIT_2) && 
        (e_info->e_state == ENEMY_HIT_2 || e_info->e_state == ENEMY_HIT_1 || 
        e_info->e_state == ENEMY_ATTACK)){
        return;//Invulnerabilità durante hit e attacco!
    }
    if(e_info->e_state != new_e_state && e_info->e_state != ENEMY_DEAD){
        switch(new_e_state){
            case ENEMY_WALK:
                switch(s_enemy->type){
                    case SpriteEAttackerCobra:
                        e_info->x_frameskip = E_FRAMSKIP_COBRA;
                        e_info->wait = enemy_random_30_100 + 100u;
                    break;
                    case SpriteEAttackerPine:                    
                        e_info->x_frameskip = E_FRAMSKIP_PINE;
                        e_info->wait = enemy_random_30_100 + 60u;
                    break;
                    case SpriteEThrowerSpider:
                    case SpriteEThrowerTarantula:
                        e_info->x_frameskip = E_FRAMSKIP_SPIDER;
                        e_info->wait = enemy_random_30_100 + 60u;
                    break;
                    case SpriteEThrowerScorpion:
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
                    changeEstate(s_enemy, ENEMY_DEAD);
                    return;
                }else if(s_enemy->type == SpriteEThrowerScorpion){
                    e_info->wait = 80u;
                    TranslateSprite(s_enemy, 0, -6 << delta_time);
                }else{
                    e_info->wait = 32u;
                    TranslateSprite(s_enemy, 0, -6 << delta_time);
                }
            break;
            case ENEMY_HIT_1:
                my_play_fx(CHANNEL_2, 60, 0x09, 0x52, 0x36, 0x87, 0x00);//SFX_ENEMY_HIT
                e_info->hp--;
                if(e_info->hp <= 0u){
                    changeEstate(s_enemy, ENEMY_DEAD);
                    return;
                }else{
                    e_info->wait = 24u;
                    if(s_enemy->type == SpriteEThrowerScorpion){
                        e_info->wait = 40u;
                    }
                    TranslateSprite(s_enemy, 0, -6 << delta_time);
                }
            break;
            case ENEMY_DEAD:
                if(e_info->vx < 0){
                    s_enemy->mirror = HV_MIRROR;
                }else{
                    s_enemy->mirror = H_MIRROR;
                }
                TranslateSprite(s_enemy, 0, -10 << delta_time);
                if(e_info->configured == 2u){
                    EspawnItem(s_enemy);
                }
                e_info->wait = 60u;
            break;
            case ENEMY_TREMBLING:
                e_info->wait = 80u;
            break;
            case ENEMY_PREATTACK:
                if((s_enemy->x < s_motherpl->x && e_info->vx < 0) ||
                    (s_enemy->x > s_motherpl->x && e_info->vx > 0)){
                    ETurn(s_enemy, e_info->vx);
                }
                e_info->wait = 56u;
            break;
            case ENEMY_ATTACK:
                if(s_enemy->mirror == NO_MIRROR){e_info->vx = 2*E_VX;}
                else{e_info->vx = 2* (-E_VX);}
                e_info->wait = enemy_random_30_100;
                e_info->x_frameskip = 0u;
            break;
            case ENEMY_THROW:
                e_info->wait = 40u;
                if((s_enemy->x < s_motherpl->x && s_enemy->mirror == V_MIRROR)
                     || (s_enemy->x > s_motherpl->x && s_enemy->mirror == NO_MIRROR)){
                    ETurn(s_enemy, e_info->vx);
                }
                switch(s_enemy->type){
                    case SpriteEThrowerSpider: EthrowWeb(e_info->e_state); break;
                    case SpriteEThrowerTarantula: EthrowAcid(e_info->e_state); break;
                    case SpriteEThrowerScorpion: EthrowProjectile(s_enemy, e_info->e_state); break;
                }
            break;
            case ENEMY_UPSIDEDOWN:
                e_info->wait = 72u;
                if(e_info->vx < 0){
                    s_enemy->mirror = HV_MIRROR;
                }else{
                    s_enemy->mirror = H_MIRROR;
                }
                if(e_info->configured == 2u){
                    EspawnItem(s_enemy);
                }
                TranslateSprite(s_enemy, 0, -10 << delta_time);
                if(e_info->hp <= 1){
                    e_info->wait = 0u;
                    changeEstate(s_enemy, ENEMY_DEAD);
                }
            break;
        }
        //UPDATE ANIMATION
            EUpdateAnimation(s_enemy, new_e_state);
        e_info->e_state = new_e_state;
    }
}

void EUpdateAnimation(Sprite* s_enemy,  ENEMY_STATE new_e_state) BANKED{
    switch(s_enemy->type){
        case SpriteEsimplesnake: EsimpleSnakeAnim(s_enemy, new_e_state);  break;
        case SpriteEsimplerat: EsimpleRatAnim(s_enemy, new_e_state);  break;
        case SpriteEAttackerCobra: EattackerCobraAnim(s_enemy, new_e_state); break;
        case SpriteEAttackerPine: EattackerPineAnim(s_enemy, new_e_state); break;
        case SpriteEThrowerSpider: EthrowerSpiderAnim(s_enemy, new_e_state); break;
        case SpriteEThrowerTarantula: EthrowerTarantulaAnim(s_enemy, new_e_state); break;
        case SpriteEThrowerScorpion: EthrowerScorpionAnim(s_enemy, new_e_state); break;
    }
}

void EspawnItem(Sprite* s_enemy) BANKED{
    struct EnemyData* e_info = (struct EnemyData*) s_enemy->custom_data;
    e_info->configured = 3u;
    //SPAWN ITEM
    INVITEMTYPE itemtype = INVITEM_MONEY;
    if(current_state == StateMine){
        if (enemy_random_30_100 < 60){
            itemtype = INVITEM_METAL;
        }else if (enemy_random_30_100 < 90){
            itemtype = INVITEM_WOOD;        
        }else{
            itemtype = INVITEM_HEART;        
        }
    }else if(current_state == StateScorpions){
        enemy_death();
        if(motherpl_hp < 3){
            itemtype = INVITEM_HEART;
        }else{
            if (enemy_random_30_100 < 60){
                itemtype = INVITEM_METAL;
            }else if (enemy_random_30_100 < 90){
                itemtype = INVITEM_MONEY;        
            }else{
                itemtype = INVITEM_HEART;
            }
        }
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
    Sprite* reward = SpriteManagerAdd(SpriteItemspawned, s_enemy->x + 4u, s_enemy->y - 12u);
    struct ItemSpawned* reward_data = (struct ItemSpawned*) reward->custom_data;
    reward_data->itemtype = itemtype;
    reward_data->quantity = quantity;
    reward_data->equippable = is_item_equippable(itemtype);
    reward_data->configured = 1u;
}

void Edestroy(Sprite* s_enemy) BANKED{
    if(enemy_counter > 0){
        enemy_counter--;
    }
    SpriteManagerRemoveSprite(s_enemy);
}

void DESTROY(){
    struct EnemyData* e_info = (struct EnemyData*) THIS->custom_data;
    if(e_info->e_state != ENEMY_DEAD){
        e_info->e_state = ENEMY_DEAD;
        Edestroy(THIS);
    }
}