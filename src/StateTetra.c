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

#include "TAnim0.h"
#include "sgb_palette.h"
#include "custom_datas_tetra.h"

#define CAPTAIN_THINKING_TIME 160

IMPORT_TILES(tilestetra);
IMPORT_TILES(fontbw);

IMPORT_MAP(maptetra);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;
extern INT8 sgb_running;
extern UINT8 pointing_configured;

const UINT8 const collision_tiles_tetra[] = {0, 0};//numero delle tile con zero finale

unsigned char EMPTY_STRING_20[21] = "                    ";

Sprite* dado1 = 0;
Sprite* dado2 = 0;
Sprite* dado3 = 0;
Sprite* dado4 = 0;
Sprite* dado5 = 0;
Sprite* dado6 = 0;
Sprite* s_hourglass = 0;
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
struct TETRA_WINNER tetra_winner = {.winner = TURN_PLAYER, .declared = 0u};

UINT8 dadi_disponibili = 6u;
UINT8 dado_scelto_da_ui = 1u;
UINT8 dado_scelto_da_ui_ok = 0u;

UINT8 generate_combo_counter = 1u;
UINT8 dragon_1_combo = 0;
UINT8 dragon_2_combo = 0;
UINT8 dragon_3_combo = 0;
UINT8 pressed_select = 0u;
UINT8 start_pick_dice_change_turn = 0u;

TETRA_CAMERA_STATE camera_state = PLAY;
UINT8 changing_camera_state = 1u;
TETRA_CAMERA_STATE next_camera_state = READ_INSTRUCTIONS;
UINT16 player_chosen_cards = 0b00000000;//SU, BLAIR, ARROWS, SHIELD
UINT16 captain_chosen_cards = 0b00000000;//SU, BLAIR, ARROWS, SHIELD
UINT8 player_counter_dragons = 0u;
UINT8 captain_counter_dragons = 0u;
UINT8 dragons_available = 0b00000000;
struct DRAGON_COSTS dragon_cost_first = {.up_cost = 0, .blair_cost = 0, .arrows_cost = 0, .shield_cost = 0};
struct DRAGON_COSTS dragon_cost_second = {.up_cost = 0, .blair_cost = 0, .arrows_cost = 0, .shield_cost = 0};
struct DRAGON_COSTS dragon_cost_third = {.up_cost = 0, .blair_cost = 0, .arrows_cost = 0, .shield_cost = 0};
UINT8 captain_thinking_time_counter = 0;
UINT8 exit_countdown = 0;
UINT8 instruction_blink_countdown = 0;
UINT8 initrand_executed = 0u;

void refresh_bkg_tiles() BANKED;
void refresh_counters(UINT8 is_player, UINT8 counter, TETRA_DADO_FACCE card) BANKED;
void card_chosen(TETRA_DADO_FACCE chosen_face) BANKED;
struct DRAGON_COSTS* get_dragon_cost(UINT8 dragon_arg) BANKED;
UINT8 buy_dragon(UINT8 dragon_arg, UINT8 just_check) BANKED;
void tetra_change_game_state(TETRA_GAME_STATE new_game_state) BANKED;
void tetra_change_turn() BANKED;
void add_dragon(UINT8 dragon_arg) BANKED;
void make_dice() BANKED;
void change_camera_state(TETRA_CAMERA_STATE new_camera_state) BANKED;
void refresh_phase() BANKED;
void winner(TETRA_TURN winner) BANKED;
void instructions_blink() BANKED;
void tetra_init_vars() BANKED;

extern void move_cursor_hand_dice(UINT8 direction) BANKED;
extern void ChangeStateThroughBetween_TetraWinner() BANKED;
extern void ChangeStateThroughBetween_TetraLoser() BANKED;        
extern void play_music_missionaccomplished() BANKED;

void START() {
	//SCROLL LIMITS
      scroll_top_movement_limit = 56u;
      scroll_bottom_movement_limit = 80u;

  //SGB COLORS
    if(sgb_running){
      set_sgb_palette_tetra();
    }
  InitScroll(BANK(maptetra), &maptetra, collision_tiles_tetra, 0);
  print_target = PRINT_WIN;
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

  tetra_init_vars();
  tetra_change_game_state(START_GAME);//INIT_GAME;
  if(initrand_executed == 0){
    initrand_executed = 1u;
    initarand(sys_time);
  } 
  INIT_FONT(fontbw, PRINT_BKG); 
  PRINT(18, 16, "%u", player_counter_dragons);
  PRINT(18, 1, "%u", captain_counter_dragons);
  refresh_phase();
}

