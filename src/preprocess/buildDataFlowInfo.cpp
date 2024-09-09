#include "preprocess/buildDataFlowInfo.h"

DataFlowInfo::DataFlowInfo(int numBlocks, const std::unordered_map<std::string, std::vector<json>>& table, const std::vector<std::string>& insertOrder) 
    : numBlocks(numBlocks), insertOrder(insertOrder) {
    buildVarkillAndUeVar(table);
    buildLiveOut(table);
}

void DataFlowInfo::buildVarkillAndUeVar(const std::unordered_map<std::string, std::vector<json>>& table) {
    for (const auto& name : insertOrder) {
        const auto& block = table.at(name);
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

void DataFlowInfo::buildLiveOut(const std::unordered_map<std::string, std::vector<json>>& table) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& name : insertOrder) {
            const auto& block = table.at(name);
            std::set<std::string> liveout_new;
            const json succs = successors(block.back());
            for (const auto& succ : succs) {
                std::string succ_str = succ.get<std::string>();
                liveout_new = setUnion(liveout_new, setUnion(uevar[succ_str], setDifference(liveout[succ_str], setIntersection(liveout[succ_str], varkill[succ_str]))));
            }
            if (liveout_new != liveout[name]) {
                liveout[name] = liveout_new;
                changed = true;
            }
        }
    }
}

void DataFlowInfo::printDataFlowInfo(const std::unordered_map<std::string, std::vector<json>>& table) {
    for (const auto& name : insertOrder) {
        LOG_DEBUG("Block: " + name);
    }
    LOG_DEBUG("============================================");
    LOG_DEBUG("Varkill: " + std::to_string(varkill.size()));
    for (const auto& [name, set] : varkill) {
        LOG_DEBUG("Block: " + name);
        for (const auto& var : set) {
            LOG_DEBUG("Variable: " + var);
        }
    }
    LOG_DEBUG("============================================");
    LOG_DEBUG("UeVar: " + std::to_string(uevar.size()));
    for (const auto& [name, set] : uevar) {
        LOG_DEBUG("Block: " + name);
        for (const auto& var : set) {
            LOG_DEBUG("Variable: " + var);
        }
    }
    LOG_DEBUG("============================================");
    LOG_DEBUG("LiveOut: " + std::to_string(liveout.size()));
    for (const auto& [name, set] : liveout) {
        LOG_DEBUG("Block: " + name);
        for (const auto& var : set) {
            LOG_DEBUG("Variable: " + var);
        }
    }
    LOG_DEBUG("============================================");
}