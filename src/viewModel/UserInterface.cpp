//
// Created by Filip Dabkowski on 18/11/2023.
//

#include "ncurses.h"

#include "../configs/UIConfigs.h"
#include "../configs/MenuConfigs.h"
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

void redefineMenu(Menu &menu) {
	menu.selected = 0;
	char **options;
	char *keys;
	switch (menu.mode) {
		case DEFAULT:
			menu.elementsCount = N_GAME_MENU_OPTIONS;
			options = (char **)(menuGameOptions);
			keys = (char *) (menuGameKeys);
			break;
		case STARTING_GAME:
			menu.elementsCount = N_STARTER_MENU_OPTIONS;
			options = (char **)(menuStarterOptions);
			keys = (char *) (menuStarterKeys);
			break;
		case PICK_POINT:
			break;
	}
	menu.elements = new MenuElement [menu.elementsCount];
	for (int i = 0; i < menu.elementsCount; ++i) {
		menu.elements[i] = MenuElement {.id=i, .key=keys[i], .value=options[i]};
	}
}
