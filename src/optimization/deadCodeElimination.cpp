#include "optimization/deadCodeElimination.h"
#include "common/logger.h"

void deadCodeEliminationGlobal(std::vector<std::vector<json>>& blocks) {
    // Delete the unused variables globally
    std::unordered_set<std::string> usedVars;
    // 1. Traverse the blocks to get all used variables
    for (auto &block : blocks) {
        for (auto &instr : block) {
            for (auto &arg : instr["args"]) {
                usedVars.insert(arg.get<std::string>());
            }
        }
    }
    // 2. Traverse the blocks to remove unused variables
    bool changed = false;
    for (auto &block : blocks) {
        std::vector<json> newBlock;
        for (auto &instr : block) {
            if (instr.find("dest") == instr.end() || usedVars.find(instr["dest"].get<std::string>()) != usedVars.end()) {
                LOG_DEBUG(instr.dump());
                newBlock.push_back(instr);
            }
        }
        if (block.size() != newBlock.size()) {
            changed = true;
        }
        block = newBlock;
    }
    if (changed) {
        deadCodeEliminationGlobal(blocks);
    }
    LOG_DEBUG("\n");
    return;
}

void deadCodeEliminationLocal(std::vector<json>& block) {
    // Delete the unused variables locally
    std::unordered_map<std::string, int> def2line;
    std::unordered_set<int> deletedLines;

    for (int i = 0; i < block.size(); i++) {
        for (auto &arg : block[i]["args"]) {
            if (def2line.find(arg) != def2line.end()) {
                // def-use pair, no need to delete
                def2line.erase(arg);
            }
        }

        if (block[i].find("dest") != block[i].end()) {
            std::string dest = block[i]["dest"].get<std::string>();
            if (def2line.find(dest) != def2line.end()) {
                // redefinition, delete the previous definition
                deletedLines.insert(def2line[dest]);
            }
            def2line[dest] = i;
        }
    }

    // 2. Delete the unused variables
    std::vector<json> newBlock;
    for (int i = 0; i < block.size(); i++) {
        if (deletedLines.find(i) == deletedLines.end()) {
            newBlock.push_back(block[i]);
        }
    }
    bool changed = block.size() != newBlock.size();
    block = newBlock;
    if (changed) {
        deadCodeEliminationLocal(block);
    }
    return;
}

void deadCodeElimination(std::vector<std::vector<json>>& blocks, DCEConfig& config) {
    if (config.enableGlobalDCE) {
        deadCodeEliminationGlobal(blocks);
    }
    if (config.enableLocalDCE) {
        for (auto &block : blocks) {
            deadCodeEliminationLocal(block);
        }
    }
    return;
}