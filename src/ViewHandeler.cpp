//
// Created by Filip Dabkowski on 12/11/2023.
//
#include <ncurses.h>
#include "cmath"
#include "string"

#include "configs/UIConfigs.h"
#include "configs/GameConfigs.h"
#include "ViewHandeler.h"
#include "Utility.h"
#include "ViewModel.h"


Placement initBoard() {
	Placement space {.min={OFFSET_X, OFFSET_Y}};
	space.max={space.min.x + N_BOARDS * (boardWidth + borders + BAR_WIDTH) - BAR_WIDTH - 1,
			   space.min.y + boardHeight + borders - 1};
	return space;
}

Placement initDice(Placement board) {
	return Placement {
		.min={board.max.x, board.min.y},
		.max={board.max.x + DICE_WIDTH + borders - 1, board.max.y},
	};
}

Pos initCenter(Placement space) {
	return Pos {
		.x=space.min.x + (space.max.x - space.min.x) / 2,
		.y=space.min.y + (space.max.y - space.min.y) / 2
	};
}

// TODO: Implement UI generating
void uiStaff(UserInterface *ui, const int *menuSelected, int *dice1, int *dice2) {

	auto *testMenu = new MenuElement[N_MENU_OPTIONS];
	for (int i = 0; i < N_MENU_OPTIONS; ++i) {
		testMenu[i] = MenuElement{.id=i, .value=menuOptions[i]};
	}

	// TODO: Separete functionS
	Placement boardSpace = initBoard();
	attron(COLOR_PAIR(FOREGROUND));

	handleBoardOutline(boardSpace);
	handleBar();
	handlePieces(boardSpace);

	Pos boardCenter = initCenter(boardSpace);

	// Dies
	Placement diceSpace = initDice(boardSpace);
	handleDices(diceSpace, boardCenter);

	// Indexes
	uint digits = nDigits(nPoints, 10);
	char **indexes = new char *[nPoints];
	for (int i = 0; i < nPoints; ++i) {
		indexes[i] = numberToString(i, (int) (digits));
	}
	drawIndexes(indexes, DEFAULT, (int) (digits), boardSpace);

	attroff(COLOR_PAIR(FOREGROUND));

	drawMenu(testMenu, N_MENU_OPTIONS, *menuSelected, boardCenter.x,
			 boardSpace.max.y + MENU_TOP_SPACING);

	// Refresh the screen to show changes
	refresh();

	/// CLEAR MEMORY!!!
	for (int i = 0; i < nPoints; ++i) {
		delete[] indexes[i];
	}
	delete[] indexes;
	delete[] testMenu;
}

void handleBoardOutline(Placement space) {
	space.max.y = space.min.y + boardHeight + borders - 1;
	for (int i = 0; i < N_BOARDS; ++i) {
		space.max.x = space.min.x + boardWidth + borders - 1;
		drawBorders(space);

		space.min.x += boardWidth + borders + BAR_WIDTH;
	}
}

void handlePieces(Placement space) {
	space.min.x += BORDER_WIDTH;
	space.min.y += BORDER_WIDTH;
	for (int i = 0; i < N_BOARDS; ++i) {
		drawPieces(space.min.x + (boardWidth + borders + BAR_WIDTH) * i, space.min.y);
	}
}

void handleBar() {
	for (int i = N_BOARDS - 1; i > 0; --i)
		drawBar(OFFSET_X + (boardWidth + borders) * i, OFFSET_Y, boardHeight + borders);
}

void handleDices(Placement space, Pos center) {
	drawBorders(space);

	int dicesHeight = DICE_HEIGHT * N_DICES + (N_DICES + 1) * BORDER_WIDTH;
	space.min.y = center.y - dicesHeight / 2;
	for (int i = 0; i < N_DICES; ++i) {
		space.max.y = space.min.y + DICE_HEIGHT + 2 * BAR_WIDTH - 1;
		drawBorders(space);
		mvaddch(space.min.y + BORDER_WIDTH, space.min.x + BORDER_WIDTH + DICE_WIDTH / 2, 0 + 48);
		space.min.y += DICE_HEIGHT + BORDER_WIDTH;
	}
}

void setColourTheme(short baseRed, short baseGreen, short baseBlue) {
	float nMultiplier = 1000.0 / 255.0;
	short nRed = multiplyFloat(baseRed, nMultiplier, 1000.0),
		nGreen = multiplyFloat(baseGreen, nMultiplier, 1000.0),
		nBlue = multiplyFloat(baseBlue, nMultiplier, 1000.0);
	init_color(COLOUR_MAIN, nRed, nGreen, nBlue);
	// TODO: Conversion in a function
	init_color(COLOUR_MAIN_DARK,
			   multiplyFloat(nRed, (1 - colorDiff), 1000.0),
			   multiplyFloat(nGreen, (1 - colorDiff), 1000.0),
			   multiplyFloat(nBlue, (1 - colorDiff), 1000.0));
	init_color(COLOUR_MAIN_LIGHT,
			   multiplyFloat(nRed, (1 + colorDiff), 1000.0),
			   multiplyFloat(nGreen, (1 + colorDiff), 1000.0),
			   multiplyFloat(nBlue, (1 + colorDiff), 1000.0));
}

