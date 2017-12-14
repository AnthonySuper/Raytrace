#include <drawable.hpp>

namespace NM {
    std::string Drawable::print() const {
        return "{UnprintableDrawable}";
    }
    
    Vec4 Drawable::midpoint() const {
        return {0, 0, 0};
    }
    
    bool Drawable::intersects(NM::RayResult &r) const {
        return false;
    }
    
    std::ostream& operator<<(std::ostream& os, const Drawable& d) {
        return os << d.print();
    }
}
