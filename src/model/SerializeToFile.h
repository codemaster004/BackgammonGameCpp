//
// Created by Filip Dabkowski on 24/11/2023.
//

#ifndef BACKGAMMONGAME_SERIALIZETOFILE_H
#define BACKGAMMONGAME_SERIALIZETOFILE_H

#include "Board.h"
#include "../viewModel/UserInterface.h"


/**
 * @brief Serializes an integer value into a buffer.
 *
 * @param value The integer value to be serialized.
 * @param buffer Pointer to the buffer where the serialized data will be stored.
 * @param offset Reference to the size_t variable indicating the current offset in the buffer.
 */
void serializeInt(int value, uint8_t *buffer, size_t &offset);

/**
 * @brief Deserializes an integer value from a buffer.
 *
 * @param buffer Pointer to the buffer containing the serialized data.
 * @param index Reference to the size_t variable indicating the current index in the buffer.
 * @return int The deserialized integer value.
 */
int deserializeInt(const uint8_t *buffer, size_t &index);

/**
 * @brief Saves the game state to a file.
 *
 * @param filename Name of the file where the game state will be saved.
 * @param game Reference to the Board structure representing the game state to be saved.
 */
void saveToFile(char filename[], Board &game);

/**
 * @brief Loads the game state from a file.
 *
 * @param filename Name of the file from which the game state will be loaded.
 * @param game Reference to the Board structure where the loaded game state will be stored.
 */
void loadFromFile(char filename[], Board &game);

/**
 * @brief Saves the players' scores to a file.
 *
 * @param filename Name of the file where the scores will be saved.
 * @param scores Array of ScorePlayer structures representing the players' scores.
 */
void saveScores(char filename[], ScorePlayer scores[]);

/**
 * @brief Loads players' scores from a file.
 *
 * @param filename Name of the file from which the scores will be loaded.
 * @param ui Reference to the UserInterface structure where the loaded scores will be stored.
 */
void loadScores(char filename[], UserInterface &ui);

#endif //BACKGAMMONGAME_SERIALIZETOFILE_H
