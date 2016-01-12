#include<sys/time.h>
#include<time.h>
#include<unistd.h>

#include"failure_detector.h"

struct timespec wake_up;

void init_P(){
    wake_up.tv_sec = 0;
    wake_up.tv_nsec = WAKE_TIME;
}

void check_node(gpointer key, gpointer value, gpointer userdata){
    node_t* node = (node_t*) value;
    struct timeval time;
    
    assert(node);
    if(pthread_mutex_lock(&node->mtx) != 0){
        perror("Cannot lock the mutex when checking node");
        exit(EXIT_FAILURE);
    }

    if(node->alive){
        gettimeofday(&time, NULL);
        if( (time.tv_sec - node->time.tv_sec) == 0 && (time.tv_sec - node->time.tv_sec) <= TIME_DEATH ){
            node->alive = false;
        }
    }
    
    if(pthread_mutex_unlock(&node->mtx) != 0){
        perror("Cannot free the mutex when checking node");
        exit(EXIT_FAILURE);
    }
}

void* run_P(){
    init_P();
    while(!terminate){
        nanosleep(&wake_up, NULL);
        /* usleep(100000); */
        g_hash_table_foreach(group, (GHFunc)check_node, NULL);
    }
}
