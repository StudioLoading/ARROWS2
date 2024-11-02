#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "custom_datas.h"
#include "Dialogs.h"


IMPORT_TILES(owsouthwesttiles);
IMPORT_TILES(owswanim1);
IMPORT_TILES(owswanim2);
IMPORT_TILES(owswanim3);
IMPORT_TILES(invdetail0tiles);
IMPORT_TILES(invdetailmoneytiles);
IMPORT_TILES(idpowdertiles);
IMPORT_TILES(idlettertiles);
IMPORT_TILES(idperfotiles);
IMPORT_TILES(idbastardtiles);
IMPORT_TILES(invdetailwoodtiles);
IMPORT_TILES(invdetailmetaltiles);
IMPORT_TILES(idnormaltiles);
IMPORT_TILES(inventorytiles);
IMPORT_TILES(inventorytilesanim);
IMPORT_TILES(idglasstiles);
IMPORT_TILES(idflowertiles);
IMPORT_TILES(idboxtiles);
IMPORT_TILES(idsteeltiles);
IMPORT_TILES(idscorpiontailtiles);
IMPORT_TILES(idherbstiles);
IMPORT_TILES(idmaptiles);
IMPORT_TILES(idsilverskull);
IMPORT_TILES(idmushroomtiles);
//IMPORT_TILES(tilesanimscutscene);
IMPORT_TILES(tilescredit);
IMPORT_TILES(tacredit0);
IMPORT_TILES(tacredit1);
IMPORT_TILES(tilesdiagcrossbow);
IMPORT_TILES(tdiagcrossbowempty);
IMPORT_TILES(tcrossbowanim);
IMPORT_TILES(minetiles);
IMPORT_TILES(minetilesanim1);
IMPORT_TILES(minetilesanim2);
IMPORT_TILES(minetilesanim3);
IMPORT_TILES(minetilesanim4);
IMPORT_TILES(minetilesanim5);
IMPORT_TILES(titlescreentiles);
IMPORT_TILES(titlescreentilesanim);
IMPORT_TILES(titlescreentilesanim2);
IMPORT_TILES(titlescreentilesanim3);
IMPORT_TILES(diaryt);
IMPORT_TILES(mapsilvert);

extern WHOSTALKING whostalking;
extern UINT8 previous_state;


