#include <face.hpp>

namespace NM {
    Face::Face(const Triangle& f,
               const Triangle& n,
               const MatPtr & m) :
    points{f},
    normals{n},
    mat{m}
    {}
    
    Face::Face(const Mat4& m,
               const Face& f) :
    points(m(f.points.a),
           m(f.points.b),
           m(f.points.c)),
    normals(m.normalTransform(f.normals.a),
            m.normalTransform(f.normals.b),
            m.normalTransform(f.normals.c)),
    mat(f.mat) {}
    
    
    RayIntersection Face::checkIntersection(const NM::Ray &r) const {
        auto ri = points.checkIntersection(r);
        return ri;
    }
    
    std::string Face::print() {
        std::stringstream ss;
        ss << "{Face: " << points << "," << normals << "}";
        return ss.str();
    }
    
    Vec4 Face::midpoint() const {
        return (1.0/3.0)*(points.a +
                          points.b +
                          points.c);
    }
    
    size_t Face::complexity() const {
        return 3;
    }
    
    void Face::expandToFit(NM::Box & b) const {
        b.expandToFit(points);
    }
    
    bool Face::intersects(NM::RayResult &r) const {
        auto rr = checkIntersection(r.originalRay);
        if(! rr) return false;
        auto dist = rr.getDistance();
        if(! r.betterDistance(dist)) return false;
        auto point = r.originalRay.position + (dist * r.originalRay.direction);
        return r.swapDistance(rr.getDistance(),
                              calcNormal(point),
                              mat.get());
    }
    
    Vec4 Face::calcNormal(const Vec4& point) const {
        auto bc = points.toBarycentric(point);
        auto norm = normals.a * bc[2] +
        normals.b * bc[0] +
        normals.c * bc[1];
        return norm.toUnit();
    }
}
