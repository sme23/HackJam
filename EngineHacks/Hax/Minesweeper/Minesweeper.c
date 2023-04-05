
#include "Minesweeper.h"

void InitMinesweeperBoard(Proc* parent) {
	//create 'initialMineCount' # of mine traps at random locations on the board
	
}

void PropagateTileSelection(Proc* parent) {
	//reveal the tile selected
	//if the tile selected is a mine, game over
	//if the tile selected has a value of 0, reveal all adjacent tiles
	//repeat recursively until all necessary tiles are revealed
	
}

void GetTileValue(u16 x, u16 y) {
	//get # of mines within the surrounding 8 tiles of the given position
	//this is just the sum of how many of 8 GetTrapAt calls return a trap
	
}

void CheckWinState(Proc* parent) {
	//check each tile on the map
	//if not uncovered and not a mine, return false to memory slot sC
	//if reaching the end of the map and everything is uncovered, return true to memory slot sC
	
}

void GenerateTileMapFromMinesAndRevealed() {
	//this function will replace whatever handles copying the chapter map into RAM
	//instead it will set everything to covered tile, then copy changes defined on another map overtop
	
}

