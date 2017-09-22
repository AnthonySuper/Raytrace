#pragma once

#include <vector>
#include <istream>
#include <string>
#include <errors.hpp>
#include <mat4.hpp>
#include <vec4.hpp>
#include <ray.hpp>
#include <triangle.hpp>

namespace NM {
    /**
     * @brief Represent a .obj file style model.
     *
     * This class follows the actual file format almost exactly.
     * Eventually, it will probably subclass some sort of "drawable" class.
     * I'm not 100% positive on that, I suppose.
     * Meh.
     */
    class Model {
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
        
        using FaceDescriptor = std::array<FaceElement, 3>;
        
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
        
        inline const PointVector& getPoints() const {
            return points;
        }
        
        inline const size_t triangleCount() const {
            return faces.size();
        }
        
        inline Vec4 vertexAt(size_type i) const {
            if(i == 0) {
                throw std::out_of_range("Triangles are one-indexed like OBJ files");
            }
            if(i > triangleCount()) {
                throw std::out_of_range("Index too high!");
            }
            if(i < -triangleCount()) {
                throw std::out_of_range("Index too low");
            }
            auto norm = (i - 1);
            if(norm < 0) {
                return points[points.size() + norm];
            }
            else {
                return points[norm];
            }
        }
        
        inline size_t facesSize() const { return faces.size(); }
        
        inline Triangle faceAt(size_t face) const {
            auto& t = faces.at(face);
            return {
                vertexAt(t[0].coordIdx),
                vertexAt(t[1].coordIdx),
                vertexAt(t[2].coordIdx)
            };
        }
        
        
    private:
        PointVector points;
        PointVector normals;
        FaceList faces;
        std::vector<std::string> topCommentBlock;
        
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
