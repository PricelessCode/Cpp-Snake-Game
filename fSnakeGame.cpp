#include "fSnakeGame.h"
#include <unistd.h>
#include <thread>

using namespace std;
using namespace std::chrono;

CharPosition::CharPosition(int col, int row) {
	x = col;
	y = row;
}

CharPosition::CharPosition() {
	x = 0;
	y = 0;
}

Item::Item(int col, int row, bool fruit) {
	pos.x = col;
	pos.y = row;
	isFruit = fruit;
}

fSnakeGame::fSnakeGame(int s) {
	// variables initialisation:
	headChar = 'O';
	partchar = '@'; // character to represent the snake
	edgechar = (char)219; // full rectangle on the key table
	gateChar = '#';
	gateEntChar = (char)79;
	gateDestChar = (char)111;
	imChar = (char)43;
	initSnakeSize = 5;
	fruitchar = '*'; 
	poisonChar = 'X';
	entGate.x = 0;
	entGate.y = 0;
	destGate.x = 0;
	destGate.y = 0;
	fruit.x = 0;
	fruit.y = 0;
	poison.x = 0;
	poison.y = 0;
	score = 0;
	del = 110000;
	bEatsFruit = false;
	bEatsPoison = false;
	bPassedGates = false;
	direction = 'l';
	stage = s;
	cntFruit = 0;
	cntPoison = 0;
	cntGate = 0;
	bLength = false;
	bFruit = false;
	bIsEnded = false;
	bPoison = false;
	bGate = false;

	srand(time(NULL));
	

	createStages();
	InitGameWindow();
	

	// Init 3 items on the screen
	for (int i = 0; i < 3; i++) {
		int tmpX = rand() % gameScreenWidth + 1;
		int tmpY = rand() % maxheight + 1;
		if (i == 2) {
			// Add 1 Poison
			items.push_back(Item(tmpX, tmpY, false));
			move(tmpY, tmpX);
			addch(poisonChar);
		} else {
			// Add 2 Fruits
			items.push_back(Item(tmpX, tmpY, true));	
			move(tmpY, tmpX);
			addch(fruitchar);
		}
		
		refresh();
	}

	DrawWindow();
	PositionGate();
	DrawSnake();
	PrintScore();

	refresh();	
}

fSnakeGame::~fSnakeGame() {
	nodelay(stdscr, true);
	getch();
	endwin();
}

// initialise the game window
void fSnakeGame::InitGameWindow() { 
	initscr(); // initialise the screen
	nodelay(stdscr, 1);
	keypad(stdscr, true); // initialise the keyboard: we can use arrows for directions
	noecho(); // user input is not displayed on the screen
	curs_set(0); // cursor symbol is not not displayed on the screen (Linux)
	// getmaxyx(stdscr, maxheight, maxwidth); // define dimensions of game window
	// maxheight = 200;
	// maxwidth = 200;
	gameScreenWidth = stages[stage].walls[0].size();
	gameScreenHeight = stages[stage].walls.size();
	return; 
}

