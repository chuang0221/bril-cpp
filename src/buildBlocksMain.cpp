#include "buildBlocks.h"
#include "common.h"
#include "logger.h"

int main() {
    try {
        Logger::getInstance().setLogLevel(LogLevel::INFO);
        json program = parseJsonFromStdin();
        
        LOG_INFO("Starting blocks building process");
        for (const auto& func : program["functions"]) {
            auto blocks = buildBlocks(func["instrs"]);
            printBlocks(blocks, true);
        }
        LOG_INFO("Blocks building process completed");
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}