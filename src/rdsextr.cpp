
#include "rdsextr.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <vector>
#include <set>
#include <map>
#include <utility> // pair
#include <random>
#include <string>


#include "gnode.h"
#include "random_walk.h"

using std::vector;
using std::set;
using std::map;
using std::pair;

void Rdsextr::init() {
    //eng.seed((unsigned long long)time(NULL));
    printf("initing ... \n");
    fflush(NULL);
    printf("Rdsextr Starting ...\ndata:%s, T:%zu, drop_rate:%f, R:%zu, E:%f\n", data.c_str(), T, drop_rate, R, epsilon);
    fflush(NULL);
    printf("displayed epsilon %zu , network file [%s] , map file [%s]\n",
                    dis_epsilon, network_filename.c_str(), map_filename.c_str());
    fflush(NULL);
    printf("result file stored in : [%s]\n", all_path_filename.c_str());
    load_link();
    //gen_random_path();
    gen_random_path_parr();
}

void Rdsextr::read_ds_size(char * line , size_t &n, size_t &m) {
    char * k = strtok(line," ");
    if( k == NULL) {
       fprintf(stderr,"[read_ds_size] error unknown val : %s\n",line);
       return;
    }
    n = atoll(k);
    k =  strtok(NULL,"\t");
    if( k == NULL) {
       fprintf(stderr,"[read_ds_size] erroe unknown val : %s\n",line);
        return ;
    }
    m = atoll(k);
}

void Rdsextr::load_link() {
    FILE *fp = fopen(network_filename.c_str(), "r");
    printf("network_filename: %s\n", network_filename.c_str());
	char line[1000];
    fgets(line,1000, fp);
    read_ds_size(line,N,M);
    printf("N: %zu, M: %zu, R=%zu\n",N, M, R);
    fflush(NULL);
    _G = new RGrapgh(N,R);
    printf("graph space allocated\n");
    fflush(NULL);

    // Global Static Environment Setting
    _G->T = T;
	_G->drop_rate = drop_rate;
    _G->R = R;

    while(!feof(fp)){
        int a, b;
        double c;
        fgets(line,100, fp);
        sscanf(line, "%d\t%d\t%lf", &a, &b,&c);
        _G->get_node(a)->neighbors.push_back(NeighborNode(b,c,c));
        _G->get_node(b)->neighbors.push_back(NeighborNode(a,c,c));
    }

}

void Rdsextr::gen_random_path_parr(){
    printf("sample size = %zu\n", R);
    printf("thread size = %zu\n", par_size);
    printf("[weight] starting...\n");
    fflush(NULL); // flush cache
    bool status = rds_parr_event_manager(_G,0,N,accum_weight_setter_event,par_size);
    printf("[weight] stopped [%s] \n",(status?"OK":"FAILED"));
    fflush(NULL); // flush cache

    printf("[rpath gene] starting...\n");
    fflush(NULL); // flush cache
    status = rds_parr_event_manager(_G,0,R,random_path_generator_event,par_size);
    printf("[rpath gene] stopped [%s] \n",(status?"OK":"FAILED"));
    fflush(NULL); // flush cache
}



void Rdsextr::save(){
    printf("[save result file] starting...\n");
    fflush(NULL); // flush cache
	for (size_t pid = 0; pid < _G->R; pid ++){
		GPath * _path = _G->get_path(pid);
		if (_path != nullptr) {
			std::string path_nodes = "";
			vector<size_t> * _node_v = &(_path->node_v);
			for (std::vector<size_t>::const_iterator it = _node_v->begin(); it != _node_v->end(); ++it)
				path_nodes += " " + std::to_string(*it);
			fprintf(all_path_file, "%s\n", path_nodes.c_str());
		}
	}
    printf("[save result file] end \n");
    fflush(NULL); // flush cache
}
