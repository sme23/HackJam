
#include "gbafe.h"

extern void ChangeClassDescription(s16 value);
extern u32 Dialogue_SetCharacterDisplayDelay(u32 value);

extern u16 gBG0MapBuffer[32][32]; // 0x02022CA8. Ew why does FE-CLib-master not do it like this?
extern u16 gBG1MapBuffer[32][32]; // 0x020234A8.
extern u16 gBG2MapBuffer[32][32]; // 0x02023CA8.
extern u8 gSpecialUiCharAllocationTable;

// likely need to add the following to your clib `.s` reference
/*
SET_DATA gBG0MapBuffer, 0x02022CA8
SET_DATA gBG1MapBuffer, 0x020234A8
SET_DATA gBG2MapBuffer, 0x02023CA8
SET_DATA gSpecialUiCharAllocationTable, 0x02028E78

*/

typedef struct CCRamifyProc CCRamifyProc;
typedef struct CCRamifyMenuSelectProc CCRamifyMenuSelectProc;
typedef struct CCRamifyParentProc CCRamifyParentProc;

u32 NewPromoScreen_OnHover(MenuProc* menuProc, MenuCommandProc* commandProc);
void DisplayPromotionBonuses(ClassData* classEntry, CCRamifyParentProc* proc);

struct CCRamifyProc {

	/* 00 */ PROC_HEADER;

	/* 2A */ s16 procFields1[11];
	/* 40 */ u8 procFields2[10];	
	
};

struct CCRamifyMenuSelectProc {

	/* 00 */ PROC_HEADER;

	/* 2A */ u8 procFields[32];
	
};

struct CCRamifyParentProc {
	/* 00 */ PROC_HEADER;

	/* 2A */ Unit* unit;
};

extern u8 ClassSkillTable[256];

#define HP_TEXT_ID 0x4E9
#define STR_TEXT_ID 0x4FE
#define MAG_TEXT_ID 0x4FF
#define SKL_TEXT_ID 0x4EC
#define SPD_TEXT_ID 0x4ED
#define DEF_TEXT_ID 0x4EF
#define RES_TEXT_ID 0x4F0
#define MOV_TEXT_ID 0x4F6
#define CON_TEXT_ID 0x4F7

extern bool IsStrMagSplitPresent;

struct MagClassTableEntry {
	u8 baseMagic;
	u8 growthMagic;
	u8 capMagic;
	u8 promotionMag;
	
	
};

extern struct MagClassTableEntry MagClassTable[256];
