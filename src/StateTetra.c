#include "Banks/SetAutoBank.h"

#include <rand.h>

#include "SGB.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Fade.h"

#include "TilesAnimations0.h"
#include "sgb_palette.h"
#include "custom_datas_tetra.h"

IMPORT_TILES(tilestetra);
IMPORT_TILES(font);

IMPORT_MAP(maptetra);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern INT8 sgb_running;
UINT8 bank_tilestetra = BANK(tilestetra);

const UINT8 const collision_tiles_tetra[] = {0, 0};//numero delle tile con zero finale


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

UINT8 generate_combo_counter = 1u;
UINT8 dragon_1_combo = 0;
UINT8 dragon_2_combo = 0;
UINT8 dragon_3_combo = 0;
UINT8 dragon_4_combo = 0;
UINT8 dragon_5_combo = 0;
UINT8 dragon_6_combo = 0;
UINT8 pressed_select = 0u;

UINT8 player_chosen_cards = 0b00000000;//SU, BLAIR, ARROWS, SHIELD
UINT8 captain_chosen_cards = 0b00000000;//SU, BLAIR, ARROWS, SHIELD

void refresh_bkg_tiles() BANKED;
void refresh_counters(UINT8 is_player, UINT8 counter, TETRA_DADO_FACCE card) BANKED;
void card_chosen(UINT8 is_player, TETRA_DADO_FACCE chosen_face) BANKED;


void START() {
	//SCROLL LIMITS
      scroll_top_movement_limit = 56u;
      scroll_bottom_movement_limit = 80u;

  //SGB COLORS
  if(sgb_running){
    set_sgb_palette01_worldmap();
  }
  
  InitScroll(BANK(maptetra), &maptetra, collision_tiles_tetra, 0);

  SHOW_BKG;

  SpriteManagerLoad(SpriteTetradado);
  dado1 = SpriteManagerAdd(SpriteTetradado, (UINT16) 2u << 3, ((UINT16) 10u << 3) -4u);
  dado2 = SpriteManagerAdd(SpriteTetradado, ((UINT16) 5u << 3) - 4u, (UINT16) 12u << 3);
  dado3 = SpriteManagerAdd(SpriteTetradado, ((UINT16) 8u << 3), ((UINT16) 10u << 3) -4u);
  dado4 = SpriteManagerAdd(SpriteTetradado, ((UINT16) 11u << 3) - 4u, (UINT16) 12u << 3);
  dado5 = SpriteManagerAdd(SpriteTetradado, (UINT16) 14u << 3, ((UINT16) 10u << 3) -4u);
  dado6 = SpriteManagerAdd(SpriteTetradado, ((UINT16) 17u << 3) - 4u, (UINT16) 12u << 3);
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
  generate_combo_counter = 1u;
  pressed_select = 0u;
  initarand(sys_time);
}

void refresh_bkg_tiles() BANKED{
  //DRAGON 1 COMBO
    UINT8 dragon_card_1 = dragon_1_combo & 0b00000011;
    UINT8 dragon_card_2 = (dragon_1_combo & 0b00001100) >> 2;
    UINT8 dragon_card_3 = (dragon_1_combo & 0b00110000) >> 4;
    UINT8 dragon_card_4 = (dragon_1_combo & 0b11000000) >> 6;
    set_bkg_tile_xy(2, 8, dragon_card_1+1);
    set_bkg_tile_xy(3, 8, dragon_card_2+1);
    set_bkg_tile_xy(4, 8, dragon_card_3+1);
    set_bkg_tile_xy(5, 8, dragon_card_4+1);
  //DRAGON 2 COMBO
    dragon_card_1 = dragon_2_combo & 0b00000011;
    dragon_card_2 = (dragon_2_combo & 0b00001100) >> 2;
    dragon_card_3 = (dragon_2_combo & 0b00110000) >> 4;
    dragon_card_4 = (dragon_2_combo & 0b11000000) >> 6;
    set_bkg_tile_xy(8, 8, dragon_card_1+1);
    set_bkg_tile_xy(9, 8, dragon_card_2+1);
    set_bkg_tile_xy(10, 8, dragon_card_3+1);
    set_bkg_tile_xy(11, 8, dragon_card_4+1);
  //DRAGON 3 COMBO
    dragon_card_1 = dragon_3_combo & 0b00000011;
    dragon_card_2 = (dragon_3_combo & 0b00001100) >> 2;
    dragon_card_3 = (dragon_3_combo & 0b00110000) >> 4;
    dragon_card_4 = (dragon_3_combo & 0b11000000) >> 6;
    set_bkg_tile_xy(14, 8, dragon_card_1+1);
    set_bkg_tile_xy(15, 8, dragon_card_2+1);
    set_bkg_tile_xy(16, 8, dragon_card_3+1);
    set_bkg_tile_xy(17, 8, dragon_card_4+1);
  //PLAYER COUNTERS
    UINT8 counter_card = 0b00000000;
    counter_card = player_chosen_cards >> 6;
    refresh_counters(0, counter_card, FACCIA_UP);
    counter_card = player_chosen_cards >> 4;
    counter_card = counter_card & 0b00000011;
    refresh_counters(0, counter_card, FACCIA_BLAIR);
    counter_card = player_chosen_cards >> 2;
    counter_card = counter_card & 0b00000011;
    refresh_counters(0, counter_card, FACCIA_ARROWS);
    counter_card = player_chosen_cards;
    counter_card = counter_card & 0b00000011;
    refresh_counters(0, counter_card, FACCIA_SHIELD);
  //CAPTAIN COUNTERS
    counter_card = captain_chosen_cards >> 6;
    refresh_counters(1, counter_card, FACCIA_UP);
    counter_card = captain_chosen_cards >> 4;
    counter_card = counter_card & 0b00000011;
    refresh_counters(1, counter_card, FACCIA_BLAIR);
    counter_card = captain_chosen_cards >> 2;
    counter_card = counter_card & 0b00000011;
    refresh_counters(1, counter_card, FACCIA_ARROWS);
    counter_card = captain_chosen_cards;
    counter_card = counter_card & 0b00000011;
    refresh_counters(1, counter_card, FACCIA_SHIELD);
}

