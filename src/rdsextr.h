
#ifndef INCLUDE_RDSEXTR_H_
#define INCLUDE_RDSEXTR_H_

#include <cstdio>
#include <cmath>

// #include <thread>  // std::thread
#include <string>


#include "gnode.h"


inline std::string to_string(size_t a) {
    char s[50];
    snprintf(s,49,"%u",a);
    return std::string(s);
}



class Rdsextr {
public :
    Rdsextr(const std::string & m_data, size_t m_t, double m_d, double m_epsilon,size_t par_size = 8) :
        data(m_data),
        T(m_t),
        drop_rate(m_d),
        epsilon(m_epsilon),
        par_size(par_size),
        R((size_t)(((double)0.5/(epsilon * epsilon))*((log2(T*(T-1)/2) + 1) + log((double)1/0.1)))),
        dis_epsilon(round(epsilon*1000000)),
        network_filename("data/" + data + ".graph"),
        map_filename("data/" + data + ".dict"),
		all_path_filename("result/"+data+"_"+to_string(T)+"_"+to_string(drop_rate*1000000)+"_"+to_string(dis_epsilon)+".allpath"),
		all_path_file(fopen(all_path_filename.c_str(), "w")){
            init();
    }

    ~Rdsextr () {
        if(_G != NULL)
            delete _G;
		if(all_path_file != NULL){
			fclose(all_path_file);
		}
    }

    void init();

    void read_ds_size(char * line , size_t &n, size_t &m); // readMandN
    void load_link();

    void gen_random_path_parr();

    void save();

    //inline int calR() {
    //    return (int)(((double)0.5/(epsilon * epsilon))*((log2(T*(T-1)/2) + 1) + log((double)1/0.1)));
    //}
    inline int get_R(){
        return R;
    }

    std::string get_path_str(int pid){
        std::string path_nodes = "";
		GPath * _path = _G->get_path(pid);
		if (_path != NULL) {
			std::vector<size_t> * _node_v = &(_path->node_v);
			for (std::vector<size_t>::const_iterator it = _node_v->begin(); it != _node_v->end(); ++it)
				path_nodes += " " + to_string(*it);
		}
        return path_nodes;
    }

private :
    const std::string data;
    size_t T;
    double epsilon;
    size_t par_size;
    size_t R;
    size_t dis_epsilon; // displayEpsilon
	double drop_rate;
    const std::string network_filename; // network_file
    const std::string map_filename; // map_file
	const std::string all_path_filename; // file name , allpath
	FILE * all_path_file;
    RGrapgh * _G;
    size_t N; // Node Size
    size_t M; // Edge Size

};


#endif  // INCLUDE_RDSEXTR_H_
