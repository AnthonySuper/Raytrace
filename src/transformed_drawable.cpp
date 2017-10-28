#include <transformed_drawable.hpp>
#include <iostream>

namespace NM {
    

    TransformedDrawable::TransformedDrawable(std::shared_ptr<Drawable> drPtr,
                                       Mat4 mat) :
    drawable(drPtr),
    transform(mat),
    inverseTransform(mat.inverse())
    {
        if(! drawable) {
            throw std::runtime_error("Invalid model file");
        }
    }
    
    RayIntersection
    TransformedDrawable::checkIntersection(const NM::Ray &ray) const {
        Ray transformed = (inverseTransform * ray);
        RayIntersection intersect = drawable->checkIntersection(transformed);
        if(! intersect) return intersect;
        // Move the intersection point into world space
        Vec4 newPoint = transform * intersect.point();
        // Move the normal into world space as well
        Vec4 newNormal = transform.normalTransform(intersect.surfaceNormal().toUnit()).toUnit();
        //std::cout << intersect.surfaceNormal() << "\t" << newNormal << std::endl;
        return {
            newPoint,
            newNormal,
            ray,
            intersect.material
        };
        
    }
    
    TransformedDrawable operator*(const Mat4& m, const TransformedDrawable& tm) {
        return {
            tm.getDrawablePtr(),
            m * tm.getTransform()
        };
    }
    
    std::ostream& operator<<(std::ostream& os, const TransformedDrawable &m) {
        return os << "{TransformedDrawable: " << m.getTransform() << ", " << m.getDrawablePtr() << std::endl;
    }
   
    
}
