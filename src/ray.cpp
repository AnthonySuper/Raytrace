#include <ray.hpp>


namespace NM {
    std::ostream& operator<<(std::ostream& os, const Ray& ray) {
        return os << "{Ray: [" << ray.position << "," << ray.direction << "]";
    }
    
    bool operator==(const Ray& lhs, const Ray& rhs) noexcept {
        return lhs.direction == rhs.direction &&
        lhs.position == rhs.position;
    }
    
    Ray operator*(const Mat4& m, const Ray& r) {
        return {
            m * r.position,
            (m.normalTransform(r.direction))
        };
    }
}