void tetra_init_vars() BANKED{
  next_dice = -1;
  tetra_game_state = START_GAME;
  tetraturn = TURN_PLAYER;
  tetra_winner.winner = TURN_PLAYER;
  tetra_winner.declared = 0u;
  dadi_disponibili = 6u;
  dado_scelto_da_ui = 1u;
  dado_scelto_da_ui_ok = 0u;
  generate_combo_counter = 1u;
  dragon_1_combo = 0;
  dragon_2_combo = 0;
  dragon_3_combo = 0;
  pressed_select = 0u;
  start_pick_dice_change_turn = 0u;
  camera_state = PLAY;
  changing_camera_state = 1u;
  next_camera_state = READ_INSTRUCTIONS;
  player_chosen_cards = 0b00000000;//SU, BLAIR, ARROWS, SHIELD
  captain_chosen_cards = 0b00000000;//SU, BLAIR, ARROWS, SHIELD
  player_counter_dragons = 0u;
  captain_counter_dragons = 0u;
  dragons_available = 0b00000000;
  dragon_cost_first.up_cost = 0;
  dragon_cost_first.blair_cost = 0;
  dragon_cost_first.arrows_cost = 0;
  dragon_cost_first.shield_cost = 0;
  dragon_cost_second.up_cost = 0;
  dragon_cost_second.blair_cost = 0;
  dragon_cost_second.arrows_cost = 0;
  dragon_cost_second.shield_cost = 0;
  dragon_cost_third.up_cost = 0;
  dragon_cost_third.blair_cost = 0;
  dragon_cost_third.arrows_cost = 0;
  dragon_cost_third.shield_cost = 0;
  captain_thinking_time_counter = 0;
  exit_countdown = 0;
  instruction_blink_countdown = 0;
}

void refresh_bkg_tiles() BANKED{
  //DRAGON 1 COMBO
    UINT8 dragon_card_1 = dragon_1_combo & 0b00000011;
    UINT8 dragon_card_2 = (dragon_1_combo & 0b00001100) >> 2;
    UINT8 dragon_card_3 = (dragon_1_combo & 0b00110000) >> 4;
    UINT8 dragon_card_4 = (dragon_1_combo & 0b11000000) >> 6;
    UINT8 is_drago_available = dragons_available & 0b10000000;
    if(is_drago_available == 0 && dragon_1_combo != 0){
      set_bkg_tile_xy(2, 8, dragon_card_1+1);
      set_bkg_tile_xy(3, 8, dragon_card_2+1);
      set_bkg_tile_xy(4, 8, dragon_card_3+1);
      set_bkg_tile_xy(5, 8, dragon_card_4+1);
    }else{
      set_bkg_tile_xy(2, 8, 0);
      set_bkg_tile_xy(3, 8, 0);
      set_bkg_tile_xy(4, 8, 0);
      set_bkg_tile_xy(5, 8, 0);
    }
  //DRAGON 2 COMBO
    dragon_card_1 = dragon_2_combo & 0b00000011;
    dragon_card_2 = (dragon_2_combo & 0b00001100) >> 2;
    dragon_card_3 = (dragon_2_combo & 0b00110000) >> 4;
    dragon_card_4 = (dragon_2_combo & 0b11000000) >> 6;
    is_drago_available = dragons_available & 0b01000000;
    if(is_drago_available == 0 && dragon_2_combo != 0){
      set_bkg_tile_xy(8, 8, dragon_card_1+1);
      set_bkg_tile_xy(9, 8, dragon_card_2+1);
      set_bkg_tile_xy(10, 8, dragon_card_3+1);
      set_bkg_tile_xy(11, 8, dragon_card_4+1);
    }else{
      set_bkg_tile_xy(8, 8, 0);
      set_bkg_tile_xy(9, 8, 0);
      set_bkg_tile_xy(10, 8, 0);
      set_bkg_tile_xy(11, 8, 0);
    }
  //DRAGON 3 COMBO
    dragon_card_1 = (dragon_3_combo & 0b11000000) >> 6;
    dragon_card_2 = (dragon_3_combo & 0b00110000) >> 4;
    dragon_card_3 = (dragon_3_combo & 0b00001100) >> 2;
    dragon_card_4 = dragon_3_combo & 0b00000011;
    is_drago_available = dragons_available & 0b00100000;
    if(is_drago_available == 0 && dragon_3_combo != 0){
      set_bkg_tile_xy(14, 8, dragon_card_1+1);
      set_bkg_tile_xy(15, 8, dragon_card_2+1);
      set_bkg_tile_xy(16, 8, dragon_card_3+1);
      set_bkg_tile_xy(17, 8, dragon_card_4+1);
    }else{
      set_bkg_tile_xy(14, 8, 0);
      set_bkg_tile_xy(15, 8, 0);
      set_bkg_tile_xy(16, 8, 0);
      set_bkg_tile_xy(17, 8, 0);
    }
  //COUNTERS
    if(scroll_target->y < 180){
      //PLAYER COUNTERS
        UINT16 counter_card = 0;
        counter_card = player_chosen_cards >> 12;
        refresh_counters(1, counter_card, FACCIA_UP);
        counter_card = player_chosen_cards >> 8;
        counter_card = counter_card & 0b00001111;
        refresh_counters(1, counter_card, FACCIA_BLAIR);
        counter_card = player_chosen_cards >> 4;
        counter_card = counter_card & 0b00001111;
        refresh_counters(1, counter_card, FACCIA_ARROWS);
        counter_card = player_chosen_cards;
        counter_card = counter_card & 0b00001111;
        refresh_counters(1, counter_card, FACCIA_SHIELD);
      //CAPTAIN COUNTERS
        counter_card = captain_chosen_cards >> 12;
        refresh_counters(0, counter_card, FACCIA_UP);
        counter_card = captain_chosen_cards >> 8;
        counter_card = counter_card & 0b0000000000001111;
        refresh_counters(0, counter_card, FACCIA_BLAIR);
        counter_card = captain_chosen_cards >> 4;
        counter_card = counter_card & 0b0000000000001111;
        refresh_counters(0, counter_card, FACCIA_ARROWS);
        counter_card = captain_chosen_cards;
        counter_card = counter_card & 0b0000000000001111;
        refresh_counters(0, counter_card, FACCIA_SHIELD);
    
      PRINT(18, 16, "%i", player_counter_dragons);
      PRINT(18, 1, "%i", captain_counter_dragons);
  }
  //refresh_phase();
}

