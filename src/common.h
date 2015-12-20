#ifndef COMMON_H
#define COMMON_H
#include<stdio.h>
#include <string.h>
#include<stdlib.h>

#include"client.h"

typedef struct node_t{
  int fd;
}node_t;

typedef struct group_t{
  node_t *nodes;
  int count;
}group_t;


#endif
