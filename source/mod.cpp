#include "mod.h"
#include "patch.h"

#include <common.h>
#include <util.h>
#include <evt_cmd.h>
#include <spm/animdrv.h>
#include <spm/bgdrv.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/eff_small_star.h>
#include <spm/eff_spm_confetti.h>
#include <spm/eff_zunbaba.h>
#include <spm/evt_cam.h>
#include <spm/evt_eff.h>
#include <spm/evt_fairy.h>
#include <spm/evt_frame.h>
#include <spm/evt_guide.h>
#include <spm/evt_hit.h>
#include <spm/evt_img.h>
#include <spm/evt_item.h>
#include <spm/evt_map.h>
#include <spm/evt_mario.h>
#include <spm/evt_msg.h>
#include <spm/evt_mobj.h>
#include <spm/evt_npc.h>
#include <spm/evt_offscreen.h>
#include <spm/evt_paper.h>
#include <spm/evt_pouch.h>
#include <spm/evt_shop.h>
#include <spm/evt_snd.h>
#include <spm/evt_sub.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/evt_door.h>
#include <spm/fontmgr.h>
#include <spm/hitdrv.h>
#include <spm/hud.h>
#include <spm/itemdrv.h>
#include <spm/item_data.h>
#include <spm/lz_embedded.h>
#include <spm/mapdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/mobjdrv.h>
#include <spm/memory.h>
#include <spm/msgdrv.h>
#include <spm/npcdrv.h>
#include <spm/parse.h>
#include <spm/seqdef.h>
#include <spm/seqdrv.h>
#include <spm/seq_title.h>
#include <spm/spmario.h>
#include <spm/swdrv.h>
#include <spm/system.h>
#include <spm/rel/dan.h>
#include <spm/rel/machi.h>
#include <wii/os/OSError.h>
#include <wii/cx.h>
#include <wii/gx.h>
#include <msl/math.h>
#include <msl/stdio.h>
#include <msl/string.h>