void fSnakeGame::createStages() {
	Stage stage1;
	stage1.walls = {
		{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
		
		
	};
	stage1.length = 6;
	stage1.nFruit = 1;
	stage1.nPoison = 1;
	stage1.nGate = 1;

	Stage stage2;
	stage2.walls = {
		{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
	};
	stage2.length = 6;
	stage2.nFruit = 2;
	stage2.nPoison = 2;
	stage2.nGate = 1;

	
	stages.push_back(stage1);
	stages.push_back(stage2);
}

// Update the board (length, )
void fSnakeGame::updateBoard() {
	// Score Board
	move(scoreScreen.y + 2, scoreScreen.x + 1);
	printw("B: %d / %d", snake.size(), stages[stage].length);
	move(scoreScreen.y + 3, scoreScreen.x + 1);
	printw("+: %d", cntFruit);
	move(scoreScreen.y + 4, scoreScreen.x + 1);
	printw("-: %d", cntPoison);
	move(scoreScreen.y + 5, scoreScreen.x + 1);
	printw("G: %d", cntGate);

	
	// Mission Board
	move(missionScreen.y + 2, missionScreen.x + 1);
	char a = snake.size() >= stages[stage].length ? 'v': ' ';
	printw("B: %d (%c)", stages[stage].length, a);
	move(missionScreen.y + 3, missionScreen.x + 1);
	printw("+: %d (%c)", stages[stage].nFruit, cntFruit >= stages[stage].nFruit ? 'v': ' ');
	move(missionScreen.y + 4, missionScreen.x + 1);
	printw("-: %d (%c)", stages[stage].nPoison, cntPoison >= stages[stage].nPoison ? 'v': ' ');
	move(missionScreen.y + 5, missionScreen.x + 1);
	printw("G: %d (%c)", stages[stage].nGate, cntGate >= stages[stage].nGate ? 'v': ' ');
	refresh();
}

bool fSnakeGame::checkCompleted() {
	if (snake.size() >= stages[stage].length
		&& cntFruit >= stages[stage].nFruit
		&& cntPoison >= stages[stage].nPoison
		&& cntGate >= stages[stage].nGate) {
		
		return true;
	}
	return false;
}

// draw the game window
void fSnakeGame::DrawWindow() {

	// Draw Walls
	for (int i = 0; i < stages[stage].walls.size(); i++) { // v
		for (int j = 0; j < stages[stage].walls[0].size(); j++) { // >
			move(i, j);
			if (stages[stage].walls[i][j] == 1) {
				addch(edgechar);
			} else if (stages[stage].walls[i][j] == 2) {
				addch(imChar);
			}
		}
	}


	// Draw Score Board
	int offSetX = 5;
	int offSetY = 0;
	int screenStartX = gameScreenWidth + offSetX;
	int screenStartY = 0 + offSetY;
	const int XSCREENSIZE = 18;
	const int YSCREENSIZE = 10;
	

	// Top
	for (int i = 0; i < XSCREENSIZE; i++) {
		move(screenStartY, screenStartX + i);
		addch(edgechar);
	}

	// Bottom
	for (int i = 0; i < XSCREENSIZE; i++) {
		move(screenStartY + YSCREENSIZE, screenStartX + i);
		addch(edgechar);
	}

	// Left
	for (int i = 0; i < YSCREENSIZE; i++) {
		move(screenStartY + i, screenStartX);
		addch(edgechar);
	}

	// Right
	for (int i = 0; i < YSCREENSIZE; i++) {
		move(screenStartY + i, screenStartX + XSCREENSIZE - 1);
		addch(edgechar);
	}
	
	move(screenStartY + 1, screenStartX + 1);
	printw("Score Board");
	scoreScreen.x = screenStartX;
	scoreScreen.y = screenStartY;

	// Draw Misison Board
	offSetX = 5;
	offSetY = 12;
	screenStartX = gameScreenWidth + offSetX;
	screenStartY = 0 + offSetY;

	// Top
	for (int i = 0; i < XSCREENSIZE; i++) {
		move(screenStartY, screenStartX + i);
		addch(edgechar);
	}

	// Bottom
	for (int i = 0; i < XSCREENSIZE; i++) {
		move(screenStartY + YSCREENSIZE, screenStartX + i);
		addch(edgechar);
	}

	// Left
	for (int i = 0; i < YSCREENSIZE; i++) {
		move(screenStartY + i, screenStartX);
		addch(edgechar);
	}

	// Right
	for (int i = 0; i < YSCREENSIZE; i++) {
		move(screenStartY + i, screenStartX + XSCREENSIZE - 1);
		addch(edgechar);
	}

	move(screenStartY + 1, screenStartX + 1);
	printw("Mission");
	missionScreen.x = screenStartX;
	missionScreen.y = screenStartY;

	return;
}

// draw snake's body
void fSnakeGame::DrawSnake() {
	for (int i = 0; i < initSnakeSize; i++) {
		snake.push_back(CharPosition(10 + i, 10));
	}

	for (int i = 0; i < snake.size(); i++) {
		move(snake[i].y, snake[i].x);
		if (i == 0) {
			addch(headChar);
		} else {
			addch(partchar);
		}
	}
	return;
}

// print score at bottom of window
void fSnakeGame::PrintScore() {
	// move(maxheight-1, 0);
	// printw("Score: %d", score);
	// return;
}
void fSnakeGame::drawItems() {

}

// position a new fruit in the game window
void fSnakeGame::PositionFruit() {
	while(1) {
		int tmpx = rand() % gameScreenWidth+ 1; // +1 to avoid the 0
		int tmpy = rand() % gameScreenHeight + 1;

		// check that the fruit is not positioned on the snake
		for (int i = 0; i < snake.size(); i++) {
			if (snake[i].x == tmpx && snake[i].y == tmpy) {
				continue; // if true, ignore the following and go back to the beginning of function
			}
		}
		
		// check that the fruit is positioned within the game window
		if (tmpx >= gameScreenWidth - 4 || tmpy >= gameScreenHeight - 4) {
			continue; // if true, ignore the following and go back to the beginning of function
		}

		// Check is not on the walls
		if (stages[stage].walls[tmpy][tmpx] == 1) {
			continue;
		}

		// check with other items
		for (int i = 0; i < 3; i++) {
			if (items[i].pos.x == tmpx && items[i].pos.y == tmpy) {
				continue;
			}
		}

		// if the coordinates are valid, add fruit in the window
		fruit.x = tmpx;
		fruit.y = tmpy;

		
		break;
	}

	if (!FatalCollision() && !bIsEnded) {
		// Erase the previous position
	Item item = items.front();
	items.pop_front();
	move(item.pos.y, item.pos.x);
	addch(' ');
	refresh();

	// Draw New position
	items.push_back(Item(fruit.x, fruit.y, true));
	move(fruit.y, fruit.x);
	addch(fruitchar);
	refresh();
	}
	
}

void fSnakeGame::PositionPoison() {
	while(true) {
		int tmpx = rand() % gameScreenWidth + 1; // +1 to avoid the 0
		int tmpy = rand() % gameScreenHeight + 1;

		// check that the poison is not positioned on the snake
		for (int i = 0; i < snake.size(); i++) {
			if (snake[i].x == tmpx && snake[i].y == tmpy) {
				continue; // if true, ignore the following and go back to the beginning of function
			}
		}

		// check with other items
		for (int i = 0; i < 3; i++) {
			if (items[i].pos.x == tmpx && items[i].pos.y == tmpy) {
				continue;
			}
		}

		

		// check that the poison is positioned within the game window
		if (tmpx >= gameScreenWidth - 4 || tmpy >= maxheight - 4) {
			continue; // if true, ignore the following and go back to the beginning of function
		}

		// Check is not on the walls
		if (stages[stage].walls[tmpy][tmpx] == 1) {
			continue;
		}

		// if the coordinates are valid, add fruit in the window
		poison.x = tmpx;
		poison.y = tmpy;
		break;
	}

	if (!FatalCollision() && !bIsEnded) {
		// Erase the previous position
	Item item = items.front();
	items.pop_front();
	move(item.pos.y, item.pos.x);
	addch(' ');
	refresh();

	// Draw New position
	items.push_back(Item(poison.x, poison.y, false));
	move(poison.y, poison.x);
	addch(poisonChar);
	refresh();
	 }

	
}

void fSnakeGame::PositionGate() {
	// Ent Gate
	DrawWindow();
	while (true) {
		int tmpX = rand() % gameScreenWidth;
		int tmpY = rand() % gameScreenHeight;

		if (tmpX >= stages[stage].walls[0].size() || tmpY >= stages[stage].walls.size()) {
			continue;
		}

		if (stages[stage].walls[tmpY][tmpX] != 1) {
			continue;
		}

		
		
		move(entGate.y, entGate.x);
		addch(edgechar);
		refresh();

		entGate.x = tmpX;
		entGate.y = tmpY;
		move(entGate.y, entGate.x);
		addch(gateEntChar);
		refresh();
		break;
	}
	// Dest Gate
	while (true) {
		int tmpX = rand() % gameScreenWidth;
		int tmpY = rand() % gameScreenHeight;

		if (tmpX >= stages[stage].walls[0].size() || tmpY >= stages[stage].walls.size()) {
			continue;
		}
		if (stages[stage].walls[tmpY][tmpX] != 1) {
			continue;
		}

		move(destGate.y, destGate.x);
		addch(edgechar);
		refresh();

		destGate.x = tmpX;
		destGate.y = tmpY;
		move(destGate.y, destGate.x);
		addch(gateDestChar);
		refresh();
		break;
	}
}

// set game over situations
bool fSnakeGame::FatalCollision() {
	if ((snake[0].x == entGate.x && snake[0].y == entGate.y) || (snake[0].x == destGate.x) && (snake[0].y == destGate.y)) {
		return false;
	}

	// if the snake hits the edge of the window
	if (snake[0].x == 0 || snake[0].x == gameScreenWidth-1 || snake[0].y == 0 || snake[0].y == maxheight-2)
	{
		return true;
	}

	// if the snake hits the level walls
	if (stages[stage].walls[snake[0].y][snake[0].x] == 1) {
		return true;
	}
	

	// if the snake collides into himself
	for (int i = 2; i < snake.size(); i++) {
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
			return true;
		}
	}

	// if size of the snake is smaller than 3
	if (snake.size() < 3) {
		return true;
	}

	return false;
}

// define behaviour when snake eats the fruit
bool fSnakeGame::GetsFruit() {
	for (int i = 0; i < 3; i++) {
		Item item = items[i];
		if (snake[0].x == item.pos.x && snake[0].y == item.pos.y && item.isFruit) {
			score +=10; 
			PrintScore();
			cntFruit++;
			return bEatsFruit = true;
		}
	}
	return bEatsFruit = false;
}

bool fSnakeGame::GetsPoison() {
	for (int i = 0; i < 3; i++) {
		Item item = items[i];
		if (snake[0].x == item.pos.x && snake[0].y == item.pos.y && !item.isFruit) {
			score -= 10; 
			PrintScore();
			cntPoison++;
			return bEatsPoison = true;
		}
	}
	return bEatsPoison = false;
}


// define snake's movements
void fSnakeGame::MoveSnake() {
	int KeyPressed = getch();
	switch(KeyPressed) {
		case KEY_LEFT:
			if (direction != 'r') {
				direction = 'l'; 
			}  
			break;
		case KEY_RIGHT:
			if (direction != 'l')
			{ direction = 'r'; }
			break;
		case KEY_UP:
			if (direction != 'd')
			{ direction = 'u'; }
			break;
		case KEY_DOWN:
			if (direction != 'u')
			{ direction = 'd'; }
			break;
		case KEY_BACKSPACE:
			direction = 'q'; // key to quit the game
			break;
	}

	// the snake doesn't eat fruit, remains same size
	if (!bEatsFruit) {
		// if fruit was not eaten remove the tail
		move(snake[snake.size()-1].y, snake[snake.size()-1].x); // moves at the end of the tail
		// if (snake[snake.size() - 1].y != destGate.y && snake[snake.size() - 1].y != destGate.x)
			addch(' '); // add empty ch to remove last character
		refresh();
		snake.pop_back(); // removes the last element in the vector, reducing the container size by one
	}

	
	if (bEatsPoison) {
		move(snake[snake.size()-1].y, snake[snake.size()-1].x); // moves at the end of the tail
		// if (snake[snake.size() - 1].y != destGate.y && snake[snake.size() - 1].y != destGate.x)
		addch(' '); // add empty ch to remove last character
		
		refresh();
		snake.pop_back(); // removes the last element in the vector, reducing the container size by one
	}

	// the snake moves and we add an extra character at the beginning of the vector
	// add a head and initialise new coordinates for CharPosition according to the direction input
	if (direction == 'l') {
		snake.insert(snake.begin(), CharPosition(snake[0].x-1, snake[0].y)); 
	} else if (direction == 'r') {
		snake.insert(snake.begin(), CharPosition(snake[0].x+1, snake[0].y));
	} else if (direction == 'u') {
		snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y-1));
	} else if (direction == 'd') {
		snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y+1)); 
	}

	for (int i = 0; i < snake.size(); i++) {
		if (snake[i].x == entGate.x && snake[i].y == entGate.y) {
			// Change direction for head
			if (i == 0) {
				cntGate++;
				// Dest at the Bottom
				if (destGate.y == gameScreenHeight - 1) {
					direction = 'u';
				// Dest at the Top
				} else if (destGate.y == 0) {
					direction = 'd';
				// Dest at the left
				} else if (destGate.x == 0) {
					direction = 'r';
				// Dest at the right
				} else if (destGate.x == gameScreenWidth - 1) {
					direction = 'l';
				} else {					
				}
			}
			snake[i].x = destGate.x;
			snake[i].y = destGate.y;
		}
		
	}

	

	// Move to the new CharPosition coordinates
	move(snake[0].y, snake[0].x);
	addch(headChar); // add a new head

	// Change previous head to body char
	move(snake[1].y, snake[1].x);
	addch(partchar);

	move(destGate.y, destGate.x);
	addch(gateDestChar);

	// If the snake has passed the Destination gate, Create another one
	if (snake[snake.size() - 1].x == destGate.x && snake[snake.size() - 1].y == destGate.y) {
		PositionGate();
	}

	refresh();
	return;
}

// Add items every 3 seconds
void fSnakeGame::createItems() {
	while (1) {
		this_thread::sleep_for(milliseconds(3000));
		rand() % 10 > 4 && !bIsEnded? PositionFruit() : PositionPoison();
		if (bIsEnded) {
			break;
		}
	}
	
	usleep(1000000);
	this_thread::sleep_for(milliseconds(300000000000));
}

void fSnakeGame::PlayGame() {
	thread t(&fSnakeGame::createItems, this);
	t.detach();

    while(1) {
        if (FatalCollision()) {
            // move((maxheight - 2) / 2,(maxwidth - 5) / 2);
            // printw("GAME OVER");
			bIsEnded = true;
            break;	
        }
		
        GetsFruit();
		GetsPoison();
        MoveSnake();
		updateBoard();

		//Check - If the user has completed the level
		if (checkCompleted()) {
			break;
		}
        usleep(del); // delay
    }
}
