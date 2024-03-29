#pragma once

#include <errors.hpp>
#include <mat4.hpp>
#include <vec4.hpp>
#include <ray.hpp>
#include <triangle.hpp>
#include <drawable.hpp>
#include <wavefront_material.hpp>
#include <wavefront_material_parser.hpp>
#include <face.hpp>
#include <vector>
#include <istream>
#include <string>
#include <iostream>
#include <memory>
#include <sstream>
#include <fstream>

namespace NM {
    /**
     * @brief Represent a .obj file style model.
     *
     * This class follows the actual file format almost exactly.
     * Eventually, it will probably subclass some sort of "drawable" class.
     * I'm not 100% positive on that, I suppose.
     * Meh.
     */
    class Model : public Drawable {
    public:
        static Model fromStream(std::istream&);
        
        struct WavefrontParser {
            
            static Model fromStream(std::istream&);
            
            Model toModel();
            
            using PointVector = std::vector<Vec4>;
            
            using size_type = ssize_t;
            
            struct FaceElement {
                size_type coordIdx;
                size_type texIdx;
                size_type normIdx;
                using ElmType = ssize_t;
            };
            struct FaceDescriptor : private std::array<FaceElement, 3> {
                using std::array<FaceElement, 3>::operator[];
                std::string materialName;
            };
            
            using FaceList = std::vector<FaceDescriptor>;
        private:
            Vec4 coordinateToPoint(FaceElement::ElmType);
            Vec4 coordinateToNormal(FaceElement::ElmType);
            Vec4 coordinateToTexture(FaceElement::ElmType);
            ssize_t wfToC(FaceElement::ElmType);
            PointVector points;
            PointVector normals;
            PointVector textures;
            FaceList faces;
            using MtlPtr = std::shared_ptr<WavefrontMaterial>;
            MtlPtr getMaterial(const std::string& key);
            WavefrontMaterialLibrary materials;
            
        };
        
        friend struct WavefrontParser;
        
        Model(const Model&) = default;
        Model(Model&&) = default;
        Model() = default;
        
        virtual std::string print() const override;
        virtual ~Model() override = default;
        virtual void swapInfo(RayResult&) const override final;
        virtual void expandToFit(Box& b) const override final;
        std::vector<Face> faces;
        
    private:
        WavefrontMaterialLibrary materials;
        friend std::ostream& operator<<(std::ostream&, const Model&);
        
    };
    
    std::istream& operator>>(std::istream&, Model::WavefrontParser::FaceElement&);
    
    std::istream& operator>>(std::istream&, Model::WavefrontParser::FaceDescriptor&);
    
    std::ostream& operator<<(std::ostream&,
                             const Model::WavefrontParser::FaceElement&);
    
    std::ostream& operator<<(std::ostream&,
                             const Model::WavefrontParser::FaceDescriptor&);
    
    std::ostream& operator<<(std::ostream&, const Model&);
}
