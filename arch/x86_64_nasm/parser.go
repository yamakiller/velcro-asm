package x86_64_nasm

import (
	"fmt"
	"math"
	"strconv"
	"strings"

	"github.com/yamakiller/velcro-asm/asm"
)

type (
	_PrefixImpl byte
)

const (
	PrefixLock _PrefixImpl = iota
	PrefixSegmentCS
	PrefixSegmentDS
	PrefixSegmentES
	PrefixSegmentFS
	PrefixSegmentGS
	PrefixSegmentSS
)

func (prx _PrefixImpl) String() string {
	switch prx {
	case PrefixLock:
		return "LOCK"
	case PrefixSegmentCS:
		return "CS"
	case PrefixSegmentDS:
		return "DS"
	case PrefixSegmentES:
		return "ES"
	case PrefixSegmentFS:
		return "FS"
	case PrefixSegmentGS:
		return "GS"
	case PrefixSegmentSS:
		return "SS"
	default:
		return "???"
	}
}

func (prx _PrefixImpl) EncodePrefix(p *asm.Instruction) {
	switch prx {
	case PrefixLock:
		this(p).LOCK()
	case PrefixSegmentCS:
		this(p).CS()
	case PrefixSegmentDS:
		this(p).DS()
	case PrefixSegmentES:
		this(p).ES()
	case PrefixSegmentFS:
		this(p).FS()
	case PrefixSegmentGS:
		this(p).GS()
	case PrefixSegmentSS:
		this(p).SS()
	default:
		panic("unreachable")
	}
}

const (
	rip Register64 = 0xff
)

var _RegBranch = map[string]bool{
	"jmp":  true,
	"call": true,
}

var _SegPrefix = map[string]_PrefixImpl{
	"cs": PrefixSegmentCS,
	"ds": PrefixSegmentDS,
	"es": PrefixSegmentES,
	"fs": PrefixSegmentFS,
	"gs": PrefixSegmentGS,
	"ss": PrefixSegmentSS,
}

type _ParserImpl struct {
	lex *asm.Tokenizer
}

func mkparser(lex *asm.Tokenizer) *_ParserImpl {
	return &_ParserImpl{lex}
}

func (par *_ParserImpl) i32(tk asm.Token, v int64) int32 {
	if v >= math.MinInt32 && v <= math.MaxUint32 {
		return int32(v)
	} else {
		panic(par.err(tk.Pos, fmt.Sprintf("32-bit integer out ouf range: %d", v)))
	}
}

func (par *_ParserImpl) err(pos int, msg string) *asm.SyntaxError {
	return &asm.SyntaxError{
		Pos:    pos,
		Row:    par.lex.Row,
		Src:    par.lex.Src,
		Reason: msg,
	}
}

func (par *_ParserImpl) rela() asm.RelativeOffset {
	var iv int64
	var tk asm.Token

	/* record the tokenizer state */
	tp := par.lex.Pos
	tr := par.lex.Row

	/* a single dot */
	if tk = par.lex.Next(); tk.Ty == asm.TokenPunc {
		switch tk.Punc() {
		case asm.PuncPlus:
			iv = 1
		case asm.PuncMinus:
			iv = -1
		}
	}

	/* neither "+" nor "-", it's a single dot, revert the tokenizer */
	if iv == 0 {
		par.lex.Pos = tp
		par.lex.Row = tr
		return 0
	}

	/* parse the offset absolute value */
	pos := par.lex.Pos
	val := par.lex.Value(nil) * iv

	/* range check */
	if val < math.MinInt32 || val > math.MaxInt32 {
		panic(par.err(pos, "relative offset out of range"))
	} else {
		return asm.RelativeOffset(val)
	}
}

func (par *_ParserImpl) relx(tk asm.Token) {
	if !tk.IsPunc(asm.PuncLBrace) {
		panic(par.err(tk.Pos, "'(' expected for RIP-relative addressing"))
	} else if tk = par.lex.Next(); par.regx(tk) != rip {
		panic(par.err(tk.Pos, "RIP-relative addressing expects %rip as the base register"))
	} else if tk = par.lex.Next(); !tk.IsPunc(asm.PuncRBrace) {
		panic(par.err(tk.Pos, "RIP-relative addressing does not support indexing or scaling"))
	}
}

func (par *_ParserImpl) regx(tk asm.Token) asm.Register {

	//if !tk.IsPunc(asm.PuncPercent) {
	//	panic(par.err(tk.Pos, "'%' expected for registers"))
	if tk = par.lex.Read(); tk.Ty != asm.TokenName {
		panic(par.err(tk.Pos, "register name expected"))
	} else if key := strings.ToLower(tk.Str); key == "rip" {
		return rip
	} else if reg, ok := Registers[key]; ok {
		return reg
	} else {
		panic(par.err(tk.Pos, "invalid register name: "+strconv.Quote(tk.Str)))
	}
}

