#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas.h"

#define MAX_X_FRAMESKIP 6
#define MAX2_X_FRAMESKIP 24

void START(){
	THIS->lim_x = 60u;
	THIS->lim_y = 0u;
    struct EnemyData* cloud_data = (struct EnemyData*) THIS->custom_data;
    cloud_data->configured = 0;
    cloud_data->x_frameskip = MAX_X_FRAMESKIP;
}

void UPDATE(){
    struct EnemyData* cloud_data = (struct EnemyData*) THIS->custom_data;
    if(cloud_data->x_frameskip > 0){
        cloud_data->x_frameskip--;
        if(cloud_data->x_frameskip == 0){
            switch(cloud_data->configured){
                case 1:
                    THIS->x--;                    
                    cloud_data->x_frameskip = MAX_X_FRAMESKIP;
                break;
                case 2:
                    THIS->x--;                    
                    cloud_data->x_frameskip = MAX2_X_FRAMESKIP;
                break;
                case 3:
                    THIS->x++;
                    cloud_data->x_frameskip = MAX_X_FRAMESKIP;
                break;
                case 4:
                    THIS->x++;
                    cloud_data->x_frameskip = MAX2_X_FRAMESKIP;
                break;
            }
        }
    }
}

void DESTROY(){

}