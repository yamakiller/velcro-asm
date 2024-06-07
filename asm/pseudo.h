#ifndef _VJIT_ATT_PSEUDO_H
#define _VJIT_ATT_PSEUDO_H

enum PseudoType : int {
    PseudoNop = 0,
    PseudoData,
    PseudoByte,
    PseudoWord,
    PseudoLong,
    PseudoQuad,
    PseudoAlign,
};

// PseudoNames records the name for every pseudo-instruction type.
/*var PseudoNames = map[PseudoType]string{
	PseudoNop:   ".nop",
	PseudoData:  ".data",
	PseudoByte:  ".byte",
	PseudoWord:  ".word",
	PseudoLong:  ".long",
	PseudoQuad:  ".quad",
	PseudoAlign: ".align",
}*/

static const char* GetPseudoName(PseudoType type);

class Pseudo {
	public:
		virtual const char* String() = 0;
		virtual PseudoType  Type() = 0;
		virtual int Encode(char *m, void *pc) = 0;
};

class Data {

};
/*type Pseudo interface {
	fmt.Stringer
	tag.Disposable
	Type() PseudoType
	Encode(m *[]byte, pc uintptr) int
}*/

#endif 