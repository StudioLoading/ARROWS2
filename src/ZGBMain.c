#include "ZGBMain.h"
#include "Math.h"

UINT8 J_JUMP=J_A;//0x10;
UINT8 J_FIRE=J_B;//0x20;

const UINT16 BONUS_PUNTA_INIT_Y = 94u;
const UINT16 BONUS_PUNTA_INIT_X = 81u;
const UINT8 TOTAL_CHAPTERS = 5;

UINT8 next_state = StateCredit;
UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	return 255u;
}
/*
UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	if(current_state == StateGame) {
		if(U_LESS_THAN(255 - (UINT16)*tile_ptr, N_SPRITE_TYPES)) {
			*tile = 0;
			return 255 - (UINT16)*tile_ptr;
		}

		*tile = *tile_ptr;
	}

	return 255u;
}
*/