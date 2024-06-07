#ifndef _VJIT_ATT_ARCH_H
#define _VJIT_ATT_ARCH_H

#include <string>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include "general/error.h"


class Instruction;
class Program;
class Assembler;
class ParsedInstruction;
class Tokenizer;

class Feature  {
    public:
        virtual std::string String() = 0;
        virtual uint8_t FeatureID() = 0;

};

// Implementation is the arch-specific implementations.
class Implementation {
    public:
        virtual  Instruction* New() = 0;
        virtual  void Free(Instruction *p) = 0;
        virtual  Error Build(Program *p, Assembler *pasm, ParsedInstruction *ins) = 0;
        virtual  void  Parse(Tokenizer *p, ParsedInstruction *ins) = 0;
        virtual  int   Encode(Instruction *p,char *m) = 0;
        virtual  char* Assemble(Program *p,void* pc) = 0;
};


class Arch {
        friend class Asm;
    public:
        Arch();
    private:
        uint8_t         m_fmax;
        std::string     m_name;
        uint64_t        m_fset[4];
        Implementation *m_impl;
};

class Asm {
    public:
        static Asm* Instance();
    private:
        static Asm *_instance;
    public:
        Arch* GetArch(const char* name);
    private:
        std::unordered_map<std::string, Arch*> m_archs;
        mutable std::shared_mutex              m_archMut;
};

#endif