//
// Created by Filip Dabkowski on 12/11/2023.
//
// TODO: Create a structure for pawn (color?, *player?, location?, isHome, isOnBar, canMoveDirection?, canBeRemoved
// Maybe keep pawn color as enum?

enum PawnColor {
    WHITE,
    RED
};

typedef struct {
    PawnColor color;
    int *owner;
    bool isHome;
    bool isOnBar;
    const short moveDirection;
} Pawn;
