#pragma once

#include <wii/types.h>

namespace wii::OSError {

extern "C" {

void OSReport(const char *message, ...);
void OSFatal(RGBA *foreground, RGBA *background, const char *messsage);

}

}
