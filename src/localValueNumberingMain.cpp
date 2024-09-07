#include "localValueNumbering.h"
#include "common.h"
#include "buildBlocks.h"
#include "logger.h"

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().setLogLevel(LogLevel::INFO);
        json program = parseJsonFromStdin();
        LOG_DEBUG("Starting local value numbering");
        for (auto& func : program["functions"]) {
            std::vector<std::vector<json>> blocks = buildBlocks(func["instrs"]);
            LOG_DEBUG("Before local value numbering");
            std::vector<std::string> args(argv + 1, argv + argc);
            LVNConfig* config = static_cast<LVNConfig*>(createPassConfig("LVN"));
            for (auto& arg : args) {
                if (arg == "-g") {
                    Logger::getInstance().setLogLevel(LogLevel::DEBUG);
                }
                if (arg == "-c") {
                    config->enableCommutative = true;
                }
                else if (arg == "-f") {
                    config->enableConstantFolding = true;
                }
                else if (arg == "-a") {
                    config->enableAlgebraicIdentity = true;
                }
            }
            localValueNumbering(blocks, *config);
            delete config;
            LOG_DEBUG("After local value numbering");
            func["instrs"] = flattenBlocks(blocks);
        }
        std::cout << program.dump(2) << '\n';
        LOG_DEBUG("Local value numbering finished");
    } catch (const std::exception& e) {
        LOG_ERROR(std::string("Error: ") + e.what());
        return 1;
    }
    return 0;
}