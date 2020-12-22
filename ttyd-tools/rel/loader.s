# C2 @ 8023e5fc

.set DVDConvertPathToEntrynum, 0x8027b910
.set memcpy, 0x80004000
.set memset, 0x80004104
.set fileAlloc, 0x8019f7dc
.set fileAsync, 0x8019fd24
.set fileFree, 0x8019fa8c
.set __MemAlloc, 0x801a626c
.set OSFatal, 0x802729b8
.set OSLink, 0x80274c0c

# r31: startdata pointer
# r30: file record pointer then bss pointer
# r29: file record data holder pointer
# r28: rel memory pointer

start:
# push stack
mflr r0
stw r0, 4 (r1)
stwu r1, -24 (r1)
stmw r28, 8 (r1)

# Get pointer to data
bl enddata

startdata:

blrInstr:
blr
removeEntrynumCheckInstr:
b tryFileAsync - entrynumCheck

p_DVDConvertPathToEntrynum:
.long DVDConvertPathToEntrynum
p_memcpy:
.long memcpy
p_memset:
.long memset
p_fileAlloc:
.long fileAlloc
p_fileAsync:
.long fileAsync
p_fileFree:
.long fileFree
p_OSFatal:
.long OSFatal
p_OSLink:
.long OSLink
p_MemAlloc:
.long __MemAlloc

relPath:
.string "./rel/mod.rel"

noRelMsg:
.string "ERROR: mod.rel was not found"
noLoadMsg:
.string "ERROR: failed to load mod.rel"
foreground:
.byte 0xff, 0xff, 0xff, 0xff
background:
.byte 0, 0, 0, 0xff

.align 2
enddata:

mflr r31

# Try get entrynum for "./rel/mod.rel"
entrynumCheck:
addi r3, r31, relPath - startdata
lwz r0, p_DVDConvertPathToEntrynum - startdata (r31)
mtlr r0
blrl
cmpwi r3, -1
bne+ haveEntrynum

# Inform user file could not be found
addi r3, r31, foreground - startdata
addi r4, r31, background - startdata
addi r5, r31, noRelMsg - startdata
lwz 0, p_OSFatal - startdata (r31)
mtlr r0
blrl
b end

# Stop this check running again
haveEntrynum:
lwz r0, removeEntrynumCheckInstr - startdata (r31)
stw r0, entrynumCheck - startdata (r31)
addi r3, r31, entrynumCheck - startdata
dcbf r0, r3
icbi r0, r3

# Call fileAsync
tryFileAsync:
addi r3, r31, relPath - startdata
li r4, 0
li r5, 0
lwz r0, p_fileAsync - startdata (r31)
mtlr r0
blrl
cmpwi r3, 0
beq+ end # file not ready yet

# Call fileAlloc
addi r3, r31, relPath - startdata
li r4, 0
li r5, 0
lwz r0, p_fileAlloc - startdata (r31)
mtlr r0
blrl
cmpwi r3, -1
bne+ fileloaded

# Inform user file could not be loaded
addi r3, r31, foreground - startdata
addi r4, r31, background - startdata
addi r5, r31, noLoadMsg - startdata
lwz 0, p_OSFatal - startdata (r31)
mtlr r0
blrl
b end

fileloaded:

# Back up record pointer and get data holder pointer
mr r30, r3
lwz r29, 0xa4 (r3)

# Allocate memory for rel
li r3, 0
lwz r4, 4 (r29)
lwz r0, p_MemAlloc - startdata (r31)
mtlr r0
blrl
mr r28, r3

# Copy rel into memory
lwz r4, 0 (r29)
lwz r5, 4 (r29)
lwz r0, p_memcpy - startdata (r31)
mtlr r0
blrl

# Free file
mr r3, r30
lwz r0, p_fileFree - startdata (r31)
mtlr r0
blrl

# Allocate bss and set to 0
li r3, 0
lwz r4, 0x20 (r28)
lwz r0, p_MemAlloc - startdata (r31)
mtlr r0
blrl
mr r30, r3
li r4, 0
lwz r5, 0x20 (r28)
lwz r0, p_memset - startdata (r31)
mtlr r0
blrl

# Link rel
mr r3, r28
mr r4, r30
lwz r0, p_OSLink - startdata (r31)
mtlr r0
blrl

# Call rel prolog
lwz r0, 0x34 (r28)
mtlr r0
blrl

# Stop this code running again
lwz r0, blrInstr - startdata (r31)
stw r0, start - startdata (r31)
addi r3, r31, start - startdata
dcbf r0, r3
icbi r0, r3

end:
# pop stack
lmw r28, 8 (r1)
addi r1, r1, 24
lwz r0, 4 (r1)
mtlr r0
blr
