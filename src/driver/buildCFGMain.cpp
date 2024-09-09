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
            CFG cfg(blocks);

            for (const auto& name : cfg.getInsertOrder()) {
                std::cout << "  " << name << '\n';
            }

            for (const auto& [name, succs] : cfg.getSuccessors()) {
                for (const auto& succ : succs) {
                    std::cout << "  " << name << "->" << succ << '\n';
                }
            }
            std::cout << "}" << '\n';

            LOG_INFO("Predecessors: ");
            const auto& predecessors = cfg.getPredecessors();
            for (const auto& name : cfg.getInsertOrder()) {
                std::cout << "  " << name << ": ";
                if (predecessors.find(name) != predecessors.end()) {
                    const auto& preds = predecessors.at(name);
                    for (const auto& pred : preds) {
                        std::cout << "  " << pred << " ";
                    }
                }
                else {
                    std::cout << "[]";
                }
                std::cout << '\n';
            }
        }
        
        LOG_INFO("CFG building process completed");
    } catch (const std::exception& e) {
        LOG_ERROR(std::string("Error: ") + e.what());
        return 1;
    }
    return 0;
}