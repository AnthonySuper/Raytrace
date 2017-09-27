#include <transformed_model.hpp>

namespace NM {
    
    /**
     Implementation of Ritter's algorithm, based on 
     https://gist.github.com/sanoonan/faa7dbc6ab93918d542e
     but not directly copied.
     */
    static Sphere getBound(Model *model, Mat4& transform) {
        using std::max;
        if(model == nullptr) {
            throw std::runtime_error("Null model while generating bounding sphere");
        }
        auto& v = model->getPoints();
        Vec4 pmin, pmax;
        pmin = pmax = (transform * v[0]);
        auto size = v.size();
        for(size_t i = 1; i < size; ++i) {
            auto current = transform * v[i];
            // GCC will almost 100% unroll this
            // or even vectorize it.
            // TODO: Make sure that it does so.
            for(int j = 0; j < 3; ++j) {
                if(pmin[j] > current[j]) {
                    pmin[j] = current[j];
                }
                if(pmax[j] < v[i][j]) {
                    pmax[j] = current[j];
                }
            }
        }
        Vec4 diff = pmax - pmin;
        FloatType maxDiff = max(diff[0], max(diff[1], diff[2]));
        Vec4 center = (pmax + pmin) * 0.5;
        FloatType radius = maxDiff / 2.0;
        FloatType sqRadius = radius*radius;
        for(const auto& vec: v) {
            const auto current = (transform * vec);
            // Point from here to the center of our circle
            Vec4 tc = (current - center);
            FloatType sqDist = (tc[0]*tc[0] +
                                tc[1]*tc[1] +
                                tc[2]*tc[2]);
            // It's outside the circle
            if(sqDist > sqRadius) {
                // make the circle bigger by this much
                FloatType dist = std::sqrt(sqDist);
                // So here's the radius difference.
                FloatType diff = dist - radius;
                // First, we increase the radius by half of this, plus some epsilon
                radius += (diff / 2) + 0.0001;
                // Then we move the center the over by half as well
                center += (tc * (diff/2) + 0.0001);
            }
        }
        return {radius, center};
    }
    
    TransformedModel::TransformedModel(std::shared_ptr<Model> modelPtr,
                                       Mat4 mat) :
    model(modelPtr),
    transform(mat),
    boundingSphere(getBound(modelPtr.get(), mat))
    {}
    
    RayIntersection TransformedModel::checkIntersection(const NM::Ray &ray) const {
        auto r = boundingSphere.checkIntersection(ray);
        if(! r) {
            return {};
        }
        RayIntersection current;
        FloatType dist = 0;
        for(int i = 0; i < model->facesSize(); ++i) {
            Triangle t = model->faceAt(i);
            t.apply(transform);
            auto test = t.checkIntersection(ray);
            if(! test) {
                continue;
            }
            FloatType d = (test.point() - ray.position).magnitude();
            if(d > dist) {
                d = dist;
                current = test;
            }
        }
        return current;
    }
    
    TransformedModel operator*(const Mat4& m, const TransformedModel& tm) {
        return {
            tm.getModel(),
            m * tm.getTransform()
        };
    }
    
    std::ostream& operator<<(std::ostream& os, const TransformedModel &m) {
        return os << "{TransformedModel: " << m.getTransform() << ", " << m.getModel() << std::endl;
    }
   
    
}
