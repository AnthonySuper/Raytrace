#include <sphere.hpp>
#include <iostream>
#include <material.hpp>

namespace NM {
    
    Vec4 Sphere::midpoint() const {
        return position;
    }
    
    bool Sphere::intersects(RayResult& r) const {
        return intersectsInline(r);
    }
    
    void Sphere::expandToFit(NM::Box &b) const {
        Vec4 directional{
            radius,
            radius,
            radius
        };
        b.expandToFit(position - directional);
        b.expandToFit(position + directional);
    }
    
    std::string Sphere::print() const {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }
    
    void Sphere::swapInfo(NM::RayResult &r) const {
        r.surfaceNormal = (r.point() - position).toUnit();
        if(material) {
          r.material = *material;
        };
    }

    Vec4 Sphere::refractExit(const Ray & r) const {
        auto &point = r.position;
        auto &dir = r.direction;
        auto fac = 2 * (position - point).dot(dir);
        auto exit = point + (fac * r.direction);
        return exit;
    }
    
    
    
    std::ostream& operator<<(std::ostream& os, const Sphere& s) {
        os << "{Sphere: pos " << s.position << ", rad: " << s.radius;
        /*
        if(s.material) {
            const Material& mat = *(s.material.get());
            os << ", Material: " << mat;
        }
         */
        os << "}";
        return os;
    }
}
