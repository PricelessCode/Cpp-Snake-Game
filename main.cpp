/*
This is the console executable, that makes use of the fSnakeGame class.
This is handling all user interaction. For game logic, please see fSnakeGame.h.
*/

#include "fSnakeGame.h"

int maxheight, maxwidth;
void PlayGame();
int IsUserReady();
int AskUserToPlayAgain();
void ClearCentre();
int UserInput();


int main () {	
	int stage = 0;
	if (IsUserReady() == 'y') { // wait for confirmation of the user
		while(true) {
			fSnakeGame NewSnake(stage);
			NewSnake.PlayGame();
			if (NewSnake.checkCompleted()) {
				clear();
				nodelay(stdscr, false);
				AskUserToPlayAgain();
			} else {
				break;
			} 
		}
	}
}

// clear the screen and centre the cursor
void ClearCentre(float x, float y)
{
	clear(); // clear the screen if the game is played for the 2nd time	
	initscr(); 
	noecho();
	getmaxyx(stdscr, maxheight, maxwidth);
	move((maxheight/y), (maxwidth/x));
}

// receive user confirmation
int UserInput()
{
	int UserInput = getch();
	refresh();
	endwin();
	clear();

	return UserInput;	
}

// print start menu
int IsUserReady() {
	 ClearCentre(3, 2.5);
	printw("Welcome to the Snake Game. Are you ready? (y/n)");
	return UserInput();
}

// print end of the game menu and ask user to play again
int AskUserToPlayAgain()
{
	ClearCentre(2.5, 2.5);
	// nodelay(stdscr, false);
	printw("Ready for the next round? (y/n)");
	return UserInput();
}
