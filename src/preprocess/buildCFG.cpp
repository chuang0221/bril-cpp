#include "common/instr2str.h"
#include "preprocess/buildBlocks.h"
#include "preprocess/buildCFG.h"
#include "common/logger.h"

CFG::CFG(const std::vector<std::vector<json>>& blocks) {
    buildTable(blocks);
    insertTerminators();
    computeSuccessorsAndPredecessors();
}

void CFG::buildTable(const std::vector<std::vector<json>>& blocks) {
    static int id = 0;
    for (const auto &block: blocks) {
        std::string name;
        if (!block.empty() && block[0].find("label") != block[0].end()) {
            name = block[0]["label"];
            table[name] = std::vector<json>(block.begin() + 1, block.end());
        }
        else {
            name = "block_" + std::to_string(id++);
            table[name] = block;
        }
        insertOrder.push_back(name);
    }
}

void CFG::insertTerminators() {
    for (size_t i = 0; i < insertOrder.size(); ++i) {
        auto& name = insertOrder[i];
        auto& block = table[name];

        if (block.empty() || !isTerminator(block.back())) {
            json terminator = (i == insertOrder.size() - 1) 
                ? json{{"op", "ret"}, {"args", json::array()}}
                : json{{"op", "jmp"}, {"labels", json::array({insertOrder[i+1]})}};
            block.push_back(std::move(terminator));
        }
    }
}

void CFG::computeSuccessorsAndPredecessors() {
    for (const auto& name : insertOrder) {
        const auto& block = table[name];
        const json& lastInstr = block.back();
        
        if (isTerminator(lastInstr)) {
            if (lastInstr["op"] != "ret") {
                for (const auto& succ : lastInstr["labels"]) {
                    successors[name].insert(succ.get<std::string>());
                    predecessors[succ.get<std::string>()].insert(name);
                }
            }
        } else {
            throw std::runtime_error(lastInstr["op"].get<std::string>() + " is not a terminator");
        }
    }
}

const std::unordered_map<std::string, std::vector<json>>& CFG::getTable() const {
    return table;
}

const std::vector<std::string>& CFG::getInsertOrder() const {
    return insertOrder;
}

const std::unordered_map<std::string, std::unordered_set<std::string>>& CFG::getSuccessors() const {
    return successors;
}

const std::unordered_map<std::string, std::unordered_set<std::string>>& CFG::getPredecessors() const {
    return predecessors;
}