
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/stat.h>

#include <utility>

#include "rdsextr.h"


int rds_start(int argc, char* argv[]) {
    srand((unsigned)time(NULL));
    int par_size = 4;
    if(argc <  5 ) {;
        printf("Usage:\n\t%s data T D epsilon [thread_number]\n",argv[0]);
        return -1;
    } else {
        char * data = argv[1];
        size_t T = atoi(argv[2]);
        size_t D = atoi(argv[3]);
        double epsilon = atof(argv[4]);
        if(T == 0 || D == 0 || epsilon == 0 ) {
            printf("Usage:\n\t%s data T D epsilon [thread_number]\n",argv[0]);
            return -1;
        } else {
            if(argc > 5 ) {
                par_size = atoi(argv[5]);
            }
            mkdir("result",0755);
            Rdsextr r(data, T, D, epsilon, par_size);
            r.save();
        }
    }
    return 0;
}


int main(int argc, char* argv[]){
    srand((unsigned)time(NULL));
    int result = rds_start(argc,argv);
    return result;
}
