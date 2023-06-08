#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "custom_datas.h"
#include "Dialogs.h"

IMPORT_TILES(tiles);
IMPORT_TILES(tiles4);
IMPORT_TILES(tiles6);
IMPORT_TILES(tiles7);
IMPORT_TILES(tilesanims);
IMPORT_TILES(tilesanimsmapworld);
IMPORT_TILES(invdetail0tiles);
IMPORT_TILES(invdetailmoneytiles);
IMPORT_TILES(idpowdertiles);
IMPORT_TILES(idlettertiles);
IMPORT_TILES(invdetailwoodtiles);
IMPORT_TILES(invdetailmetaltiles);
IMPORT_TILES(idnormaltiles);
IMPORT_TILES(inventorytiles);
//IMPORT_TILES(tilesanimscutscene);
IMPORT_TILES(tilescredit);
IMPORT_TILES(tilesanimcredit);
IMPORT_TILES(tilesdiagcrossbow);
IMPORT_TILES(tdiagcrossbowempty);
IMPORT_TILES(dialogtiles00);
IMPORT_TILES(dialogtiles01);
IMPORT_TILES(dialogmapmine);
IMPORT_TILES(dmapblackiecave);
IMPORT_TILES(dialogmapsmith);
IMPORT_TILES(dialogmapintro);
IMPORT_TILES(cavetiles);
IMPORT_TILES(cavetilesanim);
IMPORT_TILES(titlescreentiles);
IMPORT_TILES(titlescreentilesanim);
IMPORT_TILES(titlescreentilesanim2);
IMPORT_TILES(titlescreentilesanim3);

extern WHOSTALKING whostalking;
extern UINT8 previous_state;


void set_inv_bkg_data(UINT8 item, UINT8 first_tile, UINT8 nb_tiles, 
	UINT8 bank, UINT8 isEmpty, struct TilesInfo* t) NONBANKED {
    uint8_t save = _current_bank;
    SWITCH_ROM(bank);
	if(isEmpty){
		set_bkg_data(first_tile, nb_tiles, t->data+((16u) * first_tile)); 
	} else { 
		set_bkg_data(first_tile, nb_tiles, t->data);
	}
    SWITCH_ROM(save);
}

void set_banked_bkg_data(UINT8 first_tile, UINT8 nb_tiles, struct TilesInfo* t, UINT8 bank) NONBANKED {
    uint8_t save = _current_bank;
    SWITCH_ROM(bank);
    set_bkg_data(first_tile, nb_tiles, t->data+((16u) * first_tile));
	SWITCH_ROM(save);
}

void dialog_map() BANKED{
	switch(previous_state){
		case StateExzoo:
			set_banked_bkg_data(0, 101u, &dialogtiles00, BANK(dialogtiles00));
		break;
		case StateCemetery:
			set_banked_bkg_data(0, 101u, &dialogtiles01, BANK(dialogtiles01));
		break;
		case StateMine:
			set_banked_bkg_data(0, 101u, &dialogmapmine, BANK(dialogmapmine));
		break;
		case StateBlackiecave:
			set_banked_bkg_data(0, 101u, &dmapblackiecave, BANK(dmapblackiecave));
		break;
		case StateOverworld:
			if(whostalking == INTRO){
				set_banked_bkg_data(0, 101u, &dialogmapintro, BANK(dialogmapintro));
			}
		break;
	}
	switch(current_state){
		case StateSmith:
			set_banked_bkg_data(0, 101u, &dialogmapsmith, BANK(dialogmapsmith));
		break;
	}
}

void Inv_change_detail(UINT8 item, UINT8 isEmpty) BANKED{
	if(isEmpty){
		set_inv_bkg_data(item, 49u, 25, BANK(inventorytiles), isEmpty, &inventorytiles);
	}else{
		switch(item){
			case INVITEM_HEARTS:
				set_inv_bkg_data(item, 49u, 25, BANK(invdetail0tiles), isEmpty, &invdetail0tiles);
			break;
			case INVITEM_MONEY:
				set_inv_bkg_data(item, 49u, 25, BANK(invdetailmoneytiles), isEmpty, &invdetailmoneytiles);
			break;
			case INVITEM_POWDER:
				set_inv_bkg_data(item, 49u, 25, BANK(idpowdertiles), isEmpty, &idpowdertiles);
			break;			
			case INVITEM_LETTER:
				set_inv_bkg_data(item, 49u, 25, BANK(idlettertiles), isEmpty, &idlettertiles);
			break;
			case INVITEM_WOOD:
				set_inv_bkg_data(item, 49u, 25, BANK(invdetailwoodtiles), isEmpty, &invdetailwoodtiles);
			break;
			case INVITEM_METAL:
				set_inv_bkg_data(item, 49u, 25, BANK(invdetailmetaltiles), isEmpty, &invdetailmetaltiles);
			break;
			case INVITEM_ARROW_NORMAL:
				set_inv_bkg_data(item, 49u, 25, BANK(idnormaltiles), isEmpty, &idnormaltiles);
			break;
		}
	}
}

