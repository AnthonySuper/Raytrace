#include <driver.hpp>

namespace NM {
    
    Driver Driver::fromFile(std::string fname) {
        std::ifstream fstream(fname);
        if(! fstream) {
            throw FileNotFoundError(fname);
        }
        Driver driver;
        fstream >> driver;
        driver.driverName = fname;
        return driver;
    }
    
    void readSpaceVec(std::istream& is, Vec4& v) {
        is >> v.x();
        is >> v.y();
        is >> v.z();
    }
    
    std::istream& operator>>(std::istream& is, Driver& driver) {
        std::string line;
        while(std::getline(is, line)) {
            std::istringstream lineStream(line);
            std::string header;
            lineStream >> header;
            if(header == "model") {
                driver.parseModel(lineStream);
            }
            else if(header == "eye") {
                readSpaceVec(lineStream, driver.eye);
            }
            else if(header == "look") {
                readSpaceVec(lineStream, driver.look);
            }
            else if(header == "up") {
                readSpaceVec(lineStream, driver.up);
            }
            else if(header == "d") {
                lineStream >> driver.focalLength;
            }
            else if(header == "bounds") {
                lineStream >> driver.vertBounds[0];
                lineStream >> driver.horzBounds[0];
                lineStream >> driver.vertBounds[1];
                lineStream >> driver.horzBounds[1];
            }
            else if(header == "res") {
                lineStream >> driver.resX;
                lineStream >> driver.resY;
            }
            else if(header == "sphere") {
                // do nothing!
            }
            else if(header == "") {
                // do nothing!
            }
            else {
                throw ParseError(
                                 std::string("Coult not parse header '") +
                                 header + "'"
                                 );
            }
        }
        return is;
    }
    
    void Driver::parseModel(std::istream &lineStream) {
        FloatType x, y, z, angle, scale, tx, ty, tz;
        std::string fname;
        lineStream >> x;
        lineStream >> y;
        lineStream >> z;
        lineStream >> angle;
        lineStream >> scale;
        lineStream >> tx;
        lineStream >> ty;
        lineStream >> tz;
        lineStream >> fname;
        if(lineStream.fail()) {
            throw ParseError("Could not parse driver file");
        }
        angle = degToRad(angle);
        auto t = Transform::axisAngle({x, y, z}, angle) *
        Transform::scale(scale) *
        Transform::translate({tx, ty, tz});
        auto r = readModelFile(fname);
        addModel({r, t});
    }
    
    Scene Driver::toScene() {
        Scene ret;
        for(auto& model: models) {
            ret.addModel(model);
        }
        return ret;
    }
    
    std::shared_ptr<Model> Driver::readModelFile(std::string fname) {
        auto itr = modelDict.find(fname);
        if(itr == modelDict.end()) {
            std::ifstream readStream(fname);
            auto read = std::make_shared<Model>(Model::fromStream(readStream));
            modelDict.emplace(fname,
                              read);
            readStream.close();
            return read;
        }
        else {
            return itr->second;
        }
    }
    
    Camera Driver::getCamera() {
        return Camera{eye, look, -focalLength, vertBounds, horzBounds, up};
    }
    
    void Driver::addModel(TransformedModel model) {
        models.push_back(model);
    }
    
}
