#pragma once

namespace spm::seqdrv {

struct SeqWork;

}


namespace spm::seqdef {

typedef void (SeqFunc)(struct spm::seqdrv::SeqWork *);

struct SeqDef
{
    SeqFunc *init;
    SeqFunc *main;
    SeqFunc *exit;
};

extern "C" {

extern SeqDef seq_data[8];

}

}