#include "common/instr2str.h"
#include "preprocess/buildBlocks.h"
#include "preprocess/buildCFG.h"
#include "common/logger.h"

std::pair<std::unordered_map<std::string, std::vector<json>>, std::vector<std::string>> 
buildTable(const std::vector<std::vector<json>>& blocks) {
    // Here we assume that the labels are unique
    // TODO: handle the case where the labels are not unique
    static int id = 0;
    std::unordered_map<std::string, std::vector<json>> table;
    std::vector<std::string> insertOrder;

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
    return {table, insertOrder};
}

void insertTerminators(std::unordered_map<std::string, std::vector<json>>& table, const std::vector<std::string>& insertOrder) {
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

json successors(const json& instr) {
    if (isTerminator(instr)) {
        return (instr["op"] != "ret") ? instr["labels"] : json::array();
    }
    throw std::runtime_error(instr["op"].get<std::string>() + " is not a terminator");
}

json predecessors(const std::string& label, const std::unordered_map<std::string, std::vector<json>>& table, const std::vector<std::string>& insertOrder) {
    json preds;
    for (const auto& name : insertOrder) {
        const auto& block = table.at(name);
        const json succs = successors(block.back());
        if (std::find(succs.begin(), succs.end(), label) != succs.end()) {
            preds.push_back(name);
        }
    }
    return preds;
}