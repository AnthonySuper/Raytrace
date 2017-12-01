#include <box.hpp>

namespace NM {
    void Box::expandToFit(const NM::Vec4 &other) {
        min = pseudoMin(other, min);
        max = pseudoMax(other, max);
        extent = max - min;
    }
    
    void Box::expandToFit(const Triangle& other) {
        expandToFit(other.a);
        expandToFit(other.b);
        expandToFit(other.c);
    }
    
    Box::Box(const Triangle &t) {
        max = pseudoMax(t.a, t.b);
        max = pseudoMax(max, t.c);
        min = pseudoMin(t.a, t.b);
        min = pseudoMin(min, t.c);
        extent = max - min;
    }
    
    
}
