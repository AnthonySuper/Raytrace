#pragma once
#include <ray_result.hpp>
#include <box.hpp>
#include <string>

namespace NM {
    /**
     * @defgroup drawable Drawable
     * This is the group of all things we can daw inside the Raytracer.
     @{
     @}
     */
    
    /**
     * @brief Base class for anything that we can draw in the raytracer.
     * @ingroup drawable
     */
    class Drawable {
    public:
        /**
         * @brief Convert a drawable to a human-readable 3D form.
         *
         * This function is used for debugging and logging purposes only.
         * It should not neccisarily output a form which is a complete representation
         * of the drawable contained within.
         * It also should not be overly complicated to the point of being hard to read.
         * Remember, it needs to be human-readable.
         * @return a human-readable string identifying this drawable.
         */
        virtual std::string print() const;
        /**
         * @brief Obtain the 3D-space midpoint of a drawable.
         *
         * This would perhaps be better titled as the "center of mass" of an object.
         * @return the 3D space representing the center of mass of a drawable object.
         */
        virtual Vec4 midpoint() const;
        
        /**
         * @brief Expand the given Box to fit this entire object.
         *
         * This function is used in BVH construction to make the partitions.
         * As a result, it must expand the given box to entirely fix this object.
         * This incldues any outlying points.
         * @warning failure to properly expand the box will result in improper tracing!
         */
        virtual void expandToFit(Box&) const = 0;
        /**
         * @brief Attempt to intersect a ray with this drawable.
         */
        virtual bool intersects(RayResult& r) const;
        /**
         * @brief Swap auxillary information into this drawable.
         *
         * This should include the RayResult#surfaceNormal and the RayResult#material.
         * This function exists as assigning these attributes is too expensive to do
         * on every trace.
         */
        virtual void swapInfo(RayResult&) const = 0;
        /**
         * @brief virtual destructor to allow agnostic ownership pointers
         */
        virtual ~Drawable() = default;
    };
    
    std::ostream& operator<<(std::ostream& os, const Drawable& d);
    
}
