#include "token.h"
#include "utils/xstring.h"
#include "utils/xerror.h"

using namespace VelcroAsm;
Punctuation Token::Punc() {
    if (Kind != TokenPunc) {
        throw XError(XErrorNo::TokenNotPunc);
    }

    return (Punctuation)Uint;
}

bool Token::IsPunc(Punctuation punc) {
    return Kind == TokenPunc && (Punctuation)Uint == punc;
}

std::string Token::String() {
    switch(Kind) {
        case TokenEnd   : return "<END>";
        case TokenInt   : return XString::Format("<INT %d>", Uint);
        case TokenFp64  : return XString::Format("<FP64 %f>", Fp64);
        case TokenPunc  : return XString::Format("<PUNC %s>", GetPuncName((Punctuation)Uint));
        case TokenName  : return XString::Format("<Name %s>", Str);
        case TokenSpace : return "<SPACE>";
        default         : return XString::Format("<UNK:%d %d %s>", (int)Kind, Uint, Str == nullptr?"":Str);
    }
}
