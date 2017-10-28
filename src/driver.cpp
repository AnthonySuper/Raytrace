#include <driver.hpp>

namespace NM {
    static std::string getBasename(std::string in) {
        std::regex pattern("(.+)\\.",
                           std::regex_constants::extended);
        std::smatch m;
        if(! std::regex_search(in, m, pattern)) {
            throw InvalidFilenameError(in);
        }
        return m[1].str();
    }
    
    static void readVec(std::istream& is, Vec4& out) {
        is >> out.x();
        is >> out.y();
        is >> out.z();
    }
    
    static void readVec4(std::istream& is, Vec4& out) {
        is >> out.x();
        is >> out.y();
        is >> out.z();
        is >> out.w();
    }
    
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
    
    std::istream& operator>>(std::istream& is, Driver& driver) {
        std::string line;
        while(std::getline(is, line)) {
            std::cout << line << std::endl;
            std::istringstream lineStream(line);
            std::string header;
            lineStream >> header;
            if(header == "model") {
                std::string fname;
                Driver::DriverTransform dt;
                lineStream >> dt;
                lineStream >> fname;
                auto transform = dt.toMatrix();
                auto model = driver.readModelFile(fname);
                driver.models.emplace_back(model, transform);
            }
            else if(header == "eye") {
                readVec(lineStream, driver.eye);
            }
            else if(header == "look") {
                readVec(lineStream, driver.look);
            }
            else if(header == "up") {
                readVec(lineStream, driver.up);
            }
            else if(header == "d") {
                lineStream >> driver.dist;
            }
            else if(header == "bounds") {
                readVec4(lineStream, driver.bounds);
            }
            else if(header == "res") {
                lineStream >> driver.resY;
                lineStream >> driver.resX;
            }
            else if(header == "sphere") {
                Vec4 pos;
                readVec(lineStream, pos);
                FloatType rad;
                lineStream >> rad;
                Material mat;
                readVec(lineStream, mat.ambient);
                readVec(lineStream, mat.diffuse);
                readVec(lineStream, mat.specular);
                // TODO: Actually read Attunation coefficients
                mat.specularExpon = 16;
                driver.spheres.emplace_back(rad, pos, mat);
            }
            else if(header == "light") {
                Vec4 color;
                Vec4 pos;
                readVec4(lineStream, color);
                readVec(lineStream, pos);
                driver.lights.emplace_back(color, pos);
            }
            else if(header == "ambient") {
                readVec(lineStream, driver.ambient);
            }
            else if(header.length() == 0) {
                continue;
            }
            else if(header.length() > 0 && header.at(0) == '#') {
                continue;
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
    
    std::string Driver::basename() {
        return getBasename(driverName);
    }
    
    std::ostream& operator<<(std::ostream& os, const Driver& driver) {
        os << "[";
        int i = 0;
        for(const auto &model: driver.models) {
            os << model;
            i++;
            if(i != driver.models.size()) {
                os << ", ";
            }
        }
        return os << "}";
    }
    
 
    std::ostream& operator<<(std::ostream& os, const Driver::DriverModel & dr) {
        return os << "{" << dr.model <<
            ",\"" << dr.fname << "\","
            << dr.transform << "}";
    }
    
    CameraAperature Driver::getCameraAperature() const {
        return {
            dist,
            bounds.x(),
            bounds.z(),
            bounds.y(),
            bounds.w()
        };
    }
    
    CameraAxis Driver::getCameraAxis() const {
        return {
            eye,
            look,
            up
        };
    }
    
    Camera Driver::getCamera() const {
        return {
            getCameraAxis(),
            getCameraAperature()
        };
    }
    
    Scene Driver::getScene() const {
        Scene s;
        for(auto& m : models) {
            s.addObject(m);
        }
        for(auto& sp: spheres) {
            s.addObject(sp);
        }
        for(auto& l: lights) {
            s.addObject(l);
        }
        s.ambient = ambient;
        return s;
    }
    
    std::tuple<size_t, size_t> Driver::getResolution() {
        return std::make_tuple(resX, resY);
    }
    
    std::istream& operator>>(std::istream& is, Driver::DriverTransform &trans) {
        FloatType rx, ry, rz, angle, scale, tx, ty, tz;
        is >> rx;
        is >> ry;
        is >> rz;
        is >> angle;
        is >> scale;
        is >> tx;
        is >> ty;
        is >> tz;
        FloatType radAngle = NM::degToRad(angle);
        trans.axis = {rx, ry, rz};
        trans.angle = radAngle;
        trans.translate = {tx, ty, tz};
        trans.scale = scale;
        return is;
    }
    
    
    
}
