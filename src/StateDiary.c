#include "Banks/SetAutoBank.h"

#include "BankManager.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"

#include "custom_datas.h"
#include "TAnim0.h"
#include "DiaryDialogs.h"

IMPORT_TILES(fontbw);
IMPORT_MAP(diarym);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 collision_tiles_diary[] = {1, 2, 0};
extern unsigned char m0[17];
extern unsigned char m1[17];
extern unsigned char m2[17];
extern unsigned char m3[17];
extern unsigned char dd1[];
extern unsigned char dd2[];
extern unsigned char dd3[];
extern unsigned char dd4[];
extern unsigned char dd5[];
extern unsigned char dd6[];
extern unsigned char dd7[];
extern unsigned char dd8[];
extern unsigned char dd9[];
extern unsigned char dd10[];
extern unsigned char dd11[];
extern unsigned char dd12[];
extern unsigned char dd13[];
extern CHAPTERS chapter;

extern struct MISSION find_blackie;
extern struct MISSION enable_hospital;
extern struct MISSION engage_smith;
extern struct MISSION help_cemetery_woman;
extern struct MISSION outwalker_chief;
extern struct MISSION outwalker_glass;
extern struct MISSION outwalker_smith;
extern struct MISSION get_to_the_mountain;
extern struct MISSION defeat_scorpions;
extern struct MISSION find_antidote;
extern struct MISSION visit_blackie;
extern struct MISSION hungry_people;
extern struct MISSION fix_bridge;
extern struct MISSION golden_armor;
extern struct MISSION mr_smee;
extern struct MISSION broken_ship;
extern struct MISSION pirate_strike;
extern struct MISSION captain;
extern struct MISSION* missions[21];

UINT8 cursor_posx[] = {4u, 4u, 4u, 4u};// , 12u, 132u};
UINT8 cursor_posy[] = {12u, 36u, 60u, 84u};//, 116u, 116u};
INT8 cursor_posi = 0u;
INT8 cursor_old_posi = 0u;
const INT8 cursor_posimax = 4;//6
Sprite* diary_cursor = 0;
Sprite* dado0 = 0;
extern Sprite* dado1;
extern Sprite* dado2;
extern Sprite* dado3;
UINT8 idx_mission = 0u;
CHAPTERS idx_page = CHAPTER_0_BLACKIE;
UINT8 showing_detail = 0u;
UINT8 showing_missions = 0u;
extern Sprite* s_motherow;

void empty_ms();
void empty_dds();
void show_missions();
void show_detail();
void change_page(INT8 inc);
void show_pcodes();
void update_diary_cursor();

extern void ChangeState(UINT8 new_state, Sprite* s_mother, INT8 next_map) BANKED;

void START(){
    //INIT GRAPHICS
        diary_cursor = SpriteManagerAdd(SpriteDiarycursor, 24u, 24u);
        InitScroll(BANK(diarym), &diarym, collision_tiles_diary, 0);
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 10u << 3, (UINT16) 9u << 3);
        INIT_FONT(fontbw, PRINT_BKG);
        SHOW_BKG;
    //INIT VARS
        cursor_old_posi = cursor_posi;
        diary_cursor->x = cursor_posx[cursor_old_posi];
        diary_cursor->y = cursor_posy[cursor_old_posi];
        idx_page = chapter;
        show_pcodes();
        show_missions();
        showing_detail = 0u;
        update_diary_cursor();
    
	SHOW_SPRITES;
}

