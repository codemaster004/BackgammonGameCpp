//
// Created by Filip Dabkowski on 12/11/2023.
//

#include "cstring"
#include "cstdint"

#include "Board.h"
#include "SerializeToFile.h"


/*
 *   12 13 14 15 16 17   18 19 20 21 22 23
 * +-==-==-==-==-==-==-+-==-==-==-==-==-==-+
 * | 5W          3B    | 5B             2W | BLACK
 * |                   |                   |
 * | 5B          3W    | 5W             2B | WHITE
 * +-==-==-==-==-==-==-+-==-==-==-==-==-==-+
 *   11 10 09 08 07 06   05 04 03 02 01 00
 */

void clearPlayers(Board &game) {
	game.currentPlayerId = NULL;
	for (auto &player: game.players) {
		player = {.id=-1};
	}
}

void setClearBoard(Board &gameBoard) {
	// set Bar to keep 0 Pawns
	gameBoard.bar = Bar{0};

	// set Points to keep 0 Pawns
	for (auto &point: gameBoard.points) {
		point = Point{0};
	}

	// set Courts to hold 0 Pawns
	for (auto &court: gameBoard.courts) {
		court = Court{0};
	}

	// Clear the dice values
	clearDices(gameBoard.dices);

	// Reset the winner player ID
	gameBoard.winnerPlayerId = -1;
}

void clearDices(int *dices) {
	for (int i = 0; i < N_DICES; ++i) {
		dices[i] = 0;
	}
}

int calcDiceLimit(Board &game) {
	int limit = 1; // Start with a base number of Options 1

	// Increment the limit for each dice that has a value
	for (int dice: game.dices)
		if (dice > 0)
			limit += 1;

	// If the limit equals the number of dices plus one (starter value),
	// increase it further for Option to "Pick All"
	if (limit == N_DICES + 1)
		limit += 1;

	// Return the limit, ensuring it does not exceed 9
	return min(9, limit);
}

void placePawns(Board &gameBoard) {
	const Pos startingPos[][2] = {{0, 2}, {11, 5},{16, 3}, {18, 5}};
	int i = 0;
	int end = nPoints - 1;

	// Iterate over each starting position to place pawns
	for (auto pos: startingPos) {
		// Set the number of pawns for the starting point
		gameBoard.points[pos->x].pawnsInside = pos->y;
		// Mirror the placement on the opposite side
		gameBoard.points[end - pos->x].pawnsInside = pos->y;

		// Place pawns at each point
		for (int j = 0; j < pos->y; ++j) {
			int mid = PAWNS_PER_PLAYER + i;
			gameBoard.pawns[mid].isHome = isHomeBoard(end - pos->x, nPoints, gameBoard.pawns[mid].moveDirection);
			gameBoard.points[end - pos->x].pawns[j] = &gameBoard.pawns[mid];

			gameBoard.pawns[i].isHome = isHomeBoard(pos->x, nPoints, gameBoard.pawns[i].moveDirection);
			gameBoard.points[pos->x].pawns[j] = &gameBoard.pawns[i++];
		}
	}
}

void serialiseBoard(Board &board, uint8_t *buffer, size_t &offset) {
	// Serialize each player
	for (auto &player: board.players) {
		serialisePlayer(player, buffer, offset);
	}
	// Serialize each pawn
	for (Pawn &pawn : board.pawns) {
		serialisePawn(pawn, buffer, offset);
	}
	// Serialize the current player ID
	serializeInt(board.currentPlayerId, buffer, offset);
	// Serialize each point
	for (auto &point: board.points) {
		serialisePoint(point, buffer, offset);
	}
	// Serialize each court
	for (auto &court: board.courts) {
		serialiseCourt(court, buffer, offset);
	}
	// Serialize the bar
	serialiseBar(board.bar, buffer, offset);
	// Serialize each dice
	for (auto &dice: board.dices) {
		serializeInt(dice, buffer, offset);
	}
	// Serialize the winner player ID
	serializeInt(board.winnerPlayerId, buffer, offset);
}


void deserializeBoard(const uint8_t *buffer, size_t &offset, Board &board) {
	// Deserialize each player
	for (auto &player : board.players) {
		player = deserializePlayer(buffer, offset);
	}
	// Deserialize each pawn
	for (Pawn &pawn : board.pawns) {
		pawn = deserializePawn(buffer, offset);
	}
	// Deserialize the current player ID
	board.currentPlayerId = deserializeInt(buffer, offset);
	// Deserialize each point
	for (auto &point:board.points) {
		point = deserializePoint(board, buffer, offset);
	}
	// Deserialize each court
	for (auto &court : board.courts) {
		court = deserializeCourt(board, buffer, offset);
	}
	// Deserialize the bar
	board.bar = deserializeBar(board, buffer, offset);
	// Deserialize each dice
	for (auto &dice : board.dices) {
		dice = deserializeInt(buffer, offset);
	}
	// Deserialize the winner player ID
	board.winnerPlayerId = deserializeInt(buffer, offset);
}
