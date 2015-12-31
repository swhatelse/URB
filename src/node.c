#include"node.h"

bool is_the_same_node(const node_t node1, const node_t node2){
    if(node1.infos.sin_addr.s_addr == node2.infos.sin_addr.s_addr &&
       node1.infos.sin_port == node2.infos.sin_port){
        return true;
    }
    else{
        return false;
    }
}

