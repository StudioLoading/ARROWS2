#pragma bank 10

#include <gb/gb.h>
#include <gb/sgb.h>

#include "sgb_palette.h"


void set_sgb_palette_title() __banked {	
	struct {
        UINT8 command;
        UINT16 pal1[4];
        UINT16 pal2[3];
        UINT8 padding;
    } SGB_PALETTE2_PACKET = {
        .command = (SGB_PAL_12 << 3) | 1,
        .pal1 = {myRGB(255, 255, 255), myRGB(88, 216, 84), myRGB(0, 168, 0),  myRGB(0, 0, 0)}, //SGB_STATUS_RED},
        .pal2 = {SGB_STATUS_LIGHT, myRGB(247, 142, 14), myRGB(0, 0, 0)}, //myRGB(228, 92, 16)
        .padding = 0 
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_SECONDLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 1u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b01010101,0b01010101,0b01010101,0b01010101,
            0b01010101,0b10101010,0b10101010,0b10101010,
            0b10101010,0b10101010}
    };	
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_THIRDLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 2u,
        .ndata = 20u,
		.style = 0,
   		.ds = {0b01010101,0b01010101,0b01010101,0b01010101,
            0b01010101,0b01010101,0b10101010,0b10101010,
            0b10101010,0b10101010}
    };	
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_FOURTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 3u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_FIFTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 4u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_SIXTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 5u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_SEVENTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 6u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010}
    };
    sgb_transfer((void *)&SGB_PALETTE2_PACKET);
    //sgb_transfer((void *)&SGB_PALETTE_FIRSTLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_SECONDLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_THIRDLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_FOURTHLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_FIFTHLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_SIXTHLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_SEVENTHLINE_PACKET);
}

void reset_sgb_palette_title() __banked {	
	
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_FIRSTLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 0u,
        .ndata = 20u,
		.style = 0,
		.ds = {0,0,0,0,0,0,0,0,0,0}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_SECONDLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 1u,
        .ndata = 20u,
		.style = 0,
		.ds = {0,0,0,0,0,0,0,0,0,0}
    };	
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_THIRDLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 2u,
        .ndata = 20u,
		.style = 0,
   		.ds = {0,0,0,0,0,0,0,0,0,0}
    };	
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_FOURTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 3u,
        .ndata = 20u,
		.style = 0,
		.ds = {0,0,0,0,0,0,0,0,0,0}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_FIFTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 4u,
        .ndata = 20u,
		.style = 0,
		.ds = {0,0,0,0,0,0,0,0,0,0}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_SIXTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 5u,
        .ndata = 20u,
		.style = 0,
		.ds = {0,0,0,0,
        0,0,0,0,
        0,0}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_SEVENTHLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 6u,
        .ndata = 20u,
		.style = 0,
		.ds = {0,0,0,0,
        0,0,0,0,
        0,0}
    };
    sgb_transfer((void *)&SGB_PALETTE_FIRSTLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_SECONDLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_THIRDLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_FOURTHLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_FIFTHLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_SIXTHLINE_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_SEVENTHLINE_PACKET);
}

void set_sgb_palette_statusbar() __banked {	
	struct {
        UINT8 command;
        UINT16 pal1[4];
        UINT16 pal2[3];
        UINT8 padding;
    } SGB_PALETTE2_PACKET = {
        .command = (SGB_PAL_12 << 3) | 1,
        .pal1 = {myRGB(255, 255, 255), SGB_STATUS_LIGHT, SGB_STATUS_RED, SGB_STATUS_DARKER},
        .pal2 = {SGB_PAL2_LIGHT, SGB_STATUS_LIGHT, SGB_STATUS_DARKER},
        .padding = 0 
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_0 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 15u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_1 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 16u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b10101010,0b10010101,0b01010101,0b01010110,
        0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_2 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 17u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b10101010,0b10010101,0b01010101,0b01010110,
        0b10101010,0b10101010,0b10101010,0b10101010,
        0b10101010,0b10101010}
    };
    sgb_transfer((void *)&SGB_PALETTE2_PACKET);
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_0);
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_1);
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_2);
}

