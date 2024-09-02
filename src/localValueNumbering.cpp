#include "localValueNumbering.h"

ValueNumbering::ValueNumbering() : _number(0) {}

int ValueNumbering::getNumber(const std::string& name) {
    if (name2number.find(name) == name2number.end()) {
        return -1;
    }
    return name2number[name];
}

int ValueNumbering::addNumber(const std::string& name) {
    int number = getNumber(name);
    if (number != -1) return number;
    number = _number++;
    name2number[name] = number;
    number2name[number] = name;
    return number;
}

int ValueNumbering::refreshNumber(const std::string& name) {
    int number = _number++;
    name2number[name] = number;
    number2name[number] = name;
    return number;
}

void ValueNumbering::update(json& instr) {
    std::string op = instr["op"];
    std::string dest = instr["dest"];
    std::vector<std::string> args;
    for (auto& arg : instr["args"]) {
        args.push_back(arg);
    }
    if (args.size() != 2) return; // Only consider binary operations
    std::string arg1 = args[0], arg2 = args[1];
    int arg1Number = addNumber(arg1), arg2Number = addNumber(arg2);
    if (hashTable.find({arg1Number, op, arg2Number}) != hashTable.end()) {
        instr["op"] = "id";
        instr["args"] = json::array({number2name[hashTable[{arg1Number, op, arg2Number}]]});
        name2number[dest] = hashTable[{arg1Number, op, arg2Number}];
    }
    else {
        int destNumber = refreshNumber(dest);
        hashTable[{arg1Number, op, arg2Number}] = destNumber;
    }
}

void localValueNumbering(std::vector<std::vector<json>>& blocks) {
    for (auto& block : blocks) {
        ValueNumbering vn;
        for (auto& instr : block) {
            if (instr.find("dest") != instr.end()) {
                vn.update(instr);
            }
        }
    }
}