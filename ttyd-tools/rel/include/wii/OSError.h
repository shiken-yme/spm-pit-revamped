#pragma once

namespace wii::OSError {

extern "C" {

void OSReport(const char * message, ...);

}

}
