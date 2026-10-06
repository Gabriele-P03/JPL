#pragma once
#include "Font.h"

// Carica un .ttf con stb_truetype, genera l'atlas (GL_R8) e riempie `out`.
// Copre i caratteri ASCII 32..127 (96 glifi).
// pixelHeight: altezza in pixel a cui viene rasterizzato il font.
// Ritorna false se il file manca o se l'atlas e' troppo piccolo.
bool loadFontTTF(Font& out, const char* path, float pixelHeight, int atlasSize = 512);

void destroyFont(Font& f);
