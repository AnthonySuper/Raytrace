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
#include <transformed_drawable.hpp>
#include <camera.hpp>
#include <scene.hpp>

namespace NM {
    class Driver {
    public:
        struct DriverModel {
            std::shared_ptr<Model> model;
            std::string fname;
            Mat4 transform;
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
        
        CameraAperature getCameraAperature() const;
        
        CameraAxis getCameraAxis() const;
        
        Camera getCamera() const;
        
        std::tuple<size_t, size_t> getResolution();
        
        std::string basename();
        
        Scene getScene() const;
        
        unsigned int recursionLevel = 0;
    private:
        Vec4 eye;
        Vec4 look;
        Vec4 up;
        Vec4 bounds;
        FloatType dist;
        size_t resX, resY;
        std::string driverName;
        friend std::istream& operator>>(std::istream&, Driver& driver);
        friend std::ostream& operator<<(std::ostream& os, const Driver& driver);
        std::unordered_map<std::string, std::shared_ptr<Model>> modelDict;
        std::vector<TransformedDrawable> models;
        std::vector<Sphere> spheres;
        std::vector<Light> lights;
        Vec4 ambient;
        
    };
    
    std::istream& operator>>(std::istream&, Driver&);
    
    std::istream& operator>>(std::istream&, Driver::DriverTransform&);
    

    
    std::ostream& operator<<(std::ostream&, const Driver&);
    
    std::ostream& operator<<(std::ostream&, const Driver::DriverModel &);

    
}
