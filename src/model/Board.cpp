//
// Created by Filip Dabkowski on 12/11/2023.
//
// TODO: Create a structure for keeping moves already mad in the game?
// Use enum for color pallet

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

	gameBoard->currentPlayerId = 0;
	for (auto &player: gameBoard->players) {
		player = Player{};
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
void placePawns(Board *gameBoard) {
	int pawnIndex = 0;
	for (auto pos: startingPos) {
		gameBoard->points[pos->x].pawnsInside = pos->y;
		for (int j = 0; j < pos->y; ++j) {
			gameBoard->points[pos->x].pawnsId[j] = gameBoard->pawnGroups[0].pawns[pawnIndex++].id;
		}
	}

	pawnIndex = 0;
	int endIndex = nPoints - 1;
	for (auto pos: startingPos) {
		gameBoard->points[endIndex - pos->x].pawnsInside = pos->y;
		for (int j = 0; j < pos->y; ++j) {
			gameBoard->points[endIndex - pos->x].pawnsId[j] = gameBoard->pawnGroups[1].pawns[pawnIndex++].id;
		}
	}
}


