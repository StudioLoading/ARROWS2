#include "Banks/SetAutoBank.h"

#include "Keys.h"
#include "ZGBMain.h"
#include "string.h"
#include "Music.h"
#include "Scroll.h"

#include "custom_datas.h"


unsigned char d1[21];
unsigned char d2[21];
unsigned char d3[21];
unsigned char d4[21];


UINT8 Build_Next_Dialog_Banked(Sprite* archer) BANKED;
void copy_strings_to_dn(char* s1, char* s2, char* s3, char* s4);


void copy_strings_to_dn(char* s1, char* s2, char* s3, char* s4){
	memcpy(d1, s1, 20);
	memcpy(d2, s2, 20);
	memcpy(d3, s3, 20);
	memcpy(d4, s4, 20);
}

UINT8 Build_Next_Dialog_Banked(Sprite* archer) BANKED{
	UINT8 diagf = 0u;
	copy_strings_to_dn("                    ","                    ","                    ","                    ");
	
}
