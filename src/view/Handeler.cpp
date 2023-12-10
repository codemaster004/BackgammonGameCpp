//
// Created by Filip Dabkowski on 12/11/2023.
//
#include <ncurses.h>

#include "../configs/UIConfigs.h"
#include "Handeler.h"
#include "Drawing.h"
#include "../viewModel/InputControll.h"
#include "../model/SerializeToFile.h"
#include "../viewModel/Menu.h"

// PRIVATE FUNCTION HEADERS //

/**
 * @brief Initializes the placement area for a menu based on its elements and a central position.
 *
 * @param center The central position around which the menu will be aligned.
 * @param values Array of MenuElement structures representing the menu options.
 * @param nElements The number of elements in the menu.
 * @return Placement The calculated placement area for the menu.
 */
Placement initMenuSpace(Pos center, MenuElement values[], int nElements);

/**
 * @brief Initializes the placement area for a group of text strings based on their lengths and a central position.
 *
 * @param center The central position around which the text will be aligned.
 * @param values Array of strings representing the text elements.
 * @param nElements The number of text elements.
 * @return Placement The calculated placement area for the text.
 */
Placement initTextSpace(Pos center, char **values, int nElements);

/**
 * @brief Generates and displays the header for the user interface.
 *
 * @param ui Reference to the UserInterface structure containing the game and author information.
 */
void generateHeader(UserInterface &ui);

/**
 * @brief Creates the player names with their corresponding points for display.
 *
 * @param ui The UserInterface structure containing the game state and player information.
 * @param players Reference to an array of strings that will hold the player names and points.
 * @param selected Reference to an integer that will be set to the index of the current player.
 */
void createPlayerNames(UserInterface ui, char **&players, int &selected);

/**
 * @brief Generates and displays the player names and scores on the user interface.
 *
 * @param ui Reference to the UserInterface structure containing the game state and display area.
 */
void generatePlayers(UserInterface &ui);

/**
 * @brief Displays the number of pawns each player has in their respective court.
 *
 * @param ui Reference to the UserInterface structure containing the game state and layout information.
 */
void handleCourt(UserInterface ui);

/**
 * @brief Updates the scores in the user interface and saves them to a file.
 *
 * @param ui Reference to the UserInterface structure containing both the current game state and the player scores.
 */
void updateScores(UserInterface &ui);

/**
 * @brief Awards points to the winning player based on the end-game conditions.
 *
 * @param ui Reference to the UserInterface structure containing the game state and player scores.
 */
void givePointsForWinning(UserInterface &ui);

/**
 * @brief Generates an array of color IDs for a set of text strings based on a picked index.
 *
 * @param text Array of strings representing the menu options or text elements.
 * @param count The number of elements in the text array.
 * @param pickedIndex The index of the picked or selected element.
 * @param colors Reference to an array of color IDs that will be generated.
 * @return int The number of colors in the generated array (usually equal to the number of text elements).
 */
int generateColorsForIndexes(char **text, int count, int pickedIndex, UiColorsId *&colors);

/**
 * @brief Handles the display of board indexes at the top and bottom of the board.
 *
 * @param posBot The placement area for the bottom row of indexes.
 * @param posTop The placement area for the top row of indexes.
 * @param selected The index of the selected point on the board.
 * @param width The width of each index element.
 * @param indexes Array of index strings.
 * @param offsetTop The starting index for the top row of indexes.
 * @param offsetBot The starting index for the bottom row of indexes.
 */
void displayCurrentIndexes(Placement posBot, Placement posTop, int selected, int width, char **indexes, int offsetTop,
						   int offsetBot);

/**
 * @brief Draws pawns at specific board points, both at the top and bottom of the board.
 *
 * @param game Reference to the Board structure containing the game state.
 * @param space The placement area for drawing the pawns.
 * @param indexTop The index of the point at the top of the board to draw pawns for.
 * @param indexBot The index of the point at the bottom of the board to draw pawns for.
 */
void handlePawnDrawing(Board &game, Placement &space, int indexTop, int indexBot);

// PUBLIC FUNCTION DECLARATIONS //

void generateBasicBoard(UserInterface &ui) {
	// Draw the header with game and author information
	generateHeader(ui);

	// Display the player names and scores
	generatePlayers(ui);

	// Set the color for drawing the board elements
	attron(COLOR_PAIR(FOREGROUND));

	// Draw the outline of the board
	handleBoardOutline(ui.space.board);

	// Draw the pieces on the board
	handlePieces(ui.space.board);

	// Draw the dice showing the current roll
	handleDices(ui.space.dice, ui.space.boardCenter, ui.board.dices);

	// Turn off the color attribute
	attroff(COLOR_PAIR(FOREGROUND));
}

