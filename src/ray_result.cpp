#include <ray_result.hpp>

namespace NM {
    
    bool RayResult::swapDistance(double newDist,
                                 const Vec4& sn,
                                 const Material *mtlPtr) {
        if(betterDistance(newDist)) {
            distance = newDist;
            material = mtlPtr;
            surfaceNormal = sn;
            return true;
        }
        return false;
    }
}
