//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_BOARD_H
#define BACKGAMMONGAME_BOARD_H

#include "string"

#include "Pawn.h"

#define N_BOARDS 4
#define POINTS_PER_BOARD 6
#define PAWNS_PER_POINT 5
#define PAWNS_PER_PLAYER 15
#define N_PLAYERS 2
#define PLAYERS_PER_GAME 2

typedef unsigned int uint;

const uint nPoints = N_BOARDS * POINTS_PER_BOARD;
const uint totalPawns = N_PLAYERS * PAWNS_PER_PLAYER;

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
