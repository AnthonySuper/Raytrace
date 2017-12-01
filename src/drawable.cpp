#include <drawable.hpp>

namespace NM {
    std::string Drawable::print() {
        return "{UnprintableDrawable}";
    }
    
    size_t Drawable::complexity() const {
        return 0;
    }

    Vec4 Drawable::midpoint() const {
        return {0, 0, 0};
    }

    bool Drawable::intersects(const Box& b) const {
        return true;
    }
    
    bool Drawable::intersects(NM::RayResult &r) const {
        return false;
    }
}
