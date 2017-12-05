#pragma once
#include <drawable.hpp>
#include <triangle.hpp>
#include <wavefront_material.hpp>


namespace NM {
    class Face : public Drawable {
        using MatPtr = std::shared_ptr<WavefrontMaterial>;
        Triangle points;
        Triangle normals;
        MatPtr mat;
        Triangle texture;
        
    public:
        Face(const Triangle&,
             const Triangle&,
             const MatPtr& mat,
             const Triangle& texture = {{}, {}, {}});
        Face(const Mat4&,
             const Face&);
        virtual RayIntersection checkIntersection(const Ray&) const override final;
        virtual std::string print() override final;
        virtual Vec4 midpoint() const override final;
        virtual size_t complexity() const override final;
        virtual void expandToFit(Box&) const override final;
        virtual bool intersects(RayResult& r) const override final;
        virtual void swapInfo(RayResult&) const override final;
        Vec4 calcNormal(const Vec4& ri) const;
        Material getMaterial(const Vec4& barycentric, const WavefrontMaterial& wf) const;
        Vec4 toUv(const Vec4& barycentric) const;
        Vec4 calcTexture(const WavefrontMaterial::ExtImage* m, const Vec4& uv) const;
        virtual ~Face() = default;
    };
}
