#ifndef BUILD_DATA_FLOW_INFO_H
#define BUILD_DATA_FLOW_INFO_H

#include "common/common.h"
#include "common/logger.h"
#include "common/instr2str.h"
#include "preprocess/buildBlocks.h"
#include "preprocess/buildCFG.h"

class DataFlowInfo {
private:
    int numBlocks;
    std::map<std::string, std::unordered_set<std::string>> varkill; // the variables that are defined in the block
    std::map<std::string, std::unordered_set<std::string>> uevar; // the variables that are used in the block but not redefined in the block yet
    std::map<std::string, std::unordered_set<std::string>> liveout; // the variables that are live at the end of the block
public:
    DataFlowInfo(int numBlocks, std::map<std::string, std::vector<json>>& table);
    void buildVarkillAndUeVar(std::map<std::string, std::vector<json>>& table);
    void buildLiveOut(std::map<std::string, std::vector<json>>& table);
};

#endif