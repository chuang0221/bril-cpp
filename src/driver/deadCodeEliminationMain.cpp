#include "optimization/deadCodeElimination.h"
#include "common/common.h"
#include "preprocess/buildBlocks.h"
#include "common/logger.h"

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().setLogLevel(LogLevel::INFO);
        json program = parseJsonFromStdin();
        LOG_DEBUG("Starting dead code elimination");
        for (auto& func : program["functions"]) {
            std::vector<std::vector<json>> blocks = buildBlocks(func["instrs"]);
            std::vector<std::string> args(argv + 1, argv + argc);
            DCEConfig* config = static_cast<DCEConfig*>(createPassConfig("DCE"));
            config->enableGlobalDCE = true;
            for (auto& arg : args) {
                if (arg == "-g") {
                    Logger::getInstance().setLogLevel(LogLevel::DEBUG);
                }
                if (arg == "-global") {
                    config->enableGlobalDCE = true;
                }
                if (arg == "-local") {
                    config->enableLocalDCE = true;
                }
            }
            deadCodeElimination(blocks, *config);
            delete config;
            LOG_DEBUG("After dead code elimination");
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