#include <model.hpp>
#include <driver.hpp>
#include <camera.hpp>
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    using std::cerr;
    using std::cout;
    using std::endl;
    try {
        NM::Camera cam({0, 0, 0}, {8, 1, 0}, -1);
        cout << cam;
        auto v = cam.getRays(2, 2);
        cout << v.size() << endl;
        for(auto &r: v) {
            cout << r << std::endl;
        }
        cout << endl;
        cout << cam.getRay(4, 4, 0, 3);
    }
    catch(std::exception &e) {
        cerr << "Got error: " << e.what() << endl;
        return -1;
    }
    return 0;
}
