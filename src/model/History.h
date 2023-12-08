//
// Created by Filip Dabkowski on 05/12/2023.
//

#ifndef BACKGAMMONGAME_HISTORY_H
#define BACKGAMMONGAME_HISTORY_H

#include "cstdint"

enum MoveDirection {
	POINT_TO_POINT=1,
	POINT_TO_BAR,
	BAR_TO_POINT,
	POINT_TO_COURT,
	COURT_TO_POINT,
};

typedef struct MoveMade {
	MoveDirection type;
	int from;
	int to;
	int moveOrder;
	int pawnId;
	MoveMade *prevMove;
} MadeMove;

void addAfter(MoveMade data, MoveMade *lastMove);

void removeAfter(MoveMade *latestMove);

MoveMade *getFist(MoveMade head);

void serializeHistory(MoveMade &head, uint8_t *buffer, size_t &offset);

void deserializeHistory(const uint8_t *buffer, size_t limit, MoveMade &history);

void loadHistoryFromFile(char filename[], MoveMade &head);

void saveHistoryToFile(char filename[], MoveMade &head);

#endif //BACKGAMMONGAME_HISTORY_H
