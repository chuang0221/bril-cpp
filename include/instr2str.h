#ifndef INSTR2STR_H
#define INSTR2STR_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string instr2str(const json& instr);

#endif