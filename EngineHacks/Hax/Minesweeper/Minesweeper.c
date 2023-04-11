
#include "Minesweeper.h"

void InitMinesweeperBoard(struct Proc* parent) {
	//create 'initialMineCount' # of mine traps at random locations on the board
	int curX = -1;
	int curY = -1;

	for (int i = 0; i < initialMineCount; i++) {
		curX = NextRN_N(boardX-1);
		curY = NextRN_N(boardY-1);
		if (GetTrapAt(curX,curY) == 0) AddTrap(curX, curY, TRAP_MINE, 0); //if we roll the same place twice, dont place a second mine
	}
	BmMapFill(gBmMapFog, 0);

}

void UnpackChapterMap(void* into, int chapterId) {

	//write (boardX*boardY)+2 bytes of TILE_HIDDEN to buffer
	u8* cur = into;
	for (int i = 0; i < (boardX*boardY)+2; i++) {
		cur = TILE_HIDDEN;
		cur++;
	}
    // Setting map size
    gBmMapSize.x = boardX;
    gBmMapSize.y = boardY;

    // Decompress tileset config
    Decompress(
        gChapterDataAssetTable[GetROMChapterStruct(chapterId)->map.tileConfigId], sTilesetConfig);

    // Setting max camera offsets (?) TODO: figure out
    gBmSt.cameraMax.x = gBmMapSize.x*16 - 240;
    gBmSt.cameraMax.y = gBmMapSize.y*16 - 160;
}

int Map_OnAPress(struct Proc* parent) {
	//This runs when A is pressed
	//Get current cursor coords

	u8 xPosit = gBmSt.playerCursor.x;
	u8 yPosit = gBmSt.playerCursor.y;

	if (gBmMapFog[yPosit][xPosit] == 0) return 0; // false

	PropagateTileSelection(xPosit,yPosit);

	GenerateTileMapFromMinesAndRevealed(&gBmMapBuffer);

	//reload map gfx
	RenderBmMap();

	EndPlayerPhaseSideWindows(); 
	MU_EndAll(); 
	Proc_Goto(parent, 9); 
	return (-1); // true 

}


void PropagateTileSelection(u8 xPosit, u8 yPosit) {
	//reveal the tile selected
	//if the tile selected is a mine, game over
	//if the tile selected has a value of 0, reveal all adjacent tiles
	//repeat until all necessary tiles are revealed

	if (GetTrapAt(xPosit,yPosit) != 0) {
		SetEventId(0x65); //todo: replace with a direct event call 
		return;
	}

	coordArray[0].x= (u32)xPosit;
	coordArray[0].y = (u32)yPosit;

	int curIndex = 0;
	int endIndex = 1;

	while (true) { 
		
		if (curIndex == endIndex) break;

		if (gBmMapFog[coordArray[curIndex].y][coordArray[curIndex].x] == 0) {
			curIndex++;
			continue;
		}


		gBmMapFog[coordArray[curIndex].y][coordArray[curIndex].x] = 0;

		if (GetTileValue((u16)coordArray[curIndex].x,(u16)coordArray[curIndex].y) == 0) {
		
			//enqueue 8 adjacent tiles

			int realX = coordArray[curIndex].x;
			int realY = coordArray[curIndex].y;
		
			if (realX-1 >= 0) {
				coordArray[endIndex].x= (u32)realX-1;
				coordArray[endIndex].y = (u32)realY;
				endIndex++;
			}

			if (realX-1 >= 0 && realY-1 >= 0) {
				coordArray[endIndex].x= (u32)realX-1;
				coordArray[endIndex].y = (u32)realY-1;
				endIndex++;
			}

			if (realY-1 >= 0) {
				coordArray[endIndex].x= (u32)realX;
				coordArray[endIndex].y = (u32)realY-1;
				endIndex++;
			}

			if (realX+1 < boardX && realY-1 >= 0) {
				coordArray[endIndex].x= (u32)realX+1;
				coordArray[endIndex].y = (u32)realY-1;
				endIndex++;
			}

			if (realX+1 < boardX) {
				coordArray[endIndex].x= (u32)realX+1;
				coordArray[endIndex].y = (u32)realY;
				endIndex++;
			}

			if (realX+1 < boardX && realY+1 < boardY) {
				coordArray[endIndex].x= (u32)realX+1;
				coordArray[endIndex].y = (u32)realY+1;
				endIndex++;
			}

			if (realY+1 < boardY) {
				coordArray[endIndex].x= (u32)realX;
				coordArray[endIndex].y = (u32)realY+1;
				endIndex++;
			}

			if (realX-1 >= 0 && realY < boardY) {
				coordArray[endIndex].x= (u32)realX-1;
				coordArray[endIndex].y = (u32)realY+1;
				endIndex++;
			}
		}
		curIndex++;

	}

}


