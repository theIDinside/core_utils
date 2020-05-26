#include "core.hpp"

namespace cx {
    void unimplemented(const char* signature) {
        panic("Function/Method: {} not implemented", signature);
    }
}