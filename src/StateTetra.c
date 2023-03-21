#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Fade.h"

#include "TilesAnimations0.h"
#include "sgb_palette.h"
#include "custom_datas.h"

IMPORT_TILES(tilestetra);
IMPORT_TILES(font);

IMPORT_MAP(maptetra);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

UINT8 bank_tilestetra = BANK(tilestetra);

const UINT8 const collision_tiles_tetra[] = {0, 0};//numero delle tile con zero finale

Sprite* tacca11 = 0;
Sprite* tacca12 = 0;
Sprite* tacca13 = 0;
Sprite* tacca14 = 0;
Sprite* tacca21 = 0;
Sprite* tacca22 = 0;
Sprite* tacca23 = 0;
Sprite* tacca24 = 0;

Sprite* dado1 = 0;
Sprite* dado2 = 0;
Sprite* dado3 = 0;
Sprite* dado4 = 0;
Sprite* dado5 = 0;
Sprite* dado6 = 0;
struct TetradadoInfo* dado1_info = 0;
struct TetradadoInfo* dado2_info = 0;
struct TetradadoInfo* dado3_info = 0;
struct TetradadoInfo* dado4_info = 0;
struct TetradadoInfo* dado5_info = 0;
struct TetradadoInfo* dado6_info = 0;

Sprite* tetra_cursor = 0;
struct TetracursorInfo* tetra_cursor_info = 0;

INT8 next_dice = -1;

TETRA_GAME_STATE tetra_game_state = START_GAME;
TETRA_TURN tetraturn = TURN_PLAYER;

UINT8 dadi_disponibili = 6u;
UINT8 dado_scelto_da_ui = 1u;
UINT8 dado_scelto_da_ui_ok = 0u;

void START() {
	//INIT SOUND
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0xFF; //Max volume 0x77

  //SGB COLORS
  if(sgb_check()){
    set_sgb_palette01_worldmap();
  }
  InitScroll(BANK(maptetra), &maptetra, collision_tiles_tetra, 0);

  SHOW_BKG;

  SpriteManagerLoad(SpriteTetradado);
  tacca11 = SpriteManagerAdd(SpriteTetratacche, (UINT16) 12u, (UINT16) 2u << 3);
  tacca12 = SpriteManagerAdd(SpriteTetratacche, (UINT16) 44u, (UINT16) 2u << 3);
  tacca13 = SpriteManagerAdd(SpriteTetratacche, (UINT16) 76u, (UINT16) 2u << 3);
  tacca14 = SpriteManagerAdd(SpriteTetratacche, (UINT16) 108u, (UINT16) 2u << 3);
  /* UNCOMMENT
  tacca21 = SpriteManagerAdd(SpriteTetratacche, (UINT16) 12u, (UINT16) 26u << 3);
  tacca22 = SpriteManagerAdd(SpriteTetratacche, (UINT16) 44u, (UINT16) 26u << 3);
  tacca23 = SpriteManagerAdd(SpriteTetratacche, (UINT16) 76u, (UINT16) 26u << 3);
  tacca24 = SpriteManagerAdd(SpriteTetratacche, (UINT16) 108u, (UINT16) 26u << 3);
  */
  dado1 = SpriteManagerAdd(SpriteTetradado, (UINT16) 2u << 3, (UINT16) 13u << 3);
  dado2 = SpriteManagerAdd(SpriteTetradado, (UINT16) 3u << 3, (UINT16) 16u << 3);
  dado3 = SpriteManagerAdd(SpriteTetradado, (UINT16) 6u << 3, (UINT16) 14u << 3);
  dado4 = SpriteManagerAdd(SpriteTetradado, (UINT16) 10u << 3, (UINT16) 14u << 3);
  dado5 = SpriteManagerAdd(SpriteTetradado, (UINT16) 13u << 3, (UINT16) 16u << 3);
  dado6 = SpriteManagerAdd(SpriteTetradado, (UINT16) 16u << 3, (UINT16) 14u << 3);
  dado1_info = (struct TetradadoInfo*) dado1->custom_data;
  dado2_info = (struct TetradadoInfo*) dado2->custom_data;
  dado3_info = (struct TetradadoInfo*) dado3->custom_data;
  dado4_info = (struct TetradadoInfo*) dado4->custom_data;
  dado5_info = (struct TetradadoInfo*) dado5->custom_data;
  dado6_info = (struct TetradadoInfo*) dado6->custom_data;
  scroll_target = SpriteManagerAdd(SpriteTetracamera, (UINT16) 8u << 3, (UINT16) 2u << 3);
  tetra_cursor = SpriteManagerAdd(SpriteTetracursor, dado1->x, dado1->y - 8u);
  tetra_cursor_info = (struct TetracursorInfo*) tetra_cursor->custom_data;
  SHOW_SPRITES;
  
  next_dice = -1;
  dado_scelto_da_ui_ok = 0u;
  tetra_game_state = START_GAME;//INIT_GAME;
  tetraturn = TURN_PLAYER;
  dadi_disponibili = 6u;
  dado_scelto_da_ui = 1u;
}