void handleGameWon(UserInterface &ui) {
	// Update the menu to reflect the game-won state
	resetMenuTo(ui, GAME_WON);

	// Award points to the winning player based on the end-game conditions
	givePointsForWinning(ui);

	// Reset the selected menu item and the winner player ID for the next game
	ui.menu.selected = -1;
	ui.board.winnerPlayerId = -1;

	// Set the info message to indicate that the game has been won
	messageSet(ui.infoMess, "Game Won");
}

void generateInteractiveUI(UserInterface &ui) {
	// Generate index labels for the board
	uint digits = nDigits(nPoints, 10);
	char **indexes = new char *[nPoints];
	for (int i = 0; i < nPoints; ++i) {
		indexes[i] = numberToString(i, (int) (digits));
	}

	// Handle the display of the bar area
	handleBar(ui.board.bar, ui.pickedIndex);

	// Handle the display of board indexes at the top and bottom
	handleIndexes(indexes, ui.pickedIndex, (int) (digits), ui.space.indexesTop, ui.space.indexesBottom);

	// Display the game menu
	handleMenu(ui.menu, Pos{ui.space.boardCenter.x, ui.space.board.max.y + MENU_TOP_SPACING});

	// Display the pawn placement on the board
	handlePawnPlacement(ui.board, ui.space.board);

	// Handle the display of the court areas for players
	handleCourt(ui);

	// Display game messages
	handleMessages(ui);

	// Clean up the dynamically allocated memory for indexes
	for (int i = 0; i < nPoints; ++i) {
		delete[] indexes[i];
	}
	delete[] indexes;
}

void handleBoardOutline(Placement space) {
	// Iterate through each board to draw its outline
	for (int i = 0; i < N_BOARDS; ++i) {
		// Draw the borders of the current board
		drawBorders(space);
		// Move the drawing space to the start of the next board
		moveSpace(space, {boardWidth + borders + BAR_WIDTH, 0});
	}
}

void handlePieces(Placement space) {
	// Adjust the starting position to account for the border width
	space.min.x += BORDER_WIDTH;
	space.min.y += BORDER_WIDTH;

	// Iterate through each board to draw the pieces
	for (int i = 0; i < N_BOARDS; ++i) {
		// Calculate the x-coordinate for the pieces on the current board
		int boardX = space.min.x + (boardWidth + borders + BAR_WIDTH) * i;
		// Draw the pieces on the current board
		drawPieces(boardX, space.min.y);
	}
}

void handleBar(Bar bar, int selected) {
	// Initialize the count of pawns on the bar for each player
	int onBar[N_PLAYERS] = {0, 0};

	// Count the number of pawns on the bar for each player
	for (auto &pawn: bar.pawns) {
		if (pawn == nullptr)
			continue;
		if (pawn->color == PAWN_WHITE)
			onBar[0]++;
		if (pawn->color == PAWN_BLACK)
			onBar[1]++;
	}

	// Draw the bar area on each board section
	for (int i = N_BOARDS - 1; i > 0; --i)
		drawBar(OFFSET_X + (boardWidth + borders) * i + DICE_WIDTH + borders - 1,
				OFFSET_Y + BOARD_OFFSET_Y + HEADER_OFFSET + INDEX_OFFSET + TEXT_HEIGHT * 2,
				boardHeight + borders, onBar, selected);
}

void handleDices(Placement space, Pos center, int *dices) {
	// Draw the borders around the dice area
	drawBorders(space);

	// Calculate the total height needed for all dice, including borders
	int dicesHeight = DICE_HEIGHT * N_DICES + (N_DICES + 1) * BORDER_WIDTH;
	// Adjust the starting y-coordinate to center the dice vertically
	space.min.y = center.y - dicesHeight / 2;

	// Draw each die and display its value
	for (int i = 0; i < N_DICES; ++i) {
		// Set the maximum y-coordinate for the current die's box
		space.max.y = space.min.y + DICE_HEIGHT + 2 * BORDER_WIDTH - 1;
		// Draw the borders for the current die
		drawBorders(space);
		// Display the value of the die at the center of its box
		mvaddch(space.min.y + BORDER_WIDTH, space.min.x + BORDER_WIDTH + DICE_WIDTH / 2, dices[i] + 48);
		// Adjust the starting y-coordinate for the next die
		space.min.y += DICE_HEIGHT + BORDER_WIDTH;
	}
}