void set_sgb_palette(UINT16 c0, UINT16 c1, UINT16 c2, UINT16 c3) __banked {
    struct {
        UINT8 command;
        UINT16 pal0[4];
        UINT16 pal1[3];
        UINT8 padding;
    } SGB_PALETTE_PACKET = {
        .command = (SGB_PAL_01 << 3) | 1,
        .pal0 = {c0, c1, c2, c3},
        .pal1 = {0, 0, 0},
        .padding = 0 
    };
    sgb_transfer((void *)&SGB_PALETTE_PACKET);
}

void reset_sgb_palette_statusbar() __banked{	
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_0 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 15u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b00000000,0b00000000,0b00000000,0b00000000,
                0b00000000,0b00000000,0b00000000,0b00000000,
                0b00000000,0b00000000}
    };	
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_1 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 16u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000}
    };
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_STATUS_PACKET_2 = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 17u,
        .ndata = 20u,
		.style = 0,
		.ds = {0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000}
    };
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_0);	
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_1);	
    sgb_transfer((void *)&SGB_PALETTE_STATUS_PACKET_2);	
}

void set_sgb_palette_overworldsw() __banked{
    set_sgb_palette(SGB_PAL7_LIGHTER, SGB_PAL7_LIGHT, SGB_PAL7_DARK, SGB_PAL7_DARKER);
}
void set_sgb_palette_2() __banked{
    set_sgb_palette(SGB_PAL2_LIGHTER, SGB_PAL2_LIGHT, SGB_PAL2_DARK, SGB_PAL2_DARKER);
}
void set_sgb_palette_inventory() __banked{
    set_sgb_palette(SGB_INVENTORY_LIGHTER, SGB_INVENTORY_LIGHT, SGB_INVENTORY_DARK, SGB_INVENTORY_DARKER);
}
void set_sbg_credit0() __banked{
    set_sgb_palette(SGB_INVENTORY_LIGHTER, SGB_INVENTORY_DARK, SGB_INVENTORY_LIGHT, SGB_INVENTORY_DARKER);
}
void set_sgb_palette01_TITLESCREEN() __banked {
	set_sgb_palette(SGB_TITLE_LIGHTER, SGB_TITLE_LIGHT, SGB_TITLE_DARK, SGB_TITLE_DARKER);
}
void set_sgb_palette01_intro() __banked{
	reset_sgb_palette_statusbar();
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    }SGB_PALETTE_ZEROZEROLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 0u,
        .ndata = 14u,
		.style = 0,
		.ds = {0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000}
    };
    sgb_transfer((void *)&SGB_PALETTE_ZEROZEROLINE_PACKET);
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_ZEROLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 1u,
        .ndata = 14u,
		.style = 0,
		.ds = {0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000}
    };
    sgb_transfer((void *)&SGB_PALETTE_ZEROLINE_PACKET);
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_FIRSTLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 2u,
        .ndata = 14u,
		.style = 0,
		.ds = {0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000}
    };
    sgb_transfer((void *)&SGB_PALETTE_FIRSTLINE_PACKET);
	struct {
        UINT8 command;
        UINT8 xcoo;
        UINT8 ycoo;
		UINT16 ndata;
		UINT8 style;
        UINT8 ds[10];
    } SGB_PALETTE_SECONDLINE_PACKET = {
        .command = (SGB_ATTR_CHR << 3) | 1,
        .xcoo = 0,
        .ycoo = 3u,
        .ndata = 14u,
		.style = 0,
		.ds = {0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000}
    };
    sgb_transfer((void *)&SGB_PALETTE_SECONDLINE_PACKET);
	set_sgb_palette(SGB_PAL1B_LIGHTER, myRGB(255, 187, 17), myRGB(0, 0, 0), SGB_PAL1B_DARKER);

}
void set_sgb_palette01_secret() __banked{
	set_sgb_palette( myRGB(0, 64, 88), myRGB(0, 136, 136), SGB_PAL2H_DARK, myRGB(0, 0, 0));
}
void set_sgb_palette01_worldmap() __banked{
    set_sgb_palette(myRGB(255, 255, 255), myRGB(252, 160, 68), SGB_PAL4B_DARK, SGB_PAL4B_DARKER);
}
void set_sgb_palette_tetra() __banked{
    set_sgb_palette(myRGB(188, 188, 188), myRGB(160, 112, 200), myRGB(104, 48, 144), myRGB(0, 0, 0));
}

