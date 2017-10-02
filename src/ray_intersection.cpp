#include <ray_intersection.hpp>


namespace NM {
    void RayIntersection::compareExchange(const RayIntersection &other) {
        if(! intersecting) {
            *this = other;
            return;
        }
        if(! other) {
            // we're intersecting, they're not
            return;
        }
        if(other.dist < dist) {
            *this = other;
        }
    }
}
