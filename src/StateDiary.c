#include "Banks/SetAutoBank.h"

#include "SGB.h"
#include "BankManager.h"
#include "Palette.h"
#include "ZGBMain.h"
#include "Keys.h"
#include "Palette.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "string.h"
#include "Print.h"
#include "Fade.h"

#include "custom_datas.h"
#include "TilesAnimations0.h"
#include "sgb_palette.h"
#include "DiaryDialogs.h"

IMPORT_MAP(borderdiary);
IMPORT_TILES(fontbw);
IMPORT_MAP(diarym);

extern UINT8 J_JUMP;
extern UINT8 J_FIRE;

const UINT8 collision_tiles_diary[] = {1, 2, 0};
extern UINT8 border_set_diary;
extern UINT8 border_set_ow;
extern UINT8 current_map;//0=south-west, 1=south-east, 2=north-west, 3=north-east
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
extern INT8 chapter;

extern struct MISSION find_blackie;
extern struct MISSION enable_hospital;
extern struct MISSION engage_smith;
extern struct MISSION help_cemetery_woman;

UINT8 cursor_posx[] = {4u, 4u, 4u, 4u};// , 12u, 132u};
UINT8 cursor_posy[] = {12u, 36u, 60u, 84u};//, 116u, 116u};
INT8 cursor_posi = 0u;
UINT8 cursor_old_posi = 0u;
const INT8 cursor_posimax = 4;//6
Sprite* diary_cursor = 0;
Sprite* dado0 = 0;
extern Sprite* dado1;
extern Sprite* dado2;
extern Sprite* dado3;
UINT8 idx_mission = 0u;
INT8 idx_page = 0u;
UINT8 showing_detail = 0u;

void empty_ms();
void empty_dds();
void show_missions();
void show_detail();
void change_page(INT8 inc);
void show_pcodes();

extern void ChangeStateThroughBetween(UINT8 new_state, UINT8 previous_state) BANKED;

void START(){
    //HIDE_WIN;
        LOAD_SGB_BORDER(borderdiary);
	//SOUND
        NR52_REG = 0x80; //Enables sound, you should always setup this first
        NR51_REG = 0xFF; //Enables all channels (left and right)
        NR50_REG = 0x77; //Max volume
        //PlayMusic(bgm_credits, 0);    
    //INIT GRAPHICS
        diary_cursor = SpriteManagerAdd(SpriteDiarycursor, 24u, 24u);
        InitScroll(BANK(diarym), &diarym, collision_tiles_diary, 0);
        scroll_target = SpriteManagerAdd(SpriteCamerafocus, (UINT16) 10u << 3, (UINT16) 9u << 3);
        INIT_FONT(fontbw, PRINT_BKG);
        show_pcodes();
        SHOW_BKG;
    //INIT VARS
        cursor_old_posi = cursor_posi;
        diary_cursor->x = cursor_posx[cursor_old_posi];
        diary_cursor->y = cursor_posy[cursor_old_posi];
        idx_page = chapter;
        show_missions();
        showing_detail = 0u;
    
	SHOW_SPRITES;
}