void refresh_phase() BANKED{
  instruction_blink_countdown = 100;
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

void change_camera_state(TETRA_CAMERA_STATE new_camera_state) BANKED{
  changing_camera_state = 1;
  next_camera_state = new_camera_state;
}

void UPDATE(){
 if(tetra_winner.declared == 1){
  switch(tetra_winner.winner){
    case TURN_PLAYER:
      exit_countdown--;
      if(exit_countdown < 10){
         ChangeStateThroughBetween_TetraWinner();
      }
    break;
    case TURN_CAPTAIN:
      exit_countdown--;
      if(exit_countdown < 10){
         ChangeStateThroughBetween_TetraLoser();
      }
    break;
  }
  return;
 }
  if(instruction_blink_countdown > 0){
    instruction_blink();
  }
  switch(tetra_game_state){
    case START_GAME:
    case TURN_MAKE_DRAGONS:
      if(KEY_RELEASED(J_START)){pressed_select = 1u;}
      if(pressed_select == 0){return;}
      if(generate_combo_counter < 5 && pressed_select == 1u){
        switch(generate_combo_counter){
          case 1u:dragon_1_combo = rand();break;
          case 2u:dragon_2_combo = rand();break;
          case 3u:dragon_3_combo = rand();break;
          case 4u://evaluate dragon costs
            {
            UINT8 dragon_1_cost = 0u;
            UINT8 dragon_2_cost = 0u;
            UINT8 dragon_3_cost = 0u;
            UINT8 up_cost = 0;
            UINT8 blair_cost = 0;
            UINT8 arrows_cost = 0;
            UINT8 shield_cost = 0;
            UINT8 first_code = dragon_1_combo & 0b11000000;
            first_code = first_code >> 6;
            UINT8 second_code = dragon_1_combo & 0b00110000;
            second_code = second_code >> 4;
            UINT8 third_code = dragon_1_combo & 0b00001100;
            third_code = third_code >> 2;
            UINT8 fourth_code = dragon_1_combo & 0b00000011;
            switch(first_code){
              case 0u: up_cost++;break;
              case 1u: blair_cost++;break;
              case 2u: arrows_cost++;break;
              case 3u: shield_cost++;break;
            }
            switch(second_code){
              case 0u: up_cost++;break;
              case 1u: blair_cost++;break;
              case 2u: arrows_cost++;break;
              case 3u: shield_cost++;break;
            }
            switch(third_code){
              case 0u: up_cost++;break;
              case 1u: blair_cost++;break;
              case 2u: arrows_cost++;break;
              case 3u: shield_cost++;break;
            }
            switch(fourth_code){
              case 0u: up_cost++;break;
              case 1u: blair_cost++;break;
              case 2u: arrows_cost++;break;
              case 3u: shield_cost++;break;
            }
            dragon_cost_first.up_cost = up_cost;
            dragon_cost_first.blair_cost = blair_cost;
            dragon_cost_first.arrows_cost = arrows_cost;
            dragon_cost_first.shield_cost = shield_cost;

            up_cost = 0;
            blair_cost = 0;
            arrows_cost = 0;
            shield_cost = 0;
            first_code = dragon_2_combo & 0b11000000;
            first_code = first_code >> 6;
            second_code = dragon_2_combo & 0b00110000;
            second_code = second_code >> 4;
            third_code = dragon_2_combo & 0b00001100;
            third_code = third_code >> 2;
            fourth_code = dragon_2_combo & 0b00000011;
            switch(first_code){
              case 0u: up_cost++;break;
              case 1u: blair_cost++;break;
              case 2u: arrows_cost++;break;
              case 3u: shield_cost++;break;
            }
            switch(second_code){
              case 0u: up_cost++;break;
              case 1u: blair_cost++;break;
              case 2u: arrows_cost++;break;
              case 3u: shield_cost++;break;
            }
            switch(third_code){
              case 0u: up_cost++;break;
              case 1u: blair_cost++;break;
              case 2u: arrows_cost++;break;
              case 3u: shield_cost++;break;
            }
            switch(fourth_code){
              case 0u: up_cost++;break;
              case 1u: blair_cost++;break;
              case 2u: arrows_cost++;break;
              case 3u: shield_cost++;break;
            }
            dragon_cost_second.up_cost = up_cost;
            dragon_cost_second.blair_cost = blair_cost;
            dragon_cost_second.arrows_cost = arrows_cost;
            dragon_cost_second.shield_cost = shield_cost;

            up_cost = 0;
            blair_cost = 0;
            arrows_cost = 0;
            shield_cost = 0;
            first_code = dragon_3_combo & 0b11000000;
            first_code = first_code >> 6;
            second_code = dragon_3_combo & 0b00110000;
            second_code = second_code >> 4;
            third_code = dragon_3_combo & 0b00001100;
            third_code = third_code >> 2;
            fourth_code = dragon_3_combo & 0b00000011;
            switch(first_code){
              case 0u: up_cost++;break;
              case 1u: blair_cost++;break;
              case 2u: arrows_cost++;break;
              case 3u: shield_cost++;break;
            }
            switch(second_code){
              case 0u: up_cost++;break;
              case 1u: blair_cost++;break;
              case 2u: arrows_cost++;break;
              case 3u: shield_cost++;break;
            }
            switch(third_code){
              case 0u: up_cost++;break;
              case 1u: blair_cost++;break;
              case 2u: arrows_cost++;break;
              case 3u: shield_cost++;break;
            }
            switch(fourth_code){
              case 0u: up_cost++;break;
              case 1u: blair_cost++;break;
              case 2u: arrows_cost++;break;
              case 3u: shield_cost++;break;
            }
            dragon_cost_third.up_cost = up_cost;
            dragon_cost_third.blair_cost = blair_cost;
            dragon_cost_third.arrows_cost = arrows_cost;
            dragon_cost_third.shield_cost = shield_cost;

            refresh_bkg_tiles();
            }
          break;
        }
        generate_combo_counter++;
        return;
      }else{tetra_change_game_state(TURN_MAKE_DICE);}
    break;
    case TURN_MAKE_DICE:
      if(next_dice == -1){
        if(KEY_TICKED(J_FIRE) || KEY_RELEASED(J_START)){
          next_dice = 0;
          tetra_cursor->x = dado1->x;
          tetra_cursor->y = dado1->y - 8u;
          if(tetra_cursor_info->cursor_state != TRIANGLE_BLINK){
            tetra_cursor_info->cursor_state = TRIANGLE_BLINK;
          }
          dado1_info->tetradado_state = ROLLING_SUPERFAST;
          dado2_info->tetradado_state = ROLLING_SUPERFAST;
          dado3_info->tetradado_state = ROLLING_SUPERFAST;
          dado4_info->tetradado_state = ROLLING_SUPERFAST;
          dado5_info->tetradado_state = ROLLING_SUPERFAST;
          dado6_info->tetradado_state = ROLLING_SUPERFAST;
        }
      }else if(next_dice < 7){
        switch(tetraturn){
          case TURN_PLAYER:
            if(KEY_TICKED(J_FIRE)){
              make_dice();
            }
          break;          
          case TURN_CAPTAIN:
            make_dice();
          break;
        }
      }
    break;
    case TURN_PICK_DICE:
      if(dado6_info->tetradado_state < DADO_FACE){
        tetra_cursor_info->cursor_state = CURSOR_INVISIBLE;return;
      }else{
        tetra_cursor_info->cursor_state = HAND_OPENED;
      }
      if(tetraturn == TURN_CAPTAIN){//praticamente codo la parte di scelta enemy del dado,
        if(captain_thinking_time_counter > 10){captain_thinking_time_counter--; return;}
        SpriteManagerRemoveSprite(s_hourglass);
        tetra_cursor_info->cursor_state = HAND_OPENED;
        if(dadi_disponibili == 0u){//passo alla fase successiva della partita
          tetra_change_game_state(TURN_PICK_DRAGON);
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
        dado_scelto_da_ui_ok = 0u;
        card_chosen(faccia_scelta_da_ui);
      }
    break;
    case TURN_PICK_DRAGON:
      if(tetraturn == TURN_CAPTAIN){
        if(captain_thinking_time_counter > 10){captain_thinking_time_counter--; return;}
        SpriteManagerRemoveSprite(s_hourglass);
        UINT8 can_buy_dragon_1 = buy_dragon(1, 0);
        UINT8 can_buy_dragon_2 = buy_dragon(2, 0);
        UINT8 can_buy_dragon_3 = buy_dragon(3, 0);
        if(can_buy_dragon_1 || can_buy_dragon_2 || can_buy_dragon_3){
          tetra_change_game_state(TURN_BUY_DRAGON);
        }else{
          tetra_change_turn();
        }
      }
    break;
    case TURN_BUY_DRAGON:
      if(tetraturn == TURN_CAPTAIN){
        UINT8 can_buy_dragon = buy_dragon(1, 0);
        if(can_buy_dragon){
          buy_dragon(1, 1);
        }else{
          can_buy_dragon = buy_dragon(2, 0);
          if(can_buy_dragon){
            buy_dragon(2, 1);
          }else{
            can_buy_dragon = buy_dragon(3, 0);
            if(can_buy_dragon){
              buy_dragon(3, 1);
            }
          }
        }
        tetra_change_turn();
      }
    break;
  }
}

void make_dice() BANKED{
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
      dado6_info->tetradado_state = ROLLING_FAST;
      tetra_cursor->x = dado1->x;
      tetra_cursor->y = dado1->y + 16u;
      tetra_cursor_info->cursor_state = CURSOR_INVISIBLE;
      tetra_change_game_state(TURN_PICK_DICE);
    break;
  }
}

void tetra_change_game_state(TETRA_GAME_STATE new_game_state) BANKED{
 if(tetra_winner.declared != 0){return;}
  switch(new_game_state){
    case START_GAME:
    case TURN_MAKE_DRAGONS:
      player_chosen_cards = 0;//SU, BLAIR, ARROWS, SHIELD
      captain_chosen_cards = 0;//SU, BLAIR, ARROWS, SHIELD
      dragon_1_combo = 0;
      dragon_2_combo = 0;
      dragon_3_combo = 0;
      dragons_available = 0;
      dado1_info->tetradado_state = DADO_INVISIBLE;
      dado2_info->tetradado_state = DADO_INVISIBLE;
      dado3_info->tetradado_state = DADO_INVISIBLE;
      dado4_info->tetradado_state = DADO_INVISIBLE;
      dado5_info->tetradado_state = DADO_INVISIBLE;
      dado6_info->tetradado_state = DADO_INVISIBLE;
      tetra_cursor_info->cursor_state = CURSOR_INVISIBLE;
      tetra_cursor->x = dado1->x;
      tetra_cursor->y = dado1->y - 8u;
      set_bkg_tile_xy(2, 8, 0);
      set_bkg_tile_xy(3, 8, 0);
      set_bkg_tile_xy(4, 8, 0);
      set_bkg_tile_xy(5, 8, 0);
      set_bkg_tile_xy(8, 8, 0);
      set_bkg_tile_xy(9, 8, 0);
      set_bkg_tile_xy(10, 8, 0);
      set_bkg_tile_xy(11, 8, 0);
      set_bkg_tile_xy(14, 8, 0);
      set_bkg_tile_xy(15, 8, 0);
      set_bkg_tile_xy(16, 8, 0);
      set_bkg_tile_xy(17, 8, 0);
      refresh_bkg_tiles();
      refresh_phase();
    break;
    case TURN_MAKE_DICE:
      generate_combo_counter = 0;
      pressed_select = 0;
      next_dice = -1;
      dadi_disponibili = 6u;
      tetra_cursor_info->cursor_state = CURSOR_INVISIBLE;
      dado1_info->tetradado_state = DADO_INVISIBLE;
      dado2_info->tetradado_state = DADO_INVISIBLE;
      dado3_info->tetradado_state = DADO_INVISIBLE;
      dado4_info->tetradado_state = DADO_INVISIBLE;
      dado5_info->tetradado_state = DADO_INVISIBLE;
      dado6_info->tetradado_state = DADO_INVISIBLE;
      refresh_bkg_tiles();
    break;
    case TURN_PICK_DICE:
      tetra_cursor_info->cursor_state = HAND_OPENED;
      if(start_pick_dice_change_turn == 0u){
        start_pick_dice_change_turn = 1;
        tetra_change_turn();
      }else{
        start_pick_dice_change_turn = 0;
      }
      refresh_bkg_tiles();
    break;
    case TURN_PICK_DRAGON:
      tetra_cursor_info->cursor_state = HAND_OPENED;
      tetra_cursor->x = ((UINT16) 3u << 3);
      tetra_cursor->y = ((UINT16) 6u << 3);
      refresh_bkg_tiles();
    break;
    case TURN_BUY_DRAGON:
    break;
    case WINNER:
      tetra_winner.winner = TURN_PLAYER;
      tetra_winner.declared = 1;
      exit_countdown = 200;
      PRINT(0, 9, "   !!DESSA WINS!!    ");
    break;
    case LOSER:
      tetra_winner.winner = TURN_CAPTAIN;
      tetra_winner.declared = 1;
      exit_countdown = 200;
      PRINT(0, 9, "  !!CAPTAIN WINS!!   ");
    break;
  }
  tetra_game_state = new_game_state;
  refresh_phase();
}

void tetra_change_turn() BANKED{
  if(tetraturn == TURN_PLAYER){
    tetraturn = TURN_CAPTAIN;
    captain_thinking_time_counter = CAPTAIN_THINKING_TIME;
    s_hourglass = SpriteManagerAdd(SpriteTetrahourglass, 68u, (UINT16) 13 << 3);
  }else if(tetraturn == TURN_CAPTAIN){tetraturn = TURN_PLAYER;}
}

struct DRAGON_COSTS* get_dragon_cost(UINT8 dragon_arg) BANKED{
  switch(dragon_arg){
    case 1: return &dragon_cost_first; break;
    case 2: return &dragon_cost_second; break;
    case 3: return &dragon_cost_third; break;
  }
}

UINT8 buy_dragon(UINT8 dragon_arg, UINT8 just_check) BANKED{
    UINT8 result = 0;
    //check se drago già preso fra player e captain
      UINT8 taken = 0;
      switch(dragon_arg){
        case 1: 
          taken = dragons_available & 0b10000000; 
        break;
        case 2:
          taken = dragons_available & 0b01000000;
        break;
        case 3:
          taken = dragons_available & 0b001000000;
        break;
      }
      if(taken != 0){return result;}

    struct DRAGON_COSTS* dragon_cost_address = get_dragon_cost(dragon_arg);
    struct DRAGON_COSTS dragon_cost = {
      .up_cost = dragon_cost_address->up_cost,
      .blair_cost = dragon_cost_address->blair_cost,
      .arrows_cost = dragon_cost_address->arrows_cost,
      .shield_cost = dragon_cost_address->shield_cost};
    UINT8 up_cost = dragon_cost.up_cost;
    UINT8 blair_cost = dragon_cost.blair_cost;
    UINT8 arrows_cost = dragon_cost.arrows_cost;
    UINT8 shield_cost = dragon_cost.shield_cost;
    UINT16 turn_chosen_cards = 0u;
    switch(tetraturn){
      case TURN_PLAYER:turn_chosen_cards = player_chosen_cards; break;
      case TURN_CAPTAIN:turn_chosen_cards = captain_chosen_cards; break;
    }
    UINT16 up_turn = turn_chosen_cards & 0b1111000000000000;
    up_turn = up_turn >> 12;
    UINT16 blair_turn = turn_chosen_cards & 0b0000111100000000;
    blair_turn = blair_turn >> 8;
    UINT16 arrows_turn = turn_chosen_cards & 0b0000000011110000;
    arrows_turn = arrows_turn >> 4;
    UINT16 shield_turn = turn_chosen_cards & 0b0000000000001111;
    if(up_turn >= up_cost && blair_turn >= blair_cost && arrows_turn >= arrows_cost && shield_turn >= shield_cost){
      result = 1;
      if(just_check != 0){
        UINT16 new_symbol_turn = up_turn - up_cost;
        UINT16 first_op = new_symbol_turn << 12;
        UINT16 second_op = turn_chosen_cards & 0b0000111111111111;
        if(up_cost > 0){
          turn_chosen_cards = first_op | second_op;
        }
        if(blair_cost > 0){
          new_symbol_turn = blair_turn - blair_cost;
          first_op = new_symbol_turn << 8;
          second_op = turn_chosen_cards & 0b1111000011111111;
          turn_chosen_cards = first_op | second_op;
        }
        if(arrows_cost > 0){
          new_symbol_turn = arrows_turn - arrows_cost;
          first_op = new_symbol_turn << 4;
          second_op = turn_chosen_cards & 0b1111111100001111;
          turn_chosen_cards = first_op | second_op;
        }
        if(shield_cost > 0){
          first_op = shield_turn - shield_cost;
          second_op = turn_chosen_cards & 0b1111111111110000;
          turn_chosen_cards = first_op | second_op;
        }
        switch(tetraturn){
          case TURN_PLAYER:
            SpriteManagerAdd(SpriteTetrapointing,0,0);
            pointing_configured = 2;
            player_chosen_cards = turn_chosen_cards;
          break;
          case TURN_CAPTAIN:
            SpriteManagerAdd(SpriteTetrapointing,0,0);
            pointing_configured = 1;
            captain_chosen_cards = turn_chosen_cards;
          break;
        }
        add_dragon(dragon_arg);
      }
    }
    return result;
}

void add_dragon(UINT8 dragon_arg) BANKED{
  switch(tetraturn){
    case TURN_PLAYER:
      player_counter_dragons++;
    break;
    case TURN_CAPTAIN:
      captain_counter_dragons++;
    break;
  }
  switch(dragon_arg){
    case 1: 
      dragons_available = dragons_available | 0b10000000;
    break;
    case 2:
      dragons_available = dragons_available | 0b01000000;
    break;
    case 3:
      dragons_available = dragons_available | 0b00100000;
    break;
  }
  refresh_bkg_tiles();
  if(player_counter_dragons == 4){
    winner(TURN_PLAYER);
  }else if(captain_counter_dragons == 4){
    winner(TURN_CAPTAIN);
  }else{
    tetra_change_game_state(TURN_MAKE_DRAGONS);
  }
}

void card_chosen(TETRA_DADO_FACCE chosen_face) BANKED{
  switch(tetraturn){
    case TURN_PLAYER://player chose
      {
        UINT16 current_counter = player_chosen_cards >> (12 - (chosen_face << 2));
        current_counter = current_counter & 0b0000000000001111;
        if(current_counter < 4){
          current_counter++;
          current_counter = current_counter << (12 - (chosen_face << 2));
          UINT16 first_operand = 0b0;
          UINT16 second_operand = 0b0;
          switch(chosen_face){
            case FACCIA_UP://maschero per i primi due più a sinistra
              first_operand = current_counter & 0b1111000000000000;
              second_operand = player_chosen_cards & 0b0000111111111111;
            break;
            case FACCIA_BLAIR://maschero per i primi due più a sinistra
              first_operand = current_counter & 0b0000111100000000;
              second_operand = player_chosen_cards & 0b1111000011111111;
            break;
            case FACCIA_ARROWS://maschero per i primi due più a sinistra
              first_operand = current_counter & 0b0000000011110000;
              second_operand = player_chosen_cards & 0b1111111100001111;
            break;
            case FACCIA_SHIELD://maschero per i primi due più a sinistra
              first_operand = current_counter & 0b0000000000001111;
              second_operand = player_chosen_cards & 0b1111111111110000;
            break;
          }
          player_chosen_cards = first_operand | second_operand;
        }
      }
    break;
    case TURN_CAPTAIN://captain chose
      {
        UINT16 current_counter = captain_chosen_cards >> (12 - (chosen_face << 2));
        current_counter = current_counter & 0b0000000000001111;
        if(current_counter < 4){
          current_counter++;
        }
        current_counter = current_counter << (12 - (chosen_face << 2));
        UINT16 first_operand = 0b0;
        UINT16 second_operand = 0b0;
        switch(chosen_face){
          case FACCIA_UP://maschero per i primi due più a sinistra
            first_operand = current_counter & 0b1111000000000000;
            second_operand = captain_chosen_cards & 0b0000111111111111;
          break;
          case FACCIA_BLAIR://maschero per i primi due più a sinistra
            first_operand = current_counter & 0b0000111100000000;
            second_operand = captain_chosen_cards & 0b1111000011111111;
          break;
          case FACCIA_ARROWS://maschero per i primi due più a sinistra
            first_operand = current_counter & 0b0000000011110000;
            second_operand = captain_chosen_cards & 0b1111111100001111;
          break;
          case FACCIA_SHIELD://maschero per i primi due più a sinistra
            first_operand = current_counter & 0b0000000000001111;
            second_operand = captain_chosen_cards & 0b1111111111110000;
          break;
        }
        captain_chosen_cards = first_operand | second_operand;
      }
    break;
  }
  refresh_bkg_tiles();
  dadi_disponibili--;  
  if(dadi_disponibili == 0){//passo alla fase successiva della partita
    tetra_change_game_state(TURN_PICK_DRAGON);
  }else{
    //move_cursor_hand_dice(J_RIGHT);
    tetra_change_turn();
  }
}

void instruction_blink() BANKED{
  instruction_blink_countdown--;
  if(instruction_blink_countdown < 20 || (instruction_blink_countdown % 10 == 0)){
    set_bkg_tile_xy(1, 0, 32u);
    set_bkg_tile_xy(2, 0, 32u);
    set_bkg_tile_xy(3, 0, 32u);
    set_bkg_tile_xy(4, 0, 32u);
    set_bkg_tile_xy(5, 0, 32u);
    set_bkg_tile_xy(6, 0, 32u);
    set_bkg_tile_xy(7, 0, 32u);
    set_bkg_tile_xy(8, 0, 32u);
    set_bkg_tile_xy(9, 0, 32u);
    set_bkg_tile_xy(10, 0, 32u);
    set_bkg_tile_xy(11, 0, 32u);
    set_bkg_tile_xy(12, 0, 32u);
    set_bkg_tile_xy(13, 0, 32u);
    set_bkg_tile_xy(14, 0, 32u);
    set_bkg_tile_xy(15, 0, 32u);
    set_bkg_tile_xy(16, 0, 32u);
    set_bkg_tile_xy(17, 0, 32u);
    set_bkg_tile_xy(18, 0, 32u);
    if(instruction_blink_countdown < 20){
      instruction_blink_countdown=0;
    }
  }else{
    switch(tetra_game_state){
      case START_GAME:
        PRINT(6, 0, "PRESS DOWN");
      break;
      case TURN_MAKE_DRAGONS:
        PRINT(6, 0, "PRESS START");
      break;
      case TURN_MAKE_DICE:
        PRINT(2, 0, "FIRE: ROLL DICE");
      break;
      case TURN_PICK_DICE:
        if(dado6_info->tetradado_state == DADO_FACE){
          PRINT(4, 0, "PICK DICE");
        }
      break;
      case TURN_PICK_DRAGON:
        if(tetraturn == TURN_PLAYER){
          PRINT(4, 0, "PICK DRAGON");
        }
      break;
      case TURN_BUY_DRAGON:
        if(tetraturn == TURN_PLAYER){
          PRINT(5, 0, "BUY DRAGON");
        }
      break;
    }
  }
}

void winner(TETRA_TURN winner) BANKED{
  SpriteManagerRemoveSprite(dado1);
  SpriteManagerRemoveSprite(dado2);
  SpriteManagerRemoveSprite(dado3);
  SpriteManagerRemoveSprite(dado4);
  SpriteManagerRemoveSprite(dado5);
  SpriteManagerRemoveSprite(dado6);
  tetra_cursor_info->cursor_state = CURSOR_INVISIBLE;
  switch(winner){
    case TURN_PLAYER:
      play_music_missionaccomplished();
      tetra_change_game_state(WINNER);
    break;
    case TURN_CAPTAIN:
      tetra_change_game_state(LOSER);
    break;
  }
}