void drawCorners(Placement pos) {
	mvprintw(pos.min.y, pos.min.x, borderCorner);
	mvprintw(pos.min.y, pos.max.x, borderCorner);
	mvprintw(pos.max.y, pos.min.x, borderCorner);
	mvprintw(pos.max.y, pos.max.x, borderCorner);
}

void drawBorders(Placement pos) {
	drawLine(borderHorizon, Placement{pos.min.x, pos.min.y, pos.max.x, pos.min.y});
	drawLine(borderHorizon, Placement{pos.min.x, pos.max.y, pos.max.x, pos.max.y});
	drawLine(borderVertical, Placement{pos.min.x, pos.min.y, pos.min.x, pos.max.y});
	drawLine(borderVertical, Placement{pos.max.x, pos.min.y, pos.max.x, pos.max.y});
	drawCorners(pos);
}

void drawPiece(const char *symbol, int offsetX, int offsetY) {
	int totalOffset = offsetX + pieceSpacing / 2;
	for (int i = 0; i < PAWNS_PER_POINT; i += 2) {
		Placement offset = {
			.min = Pos{.x = totalOffset, .y = offsetY},
			.max = Pos{.x = totalOffset, .y = offsetY + PAWNS_PER_POINT}
		};
		// placement
		drawLine(symbol, offset);
		totalOffset += (pieceWidth + pieceSpacing) * 2;
	}
}

void drawLine(const char *symbol, Placement pos) {
	int deltaX = pos.max.x - pos.min.x;
	int deltaY = pos.max.y - pos.min.y;

	int steps = max(abs(deltaX), abs(deltaY));

	float xIncrement = (float) (deltaX) / (float) (steps);
	float yIncrement = (float) (deltaY) / (float) (steps);

	auto x = (float) (pos.min.x);
	auto y = (float) (pos.min.y);

	for (int i = 0; i < steps; ++i) {
		mvprintw((int) (y), (int) (x), "%s", symbol);
		x += xIncrement;
		y += yIncrement;
	}
}

void drawPieces(int offsetX, int offsetY) {
	char *reversePiece = reverseString(piece1);
	drawPiece(reversePiece, offsetX, offsetY);
	reversePiece = reverseString(piece2);
	drawPiece(reversePiece, offsetX + pieceWidth + pieceSpacing, offsetY);
	drawPiece(piece1, offsetX + pieceWidth + pieceSpacing,
			  offsetY + PAWNS_PER_POINT + pieceSpacing);
	drawPiece(piece2, offsetX, offsetY + PAWNS_PER_POINT + pieceSpacing);
}

void drawBar(int offsetX, int offsetY, int height) {
	mvprintw(offsetY, offsetX, borderCorner);
	drawLine(borderVertical, Placement{offsetX, offsetY + 1,
									   offsetX, offsetY + height - 1});
	mvprintw(offsetY + height - 1, offsetX, borderCorner);

	mvprintw(offsetY + (height) / 2, offsetX - (int) (sizeof(barLabel)) / 2 + 1, barLabel);
}

void drawIndexes(char **indexes, MenuMode drawMode, int digits, Placement pos) {

	revertTable(indexes, indexes + nPoints / 2);

	int start = pos.min.x + BORDER_WIDTH + pieceSpacing / 2;
	for (int i = 0; i < N_BOARDS; ++i) {
		int change = pieceWidth * POINTS_PER_BOARD + pieceSpacing * (POINTS_PER_BOARD - 1);
		drawSpacedText(start, start + change, pos.max.y + 1, pieceSpacing, (int) (digits),
					   &indexes[i * POINTS_PER_BOARD], POINTS_PER_BOARD);
		drawSpacedText(start, start + change, pos.min.y - 1, pieceSpacing, (int) (digits),
					   &indexes[nPoints / 2 + i * POINTS_PER_BOARD], POINTS_PER_BOARD);
		start += change + BORDER_WIDTH * 2 + BAR_WIDTH + pieceSpacing / 2 * 2;
	}
}

void drawSpacedText(int minX, int maxX, int offsetY, int spacing, int len, char **text, int count) {
	int center = minX + (maxX - minX) / 2;
	int textWidth = count * (len) + spacing * (count - 1);
	int startPoint = center - textWidth / 2;
	if (startPoint < minX)
		return;

	for (int i = 0; i < count; ++i) {
		mvprintw(offsetY, startPoint, "%s", text[i]);
		startPoint += len + spacing;
	}
}

// TODO: DIVIDE
void drawMenu(MenuElement *options, int length, int selected, int centerX, int offsetY) {
	uint menuRealLen = OPTION_SPACING * (length - 1);
	for (int i = 0; i < length; ++i) {
		menuRealLen += len(options[i].value) - 1;
	}

	int startingPoint = centerX - (int) (menuRealLen / 2);
	for (int i = 0; i < length; ++i) {
		// TODO: Maybe callback?
		if (options[i].id == selected) {
			attron(COLOR_PAIR(FOREGROUND_LIGHT));
			mvprintw(offsetY, startingPoint, "%s", options[i].value);
			attroff(COLOR_PAIR(FOREGROUND_LIGHT));
		} else {
			attron(COLOR_PAIR(FOREGROUND_DARK));
			mvprintw(offsetY, startingPoint, "%s", options[i].value);
			attroff(COLOR_PAIR(FOREGROUND_DARK));
		}
		startingPoint += (int) (len(options[i].value) - 1) + OPTION_SPACING;
	}
}
