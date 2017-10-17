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
        /**
         * What type do we use to list out all the verticies?
         */
        using PointVector = std::vector<Vec4>;
       
        using size_type = ssize_t;

        struct FaceElement {
            size_type coordIdx;
            size_type texIdx;
            size_type normIdx;
            using ElmType = size_type;
        };
        
       struct FaceDescriptor {
           std::shared_ptr<std::string> materialPtr;
           std::array<FaceElement, 3> elements;
           FaceDescriptor(const FaceDescriptor&) = default;
           FaceDescriptor(FaceDescriptor&&) = default;
           FaceDescriptor& operator=(FaceDescriptor&&) = default;
           FaceDescriptor() = default;
           inline FaceDescriptor(std::array<FaceElement, 3> el,
                                 std::unique_ptr<std::string> && e) :
           materialPtr{std::forward<typeof(e)>(e)}, elements{el} {}
           
        };
        
        using FaceList = std::vector<FaceDescriptor>;

        static Model fromStream(std::istream& stream);
        Model(const Model&) = default;
        Model(Model&&) = default;
        Model() = default;
        /**
         * @brief Write a new obj file to the stream.
         * This will include commenst, normals, and all other components.
         */
        void writeObj(std::ostream& stream) const;
        
        void debugCompare(const Model&, std::ostream& output = std::cout);
        
        inline const PointVector& getPoints() const {
            return points;
        }
        
        Triangle faceAt(size_t idx) const;
        
        inline size_t facesSize() const {
            return faces.size();
        }
        
        Vec4 pointAtObjCoord(ssize_t idx) const;

        virtual RayIntersection checkIntersection(const Ray&) const override;

        virtual ~Model() override = default;
        
    private:
        PointVector points;
        PointVector normals;
        FaceList faces;
        std::vector<std::string> topCommentBlock;
        MaterialLibrary materials;
        Triangle faceToTriangle(const FaceDescriptor&) const;
        void swapMaterial(const FaceDescriptor&, Material&) const;
        
        friend std::ostream& operator<<(std::ostream&, const Model&);
        
        friend Model operator*(const Mat4& mat, const Model&);
    };
    
    std::istream& operator>>(std::istream&, Model::FaceElement&);
    
    std::istream& operator>>(std::istream&, Model::FaceDescriptor&);
    
    std::ostream& operator<<(std::ostream&, const Model::FaceElement&);
    
    std::ostream& operator<<(std::ostream&, const Model::FaceDescriptor&);
    
    std::ostream& operator<<(std::ostream&, const Model&);
    Model operator*(const Mat4& mat, const Model&);
}
