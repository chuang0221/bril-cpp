#include "localValueNumbering.h"
#include "common.h"
#include "buildBlocks.h"
#include "logger.h"

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().setLogLevel(LogLevel::INFO);
        const json program = parseJsonFromStdin();
        LOG_INFO("Starting local value numbering");
        for (auto& func : program["functions"]) {
            std::vector<std::vector<json>> blocks = buildBlocks(func["instrs"]);
            LOG_INFO("Before local value numbering");
            printBlocks(blocks, true);
            std::vector<std::string> args(argv + 1, argv + argc);
            Config config(false, false, false);
            for (auto& arg : args) {
                if (arg == "-g") {
                    Logger::getInstance().setLogLevel(LogLevel::DEBUG);
                }
                if (arg == "-c") {
                    config.enableCommutative = true;
                }
                else if (arg == "-f") {
                    config.enableConstantFolding = true;
                }
                else if (arg == "-a") {
                    config.enableAlgebraicIdentity = true;
                }
            }
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