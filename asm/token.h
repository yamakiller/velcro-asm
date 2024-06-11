#ifndef VELCRO_ASM_TOKEN_H
#define VELCRO_ASM_TOKEN_H

#include <cstdint>
#include <string>
#include "punctuation.h"

namespace VelcroAsm {
  // Token 类型
  enum TokenKind : int {
      TokenEnd = 1, // 结束
      TokenInt,
      TokenFp64,
      TokenName,
      TokenPunc,   // 标点符号
      TokenSpace,  // 空格
  };

  class Token {
  public:
    TokenKind   Kind;
    int         Pos;
    int         End;
    char       *Str;
    uint64_t    Uint;
    double      Fp64;
  public:
    /// @brief 创建TokenEnd/TokenSpace
    Token(TokenKind kind, int p, int end) : Kind(kind), Pos(p), End(end) {}
    /// @brief 创建TokenInt
    Token(int p, uint64_t val) : Kind(TokenInt), Pos(p), Uint(val) {}
    /// @brief 创建TokenFp64
    Token(int p, double   val) : Kind(TokenFp64), Pos(p), Fp64(val) {}
    /// @brief 创建TokenName
    Token(int p, char* name) : Kind(TokenName), Pos(p), Str(name) {}
    /// @brief 创建TokenPunc
    Token(int p, Punctuation punc) : Kind(TokenPunc), Pos(p), Uint(uint64_t(punc)) {}
  public:
    /// @brief 返回当强标点符号
    /// @return 标点符号，如果不是标点符号抛出异常.
    Punctuation Punc();
    /// @brief 判断是否是指定的标点符号
    /// @param punc 指定的标点符号
    /// @return True/False(是/不是)
    bool        IsPunc(Punctuation punc);
    /// @brief 转换未字符串
    /// @return 返回字符串
    std::string String();
  };
}

#endif // VELCRO_ASM_TOKEN_H