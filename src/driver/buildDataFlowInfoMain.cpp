#include "common/common.h"
#include "common/config.h"
#include "common/instr2str.h"
#include "common/logger.h"
#include "preprocess/buildCFG.h"
#include "preprocess/buildDataFlowInfo.h"

int main() {
    try {
        Logger::getInstance().setLogLevel(LogLevel::DEBUG);
        const json program = parseJsonFromStdin();
        
        LOG_DEBUG("Starting data flow info building process");
        for (const auto& func : program["functions"]) {
            const auto blocks = buildBlocks(func["instrs"]);
            auto table = buildTable(blocks);
            insertTerminators(table);
            DataFlowInfo dataFlowInfo(table.size(), table);
            dataFlowInfo.printDataFlowInfo(table);
        }
        LOG_DEBUG("Finished data flow info building process");
        return 0;
    }
    catch (const std::exception& e) {
        LOG_ERROR(std::string("Error: ") + e.what());
        return 1;
    }
}