#pragma once
#include <iostream>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include <regex>
#include <model.hpp>
#include <globals.hpp>
#include <transforms.hpp>
#include <file_utils.hpp>
#include <transformed_model.hpp>
#include <scene.hpp>

namespace NM {
    class Driver {
    public:
        std::shared_ptr<Model> readModelFile(std::string fname);
        static Driver fromFile(std::string fname);
        Scene toScene();
        FloatType resX;
        FloatType resY;
        
        Camera getCamera();
    private:
        std::string driverName;
        void addModel(TransformedModel model);
        void parseModel(std::istream&);
        void parseCamera(std::istream& cam);

        friend std::istream& operator>>(std::istream&, Driver& driver);
        friend std::ostream& operator<<(std::ostream& os, const Driver& driver);
        std::unordered_map<std::string, std::shared_ptr<Model>> modelDict;
        std::vector<TransformedModel> models;
        Vec4 eye;
        Vec4 look;
        Vec4 up;
        FloatType focalLength;
        FloatType vertBounds[2];
        FloatType horzBounds[2];
        
    };
    
}
