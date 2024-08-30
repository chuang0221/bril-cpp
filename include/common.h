#ifndef COMMON_H
#define COMMON_H

#include <nlohmann/json.hpp>
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

json parseJsonFromStdin();

#endif