#ifndef BUILDCFG_H
#define BUILDCFG_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <map>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::map<std::string, std::vector<json>> buildTable(const std::vector<std::vector<json>>& blocks);
void insertTerminators(std::map<std::string, std::vector<json>>& table);
json successors(const json& instr);

#endif