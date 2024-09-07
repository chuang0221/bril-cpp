#include "common/common.h"

json parseJsonFromStdin() {
    json program;
    try {
        std::cin >> program;
    }
    catch (json::parse_error& e) {
        std::cerr << "Error parsing JSON: " << e.what() << '\n';
        throw;
    }
    return program;
}