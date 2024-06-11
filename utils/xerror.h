#ifndef VELCRO_ASM_XERROR_H
#define VELCRO_ASM_XERROR_H

#include <string>
#include <exception>

namespace VelcroAsm {
//missing immediate value
    /// @brief 错误码
    enum XErrorNo {
        NoError = 0,
        UnKnownPunc,
        TokenNotPunc,
        StringNotTerminated,
        ESCAPENotTerminated,
        EmptyConstant,
        UnExpectedEOF,
        NotImmediateValue,
        CustomizeError,
    };

    class XError : std::exception {
        public:
            XError(XErrorNo no) : m_errNo(no) {}
            XError(const char* errMsg) : m_errNo(XErrorNo::CustomizeError), m_error(errMsg) {}

            virtual const char* what() const noexcept override;
        private:
            XErrorNo     m_errNo;
            std::string m_error;
    };

}

#endif 