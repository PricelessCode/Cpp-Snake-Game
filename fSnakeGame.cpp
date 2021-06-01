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

fSnakeGame::fSnakeGame() {
	// variables initialisation:
	headChar = 'O';
	partchar = '@'; // character to represent the snake
	edgechar = (char)219; // full rectangle on the key table
	imChar = (char)43;
	fruitchar = '*'; 
	poisonChar = 'X';
	fruit.x = 0;
	fruit.y = 0;
	poison.x = 0;
	poison.y = 0;
	score = 0;
	itemCount = 0;
	del = 110000;
	bool bEatsFruit = false;
	bool bEatsPoison = false;
	direction = 'l';
	srand(time(NULL));
	
	InitGameWindow();
	PositionFruit();
	PositionPoison();
	DrawWindow();
	DrawSnake();
	PrintScore();

	refresh();	
}

fSnakeGame::~fSnakeGame()
{
	nodelay(stdscr, true);
	getch();
	endwin();
}

// initialise the game window
void fSnakeGame::InitGameWindow() { 
	initscr(); // initialise the screen
	nodelay(stdscr,TRUE);
	keypad(stdscr, true); // initialise the keyboard: we can use arrows for directions
	noecho(); // user input is not displayed on the screen
	curs_set(0); // cursor symbol is not not displayed on the screen (Linux)
	getmaxyx(stdscr, maxheight, maxwidth); // define dimensions of game window
	return; 
}

// draw the game window
void fSnakeGame::DrawWindow()
{
	for (int i = 0; i < maxwidth; i++) // draws top
	{
		move(0, i);
		addch(edgechar);
	}

	for (int i = 0; i < maxwidth; i++) // draws bottom
	{
		move(maxheight-2, i);
		addch(edgechar);
	}

	for (int i = 0; i < maxheight-1; i++) // draws left side
	{
		move(i, 0);
		if (i == 0 || i == maxheight - 2) {
			addch(imChar);
		} else {
			addch(edgechar);	
		}
	}

	for (int i = 0; i < maxheight-1; i++) // draws right side
	{
		move(i, maxwidth-1);
		if (i == 0 || i == maxheight - 2) {
			addch(imChar);
		} else {
			addch(edgechar);	
		}
	}
	return;
}

// draw snake's body
void fSnakeGame::DrawSnake() {
	for (int i = 0; i < 5; i++) {
		snake.push_back(CharPosition(30 + i, 10));
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
	move(maxheight-1, 0);
	printw("Score: %d", score);
	return;
}

// position a new fruit in the game window
void fSnakeGame::PositionFruit() {
	while(1) {
		int tmpx = rand() % maxwidth + 1; // +1 to avoid the 0
		int tmpy = rand() % maxheight + 1;

		// check that the fruit is not positioned on the snake
		for (int i = 0; i < snake.size(); i++) {
			if (snake[i].x == tmpx && snake[i].y == tmpy) {
				continue; // if true, ignore the following and go back to the beginning of function
			}
		}

		// check that the fruit is positioned within the game window
		if (tmpx >= maxwidth-2 || tmpy >= maxheight-3) {
			continue; // if true, ignore the following and go back to the beginning of function
		}

		// if the coordinates are valid, add fruit in the window
		fruit.x = tmpx;
		fruit.y = tmpy;
		break;
	}

	move(fruit.y, fruit.x); 
	addch(fruitchar);
	refresh();
}

void fSnakeGame::PositionPoison() {
	while(true) {
		int tmpx = rand() % maxwidth + 1; // +1 to avoid the 0
		int tmpy = rand() % maxheight + 1;

		// check that the poison is not positioned on the snake
		for (int i = 0; i < snake.size(); i++) {
			if (snake[i].x == tmpx && snake[i].y == tmpy) {
				continue; // if true, ignore the following and go back to the beginning of function
			}
		}

		// Check that the poison is not positioned on the fruit
		if (fruit.x == tmpx && fruit.y == tmpy) {
			continue;
		}

		// check that the poison is positioned within the game window
		if (tmpx >= maxwidth - 2 || tmpy >= maxheight - 3) {
			continue; // if true, ignore the following and go back to the beginning of function
		}

		// if the coordinates are valid, add fruit in the window
		poison.x = tmpx;
		poison.y = tmpy;
		break;
	}

	move(poison.y, poison.x); 
	addch(poisonChar);
	refresh();
}

// set game over situations
bool fSnakeGame::FatalCollision() {
	// if the snake hits the edge of the window
	if (snake[0].x == 0 || snake[0].x == maxwidth-1 || snake[0].y == 0 || snake[0].y == maxheight-2)
	{
		return true;
	}

	// if the snake collides into himself
	for (int i = 2; i < snake.size(); i++)
	{
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
		{
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
	if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
		PositionFruit(); 
		score +=10; 
		PrintScore();

		// if score is a multiple of 100, increase snake speed
		if ((score % 100) == 0) {
			del -= 1000;
		}
		return bEatsFruit = true;
	}
	else 
	{
		return bEatsFruit = false;
	}
	return bEatsFruit;
}

bool fSnakeGame::GetsPoison() {
	if (snake[0].x == poison.x && snake[0].y == poison.y) {
		PositionPoison(); 
		score -= 10;
		PrintScore();

		return bEatsPoison = true;
	} else {
		return bEatsPoison = false;
	}
	return bEatsPoison;
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
		printw(" "); // add empty ch to remove last character
		refresh();
		snake.pop_back(); // removes the last element in the vector, reducing the container size by one
	}

	
	if (bEatsPoison) {
		move(snake[snake.size()-1].y, snake[snake.size()-1].x); // moves at the end of the tail
		printw(" "); // add empty ch to remove last character
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

	// Move to the new CharPosition coordinates
	move(snake[0].y, snake[0].x);
	addch(headChar); // add a new head

	// Change previous head to body char
	move(snake[1].y, snake[1].x);
	addch(partchar);
	refresh();
	return;
}

// Add items every 3 seconds
void fSnakeGame::createItems() {
	while(1) {
		move(4, 4);
		addch('T');
		this_thread::sleep_for(milliseconds(5000));
	}
}

void fSnakeGame::PlayGame() {
	thread t(&fSnakeGame::createItems, this);
    while(1) {
        if (FatalCollision()) {
            move((maxheight - 2) / 2,(maxwidth - 5) / 2);
            printw("GAME OVER");
			t.detach();
            break;
        }

		
        GetsFruit();
		GetsPoison();
        MoveSnake();

		// exit
        if (direction=='q') {
        	break;
        }

        usleep(del); // delay
	
    }
	
}
