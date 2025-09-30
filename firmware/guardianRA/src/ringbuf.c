#include "ringbuf.h"

void ringbuf_f32_init(ringbuf_f32_t *rb, float *storage, uint16_t cap)
{
    rb->data = storage;
    rb->cap = cap;
    rb->head = 0u;
    rb->count = 0u;
    for (uint16_t i = 0; i < cap; ++i) rb->data[i] = 0.0f;
}

void ringbuf_f32_push(ringbuf_f32_t *rb, float v)
{
    rb->data[rb->head] = v;
    rb->head = (uint16_t)((rb->head + 1u) % rb->cap);
    if (rb->count < rb->cap) {
        rb->count++;
    }
}

float ringbuf_f32_avg(const ringbuf_f32_t *rb)
{
    if (!rb->count) return 0.0f;
    float s = 0.0f;
    for (uint16_t i = 0; i < rb->count; ++i) s += rb->data[i];
    return s / (float)rb->count;
}
