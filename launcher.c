#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>

#include "defines.h"
#include "worker.h"

/***********************************************************/
typedef struct _location {
    unsigned int i;
    unsigned int j;
} location_t;

typedef struct _unit {
    pthread_t handle;
    location_t pos;
} unit_t;
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
int main(int argc, char** argv) {
    unsigned char field[FIELD_H][FIELD_W] = { {0} };
    unsigned char next_step[FIELD_H][FIELD_W] = { {0} };
    int i, j;
    int res;
    int render_q;

    /* Random seed initialization */
    srand((unsigned int)time(NULL));

    res = run_worker_threads();
    if (res != 0) {
       printf("error while init..");
    }

//    mq_send(render_q, (char*)&p, sizeof(p), 0);
//   
//    sleep(2);
//
//    res = stop_worker_threads();
//    if (res != 0) {
//        printf("[%s:%d] error while stoping...\n", __FUNCTION__, __LINE__);
//    }
//
    printf("[%s:%d] done...\n", __FUNCTION__, __LINE__);

    return 0;
}
