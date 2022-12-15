
#include "StartingOptionsMenu.h"

// lovingly borrowed from circles' self-rando
static const ProcCode StartingOptionsProc[] = {
  PROC_SET_NAME("StartingOptions"),
  PROC_CALL_ROUTINE(LockGameLogic),
  PROC_END_ALL(0x8a20b1c),
  // PROC_END_ALL(0x8a206a8), //savemenu drawing

  PROC_CALL_ROUTINE(StartingOptionsSetup),
  // PROC_CALL_ROUTINE(0x80b1a09), //original config drawing

    PROC_CALL_ROUTINE_ARG(NewFadeIn, 8),
    PROC_WHILE_ROUTINE(FadeInExists),
    PROC_SLEEP(1),

  PROC_LOOP_ROUTINE(StartingOptionsLoop), //wait for B button
  
  PROC_CALL_ROUTINE_ARG(NewFadeOut, 0x10),
  PROC_WHILE_ROUTINE(FadeOutExists),
  PROC_SLEEP(10),

  PROC_END
};

static const ProcCode SpinProc[] = {
  PROC_SET_NAME("SpinnyBoi"),
  PROC_SET_MARK(0xD),
  PROC_CALL_ROUTINE(&SpinRoutine1),
  PROC_LOOP_ROUTINE(&SpinRoutine2),
  PROC_END
};

static const ProcCode NewGameDifficultySelect[] = {
  PROC_SET_NAME("DifficultySelect"),

  PROC_SET_DESTRUCTOR(0x80ac078+1),
  PROC_CALL_ROUTINE(0x80ad5b4+1),
  PROC_YIELD,
  PROC_CALL_ROUTINE(0x80ac1a8+1),
  PROC_SLEEP(1),
  PROC_CALL_ROUTINE(EnableAllGfx),//EnableAllGfx
  PROC_CALL_ROUTINE_ARG(NewFadeIn, 8),
  PROC_WHILE_ROUTINE(FadeInExists),
  PROC_LABEL(0),
  PROC_LOOP_ROUTINE(0x80ac288+1),
  PROC_LABEL(1),
    PROC_CALL_ROUTINE_ARG(NewFadeOut, 8),
    PROC_WHILE_ROUTINE(FadeOutExists),
    PROC_SLEEP(10),
    
      PROC_NEW_CHILD(SpinProc), //one spinny boi

    PROC_NEW_CHILD_BLOCKING(StartingOptionsProc),

    // PROC_NEW_CHILD_BLOCKING(0x8a2ece0), //config proc
    PROC_SLEEP(10),

  PROC_LABEL(2),
  PROC_CALL_ROUTINE_ARG(NewFadeOut, 8),
  PROC_WHILE_ROUTINE(FadeOutExists),
  PROC_CALL_ROUTINE(nullsub_64),

  PROC_YIELD,

  PROC_CALL_ROUTINE(0x80a8c2c+1),
  PROC_YIELD,
  PROC_CALL_ROUTINE(0x80a8cd4+1),
  PROC_CALL_ROUTINE(0x80a8f04+1),
  PROC_YIELD,
  PROC_CALL_ROUTINE(UnlockGameLogic),
  PROC_END
};


static const LocationTable CursorLocationTable[] = {
  {10, 0x18},
  {10, 0x28},
  {10, 0x38},
  {10, 0x48},
  {10, 0x58},
  {10, 0x68},
  {10, 0x78} //,
  // {10, 0x88} //leave room for a description?
};

//POIN to this at $a20164
void NewDifficultySelectFunc(ProcState* input){
  ProcStartBlocking(NewGameDifficultySelect, input);
};

void StartingOptionsSetup(OptionsProc* CurrentProc){
	//set up bg graphics
	ClearBG0BG1();
	EnableBgSyncByIndex(0);
	CpuSet(0x859ED70, (0x020228A8 + 16 * 0x20), 0x20); //ui palette


	CpuSet(0x8b1754c+0x20, (0x020228A8 + 8 * 0x20), 0x20); //bg palette

	VBlankIntrWait();
	LZ77UnCompVram(0x8b12db4, 0x6003000); //bg (changed from 0x6008000)
	GenerateBGTsa((u16*)BG1Buffer, 0x280, 8, 0x180); //was BG1Buffer
	FillBgMap(0x6006000, 0); //clear bg0 tilemap
	FillBgMap(0x6006800, 0); //clear bg1 tilemap
	VBlankIntrWait();
	*gColorSpecialEffectsSelectionBuffer = 0xA44; //blending set
	*gBg1ControlBuffer = 0xD03; //priority set

	//Load fonts
	// SetupDebugFontForBG(2, 0);
	// SetupDebugFontForOBJ(-1, 14);
	// SetupDebugFontForOBJ(0x6017800, 14);
	InitDefaultFont();
	// InitText(0, 0);

	//set up cursor
	CurrentProc->CursorIndex = 0;
	CurrentProc->Page = 1;
	CurrentProc->CasualMode = 0;
	CurrentProc->FixedGrowths = 0;
	CurrentProc->RN = 0;

	updateOptionsPage(CurrentProc);
};

