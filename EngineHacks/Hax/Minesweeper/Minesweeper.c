
#include "Minesweeper.h"

void InitMinesweeperBoard(struct Proc* parent) {
	//create 'initialMineCount' # of mine traps at random locations on the board
	int curX = -1;
	int curY = -1;

	for (int i = 0; i < initialMineCount; i++) {
		curX = NextRN_N(boardX-1);
		curY = NextRN_N(boardY-1);
		if (!IsTrapAt(curX,curY)) AddTrap(curX, curY, TRAP_MINE, 0); //if we roll the same place twice, dont place a second mine
	}
	BmMapFill(gBmMapFog, 1);

	//set game options to turn off player phase windows (todo: remove as an option entirely)
	SetGameOption(GAME_OPTION_TERRAIN,1);
	SetGameOption(GAME_OPTION_OBJECTIVE,1);

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

	if (gBmMapFog[yPosit][xPosit] == 2) return 0; // false
 
	PropagateTileSelection(xPosit,yPosit);

	GenerateTileMapFromMinesAndRevealed(&gBmMapBuffer);

	//reload map gfx
	RenderBmMap();
	
	//EndPlayerPhaseSideWindows(); 
	//MU_EndAll(); 
	if (CheckEventId(0x65)) {
		CallEvent(&loseEvent, 1);
		
	} 
	Proc_Goto(parent, 9);

	return (-1); // true 

}

int Map_CheckBPress(struct Proc* parent) { //todo: hook for this just after the a button hook returns
	if (gKeyStatusPtr->newKeys & B_BUTTON) return Map_OnBPress(parent);

}

int Map_OnBPress(struct Proc* parent) {
	//This runs when B is pressed
	//Get current cursor coords

	u8 xPosit = gBmSt.playerCursor.x;
	u8 yPosit = gBmSt.playerCursor.y;

	int val = gBmMapFog[yPosit][xPosit]; 
	
	if (val == 1) {
		gBmMapFog[yPosit][xPosit] = -2;
	}
	else if (val == -2) {
		gBmMapFog[yPosit][xPosit] = 1;
	}
	else return 0;

	GenerateTileMapFromMinesAndRevealed(&gBmMapBuffer);
	RenderBmMap();
	Proc_Goto(parent, 9); 
	return (-1); // true 

}


void RevealTileAt_ASMC(struct Proc* parent) { //converts sB coords to individual parameters
	RevealTileAt((gEventSlots[0xB] & 0xFFFF0000)>>16,gEventSlots[0xB] & 0xFFFF);
}

void RevealTileAt(u16 x, u16 y) {
	PropagateTileSelection(x,y);
	GenerateTileMapFromMinesAndRevealed(&gBmMapBuffer);
	RenderBmMap();
}

void RevealAllMines() {
	//for each trap, reveal the tile at that position
}

bool inline PositionIsValid(u16 x, u16 y) {
	return (x >= 0) && 
		   (x < boardX) && 
		   (y >= 0) && 
		   (y < boardY) && 
		   (gBmMapFog[y][x] == 1 || gBmMapFog[y][x] == 3) &&
		   (!IsTrapAt(x,y));
}

void PropagateTileSelection(u8 xPosit, u8 yPosit) {
	
	//todo: implement this for a 5th time

	//temp until removing the thing that sets this in the first place
	if (CheckEventId(0x65)) UnsetEventId(0x65);

	if (IsTrapAt(xPosit,yPosit)) {
		SetEventId(0x65); //todo: replace with a direct event call 
		gBmMapFog[yPosit][xPosit] = -1;
		return;
	}

	gBmMapFog[yPosit][xPosit] = 2;

	if (GetTileValue(xPosit,yPosit) == 0) {
		//do the thing to reveal the 8 adjacent tiles
		int pos = 0;
		int end = 1;

		adjPos[0].x = xPosit;
		adjPos[0].y = yPosit;

		while (pos != end) {
			int curX =adjPos[pos].x;
			int curY =adjPos[pos].y;
			gBmMapFog[curY][curX] = 2;

			if (GetTileValue(curX,curY) == 0) {

				//reveal adjacent
				if (PositionIsValid(curX-1,curY-1)) {
					adjPos[end].x = curX-1;
					adjPos[end].y = curY-1;
					gBmMapFog[curY-1][curX-1] = 3;
					end++;
				}
				if (PositionIsValid(curX-1,curY)) {
					adjPos[end].x = curX-1;
					adjPos[end].y = curY;
					gBmMapFog[curY][curX-1] = 3;
					end++;
				}
				if (PositionIsValid(curX,curY-1)) {
					adjPos[end].x = curX;
					adjPos[end].y = curY-1;
					gBmMapFog[curY-1][curX] = 3;
					end++;
				}
				if (PositionIsValid(curX-1,curY+1)) {
					adjPos[end].x = curX-1;
					adjPos[end].y = curY+1;
					gBmMapFog[curY+1][curX-1] = 3;
					end++;
				}
				if (PositionIsValid(curX+1,curY-1)) {
					adjPos[end].x = curX+1;
					adjPos[end].y = curY-1;
					gBmMapFog[curY-1][curX+1] = 3;
					end++;
				}
				if (PositionIsValid(curX,curY+1)) {
					adjPos[end].x = curX;
					adjPos[end].y = curY+1;
					gBmMapFog[curY+1][curX] = 3;
					end++;
				}
				if (PositionIsValid(curX+1,curY)) {
					adjPos[end].x = curX+1;
					adjPos[end].y = curY;
					gBmMapFog[curY][curX+1] = 3;
					end++;
				}
				if (PositionIsValid(curX+1,curY+1)) {
					adjPos[end].x = curX+1;
					adjPos[end].y = curY+1;
					gBmMapFog[curY+1][curX+1] = 3;
					end++;
				}
			}
			pos++;
		}

	}


	/*
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
	*/

}


