
#include "Minesweeper.h"

void InitMinesweeperBoard(struct Proc* parent) {
	//create 'initialMineCount' # of mine traps at random locations on the board

	for (int i = 0; i < initialMineCount; i++) {
		AddTrap(NextRN_N(boardX-1), NextRN_N(boardY-1), TRAP_MINE, 0);
	}
	BmMapFill(gBmMapMovement, 0);

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

	PropagateTileSelection(xPosit,yPosit);


	GenerateTileMapFromMinesAndRevealed(&gBmMapBuffer);


	EndPlayerPhaseSideWindows(); 
	MU_EndAll(); 
	Proc_Goto(parent, 9); 
	return (-1); // true 

	return false;
}


void PropagateTileSelection(u8 xPosit, u8 yPosit) {
	//reveal the tile selected
	//if the tile selected is a mine, game over
	//if the tile selected has a value of 0, reveal all adjacent tiles
	//repeat recursively until all necessary tiles are revealed

	if (!GetTrapAt(xPosit,yPosit)) {
		SetEventId(0x65);
		return;
	}

	u8* pos = *gBmMapMovement;
	pos = pos+(xPosit*yPosit);
	*pos = 1;


	if (!GetTileValue(xPosit,yPosit)) {	// if value of 0, reveal the 4 adjacent tiles too
		if (xPosit-1 >= 0) PropagateTileSelection(xPosit-1,yPosit);
		if (xPosit+1 <= boardX) PropagateTileSelection(xPosit+1,yPosit);
		if (yPosit-1 >= 0) PropagateTileSelection(xPosit,yPosit-1);
		if (yPosit+1 <= boardY) PropagateTileSelection(xPosit,yPosit+1);
	}

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
	u8* curPos = *gBmMapMovement;
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
		
	memset(pool, TILE_HIDDEN, 0x2000);
	u8* buffer = pool;

	//go through (boardX*boardY) tiles in gMapMovement and if a tile is revealed change its appearance
	if (gBmMapMovement == 0) return;
	u8* curPos = *gBmMapMovement;
	u8 curX = 0;
	u8 curY = 0;

	for (int i = 0; i < (boardX*boardY); i++) {
		if (*curPos) {
			u8* b = buffer+i;
			*b = GetTileIndexFromInt(GetTileValue(curX,curY));
		}
		curX ++;
		if (curX == boardX) {
			curX = 0;
			curY ++;
		}
		curPos++;
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

