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
    boundingBox(bt.boundingBox), drawables(bt.drawables) {
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
        std::cout << "tree has " << leaves << "leaves!" << std::endl;
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
            FloatType blbp = leftBox.surfaceArea() / parentArea;
            FloatType costFL = (i * BonsaiTree::IntersectCost);
            FloatType brbp = rightBox.surfaceArea() / parentArea;
            FloatType costFR = (iv.size() - i) * BonsaiTree::IntersectCost;
            FloatType cost = BonsaiTree::TraversalCost +
            blbp*costFL + brbp*costFR;
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
        if(split == -1 || (end - start) < 4) {
            printSpaces();
            std::cout << "Found a leaf at depth " << depth;
            std::cout << ", marking and returning...";
            std::cout << std::endl;
            node.isLeaf = true;
            node.begin = start;
            node.end = start;
            return;
        }
        split += start;
        auto ri = node.rightIndex();
        auto li = node.leftIndex();
        printSpaces();
        std::cout << "Should split (" << start << "," << end << ")";
        std::cout << " at split " << split << std::endl;
        if(nodes.size() < ri) {
            nodes.resize(ri + 1);
        }
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
        using Tup = std::pair<Node, FloatType>;
        std::vector<Tup> stack;
        Node nextCheck = nodes[0];
        FloatType nextDist = nextCheck.bounds.intersectOrInf(rr, iv);
        bool keepGoing = true;
        // Gross horrible horrific explicit stack hackery
        // not safe for children
        auto t = [&]() {
            // If the current node has a bad distance,
            // then it can *never* be the one we intersect with
            // So basically, bail early
            if(! r.betterDistance(nextDist)) {
                keepGoing = false;
                return;
            }
            // If it's a leaf node
            if(nextCheck.isLeaf) {
                checkLeaf(nextCheck, r);
                return;
            }
            // Get left and right nodes
            auto& ln = nodes[nextCheck.leftIndex()];
            auto& rn = nodes[nextCheck.rightIndex()];
            auto ld = ln.bounds.intersectOrInf(rr, iv);
            auto rd = rn.bounds.intersectOrInf(rr, iv);
            // left is closer, check it next
            if(ld < rd) {
                stack.push_back({rn, rd});
                stack.push_back({ln, ld});
            }
            // Otherwise put right on the stack next
            else {
                stack.push_back({ln, ld});
                stack.push_back({rn, rd});
            }
        };
        do {
            t();
            if(stack.size() == 0) {
                break;
            }
            // get our next element from the stack
            auto r = stack[stack.size() - 1];
            stack.pop_back();
            nextCheck = r.first;
            nextDist = r.second;
        } while(keepGoing);
    }
    
    void BonsaiTree::checkLeaf(const BonsaiTree::Node &n, RayResult &r) const {
        for(auto i = n.begin; i < n.end; ++i) {
            drawables[i]->intersects(r);
        }
    }
    
    void BonsaiTree::expandBox() {
        for(const auto& d: drawables) {
            d->expandToFit(boundingBox);
        }
    }
    
    void BonsaiTree::intersectRecursive(RayResult& r, size_t nodeIdx, bool cr) const {
        const Node& node = nodes.at(nodeIdx);
        if(! node.bounds.intersect(r)) {
            return;
        }
        if(node.isLeaf) {
            checkLeaf(node, r);
            return;
        }
        const auto& li = node.leftIndex();
        const auto& ri = node.rightIndex();
        const auto& ln = nodes.at(li);
        const auto& rn = nodes.at(ri);
        const auto& rd = rn.bounds.intersectOrInf(r.originalRay, r.invDir);
        const auto& ld = ln.bounds.intersectOrInf(r.originalRay, r.invDir);
        if(ld < rd) {
            if(ld > r.distance) return;
            intersectRecursive(r, li, false);
            if(! (r.distance < rd))
                intersectRecursive(r, ri, false);
        }
        else {
            if(rd > r.distance) return;
            intersectRecursive(r, ri, false);
            if(! (r.distance < ld))
                intersectRecursive(r, li, false);
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