void UPDATE(){
  if(KEY_PRESSED(J_DOWN) && KEY_PRESSED(J_START)){
    SetState(StateExzoo);
  }
  switch(tetra_game_state){
    case START_GAME:
    case TURN_MAKE_DICE:
      dadi_disponibili = 6u;
      if(next_dice == -1){
        if(KEY_TICKED(J_FIRE)){
          next_dice = 0;
          tetra_cursor_info->cursor_state = TRIANGLE_BLINK;
          dado1_info->tetradado_state = ROLLING_SUPERFAST;
          dado2_info->tetradado_state = ROLLING_SUPERFAST;
          dado3_info->tetradado_state = ROLLING_SUPERFAST;
          dado4_info->tetradado_state = ROLLING_SUPERFAST;
          dado5_info->tetradado_state = ROLLING_SUPERFAST;
          dado6_info->tetradado_state = ROLLING_SUPERFAST;
        }
      }else if(next_dice < 7){
        if(KEY_TICKED(J_FIRE)){
          next_dice++;
          switch(next_dice){
            case 1:
              tetra_cursor->x = dado2->x;
              tetra_cursor->y = dado2->y - 8u;
              dado1_info->tetradado_state = ROLLING_FAST;
            break;
            case 2:
              tetra_cursor->x = dado3->x;
              tetra_cursor->y = dado3->y - 8u;
              dado2_info->tetradado_state = ROLLING_FAST;
            break;
            case 3:
              tetra_cursor->x = dado4->x;
              tetra_cursor->y = dado4->y - 8u;
              dado3_info->tetradado_state = ROLLING_FAST;
            break;
            case 4:
              tetra_cursor->x = dado5->x;
              tetra_cursor->y = dado5->y - 8u;
              dado4_info->tetradado_state = ROLLING_FAST;
            break;
            case 5:
              tetra_cursor->x = dado6->x;
              tetra_cursor->y = dado6->y - 8u;
              dado5_info->tetradado_state = ROLLING_FAST;
            break;
            case 6:            
              tetra_game_state = TURN_PICK_DICE;
              tetra_cursor->x = dado1->x;
              tetra_cursor->y = dado1->y + 16u;
              tetra_cursor_info->cursor_state = HAND_OPENED;
              dado6_info->tetradado_state = ROLLING_FAST;
            break;
          }
        }
      }
    break;
    case TURN_PICK_DICE:
      if(tetraturn == TURN_ENEMY){
        //praticamente codo la parte di scelta enemy del dado,
        if(dadi_disponibili == 0u){
          //passo alla fase successiva della partita
          tetra_game_state = TURN_GIVE_DICE;
          return;
        }
        //scelgo il dado
        dado_scelto_da_ui += tetra_cursor->x;
        dado_scelto_da_ui += scroll_target->y;
        dado_scelto_da_ui = dado_scelto_da_ui % 6;
        while (dado_scelto_da_ui_ok == 0u){
          switch(dado_scelto_da_ui){ //vado a destra di dado_scelto_da_ui
            case 1:
              if(dado1_info->tetradado_state == DADO_FACE){
                dado1_info->tetradado_state = DADO_SELECTED_ENEMY;
                dado_scelto_da_ui_ok = 1u;
              }
            break;
            case 2:
              if(dado2_info->tetradado_state == DADO_FACE){
                dado2_info->tetradado_state = DADO_SELECTED_ENEMY;
                dado_scelto_da_ui_ok = 1u;
              }
            break;
            case 3:
              if(dado3_info->tetradado_state == DADO_FACE){
                dado3_info->tetradado_state = DADO_SELECTED_ENEMY;
                dado_scelto_da_ui_ok = 1u;
              }
            break;
            case 4:
              if(dado4_info->tetradado_state == DADO_FACE){
                dado4_info->tetradado_state = DADO_SELECTED_ENEMY;
                dado_scelto_da_ui_ok = 1u;
              }
            break;
            case 5:
              if(dado5_info->tetradado_state == DADO_FACE){
                dado5_info->tetradado_state = DADO_SELECTED_ENEMY;
                dado_scelto_da_ui_ok = 1u;
              }
            break;
            case 6:
              if(dado6_info->tetradado_state == DADO_FACE){
                dado6_info->tetradado_state = DADO_SELECTED_ENEMY;
                dado_scelto_da_ui_ok = 1u;
              }
            break;
          }
          if(dado_scelto_da_ui_ok == 0u){
            dado_scelto_da_ui++;
            if(dado_scelto_da_ui > 6){
              dado_scelto_da_ui = dado_scelto_da_ui % 6;
            }            
          }
        }
        dadi_disponibili--;        
        if(dadi_disponibili == 0){
          //passo alla fase successiva della partita
          tetra_game_state = TURN_GIVE_DICE;
        }else{
          dado_scelto_da_ui_ok = 0u;
          tetraturn = TURN_PLAYER;
        }
      }
    break;
  }

}