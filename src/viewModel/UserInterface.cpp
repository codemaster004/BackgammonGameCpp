//
// Created by Filip Dabkowski on 18/11/2023.
//

#include "ncurses.h"

#include "../configs/UIConfigs.h"
#include "UserInterface.h"
#include "../view/Handeler.h"

UserInterface initUI() {
	UserInterface ui = UserInterface{
		.authorId=197712,
//		.menuMode=DEFAULT,
		.space=GameSpace{initBoard(),
		}
	};
	Placement board = ui.space.board;
	ui.space.boardCenter = initCenter(board);
	ui.space.dice = initDice(board);
	ui.space.indexesTop = initIndex(board, board.min.y - 1);
	ui.space.indexesBottom = initIndex(board, board.max.y + 1);
	ui.needToRefresh = true;

	return ui;
}

void titleArt(int offsetX, int offsetY) {
	attron(COLOR_PAIR(FOREGROUND));
	mvprintw(offsetY + 0, offsetX, R"( ____             _     ___)") ;
	mvprintw(offsetY + 1, offsetX, R"(| __ )  __ _  ___| |__ / __|  __ _ _ __ ___  _ __ ___   ____  _ __)");
	mvprintw(offsetY + 2, offsetX, R"(|  _ \ / _` |/ __| / // / __ / _` | '_ ` _ \| '_ ` _ \ / __ \| '_ \)");
	mvprintw(offsetY + 3, offsetX, R"(| |_) | (_| | (__|   \\ \_| | (_| | | | | | | | | | | |  \/  | | | |)");
	mvprintw(offsetY + 4, offsetX, R"(|____/ \__,_|\___|_|\_\\___/ \__,_|_| |_| |_|_| |_| |_|\____/|_| |_|)");
	attroff(COLOR_PAIR(FOREGROUND));
}

void starterScreen(UserInterface &ui) {
	// TODO: Const
	titleArt(35, 12);
	handleMenu(ui.menu, Pos{ui.space.boardCenter.x, ui.space.board.max.y + MENU_TOP_SPACING});
}

// TODO: new config
void createStarterMenu(Menu &menu) {
	menu.elementsCount = 3;
	menu.selected = 0;
	menu.elements = new MenuElement [menu.elementsCount];
	menu.elements[0] = MenuElement {.id=0, .key='n', .value="(N)ew Game"};
	menu.elements[1] = MenuElement {.id=1, .key='l', .value="(L)oad Game"};
	menu.elements[2] = MenuElement {.id=2, .key='q', .value="(Q)uit"};
}

void createGameMenu(Menu &menu) {
	menu.elementsCount = 5;
	menu.selected = 0;
	menu.elements = new MenuElement [menu.elementsCount];
	menu.elements[0] = MenuElement {.id=0, .key='m', .value="(M)ove"};
	menu.elements[1] = MenuElement {.id=1, .key='r', .value="(R)oll"};
	menu.elements[2] = MenuElement {.id=2, .key='u', .value="(U)ndo"};
	menu.elements[3] = MenuElement {.id=3, .key='s', .value="(S)ave"};
	menu.elements[4] = MenuElement {.id=4, .key='q', .value="(Q)uit"};
}

void redefineMenu(Menu &menu) {
	switch (menu.mode) {
		case DEFAULT:
			createGameMenu(menu);
			break;
		case STARTING_GAME:
			createStarterMenu(menu);
			break;
		case PICK_POINT:
			break;
	}
}
