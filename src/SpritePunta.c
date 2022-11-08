#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern UINT16 BONUS_PUNTA_INIT_Y;
extern UINT16 BONUS_PUNTA_INIT_X;

const UINT8 punta_idle[] = {1, 0}; //The first number indicates the number of frames
const UINT8 punta_just_thrown[] = {1, 1}; //The first number indicates the number of frames
const UINT8 punta_going[] = {1, 2}; //The first number indicates the number of frames
const UINT8 punta_far[] = {1, 3}; //The first number indicates the number of frames

void START(){

}

void UPDATE(){
    struct PuntaInfo* this_punta_info = (struct PuntaInfo*) THIS->custom_data;
    switch(this_punta_info->punta_state){
        case IDLE:
            SetSpriteAnim(THIS, punta_idle, 4u);
            THIS->x = BONUS_PUNTA_INIT_X;
            THIS->y = BONUS_PUNTA_INIT_Y;
	        THIS->x -= 4u;
	        THIS->y += 7u;
            return;
        break;
        case JUST_THROWN://StateBonus mette questo stato
            SetSpriteAnim(THIS, punta_just_thrown, 4u);
            this_punta_info->vy = -3;
            TranslateSprite(THIS, 0 << delta_time, this_punta_info->vy << delta_time);
            if(THIS->y < (UINT16)15u<<3){
                SetSpriteAnim(THIS, punta_going, 4u);
                this_punta_info->punta_state = GOING;
                //this_punta_info->vy = -2;
            }
        break;
        case GOING:
            TranslateSprite(THIS, 0 << delta_time, this_punta_info->vy << delta_time);
            if(THIS->y < (UINT16)10u<<3){
                SetSpriteAnim(THIS, punta_far, 4u);
                this_punta_info->punta_state = FAR;
            }
        break;
        case FAR:
            TranslateSprite(THIS, 0 << delta_time, this_punta_info->vy << delta_time);
        break;
    }
    

	UINT8 scroll_p_tile;
	Sprite* puspr;
	
	//Check sprite collision platform/enemy
	SPRITEMANAGER_ITERATE(scroll_p_tile, puspr) {
		if(CheckCollision(THIS, puspr) && this_punta_info->punta_state != IDLE) {
            struct TargetInfo* target_info = 0;
			switch (puspr->type){
				case SpriteMirino:
                    this_punta_info->punta_state = IDLE;
                break;
                case SpriteTarget:
                    target_info = (struct TargetInfo*)puspr->custom_data;
                    if(target_info->enabled == 1 && target_info->target_state != TARGET_STROKE){
                        target_info->target_state = TARGET_STROKE;
                    }
                break;
            }
        }
    }
}

void DESTROY(){

}