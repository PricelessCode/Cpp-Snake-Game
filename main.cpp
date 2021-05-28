#include <ncurses.h>
#include <iostream>


#define WHITE 1
#define BLACK 2
#define BLUE 3

int main() {
    initscr();
    resize_term(50,100);
    start_color();
    init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);
    init_pair(BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLUE);
    
    // Background color for the mother window
    bkgd(COLOR_PAIR(WHITE));

    // Remove the cursor
    curs_set(0);
    
    refresh();

    // Game Window
    WINDOW * gameWindow = newwin(25, 50, 1, 1);
    wbkgd(gameWindow, COLOR_PAIR(BLUE));
    wattron(gameWindow, COLOR_PAIR(BLACK));
    wborder(gameWindow, '#', '#', '#', '#', '#', '#', '#', '#');
    wrefresh(gameWindow);


    getch();
    delwin(gameWindow);
    endwin();

    return 0;
}