//
// Created by Filip Dabkowski on 06/12/2023.
//

#include "ByteContainer.h"

// PRIVATE FUNCTION HEADERS //

// PUBLIC FUNCTION DECLARATIONS //

// PRIVATE FUNCTION DECLARATIONS //

void initByteContainer(ByteContainer &self, int size, int partitionSize)  {
	self.partitionSize = partitionSize;
	self.nPartitions = size / partitionSize + 1;
	self.dataCount = 0;

	self.data = new uint8_t *[self.nPartitions];
	for (size_t i = 0; i < self.nPartitions; ++i) {
		self.data[i] = new uint8_t[partitionSize];
	}
}

void destroyByteContainer(ByteContainer &self) {
	for (size_t i = 0; i < self.nPartitions; ++i) {
		delete[] self.data[i];
	}
	delete[] self.data;
}

void expandFirstDimension(ByteContainer &self) {
	auto **newData = new uint8_t *[self.nPartitions + 1];

	std::copy(self.data, self.data + self.nPartitions, newData);

	newData[self.nPartitions] = new uint8_t[self.partitionSize];

	delete[] self.data;
	self.data = newData;

	self.nPartitions++;
}

void addElement(ByteContainer &self, uint8_t byte) {
	size_t rowIndex = self.dataCount / self.partitionSize;
	size_t columnIndex = self.dataCount % self.partitionSize;

	if (rowIndex >= self.nPartitions) {
		// Expand the first dimension if needed
		expandFirstDimension(self);
	}

	self.data[rowIndex][columnIndex] = byte;
	self.dataCount++;
}

uint8_t getByte(ByteContainer &self, size_t index) {
	size_t rowIndex = index / self.partitionSize;
	size_t columnIndex = index % self.partitionSize;

	return self.data[rowIndex][columnIndex];
}

void flatten(ByteContainer &self, char *buffer) {
	for (int i = 0; i < self.dataCount; ++i) {
		buffer[i] = (char)(getByte(self, i));
	}
}
