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
    /**
     * @brief Bounding Volume Hierarchy acceleration structure
     *
     * This class is slightly misnamed---technically it should be called
     * a "BVHTree," as it does not implement the Bonsai algorithm at the
     * moment. However, it may in the future.
     *
     * A BVH is an acceleration structure for raytracing.
     * Essentially, it is a binary tree of boxes, each of which holds a 
     * subset of the primitives to draw.
     * These boxes are split recursively inside each other.
     * This lessens the time required to trace a given ray,
     * turning the running time for tracing through \f$ n \f$ primitives
     * from \f$ O(n) \f$ to \f$ O(\log(n)) \f$.
     * This asymptotic speedup is worth the extra work of construction.
     *
     */
    class BonsaiTree {
    public:
        using T = Drawable*;
        using ItemVector = std::vector<T>;
        using ItemIdx = ItemVector::size_type;
        /**
         * @brief How expensive do we consider intersecting a primitive?
         *
         * Used for the SAH.
         */
        static constexpr const FloatType IntersectCost =  5.0;
        /**
         * @brief How expensive do we consider traversing the tree?
         *
         * Used for the SAH.
         */
        static constexpr const FloatType TraversalCost = 1.0;
        
    protected:
        /**
         * @brief A node in the BVH.
         *
         * This holds some helper functions and information about
         * a given node, as well as the bounding box for that node.
         */
        struct Node {
            /**
             * @brief Is this node a leaf?
             */
            bool isLeaf = false;
            /**
             * @brief The start index into the parent Drawables
             * 
             * We keep our drawables as a binary tree in-place.
             * This is the start index of a node in said tree.
             */
            ItemIdx begin;
            /**
             * @brief The end index into the parent Drawables
             *
             * We keep our drawables as a binary tree in-place.
             * This is the end index of a given node in said tree.
             */
            ItemIdx end;
            /**
             * @brief A Bounding Box for all the primitives in this node
             *
             * We use this to test things later on.
             */
            Box bounds;

            /**
             * @brief Which node are we?
             * This is basically our index into the node list.
             */
            size_t nodeIndex;
            
            /**
             * Get the index into the node list of our left child.
             */
            inline size_t leftIndex() const {
                return 2 * nodeIndex + 1;
            }
            /**
             * Get the index into the node list of our right child.
             */
            inline size_t rightIndex() const {
                return 2 * nodeIndex + 2;
            }
            
            /**
             * Assuming that #start and #end are filled, this will
             * properly expand our box to fit all of the primitives.
             */
            void generateBounds(const ItemVector& b);
        };
       
        /**
         * @brief A list of the nodes in our binary tree.
         */
        std::vector<Node> nodes;

        /**
         * @brief The overall bounding box of the tree.
         * Used to save us time if the ray will never actually hit 
         * any primitive, which can sometimes happen.
         */
        Box boundingBox;
        /**
         * @brief list of the primitives we test against.
         *
         * This list is sorted in-place.
         *
         * @warning this list is non-owning, and failure to respect that
         * will probably result in segfaults!
         */
        std::vector<Drawable*> drawables;

        /**
         * @brief A running count of the intersect tests performed.
         *
         * This function is marked mutable as it is thread-safe.
         * Throughout the raytracer, all const functions are thread-safe.
         * Thus, we are allowed to mark this as mutable, as it doesn't
         * break that contract.
         */
        mutable std::atomic<unsigned long> intersectsTested;
        /**
         * @brief A running count of intersection tests skipped.
         *
         * See #intersectsTested for why it is marked mutable.
         */
        mutable std::atomic<unsigned long> intersectsSkipped;
        mutable std::atomic<unsigned long> treelessIntersects;
        
    public:
        constexpr static size_t threshold = 7;
        BonsaiTree();
        BonsaiTree(const BonsaiTree&);
        BonsaiTree(BonsaiTree&&) = default;
        
        /**
         * @brief use a fast stack-based traversal algorithm to perform
         * an intersection test on all primitives in this tree.
         *
         * @warning #partition must be called first!
         *
         * @todo Make this throw an exception if called before #partition
         */
        void intersectStack(RayResult&) const;
        /**
         * @brief use the recursive algorithm to find the intersection.
         *
         * @warning This is, in practice, slower than #intersectStack
         * which accomplishes the exact same thing.
         * It should only be used for exceptionally large trees for which
         * intersectStack breaks---but, AFAICT, that has never happened.
         */
        void intersectRecursive(RayResult&, 
                size_t nodeIdx = 0, 
                bool checkOuter = true) const;
        /**
         * @brief Add a drawable into the tree.
         *
         * @warning Calling this post-partition is undefined.
         */
        void add(Drawable* drawable);
        /**
         * @brief Partition this tree to use for tracing.
         *
         * @warning *must* be called before #intersectStack or
         * #intersectRecursive
         *
         * @todo fix this so it is *required* to be called before either of
         * those by making them throw an exception.
         */
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
