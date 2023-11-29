//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_PAWN_H
#define BACKGAMMONGAME_PAWN_H

typedef struct Player Player;
typedef struct Board Board;

enum moveToPoint {
	BLOCKED,
	POSSIBLE,
	CAPTURE,
	NOT_ALLOWED,
};

enum PawnColor {
	PAWN_WHITE,
	PAWN_BLACK,
};

typedef struct {
	int ownerId;
	int id;
	bool isHome;
	bool isOnBar;
	PawnColor color;
	short moveDirection;
} Pawn;

int canBeMoved(Board &game, int pointIndex, int moveBy);

moveToPoint canMoveTo(Board &game, int fromIndex, int toIndex);

bool enumToBool(moveToPoint value);

/// Handle Serialization of Pawn object
void serialisePawn(Pawn pawn, uint8_t *buffer, size_t &offset);

Pawn deserializePawn(const uint8_t *buffer, size_t &index);

/// Handle POINTER serialization to a Pawn object
void serialisePawnPointer(Pawn *pawn, uint8_t* buffer, size_t &offset);

Pawn *deserializePawnPointer(Board &board, const uint8_t *buffer, size_t &offset);

#endif //BACKGAMMONGAME_PAWN_H
