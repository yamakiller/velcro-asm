#include "tokenizer.h"
#include <string.h>
#include <locale.h>

#include "utils/xerror.h"
#include "utils/xstring.h"
#include "token.h"

using namespace VelcroAsm;
Tokenizer::Tokenizer(const char* src) {
    int  i;
    char ch;
    trimState st = TS_normal;
    
    Pos = 0;
    Src = src;

    /* remove commends, including "//" and "##" */
    for (i = 0; i < Src.length();i++) {
        ch = Src[Pos];
        if (st == TS_normal) {
            switch(ch) {
                case '/' : st = TS_slcomm; break;
                case '"' : st = TS_string; break;
                case ';' : st = TS_accept; goto loop_end;
                case '#' : st = TS_hscomm; break;
            }
        } else if (st == TS_slcomm) {
            switch (ch) {
                case '/' : st = TS_nolast; goto loop_end;
                default  : st = TS_normal; break;
            }
        } else if (st == TS_hscomm) {
            switch (ch) {
                case '#' : st = TS_nolast; goto loop_end;
                default  : st = TS_normal; break;
            }
        } else if (st == TS_string) {
                switch (ch) {
                case '"' : st = TS_normal; break;
                case '\\': st = TS_escape; break;
            }
        } else if (st == TS_escape) {
            st = TS_string;
        }
    }
    loop_end:
    // check for errors
    switch(st) {
        case TS_accept: Src = Src.substr(0, i); break;
        case TS_nolast: Src = Src.substr(0, i - 1); break;
        case TS_string: throw XError(XErrorNo::StringNotTerminated); break;
        case TS_escape: throw XError(XErrorNo::ESCAPENotTerminated); break;
    }
}

char Tokenizer::Ch() {
    return Src[Pos];
}

bool Tokenizer::Eof() {
    return Pos >= Src.length();
}

char Tokenizer::Rch() {
    char ret = Src[Pos];
    Pos++;
    return ret;
}

std::string Tokenizer::find(int pos, std::function<bool(char)> check) {
    skip(check);
    return Src.substr(pos, Pos - pos);
}

void Tokenizer::skip(std::function<bool(char)> check) {
    for (;!Eof()&& check(Ch());) {
        Pos++;
        if (Src[Pos - 1] == '\n') {
            Row++;
        }
    }
}

Token* Tokenizer::chrv(int p) {
    //var err error
    uint64_t val; 

    /* starting and ending position */
    int p0 = p + 1;
    int p1 = p0 + 1;

    /* 查找文字的结尾 */
    for(; p1 < Src.length() && Src[p1] != '\'';) {
        p1++;
        if (Src[p1 -1] == '\\') {
            p1++;
        }
    }

    /* 如果是个空的字符串抛出异常 */
    if (p1 == p0) {
        throw XError(XErrorNo::EmptyConstant);
    }

    /* check for EOF */
    if (p1 == Src.length()) {
        throw XError(XErrorNo::UnExpectedEOF);
    }

    // TODO: 将当前字符串转换为数值
    //
    // TODO: 编写代码或函数
    //
    // TODO: 


    /* skip the closing '\'' */
    Pos = p1 + 1;
    return new Token(p, val);
}

Token* Tokenizer::numv(int p) {
    std::string num = find(p, VelcroAsm::XString::IsNumber);
    if (num.empty()) {
        throw XError(XErrorNo::NotImmediateValue);
    }
}