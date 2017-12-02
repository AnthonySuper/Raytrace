#include <box.hpp>

namespace NM {
    void Box::expandToFit(const NM::Vec4 &other) {

        min = pseudoMin(other, min);
        max = pseudoMax(other, max);
    }
    
    void Box::expandToFit(const Triangle& other) {
        expandToFit(other.a);
        expandToFit(other.b);
        expandToFit(other.c);
    }
    
    Box::Box(const Triangle &t) {
        max = pseudoMax(max, pseudoMax(t.c, pseudoMax(t.a, t.b)));
        min = pseudoMin(min, pseudoMin(t.c, pseudoMin(t.a, t.b)));
    }
    
    FloatType Box::surfaceArea() const {
        auto l = max[0] - min[0];
        auto h = max[1] - min[1];
        auto w = max[2] - min[2];
        return 2 * (
            h * w + h * l + w * l
        );
    }
    
    
    std::ostream& operator<<(std::ostream& os, const Box& b) {
        return os << "{Box: " << b.min << ", " << b.max << "}";
    }
    
}
