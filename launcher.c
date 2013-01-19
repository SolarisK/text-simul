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

/***********************************************************/

void init_field(void) {
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
void render_routine(void* no_params) {
    printf("[%s:%d] DO WORK!!!\n", __FUNCTION__, __LINE__);
}
/***********************************************************/

/***********************************************************/

int main(int argc, char** argv) {
    unsigned char field[FIELD_H][FIELD_W] = { {0} };
    unsigned char next_step[FIELD_H][FIELD_W] = { {0} };
    int i, j;
    int res;
    int render_q;

    /* Random seed initialization */
    srand((unsigned int)time(NULL));

    
    render_params_t p;
    p.some_param = 5;
    p.render = render_routine;

    mq_send(render_q, (char*)&p, sizeof(p), 0);

    p.render = NULL;
    mq_send(render_q, (char*)&p, sizeof(p), 0);


    res = mq_close(render_q);
    if (res != 0) {
        perror("mq_close");
    }

    printf("[%s:%d] done...\n", __FUNCTION__, __LINE__);

    return 0;
}