bool TileNotRevealed(u8 xPosit, u8 yPosit) {
	if (gBmMapFog[yPosit][xPosit] == 0) return false; //cant return directly because iirc default value is -1 not 0; otherwise its return !gBmMapFog[yPosit][xPosit]
	return true;
}


u8 GetTileValue(u16 x, u16 y) {
	//get # of mines within the surrounding 8 tiles of the given position
	//this is just the sum of how many of 8 GetTrapAt calls return a trap

	u8 i = 0;

	if (x-1 >= 0) if (GetTrapAt(x-1,y)) i++;
	if (x+1 <= boardX) if (GetTrapAt(x+1,y)) i++;
	if (y-1 >= 0) if (GetTrapAt(x,y-1)) i++;
	if (y+1 <= boardY) if (GetTrapAt(x,y+1)) i++;

	if (x-1 >= 0 && y-1 >= 0) if (GetTrapAt(x-1,y-1)) i++;
	if (x+1 <= boardX && y-1 >= 0) if (GetTrapAt(x+1,y-1)) i++;
	if (x-1 >= 0 && y+1 <= boardY) if (GetTrapAt(x-1,y+1)) i++;
	if (x+1 <= boardX && y+1 <= boardY) if (GetTrapAt(x+1,y+1)) i++;

	return i;
	
}

void CheckWinState(struct Proc* parent) {
	//check each tile on the map
	//if not uncovered and not a mine, return false to memory slot sC
	//if reaching the end of the map and everything is uncovered, return true to memory slot sC
	u8* curPos = *gBmMapFog;
	bool j = false;
	for (int i = 0; i < (boardX*boardY); i++) {
		if (!*curPos) j = true;
		curPos++;
	}
	if (j) gEventSlots[0xC] = false;
	else gEventSlots[0xC] = true;
	
}

void GenerateTileMapFromMinesAndRevealed(void* pool) {
	//this function will replace whatever handles copying the chapter map into RAM
	//instead it will set everything to covered tile, then copy changes defined on another map overtop
		


	memset(&gBmMapBuffer, TILE_HIDDEN, 0x2000);
	
	for (int y = 0; y < boardY; y++) {
		for (int x = 0; x < boardX; x++) {
			if (gBmMapFog[y][x] == 0) {
				gBmMapBuffer[(y*x)+(4*y)] = GetTileIndexFromInt(GetTileValue(x,y));
			}
		}
	}
}

u8 GetTileIndexFromInt(int i) {
	switch (i) {
	case 0:
		return TILE_0;
	case 1:
		return TILE_1;
	case 2:
		return TILE_2;
	case 3:
		return TILE_3;
	case 4:
		return TILE_4;
	case 5:
		return TILE_5;
	case 6:
		return TILE_6;
	case 7:
		return TILE_7;
	case 8:
		return TILE_8;
	}
}

// take an 8-byte input string, cast it to a u16 array, pass to this function
/* void ApplyRNGSeed(u16* seed) {
	StoreRNState(seed);
} */
//decomp does this such that there's no purpose to having this be a helper function


void PackMinesweeperData() {

}

void UnpackMinesweeperData() {

}

