#include "preprocess/buildBlocks.h"
#include "preprocess/buildCFG.h"
#include "common/common.h"
#include "common/logger.h"
#include "common/instr2str.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        Logger::getInstance().setLogLevel(LogLevel::INFO);
        const json program = parseJsonFromStdin();
        
        LOG_INFO("Starting CFG building process");
        
        for (const auto& func : program["functions"]) {
            std::cout << "digraph: " << func["name"] << " {" << '\n';
            const auto blocks = buildBlocks(func["instrs"]);
            auto table = buildTable(blocks);
            insertTerminators(table);

            for (const auto& [name, _] : table) {
                std::cout << "  " << name << '\n';
            }

            for (const auto& [name, block] : table) {
                const json succs = successors(block.back());
                for (const auto& succ : succs) {
                    std::string succ_str = succ.get<std::string>();
                    std::cout << "  " << name << "->" << succ_str << '\n';
                }
            }
            std::cout << "}" << '\n';
        }
        
        LOG_INFO("CFG building process completed");
    } catch (const std::exception& e) {
        LOG_ERROR(std::string("Error: ") + e.what());
        return 1;
    }
    return 0;
}