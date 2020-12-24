# Custom Rel Loader for Super Paper Mario

A project to allow rel files created by PistonMiner's rel tools to be ran in Super Paper Mario. Will eventually include a library to interact with the game with in the rel.

The file "/rel/mod.rel" is loaded from the disc (add with [Riivolution](http://rvlution.net/wiki/Riivolution/) or by ISO patching with [Wiimms ISO Tools](https://wit.wiimm.de/)) and executed during boot, before the game's rel, by the cheat code given in loader.s (assemble with a tool like [Pyiiasmh](https://github.com/JoshuaMKW/pyiiasmh) or [Codewrite](https://github.com/TheGag96/CodeWrite)).

# Regions & Revisions

Currently, only supports PAL revision 0 and NTSC-K revision 0 are supported.

To change the target region or revsion for a rel, use `make rgX`, where rg is the region code ('eu', 'us', 'jp' or 'kr') and X is the revision number (0-1 for eu & jp, 0-2 for us, 0 for kr).

To change the target for the loader, edit the definitions of REGION ('e' for eu, 'u' for us, 'j' for jp and 'k' for kr) and REVISION (0-1 for eu & jp, 0-2 for us, 0 for kr) and change the injection address in the tool you're using to assemble the gecko code to the corresponding one listed at the top of the file.

## Contents
  * rel: Framework for writing code on-top of existing code in Super Paper Mario
  * elf2rel: Convert from ELF file to REL file

## Original Project Credits
 * Technical assistance and additional reverse engineering by **JasperRLZ**
 * Reverse engineering with focus on the battle system by **Jdaster64**
 * Tool implementation and reverse engineering by **PistonMiner**
 * Debugging assistance and reverse engineering by **Zephiles**
