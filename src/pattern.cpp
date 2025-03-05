#include "pattern.hpp"
#include <algorithm>
#include <queue>

namespace grami {

void Pattern::addVertex(int id, const std::string& label) {
    vertices.emplace_back(id, label);
}

void Pattern::addEdge(int source, int target, const std::string& label) {
    edges.emplace_back(source, target, label);
}

void Pattern::buildAdjacencyList() const {
    adjacencyList.clear();
    for (const auto& edge : edges) {
        adjacencyList[edge.source].push_back(edge.target);
    }
}

std::vector<Pattern::Match> Pattern::findMatches(const Graph& targetGraph) const {
    std::vector<Match> matches;
    std::unordered_map<int, int> currentMapping;
    
    // 使用回溯算法查找所有可能的匹配
    findMatchesRecursive(targetGraph, 0, currentMapping, matches);
    
    return matches;
}

void Pattern::findMatchesRecursive(const Graph& targetGraph, 
                                 int patternVertex,
                                 std::unordered_map<int, int>& currentMapping,
                                 std::vector<Match>& matches) const {
    if (static_cast<size_t>(patternVertex) == vertices.size()) {
        // 找到一个完整的匹配
        Match match;
        match.vertexMapping = currentMapping;
        // 计算这个匹配的支持度
        match.support = 1.0 / targetGraph.getVertexCount();
        matches.push_back(match);
        return;
    }
    
    // 尝试将当前模式顶点映射到目标图的每个顶点
    for (const auto& targetVertex : targetGraph.vertices) {
        if (isValidMapping(patternVertex, targetVertex.id, currentMapping, targetGraph)) {
            currentMapping[patternVertex] = targetVertex.id;
            findMatchesRecursive(targetGraph, patternVertex + 1, currentMapping, matches);
            currentMapping.erase(patternVertex);
        }
    }
}

bool Pattern::isValidMapping(int patternVertex, int targetVertex,
                           const std::unordered_map<int, int>& currentMapping,
                           const Graph& targetGraph) const {
    // 检查标签是否匹配
    if (vertices[patternVertex].label != targetGraph.vertices[targetVertex].label) {
        return false;
    }
    
    // 检查与已映射顶点的边是否匹配
    for (const auto& edge : edges) {
        if (edge.source == patternVertex && currentMapping.count(edge.target)) {
            bool edgeExists = false;
            for (const auto& targetEdge : targetGraph.edges) {
                if (targetEdge.source == targetVertex &&
                    targetEdge.target == currentMapping.at(edge.target) &&
                    targetEdge.label == edge.label) {
                    edgeExists = true;
                    break;
                }
            }
            if (!edgeExists) return false;
        }
    }
    
    return true;
}

double Pattern::calculateSupport(const Graph& targetGraph) const {
    auto matches = findMatches(targetGraph);
    if (matches.empty()) return 0.0;
    
    // 计算支持度（使用匹配数量除以目标图顶点数）
    return static_cast<double>(matches.size()) / targetGraph.getVertexCount();
}

bool Pattern::isValid() const {
    // 检查是否至少有一个顶点
    if (vertices.empty()) return false;
    
    // 检查所有边的顶点是否有效
    for (const auto& edge : edges) {
        if (static_cast<size_t>(edge.source) >= vertices.size() || 
            static_cast<size_t>(edge.target) >= vertices.size()) {
            return false;
        }
    }
    
    return true;
}

} // namespace grami 