void refresh_counters(UINT8 is_player, UINT8 counter, TETRA_DADO_FACCE card) BANKED{
  switch(is_player){
    case 0u:
      switch(counter){
        case 0: set_bkg_tile_xy(2 + (card << 2), 3, 25); set_bkg_tile_xy(2+ (card << 2), 2, 25);break;
        case 1: set_bkg_tile_xy(2 + (card << 2), 3, 26); set_bkg_tile_xy(2+ (card << 2), 2, 25);break;
        case 2: set_bkg_tile_xy(2 + (card << 2), 3, 27); set_bkg_tile_xy(2+ (card << 2), 2, 25);break;
        case 3: set_bkg_tile_xy(2 + (card << 2), 3, 27); set_bkg_tile_xy(2+ (card << 2), 2, 26);break;
        case 4: set_bkg_tile_xy(2 + (card << 2), 3, 27); set_bkg_tile_xy(2+ (card << 2), 2, 27);break;
      }
    break;
    case 1:
      switch(counter){
        case 0: set_bkg_tile_xy(2 + (card << 2), 16, 25); set_bkg_tile_xy(2 + (card << 2), 15, 25);break;
        case 1: set_bkg_tile_xy(2 + (card << 2), 16, 26); set_bkg_tile_xy(2 + (card << 2), 15, 25);break;
        case 2: set_bkg_tile_xy(2 + (card << 2), 16, 27); set_bkg_tile_xy(2 + (card << 2), 15, 25);break;
        case 3: set_bkg_tile_xy(2 + (card << 2), 16, 27); set_bkg_tile_xy(2 + (card << 2), 15, 26);break;
        case 4: set_bkg_tile_xy(2 + (card << 2), 16, 27); set_bkg_tile_xy(2 + (card << 2), 15, 27);break;
      }
    break;
  }
}

