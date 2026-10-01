#pragma once
#include <stdint.h>
#include <stddef.h>

typedef struct {
	uint64_t *words;
	uint64_t inlineWord;
	size_t bitCount;
	size_t wordCount;
} BitSet;

bool bitsetInit(BitSet *bs, size_t bitCount);
bool bitsetSetUInt64(BitSet *bs, uint64_t value);

bool bitsetGet(const BitSet *bs, size_t index);
void bitsetSet(BitSet *bs, size_t index);
/*
void bitsetSetAll(BitSet *bs);
bool bitsetResize(BitSet *bs, size_t bitcount);
void bitsetClear(BitSet *bs);
void bitsetClearBit(BitSet *bs, size_t index);
void bitsetToggle(BitSet *bs, size_t index);
*/

void bitsetDestroy(BitSet *bs);

