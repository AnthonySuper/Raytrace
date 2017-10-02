#pragma once
#include <memory>
#include <model.hpp>
#include <sphere.hpp>
#include <cmath>
#include <ostream>
#include <triangle.hpp>

namespace NM {
    class TransformedModel {
    public:
        TransformedModel(std::shared_ptr<Model>,
                         Mat4 m = Mat4::id());
        TransformedModel(TransformedModel&&) = default;
        TransformedModel(const TransformedModel&) = default;
        inline const Mat4& getTransform() const { return transform; }
        inline std::shared_ptr<Model> getModel() const { return model; }
        RayIntersection checkIntersection(const Ray& ray) const;
    private:
        std::shared_ptr<Model> model;
        Mat4 transform;
        Sphere boundingSphere;
    };
    
    TransformedModel operator*(const Mat4&, TransformedModel&);
    std::ostream& operator<<(std::ostream&, const TransformedModel&);
}
