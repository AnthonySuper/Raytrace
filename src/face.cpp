#include <face.hpp>

namespace NM {
    Face::Face(const Triangle& f,
               const Triangle& n,
               const MatPtr & m,
               const Triangle& tex) :
    points{f},
    normals{n},
    mat{m},
    texture{tex}
    {}
    
    Face::Face(const Mat4& m,
               const Face& f) :
    points(m(f.points.a),
           m(f.points.b),
           m(f.points.c)),
    normals(m.normalTransform(f.normals.a),
            m.normalTransform(f.normals.b),
            m.normalTransform(f.normals.c)),
    mat(f.mat),
    texture(f.texture)
     {}
    
    
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
        return r.swapDistance(rr.getDistance(),
                              this);
    }
    
    Vec4 Face::calcNormal(const Vec4& bc) const {
        auto norm = normals.a * bc[2] +
        normals.b * bc[0] +
        normals.c * bc[1];
        return norm.toUnit();
    }
    
    Material Face::getMaterial(const Vec4& bc, const WavefrontMaterial& wf) const {
        Material m = wf.originalMaterial;
        auto uv = texture.a * bc[2] + texture.b * bc[0] + texture.c * bc[1];
        if(wf.m_kd) {
            m.diffuse = (m.diffuse.pairwiseProduct(calcTexture(wf.m_kd.get(), uv)));
        }
        return m;
    }
    
    void Face::swapInfo(NM::RayResult & r) const {
        Vec4 barycentric = points.toBarycentric(r.point());
        r.surfaceNormal = calcNormal(barycentric);
        if(mat) {
            r.material = getMaterial(barycentric, *mat);
            r.material.attunation = {0, 0, 0};
        }
        else {
            r.material = Material{};
        }
    }
    
    Vec4 Face::calcTexture(const WavefrontMaterial::ExtImage *m, const Vec4& uv) const {
        FloatType iU = fmod(m->width() * uv[0], m->width() - 1);
        if(iU < 0) iU += (m->width() - 1);
        FloatType iV = fmod(m->height() * uv[1], m->height() - 1);
        if(iV < 0) iV += (m->height() - 1);
        FloatType r = m->linear_atXY(iU, iV, 0, 0, 1.0);
        FloatType g = m->linear_atXY(iU, iV, 0, 1, 1.0);
        FloatType b = m->linear_atXY(iU, iV, 0, 2, 1.0);
        return (1.0 / 256.0)*Vec4{r, g, b};
    }
}
