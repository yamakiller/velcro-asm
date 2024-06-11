#ifndef VELCRO_ASM_TOKENIZER_H
#define VELCRO_ASM_TOKENIZER_H

#include <string>
#include <functional>

namespace VelcroAsm {
    struct SyntaxError;
    class  Token;
    enum trimState : int {
          TS_normal = 0,
          TS_slcomm,
          TS_hscomm,
          TS_string,
          TS_escape,
          TS_accept,
          TS_nolast,
    };


    class Tokenizer {
    public:
        Tokenizer(const char* src);

        char Ch();
        bool Eof();
        char Rch();
        SyntaxError* Err(int pos, const char* msg);

        
    private:
        std::string find(int pos, std::function<bool(char)> check);
        void   skip(std::function<bool(char)> check);
        Token* chrv(int p);
        Token* numv(int p);
    public:
        int Pos;
        int Row;
        std::string Src; 
    };
}

#endif  // VELCRO_ASM_TOKENIZER_H