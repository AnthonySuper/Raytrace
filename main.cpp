#include <model.hpp>
#include <driver.hpp>
#include <iostream>
#include <fstream>
#include <unordered_set>

int main(int argc, char** argv) {
    using std::cerr;
    using std::cout;
    using std::endl;
    if(argc < 2) {
        cerr << "No input!" << endl;
        return -1;
    }
    NM::Driver d = NM::Driver::fromFile(argv[1]);
    NM::Scene s = d.getScene();
    auto res = d.getResolution();
    NM::Image img(std::get<0>(res), std::get<1>(res));
    s.render(img, d.getCamera());
    std::ofstream of(argv[2]);
    img.writePPM(of);
    return 0;
}
