#ifndef _VJIT_ATT_INSTRUCTIONS_H
#define _VJIT_ATT_INSTRUCTIONS_H

#include <string>
#include <any>
#include <atomic>

// MaxOperands is the maximum number of operands an instruction can take.
#define MAX_OPERANDS 6

typedef std::any Operands[MAX_OPERANDS];
class Pseudo;

// BranchType represents the branch type of an instruction.
enum BranchType : uint8_t {
	BranchNone = 0,
	BranchAlways,
	BranchConditional
};

// InstructionDomain represents the domain of an instruction.
enum InstructionDomain : uint8_t {
	DomainGeneric = 0,
	DomainPseudo,
	DomainArchSpecific,
};

class Arch;
class Instruction {
	public:
	    void Free();
        void clearArgs();

    public:
		void    	*PC;
		std::string Name;
		int         Argc;
		Operands    Argv;
		Pseudo*     Pseudo;
		BranchType  Branch;
		InstructionDomain Domain;
    private:
        std::atomic_int64_t  m_refs;
		Arch *               m_arch;
};



#endif