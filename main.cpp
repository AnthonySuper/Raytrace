#include <model.hpp>
#include <driver.hpp>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <timer.hpp>

int main(int argc, char** argv) {
    using std::cerr;
    using std::cout;
    using std::endl;
    if(argc < 3) {
        cerr << "No input!" << endl;
        return -1;
    }
    NM::Driver d = NM::Driver::fromFile(argv[1]);
    NM::Scene s = d.getScene();
    s.finalize();
    auto res = d.getResolution();
    NM::Image img(std::get<0>(res), std::get<1>(res));
    s.recursionDepth = d.recursionLevel;
    s.render(img, d.getCamera());
    NM::Timer::report("Rendered ");
    img.writeFile(argv[2]);
    return 0;
}
