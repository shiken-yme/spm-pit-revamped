#include "mod.h"
#include "patch.h"

#include <spm/fontmgr.h>
#include <spm/mapdata.h>
#include <spm/relmgr.h>
#include <spm/seqdrv.h>
#include <spm/seqdef.h>
#include <wii/OSError.h>

#include <cstdint>
#include <cstdio>
#include <cstring>

namespace mod {

Mod *gMod = nullptr;

void main()
{
	Mod *mod = new Mod();
	mod->init();
}

/*
static void dumpEvtLocs()
{
    for (int i = 0; i < MAP_ID_MAX; i++)
	{
        spm::mapdata::MapData * map = spm::mapdata::mapDataList[i];
        wii::OSError::OSReport("%d %s: 0x%x\n", i, map->name, map->script);
    }
}
typedef void (RelPrologFn)();
static void handleGameRelProlog()
{
	// Get pointer to rel prolog from caller
	RelPrologFn * prolog;
	asm("mr %0, 12" : "=r" (prolog));

	// Hack prolog to be  stable enough to give script pointers
	uint32_t * relBase = (uint32_t *) spm::relmgr::relHolder->relFile;
	relBase[0x66304 / 4] = 0; // block global ctor
	patch::writeBranch((void *) 0x8002c060, (void *) &spm::mapdata::mapDataPtr); // hackily link mapDataPtr
	prolog();

	// Call custom code
	wii::OSError::OSReport("Rel loaded at 0x%x\n", (uint32_t) spm::relmgr::relHolder->relFile);
	dumpEvtLocs();

	// Inform user process has finished (game would just crash after this)
	wii::RGBA fg = {0xff, 0xff, 0xff, 0xff};
	wii::RGBA bg = {0x00, 0x00, 0x00, 0xff};
	wii::OSError::OSFatal(&fg, &bg, "Script dump complete.");
}

static void relDotBinLoadHack()
{
	spm::relmgr::relCompName = "rel.bin";
	patch::writeBranch(
		(void *) ((uint32_t) &spm::relmgr::loadRel + 0x194), // bctrl to game's rel prolog function
		(void *) &handleGameRelProlog,
		true
	);
}
*/

static spm::seqdef::SeqFunc *seq_titleMainReal;
static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp) {
	wii::RGBA green {0, 255, 0, 255};
	spm::fontmgr::FontDrawStart();
	spm::fontmgr::FontDrawEdge();
	spm::fontmgr::FontDrawColor(&green);
	spm::fontmgr::FontDrawScale(0.8);
	spm::fontmgr::FontDrawNoiseOff();
	spm::fontmgr::FontDrawRainbowColorOff();
	spm::fontmgr::FontDrawString(-100.0f, 200.0f, "SPM Rel Loader V1.0");
	seq_titleMainReal(wp);
}
static void titleScreenCustomTextPatch()
{
	seq_titleMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main;
	spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
}

Mod::Mod()
{
	wii::OSError::OSReport("The mod has ran! Object at 0x%x\n", (uint32_t) this);
}

void Mod::init()
{
	gMod = this;
	// relDotBinLoadHack();
	titleScreenCustomTextPatch();
}

}
