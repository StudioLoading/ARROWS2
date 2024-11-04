#include <gb/gb.h>

#define myRGB(r, g, b) (((UINT16)((b) >> 3) << 10) | ((UINT16)((g) >> 3) << 5) | ((UINT16)((r) >> 3) << 0))

#define SGB_PALBLUE_DARK      myRGB(0, 0, 252)

#define SGB_TITLE_LIGHTER     myRGB(255, 255, 255)
#define SGB_TITLE_LIGHT       myRGB(104, 136, 252)
#define SGB_TITLE_DARK        myRGB(0, 0, 188)
#define SGB_TITLE_DARKER      myRGB(0, 0, 0)

#define SGB_COMUNEKO_LIGHTER  myRGB(250, 250, 250)
#define SGB_COMUNEKO_LIGHT    myRGB(180, 219, 214)
#define SGB_COMUNEKO_DARK     myRGB(193, 166, 224)
#define SGB_COMUNEKO_DARKER	myRGB(0, 0, 0)

#define SGB_STATUS_LIGHT      myRGB(248, 184, 0)
#define SGB_STATUS_DARK       myRGB(0, 88, 248)
#define SGB_STATUS_DARKER 	myRGB(0, 0, 0)
#define SGB_STATUS_RED	 	myRGB(254, 0, 0)

#define SGB_INVENTORY_LIGHTER myRGB(250, 250, 250)
#define SGB_INVENTORY_LIGHT   myRGB(240, 208, 176)
#define SGB_INVENTORY_DARK    myRGB(228,  92,  16)
#define SGB_INVENTORY_DARKER  myRGB(0, 0, 0)


#define SGB_PAL1A_LIGHTER 	myRGB(255, 232, 207)
#define SGB_PAL1A_LIGHT 	myRGB(223, 144, 79)
#define SGB_PAL1A_DARK		myRGB(175, 40, 32)
#define SGB_PAL1A_DARKER 	myRGB(48, 24, 80)

#define SGB_PAL1B_LIGHTER 	myRGB(223, 216, 192)
#define SGB_PAL1B_LIGHT 	myRGB(207, 176, 112)
#define SGB_PAL1B_DARK		myRGB(176, 80, 16)
#define SGB_PAL1B_DARKER 	myRGB(0, 0, 0)

#define SGB_PAL1D_LIGHTER 	myRGB(255, 248, 175)
#define SGB_PAL1D_LIGHT 	myRGB(192, 128, 79)
#define SGB_PAL1D_DARK		myRGB(255, 0, 0)
#define SGB_PAL1D_DARKER 	myRGB(80, 24, 0)

#define SGB_PAL1E_LIGHTER 	myRGB(255, 248, 175)
#define SGB_PAL1E_LIGHT 	myRGB(127, 192, 127)
#define SGB_PAL1E_DARK		myRGB(111, 136, 64)
#define SGB_PAL1E_DARKER 	myRGB(95, 56, 32)

#define SGB_PAL1G_LIGHTER 	myRGB(255, 248, 95)
#define SGB_PAL1G_LIGHT 	myRGB(127, 120, 0)
#define SGB_PAL1G_DARK		myRGB(0, 160, 239)
#define SGB_PAL1G_DARKER 	myRGB(0, 0, 80)

#define SGB_PAL1F_LIGHTER 	myRGB(223, 232, 255)
#define SGB_PAL1F_LIGHT 	myRGB(224, 136, 80)
#define SGB_PAL1F_DARK		myRGB(175, 0, 0)
#define SGB_PAL1F_DARKER 	myRGB(0, 64, 16)

#define SGB_PAL2A_LIGHTER 	myRGB(240, 200, 160)
#define SGB_PAL2A_LIGHT 	myRGB(192, 136, 79)
#define SGB_PAL2A_DARK		myRGB(47, 120, 0)
#define SGB_PAL2A_DARKER 	myRGB(0, 0, 0)

#define SGB_PAL2B_LIGHTER 	myRGB(255, 248, 255)
#define SGB_PAL2B_LIGHT 	myRGB(191, 184, 191)
#define SGB_PAL2B_DARK		myRGB(112, 112, 112)
#define SGB_PAL2B_DARKER 	myRGB(0, 0, 0)

