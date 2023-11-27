//
// Created by Filip Dabkowski on 18/11/2023.
//

#include "UserInterface.h"

UserInterface initUI() {
	UserInterface ui = UserInterface{
		.authorId=197712,
		.menuMode=DEFAULT,
		.space=GameSpace{initBoard(),
		}
	};
	Placement board = ui.space.board;
	ui.space.boardCenter = initCenter(board);
	ui.space.dice = initDice(board);
	ui.space.indexesTop = initIndex(board, board.min.y - 1);
	ui.space.indexesBottom = initIndex(board, board.max.y + 1);

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

	// TODO: calculate
	auto *testMenu = new MenuElement[3];
	testMenu[0] = MenuElement {.id=0, .value="(N)ew Game"};
	testMenu[1] = MenuElement {.id=1, .value="(L)oad Game"};
	testMenu[2] = MenuElement {.id=2, .value="(Q)uit"};

	handleMenu(testMenu, 3, ui.menu.selected,
			   Pos{ui.space.boardCenter.x, ui.space.board.max.y + MENU_TOP_SPACING});

}
