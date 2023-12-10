//
// Created by Filip Dabkowski on 25/11/2023.
//

#ifndef BACKGAMMONGAME_STORAGE_H
#define BACKGAMMONGAME_STORAGE_H

#include "../configs/GameConfigs.h"
#include "Pawn.h"

/**
 * @brief Represents a point on the game board.
 *
 * Each point can hold a certain number of pawns. This structure tracks the number of pawns
 * currently at this point and stores references to these pawns.
 */
typedef struct Point {
	int pawnsInside;               ///< Number of pawns currently at this point.
	Pawn *pawns[PAWNS_PER_POINT];  ///< Array of pointers to pawns at this point.
} Point;

/**
 * @brief Represents the bar area on the game board.
 *
 * The bar is a special area where pawns are placed after being hit. This structure tracks
 * the number of pawns on the bar and stores references to these pawns.
 */
typedef struct Bar {
	int pawnsInside;          ///< Number of pawns currently on the bar.
	Pawn *pawns[totalPawns];  ///< Array of pointers to pawns on the bar.
} Bar;

/**
 * @brief Represents a player's court in the game.
 *
 * The court is an area where a player collects their pawns that have escaped the Board. This
 * structure tracks the number of pawns in the court and stores references to these pawns,
 * as well as a reference to the court's owner.
 */
typedef struct Court {
	int pawnsInside;               ///< Number of pawns in the court.
	Pawn *pawns[PAWNS_PER_PLAYER]; ///< Array of pointers to pawns in the court.
	Player *owner;                 ///< Pointer to the player who owns this court.
} Court;


/**
 * @brief Checks if a move involves removing a pawn from the bar.
 *
 * @param game Reference to the Board structure representing the game state.
 * @param move The Move structure containing details of the move to be checked.
 * @return bool True if the move involves removing a pawn from the bar, false otherwise.
 */
bool removingFromBar(Board &game, Move move);

/**
 * @brief Finds the index of a pawn belonging to a specific player on the bar.
 *
 * @param game Reference to the Board structure representing the game state.
 * @param playerId The ID of the player whose pawn is being searched for.
 * @return int The index of the pawn on the bar, or -1 if not found.
 */
int pawnIndexOnBar(Board &game, int playerId);

/**
 * @brief Determines if a point index is within a player's home board.
 *
 * @param index The index of the point to check.
 * @param listSize The total number of points on the board.
 * @param direction The direction of the player's pawn movement (1 or -1).
 * @return bool True if the index is within the player's home board, false otherwise.
 */
bool isHomeBoard(int index, int listSize, int direction);

/**
 * @brief Counts the number of pawns a player has on their home board.
 *
 * @param game Reference to the Board structure representing the game state.
 * @return int The number of pawns the current player has on their home board.
 */
int pawnsOnHomeBoard(Board &game);

/**
 * @brief Retrieves the court belonging to the pawn's owner.
 *
 * @param game Reference to the Board structure representing the game state.
 * @param pawn Pointer to the Pawn structure whose owner's court is to be found.
 * @return Court* Pointer to the Court structure belonging to the pawn's owner, or nullptr if not found.
 */
Court *pawnsCourt(Board &game, Pawn *pawn);

/**
 * @brief Retrieves the court belonging to a specified player.
 *
 * @param game Reference to the Board structure representing the game state.
 * @param playerId The ID of the player whose court is to be found.
 * @return Court* Pointer to the Court structure belonging to the specified player, or nullptr if not found.
 */
Court *playersCourt(Board &game, int playerId);

/**
 * @brief Moves a pawn from the bar to a point on the board.
 *
 * @param game Reference to the Board structure representing the game state.
 * @param history Reference to the MoveMade structure for recording the move.
 * @param fromIndex The index of the pawn on the bar.
 * @param toIndex The index of the destination point on the board.
 * @param order The order of the move, used for recording in history.
 */
void moveBarToPoint(Board &game, MoveMade &history, int fromIndex, int toIndex, int order=0);

/**
 * @brief Moves a pawn from one point to another on the board.
 *
 * @param game Reference to the Board structure representing the game state.
 * @param history Reference to the MoveMade structure for recording the move.
 * @param fromIndex The index of the starting point.
 * @param toIndex The index of the destination point.
 * @param order The order of the move, used for recording in history.
 */
