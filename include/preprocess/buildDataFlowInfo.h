#ifndef BUILD_DATA_FLOW_INFO_H
#define BUILD_DATA_FLOW_INFO_H

#include "common/common.h"
#include "common/logger.h"
#include "common/instr2str.h"
#include "preprocess/buildBlocks.h"
#include "preprocess/buildCFG.h"
#include <algorithm>
#include <set>

class DataFlowInfo {
private:
    int numBlocks;
    std::vector<std::string> insertOrder;
    std::unordered_map<std::string, std::set<std::string>> varkill;
    std::unordered_map<std::string, std::set<std::string>> uevar;
    std::unordered_map<std::string, std::set<std::string>> liveout;
public:
    DataFlowInfo(int numBlocks, const std::unordered_map<std::string, std::vector<json>>& table, const std::vector<std::string>& insertOrder);
    void buildVarkillAndUeVar(const std::unordered_map<std::string, std::vector<json>>& table);
    void buildLiveOut(const std::unordered_map<std::string, std::vector<json>>& table);
    void printDataFlowInfo(const std::unordered_map<std::string, std::vector<json>>& table);
};

template<typename T>
std::set<T> setUnion(const std::set<T>& set1, const std::set<T>& set2) {
    std::vector<T> result;
    std::set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), std::back_inserter(result));
    return std::set<T>(result.begin(), result.end());
}

template<typename T>
std::set<T> setIntersection(const std::set<T>& set1, const std::set<T>& set2) {
    std::vector<T> result;
    std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::back_inserter(result));
    return std::set<T>(result.begin(), result.end());
}

template<typename T>
std::set<T> setDifference(const std::set<T>& set1, const std::set<T>& set2) {
    std::vector<T> result;
    std::set_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), std::back_inserter(result));
    return std::set<T>(result.begin(), result.end());
}

#endif