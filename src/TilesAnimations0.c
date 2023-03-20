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


void set_inv_bkg_data(UINT8 item, UINT8 first_tile, UINT8 nb_tiles, UINT8 bank, UINT8 isEmpty) NONBANKED {
    uint8_t save = _current_bank;
    SWITCH_ROM(bank);
	if(isEmpty){
		set_bkg_data(first_tile, nb_tiles, inventorytiles.data+((16u) * first_tile)); 
	} else { 
		switch(item){
			case INVITEM_CROSSBOW:set_bkg_data(first_tile, nb_tiles, invdetail0tiles.data);break;
			case INVITEM_MONEY:set_bkg_data(first_tile, nb_tiles, invdetailmoneytiles.data);break;
			case INVITEM_POWDER:set_bkg_data(first_tile, nb_tiles, idpowdertiles.data);break;
			case INVITEM_WOOD:set_bkg_data(first_tile, nb_tiles, invdetailwoodtiles.data);break;
			case INVITEM_METAL:set_bkg_data(first_tile, nb_tiles, invdetailmetaltiles.data);break;
			case INVITEM_ARROW_NORMAL:set_bkg_data(first_tile, nb_tiles, idnormaltiles.data);break;
		}
	}
    SWITCH_ROM(save);
}

void set_banked_bkg_data(UINT8 first_tile, UINT8 nb_tiles, UINT8 tiles_used, UINT8 bank) NONBANKED {
    uint8_t save = _current_bank;
    SWITCH_ROM(bank);
	switch(tiles_used){
		case 0u:
    		set_bkg_data(first_tile, nb_tiles, tiles.data+((16u) * first_tile));
		break;
		case 1u:
		    set_bkg_data(first_tile, nb_tiles, tilesanims.data+((16u) * first_tile));
		break;
		case 2u:
		    set_bkg_data(first_tile, nb_tiles, tilesanimsmapworld.data+((16u) * first_tile));
		break;
		case 4u:
		    set_bkg_data(first_tile, nb_tiles, tiles4.data+((16u) * first_tile));
		break;
		case 5u:
		    //set_bkg_data(first_tile, nb_tiles, tilesanimscutscene.data+((16u) * first_tile));
		break;
		case 6u:
		    set_bkg_data(first_tile, nb_tiles, tiles6.data+((16u) * first_tile));
		break;
		case 7u:
		    set_bkg_data(first_tile, nb_tiles, tiles7.data+((16u) * first_tile));
		break;
		case 8u:
		    set_bkg_data(first_tile, nb_tiles, tilescredit.data+((16u) * first_tile));
		break;
		case 9u:
		    set_bkg_data(first_tile, nb_tiles, tilesanimcredit.data+((16u) * first_tile));
		break;
		case 10u:
		    set_bkg_data(first_tile, nb_tiles, tilesdiagcrossbow.data+((16u) * first_tile));
		break;
		case 11u:
		    set_bkg_data(first_tile, nb_tiles, tdiagcrossbowempty.data+((16u) * first_tile));
		break;
		case 12u:
			set_bkg_data(first_tile, nb_tiles, cavetiles.data+((16u) * first_tile));
		break;
		case 13u:
			set_bkg_data(first_tile, nb_tiles, cavetilesanim.data+((16u) * first_tile));
		break;
		case 14u:
			set_bkg_data(first_tile, nb_tiles, titlescreentiles.data+((16u) * first_tile));
		break;
		case 15u:
			set_bkg_data(first_tile, nb_tiles, titlescreentilesanim.data+((16u) * first_tile));
		break;
		case 16u:
			set_bkg_data(first_tile, nb_tiles, titlescreentilesanim2.data+((16u) * first_tile));
		break;
		case 17u:
			set_bkg_data(first_tile, nb_tiles, titlescreentilesanim3.data+((16u) * first_tile));
		break;
	}
    SWITCH_ROM(save);
}

