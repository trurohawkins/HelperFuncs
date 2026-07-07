#ifndef ATOMIC_QUEUE
#define ATOMIC_QUEUE

#include <stdatomic.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

typedef enum {
	AQ_PUSH_FAILED,
	AQ_PUSH_SUCCESS,
	AQ_PUSH_SUCCESS_EMPTY,
} AqPushResult;

//CAPACITY needs to be a power of 2

#define DECLARE_SPSC_QUEUE(TYPE, NAME, CAPACITY)\
	typedef struct {\
		TYPE buffer[CAPACITY];\
		atomic_uint writeIndex;\
		atomic_uint readIndex;\
	} NAME;\

#define DEFINE_SPSC_PUSH(TYPE, NAME, CAPACITY)\
	static inline AqPushResult NAME##_aqPush(NAME *q, TYPE value) {\
		unsigned w = atomic_load_explicit(&q->writeIndex, memory_order_relaxed);\
		unsigned r = atomic_load_explicit(&q->readIndex, memory_order_acquire);\
		bool wasEmpty = (w == r);\
		unsigned next = (w + 1) & (CAPACITY - 1);\
		if (next == r) {\
			return AQ_PUSH_FAILED; /*full*/\
		}\
		q->buffer[w] = value;\
		atomic_store_explicit(&q->writeIndex, next, memory_order_release);\
		return wasEmpty ? AQ_PUSH_SUCCESS_EMPTY : AQ_PUSH_SUCCESS;\
	}\

#define DEFINE_SPSC_POP(TYPE, NAME, CAPACITY)\
	static inline bool NAME##_aqPop(NAME *q, TYPE *out) {\
		unsigned r = atomic_load_explicit(&q->readIndex, memory_order_relaxed);\
		unsigned w = atomic_load_explicit(&q->writeIndex, memory_order_acquire);\
		if (r == w) {\
			return false;\
		}\
		*out = q->buffer[r];\
		unsigned next = (r + 1) & (CAPACITY -1);\
		atomic_store_explicit(&q->readIndex, next, memory_order_release);\
		return true;\
	}\

#define DECLARE_SPSC(TYPE, NAME, CAPACITY)\
	DECLARE_SPSC_QUEUE(TYPE, NAME, CAPACITY)\
	DEFINE_SPSC_PUSH(TYPE, NAME, CAPACITY)\
	DEFINE_SPSC_POP(TYPE, NAME, CAPACITY)

DECLARE_SPSC(int, IntQueue, 256)
#endif
