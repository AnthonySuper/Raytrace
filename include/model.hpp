#pragma once

#include <vector>
#include <istream>
#include <string>
#include <errors.hpp>
#include <mat4.hpp>
#include <vec4.hpp>
#include <ray.hpp>
#include <iostream>

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
        void debugCompare(const Model&, std::ostream& output = std::cout);
        
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
