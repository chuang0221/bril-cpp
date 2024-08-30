#ifndef BUILDBLOCKS_H
#define BUILDBLOCKS_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
bool isTerminator(json instr);
std::vector<std::vector<json>> buildBlocks(json instrs);
void printBlock(std::vector<json> block, bool withLabel);
void printBlocks(std::vector<std::vector<json>> blocks, bool withLabel);

#endif