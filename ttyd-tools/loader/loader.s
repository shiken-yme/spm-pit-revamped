# C2 Insert at relMain:
#   eu0 8023e444
#   eu1 8023e444
#   us0 8023be50
#   us1 
#   us2 
#   jp0 
#   jp1 
#   kr0 80236b10

.set REGION, 'e' # e(u), u(s), j(p), k(r)
.set REVISION, 0 # 0-1,  0-2,  0-1,  0

.set memcpy, 0x80004000 # region free
.if (REGION == 'e') # both revisions have identical dols
    .set DVDConvertPathToEntrynum, 0x8027b910
    .set fileAlloc, 0x8019f7dc
    .set fileAsync, 0x8019fd24
    .set fileFree, 0x8019fa8c
    .set __memAlloc, 0x801a626c
    .set OSFatal, 0x802729b8
    .set OSLink, 0x80274c0c
.elseif ((REGION == 'e') && (REVISION == 1))
    .err # unported
.elseif ((REGION == 'u') && (REVISION == 0))
    .set DVDConvertPathToEntrynum, 0x80278f74
    .set fileAlloc, 0x8019ea50 
    .set fileAsync, 0x8019ef98
    .set fileFree, 0x8019ed00
    .set __memAlloc, 0x801a5634
    .set OSFatal, 0x80270198 
    .set OSLink, 0x802723cc
.elseif ((REGION == 'u') && (REVISION == 1))
    .err # unported
.elseif ((REGION == 'u') && (REVISION == 2))
    .err # unported
.elseif ((REGION == 'j') && (REVISION == 0))
    .err # unported
.elseif ((REGION == 'j') && (REVISION == 1))
    .err # unported
.elseif ((REGION == 'k') && (REVISION == 0))
    .set DVDConvertPathToEntrynum, 0x8027F85C
    .set fileAlloc, 0x8019B8B0
    .set fileAsync, 0x8019BDF8
    .set fileFree, 0x8019BB60
    .set __memAlloc, 0x8019EB44
    .set OSFatal, 0x80275114
    .set OSLink, 0x80277328
.else
    .err # Unknown version
.endif

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

removeFullCodeInstr:
b default - start
removeEntrynumCheckInstr:
b tryFileAsync - entrynumCheck

p_DVDConvertPathToEntrynum:
.long DVDConvertPathToEntrynum
p_memcpy:
.long memcpy
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
p_memAlloc:
.long __memAlloc

relPath:
.string "./mod/mod.rel"

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

# Try get entrynum for "./mod/mod.rel"
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
dcbf 0, r3
icbi 0, r3

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
lwz r0, p_memAlloc - startdata (r31)
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

# Allocate bss - no need to memset to 0 like game's rel loading
# function does, OSLink already does that
li r3, 0
lwz r4, 0x20 (r28)
lwz r0, p_memAlloc - startdata (r31)
mtlr r0
blrl

# Link rel
mr r4, r3
mr r3, r28
lwz r0, p_OSLink - startdata (r31)
mtlr r0
blrl

# Call rel prolog
lwz r0, 0x34 (r28)
mtlr r0
blrl

# Stop this code running again and let the game's rel load
lwz r0, removeFullCodeInstr - startdata (r31)
stw r0, start - startdata (r31)
addi r3, r31, start - startdata
dcbf 0, r3
icbi 0, r3

end:
# pop stack
lmw r28, 8 (r1)
addi r1, r1, 24
lwz r0, 4 (r1)
mtlr r0
blr

default: # will flow into the function to load the game's rel
stwu r1, -0x60 (r1) # default instruction at hook address