void set_inv_bkg_data(UINT8 first_tile, UINT8 nb_tiles, 
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

void Inv_change_detail(UINT8 item, UINT8 isEmpty) BANKED{
	if(isEmpty){
		set_inv_bkg_data(49u, 25, BANK(inventorytiles), isEmpty, &inventorytiles);
	}else{
		switch(item){
			case INVITEM_MONEY:
				set_inv_bkg_data(49u, 25, BANK(invdetailmoneytiles), isEmpty, &invdetailmoneytiles);
			break;
			case INVITEM_POWDER:
				set_inv_bkg_data(49u, 25, BANK(idpowdertiles), isEmpty, &idpowdertiles);
			break;
			case INVITEM_LETTER:
			case INVITEM_PASS:
				set_inv_bkg_data(49u, 25, BANK(idlettertiles), isEmpty, &idlettertiles);
			break;
			case INVITEM_WOOD:
				set_inv_bkg_data(49u, 25, BANK(invdetailwoodtiles), isEmpty, &invdetailwoodtiles);
			break;
			case INVITEM_METAL:
				set_inv_bkg_data(49u, 25, BANK(invdetailmetaltiles), isEmpty, &invdetailmetaltiles);
			break;
			case INVITEM_ARROW_NORMAL:
				set_inv_bkg_data(49u, 25, BANK(idnormaltiles), isEmpty, &idnormaltiles);
			break;
			case INVITEM_ARROW_PERFO:
				set_inv_bkg_data(49u, 25, BANK(idperfotiles), isEmpty, &idperfotiles);
			break;
			case INVITEM_ARROW_BASTARD:
				set_inv_bkg_data(49u, 25, BANK(idbastardtiles), isEmpty, &idbastardtiles);
			break;
			case INVITEM_GLASSES:
				set_inv_bkg_data(49u, 25, BANK(idglasstiles), isEmpty, &idglasstiles);
			break;
			case INVITEM_FLOWER:
				set_inv_bkg_data(49u, 25, BANK(idflowertiles), isEmpty, &idflowertiles);
			break;
			case INVITEM_BOX:
				set_inv_bkg_data(49u, 25, BANK(idboxtiles), isEmpty, &idboxtiles);
			break;
			case INVITEM_MUSHROOM:
				set_inv_bkg_data(49u, 25, BANK(idmushroomtiles), isEmpty, &idmushroomtiles);
			break;
			case INVITEM_SILVER:
				set_inv_bkg_data(49u, 25, BANK(idsteeltiles), isEmpty, &idsteeltiles);
			break;
			case INVITEM_SCORPIONTAIL:
				set_inv_bkg_data(49u, 25, BANK(idscorpiontailtiles), isEmpty, &idscorpiontailtiles);
			break;
			case INVITEM_HERB:
				set_inv_bkg_data(49u, 25, BANK(idherbstiles), isEmpty, &idherbstiles);
			break;
			case INVITEM_LIAM_HANDWRITTEN:
				set_inv_bkg_data(49u, 25, BANK(idmaptiles), isEmpty, &idmaptiles);
			break;
			case INVITEM_SILVERSKULL:
				set_inv_bkg_data(49u, 25, BANK(idsilverskull), isEmpty, &idsilverskull);
			break;
		}
	}
}

void Anim_Ow_0() BANKED{
	set_banked_bkg_data(7u, 1u, &owsouthwesttiles, BANK(owsouthwesttiles));//grass
	set_banked_bkg_data(103u, 5u, &owsouthwesttiles, BANK(owsouthwesttiles));//water
	set_banked_bkg_data(105u, 2u, &owsouthwesttiles, BANK(owsouthwesttiles));//updown arrows
	set_banked_bkg_data(132u, 2u, &owsouthwesttiles, BANK(owsouthwesttiles));//leftright arrows
	set_banked_bkg_data(3u, 2u, &owsouthwesttiles, BANK(owsouthwesttiles));//flag
	set_banked_bkg_data(40u, 1u, &owsouthwesttiles, BANK(owsouthwesttiles));//flag
	set_banked_bkg_data(82u, 1u, &owsouthwesttiles, BANK(owsouthwesttiles));//flag
}
void Anim_Ow_1() BANKED{
	set_banked_bkg_data(7u, 1u, &owswanim1, BANK(owswanim1));//grass
	set_banked_bkg_data(103u, 2u, &owswanim1, BANK(owswanim1));//water
	set_banked_bkg_data(3u, 2u, &owswanim1, BANK(owswanim1));//flag
	set_banked_bkg_data(40u, 1u, &owswanim1, BANK(owswanim1));//flag
	set_banked_bkg_data(82u, 1u, &owswanim1, BANK(owswanim1));//flag
}
void Anim_Ow_2() BANKED{
	set_banked_bkg_data(7u, 1u, &owswanim2, BANK(owswanim2));//grass
	set_banked_bkg_data(103u, 2u, &owswanim2, BANK(owswanim2));//water
	set_banked_bkg_data(3u, 2u, &owswanim2, BANK(owswanim2));//flag
	set_banked_bkg_data(40u, 1u, &owswanim2, BANK(owswanim2));//flag
	set_banked_bkg_data(82u, 1u, &owswanim2, BANK(owswanim2));//flag
}
void Anim_Ow_3() BANKED{
	set_banked_bkg_data(7u, 1u, &owswanim3, BANK(owswanim3));//grass
	set_banked_bkg_data(103u, 2u, &owswanim3, BANK(owswanim3));//water
	set_banked_bkg_data(105u, 2u, &owswanim3, BANK(owswanim3));//updown arrows
	set_banked_bkg_data(132u, 2u, &owswanim3, BANK(owswanim3));//leftright arrows
	set_banked_bkg_data(3u, 2u, &owswanim3, BANK(owswanim3));//flag
	set_banked_bkg_data(40u, 1u, &owswanim3, BANK(owswanim3));//flag
	set_banked_bkg_data(82u, 1u, &owswanim3, BANK(owswanim3));//flag
}
void Anim_Ow_4() BANKED{
	set_banked_bkg_data(7u, 1u, &owswanim2, BANK(owswanim2));//grass
	set_banked_bkg_data(103u, 2u, &owswanim2, BANK(owswanim2));//water
}
void Anim_Ow_5() BANKED{
	set_banked_bkg_data(7u, 1u, &owswanim1, BANK(owswanim1));//grass
	set_banked_bkg_data(103u, 2u, &owswanim1, BANK(owswanim1));//water
}

void Anim_Ow_see_0() BANKED{
	set_banked_bkg_data(80u, 2u, &owsouthwesttiles, BANK(owsouthwesttiles));//water-waves
}
void Anim_Ow_see_1() BANKED{
	set_banked_bkg_data(80u, 2u, &owswanim1, BANK(owswanim1));//water-waves
}
void Anim_Ow_see_2() BANKED{
	set_banked_bkg_data(80u, 2u, &owswanim2, BANK(owswanim2));//water-waves
}
void Anim_Ow_see_3() BANKED{
	set_banked_bkg_data(80u, 2u, &owswanim3, BANK(owswanim3));//water-waves
}


void Anim_Ow_flag_0() BANKED{
	set_banked_bkg_data(3u, 2u, &owsouthwesttiles, BANK(owsouthwesttiles));//flag
	set_banked_bkg_data(40u, 1u, &owsouthwesttiles, BANK(owsouthwesttiles));//flag
	set_banked_bkg_data(82u, 1u, &owsouthwesttiles, BANK(owsouthwesttiles));//flag
}
void Anim_Ow_flag_1() BANKED{
	set_banked_bkg_data(3u, 2u, &owswanim1, BANK(owswanim1));//flag
	set_banked_bkg_data(40u, 1u, &owswanim1, BANK(owswanim1));//flag
	set_banked_bkg_data(82u, 1u, &owswanim1, BANK(owswanim1));//flag
}
void Anim_Ow_flag_2() BANKED{
	set_banked_bkg_data(3u, 2u, &owswanim2, BANK(owswanim2));//flag
	set_banked_bkg_data(40u, 1u, &owswanim2, BANK(owswanim2));//flag
	set_banked_bkg_data(82u, 1u, &owswanim2, BANK(owswanim2));//flag
}
void Anim_Ow_flag_3() BANKED{
	set_banked_bkg_data(3u, 2u, &owswanim3, BANK(owswanim3));//flag
	set_banked_bkg_data(40u, 1u, &owswanim3, BANK(owswanim3));//flag
	set_banked_bkg_data(82u, 1u, &owswanim3, BANK(owswanim3));//flag
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

void Anim_Cart_0() BANKED{
	set_banked_bkg_data(23u, 2u, &minetiles, BANK(minetiles));//fiammella
	set_banked_bkg_data(8u, 2u, &minetiles, BANK(minetiles));//elevator
	set_banked_bkg_data(93u, 1u, &minetiles, BANK(minetiles));//electric
}
void Anim_Cart_1() BANKED{
	set_banked_bkg_data(23u, 2u, &minetilesanim1, BANK(minetilesanim1));//fiammella
	set_banked_bkg_data(8u, 2u, &minetilesanim1, BANK(minetilesanim1));//elevator
	set_banked_bkg_data(93u, 1u, &minetilesanim1, BANK(minetilesanim1));//electric
}
void Anim_Cart_2() BANKED{
	set_banked_bkg_data(23u, 2u, &minetilesanim2, BANK(minetilesanim2));//fiammella
	set_banked_bkg_data(8u, 2u, &minetilesanim2, BANK(minetilesanim2));//elevator
	set_banked_bkg_data(93u, 1u, &minetilesanim2, BANK(minetilesanim2));//electric
}
void Anim_Cart_3() BANKED{
	set_banked_bkg_data(23u, 2u, &minetilesanim3, BANK(minetilesanim3));//fiammella
	set_banked_bkg_data(8u, 2u, &minetilesanim3, BANK(minetilesanim3));//elevator
	set_banked_bkg_data(93u, 1u, &minetilesanim3, BANK(minetilesanim3));//electric
}
void Anim_Cart_4() BANKED{
	set_banked_bkg_data(23u, 2u, &minetilesanim4, BANK(minetilesanim4));//fiammella
	set_banked_bkg_data(8u, 2u, &minetilesanim4, BANK(minetilesanim4));//elevator
	set_banked_bkg_data(93u, 1u, &minetilesanim4, BANK(minetilesanim4));//electric
}
void Anim_Cart_5() BANKED{
	set_banked_bkg_data(23u, 2u, &minetilesanim5, BANK(minetilesanim5));//fiammella
	set_banked_bkg_data(8u, 2u, &minetilesanim5, BANK(minetilesanim5));//elevator
	set_banked_bkg_data(93u, 1u, &minetilesanim5, BANK(minetilesanim5));//electric
}

void Anim_Cave_0() BANKED{
	set_banked_bkg_data(23u, 2u, &minetiles, BANK(minetiles));//fiammella
	set_banked_bkg_data(41u, 7u, &minetiles, BANK(minetiles));//luce lanterna
	set_banked_bkg_data(60u, 2u, &minetiles, BANK(minetiles));//carrello
}
void Anim_Cave_1() BANKED{
	set_banked_bkg_data(23u, 2u, &minetilesanim1, BANK(minetilesanim1));//fiammella
	set_banked_bkg_data(41u, 7u, &minetilesanim1, BANK(minetilesanim1));//luce lanterna
	set_banked_bkg_data(60u, 2u, &minetilesanim1, BANK(minetilesanim1));//carrello
}
void Anim_Cave_2() BANKED{
	set_banked_bkg_data(23u, 2u, &minetilesanim2, BANK(minetilesanim2));//fiammella
	set_banked_bkg_data(41u, 7u, &minetilesanim2, BANK(minetilesanim2));//luce lanterna
	set_banked_bkg_data(60u, 2u, &minetilesanim2, BANK(minetilesanim2));//carrello
}
void Anim_Cave_3() BANKED{
	set_banked_bkg_data(23u, 2u, &minetilesanim3, BANK(minetilesanim3));//fiammella
	set_banked_bkg_data(41u, 7u, &minetilesanim3, BANK(minetilesanim3));//luce lanterna
	set_banked_bkg_data(60u, 2u, &minetilesanim3, BANK(minetilesanim3));//carrello
}
void Anim_Cave_4() BANKED{
	set_banked_bkg_data(23u, 2u, &minetilesanim4, BANK(minetilesanim4));//fiammella
	set_banked_bkg_data(41u, 7u, &minetilesanim4, BANK(minetilesanim4));//luce lanterna
	set_banked_bkg_data(60u, 2u, &minetilesanim4, BANK(minetilesanim4));//carrello
}
void Anim_Cave_5() BANKED{
	set_banked_bkg_data(23u, 2u, &minetilesanim5, BANK(minetilesanim5));//fiammella
	set_banked_bkg_data(41u, 7u, &minetilesanim5, BANK(minetilesanim5));//luce lanterna
	set_banked_bkg_data(60u, 2u, &minetilesanim5, BANK(minetilesanim5));//carrello
}

void Anim_StudioLoading_0() BANKED{
	set_banked_bkg_data(14u, 10u, &tilescredit, BANK(tilescredit));
	set_banked_bkg_data(30u, 1u, &tilescredit, BANK(tilescredit));
	set_banked_bkg_data(40u, 5u, &tilescredit, BANK(tilescredit));
	set_banked_bkg_data(48u, 5u, &tilescredit, BANK(tilescredit));
}
void Anim_StudioLoading_1() BANKED{
	set_banked_bkg_data(14u, 10u, &tacredit0, BANK(tacredit0));
	set_banked_bkg_data(30u, 1u, &tacredit0, BANK(tacredit0));
	set_banked_bkg_data(40u, 5u, &tacredit0, BANK(tacredit0));
	set_banked_bkg_data(48u, 5u, &tacredit0, BANK(tacredit0));
}
void Anim_StudioLoading_2() BANKED{
	set_banked_bkg_data(14u, 10u, &tacredit1, BANK(tacredit1));
	set_banked_bkg_data(30u, 1u, &tacredit1, BANK(tacredit1));
	set_banked_bkg_data(40u, 5u, &tacredit1, BANK(tacredit1));
	set_banked_bkg_data(48u, 5u, &tacredit1, BANK(tacredit1));
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

void Anim_FPS_gator_0() BANKED{
	set_banked_bkg_data(84u, 4u, &tilesdiagcrossbow, BANK(tilesdiagcrossbow));
}

void Anim_FPS_gator_1() BANKED{
	set_banked_bkg_data(84u, 4u, &tcrossbowanim, BANK(tcrossbowanim));
}

void Anim_Silver_0() BANKED{
	set_banked_bkg_data(23u, 2u, &mapsilvert, BANK(mapsilvert));//fiammella
	set_banked_bkg_data(37u, 6u, &mapsilvert, BANK(mapsilvert));//lava
}
void Anim_Silver_1() BANKED{
	set_banked_bkg_data(23u, 2u, &minetilesanim1, BANK(minetilesanim1));//fiammella
}
void Anim_Silver_2() BANKED{
	set_banked_bkg_data(23u, 2u, &minetilesanim2, BANK(minetilesanim2));//fiammella
	set_banked_bkg_data(37u, 6u, &minetilesanim1, BANK(minetilesanim1));//lava
}
void Anim_Silver_3() BANKED{
	set_banked_bkg_data(23u, 2u, &minetilesanim3, BANK(minetilesanim3));//fiammella
	set_banked_bkg_data(37u, 6u, &minetilesanim2, BANK(minetilesanim2));//lava
}
void Anim_Silver_4() BANKED{
	set_banked_bkg_data(23u, 2u, &minetilesanim4, BANK(minetilesanim4));//fiammella
}
void Anim_Silver_5() BANKED{
	set_banked_bkg_data(23u, 2u, &minetilesanim5, BANK(minetilesanim5));//fiammella
	set_banked_bkg_data(37u, 6u, &minetilesanim3, BANK(minetilesanim3));//lava
}
void Anim_Inventory_0() BANKED{
	set_banked_bkg_data(28, 2u, &inventorytiles, BANK(inventorytiles));//occhi Dessa
}
void Anim_Inventory_1() BANKED{
	set_banked_bkg_data(28u, 2u, &inventorytilesanim, BANK(inventorytilesanim));//occhi Dessa
}