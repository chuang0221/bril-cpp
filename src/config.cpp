#include "config.h"

PassConfig* createPassConfig(const std::string& passName) {
    if (passName == "LVN") {
        return new LVNConfig(false, false, false);
    } else if (passName == "DCE") {
        return new DCEConfig(false, false);
    }
    return nullptr;
}