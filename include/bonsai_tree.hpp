#pragma once
#include <algorithm>
#include <globals.hpp>
#include <drawable.hpp>
#include <box.hpp>
#include <thread>
#include <vector>
#include <sphere.hpp>
#include <iomanip>
#include <atomic>
#include <memory>

namespace NM {

    class BonsaiTree {
    public:
        using T = Drawable*;
        using ItemVector = std::vector<T>;
        using ItemIdx = ItemVector::size_type;
        static constexpr const FloatType IntersectCost =  5.0;
        static constexpr const FloatType TraversalCost = 1.0;
        
    protected:
        struct Node {
            bool isLeaf = false;
            ItemIdx begin;
            ItemIdx end;
            Box bounds;
            size_t nodeIndex;
            
            inline size_t leftIndex() const {
                return 2 * nodeIndex + 1;
            }
            inline size_t rightIndex() const {
                return 2 * nodeIndex + 2;
            }
            
            void generateBounds(const ItemVector& b);
        };
       
        
        std::vector<Node> nodes;
        Box boundingBox;
        std::vector<Drawable*> drawables;
        std::unique_ptr<BonsaiTree> greater;
        std::unique_ptr<BonsaiTree> less;
        BonsaiTree(const Box&);
        mutable std::atomic<unsigned long> intersectsTested;
        mutable std::atomic<unsigned long> intersectsSkipped;
        mutable std::atomic<unsigned long> treelessIntersects;
        
    public:
        constexpr static size_t threshold = 7;
        BonsaiTree();
        BonsaiTree(const BonsaiTree&);
        BonsaiTree(BonsaiTree&&) = default;
        
        void intersectStack(RayResult&) const;
        void intersectRecursive(RayResult&, size_t nodeIdx = 0, bool checkOuter = true) const;
        void add(Drawable* drawable);
        void partition(size_t concurrency = 0);
        
        void expandBox();
        void reset();
        void treeHealth(size_t& totalContained, size_t& totalLeaves);
        inline std::vector<Drawable*>::size_type size() const {
            return drawables.size();
        }
        
        inline std::pair<unsigned int, unsigned int> getIntersects() const {
            return {intersectsTested, treelessIntersects};
        }
        
        inline size_t drawableSize() const {
            return drawables.size();
        }
        
        std::string toString() const;

    private:
        void buildRecursive(ItemIdx start,
                            ItemIdx end,
                            size_t nodeIdx,
                            size_t depth);
        void intersectChildren(RayResult& r) const;
        
        void checkLeaf(const Node&, RayResult&) const;
        
        void toStringRecursive(std::ostream& ss, size_t nodeIdx, size_t depth) const;
        
        
    };
    
    std::ostream& operator<<(std::ostream&, const BonsaiTree&);
}
