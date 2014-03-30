#include <pthread.h>
#include <semaphore.h>

#ifndef TRACE_RINGBUFFER_H
#define TRACE_RINGBUFFER_H

#define LIBTRACE_RINGBUFFER_BLOCKING 0
#define LIBTRACE_RINGBUFFER_POLLING 1

// All of start, elements and end must be accessed in the listed order
// if LIBTRACE_RINGBUFFER_POLLING is to work.
typedef struct libtrace_ringbuffer {
	volatile int start;
	int size;
	int mode;
	void *volatile*elements;
	pthread_mutex_t wlock;
	pthread_mutex_t rlock;
	pthread_spinlock_t swlock;
	pthread_spinlock_t srlock;
	sem_t semrlock;
	sem_t semwlock;
	sem_t emptys;
	sem_t fulls;
	// Aim to get this on a separate cache line to start - important if spinning
	volatile int end;
	
} libtrace_ringbuffer_t;

void libtrace_ringbuffer_init(libtrace_ringbuffer_t * rb, int size, int mode);
inline void libtrace_zero_ringbuffer(libtrace_ringbuffer_t * rb);
void libtrace_ringbuffer_destroy(libtrace_ringbuffer_t * rb);
int libtrace_ringbuffer_is_empty(const libtrace_ringbuffer_t * rb);
int libtrace_ringbuffer_is_full(const libtrace_ringbuffer_t * rb);

void libtrace_ringbuffer_write(libtrace_ringbuffer_t * rb, void* value);
int libtrace_ringbuffer_try_write(libtrace_ringbuffer_t * rb, void* value);
void libtrace_ringbuffer_swrite(libtrace_ringbuffer_t * rb, void* value);
int libtrace_ringbuffer_try_swrite(libtrace_ringbuffer_t * rb, void* value);
int libtrace_ringbuffer_try_swrite_bl(libtrace_ringbuffer_t * rb, void* value);

void* libtrace_ringbuffer_read(libtrace_ringbuffer_t *rb) ;
int libtrace_ringbuffer_try_read(libtrace_ringbuffer_t *rb, void ** value);
void * libtrace_ringbuffer_sread(libtrace_ringbuffer_t *rb);
int libtrace_ringbuffer_try_sread(libtrace_ringbuffer_t *rb, void ** value);
int libtrace_ringbuffer_try_sread_bl(libtrace_ringbuffer_t *rb, void ** value);

#endif