void GenerateBGTsa(u16 *MapOffset, u32 NumberOfTiles, u8 PaletteId, u16 baseTile) {
  for(u16 i = baseTile; i < (baseTile+NumberOfTiles/2)+1; i++) {
    MapOffset[i-baseTile] = (i | (PaletteId << 12));
    MapOffset[NumberOfTiles-(i-baseTile)] = (i | (PaletteId << 12) | (3<<10)); //v and h flipped
  }
}

void updateOptionsPage(OptionsProc* CurrentProc) {

	//clear bg font buffers
	Text_ResetTileAllocation();
	// ClearBG0BG1();
	FillBgMap(BG0Buffer, 0);
	EnableBgSyncByIndex(0);
	//Print Headings
	DrawTextInline(0, BGLoc(BG0Buffer, 2, 0), 4, 0, 8, "Settings");
	//DrawTextInline(0, BGLoc(BG0Buffer, 19, 0), 4, 0, 8, "<< L/R >>");

	if (thisPage == 1){
		//option names
		DrawTextInline(0, BGLoc(BG0Buffer, 2, 3), 3, 0, 7, "Casual Mode:");
		DrawTextInline(0, BGLoc(BG0Buffer, 2, 5), 3, 0, 8, "Growths:");
		DrawTextInline(0, BGLoc(BG0Buffer, 2, 7), 3, 0, 7, "Hit RNG:");
		// DrawTextInline(0, BGLoc(BG0Buffer, 2, 9), 3, 0, 10, "Peak/Water Units:");
		// DrawTextInline(0, BGLoc(BG0Buffer, 2, 11), 3, 0, 8, "Weapon Stats:");
		// DrawTextInline(0, BGLoc(BG0Buffer, 2, 13), 3, 0, 8, "Random Items:");
		// DrawTextInline(0, BGLoc(BG0Buffer, 2, 15), 3, 0, 5, "Mode:");
		// DrawTextInline(0, BGLoc(BG0Buffer, 2, 17), 3, 0, 7, "Map Music:");


		//option values
		
		if (CurrentProc->CasualMode == 0) { 
			DrawTextInline(0, BGLoc(BG0Buffer, 15, 3), 2, 0, 5, "Off"); 
			OptionsSaved->CasualMode = 0;
		}
		else if (CurrentProc->CasualMode == 1) { 
			DrawTextInline(0, BGLoc(BG0Buffer, 15, 3), 2, 0, 5, "On"); 
			OptionsSaved->CasualMode = 1;
		}
		
		 
		switch (CurrentProc->FixedGrowths) {
			
			case 0:
			DrawTextInline(0, BGLoc(BG0Buffer, 15, 5), 2, 0, 10, "Standard"); 
			OptionsSaved->GrowthSetting = 0;
			break;
			
			case 1:
			DrawTextInline(0, BGLoc(BG0Buffer, 15, 5), 2, 0, 10, "Fixed"); 
			OptionsSaved->GrowthSetting = 1;
			break;
			
			case 2:
			DrawTextInline(0, BGLoc(BG0Buffer, 15, 5), 2, 0, 10, "0%"); 
			OptionsSaved->GrowthSetting = 2;
			break;
			
			case 3:
			DrawTextInline(0, BGLoc(BG0Buffer, 15, 5), 2, 0, 10, "100%"); 
			OptionsSaved->GrowthSetting = 3;
			break;
			
			default:
			break;
			
		}
		
		switch (CurrentProc->RN) {

			case 0:
			DrawTextInline(0, BGLoc(BG0Buffer, 15, 7), 2, 0, 10, "True Hit");
			OptionsSaved->RNGSetting = 0;
			break;
			
			case 1:
			DrawTextInline(0, BGLoc(BG0Buffer, 15, 7), 2, 0, 10, "Display Hit");
			OptionsSaved->RNGSetting = 1;
			break;
			
			case 2:
			DrawTextInline(0, BGLoc(BG0Buffer, 15, 7), 2, 0, 10, "Fates Hit");
			OptionsSaved->RNGSetting = 2;
			break;
			
			case 3:
			DrawTextInline(0, BGLoc(BG0Buffer, 15, 7), 2, 0, 10, "False Hit");
			OptionsSaved->RNGSetting = 3;
			break;
			
			case 4:
			DrawTextInline(0, BGLoc(BG0Buffer, 15, 7), 2, 0, 10, "Perfect Hit");
			OptionsSaved->RNGSetting = 4;
			break;
			
			case 5:
			DrawTextInline(0, BGLoc(BG0Buffer, 15, 7), 2, 0, 10, "Nice Hit");
			OptionsSaved->RNGSetting = 5;
				break;
			
			case 6:
			DrawTextInline(0, BGLoc(BG0Buffer, 15, 7), 2, 0, 10, "Coin Toss");
			OptionsSaved->RNGSetting = 6;
			break;
			
			
			default:
			break;

		}
		
		//now we get to do it all again, but for description text :/
		
		if (CurrentProc->CasualMode == 0) { 
			if (CurrentProc->CursorIndex == 0) {
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 13), 0, 0, 12, "Units die when they");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 15), 0, 0, 10, "fall in battle.");
			}
		}
		else if (CurrentProc->CasualMode == 1) { 
			if (CurrentProc->CursorIndex == 0) {
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 13), 0, 0, 13, "Units retreat when");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 15), 0, 0, 10, "they fall in battle.");
			}
		}
		
		 
		switch (CurrentProc->FixedGrowths) {
			
			case 0:
			if (CurrentProc->CursorIndex == 1) {
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 13), 0, 0, 14, "Random chance of gaining");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 15), 0, 0, 10, "stats on level ups.");
			}
			break;
			
			case 1:
			if (CurrentProc->CursorIndex == 1) {
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 13), 0, 0, 12, "Gain average stats");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 15), 0, 0, 10, "on level ups.");
			}
			break;
			
			case 2:
			if (CurrentProc->CursorIndex == 1) {
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 13), 0, 0, 12, "No stat gains");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 15), 0, 0, 10, "on level ups.");
			}
			break;
			
			case 3:
			if (CurrentProc->CursorIndex == 1) {
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 13), 0, 0, 13, "Guaranteed stat gains");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 15), 0, 0, 10, "on level ups.");
			}
			break;
			
			default:
			break;
			
		}
		
		switch (CurrentProc->RN) {

			case 0:
			if (CurrentProc->CursorIndex == 2) {
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 13), 0, 0, 12, "Higher hit rates land");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 15), 0, 0, 13, "more often, while lower");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 17), 0, 0, 14, "hit rates land less often.");
			}
			break;
			
			case 1:
			if (CurrentProc->CursorIndex == 2) {
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 13), 0, 0, 10, "Hit rate is exactly");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 15), 0, 0, 10, "as displayed.");
			}
			break;
			
			case 2:
			if (CurrentProc->CursorIndex == 2) {
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 13), 0, 0, 13, "Lower hit rates are as");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 15), 0, 0, 12, "displayed, but higher");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 17), 0, 0, 14, "hit rates land more often.");
				}
			break;
			
			case 3:
			if (CurrentProc->CursorIndex == 2) {
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 13), 0, 0, 12, "Higher hit rates land");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 15), 0, 0, 12, "less often, while lower");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 17), 0, 0, 14, "hit rates land more often.");
				}
			break;
			
			case 4:
			if (CurrentProc->CursorIndex == 2) {
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 13), 0, 0, 10, "Hits always land if");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 15), 0, 0, 10, "hit rate is not 0.");
				}
			break;
			
			case 5:
			if (CurrentProc->CursorIndex == 2) {
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 13), 0, 0, 10, "True Hit, but");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 15), 0, 0, 10, "hit rates of 69");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 17), 0, 0, 10, "always land.");
			}
				break;
			
			case 6:
			if (CurrentProc->CursorIndex == 2) {
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 13), 0, 0, 14, "Always a 50/50 chance");
				DrawTextInline(0, BGLoc(BG0Buffer, 2, 15), 0, 0, 10, "of landing a hit.");
				}
			break;
			
			
			default:
			break;

		}
		
	}
	
 };
 