func (par *_ParserImpl) regv(tk asm.Token) asm.Register {
	if reg := par.regx(tk); reg == rip {
		panic(par.err(tk.Pos, "rip is not accessable as a dedicated register"))
	} else {
		return reg
	}
}

func (par *_ParserImpl) disp(vv int32) asm.MemoryAddress {
	pos := par.lex.Pos
	row := par.lex.Row
	tok := par.lex.Next()

	/* absolute addressing, may require rewinding */
	if tok.Ty == asm.TokenEnd || tok.IsPunc(asm.PuncComma) {
		par.lex.Pos = pos
		par.lex.Row = row
		return MemoryAddress(nil, nil, 0, vv)
	}

	/* begin of memory address */
	if !tok.IsPunc(asm.PuncLBrace) {
		panic(par.err(tok.Pos, "'(' expected"))
	}

	/* memory address */
	if par.lex.MatchPunc(asm.PuncComma) {
		return par.index(nil, vv)
	} else {
		return par.base(par.lex.Next(), vv)
	}
}

func (par *_ParserImpl) base(tk asm.Token, disp int32) asm.MemoryAddress {
	rr := par.regx(tk)
	nk := par.lex.Next()

	/* check for register indirection or base-index addressing */
	if !isReg64(rr) {
		panic(par.err(tk.Pos, "not a valid base register"))
	} else if nk.Ty != asm.TokenPunc {
		panic(par.err(nk.Pos, "',' or ')' expected"))
	} else if nk.Punc() == asm.PuncComma {
		return par.index(rr, disp)
	} else if nk.Punc() == asm.PuncRBrace {
		return MemoryAddress(rr, nil, 0, disp)
	} else {
		panic(par.err(nk.Pos, "',' or ')' expected"))
	}
}

func (par *_ParserImpl) index(base asm.Register, disp int32) asm.MemoryAddress {
	tk := par.lex.Next()
	rr := par.regx(tk)
	nk := par.lex.Next()

	/* check for scaled indexing */
	if base == rip {
		panic(par.err(tk.Pos, "RIP-relative addressing does not support indexing or scaling"))
	} else if !isIndexable(rr) {
		panic(par.err(tk.Pos, "not a valid index register"))
	} else if nk.Ty != asm.TokenPunc {
		panic(par.err(nk.Pos, "',' or ')' expected"))
	} else if nk.Punc() == asm.PuncComma {
		return par.scale(base, rr, disp)
	} else if nk.Punc() == asm.PuncRBrace {
		return MemoryAddress(base, rr, 1, disp)
	} else {
		panic(par.err(nk.Pos, "',' or ')' expected"))
	}
}

func (par *_ParserImpl) scale(base asm.Register, index asm.Register, disp int32) asm.MemoryAddress {
	tk := par.lex.Next()
	bv := 1 << tk.Uint

	/* must be an integer */
	if tk.Ty != asm.TokenInt {
		panic(par.err(tk.Pos, "integer expected"))
	}

	/* check for scale value */
	if tk.Uint == 0 || _Scales&bv == 0 {
		panic(par.err(tk.Pos, "scale can only be 1, 2, 4 or 8"))
	}

	/* match the closing ")" */
	par.lex.ExpectPunc(asm.PuncRBrace)
	return MemoryAddress(base, index, uint8(tk.Uint), disp)
}

