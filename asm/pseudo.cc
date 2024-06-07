#include "pseudo.h"

const char* GetPseudoName(PseudoType type) {
    switch(type) {
        case PseudoNop:
        return ".nop";
        case PseudoData:  
        return ".data";
	    case PseudoByte:  
        return ".byte";
	    case PseudoWord:  
        return ".word";
	    case PseudoLong:  
        return ".long";
	    case PseudoQuad: 
        return ".quad";
	    case PseudoAlign:
        return ".align";
    }
}

