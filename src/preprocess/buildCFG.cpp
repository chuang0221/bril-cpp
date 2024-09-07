#include "common/instr2str.h"
#include "preprocess/buildBlocks.h"
#include "preprocess/buildCFG.h"

std::map<std::string, std::vector<json>> buildTable(const std::vector<std::vector<json>>& blocks) {
    static int id = 0;
    std::map<std::string, std::vector<json>> table;
    for (const auto &block: blocks) {
        std::string name;
        if (block[0].find("label") != block[0].end()) {
            name = block[0]["label"];
            table[name] = std::vector<json>(block.begin() + 1, block.end());
        }
        else {
            name = "block_" + std::to_string(id++);
            table[name] = block;
        }
    }
    return table;
}

void insertTerminators(std::map<std::string, std::vector<json>>& table) {
    for (auto it = table.begin(); it != table.end(); ++it) {
        auto nextIt = next(it);
        auto& [name, block] = *it;

        if (block.empty() || !isTerminator(block.back())) {
            json terminator = (nextIt == table.end()) 
                ? json{{"op", "ret"}, {"args", json::array()}}
                : json{{"op", "jmp"}, {"labels", json::array({nextIt->first})}};
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