#pragma once

#include <errors.hpp>
#include <mat4.hpp>
#include <vec4.hpp>
#include <ray.hpp>
#include <triangle.hpp>
#include <drawable.hpp>
#include <wavefront_material_parser.hpp>
#include <vector>
#include <istream>
#include <string>
#include <iostream>
#include <memory>
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
            ssize_t wfToC(FaceElement::ElmType);
            PointVector points;
            PointVector normals;
            FaceList faces;
            using MtlPtr = std::shared_ptr<Material>;
            MtlPtr getMaterial(const std::string& key);
            MaterialLibrary materials;
        
        };
        
        friend struct WavefrontParser;
        
        struct Face {
            Triangle tri;
            Triangle normals;
            Face() :
            tri{{}, {}, {}},
            normals{{}, {}, {}},
            material{std::make_shared<Material>()} {}
            
            inline Face(const Triangle& coords) :
            tri{coords},
            normals{{},{},{}},
            material{std::make_shared<Material>()} {}
            
            inline Face(const Triangle& coords, const Triangle& norms) :
            tri{coords},
            normals{norms},
            material{std::make_shared<Material>()} {}
            
            inline Face(const Triangle& coords, const Triangle& norms,
                        const std::shared_ptr<Material> material) :
            tri{coords}, normals{norms}, material{material} {}
            
            Vec4 calcNormal(const RayIntersection& ray) const;
            std::shared_ptr<Material> material;
        };
       
        Model(const Model&) = default;
        Model(Model&&) = default;
        Model() = default;
        /**
         * @brief Write a new obj file to the stream.
         * This will include commenst, normals, and all other components.
         */
        void writeObj(std::ostream& stream) const;
        
        Triangle faceAt(size_t idx) const;
        
        
        Vec4 pointAtObjCoord(ssize_t idx) const;

        virtual RayIntersection checkIntersection(const Ray&) const override;

        virtual ~Model() override = default;
        
    private:
        std::vector<Face> faces;
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
