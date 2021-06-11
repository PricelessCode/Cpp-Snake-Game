#include <cstdlib>
#include <ctime>
#include <thread>
#include <iostream>
#include <vector>
#include <deque>
#include <ncurses.h>

#ifndef SNAKE_H
#define SNAKE

struct CharPosition{
	int x, y;
	CharPosition(int col, int row);
	CharPosition();
};

class Snake{
private:
	// score : Total Score
	// del : delay
	// maxwidth : Max Width Value for Game Window
	// maxheight : Max Height Value for Game Window
	int score, maxwidth, maxheight; // del;
	// direction : Moving Direction of Snake
	// Headchar : Snake Head
	// partChar : Snake Body
	// edgeChar : Wall Unpassable
	// imChar : immune Wall (Passable)
	char direction;// headChar, partChar, edgeChar, imChar;
	//Represent Snake's Body
	std::vector<CharPosition> snake;
	
	void InitGameWindow();
	void DrawWindow();
	void DrawSnake();
	void PrintScore();
	bool FatalCollision();
	void MoveSnake();
	
public:
	//Constructor
	Snake();
	//Destructor
	~Snake();
	void PlayGame();
};
#endif
