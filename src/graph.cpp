#include "graph.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace grami {

void Graph::addVertex(int id, const std::string& label) {
    vertices.emplace_back(id, label);
}

void Graph::addEdge(int source, int target, const std::string& label) {
    edges.emplace_back(source, target, label);
}

void Graph::buildAdjacencyList() {
    adjacencyList.clear();
    for (const auto& edge : edges) {
        adjacencyList[edge.source].push_back(edge.target);
        if (!directed) {
            adjacencyList[edge.target].push_back(edge.source);
        }
    }
}

std::unique_ptr<Graph> Graph::loadFromFile(const std::string& filename) {
    auto graph = std::make_unique<Graph>();
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filename);
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        
        if (type == "v") {
            int id;
            std::string label;
            iss >> id >> label;
            graph->addVertex(id, label);
        }
        else if (type == "e") {
            int source, target;
            std::string label;
            iss >> source >> target >> label;
            graph->addEdge(source, target, label);
        }
        else if (type == "d") {
            // 设置图的方向性
            int isDirected;
            iss >> isDirected;
            graph->directed = (isDirected == 1);
        }
    }
    
    graph->buildAdjacencyList();
    return graph;
}

} // namespace grami 