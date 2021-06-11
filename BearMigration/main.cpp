#include "Snake.h"
#include "SnakeSetting.h"
//#include "Snake.cpp"


int maxheight, maxwidth;
void PlayGame();
int IsUserReady();
int AskUserToPlayAgain();
void ClearCentre();
int UserInput();

void ClearCentre(float x, float y){
	clear();
	initscr();
	noecho();
	getmaxyx(stdscr, maxheight, maxwidth);
	move((maxheight/y), (maxwidth/x));
}

int UserInput(){
	int UserInput = getch();
	refresh();
	endwin();
	clear();
	
	return UserInput;
}

//print Start Menu
int isUserReady(){
	ClearCentre(3, 2.5);
	printw("Welcome to the Snake ! Are U Ready? (Y/N)");
	return UserInput();
}

int AskUserToPlayAgain(){
	ClearCentre(2.5, 2.5);
	printw("Do you want to play Again? (Y/N)");
	return UserInput();
}




int main(){
	
	//wait for user
	if (isUserReady() == 'y') 
		do 
		{
			{
			Snake newSnake;
			newSnake.PlayGame();
			}
		}
		while(AskUserToPlayAgain() == 'y');
	}

