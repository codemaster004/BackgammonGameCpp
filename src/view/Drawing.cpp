//
// Created by Filip Dabkowski on 22/11/2023.
//

#include <ncurses.h>
#include "cmath"

#include "../configs/UIConfigs.h"
#include "../model/Board.h"
#include "Drawing.h"

// PRIVATE FUNCTION HEADERS //


/**
 * @brief Generates an array of color IDs for UI elements.
 *
 * @param count The total number of elements for which colors are needed.
 * @param selected The index of the selected element (highlighted differently).
 * @return UiColorsId* Pointer to the array of color IDs.
 */
UiColorsId *generateColors(int count, int selected);

/**
 * @brief Draws a line with the specified symbol from a starting position to an ending position.
 *
 * @param symbol The character symbol used to draw the line.
 * @param pos The starting and ending coordinates for the line.
 */
void drawLine(char symbol, Placement pos);

/**
 * @brief Draws a line with the specified symbol from a starting position to an ending position.
 *
 * @param symbol The string used to draw the line.
 * @param pos The starting and ending coordinates for the line.
 */
void drawLineString(const char symbol[], Placement pos);

/**
 * @brief Draws corners of a rectangle at the specified positions.
 *
 * Places a corner symbol at each of the four corners of a rectangular area defined by the given position coordinates.
 *
 * @param pos The coordinates defining the rectangular area.
 */
void drawCorners(Placement pos);

/**
 * @brief Sets a color with modified RGB values.
 *
 * @param colorName The identifier for the color.
 * @param nRed The red component of the color (0-1000).
 * @param nGreen The green component of the color (0-1000).
 * @param nBlue The blue component of the color (0-1000).
 * @param diff The multiplier to adjust the color's brightness or tone.
 */
void setColor(short colorName, short nRed, short nGreen, short nBlue, float diff);

/**
 * @brief Draws the label for the bar area on the board.
 *
 * @param offsetX The horizontal offset for the bar's position.
 * @param offsetY The vertical offset for the bar's position.
 * @param height The height of the bar area.
 * @param onBar Array containing the count of pawns each player has on the bar.
 * @param pickedPoint The index of the currently picked point on the board.
 */
void drawBarPoint(int offsetX, int offsetY, int height, const int onBar[2], int pickedPoint);

// PUBLIC FUNCTION DECLARATIONS //

void printColorString(UiColorsId color, int x, int y, const char *text) {
	attron(COLOR_PAIR(color)); // Turn on the specified color
	mvprintw(y, x, "%s", text); // Print the text at the specified coordinates
	attroff(COLOR_PAIR(color)); // Turn off the specified color
}

void printColorChar(UiColorsId color, int x, int y, char text) {
	attron(COLOR_PAIR(color)); // Turn on the specified color
	mvprintw(y, x, "%c", text); // Print the character at the specified coordinates
	attroff(COLOR_PAIR(color)); // Turn off the specified color
}

void drawLinePawn(Pawn pawn, Placement pos) {
	// Initialize Pawn Strings
	const char pawn1[] = {")("};
	const char pawn2[] = {"[]"};

	// Iterate over pawns and print as Colored String
	for (int y = pos.min.y; y < pos.max.y; y++) {
		if (pawn.color == PAWN_WHITE) {
			printColorString(BACKGROUND_LIGHT, pos.min.x, y, pawn1);
		} else if (pawn.color == PAWN_BLACK) {
			printColorString(BACKGROUND_DARK, pos.min.x, y, pawn2);
		}
	}
}

void drawBorders(Placement pos) {
	// Draw top horizontal border
	drawLine(borderHorizon, Placement{pos.min.x, pos.min.y, pos.max.x, pos.min.y});
	// Draw bottom horizontal border
	drawLine(borderHorizon, Placement{pos.min.x, pos.max.y, pos.max.x, pos.max.y});
	// Draw left vertical border
	drawLine(borderVertical, Placement{pos.min.x, pos.min.y, pos.min.x, pos.max.y});
	// Draw right vertical border
	drawLine(borderVertical, Placement{pos.max.x, pos.min.y, pos.max.x, pos.max.y});
	// Draw corner symbols
	drawCorners(pos);
}

void drawPiece(const char *symbol, int offsetX, int offsetY) {
	int totalOffset = offsetX + pieceSpacing / 2;
	// Iterate through points on the board
	for (int i = 0; i < POINTS_PER_BOARD; i += 2) {
		// Define the position for drawing the piece
		Placement offset = {
			.min = Pos{.x = totalOffset, .y = offsetY},
			.max = Pos{.x = totalOffset, .y = offsetY + POINT_HEIGHT}
		};

		// Draw the piece at the defined position
		drawLineString(symbol, offset);
		// Increment the total offset for the next piece
		totalOffset += (pieceWidth + pieceSpacing) * 2;
	}
}

