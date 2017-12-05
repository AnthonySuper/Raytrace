#include <ray_result.hpp>

namespace NM {
    
    bool RayResult::swapDistance(double newDist,
                                 const Drawable *ptr) {
        if(betterDistance(newDist)) {
            distance = newDist;
            drawable = ptr;
            return true;
        }
        return false;
    }
}
