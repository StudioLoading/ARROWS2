#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define GATOR_IDLE_DISTANCE 200
#define GATOR_WALK_DISTANCE 250
#define GATOR_ATTACK_DISTANCE 40

const UINT8 a_gator_idle[] = {4, 1,1,2,1}; //The first number indicates the number of frames
const UINT8 a_gator_walk[] = {4, 7,8,7,7}; //The first number indicates the number of frames
const UINT8 a_gator_attack[] = {7, 6,5,4,3,6,6,6}; //The first number indicates the number of frames
const UINT8 a_gator_dead[] = {8, 9,10,9,10,9,10,11,0}; //The first number indicates the number of frames

void gator_change_state(Sprite* s_gator, ENEMY_STATE new_gator_state) BANKED;
void gator_turn() BANKED;

void START() {
    THIS->lim_x = 6000u;
    THIS->lim_y = 6000u;
	SetSpriteAnim(THIS, a_gator_idle, 24u);
	struct PlatformInfo* gator_platform = (struct PlatformInfo*)THIS->custom_data;
	gator_platform->configured = 0;
	gator_platform->step = 0;
    gator_platform->vx = 0;
	gator_platform->p_state = ENEMY_IDLE;
}

void UPDATE() {
	struct PlatformInfo* gator_platform = (struct PlatformInfo*)THIS->custom_data;
    if(gator_platform->configured == 0){
        gator_platform->distance--;
        if(gator_platform->distance == 0){gator_platform->configured = 1;}
        return;
    }
	switch(gator_platform->p_state){
		case ENEMY_IDLE:{
			gator_platform->distance = GATOR_IDLE_DISTANCE;
			UINT8 r = 0x1;//2;
			if (gator_platform->step < (gator_platform->distance/6) 
			|| gator_platform->step > (gator_platform->distance-(gator_platform->distance/6))){
				r=0x7;//8;
				//TURBINE
					if(gator_platform->step == 10){THIS->y -= gator_platform->vy;}
					if(gator_platform->step == (gator_platform->distance - 20)){THIS->y-=gator_platform->vy;}
					if(gator_platform->step == 0){THIS->y-=gator_platform->vy;}
					if(gator_platform->step 
					== (gator_platform->distance - (gator_platform->distance/20))){THIS->y-=gator_platform->vy;}
			}else if(gator_platform->step > ((gator_platform->distance>>1)-(gator_platform->distance/6) ) 
				&& gator_platform->step < ((gator_platform->distance>>1)+(gator_platform->distance/6) ) ){
				r=0x7;//8;		
				if(gator_platform->step == ((gator_platform->distance>>1) - 20)
					|| gator_platform->step == ((gator_platform->distance>>1) - 10)
					|| gator_platform->step == (gator_platform->distance>>1)
					|| gator_platform->step == ((gator_platform->distance>>1) + 10)
				){
					THIS->y+=gator_platform->vy;
				}
			}
			if ((gator_platform->step & r) == 0 && gator_platform->step != 0){
				if (gator_platform->step < (gator_platform->distance >> 1)){
					if(gator_platform->clockwise == 0){
						gator_platform->vx = 1;
					}else{
						gator_platform->vx = -1;
					}
				}else{		
					if(gator_platform->clockwise == 0){
						gator_platform->vx = -1;
					}else{
						gator_platform->vx = 1;
					}
				}
			}else{
				gator_platform->vx = 0;
			}
			//THIS->x += gator_platform->vx;
			//THIS->y += gator_platform->vy;
			THIS->x += gator_platform->vx;
			/*
			UINT8 coll = TranslateSprite(THIS, gator_platform->vx, 0);
			if(coll){
				SpriteManagerRemoveSprite(THIS);
			}
			*/
			gator_platform->step += 1;
			if (gator_platform->step >= gator_platform->distance){
				gator_platform->step = 0;
				//THIS->x = gator_platform->initx;		
			}
			gator_platform->plat_counter++;
			if(gator_platform->plat_counter == (THIS->x % 200)){
				if((gator_platform->plat_counter % 2) == 0){
					gator_change_state(THIS, ENEMY_WALK);
				}else{gator_change_state(THIS, ENEMY_ATTACK);}
			}
		}
		break;
		case ENEMY_WALK:
			gator_platform->plat_counter++;
			if((gator_platform->plat_counter % 16) == 0){
                SpriteManagerAdd(SpritePuntawater, THIS->x, THIS->y+1u);
			}
			if(THIS->x <= 130u && gator_platform->vx != 1){gator_turn();}
			else if(THIS->x >= 300u && gator_platform->vx != -1){gator_turn();}
			else if((gator_platform->plat_counter & 1) == 1){THIS->x += gator_platform->vx;}
			if(gator_platform->plat_counter == GATOR_WALK_DISTANCE){
				gator_change_state(THIS, ENEMY_ATTACK);
			}
			if(gator_platform->vx > 0 && THIS->mirror != V_MIRROR){
				THIS->mirror = V_MIRROR;
			}else if(gator_platform->vx < 0 && THIS->mirror != NO_MIRROR){
				THIS->mirror = NO_MIRROR;
			}
		break;
		case ENEMY_ATTACK:
			if(THIS->anim_frame == 6){
				gator_change_state(THIS, ENEMY_IDLE);
			}
		break;
		case ENEMY_DEAD:
			if(THIS->anim_frame == 7){
                SpriteManagerAdd(SpritePuntawater, THIS->x, THIS->y);
				SpriteManagerRemoveSprite(THIS);
			}
		break;
	}
}

void gator_turn() BANKED{
	struct PlatformInfo* gator_platform = (struct PlatformInfo*)THIS->custom_data;
	gator_platform->vx = -gator_platform->vx;
}

void gator_change_state(Sprite* s_gator, ENEMY_STATE new_gator_state) BANKED{
	struct PlatformInfo* gator_platform = (struct PlatformInfo*)s_gator->custom_data;
	switch(new_gator_state){
		case ENEMY_IDLE:
			SetSpriteAnim(s_gator, a_gator_idle, 24u);
		break;
		case ENEMY_WALK:		
			if(gator_platform->vx == 0){
				if((gator_platform->plat_counter % 2) == 0){
					gator_platform->vx = 1;
					THIS->mirror = V_MIRROR;
				}else{gator_platform->vx = -1;
					THIS->mirror = NO_MIRROR;}
			}
			SetSpriteAnim(s_gator, a_gator_walk, 16u);
		break;
		case ENEMY_ATTACK:
			SetSpriteAnim(s_gator, a_gator_attack, 12u);
		break;
		case ENEMY_DEAD:
			SetSpriteAnim(s_gator, a_gator_dead, 8u);
		break;
	}
	gator_platform->plat_counter = 0;
	gator_platform->p_state = new_gator_state;
}

void DESTROY() {
}
