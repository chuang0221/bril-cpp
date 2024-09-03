#ifndef LOCAL_VALUE_NUMBERING_H
#define LOCAL_VALUE_NUMBERING_H

#include <nlohmann/json.hpp>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <functional>
#include <iostream>

using json = nlohmann::json;

namespace std {
    template <>
    struct hash<std::tuple<int, std::string, int>> {
        size_t operator()(const std::tuple<int, std::string, int>& key) const {
            auto hash1 = std::hash<int>{}(std::get<0>(key));
            auto hash2 = std::hash<std::string>{}(std::get<1>(key));
            auto hash3 = std::hash<int>{}(std::get<2>(key));
            return hash1 ^ (hash2 << 1) ^ (hash3 << 2);
        }
    };
}

class ValueNumbering {
private:
    int _number;
    std::unordered_map<std::string, int> name2number;
    std::unordered_map<int, std::string> number2name;
    std::unordered_map<std::tuple<int, std::string, int>, int> hashTable;
public:
    ValueNumbering();
    int getNumber(const std::string& name);
    int addNumber(const std::string& name);
    int refreshNumber(const std::string& name);
    void update(json& instr);
};

void checkCommutative(std::tuple<int, std::string, int>& nameTuple);

void localValueNumbering(std::vector<std::vector<json>>& blocks);
#endif