//
// Created by Filip Dabkowski on 18/11/2023.
//

#include "ncurses.h"

#include "../configs/UIConfigs.h"
#include "UserInterface.h"
#include "../view/Handeler.h"
#include "InputControll.h"

UserInterface initUI() {
	auto ui = UserInterface{
		.gameName="Backgammon",
		.authorId="s197712",
		.authorName="Filip Dabkowski",
		.space=GameSpace{.gameSpace=initGameSpace()},
		.currentMove=initMove(),
		.pickedIndex=-1,
		.history=MoveMade{},
	};

	// Initialize Placements for Board Elements
	ui.space.board = initBoard(ui.space.gameSpace);
	Placement board = ui.space.board;
	ui.space.boardCenter = initCenter(board, ui.space.gameSpace);
	ui.space.dice = initDice(ui.space.gameSpace);
	ui.space.indexesTop = initIndex(board, board.min.y - INDEX_OFFSET);
	ui.space.indexesBottom = initIndex(board, board.max.y + INDEX_OFFSET);
	ui.needToRefresh = true;

	return ui;
}

Move initMove() {
	// Return a Move with all fields set to zero
	return Move{0, 0, 0};
}

void setBasicGameState(UserInterface &ui) {
	clearDices(ui.board.dices);
	ui.currentMove = {0, 0, 0};
	resetMenuTo(ui, DEFAULT);
	resetMessages(ui);
}

void titleArt(int offsetX, int offsetY) {
	attron(COLOR_PAIR(FOREGROUND)); // Turn on the foreground color pair for the title art

	// Print each line of the title art at the specified offsets
	mvprintw(offsetY + 0, offsetX, R"( ____             _     ___)") ;
	mvprintw(offsetY + 1, offsetX, R"(| __ )  __ _  ___| |__ / __|  __ _ _ __ ___  _ __ ___   ____  _ __)");
	mvprintw(offsetY + 2, offsetX, R"(|  _ \ / _` |/ __| / // / __ / _` | '_ ` _ \| '_ ` _ \ / __ \| '_ \)");
	mvprintw(offsetY + 3, offsetX, R"(| |_) | (_| | (__|   \\ \_| | (_| | | | | | | | | | | |  \/  | | | |)");
	mvprintw(offsetY + 4, offsetX, R"(|____/ \__,_|\___|_|\_\\___/ \__,_|_| |_| |_|_| |_| |_|\____/|_| |_|)");

	attroff(COLOR_PAIR(FOREGROUND)); // Turn off
}

void starterScreen(UserInterface &ui) {
	// Display the title art at a specified position
	titleArt(OFFSET_X, OFFSET_Y + boardHeight / 2);

	// Handle and display the game menu at a position relative to the board's center and maximum Y value
	handleMenu(ui.menu, Pos{ui.space.boardCenter.x, ui.space.board.max.y + MENU_TOP_SPACING});
}

void resetMessage(char message[MAX_MESSAGE_LEN]) {
	// Fill each character in the message array with a blank space
	for (int i = 0; i < MAX_MESSAGE_LEN; ++i) {
		message[i] = ' ';
	}
}

void resetMessages(UserInterface &ui) {
	resetMessage(ui.infoMess); // Reset the information message
	resetMessage(ui.errorMess); // Reset the error message
}

void messageSet(char message[MAX_MESSAGE_LEN], const char *newMessage) {
	// Determine the length to copy
	int length = min(MAX_MESSAGE_LEN - 1, (int)(len(newMessage)));

	// Copy the new message into the message array up to the determined length
	for (int i = 0; i < length; ++i) {
		message[i] = newMessage[i];
	}

	// Ensure the message is null-terminated
	message[length] = '\0';
}
