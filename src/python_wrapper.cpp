#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/stat.h>
#include <string>
#include <utility>

#include "rdsextr.h"

#include <boost/python.hpp>


namespace py = boost::python;

py::list damn_get_randpaths(std::string data, int T, double drop_rate, double epsilon, int par_size = 4)
{
    mkdir("result",0755);
    Rdsextr damn(data, T, drop_rate, epsilon, par_size);
    damn.save();
	py::list paths;
    int R = damn.get_R();
    std::string path_str;
    for (int i = 0; i < R; ++i) {
        path_str = damn.get_path_str(i);
        paths.append(path_str);
    }
    return paths;
}

BOOST_PYTHON_MODULE(randpaths) {
    def("damn_get_randpaths", damn_get_randpaths);
}