void drawPieces(int offsetX, int offsetY) {
	const char piece1[] = {"/\\"};
	const char piece2[] = {"''"};

	// Draw the first set of pieces with reversed symbols
	char *reversePiece = reverseString(piece1);
	drawPiece(reversePiece, offsetX, offsetY);

	reversePiece = reverseString(piece2);
	drawPiece(reversePiece, offsetX + pieceWidth + pieceSpacing, offsetY);

	// Draw the second set of pieces with original symbols
	drawPiece(piece1, offsetX + pieceWidth + pieceSpacing,
			  offsetY + POINT_HEIGHT + pieceSpacing);
	drawPiece(piece2, offsetX, offsetY + POINT_HEIGHT + pieceSpacing);
}

void drawCenteredText(Placement pos, int spacing, int len, char **text, int count, UiColorsId *colors, int nColors) {
	// Calculate the center point of the area
	int center = pos.min.x + (pos.max.x - pos.min.x) / 2;
	// Calculate the total width of all text strings including spacing
	int textWidth = count * (len) + spacing * (count - 1);
	// Calculate the starting point to center the text
	int startPoint = center - textWidth / 2;

	// If the starting point is out of the defined area, return without drawing
	if (startPoint < pos.min.x)
		return;

	// Draw each text string at the calculated position
	for (int i = 0; i < count; ++i) {
		printColorString(colors[i % nColors], startPoint, pos.min.y, text[i]);
		// Move the starting point for the next text string
		startPoint += len + spacing;
	}
}

void drawSpreadText(Placement pos, char **text, int count, int selected) {
	// Generate colors for the text strings, highlighting the selected string
	UiColorsId *colors = generateColors(count, selected);

	// Calculate the space allocated for each word
	int wordSpace = (pos.max.x - pos.min.x) / count;

	// Initialize a temporary position for drawing each text string
	Placement tempPos = pos;
	tempPos.max.x = tempPos.min.x + wordSpace;

	// Draw each text string in its allocated space
	for (int i = 0; i < count; ++i) {
		uint textLen = len(text[i]);
		// Draw the text string centered in its allocated space
		drawCenteredText(tempPos, 0, (int) (textLen), text + i, 1, colors + i, 1);
		// Move the temporary position for the next text string
		moveSpace(tempPos, Pos{wordSpace});
	}

	// Free the memory allocated for colors
	delete[] colors;
}

void drawSpreadTextVertical(Placement pos, char **text, int count, int selected, int spacing) {
	// Generate colors for the text strings, highlighting the selected string
	UiColorsId *colors = generateColors(count, selected);

	// Calculate the space allocated for each word, including additional spacing
	int wordSpace = TEXT_HEIGHT + spacing;

	// Initialize a temporary position for drawing each text string
	Placement tempPos = pos;
	tempPos.max.y = tempPos.min.y + wordSpace;

	// Draw each text string in its allocated space
	for (int i = 0; i < count; ++i) {
		uint textLen = len(text[i]);
		// Draw the text string centered in its allocated vertical space
		drawCenteredText(tempPos, 0, (int) (textLen), text + i, 1, colors + i, 1);
		// Move the temporary position downward for the next text string
		moveSpace(tempPos, Pos{.y=wordSpace});
	}

	// Free the memory allocated for colors
	delete[] colors;
}

void setColourTheme(short baseRed, short baseGreen, short baseBlue) {
	// Convert the base RGB values from the 0-255 range to the 0-1000 range
	float nMultiplier = 1000.0 / 255.0;
	short nRed = capAt(multiply(baseRed, nMultiplier), 1000),
		nGreen = capAt(multiply(baseGreen, nMultiplier), 1000),
		nBlue = capAt(multiply(baseBlue, nMultiplier), 1000);

	// Set the main color for the theme
	setColor(COLOUR_MAIN, nRed, nGreen, nBlue, 1);
	// Set a darker variant of the main color
	setColor(COLOUR_MAIN_DARK, nRed, nGreen, nBlue, (1 - colorDiff));
	// Set a lighter variant of the main color
	setColor(COLOUR_MAIN_LIGHT, nRed, nGreen, nBlue, (1 + colorDiff));
}

void drawVertically(Pos pos, const char *text, UiColorsId color) {
	int length = (int) (len(text)); // Get the length of the text
	// Iterate through each character in the text
	for (int i = 0; i < length; ++i) {
		// Print each character vertically, moving down for each new character
		printColorChar(color, pos.x, pos.y + i, text[i]);
	}
}

void drawVerticalInfo(Pos pos, const char *label, int value, UiColorsId color, bool force) {
	// Check if the value should be displayed
	if (value > 0 || force) {
		// Convert the numeric value to a string
		char *num = numberToString(value, 2);
		// Concatenate the label with the numeric string
		char *text = joinStrings2(label, num);

		// Draw the concatenated string vertically
		drawVertically(pos, text, color);

		// Free the allocated memory for the numeric and concatenated strings
		delete[] num;
		delete[] text;
	}
}

