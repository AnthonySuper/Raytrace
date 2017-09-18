#include <ray.hpp>


namespace NM {
    std::ostream& operator<<(std::ostream& os, const Ray& ray) {
        return os << "{Ray: [" << ray.position << "," << ray.direction << "]";
    }
}
