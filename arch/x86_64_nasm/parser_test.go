package x86_64_nasm

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/require"
	"github.com/yamakiller/velcro-asm/asm"
)

func TestParser_Parse(t *testing.T) {
	p := asm.GetArch("x86_64_nasm").CreateParser()
	v, e := p.Feed("mov rax, 123")
	require.NoError(t, e)
	require.Equal(t, asm.LineInstruction, v.Kind)
	println("Mnemonic    :", v.Instruction.Mnemonic)
	for i, x := range v.Instruction.Operands {
		println(fmt.Sprintf("Operand %3d : %s", i, x.String()))
	}
}

/*`-123`,
`rcx`,
`(rax)`,
`(rax)123`,
`(rax)-123`,
`(rax,rbx,4)`,
`(rax,rbx,4)1234`,
`(rax,8,)`,
`(rax,8,)1234`,
`(123 + 456)`,
`(rax)(123 + 456)`,
`'asd'`,
`'asdf'`,
`'asdfghj'`,
`'asdfghjk'`,*/