void drawBar(int offsetX, int offsetY, int height, int onBar[2], int selected) {
	// Set the color for the bar's border
	attron(COLOR_PAIR(FOREGROUND));
	// Draw the top corner of the bar
	mvprintw(offsetY, offsetX, "%c", borderCorner);
	// Draw the vertical line representing the bar
	drawLine(borderVertical, Placement{offsetX, offsetY + 1,
									   offsetX, offsetY + height - 1});
	// Draw the bottom corner of the bar
	mvprintw(offsetY + height - 1, offsetX, "%c", borderCorner);

	// Draw the number of white pawns on the bar
	drawVerticalInfo({offsetX, offsetY + BORDER_WIDTH}, "WHT ", onBar[0]);
	// Draw the number of black pawns on the bar
	drawVerticalInfo({offsetX, offsetY + (height) / 2 + BORDER_WIDTH + 1}, "BLC ", onBar[1]);

	// Draw the label for the bar area
	drawBarPoint(offsetX, offsetY, height, onBar, selected);
}

// PRIVATE FUNCTION DECLARATIONS //

UiColorsId *generateColors(int count, int selected) {
	// Allocate memory for the color array
	auto *colors = new UiColorsId[count];
	for (int i = 0; i < count; ++i) {
		// Set colors based on whether the element is selected or not
		if (selected < 0) {
			colors[i] = FOREGROUND;
		} else {
			colors[i] = i == selected ? FOREGROUND_LIGHT : FOREGROUND_DARK;
		}
	}
	// Return the array of color IDs
	return colors;
}

void drawLine(char symbol, Placement pos) {
	// Calculate the differences in the x and y coordinates
	int deltaX = pos.max.x - pos.min.x;
	int deltaY = pos.max.y - pos.min.y;

	// Determine the number of steps required to draw the line
	int steps = max(abs(deltaX), abs(deltaY));

	// Calculate the increments in x and y for each step
	float xIncrement = (float) (deltaX) / (float) (steps);
	float yIncrement = (float) (deltaY) / (float) (steps);

	// Initialize starting positions
	auto x = (float) (pos.min.x);
	auto y = (float) (pos.min.y);

	// Draw the line by printing the symbol at each step
	for (int i = 0; i < steps; ++i) {
		mvprintw((int) (y), (int) (x), "%c", symbol);
		x += xIncrement; // Increment x position
		y += yIncrement; // Increment y position
	}
}


void drawLineString(const char symbol[], Placement pos) {
	// Calculate the differences in the x and y coordinates
	int deltaX = pos.max.x - pos.min.x;
	int deltaY = pos.max.y - pos.min.y;

	// Determine the number of steps required to draw the line
	int steps = max(abs(deltaX), abs(deltaY));

	// Calculate the increments in x and y for each step
	float xIncrement = (float) (deltaX) / (float) (steps);
	float yIncrement = (float) (deltaY) / (float) (steps);

	// Initialize starting positions
	auto x = (float) (pos.min.x);
	auto y = (float) (pos.min.y);

	// Draw the line by printing the symbol at each step
	for (int i = 0; i < steps; ++i) {
		mvprintw((int) (y), (int) (x), "%s", symbol);
		x += xIncrement; // Increment x position
		y += yIncrement; // Increment y position
	}
}

void drawCorners(Placement pos) {
	// Draw corner symbol in the top-left corner
	mvprintw(pos.min.y, pos.min.x, "%c", borderCorner);
	// Draw corner symbol in the top-right corner
	mvprintw(pos.min.y, pos.max.x, "%c", borderCorner);
	// Draw corner symbol in the bottom-left corner
	mvprintw(pos.max.y, pos.min.x, "%c", borderCorner);
	// Draw corner symbol in the bottom-right corner
	mvprintw(pos.max.y, pos.max.x, "%c", borderCorner);
}

void setColor(short colorName, short nRed, short nGreen, short nBlue, float diff) {
	// Initialize the color with modified RGB values, capped within the valid range
	init_color(colorName,
			   capAt(multiply(nRed, diff), 1000),    // Adjust and cap the red component
			   capAt(multiply(nGreen, diff), 1000),  // Adjust and cap the green component
			   capAt(multiply(nBlue, diff), 1000));  // Adjust and cap the blue component
}

void drawBarPoint(int offsetX, int offsetY, int height, const int onBar[2], int pickedPoint) {
	const char barLabel[] = {"[BAR]"};
	// Default color for the bar label
	UiColorsId color = FOREGROUND;

	// Change the color if the bar is the picked point and has pawns
	if ((onBar[0] || onBar[1]) && pickedPoint >= 0) {
		color = pickedPoint == nPoints ? FOREGROUND_LIGHT : FOREGROUND_DARK;
	}

	// Print the bar label at the specified position with the determined color
	printColorString(color, offsetX - (int) (sizeof(barLabel)) / 2 + 1, offsetY + (height) / 2, barLabel);
}
