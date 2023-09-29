#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

const UINT8 a_dcursor_todo[] = {5, 0,0,0,1,1}; //The first number indicates the number of frames
const UINT8 a_dcursor_done[] = {1, 2}; //The first number indicates the number of frames

void START(){
	THIS->lim_x = 255u;
	THIS->lim_y = 255u;
    SetSpriteAnim(THIS, a_dcursor_todo, 16u);
	struct EnemyData* dcursor_data = (struct EnemyData*) THIS->custom_data;
	dcursor_data->configured = 0;
	dcursor_data->wait = 0;
}

void UPDATE(){
	struct EnemyData* dcursor_data = (struct EnemyData*) THIS->custom_data;
	if(dcursor_data->wait > 0){
		switch(dcursor_data->configured){
			case 0u:
    			SetSpriteAnim(THIS, a_dcursor_todo, 16u);
			break;
			case 1u:
    			SetSpriteAnim(THIS, a_dcursor_done, 1);
			break;
		}
		dcursor_data->wait = 0;
	}
    /*
struct EnemyData{
	INT8 hp;
	INT8 vx;
	UINT8 wait;
	UINT8 configured;*/

}

void DESTROY(){

}