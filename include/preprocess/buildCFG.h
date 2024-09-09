#ifndef BUILDCFG_H
#define BUILDCFG_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <map>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::pair<std::unordered_map<std::string, std::vector<json>>, std::vector<std::string>> 
buildTable(const std::vector<std::vector<json>>& blocks);
void insertTerminators(std::unordered_map<std::string, std::vector<json>>& table, const std::vector<std::string>& insertOrder);
json successors(const json& instr);
json predecessors(const std::string& label, const std::unordered_map<std::string, std::vector<json>>& table, const std::vector<std::string>& insertOrder);

#endif