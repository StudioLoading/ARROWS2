#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

extern struct MirinoInfo* mirino_info;

const UINT8 target_palloncino_idle[] = {1, 0}; //The first number indicates the number of frames
const UINT8 target_axe_idle[] = {3, 1, 2, 3}; //The first number indicates the number of frames

void START(){
    struct TargetInfo* this_target_info = (struct TargetInfo*) THIS->custom_data;
    this_target_info->enabled = 0;
    this_target_info->target_state = TARGET_UNINIT;
    THIS->lim_y = 24u;
}

void UPDATE(){
    struct TargetInfo* this_target_info = (struct TargetInfo*) THIS->custom_data;
    switch(this_target_info->target_state){
        case TARGET_UNINIT:
            return;
        break;
        case TARGET_INIT_PALLONCINO:
            SetSpriteAnim(THIS, target_palloncino_idle, 4u);
            this_target_info->target_state = TARGET_IDLE_PALLONCINO;
        break;
        case TARGET_INIT_AXE:
            SetSpriteAnim(THIS, target_axe_idle, 16u);
            this_target_info->vx = -2;
            this_target_info->vy = -20;
            this_target_info->target_state = TARGET_IDLE_AXE;
        break;
        case TARGET_IDLE_PALLONCINO:    
            if(KEY_PRESSED(J_RIGHT)){
                this_target_info->vx = -1;
            }else if(KEY_PRESSED(J_LEFT)){
                this_target_info->vx = 1;
            }else if(KEY_RELEASED(J_RIGHT) || KEY_RELEASED(J_LEFT)){
                this_target_info->vx = 0;
            }
            TranslateSprite(THIS, this_target_info->vx << delta_time, this_target_info->vy << delta_time);
        break;
        case TARGET_IDLE_AXE:
            if(KEY_PRESSED(J_RIGHT) ){
                this_target_info->vx -= 1;
            }else if(KEY_PRESSED(J_LEFT)){
                this_target_info->vx += 1;
            }else if(KEY_RELEASED(J_RIGHT) || KEY_RELEASED(J_LEFT)){
                this_target_info->vx = 0;
            } 
            if(this_target_info->vy < 24) {
                this_target_info->vy += 1;
            }
            if(this_target_info->wait & 1){
                TranslateSprite(THIS, 0, (this_target_info->vy >> 3) << delta_time);	
            }else{
                TranslateSprite(THIS, this_target_info->vx, (this_target_info->vy >> 3) << delta_time);	
            }
        break;   
    }
    
    if(this_target_info->target_state == TARGET_STROKE){
        SpriteManagerRemoveSprite(THIS);
    }

	UINT8 scroll_t_tile;
	Sprite* taspr;
	
	//Check sprite collision platform/enemy
	SPRITEMANAGER_ITERATE(scroll_t_tile, taspr) {
		if(CheckCollision(THIS, taspr)) {
			switch (taspr->type){
				case SpriteMirino:
                    this_target_info->enabled = 1; 
                    if(mirino_info->on_target == 0){
                        mirino_info->on_target = 1; 
                        mirino_info->target = THIS;
                    }
                break;
            }
        }else if(mirino_info->on_target == 1 && mirino_info->target == THIS){
            if(taspr->type == SpriteMirino){
                this_target_info->enabled = 0;
                mirino_info->on_target = 0;
            }
        }
    }
}

void DESTROY(){
    SpriteManagerAdd(SpritePuff, THIS->x, THIS->y);
}