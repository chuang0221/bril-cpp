#include "buildBlocks.h"
#include "instr2str.h"

using json = nlohmann::json;

bool isTerminator(json instr) {
    std::unordered_set<std::string> set = {
        "br", "jmp", "ret" 
    };
    return (set.find(instr["op"]) != set.end());
}

std::vector<std::vector<json>> buildBlocks(json instrs) {
    std::vector<std::vector<json>> blocks;
    std::vector<json> currentBlock;
    for (auto instr: instrs) {
        if (instr.contains("op")) {
            // Instruction
            if (isTerminator(instr)) {
                currentBlock.push_back(instr);
                blocks.push_back(currentBlock);
                currentBlock.clear();
            }
            else {
                currentBlock.push_back(instr);
            }
        }
        else {
            // Label
            if (!currentBlock.empty()) {
                blocks.push_back(currentBlock);
            }
            currentBlock.clear();
            currentBlock.push_back(instr);
        }
    }
    if (!currentBlock.empty()) {
        blocks.push_back(currentBlock);
    }
    return blocks;
}

json flattenBlocks(std::vector<std::vector<json>> blocks) {
    json flat;
    for (auto block: blocks) {
        for (auto instr: block) {
            flat.push_back(instr);
        }
    }
    return flat;
}

void printBlock(std::vector<json> block, bool withLabel) {
    for (int i = 0; i < block.size(); i++) {
        if (withLabel) {
            if (i == 0 && block[i].contains("label")) {
                std::cout << "block " << block[i]["label"] << ":" << '\n';
                continue;
            }
            else if (i == 0){
                std::cout << "anonymous block:" << '\n';
            }
        }
        std::cout << "  " << instr2str(block[i]) << '\n';
    }
}

void printBlocks(std::vector<std::vector<json>> blocks, bool withLabel) {
    for (auto block: blocks) {
        printBlock(block, withLabel);
    }
}