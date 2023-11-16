//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_BOARD_H
#define BACKGAMMONGAME_BOARD_H

#include "string"

#include "../DefineConsts.h"
#include "Pawn.h"


enum PointColor {
    LIGHT,
    DARK
};

typedef struct {
    Pawn pawns[PAWNS_PER_POINT];
    PointColor color;
} Point;

typedef struct {
    Pawn *pawns[totalPawns];
} Bar;

typedef struct {
    Pawn *pawns[PAWNS_PER_PLAYER];
    // TODO: Use player structure here
    int *owner;
} Court;

typedef struct {
    Point points[nPoints];
    Bar bar;
    Court courts[N_PLAYERS];
} Board;

typedef struct {
    std::string gameName;
    std::string authorName;
    int authorId;
    // TODO: implement player structure
    int players[N_PLAYERS];
    int currentPlayer;
    Board board;
    // TODO: create/implement structure for keeping currentPlayersScores
    int currentScores[PLAYERS_PER_GAME];

} UserInterface;

#endif //BACKGAMMONGAME_BOARD_H
