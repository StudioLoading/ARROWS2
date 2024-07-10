#include "Banks/SetAutoBank.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "string.h"
#include "Scroll.h"

#include "InventoryDialogs.h"

unsigned char ddinv1[12];
unsigned char ddinv2[12];
unsigned char ddinv3[12];
unsigned char ddinv4[12];
unsigned char ddinv5[12];
unsigned char ddinv6[12];
unsigned char ddinv7[12];
unsigned char ddinv8[12];


void GetLocalizedINVLabel_EN(INV_TO_BE_LOCALIZED label, char* d) BANKED{
	switch(label){
		case INV_EMPTY_STRING:memcpy(d, "           ", 12);break;
		case CROSSBOW_NAME:memcpy(d, "CROSSBOW:  ", 12);break;
		case CROSSBOW_DETAIL1:memcpy(d, "SHORT-     ", 12);break;
		case CROSSBOW_DETAIL2:memcpy(d, "LIGHWEIGHT ", 12);break;
		case CROSSBOW_DETAIL3:memcpy(d, "WOODEN HAND", 12);break;
		case CROSSBOW_DETAIL4:memcpy(d, "MADE.     ", 12);break;
		case MONEY_NAME:memcpy(d, " COINS:    ", 12);break;
		case MONEY_DETAIL1:memcpy(d, "SOME GOLDEN", 12);break;
		case MONEY_DETAIL2:memcpy(d, "COINS ARE  ", 12);break;
		case MONEY_DETAIL3:memcpy(d, "USEFUL IN  ", 12);break;
		case MONEY_DETAIL4:memcpy(d, "THIS WORLD.", 12);break;
		case BOMB_NAME:memcpy(d, " BOMB:     ", 12);break;
		case BOMB_DETAIL1:memcpy(d, "AS SOON AS ", 12);break;
		case BOMB_DETAIL2:memcpy(d, "IT CRASHES ", 12);break;
		case BOMB_DETAIL3:memcpy(d, "THE FLOOR- ", 12);break;
		case BOMB_DETAIL4:memcpy(d, "IT EXPLODES", 12);break;
		case ARROWNORMAL_NAME:memcpy(d, "  NORMAL:  ", 12);break;
		case ARROWNORMAL_DETAIL1:memcpy(d, "MOST COMMON", 12);break;
		case ARROWNORMAL_DETAIL2:memcpy(d, "ARROWS I   ", 12);break;
		case ARROWNORMAL_DETAIL3:memcpy(d, "CAN FIND.  ", 12);break;
		case ARROWNORMAL_DETAIL4:memcpy(d, "           ", 12);break;
		case ARROWPERFO_NAME:memcpy(d, "  PIERCING:", 12);break;
		case ARROWPERFO_DETAIL1:memcpy(d, "NO LIVING  ", 12);break;
		case ARROWPERFO_DETAIL2:memcpy(d, "CREATURE   ", 12);break;
		case ARROWPERFO_DETAIL3:memcpy(d, "CAN STOP   ", 12);break;
		case ARROWPERFO_DETAIL4:memcpy(d, "THESE.     ", 12);break;
		case ARROWBASTARD_NAME:memcpy(d, "  BASTARD: ", 12);break;
		case ARROWBASTARD_DETAIL1:memcpy(d, "MOST POWER ", 12);break;
		case ARROWBASTARD_DETAIL2:memcpy(d, "FUL ARROWS ", 12);break;
		case ARROWBASTARD_DETAIL3:memcpy(d, "I CAN FIND ", 12);break;
		case ARROWBASTARD_DETAIL4:memcpy(d, "           ", 12);break;
		case METAL_NAME:memcpy(d, "   METAL:  ", 12);break;
		case METAL_DETAIL1:memcpy(d, "PIECE OF   ", 12);break;
		case METAL_DETAIL2:memcpy(d, "METAL.     ", 12);break;
		case METAL_DETAIL3:memcpy(d, "           ", 12);break;
		case METAL_DETAIL4:memcpy(d, "           ", 12);break;
		case STEEL_NAME:memcpy(d, "   STEEL:  ", 12);break;
		case STEEL_DETAIL1:memcpy(d, "PIECE OF   ", 12);break;
		case STEEL_DETAIL2:memcpy(d, "STEEL.     ", 12);break;
		case STEEL_DETAIL3:memcpy(d, "           ", 12);break;
		case STEEL_DETAIL4:memcpy(d, "           ", 12);break;
		case WOOD_NAME:memcpy(d, "    WOOD:  ", 12);break;
		case WOOD_DETAIL1:memcpy(d, "PIECE OF   ", 12);break;
		case WOOD_DETAIL2:memcpy(d, "WOOD.      ", 12);break;
		case WOOD_DETAIL3:memcpy(d, "           ", 12);break;
		case WOOD_DETAIL4:memcpy(d, "           ", 12);break;
		case POWDER_NAME:memcpy(d, "GUNPOWDER: ", 12);break;
		case POWDER_DETAIL1:memcpy(d, "SOME GUNPOW", 12);break;
		case POWDER_DETAIL2:memcpy(d, "DER.THE SMI", 12);break;
		case POWDER_DETAIL3:memcpy(d, "TH MAY USE ", 12);break;
		case POWDER_DETAIL4:memcpy(d, "IT         ", 12);break;
		case LETTER_NAME:memcpy(d, "LETTER:    ", 12);break;
		case LETTER_DETAIL1:memcpy(d, "THIS IS THE", 12);break;
		case LETTER_DETAIL2:memcpy(d, "REASON WHY ", 12);break;
		case LETTER_DETAIL3:memcpy(d, "I HAVE TO G", 12);break;
		case LETTER_DETAIL4:memcpy(d, "O GET LIAM.", 12);break;
		case PASS_NAME:memcpy(d, "   PASS    ", 12);break;
		case PASS_DETAIL1:memcpy(d, "THIS CAN   ", 12);break;
		case PASS_DETAIL2:memcpy(d, "GET ME INTO", 12);break;
		case PASS_DETAIL3:memcpy(d, "OUTWALKER' ", 12);break;
		case PASS_DETAIL4:memcpy(d, "PLACE.     ", 12);break;
		case GLASS_NAME:memcpy(d, "  GLASSES  ", 12);break;
		case GLASS_DETAIL1:memcpy(d, "THIS BELONG", 12);break;
		case GLASS_DETAIL2:memcpy(d, "TO JACK THE", 12);break;
		case GLASS_DETAIL3:memcpy(d, "OUTWALKER  ", 12);break;
		case GLASS_DETAIL4:memcpy(d, "           ", 12);break;
		case FLOWER_NAME:memcpy(d, "  FLOWER   ", 12);break;
		case FLOWER_DETAIL1:memcpy(d, "THE SMITH  ", 12);break;
		case FLOWER_DETAIL2:memcpy(d, "WANTS THESE", 12);break;
		case FLOWER_DETAIL3:memcpy(d, "FOR HIS    ", 12);break;
		case FLOWER_DETAIL4:memcpy(d, "PROPOSAL   ", 12);break;
		case BOX_NAME:memcpy(d, "    BOX    ", 12);break;
		case BOX_DETAIL1:memcpy(d, "STUFF I HA ", 12);break;
		case BOX_DETAIL2:memcpy(d, "VE TO BRING", 12);break;
		case BOX_DETAIL3:memcpy(d, "TO THE OUT ", 12);break;
		case BOX_DETAIL4:memcpy(d, "WALKERS    ", 12);break;
		case SCORPIONTAIL_NAME:memcpy(d, " POISONED! ", 12);break;
		case SCORPIONTAIL_DETAIL1:memcpy(d, "THE STING  ", 12);break;
		case SCORPIONTAIL_DETAIL2:memcpy(d, "OF THE MOTH", 12);break;
		case SCORPIONTAIL_DETAIL3:memcpy(d, "ER OF THE  ", 12);break;
		case SCORPIONTAIL_DETAIL4:memcpy(d, "SCORPIONS  ", 12);break;
		case HERBS_NAME:memcpy(d, "   HERBS   ", 12);break;
		case HERBS_DETAIL1:memcpy(d, "THE SECOND ", 12);break;
		case HERBS_DETAIL2:memcpy(d, "INGREDIENT ", 12);break;
		case HERBS_DETAIL3:memcpy(d, "FOR THE    ", 12);break;
		case HERBS_DETAIL4:memcpy(d, "ANTIDOTE   ", 12);break;
		case MAP_NAME:memcpy(d, " LIAM WRITE ", 12);break;
		case MAP_DETAIL1:memcpy(d, "PRESS FIRE ", 12);break;
		case MAP_DETAIL2:memcpy(d, "TO ANALYZE ", 12);break;
		case MAP_DETAIL3:memcpy(d, "IT.        ", 12);break;
		case MAP_DETAIL4:memcpy(d, "           ", 12);break;
		case SILVERSKULL_NAME:memcpy(d, "SILVERSKULL", 12);break;
		case SILVERSKULL_DETAIL1:memcpy(d, "THE SKULL ", 12);break;
		case SILVERSKULL_DETAIL2:memcpy(d, "TO BE USED", 12);break;
		case SILVERSKULL_DETAIL3:memcpy(d, "TO FORGE  ", 12);break;
		case SILVERSKULL_DETAIL4:memcpy(d, "THE ARMOR ", 12);break;

		/*
		,
		ARROWBASTARD_DETAIL*/
	}	
}
