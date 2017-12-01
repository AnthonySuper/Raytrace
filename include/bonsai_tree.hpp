#pragma once
#include <globals.hpp>
#include <drawable.hpp>
#include <box.hpp>
#include <thread>
#include <vector>
#include <sphere.hpp>

namespace NM {

    class BonsaiTree {
    protected:
        Box boundingBox;
        std::vector<Drawable*> drawables;
        std::unique_ptr<BonsaiTree> greater;
        std::unique_ptr<BonsaiTree> less;
        BonsaiTree(const Box&);
    public:
        BonsaiTree();
        BonsaiTree(const BonsaiTree&);
        BonsaiTree(BonsaiTree&&) = default;
        void intersectRecursive(RayResult&) const;
        void add(Drawable* drawable);
        void partition(size_t concurrency = 0);
        void expandBox();
        void reset();
        void treeHealth(size_t& totalContained, size_t& totalLeaves);
        inline std::vector<Drawable*>::size_type size() const {
            return drawables.size();
        }

    private:
        void constructChildren(Vec4::Axis partAxis,
                               FloatType midPoint,
                               size_t recursionLevel = 0);
    };
}
