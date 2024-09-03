#include "localValueNumbering.h"
#include "common.h"
#include "buildBlocks.h"
#include "logger.h"

int main(void) {
    try {
        Logger::getInstance().setLogLevel(LogLevel::INFO);
        const json program = parseJsonFromStdin();
        LOG_INFO("Starting local value numbering");
        for (auto& func : program["functions"]) {
            std::vector<std::vector<json>> blocks = buildBlocks(func["instrs"]);
            LOG_INFO("Before local value numbering");
            printBlocks(blocks, true);
            Config config(true, true, true);
            localValueNumbering(blocks, config);
            LOG_INFO("After local value numbering");
            printBlocks(blocks, true);
        }
        LOG_INFO("Local value numbering finished");
    } catch (const std::exception& e) {
        LOG_ERROR(std::string("Error: ") + e.what());
        return 1;
    }
    return 0;
}