void set_sgb_palette02_worldmap() __banked{
    set_sgb_palette(myRGB(255, 255, 255), myRGB(202, 169, 204), myRGB(141, 205, 83), myRGB(17, 56, 15));
}

void set_sgb_worldmap_nw() __banked{
	set_sgb_palette(myRGB(255, 255, 255), myRGB(240, 208, 176), myRGB(0, 120, 0), myRGB(80, 48, 0));
}

void set_sgb_worldmap_e() __banked{
	set_sgb_palette(myRGB(248, 248, 248), myRGB(22, 216, 84), myRGB(0, 120, 0), myRGB(0, 0, 0));
}

void set_sgb_crab() __banked{
	set_sgb_palette(myRGB(252, 252, 252), myRGB(164, 228, 252), myRGB(60, 188, 252), myRGB(0, 64, 88));
}
void set_sgb_harbor() __banked{
	set_sgb_palette(myRGB(252, 252, 252), myRGB(252, 238, 166), myRGB(60, 188, 252), myRGB(0, 64, 88));
}
void set_sgb_palette01_worldmap_maze() __banked{
	set_sgb_palette(myRGB(255, 255, 255), myRGB(240, 208, 176), myRGB(148, 0, 132), myRGB(0, 0, 0));
}
void set_sgb_palette01_WOLF() __banked{
	set_sgb_palette(SGB_PAL2H_LIGHTER, SGB_PAL2H_LIGHT, SGB_PAL2H_DARK, SGB_PAL2H_DARKER);
}
void set_sgb_palette01_BLACKIECAVE() __banked{
    set_sgb_palette(SGB_PAL2H_LIGHTER, SGB_PAL2H_LIGHT, SGB_PALBLUE_DARK, SGB_PAL2H_DARKER);
}
void set_sgb_palette01_MISTYHILLS() __banked{
	set_sgb_palette(SGB_PAL2H_LIGHTER, SGB_PAL2H_LIGHT, myRGB(4, 255, 0), SGB_PAL2H_DARKER);
}
void set_sgb_palette01_TITLEINVERTED() __banked{
	set_sgb_palette(SGB_PAL2A_LIGHTER, SGB_PAL2A_DARK, SGB_PAL2A_LIGHT, SGB_PAL2A_DARKER);
}
void set_sgb_palette01_COMUNEKO() __banked{
	set_sgb_palette(SGB_COMUNEKO_LIGHTER, SGB_COMUNEKO_LIGHT, SGB_COMUNEKO_DARK, SGB_COMUNEKO_DARKER);
}
void set_sgb_palette01_HOOD() __banked{
	set_sgb_palette(myRGB(255, 255, 255), myRGB(168, 224, 56), myRGB(0, 123, 0), myRGB(80, 48, 0));
}

void set_sgb_palette01_OUTWALKER() __banked{
	set_sgb_palette(myRGB(248, 248, 248), myRGB(172, 124, 0), myRGB(0, 123, 0), myRGB(80, 48, 0));
}

