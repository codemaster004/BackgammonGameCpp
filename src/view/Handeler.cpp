//
// Created by Filip Dabkowski on 12/11/2023.
//
#include <ncurses.h>

#include "../configs/UIConfigs.h"
#include "Handeler.h"
#include "Drawing.h"
#include "../controller/InputControll.h"


Placement initMenuSpace(Pos center, MenuElement values[], int nElements) {
	uint realLen = OPTION_SPACING * (nElements - 1);
	for (int i = 0; i < nElements; ++i) {
		realLen += len(values[i].value) - 1;
	}
	return Placement{
		.min={
			.x=center.x - (int) (realLen / 2),
			.y=center.y
		},
		.max {.x=center.x + (int) (realLen / 2)}
	};
}

Placement initTextSpace(Pos center, char **values, int nElements) {
	uint realLen = OPTION_SPACING * (nElements - 1);
	for (int i = 0; i < nElements; ++i) {
		realLen += len(values[i]) - 1;
	}
	return Placement{
		.min={
			.x=center.x - (int) (realLen / 2),
			.y=center.y
		},
		.max {.x=center.x + (int) (realLen / 2)}
	};
}

void generateHeader(UserInterface &ui) {
	const char *title[3] = {ui.authorId, ui.authorName, ui.gameName};

	Placement textSpace = ui.space.gameSpace;
	textSpace.min.y = ui.space.gameSpace.min.y + HEADER_OFFSET;
	drawSpreadText(textSpace, (char **) (title), 3);
}

void createPlayerNames(UserInterface ui, char **&players, int &selected) {
	const char pawn1[] = {")("};
	const char pawn2[] = {"[]"};

	const int count = 5;
	for (int i = 0; i < N_PLAYERS; ++i) {
		players[i] = new char[MAX_NAME_LENGTH + 4];

		const char *data[count] = {
			(char *) (ui.board.players[i].id ? pawn2 : pawn1),
			" ",
			ui.board.players[i].name,
			": ",
			numberToString(ui.board.players[0].points, 2),
		};

		joinStrings(players[i], data, count);

		if (ui.board.players[i].id == ui.board.currentPlayerId)
			selected = i;
	}
}

void generatePlayers(UserInterface &ui) {

	auto players = new char *[N_PLAYERS];
	int selected = -1;
	createPlayerNames(ui, players, selected);

	Placement textSpace = ui.space.gameSpace;
	textSpace.min.y = ui.space.gameSpace.min.y + HEADER_OFFSET + 2;
	drawSpreadText(textSpace, (char **) (players), N_PLAYERS, selected);

	for (int i = 0; i < N_PLAYERS; ++i) {
		delete[] players[i];
	}
	delete[] players;
}

void generateBasicBoard(UserInterface &ui) {
	generateHeader(ui);

	generatePlayers(ui);

	attron(COLOR_PAIR(FOREGROUND));

	handleBoardOutline(ui.space.board);

	handlePieces(ui.space.board);

	// Dies
	handleDices(ui.space.dice, ui.space.boardCenter, ui.board.dices);

	attroff(COLOR_PAIR(FOREGROUND));
}

void handleCourt(UserInterface ui) {
	Pos courtTop = {ui.space.gameSpace.max.x, ui.space.boardCenter.y - COURT_OFFSET_Y - 5};
	Pos courtBottom = {ui.space.gameSpace.max.x, ui.space.boardCenter.y + COURT_OFFSET_Y};
	attron(A_BOLD);
	drawVerticalInfo(courtTop, "WHI ", ui.board.courts[0].pawnsInside, FOREGROUND_LIGHT, true);
	drawVerticalInfo(courtBottom, "BLC ", ui.board.courts[1].pawnsInside, FOREGROUND_DARK, true);
	attroff(A_BOLD);
}

void handleGameWon(UserInterface &ui) {
	resetMenuTo(ui, GAME_WON);
	ui.menu.selected = -1;
	messageSet(ui.infoMess, "Game Won");
}

