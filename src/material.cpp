#include <material.hpp>

namespace NM {
    std::ostream& operator<<(std::ostream& os, const Material& m) {
        os << "{Material ";
        os << "amb: " << m.ambient;
        os << ", diff: " << m.diffuse;
        os << ", spec: " << m.specular;
        os << ", att: " << m.attunation;
        os << ", specExpon: " << m.specularExpon;
        os << "}";
        return os;
    }
}