func (par *_ParserImpl) value(ins *asm.ParsedInstruction, rr *bool) {

	var ok bool
	var tk asm.Token
	var pp _PrefixImpl

	/* save the tokenizer state */
	pos := par.lex.Pos
	row := par.lex.Row

	/* encountered an integer, must be a SIB memory address */
	if tk = par.lex.Next(); tk.Ty == asm.TokenInt {
		ins.Mem(par.disp(par.i32(tk, int64(tk.Uint))))
		return
	}

	/* encountered an identifier, maybe an expression or a jump target, or a segment override prefix */
	if tk.Ty == asm.TokenName {
		ts := tk.Str
		tp := par.lex.Pos
		tr := par.lex.Row

		/* if the next token is EOF or a comma, it's a jumpt target */
		if tk = par.lex.Next(); tk.Ty == asm.TokenEnd || tk.IsPunc(asm.PuncComma) {
			par.lex.Pos = tp
			par.lex.Row = tr
			ins.Target(ts)
			return
		}

		/* if it is a colon, it's a segment override prefix, otherwise it must be an RIP-relative addressing operand */
		if !tk.IsPunc(asm.PuncColon) {
			par.relx(tk)
			ins.Reference(ts)
			return
		}

		/* lookup segment prefixes */
		if pp, ok = _SegPrefix[strings.ToLower(ts)]; !ok {
			panic(par.err(tk.Pos, "invalid segment name"))
		}

		/* read the next token */
		tk = par.lex.Next()
		ins.Prefixes = append(ins.Prefixes, pp)

		/* encountered an integer, must be a SIB memory address */
		if tk.Ty == asm.TokenInt {
			ins.Mem(par.disp(par.i32(tk, int64(tk.Uint))))
			return
		}
	}

	/* certain instructions may have a "*" before operands */
	if tk.IsPunc(asm.PuncStar) {
		tk = par.lex.Next()
		*rr = true
	}

	/* ... otherwise it must be a punctuation */
	if tk.Ty != asm.TokenPunc {
		panic(par.err(tk.Pos, "'$', '-' or '(' expected"))
	}

	/* '[' is special because it might be either `[expr]` or just `SIB`*/
	if tk.Punc() != asm.PuncLIndex {
		switch tk.Punc() {
		case asm.PuncDot: // `.`
			ins.PCrel(par.rela())
			break
		case asm.PuncMinus: // "-"
			ins.Mem(par.disp(par.i32(tk, -par.lex.Value(nil))))
			return
		default:

			if _, ok := Registers[tk.Str]; ok {
				ins.Reg(par.regv(tk))
				return
			}

			if par.lex.Pos == 0 {

			}
			//if ins.Imm(par.lex.Value(nil))
			//(*expr.Parser).Value
			panic(par.err(tk.Pos, "'expr', 'register', '-' or '(' expected"))
		}
		return
	}

	/* '(' is special because it might be either `(SIB)(expr)` or just `(SIB)`,
	 * read one more token to confirm, handle it later */
	/*if tk.Punc() != asm.PuncLIndex {
		switch tk.Punc() {
		case asm.PuncDot: // "."
			ins.PCrel(par.rela())
			return
		case asm.PuncMinus: // "-"
			ins.Mem(par.disp(par.i32(tk, -par.lex.Value(nil))))
			return
		case asm.PuncDollar: // "$"
			ins.Imm(par.lex.Value(nil))
			return
		default:
			if _, ok := Registers[tk.Str]; ok {
				ins.Reg(par.regv(tk))
				return
			}
			panic(par.err(tk.Pos, "'$', '%', '-' or '(' expected"))
		}
	}*/

	/* the next token is '%', it's a memory address, or ',' if it's a memory address without base */
	if tk = par.lex.Next(); tk.Ty == asm.TokenPunc {
		switch tk.Punc() {
		case asm.PuncPercent:
			ins.Mem(par.base(tk, 0))
			return
		case asm.PuncComma:
			ins.Mem(par.index(nil, 0))
			return
		}
	} else if tk.Ty == asm.TokenName {
		if _, ok := Registers[tk.Str]; ok {
			ins.Reg(par.regv(tk))
			return
		}
	}

	/* ... otherwise it must be `(expr)(SIB)`, revert the tokenizer */
	par.lex.Pos = pos
	par.lex.Row = row

	ins.Mem(par.disp(par.i32(tk, par.lex.Value(nil))))
}

func (par *_ParserImpl) parse(ins *asm.ParsedInstruction) {
	rr := false
	tk := par.lex.Next()

	/* special case for the "lock" prefix */
	if tk.Ty == asm.TokenName && strings.ToLower(tk.Str) == "lock" {
		tk = par.lex.Next()
		ins.Prefixes = append(ins.Prefixes, PrefixLock)
	}

	/* must be an instruction */
	if tk.Ty != asm.TokenName {
		panic(par.err(tk.Pos, "identifier expected"))
	}

	/* set the mnemonic */
	pos := tk.Pos
	ins.Mnemonic = strings.ToLower(tk.Str)

	/* check if the instruction has operands */
	if par.lex.MatchEnd() {
		return
	}

	/* construct a placeholder token */
	tk.Ty = asm.TokenPunc
	tk.Uint = uint64(asm.PuncComma)

	/* parse all the operands */
	for tk.IsPunc(asm.PuncComma) {
		par.value(ins, &rr)
		tk = par.lex.Next()
	}

	/* should be the end of instruction */
	if tk.Ty != asm.TokenEnd {
		panic(par.err(tk.Pos, "garbage after instruction"))
	}

	/* check "jmp" and "call" instructions */
	if _RegBranch[ins.Mnemonic] {
		if len(ins.Operands) != 1 {
			panic(par.err(pos, fmt.Sprintf(`"%s" requires exact 1 argument`, ins.Mnemonic)))
		} else if op := ins.Operands[0].Op; !rr && op != asm.OpReg && op != asm.OpPCrel && op != asm.OpLabel {
			panic(par.err(pos, fmt.Sprintf(`invalid operand for "%s" instruction`, ins.Mnemonic)))
		}
	}
}
