#include <wavefront_material_parser.hpp>

namespace NM {
    WavefrontMaterialParser::WavefrontMaterialParser(std::istream& i, std::string fn) :
    is(i), fileName(fn), library(std::make_shared<MaterialLibrary>())
    {}
    
    std::shared_ptr<MaterialLibrary> WavefrontMaterialParser::parse() {
        while(! is.eof()) {
            std::string lineHeader;
            is >> lineHeader;
            if(lineHeader == "#") {
                std::string comment;
                std::getline(is, comment);
            }
            else if(lineHeader == "newmtl") {
                parseNewMaterial();
            }
            else if(lineHeader == "Ka") {
                readKa();
            }
            else if(lineHeader == "Kd") {
                readKd();
            }
            else if(lineHeader == "Ks") {
                readKs();
            }
            else if(lineHeader == "d") {
                readD();
            }
            else if(lineHeader == "illum") {
                readIllum();
            }
            else if(lineHeader == "Ks") {
                readSpecularExponent();
            }
            else {
                is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        flushMaterial();
        return library;
    }
    
    void WavefrontMaterialParser::parseNewMaterial() {
        // We've parsed materials before
        // So flush this one, basically
        if(hasFirstPush) {
            flushMaterial();
        }
        else {
            hasFirstPush = true;
        }
        is >> currentMatName;
    }
    
    void WavefrontMaterialParser::readKa() {
        currentMaterial.ambient = readVec();
    }
    
    void WavefrontMaterialParser::readKs() {
        currentMaterial.specular = readVec();
    }
    
    void WavefrontMaterialParser::readKd() {
        currentMaterial.diffuse = readVec();
    }
    
    void WavefrontMaterialParser::readD() {
        currentMaterial.dissolve = readTruncedFloat();
    }
    
    void WavefrontMaterialParser::readIllum() {
        int i;
        is >> i;
        if(i < 0 || i > 10) {
            parseError("reading illumination");
        }
        currentMaterial.illum = static_cast<Material::IlluminationMode>(i);
    }
    
    void WavefrontMaterialParser::readSpecularExponent() {
        currentMaterial.specularExpon = readFloat();
    }
    
    
    void WavefrontMaterialParser::flushMaterial() {
        library->emplace(currentMatName,
                         std::make_shared<Material>(currentMaterial));
        currentMatName.clear();
        currentMaterial = Material();
    }
    
    Vec4 WavefrontMaterialParser::readVec() {
        auto pe = [this]() {
            this->parseError("While reading a Vec4");
        };
        FloatType a, b, c;
        is >> a;
        if(is.fail()) {
            pe();
        }
        is >> b;
        if(is.fail()) {
            pe();
        }
        is >> c;
        if(is.bad()) {
            pe();
        }
        return {a, b, c};
    }
    
    FloatType WavefrontMaterialParser::readTruncedFloat() {
        FloatType f;
        is >> f;
        if(f < 0 || f > 1.0) {
            parseError("Float out of bounds");
        }
        return f;
    }
    
    FloatType WavefrontMaterialParser::readFloat() {
        FloatType f;
        is >> f;
        return f;
    }
    
    void WavefrontMaterialParser::parseError(std::string msg) {
        std::stringstream ss;
        ss << "Error parsing material `" << fileName << "`, ";
        ss << " Error \"" << msg << "\" at char " << is.tellg();
        throw ParseError(ss.str());
    }
}
