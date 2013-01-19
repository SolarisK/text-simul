#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <mqueue.h>
#include "defines.h"
#include "worker.h"

pthread_t workers[MAX_WORKERS];

static int render_queue_id = 0;

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

int run_worker_threads() {
    int i;
    int res = 0;

/*  
    struct mq_attr render_q_attr;
    render_q_attr.mq_flags = 0;
    render_q_attr.mq_maxmsg = 10;
    render_q_attr.mq_msgsize = sizeof(render_params_t);
    render_q_attr.mq_curmsgs = 0; 
*/

    render_queue_id = mq_open(RENDER_QUEUE_NAME, O_CREAT | O_WRONLY, 0600, NULL);
    if (render_queue_id == -1) {
        printf("[%s:%d] error = %d\n", __FUNCTION__, __LINE__, errno);
        perror("mq_open");
        return 1;
    }

    for (i = 0; i < MAX_WORKERS; i ++) {
        res = pthread_create(&workers[i], NULL, worker_routine, NULL);
        if (res) {
            perror("ptrhead_create");
            return 1;  /* TODO: define list of errors */
        }
    }

    return 0;
}

void stop_worker_threads() {
    int i;

    for (i = 0; i < MAX_WORKERS; i++) {
        //mq_send
    }
}
