#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>
#include "defines.h"
#include "worker.h"

pthread_t workers[MAX_WORKERS];

static void* worker_routine(void *params) {
    int render_q = 0;
    int stop = 0;
    int res = 0;
    render_params_t task;

    render_q = mq_open(RENDER_QUEUE_NAME, O_RDONLY);
    if (render_q == -1) {
        perror("mq_open");
        return NULL;
    }

    while (stop != 1) {
        res = mq_receive(render_q, (char*)&task, sizeof(task), NULL);
        if (res == -1) {
            perror("mq_receive");
        }

        stop = (task.render == NULL) ? 1 : 0;

        if (task.render != NULL) {
            task.render(NULL);
        }
    }
    printf("[%s:%d] stopping worker...\n", __FUNCTION__, __LINE__);

    mq_close(render_q);

    return NULL;
}

void run_worker_threads() {
    int i;
    int res = 0;

    for (i = 0; i < MAX_WORKERS; i ++) {
        res = pthread_create(&workers[i], NULL, worker_routine, NULL);
        if (res) {
            perror("ptrhead_create");
        }

    }
}

void stop_worker_threads() {
    int i;

    for (i = 0; i < MAX_WORKERS; i++) {
        mq_send
    }
}
