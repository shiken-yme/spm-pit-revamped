# Custom Rel Loader for Super Paper Mario

A project to allow rel files created by PistonMiner's rel tools to be ran in Super Paper Mario. Will eventually include a library to interact with the game with in the rel.

The file "/rel/mod.rel" is loaded from the disc (add with [Riivolution](http://rvlution.net/wiki/Riivolution/) or by ISO patching with [Wiimms ISO Tools](https://wit.wiimm.de/)) and executed during boot by the cheat code given in loader.s (assemble with a tool like [pyiiasmh](https://github.com/JoshuaMKW/pyiiasmh) or [codewrite](https://github.com/TheGag96/CodeWrite)).

Currently only supports PAL revision 0.

## Contents
  * rel: Framework for writing code on-top of existing code in Super Paper Mario
  * elf2rel: Convert from ELF file to REL file

## Original Project Credits
 * Technical assistance and additional reverse engineering by **JasperRLZ**
 * Reverse engineering with focus on the battle system by **Jdaster64**
 * Tool implementation and reverse engineering by **PistonMiner**
 * Debugging assistance and reverse engineering by **Zephiles**
