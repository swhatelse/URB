#include"node.h"

bool is_the_same_node(const node_t node1, const node_t node2){
    if(node1.id == node2.id){
        return true;
    }
    else{
        return false;
    }
}

