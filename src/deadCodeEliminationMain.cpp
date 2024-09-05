#include "deadCodeElimination.h"
#include "common.h"
#include "buildBlocks.h"
#include "logger.h"

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().setLogLevel(LogLevel::INFO);
        json program = parseJsonFromStdin();
        LOG_DEBUG("Starting dead code elimination");
        for (auto& func : program["functions"]) {
            std::vector<std::vector<json>> blocks = buildBlocks(func["instrs"]);
            std::vector<std::string> args(argv + 1, argv + argc);
            DCEConfig* config = static_cast<DCEConfig*>(createPassConfig("DCE"));
            for (auto& arg : args) {
                if (arg == "-g") {
                    Logger::getInstance().setLogLevel(LogLevel::DEBUG);
                }
                if (arg == "-a") {
                    config->enableAggressiveDCE = true;
                }
            }
            deadCodeElimination(blocks, *config);
            delete config;
            LOG_DEBUG("After dead code elimination");
            //printBlocks(blocks, false);
            func["instrs"] = flattenBlocks(blocks);
        }
        std::cout << program.dump(2) << std::endl;
        LOG_DEBUG("Local value numbering finished");
    } catch (const std::exception& e) {
        LOG_ERROR(std::string("Error: ") + e.what());
        return 1;
    }
    return 0;
}