void UPDATE(){
  if(KEY_RELEASED(J_START)){pressed_select = 1u;}
  if(pressed_select == 0){return;}
  if(generate_combo_counter < 5 && pressed_select == 1u){
    switch(generate_combo_counter){
      case 1u:dragon_1_combo = rand();break;
      case 2u:dragon_2_combo = rand();break;
      case 3u:dragon_3_combo = rand();break;
      case 4u:refresh_bkg_tiles();break;
    }
    generate_combo_counter++;
    return;
  }
  if(KEY_PRESSED(J_DOWN) || KEY_PRESSED(J_UP) || KEY_PRESSED(J_LEFT) || KEY_PRESSED(J_RIGHT)){
    refresh_bkg_tiles();
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
      if(tetraturn == TURN_ENEMY){//praticamente codo la parte di scelta enemy del dado,
        if(dadi_disponibili == 0u){//passo alla fase successiva della partita
          tetra_game_state = TURN_GIVE_DICE;
          return;
        }
        //scelgo il dado
        dado_scelto_da_ui += tetra_cursor->x;
        dado_scelto_da_ui += scroll_target->y;
        dado_scelto_da_ui = dado_scelto_da_ui % 6;
        TETRA_DADO_FACCE faccia_scelta_da_ui = FACCIA_UP;
        while (dado_scelto_da_ui_ok == 0u){
          switch(dado_scelto_da_ui){ //vado a destra di dado_scelto_da_ui
            case 1:
              if(dado1_info->tetradado_state == DADO_FACE){
                dado1_info->tetradado_state = DADO_SELECTED_ENEMY;
                faccia_scelta_da_ui = dado1_info->tetradado_faccia;
                dado_scelto_da_ui_ok = 1u;
              }
            break;
            case 2:
              if(dado2_info->tetradado_state == DADO_FACE){
                dado2_info->tetradado_state = DADO_SELECTED_ENEMY;
                faccia_scelta_da_ui = dado2_info->tetradado_faccia;
                dado_scelto_da_ui_ok = 1u;
              }
            break;
            case 3:
              if(dado3_info->tetradado_state == DADO_FACE){
                dado3_info->tetradado_state = DADO_SELECTED_ENEMY;
                faccia_scelta_da_ui = dado3_info->tetradado_faccia;
                dado_scelto_da_ui_ok = 1u;
              }
            break;
            case 4:
              if(dado4_info->tetradado_state == DADO_FACE){
                dado4_info->tetradado_state = DADO_SELECTED_ENEMY;
                faccia_scelta_da_ui = dado4_info->tetradado_faccia;
                dado_scelto_da_ui_ok = 1u;
              }
            break;
            case 5:
              if(dado5_info->tetradado_state == DADO_FACE){
                dado5_info->tetradado_state = DADO_SELECTED_ENEMY;
                faccia_scelta_da_ui = dado5_info->tetradado_faccia;
                dado_scelto_da_ui_ok = 1u;
              }
            break;
            case 6:
              if(dado6_info->tetradado_state == DADO_FACE){
                dado6_info->tetradado_state = DADO_SELECTED_ENEMY;
                faccia_scelta_da_ui = dado6_info->tetradado_faccia;
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
        card_chosen(1, faccia_scelta_da_ui);
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

void card_chosen(UINT8 is_player, TETRA_DADO_FACCE chosen_face) BANKED{
  switch(is_player){
    case 0u://player chose
      {
        UINT8 current_counter = player_chosen_cards >> (6 - (chosen_face << 1));
        current_counter = current_counter & 0b00000011;
        if(current_counter < 4){
          current_counter++;
        }
        current_counter = current_counter << (6 - (chosen_face << 1));
        UINT8 first_operand = 0b0;
        UINT8 second_operand = 0b0;
        switch(chosen_face){
          case FACCIA_UP://maschero per i primi due più a sinistra
            first_operand = current_counter & 0b11000000;
            second_operand = player_chosen_cards & 0b00111111;
          break;
          case FACCIA_BLAIR://maschero per i primi due più a sinistra
            first_operand = current_counter & 0b00110000;
            second_operand = player_chosen_cards & 0b11001111;
          break;
          case FACCIA_ARROWS://maschero per i primi due più a sinistra
            first_operand = current_counter & 0b00001100;
            second_operand = player_chosen_cards & 0b11110011;
          break;
          case FACCIA_SHIELD://maschero per i primi due più a sinistra
            first_operand = current_counter & 0b00000011;
            second_operand = player_chosen_cards & 0b11111100;
          break;
        }
        player_chosen_cards = first_operand | second_operand;
      }
    break;
    case 1u://captain chose
      {
        UINT8 current_counter = captain_chosen_cards >> (6 - (chosen_face << 1));
        current_counter = current_counter & 0b00000011;
        if(current_counter < 4){
          current_counter++;
        }
        current_counter = current_counter << (6 - (chosen_face << 1));
        UINT8 first_operand = 0b0;
        UINT8 second_operand = 0b0;
        switch(chosen_face){
          case FACCIA_UP://maschero per i primi due più a sinistra
            first_operand = current_counter & 0b11000000;
            second_operand = captain_chosen_cards & 0b00111111;
          break;
          case FACCIA_BLAIR://maschero per i primi due più a sinistra
            first_operand = current_counter & 0b00110000;
            second_operand = captain_chosen_cards & 0b11001111;
          break;
          case FACCIA_ARROWS://maschero per i primi due più a sinistra
            first_operand = current_counter & 0b00001100;
            second_operand = captain_chosen_cards & 0b11110011;
          break;
          case FACCIA_SHIELD://maschero per i primi due più a sinistra
            first_operand = current_counter & 0b00000011;
            second_operand = captain_chosen_cards & 0b11111100;
          break;
        }
        captain_chosen_cards = first_operand | second_operand;
      }
    break;
  }
  refresh_bkg_tiles();
}