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
        bounds = {{}, {}};
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
        int readAxis = static_cast<int>(av);
        sortDrawablesOnAxis(iv, av);
        // Set our bestCost to something really high initially
        FloatType bestCost = std::numeric_limits<FloatType>::max();
        if(readAxis == 2) {
            auto& last = iv[iv.size() - 1];
        }
        size_t bestSplit = 0;
        for(int i = 1; i < iv.size() - 1; ++i) {
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
        auto printSpaces = [=]() {
            for(int i = 0; i < depth; ++i) {
                std::cout << "  ";
            }
        };
        if((end - start) < 4) {
            printSpaces();
            std::cout << "Leaf is too small to split, ignoring... " << std::endl;
            node.isLeaf = true;
            return;
        }
        printSpaces();
        std::cout << "Splitting inside " << start << "," << end;
        std::cout << " at depth " << depth << " and node " << node.nodeIndex;
        std::cout << std::endl;
        auto split = getBestPartition(drawables.begin() + start,
                                      drawables.begin() + end,
                                      node.bounds.surfaceArea());
        if(split == -1 || (end - start) < depth * 2 || (end - start) < 6) {
            printSpaces();
            std::cout << "Found a leaf at depth " << depth;
            std::cout << ", marking and returning...";
            std::cout << std::endl;
            node.isLeaf = true;
            return;
        }
        split += start;
        auto ri = node.rightIndex();
        auto li = node.leftIndex();
        printSpaces();
        std::cout << "Should split (" << start << "," << end << ")";
        std::cout << " at split " << split << std::endl;
        printSpaces();
        std::cout << "Generating left... (in index " << li << ")" << std::endl;
        buildRecursive(start,
                       split,
                       li,
                       depth + 1);

        printSpaces();
        std::cout << "Generating right... (in index " << ri << ")" << std::endl;
        buildRecursive(split,
                       end,
                       ri,
                       depth + 1);
    }
    
    void BonsaiTree::intersectStack(NM::RayResult & r) const {
        const auto& rr = r.originalRay;
        const auto& iv = r.invDir;
        auto distanceTo = [&](const Node* n) {
            return n->bounds.intersectOrInf(rr, iv);
        };
        
        using Tup = std::pair<const Node*, FloatType>;
        std::vector<Tup> stack;
        stack.reserve(15);
        const Node* node = &nodes[0];
        FloatType dist = distanceTo(node);
        auto loopBody = [&]() {
            if(dist >= r.distance) {
                intersectsSkipped.fetch_add(node->end - node->begin,
                                            std::memory_order_relaxed);
                return;
            }
            if(node->isLeaf) {
                checkLeaf(*node, r);
                return;
            }
            auto l = &nodes[node->leftIndex()];
            auto r = &nodes[node->rightIndex()];
            auto ld = distanceTo(l);
            auto rd = distanceTo(r);
            intersectsTested.fetch_add(2, std::memory_order_relaxed);
            if(ld < rd) {
                stack.emplace_back(r, rd);
                stack.emplace_back(l, ld);
            }
            else {
                stack.emplace_back(l, ld);
                stack.emplace_back(r, rd);
            }
        };
        while(true) {
            loopBody();
            if(stack.size() == 0) return;
            auto p = stack[stack.size() - 1];
            node = p.first;
            dist = p.second;
            stack.pop_back();
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
            ss << ", " << node.end - node.begin << " total\n";
            spaceOut();
            auto ourSize = static_cast<FloatType>(node.end) - node.begin;
            auto l = nodes[node.leftIndex()];
            auto leftPercent = (l.end - l.begin) / ourSize;
            ss << "    Left (" << leftPercent * 100 << "%):\n";
            toStringRecursive(ss, node.leftIndex(), depth + 1);
            spaceOut();
            auto r = nodes[node.rightIndex()];
            auto rightPercent = (r.end - r.begin) / ourSize;
            ss << "    Right (" << rightPercent*100 << "%):\n";
            toStringRecursive(ss, node.rightIndex(), depth + 1);
            spaceOut();
            ss << "}\n";
        }
    }
    
    std::ostream& operator<<(std::ostream& os , const BonsaiTree& t) {
        return os << t.toString();
    }
}
