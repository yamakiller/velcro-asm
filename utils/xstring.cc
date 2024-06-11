#include "xstring.h"

namespace VelcroAsm::XString {
    std::string Format(const char* fmt,...) {
        va_list args, args1;
        va_start(args, fmt);
        va_copy(args1, args);

        std::string res(1 + vsnprintf(nullptr, 0, fmt, args1), 0);

        va_end(args1);

        vsnprintf(&res[0], res.size(), fmt, args);

        va_end(args);

        return res;
    }

    bool IsDigit(const char cc) {
        return '0' <= cc && cc <= '9';
    }

    bool IsAlpha(const char cc) {
        return (cc >= 'a' && cc <= 'z') || (cc >= 'A' && cc <= 'Z');
    }

    bool IsNumber(const char cc) {
        return (cc == 'b' || cc == 'B') ||
           (cc == 'o' || cc == 'O') ||
           (cc == 'x' || cc == 'X') ||
           (cc >= '0' && cc <= '9') ||
           (cc >= 'a' && cc <= 'f') ||
           (cc >= 'A' && cc <= 'F');
    }

    bool IsIdent(const char cc) {
        return cc == '_' || isalpha(cc) || isdigit(cc);
    }

    bool IsIdent0(const char cc) {
        return cc == '_' || isalpha(cc);
    }

    /*int64_t Literal64(const char *str, const int length) {
        int  nb;
        char ch;
        char mm[12];

        for (;;) {

        }
    }*/
}