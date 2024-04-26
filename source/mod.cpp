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
   double ShroomShake = msl::math::pow(300, floorwiseMultiplier);
   double FireBurst = msl::math::pow(250, floorwiseMultiplier);
   double StarMedal = msl::math::pow(200, floorwiseMultiplier);
   double BigEgg = msl::math::pow(180, floorwiseMultiplier);
   double ShellShock = msl::math::pow(160, floorwiseMultiplier);
   double FriedShroomPlate = msl::math::pow(130, floorwiseMultiplier);
   double HoneySuper = msl::math::pow(120, floorwiseMultiplier);
   double IceStorm = msl::math::pow(100, floorwiseMultiplier);
   double MysteryBox = msl::math::pow(73, floorwiseMultiplier);
   double BlockBlock = msl::math::pow(64, floorwiseMultiplier);
   double ThunderRage = msl::math::pow(60, floorwiseMultiplier);
   double MightyTonic = msl::math::pow(50, floorwiseMultiplier);
   double RoastedShroomDish = msl::math::pow(40, floorwiseMultiplier);
   double ShootingStar = msl::math::pow(30, floorwiseMultiplier);
   double LifeShroom = msl::math::pow(30, floorwiseMultiplier);
   double MeteorMeal = msl::math::pow(25, floorwiseMultiplier);
   double GoldMedal = msl::math::pow(25, floorwiseMultiplier);
   double UltraShroomShake = msl::math::pow(20, floorwiseMultiplier);
   double DyllisDeluxe = msl::math::pow(10, floorwiseMultiplier);
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
            // Map... Remapping...? Protocol
            switch(no) {
                case 0:
                if (boobies <= 40) {
                spm::dan::dan_wp->dungeons[no].map = 1300;
                } else {
                spm::dan::dan_wp->dungeons[no].map = 33816;
                } break;
                case 1:
                spm::dan::dan_wp->dungeons[no].map = 558598;
                break;
                case 2:
                spm::dan::dan_wp->dungeons[no].map = 558598;
                break;
                case 3:
                spm::dan::dan_wp->dungeons[no].map = 558598;
                break;
                case 4:
                spm::dan::dan_wp->dungeons[no].map = 558598;
                break;
                case 5:
                spm::dan::dan_wp->dungeons[no].map = 558598;
                break;
                case 6:
                spm::dan::dan_wp->dungeons[no].map = 558598;
                break;
                case 7:
                spm::dan::dan_wp->dungeons[no].map = 558598;
                break;
                case 8:
                spm::dan::dan_wp->dungeons[no].map = 558598;
                break;
                
            }

        // Read doors
        while (spm::parse::parsePush("<door>"))
        {
            spm::parse::parseTagGet1("<enter>", spm::parse::PARSE_VALUE_TYPE_INT, &spm::dan::dan_wp->dungeons[no].doors[i].enter);
            spm::parse::parseTagGet1("<exit>", spm::parse::PARSE_VALUE_TYPE_INT, &spm::dan::dan_wp->dungeons[no].doors[i].exit);
            // Door substitution Will be Real in 40 smin tits . Oh yeah . Hee hehehehe heh owo :3 XDXDXD x3 Hoohohoohohooo xDDDD
             switch(no) {
                case 0:
                if (boobies <= 60) {
                spm::dan::dan_wp->dungeons[no].doors[i].enter = 16;
                spm::dan::dan_wp->dungeons[no].doors[i].exit = 28;
                }
                else {
                spm::dan::dan_wp->dungeons[no].doors[i].enter = 15;
                spm::dan::dan_wp->dungeons[no].doors[i].exit = 27;
                }
                break;
                case 1:
                if (boobies >= 40) {
                spm::dan::dan_wp->dungeons[no].doors[i].enter = 2;
                spm::dan::dan_wp->dungeons[no].doors[i].exit = 18;
                }
                else {
                spm::dan::dan_wp->dungeons[no].doors[i].enter = 29;
                spm::dan::dan_wp->dungeons[no].doors[i].exit = 13;
                }
                break;
                case 2:

             }
            spm::parse::parsePopNext();
            i++;
        }

        spm::dan::dan_wp->dungeons[no].doorCount = i;

        // Read enemies
        i = 0;
        while (spm::parse::parsePush("<enemy>"))
        {
            spm::parse::parseTagGet1("<name>", spm::parse::PARSE_VALUE_TYPE_INT, &spm::dan::dan_wp->dungeons[no].enemies[i].name);
            spm::parse::parseTagGet1("<num>", spm::parse::PARSE_VALUE_TYPE_INT, &spm::dan::dan_wp->dungeons[no].enemies[i].num);
            spm::parse::parseTagGet1("<pos>", spm::parse::PARSE_VALUE_TYPE_INT, &spm::dan::dan_wp->dungeons[no].enemies[i].pos);
            // Enemy Substition Protocol
             switch(no) {
                case 0:
                    // Room 1: Goombas, Koopas OR Goombas, Red Koopas OR Goombas
                    i = 0;
                if (boobies >= 40) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 1;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 5;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 12;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 4;
                } else if (boobies >= 10) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 1;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 4;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 15;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 4;
                } else {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 1;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 12;
                }
                break;
                case 1:
                    // Room 2: Squiglets, Squigs, Paragoombas OR Squigs, Squiglets OR Squigs
                    i = 0;
                if (boobies >= 40) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 126;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 4;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 127;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 8;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                } else if (boobies >= 10) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 126;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 127;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 5;
                } else {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 127;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 9;
                }
                break;
                case 2:
                    // Room 3: Ice Cherbils, Flip Goombas OR Cherbils, Paragoombas OR Cherbils, Ice Cherbils
                    i = 0;
                if (boobies >= 40) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 445;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 2;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 6;
                } else if (boobies >= 10) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 441;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 3;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 8;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 4;
                } else {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 441;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 445;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 3;
                }
                break;
                case 3:
                    // Room 4: Sproing-Oings, Parakoopas OR Buzzy Beetles, Bald Clefts OR Sproing-Oings
                    i = 0;
                if (boobies >= 40) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 135;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 3;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 21;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 4;
                } else if (boobies >= 10) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 26;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 3;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 100;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 4;
                } else {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 135;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 6;
                }
                break;
                case 4:
                    // Room 5: Buzzy Beetles, Spiked Goombas OR Ice Cherbils, Boomboxers OR Spiked Goombas
                    i = 0;
                if (boobies >= 40) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 26;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 6;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 5;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 4;
                } else if (boobies >= 10) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 445;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 143;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 4;
                } else {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 6;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 5;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 6;
                }
                break;
                case 5:
                    // Room 6: Boomboxer, Bald Cleft OR Paragoomba, Parakoopa OR Paragoomba, Bald Cleft
                    i = 0;
                if (boobies >= 40) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 143;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 3;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 100;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                } else if (boobies >= 10) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 8;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 4;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 21;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 3;
                } else {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 8;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 3;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 100;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 5;
                }
                break;
                case 6:
                    // Room 7: Flip Goomba, Koopa, Frackle OR Jawbus, Bald Cleft OR Goomba, Flip Goomba, Paragoomba, Spiked Goomba
                    i = 0;
                if (boobies >= 40) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 2;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 12;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 3;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 451;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 1;
                    spm::dan::dan_wp->dungeons[no].enemies[i].pos = 28;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 451;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 1;
                    spm::dan::dan_wp->dungeons[no].enemies[i].pos = 32;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 451;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 1;
                    spm::dan::dan_wp->dungeons[no].enemies[i].pos = 30;
                } else if (boobies >= 10) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 169;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 100;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 5;
                } else {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 1;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 2;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 8;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 6;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                }
                break;
                case 7:
                // Room 8: Sproing-Oing, Squig, Jawbus OR Koopa, Frackles OR 2 Buzzy Beetle, 2 Red Koopa, 2 Frackles
                    i = 0;
                if (boobies >= 40) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 135;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 127;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 4;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 169;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                } else if (boobies >= 10) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 12;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 5;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 451;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 1;
                    spm::dan::dan_wp->dungeons[no].enemies[i].pos = 29;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 451;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 1;
                    spm::dan::dan_wp->dungeons[no].enemies[i].pos = 30;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 451;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 1;
                    spm::dan::dan_wp->dungeons[no].enemies[i].pos = 27;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 451;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 1;
                    spm::dan::dan_wp->dungeons[no].enemies[i].pos = 28;
                } else {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 15;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 26;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 451;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 1;
                    spm::dan::dan_wp->dungeons[no].enemies[i].pos = 28;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 451;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 1;
                    spm::dan::dan_wp->dungeons[no].enemies[i].pos = 31;
                }
                break;
                case 8:
                    // Room 9: Squig, Buzzy Beetle OR Squiglet, Squig, Bald Cleft OR Megabite
                    i = 0;
                if (boobies >= 40) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 127;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 5;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 26;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 1;
                } else if (boobies >= 10) {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 126;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 3;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 127;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 2;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 100;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 4;
                } else {
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 255;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 1;
                }
                break;
                case 10:
                    // Room 11: Spiked Goombas, Red Koopa, Swoopers OR 
                    i = 0;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 26;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 3;
                        ++i;
                    spm::dan::dan_wp->dungeons[no].enemies[i].name = 26;
                    spm::dan::dan_wp->dungeons[no].enemies[i].num = 1;
                break;
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

/*
    Title Screen Custom Text
    Prints "SPM Rel Loader" at the top of the title screen
*/

static spm::seqdef::SeqFunc *seq_titleMainReal;
static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp)
{
    wii::gx::GXColor notgreen = {230, 116, 216, 255};
    f32 scale = 0.8f;
    const char * msg = "Flipside Pit Revamped alpha v???";
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
  //  seq_gameMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main;
  //  spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main = &debugDisplay;
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
// danBonusItems();
}

}