#ifndef _VJIT_ATT_INSTRUCTION_H
#define _VJIT_ATT_INSTRUCTION_H

type Instruction struct {
	refs   int64
	arch   *Arch
	PC     uintptr
	Name   string
	Argc   int
	Argv   Operands
	Pseudo Pseudo
	Branch BranchType
	Domain InstructionDomain
}

class Instruction {
    public:
    private:
        int64_t m_refs;
}

#endif 