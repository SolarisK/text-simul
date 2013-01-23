#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <mqueue.h>
#include "defines.h"
#include "worker.h"

pthread_t workers[MAX_WORKERS];

static int render_queue_id = 0;
static int stop = 0;

static void* worker_routine(void *params) {
    int render_q = 0;
    int res = 0;
    render_params_t task;
    printf("Worker intialization\n");

    render_q = mq_open(RENDER_QUEUE_NAME, O_RDONLY);
    if (render_q == -1) {
        perror("mq_open");
        return NULL;
    }
    printf("queue opened\n");

    mq_getattr(render_q, &render_q_attr);
    printf("render params f=%d maxmsg=%d msgsize=%d curmsg=%d\n", render_q_attr.mq_flags, render_q_attr.mq_maxmsg, render_q_attr.mq_msgsize, render_q_attr.mq_curmsgs);
    while (stop != 1) {
        res = mq_receive(render_q, (char*)&task, sizeof(task), NULL);
        if (res == -1) {
            perror("mq_receive");
            return NULL;
        }

        printf("Task received\n");

        if (task.render != NULL) {
            task.render(NULL);
        }
    }

    printf("[%s:%d] stopping worker...\n", __FUNCTION__, __LINE__);

    mq_close(render_q);

    return NULL;
}

void stub_render_routine(void* no_params) {
    printf("[%s:%d] DO WORK!!!\n", __FUNCTION__, __LINE__);
}

int run_worker_threads() {
    int i;
    int res = 0;

    struct mq_attr render_q_attr;
/*  render_q_attr.mq_flags = 0;
    render_q_attr.mq_maxmsg = 10;
    render_q_attr.mq_msgsize = sizeof(render_params_t);
    render_q_attr.mq_curmsgs = 0; 
*/

    /* create message queue with default paraemters */
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

int do_render(render_params_t *task) {
    if (NULL == task) {
        return 1;
    }
    /* copy struct here and send to render queue */
    return 0;
}

int stop_worker_threads() {
    int i;
    int res = 0;
    void* thread_result = NULL;

    render_params_t stop_task;

    stop_task.some_param = 0;
    stop_task.render = NULL;

    if (render_queue_id == 0) {
        return 1;
    }
    printf("queue exist\n");

    res = mq_unlink(RENDER_QUEUE_NAME);
    if (res != 0) {
        perror("mq_unlink");
        return 1;
    }

    printf("queue unlinked\n");

    for (i = 0; i < MAX_WORKERS; i++) {
        res = pthread_join(workers[i], &thread_result);
        if (res != 0) {
            perror("pthread_join");
        }
    }
    printf("all workers are stopped\n");
    return 0;
}
