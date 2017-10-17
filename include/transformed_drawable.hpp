#pragma once
#include <memory>
#include <drawable.hpp>
#include <sphere.hpp>
#include <cmath>
#include <ostream>

namespace NM {
    class TransformedDrawable {
    public:
        TransformedDrawable(std::shared_ptr<Drawable>,
                         Mat4 m = Mat4::id());
        TransformedDrawable(TransformedDrawable&&) = default;
        TransformedDrawable(const TransformedDrawable&) = default;
        inline const Mat4& getTransform() const { return transform; }
        inline const Drawable& getDrawable()  const { return *drawable; }
        inline std::shared_ptr<Drawable> getDrawablePtr() const {
            return drawable;
        }
        RayIntersection checkIntersection(const Ray& ray) const;
    private:
        std::shared_ptr<Drawable> drawable;
        Mat4 transform;
        Mat4 inverseTransform;
    };
    
    TransformedDrawable operator*(const Mat4&, TransformedDrawable&);
    std::ostream& operator<<(std::ostream&, const TransformedDrawable&);
}
