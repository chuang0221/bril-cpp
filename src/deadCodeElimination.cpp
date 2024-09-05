#include "deadCodeElimination.h"
#include "logger.h"

void deadCodeElimination(std::vector<std::vector<json>>& blocks, DCEConfig& config) {
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
        deadCodeElimination(blocks, config);
    }
    LOG_DEBUG("\n");
    return;
}