#include "instructions.h"


void Instruction::Free() {
   
	/*if atomic.AddInt64(&inst.refs, -1) == 0 {
		inst.clearArgs()
		inst.clearPseudo()
		inst.arch.impl.Free(inst)
	}*/

    if (m_refs.fetch_add(-1, std::memory_order_acq_rel) == 0) {
        clearArgs();
    }
}

void Instruction::clearArgs() {
    for (int i =0;i < Argc;i++) {
        var v = dynamic_cast<tag.Disposable>(Argv[i]);
        if (v != nullptr_t) {
            v.Free();
        }
        
    }
    	for i := 0; i < inst.Argc; i++ {
		if v, ok := inst.Argv[i].(tag.Disposable); ok {
			v.Free()
		}
	}
}