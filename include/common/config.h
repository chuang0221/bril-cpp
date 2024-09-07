#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class PassConfig {
public:
    virtual ~PassConfig() = default;
};

class LVNConfig : public PassConfig {
public:
    bool enableCommutative;
    bool enableConstantFolding;
    bool enableAlgebraicIdentity;

    LVNConfig(bool enableCommutative, bool enableConstantFolding, bool enableAlgebraicIdentity)
        : enableCommutative(enableCommutative), enableConstantFolding(enableConstantFolding), enableAlgebraicIdentity(enableAlgebraicIdentity) {}
};

class DCEConfig : public PassConfig {
public:
    bool enableGlobalDCE;
    bool enableLocalDCE;
    DCEConfig(bool enableGlobalDCE, bool enableLocalDCE) : enableGlobalDCE(enableGlobalDCE), enableLocalDCE(enableLocalDCE) {}
};

PassConfig* createPassConfig(const std::string& passName);

#endif