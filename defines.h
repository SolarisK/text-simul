#ifndef __DEFINES_H_
#define __DEFINES_H_

#define FIELD_H     100
#define FIELD_W     100

#define RENDER_QUEUE_NAME "/render_queue"
#define MAX_WORKERS 3

typedef void (*render_routine_t)(void*);

typedef struct _RENDER_PARAMS {
    int some_param;
    render_routine_t render;
} render_params_t;

#endif
