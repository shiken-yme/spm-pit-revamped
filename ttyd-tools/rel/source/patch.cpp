#include "patch.h"

#include <wii/OSCache.h>

#include <cstdint>

namespace mod::patch {

void clear_IC_DC_Cache(void * ptr, uint32_t size)
{
	wii::OSCache::DCFlushRange(ptr, size);
	wii::OSCache::ICInvalidateRange(ptr, size);
}

void writeBranch(void *ptr, void *destination, bool link)
{
	uint32_t delta = reinterpret_cast<uint32_t>(destination) - reinterpret_cast<uint32_t>(ptr);
	uint32_t value = link ? 0x48000001 : 0x48000000;
	value |= (delta & 0x03FFFFFC);
	
	uint32_t *p = reinterpret_cast<uint32_t *>(ptr);
	*p = value;

	clear_IC_DC_Cache(ptr, sizeof(uint32_t));
}

}