void Anim_Titlescreen_0() BANKED{
	set_banked_bkg_data(122u, 8u, &titlescreentiles, BANK(titlescreentiles));//ship
	set_banked_bkg_data(108u, 3u, &titlescreentiles, BANK(titlescreentiles));//water
	set_banked_bkg_data(98u, 2u, &titlescreentiles, BANK(titlescreentiles));//water
}
void Anim_Titlescreen_1() BANKED{
	set_banked_bkg_data(122u, 8u, &titlescreentilesanim, BANK(titlescreentilesanim));//ship
	set_banked_bkg_data(98u, 2u, &titlescreentilesanim, BANK(titlescreentilesanim));//water
}
void Anim_Titlescreen_2() BANKED{
	set_banked_bkg_data(108u, 3u, &titlescreentilesanim2, BANK(titlescreentilesanim2));//water
	set_banked_bkg_data(98u, 2u, &titlescreentilesanim2, BANK(titlescreentilesanim2));//water
}
void Anim_Titlescreen_3() BANKED{
	set_banked_bkg_data(108u, 3u, &titlescreentilesanim3, BANK(titlescreentilesanim3));//water
	set_banked_bkg_data(98u, 2u, &titlescreentilesanim3, BANK(titlescreentilesanim3));//water
}

void Anim_Cave_0() BANKED{
	set_banked_bkg_data(29u, 2u, &cavetilesanim, BANK(cavetilesanim));//fiammella
	set_banked_bkg_data(41u, 7u, &cavetilesanim, BANK(cavetilesanim));//luce lanterna
	set_banked_bkg_data(60u, 2u, &cavetilesanim, BANK(cavetilesanim));//carrello
}
void Anim_Cave_1() BANKED{
	set_banked_bkg_data(29u, 2u, &cavetiles, BANK(cavetiles));//fiammella
	set_banked_bkg_data(41u, 7u, &cavetiles, BANK(cavetiles));//luce lanterna
	set_banked_bkg_data(60u, 2u, &cavetiles, BANK(cavetiles));//carrello
}

void Anim_StudioLoading_0() BANKED{
	set_banked_bkg_data(14u, 1u, &tilescredit, BANK(tilescredit));
	set_banked_bkg_data(15u, 1u, &tilescredit, BANK(tilescredit));
}
void Anim_StudioLoading_1() BANKED{
	set_banked_bkg_data(14u, 1u, &tilesanimcredit, BANK(tilesanimcredit));
	set_banked_bkg_data(15u, 1u, &tilesanimcredit, BANK(tilesanimcredit));
}
//set_bkg_data
void Anim_Crossbow_0() BANKED{
	set_banked_bkg_data(0u, 67u, &tilesdiagcrossbow, BANK(tilesdiagcrossbow));
}
void Anim_Crossbow_1() BANKED{
	set_banked_bkg_data(0u, 67u, &tdiagcrossbowempty, BANK(tdiagcrossbowempty));
}

void Anim_Ground_Straight_0() BANKED{
	set_banked_bkg_data(68u, 1u, &tilesdiagcrossbow, BANK(tilesdiagcrossbow));
	set_banked_bkg_data(69u, 1u, &tilesdiagcrossbow, BANK(tilesdiagcrossbow));
	set_banked_bkg_data(75u, 1u, &tilesdiagcrossbow, BANK(tilesdiagcrossbow));
}
void Anim_Ground_Straight_1() BANKED{
	set_banked_bkg_data(68u, 1u, &tilesdiagcrossbow, BANK(tilesdiagcrossbow));
	set_banked_bkg_data(69u, 1u, &tilesdiagcrossbow, BANK(tilesdiagcrossbow));
	set_banked_bkg_data(75u, 1u, &tilesdiagcrossbow, BANK(tilesdiagcrossbow));
}

void Anim_Tiles_0() BANKED{
}

void Anim_Tiles_1() BANKED{
	
}

void WorldmapQuiverStone() BANKED{
	set_banked_bkg_data(44u, 1u, &tilesanimsmapworld, BANK(tilesanimsmapworld));// 16 * 1
}
void WorldmapQuiverThunder() BANKED{
	set_banked_bkg_data(45u, 1u, &tilesanimsmapworld, BANK(tilesanimsmapworld));// 16 * 3
}
void WorldmapQuiverIce() BANKED{
	set_banked_bkg_data(56u, 1u, &tilesanimsmapworld, BANK(tilesanimsmapworld));// 16 * 6
}
void WorldmapQuiverFire() BANKED{
	set_banked_bkg_data(47u, 1u, &tilesanimsmapworld, BANK(tilesanimsmapworld));// 16 * 10
}

void CutsceneAmulet0() BANKED{
	//set_banked_bkg_data(30u, 1u, 5, BANK(tilesanimscutscene));// 16 * 10
}
void CutsceneAmulet1() BANKED{
	//set_banked_bkg_data(31u, 1u, 5, BANK(tilesanimscutscene));// 16 * 10
}
void CutsceneAmulet2() BANKED{
	//set_banked_bkg_data(38u, 1u, 5, BANK(tilesanimscutscene));
}
void CutsceneAmulet3() BANKED{
	//set_banked_bkg_data(39u, 1u, 5, BANK(tilesanimscutscene));
}
void WorldmapFinalFight() BANKED{//116 117 + 118 119
	set_banked_bkg_data(116u, 1u, &tilesanimsmapworld, BANK(tilesanimsmapworld));
	set_banked_bkg_data(117u, 1u, &tilesanimsmapworld, BANK(tilesanimsmapworld));
	set_banked_bkg_data(118u, 1u, &tilesanimsmapworld, BANK(tilesanimsmapworld));
	set_banked_bkg_data(119u, 1u, &tilesanimsmapworld, BANK(tilesanimsmapworld));
}