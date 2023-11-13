//
// Created by Filip Dabkowski on 12/11/2023.
//
#include <ncurses.h>
// TODO: Figure out how this works

using namespace std;

// TODO: Implement UI generating
void uiStaff() {
    initscr();            // Start curses mode
    start_color();        // Start the color functionality

    // Define colors
    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    // Draw the upper part of the board
    for (int i = 0; i < 12; i++) {
        mvprintw(2, 4 + i * 4, "V");  // Change coordinates accordingly
    }
    // ... Continue for each row required to create the pawns look

    // Draw the lower part of the board
    for (int i = 0; i < 12; i++) {
        mvprintw(10, 4 + i * 4, "^");  // Change coordinates accordingly
    }
    // ... Continue for each row required to create the pawns look

    // Draw the middle bar
    attron(COLOR_PAIR(1));
    mvprintw(6, 20, "[BAR]");
    attroff(COLOR_PAIR(1));

    // Draw the score and other UI elements similarly using mvprintw

    // Refresh the screen to show changes
    refresh();

    // Wait for user input
    getch();

    // End curses mode
    endwin();
}