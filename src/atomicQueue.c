#define QUEUE_CAPACITY 256
#define SLOT_SIZE 64

typedef struct {
	uint8_t buffer[QUEUE_CAPACITY][SLOT_SIZE];
	atomic_uint writeIndex;
	atomic_uint readIndex;
} AtomicQueue;

bool aqPush(AtomicQueue *aq, const void *data, size_t size);
bool aqPop(AtomicQueue *aq, void *out, size_t size);

bool aqPush(AtomicQueue *aq, const void *data, size_t size) {
	if (size > SLOT_SIZE) {
		return false;
	}

	// reading its own index
	unsigned w = atomic_load_explicit(&aq->writeIndex, memory_order_relaxed);
	// reading other index
	unsigned r = atomic_load_explicit(&aq->readIndex, memory_order_acquire);
	unsigned next = (w + 1) % QUEUE_CAPACITY;

	if (next == r) {
		return false;
	}

	memcpy(aq->buffer[w], data, size);
	atomic_store_explicit(&aq->writeIndex, next, memory_order_release);
	return true;
}

bool aqPop(AtomicQueue *aq, void *out, size_t size) {
	// reading its own index
	unsigned r = atomic_load_explicit(&aq->readIndex, memory_order_relaxed);
	// reading other index
	unsigned w = atomic_load_explicit(&aq->writeIndex, memory_order_acquire);

	if (r == w) {
		return false;
	}

	memcpy(out, &aq->buffer[r], size);
	atomic_store_explicit(&aq->readIndex, (r + 1) % QUEUE_CAPACITY, memory_order_release);
	return true;
}
