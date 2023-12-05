//
// Created by Filip Dabkowski on 05/12/2023.
//

#include "History.h"

void addAfter(MoveMade data, MoveMade *lastMove) {
	auto move = new MoveMade [1];
	*move = data;
	move->prevMove = lastMove->prevMove;
	lastMove->prevMove = move;
	lastMove->moveOrder++;
}

void removeAfter(MoveMade *latestMove) {
	MoveMade *temp = latestMove->prevMove->prevMove;
	delete[] latestMove->prevMove;
	latestMove->prevMove = temp;
	latestMove->moveOrder--;
}