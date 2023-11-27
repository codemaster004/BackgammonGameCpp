//
// Created by Filip Dabkowski on 12/11/2023.
//
// TODO: Create a structure for keeping moves already mad in the game?
// Use enum for color pallet

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

/// Clear the Board Structure for a New Game
void setClearBoard(Board &gameBoard) {

	gameBoard.currentPlayerId = NULL;
	for (auto &player: gameBoard.players) {
		player = Player{};
	}

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

	// set both Dices to 0
	for (auto &dice: gameBoard.dices) {
		dice = 0;
	}
}

/// Place Pawns on the board with default configuration
void placePawns(Board &gameBoard) {
	int pawnIndex = 0;
	for (auto pos: startingPos) {
		gameBoard.points[pos->x].pawnsInside = pos->y;
		for (int j = 0; j < pos->y; ++j) {
			gameBoard.points[pos->x].pawns[j] = &gameBoard.pawns[pawnIndex++];
		}
	}

	pawnIndex = 0;
	int endIndex = nPoints - 1;
	for (auto pos: startingPos) {
		gameBoard.points[endIndex - pos->x].pawnsInside = pos->y;
		for (int j = 0; j < pos->y; ++j) {
			gameBoard.points[endIndex - pos->x].pawns[j] = &gameBoard.pawns[PAWNS_PER_PLAYER + pawnIndex++];
		}
	}
}

/// Serialize Board object to a byte array
void serialiseBoard(Board &board, uint8_t *buffer, size_t &offset) {
	for (auto &player: board.players) {
		serialisePlayer(player, buffer, offset);
	}
	for (Pawn &pawn : board.pawns) {
		serialisePawn(pawn, buffer, offset);
	}
	serializeInt(board.currentPlayerId, buffer, offset);
	for (auto &point: board.points) {
		serialisePoint(point, buffer, offset);
	}
	for (auto &court: board.courts) {
		serialiseCourt(court, buffer, offset);
	}
	serialiseBar(board.bar, buffer, offset);
	for (auto &dice: board.dices) {
		serializeInt(dice, buffer, offset);
	}
}

/// Deserialize Board object from byte array to object
Board deserializeBoard(const uint8_t *buffer, size_t &offset) {
	Board board;
	for (auto &player : board.players) {
		player = deserializePlayer(buffer, offset);
	}
	for (Pawn &pawn : board.pawns) {
		pawn = deserializePawn(buffer, offset);
	}
	board.currentPlayerId = deserializeInt(buffer, offset);
	for (auto &point:board.points) {
		point = deserializePoint(board, buffer, offset);
	}
	for (auto &court : board.courts) {
		court = deserializeCourt(board, buffer, offset);
	}
	board.bar = deserializeBar(board, buffer, offset);
	for (auto &dice : board.dices) {
		dice = deserializeInt(buffer, offset);
	}
	return board;
}
