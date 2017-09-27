#include <model.hpp>
#include <driver.hpp>
#include <camera.hpp>
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    using std::cerr;
    using std::cout;
    using std::endl;
    using NM::Driver;
    using NM::Scene;
    using NM::Image;
    using NM::Camera;
    try {
        if(argc < 2) {
            throw std::runtime_error("Gimmie dat arg yo");
        }
        Driver d = Driver::fromFile(argv[1]);
        Scene s = d.toScene();
        NM::printContainer(cout, s.getModels());
        Camera cam = d.getCamera();
        Image i = s.render(cam, d.resX, d.resY);
        std::ofstream out("img.ppm");
        i.writePPM(out);
    }
    catch(int i) {
        
    }
    /*
    catch(std::exception &e) {
        cerr << "Got error: " << e.what() << endl;
        return -1;
    }
    catch(NM::ParseError &p) {
        cerr << "Got parse error: " << p.what() << endl;
    }
     */
    return 0;
}
