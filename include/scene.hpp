#pragma once
#include <model.hpp>
#include <camera.hpp>
#include <image.hpp>
#include <transformed_drawable.hpp>
#include <bonsai_tree.hpp>
#include <light.hpp>
#include <atomic>
#include <thread>

namespace NM {
    class Scene {
    public:
        constexpr static bool concurrent = true;
        constexpr static bool shouldLog = true;
        Vec4 ambient;
        void addObject(const Sphere &);
        void addObject(const Light &);
        RayIntersection traceIntersection(const Ray& in) const;
        void intersect(RayResult &) const;
        void colorize(RayResult& rr,
                      Vec4 &color,
                      const Vec4& refAt = {1.0, 1.0, 1.0},
                      unsigned int depth = 0) const;
        void render(Image& img, const Camera& camera) const;
        /**
         * TODO: A better design with dirty-checking and not this garbage
         */
        void finalize();
        unsigned int recursionDepth;
        std::vector<Face> faces;
    private:
        std::vector<Sphere> spheres;
        std::vector<Light> lights;
        friend std::ostream& operator<<(std::ostream&, const Scene&);
        size_t getConcurrency() const;
        Vec4 colorRay(Vec4 point,
                      Vec4 normal,
                      Vec4 toC,
                      const Light& light,
                      const Material& mtl) const;
        inline FloatType raySavings() const {
            auto p = bonsai.getIntersects();
            return (
                static_cast<FloatType>(p.first) / p.second
            ) * 100;
        }
        BonsaiTree bonsai;
    };
    
    std::ostream& operator<<(std::ostream&, const Scene&);
}
