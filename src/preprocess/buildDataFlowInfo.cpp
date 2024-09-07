#include "preprocess/buildDataFlowInfo.h"

DataFlowInfo::DataFlowInfo(int numBlocks, std::map<std::string, std::vector<json>>& table) : numBlocks(numBlocks) {
    buildVarkillAndUeVar(table);
    buildLiveOut(table);
}

void DataFlowInfo::buildVarkillAndUeVar(std::map<std::string, std::vector<json>>& table) {
    // Here we assume that the labels are unique
    for (const auto& [name, block] : table) {
        for (const auto& instr : block) {
            if (instr.find("args") != instr.end() && instr["args"].size() > 0) {
                for (const auto& arg : instr["args"]) {
                    std::string arg_str = arg.get<std::string>();
                    if (varkill[name].find(arg_str) == varkill[name].end()) {
                        uevar[name].insert(arg_str);
                    }
                }
            }
            if (instr.find("dest") != instr.end()) {
                std::string dest = instr["dest"].get<std::string>();
                varkill[name].insert(dest);
            }
        }
    }
}

void DataFlowInfo::buildLiveOut(std::map<std::string, std::vector<json>>& table) {
}