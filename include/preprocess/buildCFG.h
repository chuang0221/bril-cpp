#ifndef BUILDCFG_H
#define BUILDCFG_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class CFG {
public:
    CFG(const std::vector<std::vector<json>>& blocks);

    const std::unordered_map<std::string, std::vector<json>>& getTable() const;
    const std::vector<std::string>& getInsertOrder() const;
    const std::unordered_map<std::string, std::unordered_set<std::string>>& getSuccessors() const;
    const std::unordered_map<std::string, std::unordered_set<std::string>>& getPredecessors() const;

private:
    void buildTable(const std::vector<std::vector<json>>& blocks);
    void insertTerminators();
    void computeSuccessorsAndPredecessors();

    std::unordered_map<std::string, std::vector<json>> table;
    std::vector<std::string> insertOrder;
    std::unordered_map<std::string, std::unordered_set<std::string>> successors;
    std::unordered_map<std::string, std::unordered_set<std::string>> predecessors;
};

#endif