#include "mod.h"

#include "patch.h"

#include <wii/OSError.h>

#include <cstdio>
#include <cstring>

namespace mod {

Mod *gMod = nullptr;

void main()
{
	Mod *mod = new Mod();
	mod->init();
}

Mod::Mod()
{
	wii::OSError::OSReport("The mod has ran! Object at 0x%x", (unsigned int) this);
}

void Mod::init()
{
	gMod = this;
}

}