void movePointToPoint(Board &game, MoveMade &history, int fromIndex, int toIndex, int order=0);

/**
 * @brief Moves a pawn from a point on the board to its owner's court.
 *
 * @param game Reference to the Board structure representing the game state.
 * @param history Reference to the MoveMade structure for recording the move.
 * @param fromIndex The index of the starting point.
 * @param order The order of the move, used for recording in history.
 */
void movePointToCourt(Board &game, MoveMade &history, int fromIndex, int order=0);

/**
 * @brief Moves one or more pawns from a point on the board to the bar.
 *
 * @param game Reference to the Board structure representing the game state.
 * @param history Reference to the MoveMade structure for recording the move.
 * @param fromIndex The index of the starting point.
 * @param order The order of the move, used for recording in history.
 */
void movePointToBar(Board &game, MoveMade &history, int fromIndex, int order=0);

/**
 * @brief Moves a pawn from a player's court to a point on the board.
 *
 * @param game Reference to the Board structure representing the game state.
 * @param history Reference to the MoveMade structure for recording the move.
 * @param toIndex The index of the destination point on the board.
 * @param pawnId The ID of the pawn being moved.
 * @param order The order of the move, used for recording in history.
 */
void moveCourtToPoint(Board &game, MoveMade &history, int toIndex, int pawnId, int order);

/**
 * @brief Checks if the current player has pawns on a specific point.
 *
 * @param game Reference to the Board structure representing the game state.
 * @param index The index of the point to check.
 * @return int Non-zero if the current player has pawns on the point, zero otherwise.
 */
int hasPawnsOnPoint(Board &game, int index);

/**
 * @brief Determines if a pawn can potentially start the escaping process.
 *
 * @param game Reference to the Board structure representing the game state.
 * @param index The index of the point where the pawn is located.
 * @return bool True if the pawn can start escaping, false otherwise.
 */
bool canBeEscaping(Board &game, int index);

/**
 * @brief Serializes a Point structure into a buffer.
 *
 * @param point The Point structure to serialize.
 * @param buffer Pointer to the buffer where the serialized data will be stored.
 * @param offset Reference to the size_t variable indicating the current offset in the buffer.
 */
void serialisePoint(Point point, uint8_t *buffer, size_t &offset);

/**
 * @brief Deserializes a Point structure from a buffer.
 *
 * @param board Reference to the Board containing the Pawn structures.
 * @param buffer Pointer to the buffer containing the serialized data.
 * @param index Reference to the size_t variable indicating the current index in the buffer.
 * @return Point The deserialized Point structure.
 */
Point deserializePoint(Board &board, const uint8_t *buffer, size_t &index);

/**
 * @brief Serializes a Court structure into a buffer.
 *
 * @param court The Court structure to serialize.
 * @param buffer Pointer to the buffer where the serialized data will be stored.
 * @param offset Reference to the size_t variable indicating the current offset in the buffer.
 */
void serialiseCourt(Court court, uint8_t *buffer, size_t &offset);

/**
 * @brief Deserializes a Court structure from a buffer.
 *
 * @param board Reference to the Board containing the Pawn and Player structures.
 * @param buffer Pointer to the buffer containing the serialized data.
 * @param index Reference to the size_t variable indicating the current index in the buffer.
 * @return Court The deserialized Court structure.
 */
Court deserializeCourt(Board &board, const uint8_t *buffer, size_t &index);

/**
 * @brief Serializes a Bar structure into a buffer.
 *
 * @param bar The Bar structure to serialize.
 * @param buffer Pointer to the buffer where the serialized data will be stored.
 * @param offset Reference to the size_t variable indicating the current offset in the buffer.
 */
void serialiseBar(Bar bar, uint8_t *buffer, size_t &offset);

/**
 * @brief Deserializes a Bar structure from a buffer.
 *
 * @param board Reference to the Board containing the Pawn structures.
 * @param buffer Pointer to the buffer containing the serialized data.
 * @param index Reference to the size_t variable indicating the current index in the buffer.
 * @return Bar The deserialized Bar structure.
 */
Bar deserializeBar(Board &board, const uint8_t *buffer, size_t &index);

#endif //BACKGAMMONGAME_STORAGE_H
