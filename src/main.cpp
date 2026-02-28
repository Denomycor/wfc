#include "wave.hpp"
#include <cstdio>
#include <cstdlib>

int main(int argc, char** argv){
    srand(atoi(argv[1]));

    std::map<int, double> m{{1,50}, {2,25}, {3,25}};
    Wave wave(6, 6, m);
    wave.get_constraints().add(1, {1,2}, {1,2}, {1,2}, {1,2});
    wave.get_constraints().add(2, {1,2,3}, {1,2,3}, {1,2,3}, {1,2,3});
    wave.get_constraints().add(3, {2,3}, {2,3}, {2,3}, {2,3});

    wave.run();
    wave.print();

}

