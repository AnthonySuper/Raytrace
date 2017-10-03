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

    NM::Driver d = NM::Driver::fromFile(argv[1]);
    NM::Scene s = d.getScene();
    auto res = d.getResolution();
    NM::Image img(std::get<0>(res), std::get<1>(res));
    NM::Camera cam = d.getCamera();
    s.render(img, d.getCamera());
    std::ofstream of("img.ppm");
    img.writePPM(of);
    return 0;
}
