#include <mat4.hpp>


namespace NM {
    std::ostream& operator<<(std::ostream& os, NM::Mat4 const & mat) {
        os << "{";
        for(int i = 0; i < 4; ++i) {
            os << "{";
            for(int k = 0; k < 4; ++k) {
                os << mat[i][k];
                if(k != 3) os << ", ";
            }
            os << "} ";
        }
        os << "}";
        return os;
    }
}
