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

namespace NM {
    class Driver {
    public:
        struct DriverModel {
            std::shared_ptr<Model> model;
            std::string fname;
            Mat4 transform;
            Model transformed();
        };
        
        struct DriverTransform {
            Vec4 axis;
            FloatType angle;
            FloatType scale;
            Vec4 translate;
            
            inline Mat4 translationMatrix() const {
                return Transform::translate(translate);
            }
            
            inline Mat4 scaleMatrix() const {
                return Transform::scale(scale);
            }
            
            inline Mat4 rotationMatrix() const {
                return Transform::axisAngle(axis, angle);
            }
            
            inline Mat4 toMatrix() const {
                return translationMatrix() * scaleMatrix() * rotationMatrix();
            }
        };
        
        std::shared_ptr<Model> readModelFile(std::string fname);
        
        std::string getOutName(std::vector<DriverModel>::iterator itr);
        
        static Driver fromFile(std::string fname);
        
        void writeOut();
        
    private:
        std::string driverName;
        void addModel(DriverModel model);
        friend std::istream& operator>>(std::istream&, Driver& driver);
        friend std::ostream& operator<<(std::ostream& os, const Driver& driver);
        std::unordered_map<std::string, std::shared_ptr<Model>> modelDict;
        std::vector<DriverModel> models;
        void makeDirectory();
        std::string directoryName();
    };
    
    std::istream& operator>>(std::istream&, Driver&);
    
    std::istream& operator>>(std::istream&, Driver::DriverTransform&);
    
    std::ostream& operator<<(std::ostream&, const Driver&);
    
    std::ostream& operator<<(std::ostream&, const Driver::DriverModel &);

    
}
