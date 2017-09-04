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
            std::istringstream lineStream(line);
            std::string header;
            lineStream >> header;
            if(header == "model") {
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
                Driver::DriverModel toPush;
                toPush.transform = Transform::axisAngle({x, y, z}, angle) *
                Transform::scale(scale) *
                Transform::translate({tx, ty, tz});
                toPush.fname = fname;
                toPush.model = driver.readModelFile(fname);
                driver.addModel(toPush);
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
    
    std::string Driver::getOutName(std::vector<DriverModel>::iterator itr) {
        int count = 0;
        for(auto it = models.begin(); it < itr; ++it) {
            if(it->fname == itr->fname) {
                count++;
            }
        }
        auto basename = getBasename(itr->fname);
        std::string countStr;
        if(count > 9) {
            countStr = std::to_string(count);
        }
        else {
            countStr = "0" + std::to_string(count);
        }
        return directoryName() + "/" + basename + "_mw" + countStr + ".obj";
    }
    
    void Driver::addModel(NM::Driver::DriverModel model) {
        models.push_back(model);
    }
    
    void Driver::makeDirectory() {
        auto name = directoryName();
        FileUtils::mkdir_p(name.c_str());
    }
    
    void Driver::writeOut() {
        makeDirectory();
        for(auto itr = models.begin(); itr < models.end(); ++itr) {
            std::string outname = getOutName(itr);
            std::ofstream outStr(outname);
            if(! outStr) {
                throw std::runtime_error(std::string("Couldn't open file ") +
                                         outname);
            }
            itr->transformed().writeObj(outStr);
            outStr.close();
        }
    }
    
    std::string Driver::directoryName() {
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
    
    Model Driver::DriverModel::transformed() {
        if(! model) {
            throw std::runtime_error("Null model!");
        }
        return transform * (*model);
    }
    
    std::ostream& operator<<(std::ostream& os, const Driver::DriverModel & dr) {
        return os << "{" << dr.model <<
            ",\"" << dr.fname << "\","
            << dr.transform << "}";
    }
}
