#include "arch.h"

Asm *Asm::_instance = NULL;

Asm *Asm::Instance() {
    if (_instance == NULL) {
        _instance = new Asm();
    }

    return _instance;
}

const uint64_t featmax[] = {
    ULLONG_MAX,
    ULLONG_MAX,
    ULLONG_MAX,
    ULLONG_MAX,
};



// GetArch finds the architecture by name.
Arch* Asm::GetArch(const char* name) {
	std::shared_lock<std::shared_mutex> lock(m_archMut);
    if (m_archs.find(name) == m_archs.end()) {
        return NULL;
    }
	return m_archs[name];
}

//static archtab = make(map[string]*Arch)