void set_sgb_palette01_BATCAVE() __banked{
	set_sgb_palette(myRGB(248, 248, 248), myRGB(188, 188, 188), myRGB(168, 16, 0), myRGB(0, 0, 0));
}
/*
void set_sgb_palette01_MOTHER() __banked{
    set_sgb_palette(SGB_PAL2A_LIGHTER, myRGB(255, 194, 209), myRGB(198, 132, 74), SGB_PAL2A_DARKER);
}
*/
void set_sgb_SKY() __banked{
    set_sgb_palette(myRGB(220, 220, 220), myRGB(248, 216, 120), myRGB(120, 120, 120), myRGB(0, 0, 0));
}
void set_sgb_palette01_ZOO() __banked {
	set_sgb_palette(SGB_PAL1E_LIGHTER, SGB_PAL4B_LIGHT, SGB_PAL4B_DARK, SGB_PAL4B_DARKER);
}
void set_sgb_palette01_MINE() __banked {
	set_sgb_palette(SGB_PAL1E_LIGHTER, myRGB(252, 160, 68), myRGB(172, 124, 0), myRGB(90, 57, 33));
}
void set_sgb_palette01_TUTORIAL() __banked {
	set_sgb_palette(SGB_PAL1E_LIGHTER, myRGB(252, 160, 68), myRGB(172, 124, 0), myRGB(0, 0, 0));
}
void set_sgb_palette01_CEMATERYCRYPT() __banked {
	set_sgb_palette(myRGB(255, 255, 255), myRGB(184, 184, 184), myRGB(176, 62, 255), myRGB(40, 0, 136));
}
void set_sgb_palette01_MAZE() __banked{
    set_sgb_palette(myRGB(220, 220, 220), myRGB(160, 160, 160), myRGB(0, 128, 128), myRGB(86, 56, 32));
}
void set_sgb_palette01_cart()__banked{
    set_sgb_palette(myRGB(255, 134, 0), myRGB(255, 189, 0), myRGB(255, 57, 0), myRGB(0, 0, 0));
}
void set_sgb_palette01_SILVER() __banked{
    set_sgb_palette(myRGB(255, 255, 255), myRGB(210, 249, 253), myRGB(105, 105, 105), myRGB(0, 0, 0));
}
/*
void set_sgb_palette01_AMULET_THUNDER() __banked{
	set_sgb_palette(SGB_PAL1B_LIGHTER, myRGB(255, 187, 17), myRGB(198, 132, 74), SGB_PAL1B_DARKER);
}
void set_sgb_palette01_AMULET_ICE() __banked{
	set_sgb_palette(myRGB(255, 255, 173), myRGB(248, 248, 248), myRGB(60, 188, 252), SGB_PAL1B_DARKER);
}
void set_sgb_palette01_SEWER() __banked {
	set_sgb_palette(myRGB(188, 188, 188), myRGB(152, 120, 248), myRGB(0, 64, 88), myRGB(0, 0, 0));
}
void set_sgb_palette01_GATORSWAMP() __banked {
	set_sgb_palette(SGB_PAL1E_LIGHTER, SGB_PAL1E_LIGHT, myRGB(0, 136, 136), SGB_PAL1E_DARKER);
}
void set_sgb_palette01_GATOR() __banked {
	set_sgb_palette(SGB_PAL2A_LIGHTER, SGB_PAL2A_DARK, SGB_PAL2A_DARK, SGB_PAL2A_DARKER);
}
void set_sgb_palette01_FOREST() __banked {
	set_sgb_palette(SGB_PAL1D_LIGHTER, SGB_PAL1D_LIGHT, myRGB(82, 24, 0), myRGB(0, 0, 0));
}
void set_sgb_palette01_EAGLE() __banked {
	set_sgb_palette(SGB_PAL1F_LIGHTER, SGB_PAL1F_LIGHT, myRGB(172, 124, 0), myRGB(0, 0, 0));
}
void set_sgb_palette01_IBEX() __banked {
	set_sgb_palette( myRGB(224, 232, 1),  myRGB(168, 168, 88), myRGB(176, 176, 176), myRGB(0, 0, 0));
}
void set_sgb_palette01_BEAR() __banked {
	set_sgb_palette(SGB_PAL1E_LIGHTER, myRGB(228, 92, 16), myRGB(80, 48, 0), myRGB(90, 57, 33));
}
void set_sgb_palette01_ICE() __banked {
	set_sgb_palette(myRGB(255, 255, 255), myRGB(164, 228, 252), myRGB(29, 176, 249), myRGB(0, 0, 0));
}
void set_sgb_palette01_WALRUS() __banked {
	set_sgb_palette(myRGB(255, 255, 173), myRGB(164, 228, 252), myRGB(172, 124, 0), myRGB(90, 57, 33));
}
void set_sgb_palette01_CASTLE() __banked{
    set_sgb_palette(myRGB(220, 220, 220), myRGB(160, 160, 160), myRGB(0, 128, 128), myRGB(86, 56, 32));
}
void set_sgb_palette01_BOSS() __banked{
    set_sgb_palette(myRGB(255, 255, 255), myRGB(192, 208, 240), myRGB(255, 168, 0), myRGB(80, 8, 88));
}
void set_sgb_palette01_1A() __banked {
	set_sgb_palette(SGB_PAL1A_LIGHTER, SGB_PAL1A_LIGHT, SGB_PAL1A_DARK, SGB_PAL1A_DARKER);
}

void set_sgb_palette01_1B() __banked {
	set_sgb_palette(SGB_PAL1B_LIGHTER, myRGB(255, 187, 17), myRGB(100, 100, 100), SGB_PAL1B_DARKER);
}

void set_sgb_palette01_1D() __banked {
	set_sgb_palette(SGB_PAL1D_LIGHTER, SGB_PAL1D_LIGHT, SGB_PAL1D_DARK, SGB_PAL1D_DARKER);
}

void set_sgb_palette01_1E() __banked {
	set_sgb_palette(SGB_PAL1E_LIGHTER, SGB_PAL1E_LIGHT, SGB_PAL1E_DARK, SGB_PAL1E_DARKER);
}

void set_sgb_palette01_1G() __banked {
	set_sgb_palette(SGB_PAL1G_LIGHTER, SGB_PAL1G_LIGHT, SGB_PAL1G_DARK, SGB_PAL1G_DARKER);
}

void set_sgb_palette01_1F() __banked {
	set_sgb_palette(SGB_PAL1F_LIGHTER, SGB_PAL1F_LIGHT, SGB_PAL1F_DARK, SGB_PAL1F_DARKER);
}

void set_sgb_palette01_2A() __banked {
	set_sgb_palette(SGB_PAL2A_LIGHTER, SGB_PAL2A_LIGHT, SGB_PAL2A_DARK, SGB_PAL2A_DARKER);
}

void set_sgb_palette01_2B() __banked {
	set_sgb_palette(SGB_PAL2B_LIGHTER, SGB_PAL2B_LIGHT, SGB_PAL2B_DARK, SGB_PAL2B_DARKER);
}

void set_sgb_palette01_2E() __banked {
	set_sgb_palette(SGB_PAL2E_LIGHTER, SGB_PAL2E_LIGHT, SGB_PAL2E_DARK, SGB_PAL2E_DARKER);
}

void set_sgb_palette01_2H() __banked {
	set_sgb_palette(SGB_PAL2H_LIGHTER, SGB_PAL2H_LIGHT, SGB_PAL2H_DARK, SGB_PAL2H_DARKER);
}

void set_sgb_palette01_3C() __banked {
	set_sgb_palette(SGB_PAL3C_LIGHTER, SGB_PAL3C_LIGHT, SGB_PAL3C_DARK, SGB_PAL3C_DARKER);
}

void set_sgb_palette01_4B() __banked {
	set_sgb_palette(SGB_PAL4B_LIGHTER, SGB_PAL4B_LIGHT, SGB_PAL4B_DARK, SGB_PAL4B_DARKER);
}
*/