#define SGB_PAL2E_LIGHTER 	myRGB(255, 248, 128)
#define SGB_PAL2E_LIGHT 	myRGB(144, 176, 224)
#define SGB_PAL2E_DARK		myRGB(47, 16, 96)
#define SGB_PAL2E_DARKER 	myRGB(16, 8, 16)

#define SGB_PAL2H_LIGHTER 	myRGB(255, 248, 255)
#define SGB_PAL2H_LIGHT 	myRGB(191, 184, 191)
#define SGB_PAL2H_DARK		myRGB(112, 112, 112)
#define SGB_PAL2H_DARKER 	myRGB(0, 0, 0)
 
#define SGB_PAL3C_LIGHTER 	myRGB(224, 168, 207)
#define SGB_PAL3C_LIGHT 	myRGB(255, 248, 127)
#define SGB_PAL3C_DARK		myRGB(0, 184, 255)
#define SGB_PAL3C_DARKER 	myRGB(32, 32, 95)

#define SGB_PAL4B_LIGHTER 	myRGB(240, 232, 240)
#define SGB_PAL4B_LIGHT 	myRGB(239, 160, 96)
#define SGB_PAL4B_DARK		myRGB(64, 120, 63)
#define SGB_PAL4B_DARKER 	myRGB(31, 8, 15)
/*
void set_sgb_palette_0() __banked;
void set_sgb_palette_1() __banked;
void set_sgb_palette_2() __banked;
void set_sgb_palette_3() __banked;
void set_sgb_palette_4() __banked;
void set_sgb_palette_5() __banked;
void set_sgb_palette_6() __banked;*/

#define SGB_PAL2_LIGHTER 	myRGB(255, 255, 255)
#define SGB_PAL2_LIGHT 	      myRGB(160, 160, 160)
#define SGB_PAL2_DARK		myRGB(192, 128, 0)
#define SGB_PAL2_DARKER 	myRGB(0, 0, 0)

#define SGB_PAL7_LIGHTER 	myRGB(240, 240, 240)
#define SGB_PAL7_LIGHT 	      myRGB(160, 160, 160)
#define SGB_PAL7_DARK		myRGB(0, 128, 128)
#define SGB_PAL7_DARKER 	myRGB(96, 56, 32)

void set_sgb_palette_overworldsw() __banked;
void set_sgb_palette_2() __banked;
void set_sgb_palette_inventory() __banked;

void set_sgb_palette_title() __banked;
void reset_sgb_palette_title() __banked;
void set_sgb_palette_statusbar() __banked;
void reset_sgb_palette_statusbar() __banked;

void set_sgb_palette01_TITLESCREEN() __banked;
void set_sgb_palette01_MISTYHILLS() __banked;
void set_sgb_palette01_intro() __banked;
void set_sgb_palette01_secret() __banked;
void set_sgb_palette01_worldmap() __banked;
void set_sgb_palette02_worldmap() __banked;
void set_sgb_worldmap_nw() __banked;
void set_sgb_worldmap_e() __banked;
void set_sgb_crab() __banked;
void set_sgb_harbor() __banked;
void set_sgb_palette_tetra() __banked;
void set_sgb_palette01_worldmap_maze() __banked;
void set_sgb_palette01_WOLF() __banked;
void set_sgb_palette01_BLACKIECAVE() __banked;
void set_sgb_palette01_TITLEINVERTED() __banked;
void set_sgb_palette01_COMUNEKO() __banked;
void set_sgb_palette01_TUTORIAL() __banked;
void set_sgb_palette01_MINE() __banked;
void set_sgb_palette01_ZOO() __banked;
void set_sgb_palette01_CEMATERYCRYPT() __banked;
void set_sgb_palette01_HOOD() __banked;
void set_sgb_palette01_OUTWALKER() __banked;
void set_sgb_palette01_MAZE() __banked;
void set_sbg_credit0() __banked;
void set_sgb_SKY() __banked;
void set_sgb_palette01_cart()__banked;
void set_sgb_palette01_SILVER() __banked;
void set_sgb_palette01_BATCAVE() __banked;
/*
void set_sgb_palette01_SEWER() __banked;
void set_sgb_palette01_GATORSWAMP() __banked;
void set_sgb_palette01_GATOR() __banked;
void set_sgb_palette01_FOREST() __banked;
void set_sgb_palette01_EAGLE() __banked;
void set_sgb_palette01_IBEX() __banked;
void set_sgb_palette01_AMULET_THUNDER() __banked;
void set_sgb_palette01_AMULET_ICE() __banked;
void set_sgb_palette01_BEAR() __banked;
void set_sgb_palette01_ICE() __banked;
void set_sgb_palette01_WALRUS() __banked;
void set_sgb_palette01_CASTLE() __banked;
void set_sgb_palette01_MOTHER() __banked;
void set_sgb_palette01_BOSS() __banked;

void set_sgb_palette01_1A() __banked;	
void set_sgb_palette01_1B() __banked;
void set_sgb_palette01_1D() __banked;
void set_sgb_palette01_1E() __banked;
void set_sgb_palette01_1G() __banked;
void set_sgb_palette01_1F() __banked;
void set_sgb_palette01_2A() __banked;
void set_sgb_palette01_2B() __banked;
void set_sgb_palette01_2E() __banked;
void set_sgb_palette01_2H() __banked;
void set_sgb_palette01_3C() __banked;
void set_sgb_palette01_4B() __banked;
*/

