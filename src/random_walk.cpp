#include <cstdio>
#include <cstring>
#include <cmath>
#include <ctime>            // std::time



#include <pthread.h> // thread
#include <unistd.h> // usleep

#include <string>

#include <mutex> // std::lock_guard , mutex
#include <queue> // queue
#include <vector>
#include <set>
#include <map>
#include <thread>


#include "ThreadPool.h"

#include "gnode.h"

#include "random_walk.h"


using std::vector;
using std::set;
using std::map;
using std::string;

bool accum_weight_setter_event(RGrapgh * _G , size_t node_start, size_t node_end ) {
    for(size_t item = node_start; item < node_end ; item ++) {
        GNode * _node = _G->get_node(item);
        if(_node != NULL) {
            double sum = 0;
            std::vector<NeighborNode> * _neighbors = &(_node->neighbors);
            for(std::vector<NeighborNode>::iterator it=_neighbors->begin(); it != _neighbors->end() ; it++){
                sum += it->weight;
            }
            if(_node->neighbors.size() > 0 ) {
                _node->neighbors[0].accum_weight /= sum;
                std::vector<NeighborNode>::iterator it=_node->neighbors.begin();
                for(it++ ; it != _node->neighbors.end() ; it++){
                    it->accum_weight = it->accum_weight / sum + ((it - 1) -> accum_weight);
                }
            }
        }
    }
    return true;
}

inline size_t rpg_sim_bs(const std::vector<NeighborNode> & nabors, double val){
    //accum_weight
    if(val < nabors[0].accum_weight) return 0;
    size_t low = 1;
    size_t high = nabors.size() - 1; // nabors[nabors.size() - 1] shall always 1
    size_t middle;
    while(low <= high){
        middle = low + (high-low)/2;
        if(val <= nabors[middle].accum_weight && val > nabors[middle-1].accum_weight){
            return middle;
        }
        if(val < nabors[middle-1].accum_weight){
            high = middle - 1;
        }
        if(val > nabors[middle].accum_weight){
            low = middle + 1;
        }
    }
    return 0; // error
}

bool random_path_generator_event(RGrapgh * _G , size_t pathid_start, size_t pathid_end ) {
    size_t nsize = _G->nsize();
    for(size_t pathid = pathid_start; pathid < pathid_end ; pathid++ ){
        size_t curr_node = rand() % nsize;// 0 - N-1
        GPath * _path = _G->get_path(pathid);
        /*
        set<size_t> * _nodes = &(_path->node_s);
        _nodes->insert(curr_node);
        for(size_t t = 0 ; t < _G->T ; t ++ ) {
            GNode * _node = _G -> get_node(curr_node);
            if(_node->neighbors.size() == 0 ) break;
            //double rand_val = uni();
            double rand_val = (double)rand() / RAND_MAX;
            size_t neighbor_index = rpg_sim_bs(_node->neighbors,rand_val);
            curr_node = _node->neighbors[neighbor_index].id;
            _nodes->insert(curr_node);
        }
        */
        vector<size_t> *_node_v = &(_path->node_v);
        _node_v->push_back(curr_node);
		size_t curr_degree = -1;
        for(size_t t = 0 ; t < _G->T ; t ++ ) {
            GNode * _node = _G -> get_node(curr_node);
            if(_node->neighbors.size() == 0 ) break;
            //double rand_val = uni();
            
			double rand_val = (double)rand() / RAND_MAX;
            size_t neighbor_index = rpg_sim_bs(_node->neighbors,rand_val);
			_node_v->push_back(_node->neighbors[neighbor_index].id);
			if (curr_degree == -1)
				curr_degree = _node->neighbors.size();
			if ((double)rand() / RAND_MAX > _G->drop_rate * (curr_degree/(5+curr_degree))) {
				curr_node = _node->neighbors[neighbor_index].id;
				curr_degree = -1;
			}
			else
				curr_degree--;
        }
    }
    return true;
}



bool pathid_collector_event(RGrapgh * _G , size_t node_start, size_t node_end ) {
    for(size_t pid = 0 ; pid < _G->R ; pid ++ ){
        GPath * _path = _G->get_path(pid);
        if(_path != nullptr ) {
            /*
            set<size_t> *_node_s = &(_path->node_s);
            for (std::set<size_t>::const_iterator it=_node_s->begin(); it!= _node_s->end(); ++it) {
                if((*it) < node_end && (*it) >= node_start) {
                    _G->get_node(*it)->path.push_back(pid);
                }
            }
            */
            vector<size_t> *_node_v = &(_path->node_v);
            for (std::vector<size_t>::const_iterator it=_node_v->begin(); it!= _node_v->end(); ++it) {
                if((*it) < node_end && (*it) >= node_start) {
                    _G->get_node(*it)->path.push_back(pid);
                }
            }
        }
    }
    return true;
}


bool rds_parr_event_manager(RGrapgh * _G , size_t lower_bound, size_t upper_bound,rds_parr_event event,size_t thread_size) {
#ifdef SINGLE_THREAD_DEBUG_MODE
    ThreadPool pool(thread_size);
    size_t slice_len = (upper_bound - lower_bound) / thread_size + 1;
    bool all_done = true;
    std::vector< std::future<bool> > results;
    for(size_t i = 0 ; i < thread_size ; i++ ) {
        size_t c_lower_bound = i * slice_len;
        size_t c_upper_bound = (i+1) * slice_len;
        if(c_upper_bound > upper_bound ) {
            c_upper_bound = upper_bound;
        }
        bool c_status = event(_G,c_lower_bound,c_upper_bound);
        if(!c_status) {
            all_done = false;
        }
    }
    return all_done;
#else
    ThreadPool pool(thread_size);
    size_t slice_len = (upper_bound - lower_bound) / thread_size + 1;
    bool all_done = true;
    std::vector< std::future<bool> > results;
    for(size_t i = 0 ; i < thread_size ; i++ ) {
        results.emplace_back(
            pool.enqueue([i,lower_bound,upper_bound,slice_len,_G,event] {
                size_t c_lower_bound = i * slice_len;
                size_t c_upper_bound = (i+1) * slice_len;
                if(c_upper_bound > upper_bound ) {
                    c_upper_bound = upper_bound;
                }
                return event(_G,c_lower_bound,c_upper_bound);
            })
        );
    }

    for(auto && result: results){
        bool c_status = static_cast<bool>(result.get());
        if(!c_status){
            all_done = false;
        }
    }
    return all_done;
#endif // SINGLE_THREAD_DEBUG_MODE
}
