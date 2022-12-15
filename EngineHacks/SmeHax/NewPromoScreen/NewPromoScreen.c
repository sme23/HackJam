
#include "NewPromoScreen.h"

u32 NewPromoScreen_OnHover(MenuProc* menuProc, MenuCommandProc* commandProc) {
	
	//parent->parent is ccramify
	//parent is CCRamifyMenuSelect
	
	CCRamifyMenuSelectProc* parent1 = (CCRamifyMenuSelectProc*) menuProc->parent;
	CCRamifyProc* parent2 = (CCRamifyProc*) parent1->parent;
	
	parent2->procFields2[0] = 1;
	parent2->procFields2[1] = commandProc->commandDefinitionIndex;
	
	//asm("mov r11, r11");
	
	u16 promotedClass = parent2->procFields1[1+commandProc->commandDefinitionIndex];
	
	//asm("mov r11, r11");
	
	ClassData* promotedClassEntry = &gClassData[promotedClass];
	
	//asm("mov r11, r11");
	
	u16 firstClass = parent2->procFields1[1];
	ClassData* firstClassEntry = &gClassData[firstClass];
	
	
	LoadIconPalette(1, 0xD);
	LoadIconPalette(0, 0xE);
	
	//kill all the existing icons
	DrawIcon(&gBG0MapBuffer[12][9],-1,0);
	DrawIcon(&gBG0MapBuffer[12][11],-1,0);
	DrawIcon(&gBG0MapBuffer[12][13],-1,0);
	DrawIcon(&gBG0MapBuffer[12][15],-1,0);
	DrawIcon(&gBG0MapBuffer[12][17],-1,0);
	DrawIcon(&gBG0MapBuffer[12][19],-1,0);
	DrawIcon(&gBG0MapBuffer[12][21],-1,0);
	DrawIcon(&gBG0MapBuffer[12][23],-1,0);
	DrawIcon(&gBG0MapBuffer[12][25],-1,0);
	DrawIcon(&gBG0MapBuffer[12][27],-1,0);

	
	int classIcon = 0xFF;
	int attributes = promotedClassEntry->attributes;
	if ((attributes & CA_MOUNTED) != 0) classIcon = 0;
		if ((attributes & CA_PEGASUS) != 0) classIcon = 1;
		
		if ((attributes & CA_WYVERN) != 0) classIcon = 2;
	
	if (classIcon != 0xFF) {
		DrawIcon(&gBG0MapBuffer[12][25],classIcon|0x300,0xD000);
	}

	//draw class skill icon @ ~(26,16)
	if (ClassSkillTable[promotedClass] != 0) {
		DrawIcon(&gBG0MapBuffer[12][27],ClassSkillTable[promotedClass]|0x100,0xE000);
	}

	//draw icons for weapons w/o 0 base rank  
	//on promoted class @ ~(16,16), padded 
	//not padded actually because lol we have room for like 3 if we pad
	int wtIconOffset = 9;
	int wtIconIndex = 0;
	while (wtIconIndex < 8) {
		if (promotedClassEntry->baseRanks[wtIconIndex] != 0) {
			DrawIcon(&gBG0MapBuffer[12][wtIconOffset],wtIconIndex|0x400,0xD000);
			wtIconOffset += 2;
		}
		wtIconIndex ++;
	}
	

	//the following is done to draw text to the screen for class description
	//let's make our own replacement function

	//s16 index = parent2->procFields1[7 + commandProc->commandDefinitionIndex];
	//ChangeClassDescription(index);
	
	DisplayPromotionBonuses(promotedClassEntry,(CCRamifyParentProc*) parent2->parent);
	
	//GetStringFromIndex(firstClassEntry.nameTextId); //this fixes things for some reason
	
	return 0; //Dialogue_SetCharacterDisplayDelay(-1);
	
	
}


