//
// Created by Filip Dabkowski on 22/11/2023.
//

#ifndef BACKGAMMONGAME_DRAWING_H
#define BACKGAMMONGAME_DRAWING_H

#include "../configs/UIConfigs.h"


/**
 * @brief Prints a string with a specified color at a given position.
 *
 * @param color The color ID to use for the text.
 * @param x The x-coordinate where the text begins.
 * @param y The y-coordinate where the text begins.
 * @param text The string to be printed.
 */
void printColorString(UiColorsId color, int x, int y, const char *text);

/**
 * @brief Prints a character with a specified color at a given position.
 *
 * @param color The color ID to use for the character.
 * @param x The x-coordinate where the character is placed.
 * @param y The y-coordinate where the character is placed.
 * @param text The character to be printed.
 */
void printColorChar(UiColorsId color, int x, int y, char text);

/**
 * @brief Draws borders of a rectangle using line drawing functions.
 *
 * @param pos The coordinates defining the rectangular area.
 */
void drawBorders(Placement pos);

/**
 * @brief Draws pieces on the board with a specified symbol and offset.
 *
 * @param symbol The symbol representing a piece.
 * @param offsetX The horizontal offset for the piece's position.
 * @param offsetY The vertical offset for the piece's position.
 */
void drawPiece(const char *symbol, int offsetX, int offsetY);

/**
 * @brief Draws all pieces on the board with specified offsets and symbols.
 *
 * @param offsetX The horizontal offset for the pieces' position.
 * @param offsetY The vertical offset for the pieces' position.
 */
void drawPieces(int offsetX, int offsetY);

/**
 * @brief Draws multiple text strings centered within a specified area.
 *
 * @param pos The coordinates defining the area where the text should be centered.
 * @param spacing The space between each text string.
 * @param len The length of each text string.
 * @param text Array of strings to be drawn.
 * @param count The number of strings in the text array.
 * @param colors Array of color IDs for coloring each text string.
 * @param nColors The number of colors in the colors array.
 */
void drawCenteredText(Placement pos, int spacing, int len, char **text, int count, UiColorsId *colors, int nColors);

/**
 * @brief Draws multiple text strings evenly spread across a specified area.
 *
 * @param pos The coordinates defining the area where the text should be spread.
 * @param text Array of strings to be drawn.
 * @param count The number of strings in the text array.
 * @param selected The index of the selected text string (to be highlighted).
 */
void drawSpreadText(Placement pos, char **text, int count, int selected=-1);

/**
 * @brief Draws multiple text strings vertically spread across a specified area.
 *
 * @param pos The coordinates defining the vertical area where the text should be spread.
 * @param text Array of strings to be drawn.
 * @param count The number of strings in the text array.
 * @param selected The index of the selected text string (to be highlighted).
 * @param spacing The additional space between each text string.
 */
void drawSpreadTextVertical(Placement pos, char **text, int count, int selected, int spacing);

/**
 * @brief Draws Pawns symbol with the specified symbol from a starting position to an ending position.
 *
 * @param symbol The string used to draw the line.
 * @param pos The starting and ending coordinates for the line.
 */
void drawLinePawn(Pawn pawn, Placement pos);

/**
 * @brief Sets the color theme based on the provided base RGB values.
 *
 * @param baseRed The base red component of the color (0-255).
 * @param baseGreen The base green component of the color (0-255).
 * @param baseBlue The base blue component of the color (0-255).
 */
void setColourTheme(short baseRed = 0, short baseGreen = 166, short baseBlue = 0);

/**
 * @brief Draws text vertically at a specified position with a given color.
 *
 * @param pos The starting position for the text.
 * @param text The string to be drawn vertically.
 * @param color The color ID to use for the text.
 */
void drawVertically(Pos pos, const char *text, UiColorsId color = FOREGROUND);

/**
 * @brief Draws a label with a numeric value vertically at a specified position with a given color.
 *
 * @param pos The starting position for the text.
 * @param label The label text to be prefixed to the numeric value.
 * @param value The numeric value to be displayed after the label.
 * @param color The color ID to use for the text.
 * @param force A boolean indicating whether to display the text even if the value is zero or negative.
 */
void drawVerticalInfo(Pos pos, const char *label, int value, UiColorsId color = FOREGROUND, bool force = false);

/**
 * @brief Draws the bar area on the board, including the border, pawn counts, and label.
 *
 * @param offsetX The horizontal offset for the bar's position.
 * @param offsetY The vertical offset for the bar's position.
 * @param height The height of the bar area.
 * @param onBar Array containing the count of pawns each player has on the bar.
 * @param selected The index of the currently picked point on the board.
 */
void drawBar(int offsetX, int offsetY, int height, int onBar[2], int selected);

#endif //BACKGAMMONGAME_DRAWING_H