void SaveOptionsData(void* target, unsigned size) {
	WriteAndVerifySramFast(&OptionsSaved, target, size);
}

void LoadOptionsData(void* source, unsigned size) {
	ReadSramFast(source, &OptionsSaved, size);
}

void SetOptionFlagsASMC() {
	
	//casual mode
	if (OptionsSaved->CasualMode == 1) SetEventId(CasualModeFlagLink);
	
	//growths
	switch (OptionsSaved->GrowthSetting) {
	
		case 1: //Fixed
			SetEventId(FixedGrowthsFlagLink);
			break;
		
		case 2: //0%
			SetEventId(ZeroGrowthsFlagLink);
			break;
		
		case 3: //100%
			SetEventId(PerfectGrowthsFlagLink);
			break;
		
		default:
			break;
		
	}
	
	//hit rng
	switch (OptionsSaved->RNGSetting) {
	
		case 1:
			SetEventId(OneRNFlagLink);
			break;
		
		case 2:
			SetEventId(FatesRNFlagLink);
			break;
		
		case 3:
			SetEventId(EvilRNFlagLink);
			break;
		
		case 4:
			SetEventId(PerfectHitFlagLink);
			break;
		
		case 5:
			SetEventId(NiceRNGFlagLink);
			break;
		
		case 6:
			SetEventId(CoinTossRNGFlagLink);
			break;
		
		default:
			break;
		
	}
	
}


