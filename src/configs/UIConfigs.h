//
// Created by Filip Dabkowski on 17/11/2023.
//

#ifndef BACKGAMMONGAME_UICONFIGS_H
#define BACKGAMMONGAME_UICONFIGS_H

// ncurses does not allow creating custom colour names so let's redefine their colors to yours to standardize the code
#define COLOUR_MAIN COLOR_GREEN
#define COLOUR_MAIN_LIGHT COLOR_BLUE
#define COLOUR_MAIN_DARK COLOR_CYAN

#define BORDER_WIDTH 1

const char borderHorizon[] = { "=" };
const char borderVertical[] = { "|" };
const char borderCorner[] = { "+" };

const int boardOffsetX = 2;
const int boardOffsetY = 0;

const char piece1[] = { "/\\" };
const char piece2[] = { "''" };

const int pieceWidth = 2;
const int pieceSpacing = 3;

const float colorContrast = 0.1;

#endif //BACKGAMMONGAME_UICONFIGS_H
