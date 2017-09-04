#pragma once

#include <vector>
#include <istream>
#include <string>
#include <errors.hpp>
#include <mat4.hpp>
#include <vec4.hpp>

namespace NM {
    class Model {
    public:
        using PointVector = std::vector<Vec4>;
       
        using size_type = ssize_t;

        struct FaceElement {
            size_type coordIdx;
            size_type texIdx;
            size_type normIdx;
            using ElmType = size_type;
        };
        
        using FaceDescriptor = std::array<FaceElement, 3>;

        static Model fromStream(std::istream& stream);
        Model(const Model&) = default;
        Model(Model&&) = default;
        Model() = default;
        void writeObj(std::ostream& stream) const;
        
    private:
        PointVector points;
        PointVector normals;
        std::vector<FaceDescriptor> faces;
        
        
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
