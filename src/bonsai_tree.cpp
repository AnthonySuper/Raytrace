#include <bonsai_tree.hpp>

namespace NM {

    BonsaiTree::BonsaiTree(const Box& b) : 
        boundingBox(b) {}


    void BonsaiTree::add(Drawable *drawable) {
        if(greater != nullptr) {
            throw std::runtime_error("Added child to completed drawable tree");
        }
        drawables.push_back(drawable);
    }

    static void sortDrawablesOnAxis(std::vector<Drawable*> &dr, Vec4::Axis ax) {
        std::sort(dr.begin(), 
                dr.end(),
                [&](Drawable *a, Drawable *b) {
                const Vec4& aMid = a->midpoint();
                const Vec4& bMid = b->midpoint();
                return (aMid[ax] < bMid[ax]);
                });
    }

    static FloatType largestWidth(std::vector<Drawable*> &dr, Vec4::Axis ax) {
        auto diff = dr[0]->midpoint()[ax] - dr[dr.size() - 1]->midpoint()[ax];
        return std::abs(diff);
    }

    static FloatType midpointAxis(const std::vector<Drawable*> &dr, 
            Vec4::Axis ax,
            FloatType width) {
        return dr[0]->midpoint()[ax] + (width / 2.0);
    }
    
    BonsaiTree::BonsaiTree() :
    boundingBox{{0, 0, 0}, {0, 0, 0}},
    greater(nullptr),
    less(nullptr)
    {}
    
    BonsaiTree::BonsaiTree(const BonsaiTree& bt) :
    boundingBox(bt.boundingBox), drawables(bt.drawables) {
        if(bt.greater) {
            greater = std::unique_ptr<BonsaiTree>(
                new BonsaiTree(*bt.greater)
                                                  );
            less = std::unique_ptr<BonsaiTree>(
                new BonsaiTree(*bt.less)
                                               );
        }
    }
    

    void BonsaiTree::partition(size_t concurrency) {
        if(drawables.size() < 10) {
            return;
        }
        auto byX = drawables;
        auto byY = drawables;
        auto byZ = drawables;
        sortDrawablesOnAxis(byX, Vec4::Axis::x);
        sortDrawablesOnAxis(byY, Vec4::Axis::y);
        sortDrawablesOnAxis(byZ, Vec4::Axis::z);
        auto xWidth = largestWidth(byX, Vec4::Axis::x);
        auto yWidth = largestWidth(byX, Vec4::Axis::y);
        auto zWidth = largestWidth(byX, Vec4::Axis::z);
        Vec4::Axis largestAxis = Vec4::Axis::x;
        auto currentLargest = xWidth;
        auto partitionPoint = midpointAxis(byX, Vec4::Axis::x, currentLargest);
        if(currentLargest < yWidth) {
            largestAxis = Vec4::Axis::y;
            currentLargest = yWidth;
            partitionPoint = midpointAxis(byY,
                    Vec4::Axis::y,
                    currentLargest);
        }
        if(currentLargest < zWidth) {
            largestAxis = Vec4::Axis::z;
            currentLargest = zWidth;
            partitionPoint = midpointAxis(byZ,
                    Vec4::Axis::z,
                    currentLargest);
        }
        constructChildren(largestAxis, partitionPoint, concurrency);
    }
    
    void BonsaiTree::expandBox() {
        for(const auto& d: drawables) {
            d->expandToFit(boundingBox);
        }
    }
    
    void BonsaiTree::intersectRecursive(RayResult& r) const {
        if(! boundingBox.intersect(r)) {
            return;
        }
        if(greater) {
            less->intersectRecursive(r);
            greater->intersectRecursive(r);
            return;
        }
        // has intersected
        for(const auto& drawable: drawables) {
           drawable->intersects(r);
        }
    }
    
    void BonsaiTree::reset() {
        greater = nullptr;
        less = nullptr;
        boundingBox = {{0, 0, 0}, {0, 0, 0}};
        drawables.clear();
    }
    
    void BonsaiTree::constructChildren(Vec4::Axis partAxis, 
                                       FloatType midPoint,
                                       size_t concurrency) {
        auto leftMax = boundingBox.max;
        leftMax[partAxis] = midPoint;
        auto leftBox = Box{
            boundingBox.min,
            leftMax
        };
        auto rightMin = boundingBox.min;
        rightMin[partAxis] = midPoint;
        auto rightBox = Box{rightMin,
            boundingBox.max
        };
        less = std::unique_ptr<BonsaiTree>(new BonsaiTree(leftBox));
        greater = std::unique_ptr<BonsaiTree>(new BonsaiTree(rightBox));
        for(auto& drawable: drawables) {
            if(drawable->intersects(less->boundingBox)) {
                less->add(drawable);
            }
            if(drawable->intersects(greater->boundingBox)) {
                greater->add(drawable);
            }
        }
        auto ratio = static_cast<FloatType>(less->size()) / greater->size();
        if(ratio > 1.0) ratio = 1.0 / ratio;
        if(ratio < 0.1 ||
           size() - less->size() < 10 ||
           size() - greater->size() < 10) {
            // We have a bad cut, so just give up honestly.
            less = nullptr;
            greater = nullptr;
            return;
        }
        else {
            if(concurrency >= 2) {
                double nn = concurrency / 2.0;
                std::thread t1([&]() {
           
                    less->partition(std::ceil(nn));
                });
                std::thread t2([&]() {
                    greater->partition(std::floor(nn));
                });
                t1.join();
                t2.join();
            }
            else {
                less->partition();
                greater->partition();
            }
        }
    }
    
    void BonsaiTree::treeHealth(size_t &totalContained, size_t &totalLeaves) {
        if(greater) {
            greater->treeHealth(totalContained, totalLeaves);
            less->treeHealth(totalContained, totalLeaves);
        }
        else {
            totalContained += size();
            totalLeaves++;
        }
    }
}