void handleIndexes(char **indexes, int pickedIndex, int digits, Placement posTop, Placement posBot) {
	// Reverse the order of the indexes for one half of the board
	revertTable(indexes, indexes + nPoints / 2);

	// Calculate the width needed for displaying the indexes
	int indexWidth = pieceWidth * POINTS_PER_BOARD + pieceSpacing * (POINTS_PER_BOARD - 1);
	Pos indexChange = {indexWidth + BORDER_WIDTH * 2 + BAR_WIDTH + pieceSpacing / 2 * 2, 0};

	// Set the maximum x-coordinate for the top and bottom index placements
	posTop.max.x = posTop.min.x + indexWidth;
	posBot.max.x = posBot.min.x + indexWidth;

	// Iterate through each board to display the indexes
	for (int i = 0; i < N_BOARDS; ++i) {
		// Display the indexes for the current board section
		displayCurrentIndexes(posBot, posTop, pickedIndex, (int) (digits),
							  indexes, i * POINTS_PER_BOARD, (int) (nPoints) / 2 + i * POINTS_PER_BOARD);

		// Move the top and bottom index placements to the next board section
		moveSpace(posTop, indexChange);
		moveSpace(posBot, indexChange);
	}
}

void handleMenu(Menu menu, Pos center) {
	// Initialize the placement area for the menu
	Placement menuSpace = initMenuSpace(center, menu.elements, menu.count);

	// Iterate through each menu element and display it
	for (int i = 0; i < menu.count; ++i) {
		// Determine the color for the menu element
		UiColorsId color;
		if (menu.selected == -1) {
			// Default color if no menu item is selected
			color = FOREGROUND;
		} else {
			// Highlight the selected menu item
			if (menu.elements[i].id == menu.selected) {
				color = FOREGROUND_LIGHT;
			} else {
				color = FOREGROUND_DARK;
			}
		}
		// Display the menu element with the determined color
		printColorString(color, menuSpace.min.x, menuSpace.min.y, menu.elements[i].value);

		// Adjust the x-coordinate for the next menu element
		menuSpace.min.x += (int) (len(menu.elements[i].value) - 1) + OPTION_SPACING;
	}
}

void handlePawnPlacement(Board &game, Placement space) {
	// Adjust the starting position for pawn placement
	space.min.x += BORDER_WIDTH + pieceSpacing / 2;
	space.min.y += BORDER_WIDTH;
	space.max.x = space.min.x;
	// Calculate the change in position for each pawn placement
	int change = pieceWidth + pieceSpacing;

	// Base offset for calculating pawn positions
	int base = POINTS_PER_BOARD * N_BOARDS;

	// Iterate through each board and point to place the pawns
	for (int i = 0; i < N_BOARDS; ++i) {
		for (int j = 0; j < POINTS_PER_BOARD; ++j) {
			// Calculate the offsets for top and bottom points
			int offset = i * POINTS_PER_BOARD + j;
			// Handle the drawing of pawns at the calculated points
			handlePawnDrawing(game, space, base + offset, base - offset - 1);
			// Move the space for the next pawn placement
			moveSpace(space, Pos{change, 0});
		}
		// Adjust the space for the next board section
		moveSpace(space, Pos{pieceSpacing / 2 * 2 + borders + BAR_WIDTH - pieceSpacing, 0});
	}
}

void handleMessages(UserInterface &ui) {
	// Set up the position for displaying the informational message
	Placement textPos = ui.space.board;
	textPos.min.y += (boardHeight + borders) / 2;
	UiColorsId colours = {FOREGROUND_DARK};
	char *tempMessages[1];

	// Display the informational message
	tempMessages[0] = ui.infoMess;
	uint length = len(ui.infoMess);
	drawCenteredText(textPos, 0, (int) (length), tempMessages, 1, &colours, 1);

	// Move the position for displaying the error message
	moveSpace(textPos, {(N_BOARDS - 1) * (boardWidth + BAR_WIDTH + borders), 0});
	// Display the error message
	tempMessages[0] = ui.errorMess;
	length = len(ui.errorMess);
	drawCenteredText(textPos, 0, (int) (length), tempMessages, 1, &colours, 1);
}

