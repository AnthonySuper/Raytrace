#include <ray_result.hpp>

namespace NM {
    
    void RayResult::swapDistance(double newDist, const Material *mtlPtr) {
        if(betterDistance(newDist)) {
            distance = newDist;
            material = mtlPtr;
        }
    }
}
