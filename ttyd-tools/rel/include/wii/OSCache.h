#pragma once

#include <cstdint>

namespace wii::OSCache {

extern "C" {

void DCFlushRange(void *ptr, uint32_t size);
void ICInvalidateRange(void *ptr, uint32_t size);

}

}