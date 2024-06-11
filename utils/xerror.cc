#include "xerror.h"

namespace VelcroAsm {
    const char* XError::what() const noexcept {
        switch(m_errNo) {
            case XErrorNo::NoError : return "no error";
            case XErrorNo::UnKnownPunc : return "undefined symbol";
            case XErrorNo::TokenNotPunc: return "token is not a punctuation";
            case XErrorNo::StringNotTerminated: return "string is not terminated";
            case XErrorNo::ESCAPENotTerminated: return "escape sequence is not terminated";
            case XErrorNo::EmptyConstant: return "empty character constant";
            case XErrorNo::UnExpectedEOF: return "unexpected EOF when scanning literals";
            case XErrorNo::NotImmediateValue: return "missing immediate value";
            case XErrorNo::CustomizeError: return m_error.c_str();
            default:
            return "unknown";
        }
    }
}