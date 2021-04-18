#pragma once

#include <spm/seqdef.h>

#include <cstdint>

namespace spm::seqdrv {

enum
{
    SEQ_LOGO = 0,
    SEQ_TITLE,
    SEQ_GAME,
    SEQ_MAPCHANGE,
    SEQ_GAMEOVER,
    SEQ_LOAD
};

struct SeqWork
{
    int32_t seq; // enum above, or -1 if none
    int32_t stage;
    char *p0;
    char *p1;
    uint8_t unknown_0x10[0x20 - 0x10];
    spm::seqdef::SeqFunc *afterFunc;
};
static_assert(sizeof(SeqWork) == 0x24);

extern "C" {

extern SeqWork seqWork;

void seqInit_SPMARIO();
void seqMain();
void seqSetSeq(int32_t seq, char * p0, char * p1);
int32_t seqGetSeq();

}

}