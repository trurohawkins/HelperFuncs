#include "bitset.h"
#include <stdbool.h>
#include <stdlib.h>

#define BITSET_WORD_BITS 64

bool bitsetInit(BitSet *bs, size_t bitCount) {
	if (bs == NULL) {
		return false;
	}
	bs->words = NULL;
	bs->bitCount = bitCount;
	bs->wordCount = 0;
	bs->inlineWord = 0;

	if (bitCount == 0) {
		return true;
	}

	bs->wordCount = (bitCount + BITSET_WORD_BITS - 1) / BITSET_WORD_BITS;
	if (bs->wordCount == 1) {
		bs->words = &bs->inlineWord;
		return true;
	} else {
		bs->words = calloc(bs->wordCount, sizeof(uint64_t));
		//calloc fail
		if (bs->words == NULL) {
			bs->bitCount = 0;
			bs->wordCount = 0;
			return false;
		} else {
			return true;
		}
	}
}

bool bitsetSetUInt64(BitSet *bs, uint64_t value) {
	if (bs == NULL || bs->wordCount == 0) {
		return false;
	}
	bs->words[0] = value;
	return true;
}

bool bitsetGet(const BitSet *bs, size_t index) {
	if (bs == NULL || index >= bs->bitCount) {
		return false;
	}
	size_t word = index / BITSET_WORD_BITS;
	size_t bit = index % BITSET_WORD_BITS;

	return (bs->words[word] & (UINT64_C(1) << bit)) != 0;
}

void bitsetSet(BitSet *bs, size_t index) {
	if (bs == NULL || index >= bs->bitCount) {
		return;
	}
	size_t word = index / BITSET_WORD_BITS;
	size_t bit = index % BITSET_WORD_BITS;

	bs->words[word] |= UINT64_C(1) << bit;
}

void bitsetDestroy(BitSet *bs) {
	if (bs) {
		if (bs->words != &bs->inlineWord) {
			free(bs->words);
		}
		bs->words = NULL;
		bs->bitCount = 0;
		bs->wordCount = 0;
		bs->inlineWord = 0;
	}
}