void show_pcodes(){
    SpriteManagerRemoveSprite(dado0);
    SpriteManagerRemoveSprite(dado1);
    SpriteManagerRemoveSprite(dado2);
    SpriteManagerRemoveSprite(dado3);
    dado0 = SpriteManagerAdd(SpriteTetradado, 52u, 104u);
    struct TetradadoInfo* dado0_info = (struct TetradadoInfo*) dado0->custom_data;
    dado0_info->tetradado_state = DADO_WAITING;
    dado1 = SpriteManagerAdd(SpriteTetradado, 66u, 104u);
    struct TetradadoInfo* dado1_info = (struct TetradadoInfo*) dado1->custom_data;
    dado1_info->tetradado_state = DADO_WAITING;
    dado2 = SpriteManagerAdd(SpriteTetradado, 80u, 104u);
    struct TetradadoInfo* dado2_info = (struct TetradadoInfo*) dado2->custom_data;
    dado2_info->tetradado_state = DADO_WAITING;
    dado3 = SpriteManagerAdd(SpriteTetradado, 94u, 104u);
    struct TetradadoInfo* dado3_info = (struct TetradadoInfo*) dado3->custom_data;
    dado3_info->tetradado_state = DADO_WAITING;
    if(idx_page < chapter){//show just the pwd for completed chapters!
        switch(idx_page){
            case 0u:
                dado0_info->tetradado_state = PASSWORD;
                dado1_info->tetradado_state = PASSWORD;
                dado2_info->tetradado_state = PASSWORD;
                dado3_info->tetradado_state = PASSWORD;
                dado0_info->tetradado_faccia = FACCIA_4;
                dado1_info->tetradado_faccia = FACCIA_1;
                dado2_info->tetradado_faccia = FACCIA_4;
                dado3_info->tetradado_faccia = FACCIA_1;
            break;
        }
    }else{

    }
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
    switch(idx_page){
        case 0u:
            switch(cursor_posi){
                case 0u:
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
                case 1u:
                    if(engage_smith.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(ENGAGE_SMITH_D0, dd2);
                        GetLocalizedDDLabel_EN(ENGAGE_SMITH_D1, dd3);
                        if(engage_smith.mission_state >= MISSION_STATE_ACCOMPLISHED){
                            GetLocalizedDDLabel_EN(ENGAGE_SMITH_D2, dd4);
                            GetLocalizedDDLabel_EN(ENGAGE_SMITH_D3, dd5);
                        }
                    }
                break;
                case 2u:
                    if(enable_hospital.mission_state >= MISSION_STATE_ENABLED){
                        GetLocalizedDDLabel_EN(ENABLE_HOSPITAL_D0, dd2);
                        GetLocalizedDDLabel_EN(ENABLE_HOSPITAL_D1, dd3);
                        if(enable_hospital.mission_state >= MISSION_STATE_REWARDED){
                            GetLocalizedDDLabel_EN(ENABLE_HOSPITAL_D2, dd4);
                            GetLocalizedDDLabel_EN(ENABLE_HOSPITAL_D3, dd5);
                        }
                        if(enable_hospital.mission_state >= MISSION_STATE_ACCOMPLISHED){
                            GetLocalizedDDLabel_EN(ENABLE_HOSPITAL_D4, dd4);
                            GetLocalizedDDLabel_EN(ENABLE_HOSPITAL_D5, dd5);
                        }
                    }
                break;
            }
        break;
    }
}

void show_missions(){
    empty_ms();
    empty_dds();
    if(idx_page == chapter){
        GetLocalizedDDLabel_EN(MISSIONI_IN_CORSO, dd1);
    }else if (idx_page < chapter){
        GetLocalizedDDLabel_EN(MISSIONI_COMPLETATE, dd1);
    }else{ 
        GetLocalizedDDLabel_EN(EMPTY_STRING, dd1);
    }
	PRINT(8, 0, dd1);
    switch(idx_page){
        case 0u:
            if(find_blackie.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(FIND_BLACKIE_TITLE, m0);
            }
            if(engage_smith.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(ENGAGE_SMITH_TITLE, m1);
            }
            if(enable_hospital.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(ENABLE_HOSPITAL_TITLE, m2);
            }
        break;
        case 1u:
            if(help_cemetery_woman.mission_state >= MISSION_STATE_ENABLED){
                GetLocalizedDDLabel_EN(HELP_DESPARATE_WIDOW_TITLE, m0);
            }
        break;
    }
    PRINT(2, 2, m0);
    PRINT(2, 5, m1);
    PRINT(2, 8, m2);
    PRINT(2, 11, m3);
    
	PRINT(0, 0, "%i:%u", idx_page+1, chapter+1);
}

void change_page(INT8 inc){
    idx_page+=inc;
    if(showing_detail == 0u){//sto mostrando elenco missioni
        if(idx_page<0){idx_page = chapter;}
        else{idx_page %= (chapter+1);}
    }
    show_pcodes();
}

void UPDATE(){
    if(KEY_RELEASED(J_START)){
        border_set_diary = 0u;
        ChangeStateThroughBetween(StateOverworld, StateDiary);
    }
    if(showing_detail == 0u){
        if (scroll_target->x > (UINT16) 10u << 3){
            scroll_target->x-=3;
        }else{
            show_missions();
        }
        if(KEY_TICKED(J_A) || KEY_TICKED(J_B)){
            if(cursor_posi < 4){
                show_detail();
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
            PRINT(20, 15, dd11);
            PRINT(20, 17, dd12);
            PRINT(20, 18, dd13);
        }
        if(KEY_TICKED(J_A) || KEY_TICKED(J_B)){
            showing_detail = 0u;
        }
    }
}