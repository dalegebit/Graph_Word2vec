
#ifndef LIB_DAMN_GNODE_H_
#define LIB_DAMN_GNODE_H_

#include <vector>
#include <set>
#include <queue> // queue



class NeighborNode {
public:
    size_t id;
    double weight;
    double accum_weight;
    NeighborNode(size_t id,double weight,double accum_weight):
        id(id),
        weight(weight),
        accum_weight(accum_weight) {
    }
    ~NeighborNode(){
    }
};

class GNode{
public:
    std::vector<NeighborNode> neighbors;
    std::vector<size_t> path;
    //ConcurrentQueue<int> path_cq;
    //std::queue<int> path_q;
};

class GPath{
public:
    std::vector<size_t> node_v;
    //ConcurrentQueue<int> node_cq;
    //std::queue<int> node_q;
    //std::set<int> node_s;
};

class RGrapgh{
public:
    RGrapgh(size_t node_size,size_t path_size):
        node_size(node_size),
        path_size(path_size),
        _nodes(new GNode[node_size]),
        _paths(new GPath[path_size]){
    }

    ~RGrapgh(){
        delete [] _nodes;
        delete [] _paths;
    }

    GNode * get_node(size_t offset){
        if(offset >= node_size) {
            return NULL;
        }else{
            return &(_nodes[offset]);
        }
    }

    GPath * get_path(size_t offset){
        if(offset >= path_size) {
            return NULL;
        }else{
            return &(_paths[offset]);
        }
    }

    size_t nsize() const { return node_size;}
    size_t psize() const { return path_size;}

    size_t T;
    double drop_rate;
    size_t R;

private:
    size_t node_size;
    size_t path_size;
    GNode * _nodes;
    GPath * _paths;
};


#endif  // LIB_DAMN_GNODE_H_
