#include<sys/time.h>
#include<time.h>
#include<unistd.h>

#include"failure_detector.h"

struct timespec wake_up;

void init_P(){
    wake_up.tv_sec = 0;
    wake_up.tv_nsec = 1;
}

void check_node(gpointer key, gpointer value, gpointer userdata){
    node_t* node = (node_t*) value;
    struct timeval time;
    
    assert(node);
    if(pthread_mutex_lock(&node->mtx) != 0){
        perror("Cannot lock the mutex when checking node");
        exit(EXIT_FAILURE);
    }

    gettimeofday(&time, NULL);
    if(node->alive){
        if( ((time.tv_sec - node->time.tv_sec) > 0) || ((time.tv_sec - node->time.tv_sec) == 0  && (time.tv_usec - node->time.tv_usec) >= TIME_DEATH )){
            node->alive = false;
            DEBUG_ERR("[Node:%d] Is dead\n", node->id);
        }
    }
    else{
        if( ((time.tv_sec - node->time.tv_sec) < 0) || ((time.tv_sec - node->time.tv_sec) == 0 && (time.tv_usec - node->time.tv_usec) < TIME_DEATH) ){
            node->alive = true;
            DEBUG_ERR("[Node:%d] Is alive\n", node->id);
        }
    }
    
    if(pthread_mutex_unlock(&node->mtx) != 0){
        perror("Cannot free the mutex when checking node");
        exit(EXIT_FAILURE);
    }

    message_t msg;
    msg.type = 'H';
    msg.id = -1;
    msg.node_id = my_id;

    multicast(&msg, sizeof(message_t));
}

void* run_P(){
    init_P();
    while(!terminate){
        nanosleep(&wake_up, NULL);
        g_hash_table_foreach(group, (GHFunc)check_node, NULL);
    }
}
