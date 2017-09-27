#pragma once
#include <thread>
#include <atomic>
#include <cmath>
#include <transformed_model.hpp>
#include <image.hpp>
#include <camera.hpp>
#include <ray_intersection.hpp>
#include <algorithm>
#include <iomanip>

namespace NM {
    class Scene {
    public:
        void addModel(const TransformedModel &);
        void addModel(TransformedModel&&);
        Image render(Camera, size_t, size_t, bool progress = true);
        inline const std::vector<TransformedModel>& getModels() const { return models; }
    private:
        std::vector<TransformedModel> models;
        RayIntersection getClosestIntersection(Ray& ray) const;
    };
}
