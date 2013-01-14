#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

/***********************************************************/

#define FIELD_H     100
#define FIELD_W     100
#define MAX_UNIT    5


typedef struct _location {
    unsigned int i;
    unsigned int j;
} location_t;

typedef struct _unit {
    pthread_t handle;
    location_t pos;
} unit_t;

unit_t units[MAX_UNIT];

/***********************************************************/

void* unit_routine(void *params) {
    unsigned int i;
    unsigned int j;

    printf("[%s:%d]\n", __FUNCTION__, __LINE__);

    i = rand() % FIELD_H;
    j = rand() % FIELD_W;

    printf("[%s:%d] i = %d j = %d\n", __FUNCTION__, __LINE__, i, j);
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

int main(int argc, char** argv) {
    unsigned char field[FIELD_H][FIELD_W] = { {0} };
    unsigned char next_step[FIELD_H][FIELD_W] = { {0} };
    int i, j;
    int res;

    pthread_mutex_t mutex1 = NULL;

    srand((unsigned int)time(NULL));

    //init_field();

    res = pthread_mutex_init(&mutex, NULL);
    if (res != 0) {
        perror("pthread_mutex_init");
        return EXIT_FAILURE;
    }

    pthread_mutex_lock(&mutex1);

    printf("[%s:%d] done...\n", __FUNCTION__, __LINE__);

    return 0;
}
