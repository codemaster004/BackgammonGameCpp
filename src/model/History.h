//
// Created by Filip Dabkowski on 05/12/2023.
//

#ifndef BACKGAMMONGAME_HISTORY_H
#define BACKGAMMONGAME_HISTORY_H

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
	MoveMade *prevMove;
} MadeMove;

void addAfter(MoveMade data, MoveMade *lastMove);

void removeAfter(MoveMade *latestMove);

MoveMade *getFist(MoveMade head);



#endif //BACKGAMMONGAME_HISTORY_H