void generateInteractiveUI(UserInterface &ui) {

	// Indexes
	uint digits = nDigits(nPoints, 10);
	char **indexes = new char *[nPoints];
	for (int i = 0; i < nPoints; ++i) {
		indexes[i] = numberToString(i, (int) (digits));
	}

	handleBar(ui.board.bar, ui.pickedIndex);

	handleIndexes(indexes, ui.pickedIndex, (int) (digits), ui.space.indexesTop, ui.space.indexesBottom);

	handleMenu(ui.menu, Pos{ui.space.boardCenter.x, ui.space.board.max.y + MENU_TOP_SPACING});

	handlePawnPlacement(ui.board, ui.space.board);

	handleCourt(ui);

	handleMessages(ui);

	/// CLEAR MEMORY!!!
	for (int i = 0; i < nPoints; ++i) {
		delete[] indexes[i];
	}
	delete[] indexes;
//	delete[] testMenu;

}

void handleBoardOutline(Placement space) {
	for (int i = 0; i < N_BOARDS; ++i) {
		drawBorders(space);
		moveSpace(space, {boardWidth + borders + BAR_WIDTH, 0});
	}
}

void handlePieces(Placement space) {
	space.min.x += BORDER_WIDTH;
	space.min.y += BORDER_WIDTH;
	for (int i = 0; i < N_BOARDS; ++i) {
		drawPieces(space.min.x + (boardWidth + borders + BAR_WIDTH) * i, space.min.y);
	}
}

void handleBar(Bar bar, int selected) {
	int onBar[N_PLAYERS] = {0, 0};
	for (int i = 0; i < totalPawns; ++i) {
		if (bar.pawns[i] == nullptr)
			continue;
		if (bar.pawns[i]->color == PAWN_WHITE)
			onBar[0]++;
		if (bar.pawns[i]->color == PAWN_BLACK)
			onBar[1]++;
	}

	for (int i = N_BOARDS - 1; i > 0; --i)
		drawBar(OFFSET_X + (boardWidth + borders) * i + DICE_WIDTH + borders - 1,
				OFFSET_Y + BOARD_OFFSET_Y + HEADER_OFFSET + INDEX_OFFSET + TEXT_HEIGHT * 2,
				boardHeight + borders, onBar, selected);
}

void handleDices(Placement space, Pos center, int *dices) {
	drawBorders(space);

	int dicesHeight = DICE_HEIGHT * N_DICES + (N_DICES + 1) * BORDER_WIDTH;
	space.min.y = center.y - dicesHeight / 2;
	for (int i = 0; i < N_DICES; ++i) {
		space.max.y = space.min.y + DICE_HEIGHT + 2 * BAR_WIDTH - 1;
		drawBorders(space);
		mvaddch(space.min.y + BORDER_WIDTH, space.min.x + BORDER_WIDTH + DICE_WIDTH / 2, dices[i] + 48);
		space.min.y += DICE_HEIGHT + BORDER_WIDTH;
	}
}

int generateColorsForIndexes(char **text, int count, int pickedIndex, UiColorsId *&colors) {
	int index = -1;
	for (int i = 0; i < count; ++i) {
		if (stringToNumber(text[i]) == pickedIndex) {
			index = i;
		}
	}
	if (pickedIndex == -1) {
		colors = new UiColorsId[1];
		colors[0] = FOREGROUND;
		return 1;
	} else if (index < 0) {
		colors = new UiColorsId[1];
		colors[0] = FOREGROUND_DARK;
		return 1;
	} else {
		colors = new UiColorsId[count];
		for (int i = 0; i < count; ++i) {
			colors[i] = i == index ? FOREGROUND_LIGHT : FOREGROUND_DARK;
		}
		return count;
	}
}

void handleIndexes(Placement posBot, Placement posTop, int selected, int width, char **indexes, int offsetTop,
				   int offsetBot) {
	UiColorsId *colors = nullptr;

	int nColours = generateColorsForIndexes(&indexes[offsetTop], POINTS_PER_BOARD, selected, colors);
	drawCenteredText(posBot, pieceSpacing, width,
					 &indexes[offsetTop], POINTS_PER_BOARD, colors, nColours);
	delete[] colors;

	colors = nullptr;
	nColours = generateColorsForIndexes(&indexes[offsetBot], POINTS_PER_BOARD, selected, colors);
	drawCenteredText(posTop, pieceSpacing, width,
					 &indexes[offsetBot], POINTS_PER_BOARD, colors, nColours);
	delete[] colors;
}

