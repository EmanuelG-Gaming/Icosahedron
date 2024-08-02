#include <Icosahedron/assets/loaders/FontLoader.h>
#include <iostream>

#include <cmath>
#include <cstring>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>


ic::TextAtlas ic::FontLoader::load(const char *filePath, int atlasWidth, int atlasHeight, int characterSize, int glyphOversamplingX, int glyphOversamplingY, int firstCharacter, int numberOfCharacters) {

    ic::TextAtlas result;

    /* load font file */
    long size;
    unsigned char* fontVectorBuffer;

    FILE* fontFile = fopen(filePath, "rb");
    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile); /* how long is the file ? */
    fseek(fontFile, 0, SEEK_SET); /* reset */

    fontVectorBuffer = (unsigned char*)malloc(size);

    fread(fontVectorBuffer, size, 1, fontFile);
    fclose(fontFile);

    /* prepare font */
    stbtt_fontinfo info;
    if (!stbtt_InitFont(&info, fontVectorBuffer, 0))
    {
        printf("failed\n");
    }



    unsigned char* bitmap = (unsigned char*)calloc(atlasWidth * atlasHeight, sizeof(unsigned char));

    // Font rasterization
    stbtt_packedchar cdata[128];
    stbtt_pack_context packContext;

    if (!stbtt_PackBegin(&packContext, bitmap, atlasWidth, atlasHeight, 0, 1, nullptr)) {
        printf("Failed to initialize font");
    }

    // Mitigate blurry edges on text
    stbtt_PackSetOversampling(&packContext, glyphOversamplingX, glyphOversamplingY);
        
    if (!stbtt_PackFontRange(&packContext, fontVectorBuffer, 0, characterSize, firstCharacter, numberOfCharacters, cdata)) {
        printf("Failed to pack font, Probably the font's atlas size is too small or the font oversampling at an axis is 0.");
    }

    stbtt_PackEnd(&packContext);

    result.firstGlyph = firstCharacter;
    result.glyphCount = numberOfCharacters;
    result.characters = new ic::CharacterInfo[result.glyphCount];
    
    result.atlasWidth = atlasWidth;
    result.atlasHeight = atlasHeight;
    result.fontHeight = characterSize;

    for (int i = 0; i < 128; i++) {
        float offsetX = 0.0f, offsetY = 0.0f;

        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(cdata, atlasWidth, atlasHeight,
                            i - firstCharacter, &offsetX, &offsetY, &q, 1);

        ic::CharacterInfo glyph;
        glyph.u0 = q.s0; glyph.v0 = q.t0;
        glyph.u1 = q.s1; glyph.v1 = q.t1;

        glyph.p0x = q.x0; glyph.p0y = -q.y0;
        glyph.p1x = q.x1; glyph.p1y = -q.y1;

        glyph.advance = offsetX;

        result.characters[i] = glyph;
    }

    result.initialize_texture(bitmap);

    free(fontVectorBuffer);
    free(bitmap);


    return result;
}