void show_pcodes(){
    SpriteManagerRemoveSprite(dado0);
    SpriteManagerRemoveSprite(dado1);
    SpriteManagerRemoveSprite(dado2);
    SpriteManagerRemoveSprite(dado3);
    dado0 = SpriteManagerAdd(SpriteTetradado, 52u, 104u);
    struct TetradadoInfo* dado0_info = (struct TetradadoInfo*) dado0->custom_data;
    dado1 = SpriteManagerAdd(SpriteTetradado, 66u, 104u);
    struct TetradadoInfo* dado1_info = (struct TetradadoInfo*) dado1->custom_data;
    dado2 = SpriteManagerAdd(SpriteTetradado, 80u, 104u);
    struct TetradadoInfo* dado2_info = (struct TetradadoInfo*) dado2->custom_data;
    dado3 = SpriteManagerAdd(SpriteTetradado, 94u, 104u);
    struct TetradadoInfo* dado3_info = (struct TetradadoInfo*) dado3->custom_data;
    if(idx_page == chapter){
        dado0_info->tetradado_state = DADO_WAITING;
        dado1_info->tetradado_state = DADO_WAITING;
        dado2_info->tetradado_state = DADO_WAITING;
        dado3_info->tetradado_state = DADO_WAITING;
    }else {//show just the pwd for completed chapters!
        dado0_info->tetradado_state = PASSWORD;
        dado1_info->tetradado_state = PASSWORD;
        dado2_info->tetradado_state = PASSWORD;
        dado3_info->tetradado_state = PASSWORD;
        switch(idx_page){//evaluated as completed chapter
            case CHAPTER_0_BLACKIE:
                dado0_info->tetradado_faccia = FACCIA_SHIELD;
                dado1_info->tetradado_faccia = FACCIA_UP;
                dado2_info->tetradado_faccia = FACCIA_SHIELD;
                dado3_info->tetradado_faccia = FACCIA_UP;
            break;
            case CHAPTER_1_BANDITS:
                dado0_info->tetradado_faccia = FACCIA_SHIELD;
                dado1_info->tetradado_faccia = FACCIA_BLAIR;
                dado2_info->tetradado_faccia = FACCIA_SHIELD;
                dado3_info->tetradado_faccia = FACCIA_ARROWS;
            break;
            case CHAPTER_2_PLAGUE:
                dado0_info->tetradado_faccia = FACCIA_UP;
                dado1_info->tetradado_faccia = FACCIA_BLAIR;
                dado2_info->tetradado_faccia = FACCIA_SHIELD;
                dado3_info->tetradado_faccia = FACCIA_UP;
            break;
            case CHAPTER_3_ARMOUR:
                dado0_info->tetradado_faccia = FACCIA_ARROWS;
                dado1_info->tetradado_faccia = FACCIA_SHIELD;
                dado2_info->tetradado_faccia = FACCIA_UP;
                dado3_info->tetradado_faccia = FACCIA_ARROWS;
            break;
        }
    }
    PRINT(0, 17, "DIARY");
}

void empty_ms(){
    GetLocalizedMLabel_EN(EMPTY_STRING, m0);
    GetLocalizedMLabel_EN(EMPTY_STRING, m1);
    GetLocalizedMLabel_EN(EMPTY_STRING, m2);
    GetLocalizedMLabel_EN(EMPTY_STRING, m3);
}

void empty_dds(){
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd1);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd2);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd3);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd4);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd5);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd6);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd7);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd8);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd9);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd10);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd11);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd12);
    GetLocalizedDDLabel_EN(EMPTY_STRING, dd13);
}

