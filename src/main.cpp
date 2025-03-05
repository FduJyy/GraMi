#include "graph.hpp"
#include "miner.hpp"
#include <iostream>
#include <string>
#include <cstring>

void printUsage() {
    std::cout << "用法: grami [选项]\n"
              << "选项:\n"
              << "  -f <文件>    输入图文件\n"
              << "  -s <数值>    最小支持度\n"
              << "  -t <0|1>     图类型 (0=无向, 1=有向)\n"
              << "  -p <0|1>     挖掘类型 (0=子图, 1=模式)\n"
              << "  -d <数值>    最大距离 (用于模式挖掘)\n"
              << "  -approxA <数值> 近似误差A\n"
              << "  -approxB <数值> 近似误差B\n"
              << "  -h           显示帮助\n";
}

int main(int argc, char* argv[]) {
    std::string filename;
    double minSupport = 0.0;
    bool directed = false;
    bool patternMining = false;
    int maxDistance = 0;
    bool approximate = false;
    double approxErrorA = 0.0;
    [[maybe_unused]] double approxErrorB = 0.0;
    
    // 解析命令行参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[++i];
        }
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            minSupport = std::stod(argv[++i]);
        }
        else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            directed = (std::stoi(argv[++i]) == 1);
        }
        else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            patternMining = (std::stoi(argv[++i]) == 1);
        }
        else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            maxDistance = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-approxA") == 0 && i + 1 < argc) {
            approxErrorA = std::stod(argv[++i]);
            approximate = true;
        }
        else if (strcmp(argv[i], "-approxB") == 0 && i + 1 < argc) {
            approxErrorB = std::stod(argv[++i]);
        }
        else if (strcmp(argv[i], "-h") == 0) {
            printUsage();
            return 0;
        }
    }
    
    if (filename.empty()) {
        std::cerr << "错误: 必须指定输入文件 (-f)\n";
        printUsage();
        return 1;
    }
    
    try {
        // 加载图
        auto graph = grami::Graph::loadFromFile(filename);
        
        // 设置挖掘选项
        grami::Miner::MiningOptions options;
        options.minSupport = minSupport;
        options.directed = directed;
        options.approximate = approximate;
        options.approxError = approxErrorA;
        options.maxPatternSize = 10;  // 默认最大模式大小
        options.maxDistance = maxDistance;
        
        // 创建挖掘器
        grami::Miner miner(*graph, options);
        
        // 设置进度回调
        miner.setProgressCallback([](double progress) {
            std::cout << "进度: " << (progress * 100) << "%\n";
        });
        
        // 执行挖掘
        std::vector<grami::Pattern> results;
        if (patternMining) {
            results = miner.mineFrequentPatterns();
        } else {
            results = miner.mineFrequentSubgraphs();
        }
        
        // 输出结果
        std::cout << "找到 " << results.size() << " 个模式:\n";
        for (size_t i = 0; i < results.size(); ++i) {
            std::cout << "模式 " << (i + 1) << ":\n";
            std::cout << "  顶点数: " << results[i].getVertexCount() << "\n";
            std::cout << "  边数: " << results[i].getEdgeCount() << "\n";
            std::cout << "  支持度: " << results[i].calculateSupport(*graph) << "\n\n";
        }
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
} 