void set_dialog_bkg_data(UINT8 first_tile, UINT8 nb_tiles, WHOSTALKING whostalking, UINT8 bank) NONBANKED {
    uint8_t save = _current_bank;
    SWITCH_ROM(bank);
	switch(previous_state){
		case StateExzoo:
    		set_bkg_data(first_tile, nb_tiles, dialogtiles00.data+((16u) * first_tile));
		break;
		case StateCemetery:
    		set_bkg_data(first_tile, nb_tiles, dialogtiles01.data+((16u) * first_tile));
		break;
		case StateMine:
    		set_bkg_data(first_tile, nb_tiles, dialogmapmine.data+((16u) * first_tile));
		break;
		case StateBlackiecave:
			set_bkg_data(first_tile, nb_tiles, dmapblackiecave.data+((16u) * first_tile));
		break;
		case StateOverworld:
			if(whostalking == INTRO){
				set_bkg_data(first_tile, nb_tiles, dialogmapintro.data+((16u) * first_tile));
			}
		break;
	}
	switch(current_state){
		case StateSmith:
    		set_bkg_data(first_tile, nb_tiles, dialogmapsmith.data+((16u) * first_tile));
		break;
	}
    SWITCH_ROM(save);
}

void dialog_map() BANKED{
	switch(previous_state){
		case StateExzoo:
			set_dialog_bkg_data(0, 101u, whostalking, BANK(dialogtiles00));
		break;
		case StateCemetery:
			set_dialog_bkg_data(0, 101u, whostalking, BANK(dialogtiles01));
		break;
		case StateMine:
			set_dialog_bkg_data(0, 101u, whostalking, BANK(dialogmapmine));
		break;
		case StateBlackiecave:
			set_dialog_bkg_data(0, 101u, whostalking, BANK(dmapblackiecave));
		break;
		case StateOverworld:
			if(whostalking == INTRO){
				set_dialog_bkg_data(0, 101u, whostalking, BANK(dialogmapintro));
			}
		break;
	}
	switch(current_state){
		case StateSmith:
			set_dialog_bkg_data(0, 101u, whostalking, BANK(dialogmapsmith));
		break;
	}
}

void Inv_change_detail(UINT8 item, UINT8 isEmpty) BANKED{
	if(isEmpty){
		set_inv_bkg_data(item, 49u, 25, BANK(inventorytiles), isEmpty);
	}else{
		switch(item){
			case INVITEM_CROSSBOW:
				set_inv_bkg_data(item, 49u, 25, BANK(invdetail0tiles), isEmpty);
			break;
			case INVITEM_MONEY:
				set_inv_bkg_data(item, 49u, 25, BANK(invdetailmoneytiles), isEmpty);
			break;
			case INVITEM_POWDER:
				set_inv_bkg_data(item, 49u, 25, BANK(idpowdertiles), isEmpty);
			break;
			case INVITEM_WOOD:
				set_inv_bkg_data(item, 49u, 25, BANK(invdetailwoodtiles), isEmpty);
			break;
			case INVITEM_METAL:
				set_inv_bkg_data(item, 49u, 25, BANK(invdetailmetaltiles), isEmpty);
			break;
			case INVITEM_ARROW_NORMAL:
				set_inv_bkg_data(item, 49u, 25, BANK(idnormaltiles), isEmpty);
			break;
		}
	}
}

void Anim_Titlescreen_0() BANKED{
	set_banked_bkg_data(122u, 8u, 14u, BANK(titlescreentiles));//ship
	set_banked_bkg_data(108u, 3u, 14u, BANK(titlescreentiles));//water
	set_banked_bkg_data(98u, 2u, 14u, BANK(titlescreentiles));//water
}
void Anim_Titlescreen_1() BANKED{
	set_banked_bkg_data(122u, 8u, 15u, BANK(titlescreentilesanim));//ship
	set_banked_bkg_data(98u, 2u, 15u, BANK(titlescreentilesanim));//water
}
void Anim_Titlescreen_2() BANKED{
	set_banked_bkg_data(108u, 3u, 16u, BANK(titlescreentilesanim2));//water
	set_banked_bkg_data(98u, 2u, 16u, BANK(titlescreentilesanim2));//water
}
void Anim_Titlescreen_3() BANKED{
	set_banked_bkg_data(108u, 3u, 17u, BANK(titlescreentilesanim3));//water
	set_banked_bkg_data(98u, 2u, 17u, BANK(titlescreentilesanim3));//water
}

