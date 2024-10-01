#include "Banks/SetAutoBank.h"

#include "main.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "custom_datas_tetra.h"

#define COUNTDOWN 120


const UINT8 dado_invisible[] = {1, 4};//The first number indicates the number of frames
const UINT8 dado_anticipation[] = {1, 4};//The first number indicates the number of frames
const UINT8 dado_waiting[] = {1, 0};//The first number indicates the number of frames
const UINT8 dado_superfast[] = {4, 0, 1, 2, 3}; //The first number indicates the number of frames
const UINT8 dado_fast[] = {4, 0, 1, 2, 3}; //The first number indicates the number of frames
const UINT8 dado_slow[] = {4, 0, 1, 2, 3}; //The first number indicates the number of frames

const UINT8 dado_faccia1[] = {1, 5}; //The first number indicates the number of frames
const UINT8 dado_faccia2[] = {1, 6}; //The first number indicates the number of frames
const UINT8 dado_faccia3[] = {1, 7}; //The first number indicates the number of frames
const UINT8 dado_faccia4[] = {1, 8}; //The first number indicates the number of frames

const UINT8 dado_faccia1_selected[] = {1, 9}; //The first number indicates the number of frames
const UINT8 dado_faccia2_selected[] = {1, 10}; //The first number indicates the number of frames
const UINT8 dado_faccia3_selected[] = {1, 11}; //The first number indicates the number of frames
const UINT8 dado_faccia4_selected[] = {1, 12}; //The first number indicates the number of frames

void START(){
    THIS->lim_x = 255u;
    THIS->lim_y = 255u;
    SetSpriteAnim(THIS, dado_waiting, 4u);
    struct TetradadoInfo* dado_info = (struct TetradadoInfo*) THIS->custom_data;
    dado_info->tetradado_state = DADO_WAITING;
    dado_info->face_counter = 0u;
    dado_info->rolling_counter = 0u;
    dado_info->initial_x = THIS->x;
    dado_info->initial_y = THIS->y;
}

void UPDATE(){
    struct TetradadoInfo* dado_info = (struct TetradadoInfo*) THIS->custom_data;
    switch(dado_info->tetradado_state){
        case DADO_INVISIBLE:
            //TODO vai alle sue initial e stai invisibile sull' animazione
            SetSpriteAnim(THIS, dado_invisible, 4u);
            THIS->x = dado_info->initial_x;
            THIS->y = dado_info->initial_y;
        break;
        case DADO_WAITING:
            SetSpriteAnim(THIS, dado_waiting, 4u);
            dado_info->face_counter = 0u;
            dado_info->rolling_counter = 0u;
        break;
        case ROLLING_SUPERFAST:
            SetSpriteAnim(THIS, dado_superfast, 36u);
            dado_info->face_counter++;
        break;
        case ROLLING_FAST:
            SetSpriteAnim(THIS, dado_fast, 18u);
            dado_info->rolling_counter++;
            if(dado_info->rolling_counter == 120u){
                dado_info->tetradado_state=ROLLING_SLOW;
                SetSpriteAnim(THIS, dado_slow, 8u);
                dado_info->rolling_counter = 0u;
            }
        break;
        case ROLLING_SLOW:
            dado_info->rolling_counter++;
            if(dado_info->rolling_counter == 80u){
                dado_info->tetradado_state=ANTICIPATION;
                SetSpriteAnim(THIS, dado_anticipation, 4u);
                dado_info->rolling_counter = 0u;
            }
        break;
        case ANTICIPATION:
            dado_info->rolling_counter++;
            if(dado_info->rolling_counter == 60u){
                dado_info->tetradado_state=DADO_FACE;
                if(dado_info->face_counter > 200u){
                    dado_info->face_counter -= 200u;
                }else if(dado_info->face_counter > 100u){
                    dado_info->face_counter -= 100u;
                }
                dado_info->face_counter = dado_info->face_counter % 10;
                switch(dado_info->face_counter){
                    case 1u:
                    case 2u:
                    case 3u:
                        dado_info->tetradado_faccia = FACCIA_UP;
                        SetSpriteAnim(THIS, dado_faccia1, 4u);
                    break;
                    case 4u:
                    case 5u:
                        dado_info->tetradado_faccia =  FACCIA_BLAIR;
                        SetSpriteAnim(THIS, dado_faccia2, 4u);
                    break;
                    case 6u:
                    case 7u:
                    case 8u:
                        dado_info->tetradado_faccia =  FACCIA_ARROWS;
                        SetSpriteAnim(THIS, dado_faccia3, 4u);
                    break;
                    case 9u:
                    case 0u:
                        dado_info->tetradado_faccia =  FACCIA_SHIELD;
                        SetSpriteAnim(THIS, dado_faccia4, 4u);
                    break;
                }
                dado_info->rolling_counter = 0u;
            }
        break;
        case DADO_FACE:
            //valuto la dado_info->face_counter
            //e popolo il dado_info->tetradado_faccia
            //mostrango la faccia corrispondende
            //e metto dado_info->tetradado_state WAIT
        break;
        case DADO_SELECTED_PLAYER:
            switch(dado_info->tetradado_faccia){
                case FACCIA_UP:
                    SetSpriteAnim(THIS, dado_faccia1_selected, 4u);
                break;
                case FACCIA_BLAIR:
                    SetSpriteAnim(THIS, dado_faccia2_selected, 4u);
                break;
                case FACCIA_ARROWS:
                    SetSpriteAnim(THIS, dado_faccia3_selected, 4u);
                break;
                case FACCIA_SHIELD:
                    SetSpriteAnim(THIS, dado_faccia4_selected, 4u);
                break;
            }
        break;
        case DADO_SELECTED_ENEMY:
            switch(dado_info->tetradado_faccia){
                case FACCIA_UP:
                    SetSpriteAnim(THIS, dado_faccia1_selected, 4u);
                break;
                case FACCIA_BLAIR:
                    SetSpriteAnim(THIS, dado_faccia2_selected, 4u);
                break;
                case FACCIA_ARROWS:
                    SetSpriteAnim(THIS, dado_faccia3_selected, 4u);
                break;
                case FACCIA_SHIELD:
                    SetSpriteAnim(THIS, dado_faccia4_selected, 4u);
                break;
            }
        break;
        case PASSWORD:
            {
            switch(dado_info->tetradado_faccia){
                case FACCIA_UP:
                    SetSpriteAnim(THIS, dado_faccia1, 4u);
                break; 
                case FACCIA_BLAIR:
                    SetSpriteAnim(THIS, dado_faccia2, 4u);
                break; 
                case FACCIA_ARROWS:
                    SetSpriteAnim(THIS, dado_faccia3, 4u);
                break; 
                case FACCIA_SHIELD:
                    SetSpriteAnim(THIS, dado_faccia4, 4u);
                break; 
            }
            }
        break;
    }
}

void DESTROY(){
    
}