void show_detail(){
    empty_dds();
    empty_ms();
    showing_detail = 1u;
    showing_missions = 0u;
    switch(idx_page){
        case CHAPTER_0_BLACKIE:
            switch(cursor_posi){
                case 0u:
                    if(engage_smith.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(ENGAGE_SMITH_D0, dd2);
                        GetLocalizedDDLabel_EN(ENGAGE_SMITH_D1, dd3);
                        if(engage_smith.current_step > 0){
                            GetLocalizedDDLabel_EN(ENGAGE_SMITH_D2, dd4);
                            GetLocalizedDDLabel_EN(ENGAGE_SMITH_D3, dd5);
                            GetLocalizedDDLabel_EN(ENGAGE_SMITH_D4, dd6);
                            GetLocalizedDDLabel_EN(ENGAGE_SMITH_D5, dd7);
                        }
                        if(engage_smith.mission_state == MISSION_STATE_REWARDED){
                            GetLocalizedDDLabel_EN(ENGAGE_SMITH_D6, dd6);
                            GetLocalizedDDLabel_EN(ENGAGE_SMITH_D7, dd7);
                        }
                    }
                break;
                case 1u:
                    if(enable_hospital.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(ENABLE_HOSPITAL_D0, dd2);
                        GetLocalizedDDLabel_EN(ENABLE_HOSPITAL_D1, dd3);
                        if(enable_hospital.mission_state >= MISSION_STATE_ACCOMPLISHED){
                            GetLocalizedDDLabel_EN(ENABLE_HOSPITAL_D2, dd4);
                            GetLocalizedDDLabel_EN(ENABLE_HOSPITAL_D3, dd5);
                        }
                        if(enable_hospital.mission_state >= MISSION_STATE_REWARDED){
                            GetLocalizedDDLabel_EN(ENABLE_HOSPITAL_D4, dd6);
                            GetLocalizedDDLabel_EN(ENABLE_HOSPITAL_D5, dd7);
                        }
                    }
                break;
                case 2u:
                    GetLocalizedDDLabel_EN(FIND_BLACKIE_D0, dd2);    
                    GetLocalizedDDLabel_EN(FIND_BLACKIE_D1, dd3);
                    GetLocalizedDDLabel_EN(FIND_BLACKIE_D2, dd4); 
                    GetLocalizedDDLabel_EN(FIND_BLACKIE_D3, dd5);
                    if(find_blackie.current_step >= 1){
                        GetLocalizedDDLabel_EN(FIND_BLACKIE_D4, dd6); 
                        GetLocalizedDDLabel_EN(FIND_BLACKIE_D5, dd7);
                    }
                    if(find_blackie.current_step >= 5){
                        GetLocalizedDDLabel_EN(FIND_BLACKIE_D6, dd8); 
                        GetLocalizedDDLabel_EN(FIND_BLACKIE_D7, dd9);
                    }
                break;
                case 3u:
                    if(help_cemetery_woman.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(WIDOW_D0, dd2);
                        GetLocalizedDDLabel_EN(WIDOW_D1, dd3);
                        if(help_cemetery_woman.mission_state >= MISSION_STATE_STARTED){
                            GetLocalizedDDLabel_EN(WIDOW_D2, dd4);
                            GetLocalizedDDLabel_EN(WIDOW_D3, dd5);
                            if(help_cemetery_woman.current_step >= 2u){
                                GetLocalizedDDLabel_EN(WIDOW_D4, dd6);
                                GetLocalizedDDLabel_EN(WIDOW_D5, dd7);
                            }
                            if(help_cemetery_woman.mission_state == MISSION_STATE_REWARDED){                            
                                GetLocalizedDDLabel_EN(WIDOW_D6, dd8);
                                GetLocalizedDDLabel_EN(WIDOW_D7, dd9);
                            }
                        }
                    }
                break;
            }
        break;
        case CHAPTER_1_BANDITS:
            switch(cursor_posi){
                case 0u:
                    if(outwalker_chief.mission_state >= MISSION_STATE_ENABLED){
                        if(outwalker_chief.current_step >= 1){
                            GetLocalizedDDLabel_EN(CHIEF_D0, dd2);
                            GetLocalizedDDLabel_EN(CHIEF_D1, dd3);
                            if(outwalker_chief.current_step >= 2u){
                                GetLocalizedDDLabel_EN(CHIEF_D2, dd4);
                                GetLocalizedDDLabel_EN(CHIEF_D3, dd5);
                                if(outwalker_chief.current_step >= 3u){
                                    GetLocalizedDDLabel_EN(CHIEF_D4, dd6);
                                    GetLocalizedDDLabel_EN(CHIEF_D5, dd7);
                                }   
                            }
                        }
                    }
                break;
                case 1u:
                    if(outwalker_glass.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(GLASS_D0, dd2);
                        GetLocalizedDDLabel_EN(GLASS_D1, dd3);
                        if(outwalker_glass.current_step >= 2u){
                            GetLocalizedDDLabel_EN(GLASS_D2, dd4);
                            GetLocalizedDDLabel_EN(GLASS_D3, dd5);
                            if(outwalker_glass.current_step == 4u){
                                GetLocalizedDDLabel_EN(GLASS_D4, dd6);
                                GetLocalizedDDLabel_EN(GLASS_D5, dd7);
                            }   
                        }
                    }
                break;
                case 2u:
                    if(outwalker_smith.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(SMITH_D0, dd2);
                        GetLocalizedDDLabel_EN(SMITH_D1, dd3);
                        if(outwalker_smith.mission_state == MISSION_STATE_STARTED){
                                GetLocalizedDDLabel_EN(SMITH_D2, dd4);
                                GetLocalizedDDLabel_EN(SMITH_D3, dd5);
                            if(outwalker_smith.current_step == 0b00001111){
                                GetLocalizedDDLabel_EN(SMITH_D4, dd6);
                                GetLocalizedDDLabel_EN(SMITH_D5, dd7);
                            }
                        }
                    }
                break;
                case 3u:                
                    if(get_to_the_mountain.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(MOUNTAIN_D0, dd2);
                        GetLocalizedDDLabel_EN(MOUNTAIN_D1, dd3);
                        GetLocalizedDDLabel_EN(MOUNTAIN_D2, dd4);
                        GetLocalizedDDLabel_EN(MOUNTAIN_D3, dd5);
                        GetLocalizedDDLabel_EN(EMPTY_STRING, dd6);
                        GetLocalizedDDLabel_EN(EMPTY_STRING, dd7);                          
                        GetLocalizedDDLabel_EN(EMPTY_STRING, dd8);
                        GetLocalizedDDLabel_EN(EMPTY_STRING, dd9);
                    }
                break;
            }
        break;
        case CHAPTER_2_PLAGUE:
            switch(cursor_posi){
                case 0u:
                    if(defeat_scorpions.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(SCORPIONS_D0, dd2);
                        GetLocalizedDDLabel_EN(SCORPIONS_D1, dd3);
                        if(defeat_scorpions.phase >= 1u){
                            GetLocalizedDDLabel_EN(SCORPIONS_D2, dd4);
                            GetLocalizedDDLabel_EN(SCORPIONS_D3, dd5);
                        }
                    }
                break;
                case 1u:                    
                    if(find_antidote.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(ANTIDOTE_D0, dd2);
                        GetLocalizedDDLabel_EN(ANTIDOTE_D1, dd3);
                        if(find_antidote.phase >= 1){
                            GetLocalizedDDLabel_EN(ANTIDOTE_D2, dd4);
                            GetLocalizedDDLabel_EN(ANTIDOTE_D3, dd5);
                        }
                        if(find_antidote.phase >= 2){
                            GetLocalizedDDLabel_EN(ANTIDOTE_D4, dd6);
                            GetLocalizedDDLabel_EN(ANTIDOTE_D5, dd7);
                        }
                        if(find_antidote.phase >= 3){
                            GetLocalizedDDLabel_EN(ANTIDOTE_D6, dd8);
                        }
                        if(find_antidote.phase >= 5){
                            GetLocalizedDDLabel_EN(ANTIDOTE_D7, dd9);
                        }
                    }
                break;
                case 2u:                    
                    if(visit_blackie.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(VISIT_BLACKIE_D0, dd2);
                        GetLocalizedDDLabel_EN(VISIT_BLACKIE_D1, dd3);
                        if(visit_blackie.mission_state == MISSION_STATE_REWARDED){
                            GetLocalizedDDLabel_EN(VISIT_BLACKIE_D2, dd4);
                            GetLocalizedDDLabel_EN(VISIT_BLACKIE_D3, dd5);
                        }
                    }
                break;
            }
        break;
        case CHAPTER_3_ARMOUR:
            switch(cursor_posi){
                case 0u:
                    if(hungry_people.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(HUNGRY_D0, dd2);
                        GetLocalizedDDLabel_EN(HUNGRY_D1, dd3);
                        if(hungry_people.mission_state >= MISSION_STATE_ACCOMPLISHED){
                            GetLocalizedDDLabel_EN(HUNGRY_D2, dd4);
                            GetLocalizedDDLabel_EN(HUNGRY_D3, dd5);
                        }
                    }
                break;
                case 1u:
                    if(fix_bridge.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(FIX_BRIDGE_D0, dd2);
                        GetLocalizedDDLabel_EN(FIX_BRIDGE_D1, dd3);
                        if(fix_bridge.mission_state >= MISSION_STATE_STARTED){
                            GetLocalizedDDLabel_EN(FIX_BRIDGE_D2, dd4);
                            GetLocalizedDDLabel_EN(FIX_BRIDGE_D3, dd5);
                            if(fix_bridge.mission_state >= MISSION_STATE_ACCOMPLISHED){
                                GetLocalizedDDLabel_EN(FIX_BRIDGE_D4, dd6);
                            }
                        }
                    }
                break;
                case 2u:
                    if(golden_armor.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(GOLDEN_D0, dd2);
                        GetLocalizedDDLabel_EN(GOLDEN_D1, dd3);
                        if(golden_armor.phase >= 1){
                            GetLocalizedDDLabel_EN(GOLDEN_D2, dd4);
                            GetLocalizedDDLabel_EN(GOLDEN_D3, dd5);
                            GetLocalizedDDLabel_EN(GOLDEN_D4, dd6);
                            GetLocalizedDDLabel_EN(GOLDEN_D5, dd7);
                            if(golden_armor.mission_state >= MISSION_STATE_ACCOMPLISHED){
                                GetLocalizedDDLabel_EN(GOLDEN_D2, dd2);
                                GetLocalizedDDLabel_EN(GOLDEN_D3, dd3);
                                GetLocalizedDDLabel_EN(GOLDEN_D4, dd4);
                                GetLocalizedDDLabel_EN(GOLDEN_D5, dd5);
                                GetLocalizedDDLabel_EN(GOLDEN_D6, dd6);
                                GetLocalizedDDLabel_EN(GOLDEN_D7, dd7);
                                if(golden_armor.mission_state == MISSION_STATE_REWARDED){
                                    GetLocalizedDDLabel_EN(GOLDEN_D4, dd2);
                                    GetLocalizedDDLabel_EN(GOLDEN_D5, dd3);
                                    GetLocalizedDDLabel_EN(GOLDEN_D6, dd4);
                                    GetLocalizedDDLabel_EN(GOLDEN_D7, dd5);
                                    GetLocalizedDDLabel_EN(GOLDEN_D8, dd6);
                                    GetLocalizedDDLabel_EN(GOLDEN_D9, dd7);
                                }
                            }
                        }
                    }
                break;
            }
        break;
        case CHAPTER_4_SHIP:
            switch(cursor_posi){
                case 0u:
                    if(mr_smee.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(MRSMEE_D0, dd2);
                        GetLocalizedDDLabel_EN(MRSMEE_D1, dd3);
                        if(mr_smee.mission_state >= MISSION_STATE_ACCOMPLISHED){
                            GetLocalizedDDLabel_EN(MRSMEE_D2, dd4);
                            GetLocalizedDDLabel_EN(MRSMEE_D3, dd5);
                            if(mr_smee.mission_state >= MISSION_STATE_REWARDED){
                                GetLocalizedDDLabel_EN(MRSMEE_D4, dd6);
                                GetLocalizedDDLabel_EN(MRSMEE_D5, dd7);
                            }
                        }
                    }
                break;
                case 1u:
                    if(broken_ship.mission_state >= MISSION_STATE_ENABLED){
                            GetLocalizedDDLabel_EN(BROKENSHIP_D0, dd2);
                            GetLocalizedDDLabel_EN(BROKENSHIP_D1, dd3);
                        if(broken_ship.mission_state >= MISSION_STATE_ACCOMPLISHED){
                            GetLocalizedDDLabel_EN(BROKENSHIP_D2, dd4);
                            GetLocalizedDDLabel_EN(BROKENSHIP_D3, dd5);
                        }
                    }
                break;
                case 2u:
                    if(pirate_strike.mission_state >= MISSION_STATE_ENABLED){
                            GetLocalizedDDLabel_EN(STRIKE_D0, dd2);
                            GetLocalizedDDLabel_EN(STRIKE_D1, dd3);
                        if(pirate_strike.mission_state >= MISSION_STATE_ACCOMPLISHED){
                            GetLocalizedDDLabel_EN(STRIKE_D2, dd4);
                            GetLocalizedDDLabel_EN(STRIKE_D3, dd5);
                            if(pirate_strike.mission_state == MISSION_STATE_REWARDED){
                                GetLocalizedDDLabel_EN(STRIKE_D4, dd6);
                                GetLocalizedDDLabel_EN(STRIKE_D5, dd7);
                            }
                        }
                    }
                break;
                case 3u:
                    if(captain.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(CAPTAIN_D0, dd2);
                        GetLocalizedDDLabel_EN(CAPTAIN_D1, dd3);
                    }
                break;
            }
        break;
    }
}