void handlePlayerHallText(UserInterface ui, char **text, int index) {
	// Get the player's ID and points
	int id = ui.playersScores[index].id;
	int points = ui.playersScores[index].points;

	// Construct the text elements for the player's ranking and score
	const char *data[5] = {
		numberToString(id, nDigits(id, 10)), // Player ID
		". ",
		ui.playersScores[index].name,        // Player name
		": ",
		numberToString(points, nDigits(points, 10)), // Player points
	};

	// Allocate memory for the full string and join the text elements
	text[index] = new char[MAX_NAME_LENGTH + 8];
	joinStrings0(text[index], data, 5);

	// Free the memory allocated for dynamic strings (ID and points)
	delete[] data[0];
	delete[] data[4];
}

// PRIVATE FUNCTION DECLARATIONS //

Placement initMenuSpace(Pos center, MenuElement values[], int nElements) {
	// Calculate the total length required for all menu elements including spacing
	uint realLen = OPTION_SPACING * (nElements - 1);
	for (int i = 0; i < nElements; ++i) {
		realLen += len(values[i].value) - 1;
	}

	// Define the placement area for the menu
	return Placement{
		.min={
			.x=center.x - (int) (realLen / 2),
			.y=center.y
		},
		.max {.x=center.x + (int) (realLen / 2)}
	};
}

Placement initTextSpace(Pos center, char **values, int nElements) {
	// Calculate the total length required for all text strings including spacing
	uint realLen = OPTION_SPACING * (nElements - 1);
	for (int i = 0; i < nElements; ++i) {
		realLen += len(values[i]) - 1;
	}

	// Define the placement area for the text
	return Placement{
		.min={
			.x=center.x - (int) (realLen / 2),
			.y=center.y
		},
		.max {.x=center.x + (int) (realLen / 2)}
	};
}

void generateHeader(UserInterface &ui) {
	// Array containing the elements of the header
	const char *title[3] = {ui.authorId, ui.authorName, ui.gameName};

	// Define the placement area for the header
	Placement textSpace = ui.space.gameSpace;
	textSpace.min.y = ui.space.gameSpace.min.y + HEADER_OFFSET; // Adjust the y-coordinate for the header offset

	// Draw the header elements spread across the defined text space
	drawSpreadText(textSpace, (char **) (title), 3);
}

void createPlayerNames(UserInterface ui, char **&players, int &selected) {
	const char pawn1[] = {")("};
	const char pawn2[] = {"[]"};

	// Number of elements to be joined for each player's string
	const int count = 5;
	for (int i = 0; i < N_PLAYERS; ++i) {
		// Allocate memory for each player's string
		players[i] = new char[MAX_NAME_LENGTH + 4];

		// Get the player's points
		int playerPoints = ui.board.players[i].points;
		const char *data[count] = {
			(char *) (i ? pawn2 : pawn1), // Pawn symbol for the player
			" ",
			ui.board.players[i].name,     // Player's name
			": ",
			numberToString(playerPoints, (int) (nDigits(playerPoints, 10))), // Player's points
		};

		// Join the elements to create the full player string
		joinStrings0(players[i], data, count);
		// Free the memory for the dynamic string (player's points)
		delete[] data[4];

		// Set the selected player index if this player is the current player
		if (ui.board.players[i].id == ui.board.currentPlayerId)
			selected = i;
	}
}

void generatePlayers(UserInterface &ui) {
	// Allocate memory for storing player name strings
	auto players = new char *[N_PLAYERS];
	// Variable to hold the index of the currently selected player
	int selected = -1;

	// Create strings for each player including their names and scores
	createPlayerNames(ui, players, selected);

	// Define the placement area for displaying player information
	Placement textSpace = ui.space.gameSpace;
	textSpace.min.y = ui.space.gameSpace.min.y + HEADER_OFFSET + 2;

	// Display the player names spread evenly across the defined area
	drawSpreadText(textSpace, (char **) (players), N_PLAYERS, selected);

	// Free the memory allocated for the player name strings
	for (int i = 0; i < N_PLAYERS; ++i) {
		delete[] players[i];
	}
	delete[] players;
}

void handleCourt(UserInterface ui) {
	// Define the position for the top court (white player)
	Pos courtTop = {ui.space.gameSpace.max.x, ui.space.boardCenter.y - COURT_OFFSET_Y - 5};
	// Define the position for the bottom court (black player)
	Pos courtBottom = {ui.space.gameSpace.max.x, ui.space.boardCenter.y + COURT_OFFSET_Y};

	// Turn on the bold attribute for text display
	attron(A_BOLD);
	// Display the number of pawns in the white player's court
	drawVerticalInfo(courtTop, "WHI ", ui.board.courts[0].pawnsInside, FOREGROUND_LIGHT, true);
	// Display the number of pawns in the black player's court
	drawVerticalInfo(courtBottom, "BLC ", ui.board.courts[1].pawnsInside, FOREGROUND_DARK, true);
	// Turn off the bold attribute
	attroff(A_BOLD);
}

