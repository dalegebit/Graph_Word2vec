randpaths.so:python_wrapper.cpp random_walk.o rdsextr.o
	g++ -shared -fPIC -I/usr/include/python2.7 -L/usr/ python_wrapper.cpp random_walk.o rdsextr.o -o randpaths.so -lpython2.7 -lboost_python

random_walk.o:random_walk.cpp
	g++ random_walk.cpp -o random_walk.o -c -std=c++11 -lpthread -fPIC
rdsextr.o:rdsextr.cpp
	g++ rdsextr.cpp -o rdsextr.o -c -std=c++11 -lpthread -fPIC
clean:
	-rm *.o *.so
