//
// Created by Filip Dabkowski on 12/11/2023.
//
// TODO: Create a structure for board (pieces(triangle thingins), "?board", bar, court (home), dies?, pawns...)
// TODO: Create a structure for keeping moves already mad in the game?
// TODO: Keep Basic Student info in struct?
// Use enum for color pallet

#include "Pawn.h"
#include "Board.h"


/*
 *   12 13 14 15 16 17   18 19 20 21 22 23
 * +-==-==-==-==-==-==-+-==-==-==-==-==-==-+
 * | 5W          3B    | 5B             2W | BLACK
 * |                   |                   |
 * | 5B          3W    | 5W             2B | WHITE
 * +-==-==-==-==-==-==-+-==-==-==-==-==-==-+
 *   11 10 09 08 07 06   05 04 03 02 01 00
 */

void setClearBoard(Board *gameBoard) {
	gameBoard->bar = Bar{0};

	gameBoard->currentPlayer = 0;
	for (auto &player: gameBoard->players) {
		player = 0;
	}

	// set Points to keep 0 Pawns
	for (auto &point: gameBoard->points) {
		point = Point{0};
	}

	// set Courts to hold 0 Pawns
	for (auto &court: gameBoard->courts) {
		court = Court{0};
	}
}

// TODO: rename
void setupGame(Board *gameBoard, Pawn player1[], Pawn player2[]) {
	int pawnIndex = 0;
	for (auto pos: startingPos) {
		gameBoard->points[pos->x].pawnsInside = pos->y;
		for (int j = 0; j < pos->y; ++j) {
			gameBoard->points[pos->x].pawns[j] = &player1[pawnIndex++];
		}
	}

	pawnIndex = 0;
	int endIndex = nPoints - 1;
	for (auto pos: startingPos) {
		gameBoard->points[endIndex - pos->x].pawnsInside = pos->y;
		for (int j = 0; j < pos->y; ++j) {
			gameBoard->points[endIndex - pos->x].pawns[j] = &player2[pawnIndex++];
		}
	}
}