namespace mod {
bool youSuck = false;

// New pit entrance message
const char * D100_entrance_01 = 
"<system>\n"
"<col cfffffff>This is the entrance to a\n"
"dangerous dungeon called the\n"
"Pit of 100 Trials.\n"
"<k>\n<p>\n"
"Once you enter, it isn't easy\n"
"to leave. Let me give you \n"
"a few pointers.\n"
"<k>\n<p>\n"
"So first, you've got the\n"
"<col c00000ff>Goombas.<col cfffffff><wait 2000> They're in there.\n"
"<k>\n<p>\n"
"Then you've got the... <wait 1000><col c00000ff>Koopas!!!\n"
"<col cfffffff><wait 500>I've spent my whole life stompin'\n"
"'em. They're really tough!\n"
"<k>\n<p>\n"
"Oh, then there's <col c00000ff>Shlurps.\n"
"<col cfffffff><wait 250>They're really hard.<wait 250> Don't even\n"
"try defeating those guys.\n"
"<k>\n<p>\n"
"<wait 250>... Unless you have some kind\n"
"of <col c00000ff>bomb.<col cfffffff><wait 250> If it swallowed one,\n"
"it'd probably die.\n"
"<k>\n<p>\n"
"But...\n"
"<wait 500>what kind of cheater would\n"
"bring <dynamic 3>bombs</dynamic> to the Pit?\n"
"<k>\n<p>\n"
"<shake>You know, back in my day,\n"
"we didn't <wait 250><dynamic 3>HAVE</dynamic> any bombs.\n"
"Got by fine without 'em.\n"
"</shake><k>\n<p>\n"
"<dynamic 3>Y'know what we called it?\n"
"<wait 500><shake><col c00000ff>\"Boomerless Pit.\"\n"
"<col cfffffff></shake></dynamic><k>\n<p>\n"
"<wait 250>... Well, I don't know\n"
"what a boomer is, but it\n"
"sounds evil.<wait 250> And smelly.\n"
"<k>\n<p>\n"
"It also sounds a bit\n"
"like 'bomb.'<wait 250> And I don't\n"
"like that.<wait 250> Bombs are <shake>cheating.\n"
"</shake><k>\n<p>\n"
"<wait 250>Where was I again?\n"
"<wait 500>Right. This generation always\n"
"taking the easy way out.\n"
"<k>\n<p>\n"
"<wait 250>Excuse me for a second.\n"
"<wait 1000><scale 0.67><shake>Ahem...\n"
"</shake></scale><k>\n<p>\n"
"<dynamic 3><scale 1.8><wait 500>SHAME<wait 500> ON<wait 500> YOU!!!\n"
"<wait 1000></scale>\"Classic Pit\" runner!\n"
"</dynamic>\n<k>\n<p>\n"
"<wait 1000>So...<wait 500> do you still want\n"
"to go in?\n"
"<o>";

const char * mac_kanban_003 = 
"<kanban>\n"
"<center><col c00000ff>Flipside Pit of 100 Trials</center>\n"
"</col><col c000fff0><center>Revamped</center>\n"
"</col><center>Discord: @tartt</center>\n"
"<k>";

const char * modoridokan_D100_01 = 
"<system>\n"
"Oh, it's you again. How's\n"
"the Pit been treating you?\n"
"<k>\n<p>\n"
"Oh, what's that you're\n"
"saying? \"Mr. Pipe, will you\n"
"let me out, pleeeeeease?\"\n"
"<k>\n<p>\n"
"You know what you are?\n"
"I know what you are. You're\n"
"just a<dynamic 2> QUITTER.\n"
"</dynamic><k>\n<p>\n"
"You know what we used to\n"
"do back in the day?\n"
"<k>\n<p>\n"
"Me and my buddies, we'd\n"
"go to the Flipside Pit...\n"
"EVERY day after school.\n"
"<k>\n<p>\n"
"Well, that part's a lie,\n"
"I've never been to a school.\n"
"But the point is...\n"
"<k>\n<p>\n"
"...we'd go to the Pit,\n"
"and we'd exit from the\n"
"100th Trial. Every time.\n"
"<k>\n<p>\n"
"And you know what?\n"
"We did it without any of\n"
"your darned Pixls.\n"
"<k>\n<p>\n"
"None of those \"items\" or all\n"
"that other hullabaloo. Just\n"
"us and our Ultra Hammers.\n"
"<k>\n<p>\n"
"You youngsters just don't\n"
"know when to quit or when\n"
"to keep going...\n"
"<k>\n<p>\n"
"You do know that if you leave\n"
"now, I won't just put you\n"
"back here, right?\n"
"<k>\n<p>\n"
"Yeah, that's right. You leave\n"
"now, and you get to start\n"
"ALL over again.\n"
"<k>\n<p>\n"
"<dynamic 1>\"Oh, but that's so cruel!\n"
"What kind of questionable\n"
"game design is this?\"\n"
"</dynamic><k>\n<p>\n"
"I'll tell you what kind of\n"
"\"game design\" it is.\n"
"<shake>Good and old-fashioned.\n"
"</shake><k>\n<p>\n"
"You children think this is\n"
"a game? You're supposed to\n"
"be saving the world!\n"
"<k>\n<p>\n"
"Yeah, I went there.\n"
"I can see the headlines\n"
"already...\n"
"<k>\n<p>\n"
"<dynamic 3>\"Local Hero of the Light\n"
"Prognosticus: Abandoning All\n"
"Worlds, Playing in a Pit?!\"\n"
"</dynamic><k>\n<p>\n"
"I mean, here I was, trying\n"
"my best not to judge you.\n"
"But I can't help myself!\n"
"<k>\n<p>\n"
"You're the Hero, and you\n"
"can't even handle 100\n"
"simulated battles?\n"
"<k>\n<p>\n"
"I thought I'd gotten this\n"
"through to you the first\n"
"time, but...<wait 1000><scale 0.67><shake>  Ahem...\n"
"</shake></scale><k>\n<p>\n"
"<dynamic 3><scale 1.8><wait 500>SHAME<wait 500> ON<wait 500> YOU!!!\n"
"<wait 1000></scale>\nUseless quitter!\n"
"</dynamic>\n<k>\n<p>\n"
"... Well, I suppose it's fine to\n"
"let you go and save all the\n"
"worlds now.\n"
"<k>\n<p>\n"
"Do you want to leave the\n"
"Flipside Pit of 100 Trials?\n"
// "<wait 5000>Quitter!!!!!!!!\n"
"<o>";
/*
const char * m_noroi = 
"lmao skill issue\n"
"<k>";
*/

static double boobies = 0;

// Somewhere here, do the dan.c thing monkaS
s32 evt_dan_read_data_new(spm::evtmgr::EvtEntry * entry, bool isFirstCall)
{
    (void) entry;

    // Allocate work on first run
    // (check is a relD leftover, this only runs once on retail anyway)
    if (isFirstCall)
    {
        spm::dan::dan_wp = (spm::dan::DanWork *) spm::memory::__memAlloc(spm::memory::Heap::HEAP_MAP, sizeof(spm::dan::DanWork));
        msl::string::memset(spm::dan::dan_wp, 0, sizeof(*spm::dan::dan_wp));
        spm::dan::dan_wp->dungeons = (spm::dan::DanDungeon *) spm::memory::__memAlloc(spm::memory::Heap::HEAP_MAP, sizeof(spm::dan::DanDungeon[DUNGEON_MAX]));
        msl::string::memset(spm::dan::dan_wp->dungeons, 0, sizeof(spm::dan::DanDungeon[DUNGEON_MAX]));
    }
    
    // Reset Pit chests
    spm::swdrv::swClear(433);
    spm::swdrv::swClear(434);
    spm::swdrv::swClear(435);
    spm::swdrv::swClear(436);
    spm::swdrv::swClear(437);
    spm::swdrv::swClear(438);
    spm::swdrv::swClear(439);
    spm::swdrv::swClear(440);
    spm::swdrv::swClear(441);
    bool nipples = spm::swdrv::swGet(409);
    

    // Prepare pit text to be read
    u32 size = wii::cx::CXGetCompressionHeader(spm::lz_embedded::pitText).decompSize;
    char * decompPitText = (char *) spm::memory::__memAlloc(0, size);
    wii::cx::CXUncompressLZ(spm::lz_embedded::pitText, decompPitText);
    spm::parse::parseInit(decompPitText, size);

    // Add all dungeon entries to work
    while (spm::parse::parsePush("<Dungeon>"))
    {
        // Read no (dungeon id)
        s32 no = 0;
        s32 i = 0;
        spm::parse::parseTagGet1("<no>", spm::parse::PARSE_VALUE_TYPE_INT, &no);
        assertf(no >= 0 && no < DUNGEON_MAX, "なんか番号がおかしい [%d]", no);

    // Global room generation RNG value (1-100, 5 decimal places)
    double thighs = spm::system::rand() % 100 + 1;
    double divisor = 10;
    double ass = (spm::system::rand() % 10) / divisor;
    divisor = 100;
    double booty = (spm::system::rand() % 10) / divisor;
    divisor = 1000;
    double butt = (spm::system::rand() % 10) / divisor;
    divisor = 10000;
    double bottom = (spm::system::rand() % 10) / divisor;
    divisor = 100000;
    double buttocks = (spm::system::rand() % 10) / divisor;
    boobies = (thighs + ass + booty + butt + bottom + buttocks);
    if (boobies > 100) {
        boobies = boobies - 1;
    }

// Start pit room randomization process
    // Establish default doors + other vars
        int activeDoorCount = 19; // Actually 20, but let's not complicate things
        int doorOn[31] = {2, 3, 6, 7, 9, 12, 13, 16, 18, 19, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32};
        int firstSegment = 0;
        int roomDecCode = 0;
        bool segment400  = false;
        bool segment1  = false;
        bool segment800  = false;
        bool segment80  = false;
        bool segment40  = false;
        bool segment2  = false;
        bool segment2000  = false;
        bool segment1000  = false;
        bool segment8  = false;
        bool segment4  = false;
        bool segment4000  = false;
        bool segment200  = false;
        bool segment100  = false;
        bool segment10  = false;
        bool segment8000  = false;
        bool segment20  = false;
        bool segment10000  = false;
        bool segment20000  = false;
        bool segment40000  = false;
        bool segment80000  = false;
        bool segment400Checked  = false;
        bool segment1Checked  = false;
        bool segment800Checked  = false;
        bool segment80Checked  = false;
        bool segment40Checked  = false;
        bool segment2Checked  = false;
        bool segment2000Checked  = false;
        bool segment1000Checked  = false;
        bool segment8Checked  = false;
        bool segment4Checked  = false;
        bool segment4000Checked  = false;
        bool segment200Checked  = false;
        bool segment100Checked  = false;
        bool segment10Checked  = false;
        bool segment8000Checked  = false;
        bool segment20Checked  = false;
        
        
    // Random segment generation + add'l doors 
        i = 0;
        while (i <= 16) {
            int segmentID = spm::system::rand() % 16;
            int ballsack = spm::system::rand() % 25 + 1;
            int taint = 0;
            switch(segmentID) {
                case 0:
                // Segment 400
                if (segment400Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 1024;
                    segment400 = true;
                    segment400Checked = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 1;
                    break;
                } else if (segment1000 == true && segment8000 == true) {
                    taint = 0;
                } else if (segment1000 == true && segment2000 == true) {
                    taint = 0;
                } else if (segment40 == true && segment4 == true && segment20 == true) {
                    taint = 0;
                } else if (segment1000 == true && segment4000 == true) {
                    taint = 0;
                } else if (segment800 == true) {
                    taint = 4;
                } else {
                    taint = 20;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 1024;
                    segment400 = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 1;
                    if (taint == 4) {
                        segment10000 = true;
                        segment20000 = true;
                        segment40000 = true;
                        segment80000 = true;
                    }
                }
                segment400Checked  = true;
                break;

                case 1:
                // Segment 1
                if (segment1Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 1;
                    segment1 = true;
                    segment1Checked = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 17;
                    break;
                } else if (segment100 == true && segment4 == true && segment8000 == true) {
                    taint = 0;
                } else if (segment4 == true && segment20 == true) {
                    taint = 0;
                } else if (segment80 == true || segment2 == true || segment800 == true) {
                    taint = 6;
                } else {
                    taint = 20;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 1;
                    segment1 = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 17;
                    if (taint == 6) {
                        segment40000 = true;
                        segment80000 = true;
                    }
                }
                segment1Checked = true;
                break;
                
                case 2:
                // Segment 40
                if (segment40Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 64;
                    segment40 = true;
                    segment40Checked  = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 10;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 11;
                    break;
                } else if (segment100 == true) {
                    taint = 0;
                } else if (segment400 == true && segment4 == true && segment20 == true) {
                    taint = 0;
                } else {
                    taint = 20;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 64;
                    segment40  = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 10;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 11;
                }
                segment40Checked  = true;
                break;
                
                case 3:
                // Segment 1000
                if (segment1000Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 4096;
                    segment1000  = true;
                    segment1000Checked  = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 4;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 5;
                    break;
                } else if (segment400 == true && segment8000 == true) {
                    taint = 0;
                } else if (segment800 == true && segment4000 == true) {
                    taint = 0;
                } else if (segment800 == true && segment2000 == true) {
                    taint = 0;
                } else if (segment8000 == true && segment2000 == true) {
                    taint = 0;
                } else if (segment2000 == true && segment4000 == true) {
                    taint = 0;
                } else if (segment1000 == true && segment800 == true) {
                    taint = 0;
                } else if (segment2000 == true && segment400 == true) {
                    taint = 0;
                } else if (segment4000 == true && segment400 == true) {
                    taint = 0;
                } else if (segment800 == true && segment8000 == true) {
                    taint = 0;
                } else {
                    taint = 14;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 4096;
                    segment1000  = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 4;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 5;
                }
                segment1000Checked  = true;
                break;

                case 4:
                // Segment 4
                if (segment4Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 4;
                    segment4  = true;
                    segment4Checked  = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 20;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 21;
                    break;
                } else if (segment100 == true && segment8000 == true && segment1 == true) {
                    taint = 0;
                } else if (segment20 == true && segment1 == true) {
                    taint = 0;
                } else if (segment40 == true && segment400 == true && segment20 == true) {
                    taint = 0;
                } else {
                    taint = 14;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 4;
                    segment4  = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 20;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 21;
                }
                segment4Checked  = true;
                break;

                case 5:
                // Segment 100
                if (segment100Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 256;
                    segment100  = true;
                    segment100Checked  = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 14;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 15;
                    break;
                } else if (segment8000 == true && segment4 == true && segment1 == true) {
                    taint = 0;
                } else if (segment40 == true) {
                    taint = 0;
                } else {
                    taint = 18;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 256;
                    segment100  = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 14;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 15;
                }
                segment100Checked  = true;
                break;

                case 6:
                // Segment 8000
                if (segment8000Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 32768;
                    segment8000  = true;
                    segment8000Checked  = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 8;
                    break;
                } else if (segment100 == true && segment4 == true && segment1 == true) {
                    taint = 0;
                } else if (segment1000 == true && segment400 == true) {
                    taint = 0;
                } else if (segment1000 == true && segment2000 == true) {
                    taint = 0;
                } else if (segment1000 == true && segment800 == true) {
                    taint = 0;
                } else if (segment4000 == true) {
                    taint = 4;
                } else {
                    taint = 20;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 32768;
                    segment8000  = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 8;
                    if (taint == 4) {
                        segment10000  = true;
                        segment20000  = true;
                        segment40000  = true;
                        segment80000  = true;
                    }
                }
                segment8000Checked  = true;
                break;
                
                case 7:
                // Segment 20
                if (segment20Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 32;
                    segment20  = true;
                    segment20Checked  = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 24;
                    break;
                } else if (segment4 == true && segment1 == true) {
                    taint = 0;
                } else if (segment40 == true && segment4 == true && segment400 == true) {
                    taint = 0;
                } else if (segment4000 == true || segment200 == true || segment10 == true) {
                    taint = 6;
                } else {
                    taint = 21;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 32;
                    segment20  = true;
                    activeDoorCount = activeDoorCount + 1;
                    doorOn[activeDoorCount] = 24;
                    if (taint == 6) {
                        segment40000  = true;
                        segment80000  = true;
                    }
                }
                segment20Checked  = true;
                break;
                                
                case 8:
                // Segment 800
                if (segment800Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 2048;
                    segment800  = true;
                    segment800Checked  = true;
                    break;
                } else if (segment1000 == true && segment2000 == true) {
                    taint = 0;
                } else if (segment1000 == true && segment4000 == true) {
                    taint = 0;
                } else if (segment1000 == true && segment8000 == true) {
                    taint = 0;
                } else if (segment400 == true) {
                    taint = 4;
                } else if (segment80 == true && segment2 == true) {
                    taint = 4;
                } else {
                    taint = 12;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 2048;
                    segment800  = true;
                    if (taint == 4) {
                        segment10000  = true;
                        segment20000  = true;
                        segment40000  = true;
                        segment80000  = true;
                    }
                }
                segment800Checked  = true;
                break;

                case 9:
                // Segment 80
                if (segment80Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 128;
                    segment80  = true;
                    segment80Checked  = true;
                    break;
                } else if (segment1000 == true && segment2000 == true && segment4 == true && segment8 == true) {
                    taint = 0;
                } else if (segment40 == true || segment1 == true) {
                    taint = 4;
                } else if (segment800 == true && segment2 == true) {
                    taint = 4;
                } else {
                    taint = 9;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 128;
                    segment80  = true;
                    if (taint == 4) {
                        segment20000  = true;
                        segment40000  = true;
                        segment80000  = true;
                    }
                }
                segment80Checked  = true;
                break;

                case 10:
                // Segment 2
                if (segment2Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 2;
                    segment2  = true;
                    segment2Checked  = true;
                    break;
                } else if (segment8 == true) {
                    taint = 0;
                } else if (segment800 == true && segment80 == true) {
                    taint = 4;
                } else {
                    taint = 9;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 2;
                    segment2  = true;
                    if (taint == 4) {
                        segment80000  = true;
                    }
                }
                segment2Checked  = true;
                break;

                case 11:
                // Segment 2000
                if (segment2000Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 8192;
                    segment2000  = true;
                    segment2000Checked  = true;
                    break;
                } else if (segment1000 == true && segment800 == true) {
                    taint = 0;
                } else if (segment1000 == true && segment4000 == true) {
                    taint = 0;
                } else if (segment1000 == true && segment400 == true) {
                    taint = 0;
                } else if (segment80 == true && segment40 == true && segment1000 == true) {
                    taint = 0;
                } else if (segment100 == true && segment200 == true && segment1000 == true) {
                    taint = 0;
                } else if (segment8000 == true && segment1000 == true) {
                    taint = 0;
                } else if (segment1000 == true && segment4000 == true) {
                    taint = 0;
                } else if (segment1000 == true) {
                    taint = 2;
                } else {
                    taint = 4;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 8192;
                    segment2000  = true;
                }
                segment2000Checked  = true;
                break;

                case 12:
                // Segment 8
                if (segment8Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 8;
                    segment8  = true;
                    segment8Checked  = true;
                    break;
                } else if (segment1000 == true && segment800 == true) {
                    taint = 0;
                } else if (segment1000 == true && segment4000 == true) {
                    taint = 0;
                } else if (segment80 == true && segment40 == true && segment4 == true) {
                    taint = 0;
                } else if (segment1 == true && segment40 == true && segment4 == true) {
                    taint = 0;
                } else if (segment100 == true && segment200 == true && segment4 == true) {
                    taint = 0;
                } else if (segment4 == true && segment100 == true && segment20 == true) {
                    taint = 0;
                }  else if (segment2 == true && segment10 == true) {
                    taint = 2;
                }  else if (segment4 == true) {
                    taint = 4;
                } else {
                    taint = 6;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 8;
                    segment8  = true;
                }
                segment8Checked  = true;
                break;

                case 13:
                // Segment 4000
                if (segment4000Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 16384;
                    segment4000  = true;
                    segment4000Checked  = true;
                    break;
                } else if (segment1000 == true && segment2000 == true) {
                    taint = 0;
                } else if (segment1000 == true && segment800 == true) {
                    taint = 0;
                } else if (segment400 == true && segment1000 == true) {
                    taint = 0;
                } else if (segment8000 == true) {
                    taint = 5;
                } else if (segment200 == true && segment10 == true) {
                    taint = 5;
                } else {
                    taint = 11;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 16384;
                    segment4000  = true;
                }
                segment4000Checked  = true;
                break;

                case 14:
                // Segment 200
                if (segment200Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 512;
                    segment200  = true;
                    segment200Checked  = true;
                    break;
                } else if (segment1000 == true && segment2000 == true && segment4 == true && segment8 == true) {
                    taint = 0;
                } else if (segment100 == true || segment20 == true) {
                    taint = 5;
                } else if (segment10 == true && segment4000 == true) {
                    taint = 5;
                } else {
                    taint = 9;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 512;
                    segment200  = true;
                }
                segment200Checked  = true;
                break;

                case 15:
                // Segment 10
                if (segment10Checked == true) {
                    break;
                } else if (firstSegment == 0) {
                    firstSegment = 1;
                    roomDecCode = roomDecCode + 16;
                    segment10  = true;
                    segment10Checked  = true;
                    break;
                } else if (segment8 == true) {
                    taint = 0;
                } else if (segment4000 == true && segment200 == true) {
                    taint = 5;
                } else {
                    taint = 9;
                }
                if (taint >= ballsack) {
                    roomDecCode = roomDecCode + 16;
                    segment10  = true;
                }
                segment10Checked  = true;
                break;
            }
        ++i;
        }

// Room layout cleanup: choose from possible doors and add pipes
    if (segment10000 == false && segment400 == true && segment8000 == true) {
        segment10000 = true; }
    if (segment20000 == false && ((segment80 == true || segment200 == true || segment40 == true || segment100 == true) || (segment2000 == true && segment8 == true))) {
        segment20000 = true; }
    if (segment40000 == false && segment20000 == true && segment80000 == true && segment20 == true && segment1 == true) {
        segment40000 = true; }
    if (segment80000 == false && segment2 == true && segment10 == true) {
        segment80000 = true; }
    if (segment1 == false || segment20 == false) {
        segment40000 = false;
        segment80000 = false; }
    if (segment400 == false || segment8000 == false) {
        segment10000 = false; }
    if (segment10000 == true) {
        roomDecCode = roomDecCode + 65536; }
    if (segment20000 == true) {
        roomDecCode = roomDecCode + 131072; }
    if (segment40000 == true) {
        roomDecCode = roomDecCode + 262144; }
    if (segment80000 == true) {
        roomDecCode = roomDecCode + 524288; }

    // Layout post-processing: eliminate anomalies that prevent normal progression and can't be easily eliminated during generation (absolutely a skill issue on my end, but leave me alone)
    if (segment400 == true && segment800 == true && segment10000 == false) {
        segment800 = false;
        roomDecCode = roomDecCode - 2048;
    }
    if (segment8000 == true && segment4000 == true && segment10000 == false) {
        segment4000 = false;
        roomDecCode = roomDecCode - 16384;
    }
    if (segment80 == true && segment40 == true && segment1 == true && segment40000 == false) {
        segment80 = false;
        roomDecCode = roomDecCode - 128;
    }
    if (segment200 == true && segment100 == true && segment20 == true && segment40000 == false) {
        segment200 = false;
        roomDecCode = roomDecCode - 512;
    }
    if (segment4 == true && segment8 == true && (segment20 == true || segment1 == true) && (segment100 == true || segment40 == true)) {
        segment8 = false;
        roomDecCode = roomDecCode - 8;
    }
    if (segment100 == true && segment200 == true && segment80 == true && segment800 == true) {
        segment80 = false;
        roomDecCode = roomDecCode - 128;
    }
    if (segment80 == true && segment40 == true && segment200 == true && segment4000 == true) {
        segment4000 = false;
        roomDecCode = roomDecCode - 16384;
    }

    int doorEntranceArrayVal = spm::system::rand() % activeDoorCount;
    int doorEntrance = doorOn[doorEntranceArrayVal];
    int doorExitArrayVal = spm::system::rand() % activeDoorCount;
    while (doorExitArrayVal == doorEntranceArrayVal) {
        doorExitArrayVal = spm::system::rand() % activeDoorCount;
    }
    int doorExit = doorOn[doorExitArrayVal];

 // Read item id (chest contents in chest rooms, null & unused elsewhere)
        char itemName[64];
        spm::parse::parseTagGet1("<item>", spm::parse::PARSE_VALUE_TYPE_STRING, itemName);
        
            // Item Substitution Protocol
            double floorwiseMultiplier = 1;
            switch(no) {
                case 9:
                floorwiseMultiplier = 1.0;
                break;
                case 19:
                floorwiseMultiplier = 0.85;
                break;
                case 29:
                floorwiseMultiplier = 0.7;
                break;
                case 39:
                floorwiseMultiplier = 0.4;
                break;
                case 49:
                floorwiseMultiplier = 0;
                break;
                case 59:
                floorwiseMultiplier = -0.4;
                break;
                case 69:
                floorwiseMultiplier = -0.7;
                break;
                case 79:
                floorwiseMultiplier = -0.85;
                break;
                case 89:
                floorwiseMultiplier = -1.0;
                break;
            }

    // Establishing reward weights
   double ShroomShake = msl::math::pow(400, floorwiseMultiplier);
   double FireBurst = msl::math::pow(350, floorwiseMultiplier);
   double StarMedal = msl::math::pow(314, floorwiseMultiplier);
   double BigEgg = msl::math::pow(280, floorwiseMultiplier);
   double ShellShock = msl::math::pow(240, floorwiseMultiplier);
   double FriedShroomPlate = msl::math::pow(210, floorwiseMultiplier);
   double HoneySuper = msl::math::pow(160, floorwiseMultiplier);
   double IceStorm = msl::math::pow(140, floorwiseMultiplier);
   double MysteryBox = msl::math::pow(101, floorwiseMultiplier);
   double BlockBlock = msl::math::pow(64, floorwiseMultiplier);
   double ThunderRage = msl::math::pow(55, floorwiseMultiplier);
   double MightyTonic = msl::math::pow(50, floorwiseMultiplier);
   double RoastedShroomDish = msl::math::pow(40, floorwiseMultiplier);
   double ShootingStar = msl::math::pow(30, floorwiseMultiplier);
   double LifeShroom = msl::math::pow(30, floorwiseMultiplier);
   double MeteorMeal = msl::math::pow(25, floorwiseMultiplier);
   double GoldMedal = msl::math::pow(20, floorwiseMultiplier);
   double UltraShroomShake = msl::math::pow(15, floorwiseMultiplier);
   double DyllisDeluxe = msl::math::pow(5, floorwiseMultiplier);
   double TrialStew = .01;
   double sumOfLoot = (ShroomShake + FireBurst + StarMedal + BigEgg + ShellShock + FriedShroomPlate + HoneySuper + IceStorm + MysteryBox + BlockBlock + ThunderRage + MightyTonic + RoastedShroomDish + ShootingStar + LifeShroom + MeteorMeal + GoldMedal + UltraShroomShake + DyllisDeluxe + TrialStew);
    
    // Adjusting weights against floor number and normalizing them to add up to 99
    double SShake = (ShroomShake / (sumOfLoot) * 99);
    double FBurst = (FireBurst / (sumOfLoot) * 99);
    double SMedal = (StarMedal / (sumOfLoot) * 99);
    double BEgg = (BigEgg / (sumOfLoot) * 99);
    double SShock = (ShellShock / (sumOfLoot) * 99);
    double FSPlate = (FriedShroomPlate / (sumOfLoot) * 99);
    double HSuper = (HoneySuper / (sumOfLoot) * 99);
    double IStorm = (IceStorm / (sumOfLoot) * 99);
    double MBox = (MysteryBox / (sumOfLoot) * 99);
    double BBlock = (BlockBlock / (sumOfLoot) * 99);
    double TRage = (ThunderRage / (sumOfLoot) * 99);
    double MTonic = (MightyTonic / (sumOfLoot) * 99);
    double RSDish = (RoastedShroomDish / (sumOfLoot) * 99);
    double SStar = (ShootingStar / (sumOfLoot) * 99);
    double LShroom = (LifeShroom / (sumOfLoot) * 99);
    double MMeal = (MeteorMeal / (sumOfLoot) * 99);
    double GMedal = (GoldMedal / (sumOfLoot) * 99);
    double USShake = (UltraShroomShake / (sumOfLoot) * 99);
    double DDeluxe = (DyllisDeluxe / (sumOfLoot) * 99);

    // Establishing selection ranges for each item -- these should be perfectly 1-100 like boobies.
    double F_TStew = (100 - TrialStew);
    double F_SShake = (F_TStew - SShake);
    double F_FBurst = (F_SShake - FBurst);
    double F_SMedal = (F_FBurst - SMedal);
    double F_BEgg = (F_SMedal - BEgg);
    double F_SShock = (F_BEgg - SShock);
    double F_FSPlate = (F_SShock - FSPlate);
    double F_HSuper = (F_FSPlate - HSuper);
    double F_IStorm = (F_HSuper - IStorm);
    double F_MBox = (F_IStorm - MBox);
    double F_BBlock = (F_MBox - BBlock);
    double F_TRage = (F_BBlock - TRage);
    double F_MTonic = (F_TRage - MTonic);
    double F_RSDish = (F_MTonic - RSDish);
    double F_SStar = (F_RSDish - SStar);
    double F_LShroom = (F_SStar - LShroom);
    double F_MMeal = (F_LShroom - MMeal);
    double F_GMedal = (F_MMeal - GMedal);
    double F_USShake = (F_GMedal - USShake);
    double F_DDeluxe = (F_USShake - DDeluxe);

    // The rabbit is landing
     if (nipples == 0) {
            if (boobies >= F_TStew)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_TRIAL_PAN");
            else if (boobies >= F_SShake)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_HUNNY_KINOKO");
            else if (boobies >= F_FBurst)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_HONOO_SAKURETU");
            else if (boobies >= F_SMedal)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_STAR_MEDAL");
            else if (boobies >= F_BEgg)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_BIG_EGG");
            else if (boobies >= F_SShock)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_KOURA_DE_PON");
            else if (boobies >= F_FSPlate)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_KINOKO_FRY");
            else if (boobies >= F_HSuper)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_HUNNY_KINOKO_S");
            else if (boobies >= F_IStorm)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_KOORI_NO_IBUKI");
            else if (boobies >= F_MBox)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_NANIGA_OKORUKANA");
            else if (boobies >= F_BBlock)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_BARIA_FRAME");
            else if (boobies >= F_TRage)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_KAMINARI_DOKKAN");
            else if (boobies >= F_MTonic)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_TUYOTUYO_DRINK");
            else if (boobies >= F_RSDish)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_KINOKO_HOILE_FRY");
            else if (boobies >= F_SStar)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_KIRAKIRA_OTOSHI");
            else if (boobies >= F_LShroom)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_KINKYU_KINOKO");
            else if (boobies >= F_MMeal)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_DINNER");
            else if (boobies >= F_GMedal)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_GOLD_MEDAL");
            else if (boobies >= F_USShake)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_USE_ULTRA_DRINK");
            else if (boobies >= F_DDeluxe)
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_DOROCY_DELUX");
            else
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_TRIAL_PAN");
     }
        else {
                spm::dan::dan_wp->dungeons[no].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_TRIAL_PAN");
     }

        // Read map (bitflags for parts of the map to enable and disable in enemy rooms, 0 & unused elsewhere)
        spm::parse::parseTagGet1("<map>", spm::parse::PARSE_VALUE_TYPE_INT, &spm::dan::dan_wp->dungeons[no].map);
        spm::dan::dan_wp->dungeons[no].map = roomDecCode;
        
        // Read doors
        i = 0;
        while (spm::parse::parsePush("<door>"))
        {
            spm::parse::parseTagGet1("<enter>", spm::parse::PARSE_VALUE_TYPE_INT, &spm::dan::dan_wp->dungeons[no].doors[i].enter);
            spm::parse::parseTagGet1("<exit>", spm::parse::PARSE_VALUE_TYPE_INT, &spm::dan::dan_wp->dungeons[no].doors[i].exit);
            // Door substitution
            spm::dan::dan_wp->dungeons[no].doors[i].enter = doorEntrance;
            spm::dan::dan_wp->dungeons[no].doors[i].exit = doorExit;
            spm::parse::parsePopNext();
            i++;
        }

        spm::dan::dan_wp->dungeons[no].doorCount = i;
        
        // Enemy arrays per set of floors. First argument is name + 1000, second is odds of passing as a candidate / 100, third is spawn limit, fourth is "should it always be grounded?"
            // Chapter 1: add in 169 (Jawbus) later
            int enemiesCh1[17] = {1, 2, 5, 6, 8, 12, 15, 21, 26, 100, 126, 127, 135, 143, 441, 445, 451};
            // Chapter 2: 
            int enemiesCh2[27] = {1, 2, 5, 6, 8, 12, 15, 21, 26, 100, 126, 127, 135, 143, 441, 445, 451, 16, 23, 24, 85, 88, 90, 109, 151, 160, 246};
            // Chapter 3:
            int enemiesCh3[32] = {16, 23, 24, 85, 88, 90, 109, 151, 160, 246, 4, 13, 22, 35, 37, 46, 64, 65, 70, 72, 84, 89, 116, 123, 161, 164, 165, 174, 175, 178, 247, 443};
            // Chapter 4: add in 195 (Barribad) later
            int enemiesCh4[32] = {4, 13, 22, 35, 37, 46, 64, 65, 70, 72, 84, 89, 116, 123, 161, 164, 165, 174, 175, 178, 247, 443, 137, 145, 166, 167, 179, 180, 185, 189, 192, 201};
            // Chapter 5: add in 29 (Ceiling spike tops) and 170 (Rawbus) later
            int enemiesCh5[28] = {137, 145, 166, 167, 179, 180, 185, 189, 192, 201, 28, 33, 91, 95, 101, 103, 114, 118, 129, 204, 205, 206, 208, 210, 248, 249, 250, 258};
            // Chapter 7: 
            int enemiesCh7[33] = {28, 33, 91, 95, 101, 103, 114, 118, 129, 204, 205, 206, 208, 210, 248, 249, 250, 258, 31, 32, 36, 38, 40, 42, 52, 86, 110, 224, 227, 230, 233, 256, 325};
            // Chapter 8: add in 197 (Sobarribad) later
            int enemiesCh8[39] = {31, 32, 36, 38, 40, 42, 52, 86, 110, 224, 227, 230, 233, 256, 325, 48, 54, 58, 60, 74, 98, 105, 124, 139, 147, 153, 162, 171, 190, 193, 202, 225, 228, 231, 234, 238, 240, 242, 259};
            // All Enemies
            int enemiesAll[139] = {31, 32, 36, 38, 40, 42, 52, 86, 110, 224, 227, 230, 233, 256, 325, 48, 54, 58, 60, 74, 98, 105, 124, 139, 147, 153, 162, 171, 190, 193, 202, 225, 228, 231, 234, 238, 240, 242, 259, 
            1, 2, 5, 6, 8, 12, 15, 21, 26, 100, 126, 127, 135, 143, 441, 445, 451, 16, 23, 24, 85, 88, 90, 109, 151, 160, 246, 4, 13, 22, 35, 37, 46, 64, 65, 70, 72, 84, 89, 116, 123, 161, 164, 165, 174, 175, 178, 
            247, 443, 137, 145, 166, 167, 179, 180, 185, 189, 192, 201, 28, 33, 91, 95, 101, 103, 114, 118, 129, 204, 205, 206, 208, 210, 248, 249, 250, 258, 31, 32, 36, 38, 40, 42, 52, 86, 110, 224, 227, 230, 233, 256, 325,
            10, 131, 496, 497, 471, 472, 466, 467};
            // Random Bullshit: Headbonk, Squoinker, Spinia, Spunia, Flip Buzzy, Flip Spiky Buzzy, Shady, Flip Shady.. Magikoopa Family and Bleep/Kilo planned to be in with some modified stats. Maybe DBC :eyes:
            int enemiesOther[8] = {10, 131, 496, 497, 471, 472, 466, 467};

        // Read enemies
        i = 0;
        int arrayID = 0;
        int enemyTypes = 0;
        int enemyAmt = 0;
        while (spm::parse::parsePush("<enemy>"))
        {
            spm::parse::parseTagGet1("<name>", spm::parse::PARSE_VALUE_TYPE_INT, &spm::dan::dan_wp->dungeons[no].enemies[i].name);
            spm::parse::parseTagGet1("<num>", spm::parse::PARSE_VALUE_TYPE_INT, &spm::dan::dan_wp->dungeons[no].enemies[i].num);
            spm::parse::parseTagGet1("<pos>", spm::parse::PARSE_VALUE_TYPE_INT, &spm::dan::dan_wp->dungeons[no].enemies[i].pos);
            // Enemy Substition Protocol
                enemyTypes = spm::system::rand() % 4; // to match i = 0-3; 4 types max, 1 type minimum
                for (i = 0; i != enemyTypes; i = i + 1) {
                    if (i == 0) {
                        enemyAmt = spm::system::rand() % 3 + 2;
                    } else {
                        enemyAmt = spm::system::rand() % 2 + 1;
                    }
                    if (no >= 0 && no < 9) {
                        arrayID = spm::system::rand() % 17;
                        spm::dan::dan_wp->dungeons[no].enemies[i].name = enemiesCh1[arrayID];
                        spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                        spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                    } else if (no >= 10 && no < 19) {
                        arrayID = spm::system::rand() % 27;
                        spm::dan::dan_wp->dungeons[no].enemies[i].name = enemiesCh2[arrayID];
                        spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                        spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                    } else if (no >= 20 && no < 29) {
                        arrayID = spm::system::rand() % 32;
                        spm::dan::dan_wp->dungeons[no].enemies[i].name = enemiesCh3[arrayID];
                        spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                        spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                    } else if (no >= 30 && no < 39) {
                        arrayID = spm::system::rand() % 32;
                        spm::dan::dan_wp->dungeons[no].enemies[i].name = enemiesCh4[arrayID];
                        spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                        spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                    } else if (no >= 40 && no < 49) {
                        arrayID = spm::system::rand() % 28;
                        spm::dan::dan_wp->dungeons[no].enemies[i].name = enemiesCh5[arrayID];
                        spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                        spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                    } else if (no >= 50 && no < 59) {
                        arrayID = spm::system::rand() % 33;
                        spm::dan::dan_wp->dungeons[no].enemies[i].name = enemiesCh7[arrayID];
                        spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                        spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                    } else if (no >= 60 && no < 69) {
                        arrayID = spm::system::rand() % 39;
                        spm::dan::dan_wp->dungeons[no].enemies[i].name = enemiesCh8[arrayID];
                        spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                        spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                    } else if (no >= 70 && no < 98) {
                        arrayID = spm::system::rand() % 139;
                        spm::dan::dan_wp->dungeons[no].enemies[i].name = enemiesAll[arrayID];
                        spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                        spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                    } else if (no == 98) {
                        arrayID = spm::system::rand() % 8;
                        spm::dan::dan_wp->dungeons[no].enemies[i].name = enemiesOther[arrayID];
                        spm::dan::dan_wp->dungeons[no].enemies[i].num = enemyAmt;
                        spm::dan::dan_wp->dungeons[no].enemies[i].pos = 0;
                    }
                }
            spm::parse::parsePopNext();
        }
        spm::dan::dan_wp->dungeons[no].enemyCount = i;

        // Move to next dungeon
        spm::parse::parsePopNext();
    }

    // Free pit text
    spm::parse::parsePop();
    spm::memory::__memFree(0, decompPitText);

    return EVT_RET_CONTINUE;
}

static const char * (*msgSearchReal)(const char * msgName);
static void messagePatch()
{
	msgSearchReal = patch::hookFunction(spm::msgdrv::msgSearch,
		[](const char * msgName)
		{
			if (msl::string::strcmp(msgName, "D100_entrance_01") == 0)
				// flipside pit first entrance text
				return D100_entrance_01;
            else if (msl::string::strcmp(msgName, "mac_kanban_003") == 0)
				// flipside pit entrance sign
				return mac_kanban_003;
            else if (msl::string::strcmp(msgName, "modoridokan_D100_01") == 0)
				// flipside pit first exit text
				return modoridokan_D100_01;
            // else if (msl::string::strcmp(msgName, "m_noroi") == 0)
				// back cursya
				// return m_noroi;
			else
				return msgSearchReal(msgName);
		}
	);
}

// I literally had to port this entire thing just to remove a condition that makes certain enemies force the Pit to fucking crash upon entry. Fuck you Intelligent Systems. 6
s32 evt_dan_get_enemy_info_new(spm::evtmgr::EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon and enemy index
    spm::evtmgr::EvtScriptCode * args = entry->pCurData;    
    s32 no = spm::evtmgr_cmd::evtGetValue(entry, args[0]);
    s32 enemyIdx = spm::evtmgr_cmd::evtGetValue(entry, args[1]);
    spm::dan::DanDungeon * dungeon = spm::dan::dan_wp->dungeons + no;

    if ((enemyIdx < 0) || (enemyIdx >= 16))
    {
        // Return 0 for invalid enemies
        spm::evtmgr_cmd::evtSetValue(entry, args[2], 0);
        spm::evtmgr_cmd::evtSetValue(entry, args[3], 0);

        return EVT_RET_CONTINUE;
    }
    else
    {
        // Get enemy
        spm::dan::DanEnemy * enemy = dungeon->enemies + enemyIdx;

        if (enemy->num > 0)
        {
            // Find template with correct tribe id
            s32 tribeId = enemy->name - 1;
            s32 i;
            spm::npcdrv::NPCEnemyTemplate * curTemplate = spm::npcdrv::npcEnemyTemplates;
            for (i = 0; i < NPCTEMPLATE_MAX; i++, curTemplate++)
            {
                if (curTemplate->tribeId == tribeId)
                    break;
            }
            assertf(i < NPCTEMPLATE_MAX, "みつかりませんでした[%d]", tribeId);
            

            // Return template id and num
            spm::evtmgr_cmd::evtSetValue(entry, args[2], i);
            spm::evtmgr_cmd::evtSetValue(entry, args[3], enemy->num);
        }
        else
        {
            // Return 0 for empty enemy slots
            spm::evtmgr_cmd::evtSetValue(entry, args[2], 0);
            spm::evtmgr_cmd::evtSetValue(entry, args[3], 0);
        }

        return EVT_RET_CONTINUE;
    }
}

/* This code is so weirdly buggy. The RNG value displayed is different from the real RNG value - we believe that for whatever reason, RNG is being called more than once.
We don't know why. Do not enable this block and expect accurate results.
// Debug only; draws RNG value in the Pit
static spm::seqdef::SeqFunc *seq_gameMainReal;

void showBob(spm::seqdrv::SeqWork *wp) {
  if (boobies > 0){
  wii::gx::GXColor magenta = {255, 0, 155, 255};
  f32 scale = 0.8f;
  char buffer [50];
  msl::stdio::sprintf(buffer, "%lf", boobies);
  const char * msg = buffer;
  spm::fontmgr::FontDrawStart();
  spm::fontmgr::FontDrawEdge();
  spm::fontmgr::FontDrawColor(&magenta);
  spm::fontmgr::FontDrawScale(scale);
  spm::fontmgr::FontDrawNoiseOff();
  spm::fontmgr::FontDrawRainbowColorOff();
  f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
  spm::fontmgr::FontDrawString(x, 200.0f, msg);
  } seq_gameMainReal(wp);
}
void debugDisplay(spm::seqdrv::SeqWork *wp) {
  showBob(wp);
}
*/

static spm::seqdef::SeqFunc *seq_gameMainReal;

void youSuckDisplay(spm::seqdrv::SeqWork *wp) {
  if (youSuck == true) {
  wii::gx::GXColor funnyColor = {255, 255, 255, 255};
  f32 scale = 2.8f;
  char buffer [50];
  const char * youSuckText = "YOU SUCK";
  msl::stdio::sprintf(buffer, "%s", youSuckText);
  const char * msg = buffer;
  spm::fontmgr::FontDrawStart();
  spm::fontmgr::FontDrawEdge();
  spm::fontmgr::FontDrawColor(&funnyColor);
  spm::fontmgr::FontDrawScale(scale);
  spm::fontmgr::FontDrawNoiseOff();
  spm::fontmgr::FontDrawRainbowColor();
  f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
  spm::fontmgr::FontDrawString(x, 200.0f, msg);
  }
  seq_gameMainReal(wp);
}

void new_dan_gameover() {
  youSuck = true;
}

static spm::seqdef::SeqFunc *seq_titleMainReal;
static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp)
{
    wii::gx::GXColor notgreen = {230, 116, 216, 255};
    f32 scale = 0.8f;
    const char * msg = "Flipside Pit Revamped alpha v0";
    spm::fontmgr::FontDrawStart();
    spm::fontmgr::FontDrawEdge();
    spm::fontmgr::FontDrawColor(&notgreen);
    spm::fontmgr::FontDrawScale(scale);
    spm::fontmgr::FontDrawNoiseOff();
    spm::fontmgr::FontDrawRainbowColorOff();
    f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
    spm::fontmgr::FontDrawString(x, 200.0f, msg);
    seq_titleMainReal(wp);
}
static void titleScreenCustomTextPatch()
{
    seq_titleMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
    seq_gameMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main = &youSuckDisplay;
}