bool inline TileNotRevealed(u8 xPosit, u8 yPosit) {
	if (gBmMapFog[yPosit][xPosit] == 2) return false; //1 is visible and 0 is not visible
	return true;
}


//replaces the vanilla func for this, to prevent the unit map from getting initialized
void RefreshUnitsOnBmMap(void) {}

bool inline IsTrapAt(int x, int y) {
	if (GetTrapAt(x,y) != 0) return true;
	return false;
}


u8 GetTileValue(u16 x, u16 y) {
	//get # of mines within the surrounding 8 tiles of the given position
	//this is just the sum of how many of 8 GetTrapAt calls return a trap

	//if there is a mine on the tile the value is -1
	if (IsTrapAt(x,y)) return -1;

	u8 i = 0;

	if (x-1 >= 0 && IsTrapAt(x-1,y)) i++;
	if (x+1 < boardX && IsTrapAt(x+1,y)) i++;
	if (y-1 >= 0 && IsTrapAt(x,y-1)) i++;
	if (y+1 < boardY && IsTrapAt(x,y+1)) i++;

	if (x-1 >= 0 && y-1 >= 0 && IsTrapAt(x-1,y-1)) i++;
	if (x+1 < boardX && y-1 >= 0 && IsTrapAt(x+1,y-1)) i++;
	if (x-1 >= 0 && y+1 <= boardY && IsTrapAt(x-1,y+1)) i++;
	if (x+1 < boardX && y+1 <= boardY && IsTrapAt(x+1,y+1)) i++;

	return i;
	
}

void CheckWinState(struct Proc* parent) {
	//check each tile on the map
	//if not uncovered and not a mine, return false to memory slot sC
	//if reaching the end of the map and everything is uncovered, return true to memory slot sC
	bool j = false;

	for (int y = 0; y < boardY; y++) {
		for (int x = 0; x < boardX; x++) {
			if (gBmMapFog[y][x] == 1) j = true;
		}
	}
	if (j) gEventSlots[0xC] = false;
	else gEventSlots[0xC] = true;
	
}

void GenerateTileMapFromMinesAndRevealed(void* pool) {
	//this function will replace whatever handles copying the chapter map into RAM
	//instead it will set everything to covered tile, then copy changes defined on another map overtop
		


	memset(&gBmMapBaseTiles, TILE_HIDDEN, 0x2000);
	
	for (int y = 0; y < boardY; y++) {
		for (int x = 0; x < boardX; x++) {
			if (gBmMapFog[y][x] == 2) {
			//	DisplayBmTile(gBG3TilemapBuffer, x, y,
            //  (short) gBmSt.mapRenderOrigin.x + x, (short) gBmSt.mapRenderOrigin.y + y);
				gBmMapBaseTiles[y][x] = GetTileIndexFromInt(GetTileValue(x,y));
			}
			if (gBmMapFog[y][x] == -1) {
				gBmMapBaseTiles[y][x] = TILE_MINE;	
			}
			if (gBmMapFog[y][x] == -2) {
				gBmMapBaseTiles[y][x] = TILE_FLAG;	
			}
			
		}
	}
}

u8 GetTileIndexFromInt(int i) {
	switch (i) {
	case -1:
		return TILE_MINE;
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

