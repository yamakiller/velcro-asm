#ifndef VELCRO_ASM_SYNTAX_ERROR_H
#define VELCRO_ASM_SYNTAX_ERROR_H

#include <cstdint>
#include <string>
#include "utils/xstring.h"

namespace VelcroAsm {
    /// @brief SyntaxError 表示汇编语法中的错误.
    struct SyntaxError {
        int            Pos;
        int            Row;
        const wchar_t* Src;   
        std::string    Reason;
    };

    std::string SyntaxErrorString(const SyntaxError &error) {
        if (error.Pos < 0) {
            return XString::Format("Syntax error at line %d: %s", error.Row, error.Reason.c_str());
        }

        return XString::Format("Syntax error at [%d:%d]: %s", error.Row, error.Pos + 1, error.Reason.c_str());
    }
}

#endif // VELCRO_ASM_SYNTAX_ERROR_H