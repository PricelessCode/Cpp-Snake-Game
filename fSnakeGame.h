/*
This is the game logic (no user interaction).
The game is a very simple version of the classic snake game.
*/

#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <thread>
#ifndef FSNAKEGAME_H
#define FSNAKEGAME_H

struct CharPosition {
	int x, y;
	CharPosition(int col, int row);
	CharPosition();
};

class fSnakeGame {
private:
	int score, del, maxwidth, maxheight, itemCount;
	char direction, headChar, partchar, edgechar, imChar, fruitchar, poisonChar;
	// partchar is the character representing the snake's body
	// edgechar is the character representing the edge of the game window
	// fruitchar is the character representing the fruit
	// del stands for delay
	bool bEatsFruit;
	bool bEatsPoison;
	CharPosition fruit; // need to clarify this combination
	CharPosition poison;
	std::vector<CharPosition> snake; // represent the snake's body
	std::vector<CharPosition> items; // Keeping track of 3 items

	void InitGameWindow();
	void DrawWindow();  
	void DrawSnake();
	void PrintScore();
	void PositionFruit();
	void PositionPoison();
	bool FatalCollision();
	void MoveSnake();
	bool GetsFruit();
	bool GetsPoison();
	void createItems();

public:
	fSnakeGame();
	~fSnakeGame(); // destructor for cleanup and memory deallocation
	void PlayGame();
};

#endif