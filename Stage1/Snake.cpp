#include "Snake.h"
#include <unistd.h>
#include <thread>
#include "SnakeSetting.h"

using namespace std;
using namespace std::chrono;

CharPosition::CharPosition(int col, int row){
	x = col;
	y = row;
}

Snake::Snake(){
	direction = 'l';
	score = 0;

	srand(time(NULL));
	
	InitGameWindow();

	DrawWindow();
	DrawSnake();
	PrintScore();
	
	refresh();
}

Snake::~Snake(){
	nodelay(stdscr, true);
	getch();
	endwin();
}

void Snake::InitGameWindow(){
	initscr(); // Initialize the Screen
	nodelay(stdscr, 1);
	keypad(stdscr, true); // Initialize 
	noecho();
	curs_set(0);
	getmaxyx(stdscr, maxheight, maxwidth);
	return;
}

void Snake::DrawWindow(){
	//Draws Top
	for(int i=0; i<maxwidth; i++){
		move(0, i);
		addch(EDGECHAR);
	}
	//Draws Bottom	
	for(int i = 0; i<maxwidth; i++){
		move(maxheight-2,i);
		addch(EDGECHAR);
	}
	//Draws Left Side
	for(int i=0;i<maxheight-1;i++){
		move(i, 0);
		if(i==0 || i==maxheight - 2) addch(IMCHAR);
		else addch(EDGECHAR);
	}
	//Draws Right Side
	for(int i=0;i<maxheight-1;i++){
		move(i,maxwidth-2);
		if(i==0 || i==maxheight - 2) addch(IMCHAR);
		else addch(EDGECHAR);
	}
	return;
}

void Snake::DrawSnake(){
	for(int i=0; i<5;i++) snake.push_back(CharPosition(30+i, 30));
	
	for(int i=0;  i<snake.size();i++){
		if (i==0) addch(HEADCHAR);
		else addch(PARTCHAR);
	} 
	return;
}

void Snake::PrintScore(){
	move(maxheight-1, 0);
	printw("Score : %d", score);
	return;
}

bool Snake::FatalCollision(){
	for(int i =2; i<snake.size(); i++){
		if(snake[0].x == snake[i].x && snake[0].y == snake[i].y) return true;
	}
	if(snake.size() < 3) return true;

	return false;
}

void Snake::MoveSnake(){
	int KeyPressed = getch();
	switch(KeyPressed){
		case KEY_LEFT:
			if (direction != 'r'){ direction = 'l';}
		case KEY_RIGHT:
			if (direction != 'l') { direction = 'r';}
		case KEY_UP:
			if (direction != 'd') { direction = 'u';}
		case KEY_DOWN:
			if (direction != 'u') { direction = 'd';}
		case KEY_BACKSPACE:
			direction = 'q'; // InCase Quit 
			break;
	}
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
    addch(HEADCHAR); // add a new head

    // Change previous head to body char
    move(snake[1].y, snake[1].x);
    addch(PARTCHAR);
    refresh();
    return;
}
void Snake::PlayGame(){
	
	while(1){
		if(FatalCollision()) break;
		MoveSnake();
		if(direction=='q') break;
		usleep(DEL);
	}	
}


