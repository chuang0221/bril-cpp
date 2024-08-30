#include "instr2str.h"

using json = nlohmann::json;

std::string type2str(const json& type) {
    return type.get<std::string>();
}

std::string value2str(const json& type, const json& value) {
    return value.dump();
}

std::string instr2str(const json& instr) {
    std::ostringstream oss;
    if (instr["op"] == "const") {
        std::string tyann = instr.contains("type") ? ": " + type2str(instr["type"]) : "";
        oss << instr["dest"].get<std::string>() << tyann << " = const " 
            << value2str(instr["type"], instr["value"]);
    } else {
        std::string rhs = instr["op"].get<std::string>();
        if (instr.contains("funcs")) {
            for (const auto& f : instr["funcs"]) {
                rhs += " @" + f.get<std::string>();
            }
        }
        if (instr.contains("args")) {
            for (const auto& arg : instr["args"]) {
                rhs += " " + arg.get<std::string>();
            }
        }
        if (instr.contains("labels")) {
            for (const auto& label : instr["labels"]) {
                rhs += " ." + label.get<std::string>();
            }
        }
        if (instr.contains("dest")) {
            std::string tyann = instr.contains("type") ? ": " + type2str(instr["type"]) : "";
            oss << instr["dest"].get<std::string>() << tyann << " = " << rhs;
        } else {
            oss << rhs;
        }
    }
    return oss.str();
}