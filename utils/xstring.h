#ifndef VELCRO_ASM_XSTRING_H
#define VELCRO_ASM_XSTRING_H

#include <cstdio>
#include <cstdarg>
#include <string>

namespace VelcroAsm::XString {
    std::string Format(const char* fmt,...);
    /// @brief 是否是0-9
    bool IsDigit(const char cc);
    /// @brief 是否是字母
    bool IsAlpha(const char cc);
    /// @brief 是否是数字
    bool IsNumber(const char cc);
    /// @brief 是否是变量命名规则内字符
    /// 下划线,字母,数字
    bool IsIdent(const char cc);
    /// @brief 是否是变量命名规则内字符
    /// 下划线,字母
    bool IsIdent0(const char cc);

    int64_t Literal64(const char *str, const int length);
} // namespace VelcroAsm




#endif // VELCRO_ASM_XSTRING_H
