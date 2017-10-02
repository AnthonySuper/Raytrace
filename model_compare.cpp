#include <model.hpp>
#include <iostream>
#include <fstream>

using NM::Model;

int main(int argc, const char **argv) {
    if(argc < 3) {
        std::cerr << "Need more arguments!" << std::endl;
    }
    std::ifstream goodIn(argv[1]);
    std::ifstream testIn(argv[2]);
    Model m = Model::fromStream(goodIn);
    Model test = Model::fromStream(testIn);
    m.debugCompare(test);
}
