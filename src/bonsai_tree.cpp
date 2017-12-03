#include <bonsai_tree.hpp>
#include <cassert>

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
    
    BonsaiTree::BonsaiTree() :
    boundingBox{{0, 0, 0}, {0, 0, 0}},
    greater(nullptr),
    less(nullptr)
    {}
    
    BonsaiTree::BonsaiTree(const BonsaiTree& bt) :
    boundingBox(bt.boundingBox), drawables(bt.drawables), intersectsTested(0),
    intersectsSkipped(0) {
        if(bt.greater) {
            greater = std::unique_ptr<BonsaiTree>(
                new BonsaiTree(*bt.greater)
                                                  );
            less = std::unique_ptr<BonsaiTree>(
                new BonsaiTree(*bt.less));
        }
    }
    

    void BonsaiTree::partition(size_t concurrency) {
        nodes.clear();
        nodes.resize(1);
        nodes[0].nodeIndex = 0;
        nodes[0].begin = 0;
        nodes[0].end = drawables.size();
        nodes[0].generateBounds(drawables);
        buildRecursive(0,
                       drawables.size(),
                       0,
                       0);
        
        auto leaves = std::count_if(nodes.begin(), nodes.end(), [](const Node& n) {
            return n.isLeaf;
        });
        std::cout << "tree has " << leaves << " leaves!" << std::endl;
        intersectsSkipped = 0;
        intersectsTested = 0;
    }
    
    void BonsaiTree::Node::generateBounds(const ItemVector& iv) {
        bounds = Box{};
        for(auto i = begin; i < end; ++i) {
            iv[i]->expandToFit(bounds);
        }
    }
    
    /**
     * Get the cost of a partition based on nodes
     * Returns (partition_index, cost)
     */
    using PartitionPair = std::pair<size_t, FloatType>;
    PartitionPair partitionOnAxis(BonsaiTree::ItemVector &iv,
                                  Vec4::Axis av,
                                  FloatType parentArea) {

        constexpr static auto boxes_to_try = 10;
        int readAxis = static_cast<int>(av);
        sortDrawablesOnAxis(iv, av);
        // Set our bestCost to something really high initially
        FloatType bestCost = std::numeric_limits<FloatType>::max();
        if(readAxis == 2) {
            auto& last = iv[iv.size() - 1];
        }
        auto stride = iv.size() / boxes_to_try;
        if(stride == 0) return {-1, -1};
        size_t bestSplit = 0;
        for(int i = stride + 1; i < iv.size() - 1; i += stride) {
            Box leftBox;
            Box rightBox;
            for(int j = 0; j < iv.size(); ++j) {
                if(j < i) {
                    iv[j]->expandToFit(leftBox);
                }
                else {
                    iv[j]->expandToFit(rightBox);
                }
            }
            // Cool, now find the cost:
            FloatType blbp = (leftBox.surfaceArea() / parentArea) * i;
            FloatType brbp = (rightBox.surfaceArea() / parentArea) * (iv.size() - i);
            FloatType cost = BonsaiTree::TraversalCost +
            BonsaiTree::IntersectCost * (blbp + brbp);
            if(cost < bestCost) {
                bestSplit = i;
                bestCost = cost;
            }
        }
        if(bestSplit == 0 || bestSplit == iv.size()) {
            throw std::runtime_error("Rip your heuristics lul");
        }
        return {bestSplit, bestCost};
    }
    
    /**
     * Find the best partition using the Surface Area Hueristic
     * Fairly complicated stuff, eh
     */
    static ssize_t getBestPartition(BonsaiTree::ItemVector::iterator begin,
                                   BonsaiTree::ItemVector::iterator end,
                                   FloatType parentArea) {
        BonsaiTree::ItemVector onX(begin, end),
            onY(begin, end),
            onZ(begin, end);
        auto bestX = partitionOnAxis(onX,
                                     Vec4::Axis::x,
                                     parentArea);
        auto bestY = partitionOnAxis(onY,
                                     Vec4::Axis::y,
                                     parentArea);
        auto bestZ = partitionOnAxis(onZ,
                                     Vec4::Axis::z,
                                     parentArea);
        auto bestCost = bestX.second;
        auto bestPart = bestX.first;
        BonsaiTree::ItemVector *bestSorted = &onX;
        if(bestCost > bestY.second) {
            bestCost = bestY.second;
            bestPart = bestY.first;
            bestSorted = &onY;
        }
        if(bestCost > bestZ.second) {
            bestCost = bestZ.second;
            bestPart = bestZ.first;
            bestSorted = &onZ;
        }
        auto leafCost = (end - begin)*BonsaiTree::IntersectCost;
        // If the best cost is worse than traversing everything
        // Give up:
        if(bestCost > leafCost) {
            return -1;
        }
        std::copy(bestSorted->begin(), bestSorted->end(), begin);
        BonsaiTree::ItemVector testVec(begin, end);
        if(testVec != *bestSorted) {
            throw std::runtime_error("Sort breaks bruh");
        }
        if(bestPart == 0 || bestPart == onX.size()) {
            throw std::runtime_error("Null partitions must be bad");
        }
        return bestPart;
    }
    
    void BonsaiTree::buildRecursive(ItemIdx start,
                                    ItemIdx end,
                                    size_t nodeIdx,
                                    size_t depth) {
        if(nodeIdx >= nodes.size()) {
            nodes.resize(nodeIdx + 1);
        }
        auto &node = nodes.at(nodeIdx);
        node.nodeIndex = nodeIdx;
        node.begin = start;
        node.end = end;
        node.generateBounds(drawables);
        auto sa = node.bounds.surfaceArea();
        auto printSpaces = [=]() {
            for(int i = 0; i < depth; ++i) {
                std::cout << "  ";
            }
        };
        if((end - start) < 6) {
            /*
            printSpaces();
            std::cout << "Leaf is too small to split, ignoring... " << std::endl;
            */
            node.isLeaf = true;
            return;
        }
        /*
        printSpaces();
        std::cout << "Splitting inside " << start << "," << end;
        std::cout << " at depth " << depth << " and node " << node.nodeIndex;
        std::cout << std::endl;
        */
        auto split = getBestPartition(drawables.begin() + start,
                                      drawables.begin() + end,
                                      node.bounds.surfaceArea());
        if(split == -1) {
            /*
            printSpaces();
            
            std::cout << "Found a leaf at depth " << depth;
            std::cout << ", marking and returning...";
            std::cout << std::endl;
             */
            node.isLeaf = true;
            return;
        }
        split += start;
        auto ri = node.rightIndex();
        auto li = node.leftIndex();
        /*
        printSpaces();
        std::cout << "Should split (" << start << "," << end << ")";
        std::cout << " at split " << split << std::endl;
        printSpaces();
        std::cout << "Generating left... (in index " << li << ")" << std::endl;
        */
        buildRecursive(start,
                       split,
                       li,
                       depth + 1);
        /*
        assert((
            sa != nodes[li].bounds.surfaceArea()
        ));
         */
        /*
        printSpaces();
        std::cout << "Generating right... (in index " << ri << ")" << std::endl;
        */
        buildRecursive(split,
                       end,
                       ri,
                       depth + 1);
        /*
        assert((
            sa != nodes[ri].bounds.surfaceArea() * 1.1
        ));
         */
    }
    
    void BonsaiTree::intersectStack(NM::RayResult & r) const {
        constexpr FloatType FLOATMAX = std::numeric_limits<FloatType>::max();
        treelessIntersects.fetch_add(drawableSize(),
                                     std::memory_order_relaxed);
        const auto rr = r.originalRay;
        const auto iv = r.invDir;
        auto distanceTo = [=](const Node* n) -> FloatType {
            auto t = n->bounds.intersectDist(rr, iv);
            if(t.first < 0) {
                if(t.second < 0) return -1.0;
                else return 0.0;
            }
            else {
                return t.first;
            }
        };
        constexpr size_t STACK_SIZE = 70;
        struct Tup {
            const Node* first;
            FloatType second;
        };
        using Stack = std::array<Tup, STACK_SIZE>;
        Stack stack;
        size_t stackPtr = 0;
        auto push = [&](const Node *n, FloatType t) {
            if(stackPtr + 1 >= STACK_SIZE) {
                throw std::runtime_error("Stack overflow");
            }
            stack[stackPtr].first = n;
            stack[stackPtr].second = t;
            stackPtr++;
        };
        auto pop = [&]() {
            if(stackPtr == 0) {
                throw std::runtime_error("Stack Underflow!");
            }
            auto tmp = stack[stackPtr - 1];
            stackPtr--;
            return tmp;
        };
        auto empty = [&]() {
            return stackPtr == 0;
        };
        size_t itested = 0;
        auto d = distanceTo(&nodes[0]);
        push(&nodes[0], d);
        while(! empty()) {
            auto t = pop();
            FloatType dist = t.second;
            const Node* n = t.first;
            if(dist >= r.distance) {
                continue;
            }
            if(n->isLeaf) {
                for(auto i = n->begin; i < n->end; ++i) {
                    drawables[i]->intersects(r);
                    
                }
                continue;
            }
            auto ln = &nodes[n->leftIndex()];
            auto rn = &nodes[n->rightIndex()];
            auto ld = distanceTo(ln);
            auto rd = distanceTo(rn);
            if(ld >= 0 && rd >= 0) {
                if(ld < rd) {
                    push(rn, rd);
                    push(ln, ld);
                }
                else {
                    push(ln, ld);
                    push(rn, rd);
                }
            }
            else if(ld >= 0) {
                push(ln, ld);
            }
            else if(rd >= 0) {
                push(rn, rd);
            }
        }
    }
    
    void BonsaiTree::checkLeaf(const BonsaiTree::Node &n, RayResult &r) const {
        for(auto i = n.begin; i < n.end; ++i) {
            static_cast<Sphere*>(drawables[i])->intersects(r);
        }
        intersectsTested.fetch_add(n.end - n.begin,
                                   std::memory_order_relaxed);
    }
    
    void BonsaiTree::expandBox() {
        for(const auto& d: drawables) {
            d->expandToFit(boundingBox);
        }
    }
    
    void BonsaiTree::intersectRecursive(RayResult& r, size_t nodeIdx, bool cr) const {
        intersectsTested++;
        const Node& node = nodes.at(nodeIdx);
        if(! node.bounds.intersect(r)) {
            std::atomic_fetch_add(&intersectsSkipped, node.end - node.begin);
            return;
        }
        if(node.isLeaf) {
            checkLeaf(node, r);
            return;
        }
        const auto& li = node.leftIndex();
        const auto& ri = node.rightIndex();
        intersectRecursive(r, li, true);
        intersectRecursive(r, ri, true);
        
    }
    
    void BonsaiTree::reset() {
        greater = nullptr;
        less = nullptr;
        boundingBox = {{0, 0, 0}, {0, 0, 0}};
        drawables.clear();
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
    
    std::string BonsaiTree::toString() const {
        std::stringstream ss;
        toStringRecursive(ss, 0, 0);
        return ss.str();
    }
    
    void BonsaiTree::toStringRecursive(std::ostream &ss,
                                       size_t nodeIdx,
                                       size_t depth) const {
        auto spaceOut = [&]() {
            for(int i = 0; i < depth; ++i) {
                ss << "    ";
            }
        };
        auto& node = nodes[nodeIdx];
        spaceOut();
        if(node.isLeaf) {
            ss << "{Leaf (" << node.begin << "," << node.end;
            ss << "), " << node.end - node.begin << " total}";
            ss << std::endl;
        }
        else {
            ss << std::fixed;
            ss << std::setprecision(5);
            ss << "{Node (" << node.begin << "," << node.end << ")";
            ss << ", " << node.end - node.begin << " total, SA " << node.bounds.surfaceArea() << "\n";
            spaceOut();
            auto ourSize = static_cast<FloatType>(node.end) - node.begin;
            auto l = nodes[node.leftIndex()];
            auto leftPercent = (l.bounds.surfaceArea() / node.bounds.surfaceArea())*100;
            ss << "    Left (" << leftPercent << "%SA):\n";
            toStringRecursive(ss, node.leftIndex(), depth + 1);
            spaceOut();
            auto r = nodes[node.rightIndex()];
            auto rightPercent = (r.bounds.surfaceArea() / node.bounds.surfaceArea())*100;
            ss << "    Right (" << rightPercent << "% SA):\n";
            toStringRecursive(ss, node.rightIndex(), depth + 1);
            spaceOut();
            ss << "}\n";
        }
    }
    
    std::ostream& operator<<(std::ostream& os , const BonsaiTree& t) {
        return os << t.toString();
    }
}