/*
Color Palette 1-A
704 - 255 232 207
      223 144  79
      175  40  32
       48  24  80

 

Color Palette 1-B
705 - 223 216 192
      207 176 112
      176  80  16
        0   0   0

 

Color Palette 1-C
706 - 255 192 255
      239 152  80
      159  56  96
       63  56 159

 

Color Palette 1-D
707 - 255 248 175
      192 128  79
      255   0   0
       80  24   0
 

Color Palette 1-E
708 - 255 248 175
      127 192 127
      111 136  64
       95  56  32
 

Color Palette 1-F
709 - 223 232 255
      224 136  80
      175   0   0
        0  64  16
 

Color Palette 1-G
710 -   0   0  80
        0 160 239
      127 120   0
      255 248  95

 

Color Palette 1-H
711 - 255 232 224
      255 184 143
      128  64   0
       48  24   0
 

Color Palette 2-A
712 - 240 200 160
      192 136  79
       47 120   0
        0   0   0

 

Color Palette 2-B
713 - 255 248 255
      255 232  80
      255  48   0
       80   0  95
 

Color Palette 2-C
714 - 255 192 255
      239 136 143
      127  48 239
       47  40 159
 

Color Palette 2-D
715 - 255 248 160
        0 248   0
      255  48   0
        0   0  80
 

Color Palette 2-E
716 - 255 248 128
      144 176 224
       47  16  96
       16   8  16
 

Color Palette 2-F
717 - 208 248 255
      255 144  80
      160   0   0
       31   0   0
 

Color Palette 2-G
718 - 111 184  63
      224  80  64
      224 184 128
        0  24   0

 

Color Palette 2-H
719 - 255 248 255
      191 184 191
      112 112 112
        0   0   0
 

Color Palette 3-A
255 208 159
      112 192 192
      255  96  47
       48  72  96

 

Color Palette 3-B
223 216 192
      224 128  32
        0  80   0
        0  16  16

 

Color Palette 3-C
224 168 207
      255 248 127
        0 184 255
       32  32  95

 

Color Palette 3-D
 240 248 191
      224 168 127
       15 200   0
        0   0   0

 

Color Palette 3-E
255 248 191
      224 176 111
      176 120  32
       80  72 112


 

Color Palette 3-F
 127 120 207
      255 104 255
      255 208   0
       64  64  64

 

Color Palette 3-G
  96 216  80
      255 248 255
      207  48  63
       63   0   0

 

Color Palette 3-H
 224 248 160
      127 200  63
       79 136  31
       15  24   0

 

Color Palette 4-A
 240 168 111
      127 168 255
      208   0 208
        0   0 127
 

Color Palette 4-B
240 232 240
      239 160  96
       64 120  63
       31   8  15
 

Color Palette 4-C
 255 224 224
      223 160 208
      159 160 224
       15   0   0

 

Color Palette 4-D
255 248 191
      144 200 207
       79 104 127
       15  32  79	
 

Color Palette 4-E
 255 248 191
      144 200 207
       79 104 124
       15  32  79
 

Color Palette 4-F
 191 208 208
      223 128 223
      128   0 160
       63   0   0
 

Color Palette 4-G
 176 224  31
      191  32  95
       47  16   0
        0 128  96
 

Color Palette 4-H
255 248 207
      191 192  95
      128 136  64
       64  80  47
	  
	  */
