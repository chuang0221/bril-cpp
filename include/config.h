#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config {
public:
    bool enableCommutative;
    bool enableConstantFolding;
    bool enableAlgebraicIdentity;

    Config(bool enableCommutative, bool enableConstantFolding, bool enableAlgebraicIdentity)
        : enableCommutative(enableCommutative), enableConstantFolding(enableConstantFolding), enableAlgebraicIdentity(enableAlgebraicIdentity) {}
};

#endif