/*
    // Handles spawning extra items
    s32 (*evt_dan_get_chest_room_item)(spm::evtmgr::EvtEntry * evtEntry, bool firstRun);
    s32 new_get_chest_room_item(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
    spm::evtmgr::EvtVar * args = (spm::evtmgr::EvtVar *)evtEntry->pCurData; 
    s32 roomNum = args[0];

    if (roomNum == 9 && firstRun == true)
        {
            spm::itemdrv::itemEntry("item", 174, 0, 0, 0, 0, 0, 0);
        }
    return evt_dan_get_chest_room_item(evtEntry, firstRun);
    }
*/
static void danOverwrite()
{
    patch::hookFunction(spm::dan::evt_dan_read_data, evt_dan_read_data_new);
}

static void danYouSuck()
{
    patch::hookFunction(spm::dan::danCountdownDone, new_dan_gameover);
}

static void danDontFuckingCrash()
{
    patch::hookFunction(spm::dan::evt_dan_get_enemy_info, evt_dan_get_enemy_info_new);
}

/*
static void danBonusItems()
{
    evt_dan_get_chest_room_item = patch::hookFunction(spm::dan::evt_dan_get_chest_room_item, new_get_chest_room_item);
}
*/


void main()
{
    wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
    titleScreenCustomTextPatch();
//other functions
messagePatch();
danOverwrite();
danYouSuck();
danDontFuckingCrash();
// danBonusItems();
}

}