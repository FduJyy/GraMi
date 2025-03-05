#pragma once

#include "graph.hpp"
#include "pattern.hpp"
#include <vector>
#include <memory>
#include <functional>

namespace grami {

class Miner {
public:
    struct MiningOptions {
        double minSupport;           // 最小支持度
        bool directed;               // 是否是有向图
        bool approximate;            // 是否使用近似算法
        double approxError;          // 近似误差
        int maxPatternSize;         // 最大模式大小
        int maxDistance;            // 最大距离（用于模式挖掘）
    };
    
    Miner(const Graph& graph, const MiningOptions& options);
    
    // 挖掘频繁子图
    std::vector<Pattern> mineFrequentSubgraphs();
    
    // 挖掘频繁模式
    std::vector<Pattern> mineFrequentPatterns();
    
    // 设置回调函数用于报告进度
    void setProgressCallback(std::function<void(double)> callback);
    
private:
    const Graph& targetGraph;
    MiningOptions options;
    std::function<void(double)> progressCallback;
    
    // 内部辅助函数
    void mineSubgraphsRecursive(const Pattern& current, std::vector<Pattern>& results);
    void minePatternsRecursive(const Pattern& current, std::vector<Pattern>& results);
    bool isFrequent(const Pattern& pattern) const;
    void reportProgress(double progress);
    
    // 添加缺失的成员函数声明
    std::vector<Pattern> generateExtensions(const Pattern& current);
    std::vector<Pattern> generatePatternExtensions(const Pattern& current);
};

} // namespace grami 