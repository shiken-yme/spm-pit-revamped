#include "mod.h"
#include "patch.h"

#include <spm/evt_msg.h>
#include <spm/msgdrv.h>
#include <spm/camdrv.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/seqdef.h>
#include <wii/os/OSError.h>
#include <wii/gx.h>

namespace mod {
/*
    Title Screen Custom Text
    Prints "SPM Rel Loader" at the top of the title screen
*/

static spm::seqdef::SeqFunc *seq_titleMainReal;
static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp)
{
    wii::gx::GXColor notgreen = {230, 116, 216, 0};
    f32 scale = 0.8f;
    const char * msg = "Big Gay Balls";
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
}

/*
    General mod functions
*/

// New message
const char * D100_entrance_override = 
"<system>\n"
"This is the entrance to a\n"
"dangerous dungeon called the\n"
"Pit of 100 Trials.\n"
"<k>\n<p>\n"
"Once you enter, it's not easy\n"
"to leave. Let me tell you a bit\n"
"about what you're getting into.\n"
"<k>\n<p>\n"
"So first, you've got the\n"
"<col c00000ff>Goombas</col>. Those guys are total\n"
"pushovers. Don't worry about 'em.\n"
"<k>\n<p>\n"
"Then you've got the <col c00000ff>Koopas.</col>\n"
"I've spent my whole life stompin'\n"
"'em. Tough bastards.\n"
"<k>\n<p>\n"
"Oh, then there's <col c00000ff>Shlurps.</col>\n"
"<wait 250>They're impossible. Don't even\n"
"try defeating those guys.\n"
"<k>\n<p>\n"
"... Unless you have some kind\n"
"of bomb.<wait 250> If they swallow one,\n"
"they'll probably die.\n"
"<k>\n<p>\n"
"<wait 250>... I mean, to be fair,\n"
"what the hell wouldn't\n"
"die if it imploded?.\n"
"<k>\n<p>\n"
"Actually, don't answer that.<wait 250>\n"
"It doesn't even matter.\n"
"<k>\n<p>\n"
"After all...<wait 500>\n"
"what kind of cheater would\n"
"bring bombs to the Pit?\n"
"<k>\n<p>\n"
"<shake>You know, back in my day,\n"
"we didn't <wait 250><dynamic 3>HAVE</dynamic> any bombs.\n"
"Got by fine without 'em.\n"
"<k>\n<p>\n"
"Y'know what we called it?\n"
"\"Boomerless Pit.\"\n"
"<k>\n<p>\n"
"<wait 250>... Well, I don't know\n"
"what a boomer is, but it\n"
"sounds evil.<wait 250> And smelly.\n"
"<k>\n<p>\n"
"It also sounds a bit\n"
"like 'bomb.' And I don't\n"
"like that. Bombs are cheating.\n"
"<k>\n<p>\n"
"Where was I again?\n"
"<wait 500>Right. This generation always\n"
"taking the easy way out.\n"
"<k>\n<p>\n"
"Excuse me for a second.\n\n"
"<scale 0.67><shake>Ahem...</shake></scale>\n"
"<k>\n<p>\n"
"<dynamic 3><scale 1.8><wait 500>SHAME<wait 500> ON<wait 500> YOU!!!</scale>\n"
"Bitch!\n"
"</dynamic>\n<k>\n<p>"
"And you know what, son?\n"
"I've got one more thing\n"
"to tell you.\n"
"</system>\n<kanban>\n<k>\n<p>\n"
"<col c00000ff><center>Big Gay Balls</center></col>\n"
"<k>\n<p>\n"
"</kanban><wait 500><system>\n"
"So...<wait 500> do you still want\n"
"to go in?\n"
"<o>";

static const char * (*msgSearchReal)(const char * msgName);

// Message override, hopefully
static void messagePatch()
{
	msgSearchReal = patch::hookFunction(spm::msgdrv::msgSearch,
		[](const char * msgName)
		{
			if (msl::string::strcmp(msgName, "D100_entrance_01") == 0)
				// Override flipside pit entrance text
				return D100_entrance_01;
			else if (msl::string::strcmp(msgName, "D100_entrance_override") == 0)
				// Add custom tutorial message
				return D100_entrance_override;
			else
				return msgSearchReal(msgName);
		}
	);
}

void main()
{
    wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
    titleScreenCustomTextPatch();
//other functions
}

}
