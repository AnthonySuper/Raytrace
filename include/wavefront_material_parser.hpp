#pragma once
#include <limits>
#include <vec4.hpp>
#include <material.hpp>

namespace NM {
    struct WavefrontMaterialParser {
        std::istream& is;
        WavefrontMaterialParser(std::istream&, std::string fileName = "");
        std::shared_ptr<MaterialLibrary> parse();
        
    private:
        std::string fileName;
        std::shared_ptr<MaterialLibrary> library;
        bool hasFirstPush = false;
        std::string currentMatName;
        Material currentMaterial;
        void parseNewMaterial();
        void readKa();
        void readKd();
        void readKs();
        void readD();
        void readIllum();
        void readSpecularExponent();
        void flushMaterial();
        Vec4 readVec();
        FloatType readTruncedFloat();
        FloatType readFloat();
        void parseError(std::string msg);
        
    };
    
}
