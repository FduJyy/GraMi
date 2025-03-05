#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

namespace grami {

class Vertex {
public:
    int id;
    std::string label;
    
    Vertex(int id, const std::string& label) : id(id), label(label) {}
};

class Edge {
public:
    int source;
    int target;
    std::string label;
    
    Edge(int source, int target, const std::string& label) 
        : source(source), target(target), label(label) {}
};

class Graph {
public:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::unordered_map<int, std::vector<int>> adjacencyList;
    bool directed;
    
    Graph() : directed(false) {}
    
    void addVertex(int id, const std::string& label);
    void addEdge(int source, int target, const std::string& label);
    void buildAdjacencyList();
    
    // 从文件加载图
    static std::unique_ptr<Graph> loadFromFile(const std::string& filename);
    
    // 获取图的统计信息
    size_t getVertexCount() const { return vertices.size(); }
    size_t getEdgeCount() const { return edges.size(); }
};

} // namespace grami 