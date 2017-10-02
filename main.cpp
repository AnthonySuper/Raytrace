#include <model.hpp>
#include <driver.hpp>
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    using std::cerr;
    using std::cout;
    using std::endl;
    if(argc < 2) {
        cerr << "No input!" << endl;
        return -1;
    }
    try {
        NM::Driver d = NM::Driver::fromFile(argv[1]);
        
    }
    catch(std::exception &e) {
        cerr << "Got error: " << e.what() << endl;
        return -1;
    }
    return 0;
}
