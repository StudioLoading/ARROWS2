#include "Banks/SetAutoBank.h"

#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Sound.h"

#include "custom_datas.h"

#define SLOW_FRAMESKIPX 1
#define FAST_FRAMESKIPX 0
#define NORMAL_FRAMESKIPX 0//1

#define MAX_ARROWS_ONSCREEN 4

#define TILE_ARROW_SLOW 14
#define TILE_ARROW_FAST 19
#define TILE_ARROW_LEFT 17
#define TILE_ARROW_RIGHT 18

const UINT8 arrow_anim_normal[] = {1,0};
const UINT8 arrow_anim_perfo[] = {1,1};
const UINT8 arrow_anim_bastard[] = {1,2};

UINT8 arrows_onscreen = 0u;

extern struct EnemyData* crab_data;

extern void changeEstate(Sprite* s_enemy, ENEMY_STATE new_e_state) BANKED;
extern void my_play_fx(SOUND_CHANNEL c, UINT8 mute_frames, UINT8 s0, UINT8 s1, UINT8 s2, UINT8 s3, UINT8 s4) BANKED;
extern void spawnItem(INVITEMTYPE itemtype, UINT16 spawn_at_x, UINT16 spawn_at_y ) BANKED;

void START(){
    if(arrows_onscreen >= MAX_ARROWS_ONSCREEN){SpriteManagerRemoveSprite(THIS);return;}
    THIS->lim_x = 16u;
    arrows_onscreen++;
    struct ArrowData* arrow_data = (struct ArrowData*) THIS->custom_data;
    arrow_data->hit = 0u;
    arrow_data->configured = 0u;
    arrow_data->vx = 1;
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
                    arrow_data->arrow_fskipx_max = 0;
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
                my_play_fx(CHANNEL_2, 50, 0x83, 0x92, 0xBE, 0x83, 0x00);//SFX_ARROW_BOUNCE
                if(arrow_data->arrow_fskipx_max != SLOW_FRAMESKIPX){
                    arrow_data->vx = arrow_data->vx >> 1;
                    arrow_data->arrow_fskipx_max = SLOW_FRAMESKIPX;
                }
                if(arrow_data->vx < 0){THIS->x -= 8u;}
                else{THIS->x += 16u;}
            }
            else if(arrow_t_coll == TILE_ARROW_FAST){
                my_play_fx(CHANNEL_2, 50, 0x83, 0x92, 0xBE, 0x83, 0x00);//SFX_ARROW_BOUNCE
                if(arrow_data->arrow_fskipx_max != FAST_FRAMESKIPX){
                    arrow_data->vx = arrow_data->vx << 1;
                    arrow_data->arrow_fskipx_max = FAST_FRAMESKIPX;
                }
                if(arrow_data->vx < 0){THIS->x -= 8u;}
                else{THIS->x += 8u;}
            }
            else if(arrow_t_coll == TILE_ARROW_LEFT){
                my_play_fx(CHANNEL_2, 50, 0x83, 0x92, 0xBE, 0x83, 0x00);//SFX_ARROW_BOUNCE
                if(arrow_data->vx > 0){arrow_data->vx = -arrow_data->vx;}
                THIS->x -= 8u;
                THIS->y -= 4u;
            }
            else if(arrow_t_coll == TILE_ARROW_RIGHT){
                my_play_fx(CHANNEL_2, 50, 0x83, 0x92, 0xBE, 0x83, 0x00);//SFX_ARROW_BOUNCE
                if(arrow_data->vx < 0){arrow_data->vx = -arrow_data->vx;}
                THIS->x += 8u;
                THIS->y -= 4u;
            }else if(arrow_t_coll != 0){
                my_play_fx(CHANNEL_2, 50, 0x83, 0x92, 0xBE, 0x83, 0x00);//SFX_ARROW_BOUNCE
                SpriteManagerRemoveSprite(THIS);
            }
        }

    //SPRITE COLLISION
        UINT8 scroll_arr_tile;
        Sprite* iarrspr;
        struct EnemyData* enemysimple_info = 0u;
        SPRITEMANAGER_ITERATE(scroll_arr_tile, iarrspr) {
            if(CheckCollision(THIS, iarrspr)) {
                switch(iarrspr->type){
                    case SpriteEnemysimplesnake:
                    case SpriteEnemysimplerat:
                    case SpriteEnemyAttackerCobra:
                    case SpriteEnemyAttackerPine:
                    case SpriteEnemyThrowerSpider:
                    case SpriteEnemyThrowerTarantula:
                    case SpriteBosscrab:
                    //io freccia ho colpito enemy
                        switch(arrow_data->arrow_type){
                            case ARROW_NORMAL:
                                if(iarrspr->type != SpriteBosscrab){
                                    changeEstate(iarrspr, ENEMY_HIT_1);
                                }
                            break;
                            case ARROW_PERF:
                            case ARROW_BASTARD:
                                if(iarrspr->type != SpriteBosscrab){
                                    arrow_data->hit = 1u;
                                    changeEstate(iarrspr, ENEMY_HIT_2);
                                }else if(crab_data->e_state == ENEMY_IDLE){
                                    changeEstate(iarrspr, ENEMY_HIT_1);
                                }
                            break;
                        }
                    break;
                    case SpriteSuperstone:
                        SpriteManagerRemoveSprite(THIS);
                    break;
                    case SpriteLeaf:
                        if(arrow_data->arrow_type == ARROW_PERF){
                            arrow_data->hit = 1u;
                            SpriteManagerRemoveSprite(iarrspr);
                            spawnItem(INVITEM_MONEY, THIS->x + 8u, THIS->y);
                        }
                    break;
                }
            }
        };
}

void DESTROY(){
    UINT16 puff_x = THIS->x - 12u;
    struct ArrowData* arrow_data = (struct ArrowData*) THIS->custom_data;
    if(arrow_data->vx < 0){
        puff_x += 16u;
    }
    SpriteManagerAdd(SpritePuff, puff_x, THIS->y - 4u);
    arrow_data->arrow_type = ARROW_DESTROYED;
    arrows_onscreen--;
}