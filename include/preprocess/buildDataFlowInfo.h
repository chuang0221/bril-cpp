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
    const CFG& cfg;
    std::unordered_map<std::string, std::set<std::string>> varkill;
    std::unordered_map<std::string, std::set<std::string>> uevar;
    std::unordered_map<std::string, std::set<std::string>> liveout;
    std::unordered_map<std::string, std::set<std::string>> dom;

public:
    DataFlowInfo(const CFG& cfg);
    void buildVarkillAndUeVar();
    void buildLiveOut();
    void buildDom();
    void printDataFlowInfo() const;

private:
    template<typename T>
    static std::set<T> setUnion(const std::set<T>& set1, const std::set<T>& set2);

    template<typename T>
    static std::set<T> setIntersection(const std::set<T>& set1, const std::set<T>& set2);

    template<typename T>
    static std::set<T> setDifference(const std::set<T>& set1, const std::set<T>& set2);
};

#endif