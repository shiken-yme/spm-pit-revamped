#pragma once

#include <wii/types.h>

#include <cstdint>

namespace spm::fontmgr {

extern "C" {

void fontmgrInit();
void FontDrawStart();
void FontDrawStart_alpha(uint8_t alpha);
void FontDrawEdge();
void FontDrawEdgeOff();
void FontDrawRainbowColor();
void FontDrawRainbowColorOff();
void FontDrawNoise();
void FontDrawNoiseOff();
void FontDrawColorIDX(uint32_t idx);
void FontDrawColor(struct wii::RGBA * color);
void FontDrawColor_();
uint32_t FontGetDrawColor();
void FontDrawScale(float scale);
void FontDrawCode(float x, float y, uint16_t code);
void FontDrawCodeMtx(float matrix[3][4], uint16_t code);
void FontDrawString(float x, float y, const char * string);
void FontDrawStringVecPitch(float position[3], float pitch, const char * string);
void FontDrawStringMtx(float matrix[3][4], const char * string);
void FontDrawStringCenterMtx(float matrix[3][4], const char * string);
void FontDrawMessage(int x, int y, const char * message);
void FontDrawMessageMtx(float matrix[3][4], const char * message);
uint16_t hankakuSearch(uint8_t code);
uint16_t kanjiSearch(uint16_t code);
uint8_t kanjiGetWidth(uint16_t code);
uint16_t FontGetMessageWidthLine(const char * message, uint16_t * outlines);
uint16_t FontGetMessageWidth(const char * message);
uint32_t HSV2RGB(uint8_t hsv[3]);

// more

}

}