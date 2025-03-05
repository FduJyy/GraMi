#include "miner.hpp"
#include <algorithm>
#include <queue>

namespace grami {

Miner::Miner(const Graph& graph, const MiningOptions& options)
    : targetGraph(graph), options(options) {}

std::vector<Pattern> Miner::mineFrequentSubgraphs() {
    std::vector<Pattern> results;
    
    // 从单个顶点开始挖掘
    for (const auto& vertex : targetGraph.vertices) {
        Pattern initial;
        initial.addVertex(0, vertex.label);
        initial.buildAdjacencyList();
        mineSubgraphsRecursive(initial, results);
    }
    
    return results;
}

void Miner::mineSubgraphsRecursive(const Pattern& current, std::vector<Pattern>& results) {
    if (static_cast<int>(current.getVertexCount()) > options.maxPatternSize) {
        return;
    }
    
    if (isFrequent(current)) {
        results.push_back(current);
        
        // 扩展当前模式
        std::vector<Pattern> extensions = generateExtensions(current);
        for (const auto& extension : extensions) {
            if (extension.isValid()) {
                extension.buildAdjacencyList();
                mineSubgraphsRecursive(extension, results);
            }
        }
    }
}

std::vector<Pattern> Miner::mineFrequentPatterns() {
    std::vector<Pattern> results;
    
    // 从单个顶点开始挖掘
    for (const auto& vertex : targetGraph.vertices) {
        Pattern initial;
        initial.addVertex(0, vertex.label);
        initial.buildAdjacencyList();
        minePatternsRecursive(initial, results);
    }
    
    return results;
}

void Miner::minePatternsRecursive(const Pattern& current, std::vector<Pattern>& results) {
    if (static_cast<int>(current.getVertexCount()) > options.maxPatternSize) {
        return;
    }
    
    if (isFrequent(current)) {
        results.push_back(current);
        
        // 扩展当前模式，考虑传递性关系
        std::vector<Pattern> extensions = generatePatternExtensions(current);
        for (const auto& extension : extensions) {
            if (extension.isValid()) {
                extension.buildAdjacencyList();
                minePatternsRecursive(extension, results);
            }
        }
    }
}

bool Miner::isFrequent(const Pattern& pattern) const {
    double support = pattern.calculateSupport(targetGraph);
    
    if (options.approximate) {
        // 使用近似算法
        return support >= (options.minSupport * (1 - options.approxError));
    }
    
    return support >= options.minSupport;
}

std::vector<Pattern> Miner::generateExtensions(const Pattern& current) {
    std::vector<Pattern> extensions;
    
    // 添加新顶点
    for (const auto& vertex : targetGraph.vertices) {
        Pattern newPattern = current;
        newPattern.addVertex(newPattern.getVertexCount(), vertex.label);
        
        // 添加边
        for (size_t i = 0; i < current.getVertexCount(); ++i) {
            if (options.directed) {
                newPattern.addEdge(i, newPattern.getVertexCount() - 1, "e");
            } else {
                newPattern.addEdge(i, newPattern.getVertexCount() - 1, "e");
                newPattern.addEdge(newPattern.getVertexCount() - 1, i, "e");
            }
        }
        
        extensions.push_back(newPattern);
    }
    
    return extensions;
}

std::vector<Pattern> Miner::generatePatternExtensions(const Pattern& current) {
    std::vector<Pattern> extensions;
    
    // 添加新顶点
    for (const auto& vertex : targetGraph.vertices) {
        Pattern newPattern = current;
        newPattern.addVertex(newPattern.getVertexCount(), vertex.label);
        
        // 添加边，考虑传递性关系
        for (size_t i = 0; i < current.getVertexCount(); ++i) {
            // 直接边
            if (options.directed) {
                newPattern.addEdge(i, newPattern.getVertexCount() - 1, "e");
            } else {
                newPattern.addEdge(i, newPattern.getVertexCount() - 1, "e");
                newPattern.addEdge(newPattern.getVertexCount() - 1, i, "e");
            }
            
            // 传递性边
            for (size_t j = 0; j < current.getVertexCount(); ++j) {
                if (i != j) {
                    newPattern.addEdge(i, j, "t");  // t表示传递性边
                }
            }
        }
        
        extensions.push_back(newPattern);
    }
    
    return extensions;
}

void Miner::setProgressCallback(std::function<void(double)> callback) {
    progressCallback = callback;
}

void Miner::reportProgress(double progress) {
    if (progressCallback) {
        progressCallback(progress);
    }
}

} // namespace grami 