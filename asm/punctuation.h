#ifndef VELCRO_ASM_PUNCTUATION_H
#define VELCRO_ASM_PUNCTUATION_H

namespace VelcroAsm {
    enum Punctuation : int {
        PuncPlus = 1,
        PuncMinus,
        PuncStar,
        PuncSlash,
        PuncPercent,
        PuncAmp,
        PuncBar,
        PuncCaret,
        PuncShl,
        PuncShr,
        PuncTilde,
        PuncLBrace,
        PuncRBrace,
        PuncLIndex,
        PuncRIndex,
        PuncLCurly,
        PuncRCurly,
        PuncDot,
        PuncComma,
        PuncColon,
        PuncDollar,
        PuncHash,
        PuncBang,
        PuncEqual,
        PuncDDollar,
    };

   const char* GetPuncName(Punctuation Punc);
}


#endif // VELCRO_ASM_PUNCTUATION_H
