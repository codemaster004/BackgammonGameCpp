//
// Created by Filip Dabkowski on 17/11/2023.
//

#ifndef BACKGAMMONGAME_UICONFIGS_H
#define BACKGAMMONGAME_UICONFIGS_H

#include "GameConfigs.h"

// ncurses does not allow creating custom colour names so let's redefine their colors to yours to standardize the code
#define COLOUR_MAIN COLOR_GREEN
#define COLOUR_MAIN_LIGHT COLOR_BLUE
#define COLOUR_MAIN_DARK COLOR_CYAN

#define BORDER_WIDTH 1
#define BAR_WIDTH 1

#define DICE_WIDTH 3
#define DICE_HEIGHT 1

#define OPTION_SPACING 3
#define MENU_TOP_SPACING 3

#define TEXT_HEIGHT 1
#define TEXT_WIDTH 1

#define OFFSET_X 15
#define OFFSET_Y 1
#define INDEX_OFFSET 1
#define HEADER_OFFSET 0
#define BOARD_OFFSET_Y 2
#define BOARD_OFFSET_X 0
#define COURT_OFFSET_X 1
#define COURT_OFFSET_Y 2
#define VERTICAL_SPACING 1

#define POINT_HEIGHT 6

#define COLOR_THEME 255,110,10

#define MAX_MESSAGE_LEN 25


enum UiColorsId {
	FOREGROUND = 1,
	FOREGROUND_LIGHT,
	FOREGROUND_DARK,
	BACKGROUND_LIGHT,
	BACKGROUND_DARK
};

const char borderHorizon = '=';
const char borderVertical = '|';
const char borderCorner = '+';

const int pieceWidth = 2;
const int pieceSpacing = 3;

const float colorDiff = 0.3;

// Calculations
const int borders = BORDER_WIDTH * 2;

const int boardWidth = POINTS_PER_BOARD * pieceWidth + pieceSpacing * (POINTS_PER_BOARD - 1) + pieceSpacing / 2 * 2;
const int boardHeight = POINT_HEIGHT * 2 + pieceSpacing;

#endif //BACKGAMMONGAME_UICONFIGS_H
