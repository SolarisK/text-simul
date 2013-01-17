#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>

/***********************************************************/

#define FIELD_H     100
#define FIELD_W     100
#define MAX_UNIT    5

typedef void (*render_routine_t)(void*);

typedef struct _location {
    unsigned int i;
    unsigned int j;
} location_t;

typedef struct _unit {
    pthread_t handle;
    location_t pos;
} unit_t;

typedef struct _RENDER_PARAMS {
    int some_param;
    render_routine_t render;
} render_params_t;

unit_t units[MAX_UNIT];

/***********************************************************/

void* unit_routine(void *params) {
    int render_q = 0;
    int stop = 0;
    int res = 0;
    render_params_t task;

    render_q = mq_open("/render_queue", O_RDONLY);
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

/***********************************************************/

void init_field(void) {
    int result;
    int count;

    for (count = 0; count < MAX_UNIT; count++) {
        result = pthread_create(&(units[count].handle), NULL, unit_routine, NULL);
        if (result) {
            perror("Unit creation error\n");
        }
    }
    printf("[%s:%d] init done...\n", __FUNCTION__, __LINE__);
}

/***********************************************************/
int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a > b ? b : a;
}

int is_close_units(location_t *one, location_t *two) {
    if (((max(one->i, two->i) - min(one->i, two->i)) <= 2) &&
            ((max(one->j, two->j) - min(one->j, two->j)) <= 2)){
        return 1; 
    } else {
        return 0;
    }
}
/***********************************************************/
void render(void* no_params) {
    printf("[%s:%d] DO WORK!!!\n", __FUNCTION__, __LINE__);
}
/***********************************************************/

int main(int argc, char** argv) {
    unsigned char field[FIELD_H][FIELD_W] = { {0} };
    unsigned char next_step[FIELD_H][FIELD_W] = { {0} };
    int i, j;
    int res;
    int render_q;

    srand((unsigned int)time(NULL));
    //
    //    perror("lock");
    //    pthread_mutex_lock(&mutex1);
    //
    //    perror("init");
    //    init_field();
    //
    //    perror("unlock");
    //    pthread_mutex_unlock(&mutex1);
    //
    //    perror("join");
    //    pthread_join(units[0].handle, NULL);
    //
    //    perror("destroy");
    //    pthread_mutex_destroy(&mutex1);


    struct mq_attr render_q_attr;
    render_q_attr.mq_flags = 0;
    render_q_attr.mq_maxmsg = 10;
    render_q_attr.mq_msgsize = sizeof(render_params_t);
    render_q_attr.mq_curmsgs = 0;

    render_q = mq_open("/render_queue", O_CREAT | O_WRONLY, 0600, &render_q_attr);
    if (render_q == -1) {
        printf("[%s:%d] error = %d\n", __FUNCTION__, __LINE__, errno);
        perror("mq_open");
        return EXIT_FAILURE;
    }

    pthread_t worker;
    res = pthread_create(&worker, NULL, unit_routine, NULL);
    if (res) {
        perror("ptrhead_create");
    }

    render_params_t p;
    p.some_param = 5;
    p.render = render;

    mq_send(render_q, (char*)&p, sizeof(p), 0);

    p.render = NULL;
    mq_send(render_q, (char*)&p, sizeof(p), 0);

    pthread_join(worker, NULL);

    res = mq_close(render_q);
    if (res != 0) {
        perror("mq_close");
    }

    printf("[%s:%d] done...\n", __FUNCTION__, __LINE__);

    return 0;
}
