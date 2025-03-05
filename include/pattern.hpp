#pragma once

#include "graph.hpp"
#include <vector>
#include <unordered_set>
#include <memory>

namespace grami {

class Pattern {
public:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    mutable std::unordered_map<int, std::vector<int>> adjacencyList;
    
    // 模式匹配结果
    struct Match {
        std::unordered_map<int, int> vertexMapping;  // 模式顶点到目标图顶点的映射
        double support;  // 支持度
    };
    
    void addVertex(int id, const std::string& label);
    void addEdge(int source, int target, const std::string& label);
    void buildAdjacencyList() const;
    
    // 在目标图中查找所有匹配
    std::vector<Match> findMatches(const Graph& targetGraph) const;
    
    // 计算模式的支持度
    double calculateSupport(const Graph& targetGraph) const;
    
    // 检查是否是有效的模式
    bool isValid() const;
    
    // 获取模式的统计信息
    size_t getVertexCount() const { return vertices.size(); }
    size_t getEdgeCount() const { return edges.size(); }

private:
    // 添加缺失的私有成员函数声明
    void findMatchesRecursive(const Graph& targetGraph,
                            int patternVertex,
                            std::unordered_map<int, int>& currentMapping,
                            std::vector<Match>& matches) const;
                            
    bool isValidMapping(int patternVertex,
                       int targetVertex,
                       const std::unordered_map<int, int>& currentMapping,
                       const Graph& targetGraph) const;
};

} // namespace grami 