void Anim_Cave_0() BANKED{
	set_banked_bkg_data(29u, 2u, 13u, BANK(cavetilesanim));//fiammella
	set_banked_bkg_data(41u, 7u, 13u, BANK(cavetilesanim));//luce lanterna
	set_banked_bkg_data(60u, 2u, 13u, BANK(cavetilesanim));//carrello
}
void Anim_Cave_1() BANKED{
	set_banked_bkg_data(29u, 2u, 12u, BANK(cavetiles));//fiammella
	set_banked_bkg_data(41u, 7u, 12u, BANK(cavetiles));//luce lanterna
	set_banked_bkg_data(60u, 2u, 12u, BANK(cavetiles));//carrello
}

void Anim_StudioLoading_0() BANKED{
	set_banked_bkg_data(14u, 1u, 8u, BANK(tilescredit));
	set_banked_bkg_data(15u, 1u, 8u, BANK(tilescredit));
}
void Anim_StudioLoading_1() BANKED{
	set_banked_bkg_data(14u, 1u, 9u, BANK(tilesanimcredit));
	set_banked_bkg_data(15u, 1u, 9u, BANK(tilesanimcredit));
}
//set_bkg_data
void Anim_Crossbow_0() BANKED{
	set_banked_bkg_data(0u, 67u, 10u, BANK(tilesdiagcrossbow));
}
void Anim_Crossbow_1() BANKED{
	set_banked_bkg_data(0u, 67u, 11u, BANK(tdiagcrossbowempty));
}

void Anim_Ground_Straight_0() BANKED{
	set_banked_bkg_data(68u, 1u, 10u, BANK(tilesdiagcrossbow));
	set_banked_bkg_data(69u, 1u, 10u, BANK(tilesdiagcrossbow));
	set_banked_bkg_data(75u, 1u, 10u, BANK(tilesdiagcrossbow));
}
void Anim_Ground_Straight_1() BANKED{
	set_banked_bkg_data(68u, 1u, 11u, BANK(tilesdiagcrossbow));
	set_banked_bkg_data(69u, 1u, 11u, BANK(tilesdiagcrossbow));
	set_banked_bkg_data(75u, 1u, 11u, BANK(tilesdiagcrossbow));
}

void Anim_Tiles_0() BANKED{
}

void Anim_Tiles_1() BANKED{
	
}

void WorldmapQuiverStone() BANKED{
	set_banked_bkg_data(44u, 1u, 2, BANK(tilesanimsmapworld));// 16 * 1
}
void WorldmapQuiverThunder() BANKED{
	set_banked_bkg_data(45u, 1u, 2, BANK(tilesanimsmapworld));// 16 * 3
}
void WorldmapQuiverIce() BANKED{
	set_banked_bkg_data(56u, 1u, 2, BANK(tilesanimsmapworld));// 16 * 6
}
void WorldmapQuiverFire() BANKED{
	set_banked_bkg_data(47u, 1u, 2, BANK(tilesanimsmapworld));// 16 * 10
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
	set_banked_bkg_data(116u, 1u, 2, BANK(tilesanimsmapworld));
	set_banked_bkg_data(117u, 1u, 2, BANK(tilesanimsmapworld));
	set_banked_bkg_data(118u, 1u, 2, BANK(tilesanimsmapworld));
	set_banked_bkg_data(119u, 1u, 2, BANK(tilesanimsmapworld));
}