void updateScores(UserInterface &ui) {
	// Iterate through each player's score in the user interface
	for (auto &scores: ui.playersScores)
		// Find the corresponding player in the game state
		for (auto &player: ui.board.players)
			if (scores.id == player.id) {
				// Update the score with the player's current points
				scores.points = player.points;
				break; // Break the loop once the matching player is found and updated
			}

	// Filename for saving the scores
	char filename[] = "hall.txt";
	// Save the updated scores to the file
	saveScores(filename, ui.playersScores);
}

void givePointsForWinning(UserInterface &ui) {
	// Retrieve the winning player
	Player *winner = getPlayer(ui.board, ui.board.winnerPlayerId);
	// Retrieve the opponent of the winning player
	Player *opponent = getOpponent(ui.board, winner->id);
	// Get the court of the opponent
	Court *opponentsCourt = playersCourt(ui.board, opponent->id);

	// Award points based on the opponent's position
	if (opponentsCourt->pawnsInside == 0) {
		winner->points += 2; // Award 2 points if the opponent's court is empty
	} else if (hasPawnsOnBar(ui.board.bar, opponent->id)) {
		winner->points += 3; // Award 3 points if the opponent has pawns on the bar
	} else {
		winner->points += 1; // Otherwise, award 1 point
	}

	// Update and save the scores
	updateScores(ui);
}

int generateColorsForIndexes(char **text, int count, int pickedIndex, UiColorsId *&colors) {
	int index = -1;
	// Find the index of the text element matching the picked index
	for (int i = 0; i < count; ++i) {
		if (stringToNumber(text[i]) == pickedIndex) {
			index = i;
		}
	}
	// Handle cases based on whether a picked index is found or specified
	if (pickedIndex == -1) {
		// Use a default color if no index is picked
		colors = new UiColorsId[1];
		colors[0] = FOREGROUND;
		return 1;
	} else if (index < 0) {
		// Use a dark color if no matching text is found
		colors = new UiColorsId[1];
		colors[0] = FOREGROUND_DARK;
		return 1;
	} else {
		// Generate an array of colors, highlighting the matched text
		colors = new UiColorsId[count];
		for (int i = 0; i < count; ++i) {
			colors[i] = i == index ? FOREGROUND_LIGHT : FOREGROUND_DARK;
		}
		return count;
	}
}

void displayCurrentIndexes(Placement posBot, Placement posTop, int selected, int width, char **indexes,
						   int offsetTop, int offsetBot) {
	// Generate colors for the bottom row of indexes
	UiColorsId *colors = nullptr;
	int nColours = generateColorsForIndexes(&indexes[offsetTop], POINTS_PER_BOARD, selected, colors);
	// Draw the bottom row of indexes centered in the specified area
	drawCenteredText(posBot, pieceSpacing, width,
					 &indexes[offsetTop], POINTS_PER_BOARD, colors, nColours);
	delete[] colors; // Free the memory allocated for colors

	// Repeat the process for the top row of indexes
	colors = nullptr;
	nColours = generateColorsForIndexes(&indexes[offsetBot], POINTS_PER_BOARD, selected, colors);
	drawCenteredText(posTop, pieceSpacing, width,
					 &indexes[offsetBot], POINTS_PER_BOARD, colors, nColours);
	delete[] colors; // Free the memory allocated for colors
}

void handlePawnDrawing(Board &game, Placement &space, int indexTop, int indexBot) {
	// Draw pawns at the top index
	int count = min(game.points[indexTop].pawnsInside, POINT_HEIGHT);
	if (count) {
		space.max.y = space.min.y + count;
		drawLinePawn(*game.points[indexTop].pawns[0], space);
	}

	// Draw pawns at the bottom index
	count = min(game.points[indexBot].pawnsInside, POINT_HEIGHT);
	if (count) {
		space.max.y = space.min.y + count;
		// Move the space to the correct position for the bottom index
		moveSpace(space, Pos{0, boardHeight - count});
		drawLinePawn(*game.points[indexBot].pawns[0], space);
		// Reset the space position for the next operation
		moveSpace(space, Pos{0, -boardHeight + count});
	}
}
