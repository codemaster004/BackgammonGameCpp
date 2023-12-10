//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_PAWN_H
#define BACKGAMMONGAME_PAWN_H

#include "cstdint"

#include "History.h"

typedef struct Player Player;
typedef struct Board Board;
typedef struct Bar Bar;
typedef struct Point Point;

/**
 * @brief Represents a move in the game.
 *
 * This structure holds details about a player's move, including the starting point,
 * the dice value used, and the remaining moves in the turn.
 */
typedef struct Move {
	int from;        ///< Starting point of the move.
	int by;          ///< Dice value used for the move.
	int movesLeft;   ///< Number of remaining moves in the turn.
} Move;

/**
 * @brief Enumerates possible types of moves in the game.
 */
enum MoveType {
	BLOCKED,          ///< Move is blocked by opponent's pawns.
	POSSIBLE,         ///< Move is possible.
	CAPTURE,          ///< Move results in a capture.
	NOT_ALLOWED,      ///< Move is not allowed.
	ESCAPE_BOARD,     ///< Move leads to escaping the board.
	CAPTURE_POSSIBLE, ///< Capture is possible.
	ESCAPE_POSSIBLE,  ///< Escape from board is possible.
};

/**
 * @brief Enumerates the colors of pawns in the game.
 */
enum PawnColor {
	PAWN_WHITE, ///< White pawn.
	PAWN_BLACK, ///< Black pawn.
};

/**
 * @brief Enumerates the status of a move after being played.
 *
 * Private information for the checking function
 */
enum MoveStatus {
	MOVE_SUCCESSFUL, ///< Move was successful.
	PAWNS_ON_BAR,    ///< Player has pawns on the bar that must be moved first.
	MOVE_FAILED,     ///< Move failed.
	MOVE_TO_COURT,   ///< Move towards the player's court.
	FORCE_CAPTURE,   ///< Player is forced to capture an opponent's pawn.
	FORCE_ESCAPE,    ///< Player is forced to escape a pawn from the board.
};

/**
 * @brief Represents a pawn in the game.
 *
 * This structure contains details about a pawn, including its owner, ID, home status,
 * color, and the direction it moves on the board.
 */
typedef struct Pawn {
	int ownerId;          ///< ID of the pawn's owner.
	int id;               ///< Unique ID of the pawn.
	bool isHome;          ///< Indicates if the pawn is in its home area.
	PawnColor color;      ///< Color of the pawn.
	short moveDirection;  ///< Direction the pawn moves on the board.
} Pawn;


/**
 * @brief Retrieves a pointer to a Pawn structure based on its ID.
 *
 * @param game Reference to the Board structure containing the pawns.
 * @param id The ID of the pawn to be retrieved.
 * @return Pawn* Pointer to the Pawn structure with the specified ID, or nullptr if not found.
 */
Pawn *getPawn(Board &game, int id);

/**
 * @brief Converts a MoveStatus to a boolean value.
 *
 * @param type The MoveStatus to be evaluated.
 * @return bool True if the status indicates a successful move, false otherwise.
 */
bool statusToBool(MoveStatus type);

/**
 * @brief Handles the movement of a pawn based on the current move.
 *
 * @param game Reference to the Board structure representing the game state.
 * @param move The Move structure containing details of the move to be made.
 * @param history Reference to the MoveMade structure for recording the move.
 * @return MoveStatus The status of the move after execution.
 */
MoveStatus handlePawnMovement(Board &game, Move move, MoveMade &history);

/**
 * @brief Counts the number of pawns a player has on the bar.
 *
 * @param bar Reference to the Bar structure containing the pawns.
 * @param playerId The ID of the player whose pawns are being counted.
 * @return int The number of pawns the specified player has on the bar.
 */
int hasPawnsOnBar(Bar &bar, int playerId);

/**
 * @brief Reverses the moves made up to a certain point in the game.
 *
 * @param game Reference to the Board structure representing the game state.
 * @param head Reference to the MoveMade structure that serves as the head of the moves list.
 */
void reverseMoves(Board &game, MoveMade &head);

/**
 * @brief Updates the 'isHome' status of the last pawn in a point.
 *
 * @param toPoint Pointer to the Point structure where the pawn is placed.
 * @param pointIndex The index of the point on the board.
 */
void checkNewPoint(Point *toPoint, int pointIndex);

/**
 * @brief Serializes a Pawn structure into a buffer.
 *
 * Copies the Pawn structure into a buffer using memcpy, effectively serializing it.
 * The offset in the buffer is incremented by the size of Pawn.
 *
 * @param pawn The Pawn structure to be serialized.
 * @param buffer Pointer to the buffer where the serialized data will be stored.
 * @param offset Reference to the size_t variable indicating the current offset in the buffer.
 */
void serialisePawn(Pawn pawn, uint8_t *buffer, size_t &offset);

/**
 * @brief Deserializes a Pawn structure from a buffer.
 *
 * Reconstructs a Pawn structure by copying data from a buffer using memcpy.
 * The index in the buffer is incremented by the size of Pawn.
 *
 * @param buffer Pointer to the buffer containing the serialized data.
 * @param index Reference to the size_t variable indicating the current index in the buffer.
 * @return Pawn The deserialized Pawn structure.
 */
Pawn deserializePawn(const uint8_t *buffer, size_t &index);

/**
 * @brief Serializes a pointer to a Pawn structure into a buffer.
 *
 * Serializes the ID of the Pawn if the pointer is not null; otherwise, serializes -1.
 * This approach is used to handle nullable Pawn pointers.
 *
 * @param pawn Pointer to the Pawn structure to be serialized.
 * @param buffer Pointer to the buffer where the serialized data will be stored.
 * @param offset Reference to the size_t variable indicating the current offset in the buffer.
 */
void serialisePawnPointer(Pawn *pawn, uint8_t* buffer, size_t &offset);

/**
 * @brief Deserializes a pointer to a Pawn structure from a buffer.
 *
 * Reconstructs a pointer to a Pawn structure by matching the serialized ID
 * with a Pawn's ID in the board. Returns nullptr if the serialized ID is -1.
 *
 * @param board Reference to the Board containing the Pawn structures.
 * @param buffer Pointer to the buffer containing the serialized data.
 * @param offset Reference to the size_t variable indicating the current offset in the buffer.
 * @return Pawn* Pointer to the deserialized Pawn structure, or nullptr if not found.
 */
Pawn *deserializePawnPointer(Board &board, const uint8_t *buffer, size_t &offset);

#endif //BACKGAMMONGAME_PAWN_H
