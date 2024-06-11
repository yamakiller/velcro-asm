#include "punctuation.h"
#include "utils/xerror.h"

namespace VelcroAsm {
    const char* GetPuncName(Punctuation punc) {
        switch(punc) {
            case PuncPlus    : return "+";
            case PuncMinus   : return "-";
            case PuncStar    : return "*";
            case PuncSlash   : return "/";
            case PuncPercent : return "%";
            case PuncAmp     : return "&";
            case PuncBar     : return "|";
            case PuncCaret   : return "^";
            case PuncShl     : return "<<";
            case PuncShr     : return ">>";
            case PuncTilde   : return "~";
            case PuncLBrace  : return "(";
            case PuncRBrace  : return ")";
            case PuncLIndex  : return "[";
            case PuncRIndex  : return "]";
            case PuncLCurly  : return "{";
            case PuncRCurly  : return "}";
            case PuncDot     : return ".";
            case PuncComma   : return ",";
            case PuncColon   : return ":";
            case PuncDollar  : return "$";
            case PuncHash    : return "#";
            case PuncBang    : return "!";
            case PuncEqual   : return "=";
            case PuncDDollar : return "$$";
            default: throw XError(XErrorNo::UnKnownPunc);
        }
    }
}