void DisplayPromotionBonuses(ClassData* classEntry, CCRamifyParentProc* proc) {
	
	EnableBgSyncByMask(1);
	BgMapFillRect(&gBG0MapBuffer[14][0],32,16,0);
	gpCurrentFont->tileNext = 36;

	/*
	u8 *gSpecialUiCharAllocationTable = (u8 *)0x02028E78;
	gSpecialUiCharAllocationTable = (u8) 0xFF; //this is gSpecialUiCharAllocationTable except works with default clib
	*/
	gSpecialUiCharAllocationTable = 0xFF;
	
	//we want to draw 9 text IDs to the screen first
	//these are reused vanilla stat screen text so we can hardcode the IDs
	
	
	TextHandle HPHandle = {
//		.tileIndexOffset = gpCurrentFont->tileNext,
//		.tileWidth = 2
		.xCursor = 0
	};

	TextHandle StrHandle = {
//		.tileIndexOffset = gpCurrentFont->tileNext,
//		.tileWidth = 3
		.xCursor = 3
	};
	TextHandle MagHandle = {
//		.tileIndexOffset = gpCurrentFont->tileNext,
//		.tileWidth = 3
		.xCursor = 0
	};
	TextHandle SklHandle = {
//		.tileIndexOffset = gpCurrentFont->tileNext,
//		.tileWidth = 3
		.xCursor = 0
	};
	TextHandle SpdHandle = {
//		.tileIndexOffset = gpCurrentFont->tileNext,
//		.tileWidth = 3
		.xCursor = 0
	};
	TextHandle DefHandle = {
//		.tileIndexOffset = gpCurrentFont->tileNext,
//		.tileWidth = 3
		.xCursor = 0
	};
	TextHandle ResHandle = {
//		.tileIndexOffset = gpCurrentFont->tileNext,
//		.tileWidth = 3
		.xCursor = 0
	};
	TextHandle MovHandle = {
//		.tileIndexOffset = gpCurrentFont->tileNext,
//		.tileWidth = 3
		.xCursor = 0
	};
	TextHandle ConHandle = {
//		.tileIndexOffset = gpCurrentFont->tileNext,
//		.tileWidth = 3
		.xCursor = 0
	};
	

	
	const char* textID = GetStringFromIndex(HP_TEXT_ID);
	Text_Clear(&HPHandle);
	Text_InitClear(&HPHandle, 2);
	Text_DrawString(&HPHandle, textID);
	Text_Display(&HPHandle, &gBG0MapBuffer[15][2]);
	
	textID = GetStringFromIndex(STR_TEXT_ID);
	Text_Clear(&StrHandle);
	Text_InitClear(&StrHandle, 3);
	StrHandle.xCursor = 3;
	Text_DrawString(&StrHandle, textID);
	Text_Display(&StrHandle, &gBG0MapBuffer[15][4]);
	
	textID = GetStringFromIndex(MAG_TEXT_ID);
	Text_Clear(&MagHandle);
	Text_InitClear(&MagHandle, 3);
	Text_DrawString(&MagHandle, textID);
	Text_Display(&MagHandle, &gBG0MapBuffer[15][7]);
	
	textID = GetStringFromIndex(SKL_TEXT_ID);
	Text_Clear(&SklHandle);
	Text_InitClear(&SklHandle, 3);
	Text_DrawString(&SklHandle, textID);
	Text_Display(&SklHandle, &gBG0MapBuffer[15][10]);
	
	textID = GetStringFromIndex(SPD_TEXT_ID);
	Text_Clear(&SpdHandle);
	Text_InitClear(&SpdHandle, 2);
	Text_DrawString(&SpdHandle, textID);
	Text_Display(&SpdHandle, &gBG0MapBuffer[15][13]);
	
	textID = GetStringFromIndex(DEF_TEXT_ID);
	Text_Clear(&DefHandle);
	Text_InitClear(&DefHandle, 2);
	Text_DrawString(&DefHandle, textID);
	Text_Display(&DefHandle, &gBG0MapBuffer[15][16]);
	
	textID = GetStringFromIndex(RES_TEXT_ID);
	Text_Clear(&ResHandle);
	Text_InitClear(&ResHandle, 2);
	Text_DrawString(&ResHandle, textID);
	Text_Display(&ResHandle, &gBG0MapBuffer[15][19]);
	
	textID = GetStringFromIndex(MOV_TEXT_ID);
	Text_Clear(&MovHandle);
	Text_InitClear(&MovHandle, 3);
	Text_DrawString(&MovHandle, textID);
	Text_Display(&MovHandle, &gBG0MapBuffer[15][22]);
	
	textID = GetStringFromIndex(CON_TEXT_ID);
	Text_Clear(&ConHandle);
	Text_InitClear(&ConHandle, 2);
	Text_DrawString(&ConHandle, textID);
	Text_Display(&ConHandle, &gBG0MapBuffer[15][26]);
	
	
	//asm("mov r11, r11");
	//now we get promo bonuses and display them below each label
	
	DrawUiNumberOrDoubleDashes(&gBG0MapBuffer[17][2],TEXT_COLOR_GOLD,classEntry->promotionHp);

	
	if (IsStrMagSplitPresent) {	
		DrawUiNumberOrDoubleDashes(&gBG0MapBuffer[17][5],TEXT_COLOR_GOLD,classEntry->promotionPow);
		DrawUiNumberOrDoubleDashes(&gBG0MapBuffer[17][8],TEXT_COLOR_GOLD,MagClassTable[classEntry->number].promotionMag);
	}
	else {
		if ((classEntry->baseRanks[ITYPE_STAFF] + classEntry->baseRanks[ITYPE_ANIMA] + classEntry->baseRanks[ITYPE_LIGHT] + classEntry->baseRanks[ITYPE_DARK]) != 0) {
			
			DrawUiNumberOrDoubleDashes(&gBG0MapBuffer[17][5],TEXT_COLOR_GOLD,0);
			DrawUiNumberOrDoubleDashes(&gBG0MapBuffer[17][8],TEXT_COLOR_GOLD,classEntry->promotionPow);
		} 
		else {
		DrawUiNumberOrDoubleDashes(&gBG0MapBuffer[17][5],TEXT_COLOR_GOLD,classEntry->promotionPow);
		DrawUiNumberOrDoubleDashes(&gBG0MapBuffer[17][8],TEXT_COLOR_GOLD,0);		}
	}
	
	DrawUiNumberOrDoubleDashes(&gBG0MapBuffer[17][11],TEXT_COLOR_GOLD,classEntry->promotionSkl);
	
	DrawUiNumberOrDoubleDashes(&gBG0MapBuffer[17][14],TEXT_COLOR_GOLD,classEntry->promotionSpd);
	
	DrawUiNumberOrDoubleDashes(&gBG0MapBuffer[17][17],TEXT_COLOR_GOLD,classEntry->promotionDef);
	
	DrawUiNumberOrDoubleDashes(&gBG0MapBuffer[17][20],TEXT_COLOR_GOLD,classEntry->promotionRes);
	
	CCRamifyParentProc* ssItemUseProc = (CCRamifyParentProc*) ProcFind((ProcInstruction*) 0x8A19064);
	ClassData* unpromotedClass = 0;

	if (ssItemUseProc == 0) {
	//when not at prep screen, this is our current unit/class
		unpromotedClass = proc->unit->pClassData;
	} else {
		unpromotedClass = ssItemUseProc->unit->pClassData;
	}
	
	
	DrawUiNumberOrDoubleDashes(&gBG0MapBuffer[17][24],TEXT_COLOR_GOLD,(classEntry->baseMov - unpromotedClass->baseMov));
		
	DrawUiNumberOrDoubleDashes(&gBG0MapBuffer[17][27],TEXT_COLOR_GOLD,(classEntry->baseCon - unpromotedClass->baseCon));

	
	
	GetStringFromIndex(0); //this is necessary to fix something for some reason
	
}


