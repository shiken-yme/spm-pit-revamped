#pragma once

#include <cstdint>

namespace spm::evtmgr {

struct _EvtEntry;
typedef int32_t (*user_func)(struct _EvtEntry * entry, bool firstRun);
typedef int32_t EvtScriptCode;

typedef struct _EvtEntry {
  int64_t lifetime;
  uint8_t flags; // bit flags, see defines above
  uint8_t curDataLength; // number of pieces of data (4 bytes each) belonging to the current instruction
  uint8_t curOpcode; // see evtmgr_cmd.h for opcodes
  uint8_t priority;
  uint8_t type;
  int8_t blocked;
  int8_t dowhileDepth; // number of do-while loops the current instruction is nested into
  int8_t unknown_0xf;
  EvtScriptCode * pCurInstruction;
  EvtScriptCode * pCurData;
  int8_t labelIds[16]; // each correspond to an address in the jump table
  void *jumptable[16]; // addresses for each label
  struct _EvtEntry *parent;
  struct _EvtEntry *childEntry;
  struct _EvtEntry *brotherEntry;
  int32_t unknown_0x74;
  int64_t unknown_0x78;
  char unknown_0x80[0x94 - 0x80];
  user_func *userFunc; // Function set & called by the user_func opcode
  int32_t lw[16];
  uint32_t lf[3];
  void *dowhileStartPtrs[8]; // pointer to do opcodes
  int32_t dowhileCounters[8];
  char unknown_0x124[0x150 - 0x124];
  int32_t *uw;
  uint32_t *uf;
  int32_t id;
  float speed;
  char unknown_0x160[0x198 - 0x160];
  EvtScriptCode * scriptStart;
  char *name; // debug thing, unused?
  void *pPrevInstruction;
  char unknown_0x1a4[0x1a8 - 0x1a4];
} EvtEntry;
static_assert(sizeof(EvtEntry) == 0x1a8);

typedef struct {
  int32_t entryCount;
  int32_t gw[32];
  uint32_t gf[3];
  EvtEntry *entries;
  char unknown_0x94[0x98 - 0x94];
  int64_t time;
} EvtWork;

extern "C" {

EvtEntry *evtEntry(EvtScriptCode *script, uint8_t priority, uint8_t flags); // 800d8b88
EvtEntry *evtEntryType(EvtScriptCode *script, uint8_t priority, uint8_t flags, int32_t type); // 800d8df4
EvtEntry *evtChildEntry(EvtEntry *entry, EvtScriptCode *script, uint8_t flags); // 800d9060
EvtEntry *evtBrotherEntry(EvtEntry *entry, EvtScriptCode *script, uint8_t flags); // 800d9370

// more

}

}