void handleIndexes(char **indexes, int pickedIndex, int digits, Placement posTop, Placement posBot) {
	revertTable(indexes, indexes + nPoints / 2);

	int indexWidth = pieceWidth * POINTS_PER_BOARD + pieceSpacing * (POINTS_PER_BOARD - 1);
	Pos indexChange = {indexWidth + BORDER_WIDTH * 2 + BAR_WIDTH + pieceSpacing / 2 * 2, 0};
	posTop.max.x = posTop.min.x + indexWidth;
	posBot.max.x = posBot.min.x + indexWidth;
	for (int i = 0; i < N_BOARDS; ++i) {

		handleIndexes(posBot, posTop, pickedIndex, (int) (digits),
					  indexes, i * POINTS_PER_BOARD, (int) (nPoints) / 2 + i * POINTS_PER_BOARD);

		moveSpace(posTop, indexChange);
		moveSpace(posBot, indexChange);
	}
}

void handleMenu(Menu menu, Pos center) {
	Placement menuSpace = initMenuSpace(center, menu.elements, menu.count);

	for (int i = 0; i < menu.count; ++i) {
		UiColorsId color;
		if (menu.selected == -1) {
			color = FOREGROUND;
		} else {
			if (menu.elements[i].id == menu.selected) {
				color = FOREGROUND_LIGHT;
			} else {
				color = FOREGROUND_DARK;
			}
		}
		printColor(color, menuSpace.min.x, menuSpace.min.y, menu.elements[i].value);

		menuSpace.min.x += (int) (len(menu.elements[i].value) - 1) + OPTION_SPACING;
	}
}

void placePawns(Board &game, Placement &space, int indexTop, int indexBot) {

	int count = min(game.points[indexTop].pawnsInside, POINT_HEIGHT);
	if (count) {
		space.max.y = space.min.y + count;
		drawLinePawn(*game.points[indexTop].pawns[0], space);
	}


	count = min(game.points[indexBot].pawnsInside, POINT_HEIGHT);
	if (count) {
		space.max.y = space.min.y + count;
		moveSpace(space, Pos{0, boardHeight - count});
		drawLinePawn(*game.points[indexBot].pawns[0], space);
		moveSpace(space, Pos{0, -boardHeight + count});
	}
}

void handlePawnPlacement(Board &game, Placement space) {
	space.min.x += BORDER_WIDTH + pieceSpacing / 2;
	space.min.y += BORDER_WIDTH;
	space.max.x = space.min.x;
	int change = pieceWidth + pieceSpacing;

	int base = POINTS_PER_BOARD * N_BOARDS;
	for (int i = 0; i < N_BOARDS; ++i) {
		for (int j = 0; j < POINTS_PER_BOARD; ++j) {
			int offset = i * POINTS_PER_BOARD + j;
			placePawns(game, space, base + offset, base - offset - 1);
			moveSpace(space, Pos{change, 0});
		}
		moveSpace(space, Pos{pieceSpacing / 2 * 2 + borders + BAR_WIDTH - pieceSpacing, 0});
	}
}

void handleMessages(UserInterface &ui) {
	Placement textPos = ui.space.board;
	textPos.min.y += (boardHeight + borders) / 2;
	UiColorsId colours = {FOREGROUND_DARK};
	char *tempMessages[1];

	tempMessages[0] = ui.infoMess;
	uint length = len(ui.infoMess);
	drawCenteredText(textPos, 0, (int) (length), tempMessages, 1, &colours, 1);

	moveSpace(textPos, {(N_BOARDS - 1) * (boardWidth + BAR_WIDTH + borders), 0});
	tempMessages[0] = ui.errorMess;
	length = len(ui.errorMess);
	drawCenteredText(textPos, 0, (int) (length), tempMessages, 1, &colours, 1);
}
