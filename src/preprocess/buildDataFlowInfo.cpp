#include "preprocess/buildDataFlowInfo.h"

DataFlowInfo::DataFlowInfo(const CFG& cfg) : cfg(cfg) {
    buildVarkillAndUeVar();
    buildLiveOut();
    buildDom();
}

void DataFlowInfo::buildVarkillAndUeVar() {
    const auto& table = cfg.getTable();
    for (const auto& name : cfg.getInsertOrder()) {
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

void DataFlowInfo::buildLiveOut() {
    const auto& table = cfg.getTable();
    const auto& successors = cfg.getSuccessors();
    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& name : cfg.getInsertOrder()) {
            if (successors.find(name) == successors.end()) {
                continue;
            }
            std::set<std::string> liveout_new;
            const auto& succs = successors.at(name);
            for (const auto& succ : succs) {
                liveout_new = setUnion(liveout_new, setUnion(uevar[succ], setDifference(liveout[succ], setIntersection(liveout[succ], varkill[succ]))));
            }
            if (liveout_new != liveout[name]) {
                liveout[name] = liveout_new;
                changed = true;
            }
        }
    }
}

void DataFlowInfo::buildDom() {
    const auto& insertOrder = cfg.getInsertOrder();
    const auto& predecessors = cfg.getPredecessors();
    dom[insertOrder[0]].insert(insertOrder[0]);
    std::set<std::string> all(insertOrder.begin(), insertOrder.end());
    for (size_t i = 1; i < insertOrder.size(); i++) {
        dom[insertOrder[i]] = all;
    }

    bool changed = true;
    while (changed) {
        changed = false;
        for (size_t i = 1; i < insertOrder.size(); i++) {
            if (predecessors.find(insertOrder[i]) == predecessors.end()) {
                continue;
            }
            const auto& preds = predecessors.at(insertOrder[i]);
            std::set<std::string> dom_new;
            dom_new.insert(insertOrder[i]);
            std::set<std::string> dom_intersection = all;
            for (const auto& pred: preds) {
                dom_intersection = setIntersection(dom_intersection, dom[pred]);
            }
            dom_new = setUnion(dom_new, dom_intersection);
            if (dom_new != dom[insertOrder[i]]) {
                dom[insertOrder[i]] = dom_new;
                changed = true;
            }
        }
    }
}

template<typename T>
std::set<T> DataFlowInfo::setUnion(const std::set<T>& set1, const std::set<T>& set2) {
    std::set<T> result = set1;
    result.insert(set2.begin(), set2.end());
    return result;
}

template<typename T>
std::set<T> DataFlowInfo::setIntersection(const std::set<T>& set1, const std::set<T>& set2) {
    std::set<T> result;
    std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), 
                          std::inserter(result, result.begin()));
    return result;
}

template<typename T>
std::set<T> DataFlowInfo::setDifference(const std::set<T>& set1, const std::set<T>& set2) {
    std::set<T> result;
    std::set_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), 
                        std::inserter(result, result.begin()));
    return result;
}

void DataFlowInfo::printDataFlowInfo() const {
    for (const auto& name : cfg.getInsertOrder()) {
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
    LOG_DEBUG("Dom: " + std::to_string(dom.size()));
    for (const auto& [name, set] : dom) {
        LOG_DEBUG("Block: " + name);
        for (const auto& var : set) {
            LOG_DEBUG("Dom: " + var);
        }
    }
    LOG_DEBUG("============================================");
}