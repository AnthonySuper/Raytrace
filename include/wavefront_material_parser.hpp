#pragma once
#include <limits>
#include <vec4.hpp>
#include <wavefront_material.hpp>

namespace NM {
    struct WavefrontMaterialParser {
        std::istream& is;
        WavefrontMaterialParser(std::istream&, std::string fileName = "");
        std::shared_ptr<WavefrontMaterialLibrary> parse();
        
    private:
        std::string fileName;
        std::shared_ptr<WavefrontMaterialLibrary> library;
        bool hasFirstPush = false;
        std::string currentMatName;
        Material currentMaterial;
        WavefrontMaterial currentExtras;
        void parseNewMaterial();
        void readKa();
        void readKd();
        void readKs();
        void readD();
        void readIllum();
        void readSpecularExponent();
        void flushMaterial();
        void readMapKd();
        Vec4 readVec();
        FloatType readTruncedFloat();
        FloatType readFloat();
        void parseError(std::string msg);
        
    };
    
}
