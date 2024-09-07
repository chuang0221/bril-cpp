#ifndef DEAD_CODE_ELIMINATION_H
#define DEAD_CODE_ELIMINATION_H

#include "common/config.h"
#include <nlohmann/json.hpp>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <functional>
#include <iostream>

using json = nlohmann::json;

void deadCodeElimination(std::vector<std::vector<json>>& blocks, DCEConfig& config);
void deadCodeEliminationGlobal(std::vector<std::vector<json>>& blocks);
void deadCodeEliminationLocal(std::vector<json>& block);
#endif