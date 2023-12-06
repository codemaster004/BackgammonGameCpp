//
// Created by Filip Dabkowski on 06/12/2023.
//

#ifndef BACKGAMMONGAME_BYTECONTAINER_H
#define BACKGAMMONGAME_BYTECONTAINER_H

#include <cstdint>
#include <cstddef>
#include <algorithm>

typedef struct ByteContainer {
	uint8_t **data;
	size_t nPartitions;
	size_t partitionSize;
	size_t dataCount;
} ByteContainer;

void expandFirstDimension();

void initByteContainer(ByteContainer &self, int size=1, int partitionSize=64);

void destroyByteContainer(ByteContainer &self);

void addElement(uint8_t byte);

uint8_t getByte(size_t index);

#endif //BACKGAMMONGAME_BYTECONTAINER_H