void show_missions(){
    empty_ms();
    empty_dds();
    switch(idx_page){
        case CHAPTER_0_BLACKIE:
            if(engage_smith.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(ENGAGE_SMITH_TITLE, m0);
            }
            if(enable_hospital.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(ENABLE_HOSPITAL_TITLE, m1);
            }
            if(find_blackie.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(FIND_BLACKIE_TITLE, m2);
            }
            if(help_cemetery_woman.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(HELP_DESPARATE_WIDOW_TITLE, m3);
            }
        break;
        case CHAPTER_1_BANDITS:
            if(outwalker_chief.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(OUTWALKER_CHIEF_TITLE, m0);
            }
            if(outwalker_glass.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(OUTWALKER_GLASS_TITLE, m1);
            }
            if(outwalker_smith.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(OUTWALKER_SMITH_TITLE, m2);
            }
            if(get_to_the_mountain.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(GET_TO_THE_MOUNTAIN_TITLE, m3);
            }
        break;
        case CHAPTER_2_PLAGUE:
            if(defeat_scorpions.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(DEFEAT_SCORPIONS_TITLE, m0);
            }
            if(find_antidote.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(FIND_ANTIDOTE_TITLE, m1);
            }
            if(visit_blackie.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(VISIT_BLACKIE_TITLE, m2);
            }
        break;
        case CHAPTER_3_ARMOUR:
            if(hungry_people.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(HUNGRY_TITLE, m0);
            }
            if(fix_bridge.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(FIX_BRIDGE_TITLE, m1);
            }
            if(golden_armor.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(GOLDEN_ARMOR_TITLE, m2);
            }
        break;
        case CHAPTER_4_SHIP:
            if(mr_smee.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(MRSMEE_TITLE, m0);
            }
            if(broken_ship.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(BROKENSHIP_TITLE, m1);
            }
            if(pirate_strike.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(STRIKE_TITLE, m2);
            }
            if(captain.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(CAPTAIN_TITLE, m3);
            }
        break;
    }
    PRINT(2, 2, m0);
    PRINT(2, 5, m1);
    PRINT(2, 8, m2);
    PRINT(2, 11, m3);
    //PRINT(0, 0, "%i:%u", idx_page+1, chapter+1);
    update_diary_cursor();
}

