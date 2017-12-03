#pragma once
#include <drawable.hpp>
#include <triangle.hpp>

namespace NM {
    class Face : public Drawable {
        using MatPtr = std::shared_ptr<Material>;
        Triangle points;
        Triangle normals;
        MatPtr mat;
        
    public:
        Face(const Triangle&,
             const Triangle&,
             const MatPtr& mat);
        Face(const Mat4&,
             const Face&);
        virtual RayIntersection checkIntersection(const Ray&) const override final;
        virtual std::string print() override final;
        virtual Vec4 midpoint() const override final;
        virtual size_t complexity() const override final;
        virtual void expandToFit(Box&) const override final;
        virtual bool intersects(RayResult& r) const override final;
        Vec4 calcNormal(const Vec4& ri) const;
        virtual ~Face() = default;
    };
}