void StartingOptionsLoop(OptionsProc* CurrentProc){

	//make snowflakes white
	*(u32*) 0x5000262 = 0x739eFFFF; //fill in the only obj palette colour that matters lol
	
	
	if (CurrentProc->CursorIndex != CurrentProc->LastCursorIndex) updateOptionsPage(CurrentProc);
	CurrentProc->LastCursorIndex = CurrentProc->CursorIndex;
	
	// UpdateBG3HOffset();
	UpdateHandCursor(CursorLocationTable[CurrentProc->CursorIndex].x, CursorLocationTable[CurrentProc->CursorIndex].y);	
	if (((newInput & InputStart) != 0)|((newInput & InputA) != 0)) { //press A or Start to continue
		BreakProcLoop((Proc *)CurrentProc);
		m4aSongNumStart(0x6B); 
	};


  if (thisPage == 1) {
    if ((newInput & InputDown) != 0) {
      if (CurrentProc->CursorIndex < PAGE1MAXINDEX) CurrentProc->CursorIndex++;
      else CurrentProc->CursorIndex = 0;
    };
    if ((newInput & InputUp) != 0) {
      if (CurrentProc->CursorIndex != 0) CurrentProc->CursorIndex--;
      else CurrentProc->CursorIndex = PAGE1MAXINDEX;
    };
    //CasualMode
    if (CurrentProc->CursorIndex == 0) {
      if (((newInput & InputLeft) != 0)|((newInput & InputRight) != 0)) {
		  if (CurrentProc->CasualMode == 0) CurrentProc->CasualMode = 1;
		  else CurrentProc->CasualMode = 0;
		updateOptionsPage(CurrentProc);
	}}
	//FixedGrowths
	
	if (CurrentProc->CursorIndex == 1) {
      if (((newInput & InputLeft) != 0)) {
		  if (CurrentProc->FixedGrowths == 0) CurrentProc->FixedGrowths = 3;
		  else CurrentProc->FixedGrowths --;
		  updateOptionsPage(CurrentProc);
	  }
	  if (((newInput & InputRight) != 0)) {
		  if (CurrentProc->FixedGrowths == 3) CurrentProc->FixedGrowths = 0;
		  else CurrentProc->FixedGrowths ++;
		  updateOptionsPage(CurrentProc);
	  }
		
	};
		
	//RN
	if (CurrentProc->CursorIndex == 2) {
		if ((newInput & InputLeft) != 0) {
			if (CurrentProc->RN == 0) CurrentProc->RN = 6;
			else CurrentProc->RN --;
			updateOptionsPage(CurrentProc);
		}
		if ((newInput & InputRight) != 0) {
			if (CurrentProc->RN == 6) CurrentProc->RN = 0;
			else CurrentProc->RN ++;
			updateOptionsPage(CurrentProc);
		}}
		
		
	};

};