void change_page(INT8 inc){
    idx_page+=inc;
    if(showing_detail == 0u){//sto mostrando elenco missioni
        //if(idx_page<0){idx_page = chapter;}
        //else{
            idx_page %= (chapter+1);
        //}
    }
    show_pcodes();
    cursor_posi = 0;
    show_missions();
    update_diary_cursor();
}

void UPDATE(){
    if(KEY_RELEASED(J_START) || KEY_RELEASED(J_SELECT)){
        ChangeState(StateOw, s_motherow, -1);
    }
    if(showing_detail == 0u){
        if (scroll_target->x > (UINT16) 10u << 3){
            scroll_target->x-=3;
        }else if(showing_missions == 0){
            show_missions();
            showing_missions = 1;
        }
        if(KEY_TICKED(J_FIRE) || KEY_TICKED(J_JUMP)){
            if(cursor_posi < 4){
                INT8 missions_idx = cursor_posi + (idx_page * 4);
                if(missions[missions_idx]->mission_state > MISSION_STATE_DISABLED){
                    show_detail();
                }
            }else if (cursor_posi == 4){//Left cursor selected
                change_page(-1);
            }else if (cursor_posi == 5){//Left cursor selected
                change_page(1);
            }
        }
        if(KEY_TICKED(J_LEFT)){
            change_page(-1);
        }
        if(KEY_TICKED(J_RIGHT)){
            change_page(1);
        }
        if(KEY_RELEASED(J_DOWN)){
            cursor_posi++;
        }
        if(KEY_RELEASED(J_UP)){
            cursor_posi--;
        }
        if(cursor_posi < 0){
            cursor_posi = cursor_posimax - 1;
        }
        if(cursor_posi >= cursor_posimax){
            cursor_posi = 0;
        }
        if(cursor_old_posi != cursor_posi){//muovo cursor verso prossima posizione
            cursor_old_posi = cursor_posi;
            diary_cursor->x = cursor_posx[cursor_posi];
            diary_cursor->y = cursor_posy[cursor_posi];
            update_diary_cursor();
        }
    }else{
        if(scroll_target->x < (UINT16) 29u << 3){
            scroll_target->x+=3;
        }else{            
            PRINT(20, 2, dd2);
            PRINT(20, 3, dd3);
            PRINT(20, 5, dd4);
            PRINT(20, 6, dd5);
            PRINT(20, 8, dd6);
            PRINT(20, 9, dd7);
            PRINT(20, 11, dd8);
            PRINT(20, 12, dd9);
            PRINT(20, 14, dd10);
            //PRINT(20, 15, dd11);
            //PRINT(20, 17, dd12);
            //PRINT(20, 18, dd13);
        }
        if(KEY_TICKED(J_FIRE) || KEY_TICKED(J_JUMP)){
            showing_detail = 0u;
            show_missions();
        }
    }
}

void update_diary_cursor(){
    struct EnemyData* dcursor_data = (struct EnemyData*) diary_cursor->custom_data;
    INT8 missions_idx = cursor_posi + (idx_page * 4);
    if(missions_idx == -1){missions_idx = 3;}
    if(missions[missions_idx] == 0){dcursor_data->configured = 2;
    }else if(missions[missions_idx]->mission_state == MISSION_STATE_REWARDED){
        dcursor_data->configured = 1;
        GetLocalizedDDLabel_EN(MISSIONI_COMPLETATE, dd1);
    }else if(missions[missions_idx]->mission_state > MISSION_STATE_DISABLED){                
        dcursor_data->configured = 0;
        GetLocalizedDDLabel_EN(MISSIONI_IN_CORSO, dd1);
    }else{
        dcursor_data->configured = 2;
        GetLocalizedDDLabel_EN(EMPTY_STRING, dd1);
    }
    dcursor_data->wait = 1;
    PRINT(0, 0, dd1);      
}