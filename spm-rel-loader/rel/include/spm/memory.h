#pragma once

#include <cstdint>

namespace spm::memory {

extern "C" {

void *__memAlloc(uint32_t heap, uint32_t size);
void __memFree(uint32_t heap, void *ptr);

}

}