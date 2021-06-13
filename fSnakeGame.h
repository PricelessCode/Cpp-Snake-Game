/*
This is the game logic (no user interaction).
The game is a very simple version of the classic snake game.
*/

#include <iostream>
#include <vector>
#include <deque>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <map>
#include <thread>
#ifndef FSNAKEGAME_H
#define FSNAKEGAME_H

struct CharPosition {
	int x, y;
	CharPosition(int col, int row);
	CharPosition();
};

struct Stage {
	std::vector<std::vector<int>> walls;
	int length, nFruit, nPoison, nGate;	
};

struct Item {
	CharPosition pos;
	bool isFruit;
	Item(int col, int row, bool isFruit);
};

class fSnakeGame {
private:
	int score, del, gameScreenWidth, gameScreenHeight, maxwidth, maxheight, itemIndex;
	char direction, headChar, partchar, edgechar, imChar, fruitchar, poisonChar;
	char gateChar;
	char gateEntChar;
	char gateDestChar;
	bool bPassedGates;
	// partchar is the character representing the snake's body
	// edgechar is the character representing the edge of the game window
	// fruitchar is the character representing the fruit
	// del stands for delay
	bool bEatsFruit;
	bool bEatsPoison;
	CharPosition fruit; // need to clarify this combination
	CharPosition poison;
	CharPosition entGate;
	CharPosition destGate;
	std::vector<CharPosition> snake; // represent the snake's body
	
	std::deque<Item> items; // Keeping track of 3 items
	


	void InitGameWindow();
	void DrawSnake();
	void drawItems();
	void PrintScore();
	void PositionFruit();
	void PositionPoison();
	void PositionGate();
	bool FatalCollision();
	void MoveSnake();
	bool GetsFruit();
	bool GetsPoison();
	void createItems();

	// Variables, Methods for stages
	std::vector<Stage> stages;
	int stage;
	int initSnakeSize;
	int cntFruit, cntPoison, cntGate;
	bool bLength, bFruit, bPoison, bGate;

	CharPosition scoreScreen;
	CharPosition missionScreen;
	void createStages();
	void DrawWindow();
	void updateBoard();

public:
	fSnakeGame(int);
	~fSnakeGame(); // destructor for cleanup and memory deallocation
	bool checkCompleted();
	void PlayGame();
};

#endif