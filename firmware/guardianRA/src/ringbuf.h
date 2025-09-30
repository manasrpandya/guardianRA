#ifndef RINGBUF_H
#define RINGBUF_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float *data;
    uint16_t cap;
    uint16_t head;   /* next write */
    uint16_t count;  /* number of valid items */
} ringbuf_f32_t;

/* Initialize a ring buffer over caller-provided storage of size cap. */
void ringbuf_f32_init(ringbuf_f32_t *rb, float *storage, uint16_t cap);

/* Push one value. Overwrites oldest when full. */
void ringbuf_f32_push(ringbuf_f32_t *rb, float v);

/* Compute average of current contents. Returns 0.0f if empty. */
float ringbuf_f32_avg(const ringbuf_f32_t *rb);

#ifdef __cplusplus
}
#endif

#endif /* RINGBUF_H */
