#include <drawable.hpp>

namespace NM {
    std::string Drawable::print() {
        return "{UnprintableDrawable}";
    }
    
    size_t Drawable::complexity() const {
        return 0;
    }
}
