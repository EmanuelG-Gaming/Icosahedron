#include <Icosahedron/assets/loaders/FontLoader.h>
#include <iostream>

#include <cmath>
#include <cstring>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>


ic::TextAtlas ic::FontLoader::load(const char *filePath, int fontWidth, int fontHeight, int lineHeight) {

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

    int b_w = fontWidth; /* bitmap width */
    int b_h = fontHeight; /* bitmap height */
    int l_h = lineHeight; /* line height */


    unsigned char* bitmap = (unsigned char*)calloc(b_w * b_h, sizeof(unsigned char));

    // Font rasterization
    stbtt_bakedchar cdata[128];
    stbtt_BakeFontBitmap(fontVectorBuffer, stbtt_GetFontOffsetForIndex(fontVectorBuffer, 0), lineHeight, bitmap, fontWidth, lineHeight, 0, 128, cdata);

    result.atlasWidth = b_w;
    result.atlasHeight = lineHeight;

    float x = 0.0f, y = 0.0f;
    for (int i = 0; i < 128; i++) {
        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(cdata, fontWidth, fontHeight, i, &x, &y, &q, 1);

        ic::CharacterInfo glyph;
        glyph.u0 = q.s0; glyph.v0 = q.t0;
        glyph.u1 = q.s1; glyph.v1 = q.t1;

        glyph.p0x = q.x0 / (float) fontWidth; glyph.p0y = q.y0 / (float) lineHeight;
        glyph.p1x = q.x1 / (float) fontWidth; glyph.p1y = q.y1 / (float) lineHeight;

        glyph.width = glyph.u1 - glyph.u0;
        glyph.height = glyph.v1 - glyph.v0;

        glyph.shift = x / (float) fontWidth;

        result.characters[i] = glyph;

        //std::cout << glyph.u0 << " " << glyph.v0 << " " << glyph.u1 << " " << glyph.v1 << "\n";
        std::cout << glyph.p0x << " " << glyph.p0y << "\n";

    }

    result.initialize_texture(bitmap);

    free(fontVectorBuffer);
    free(bitmap);


    return result;


    /*
    ic::TextAtlas result;

    // Load font file 
    unsigned char ttfBuffer[1<<20];
    

    FILE *fontFileHandler = fopen(filePath, "rb");
    fread(ttfBuffer, 1, 1<<20, fontFileHandler);
    fclose(fontFileHandler);

    ///////////////////////////////////////////////////


    // Prepare font
    stbtt_fontinfo info;
    if (!stbtt_InitFont(&info, ttfBuffer, 0))
    {
        std::cout << "Couldn't initialize font via stb_truetype..." << "\n";
        return result;
    }
    
    int atlasBitmapWidth = fontWidth; // Bitmap width
    int atlasBitmapHeight = fontHeight; // Bitmap height
    int lineHeight = 64; // Line height (might not be equal to the letter's real height)

    // Create a bitmap for the atlas
    unsigned char* atlasBitmap = (unsigned char*) calloc(atlasBitmapWidth * atlasBitmapHeight, 1);
    
    // calculate font scaling
    float scale = stbtt_ScaleForPixelHeight(&info, lineHeight);

    
    int x = 0;
       
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    
    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);
    
    for (int i = 32; i < 128; i++) {
        // How wide is this character?
        int advanceX = 0;
	    int lsb = 0;

        char word = (char) i, nextWord = (char) (i + 1);

        stbtt_GetCodepointHMetrics(&info, word, &advanceX, &lsb);
        // (Note that each Codepoint call has an alternative Glyph version which caches the work required to lookup the character word[i].)

        // Get bounding box for character (may be offset to account for chars that dip above or below the line)
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, word, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
        
        // Compute y (different characters have different heights)
        int y = ascent + c_y1;
        
        // Render character (stride and offset is important here)
        int byteOffset = x + roundf(lsb * scale) + (y * atlasBitmapWidth);
        stbtt_MakeCodepointBitmap(&info, atlasBitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, atlasBitmapWidth, scale, scale, i);

        // Advance x
        x += roundf(advanceX * scale);
        
        // Add kerning (space between letters)
        int kern = stbtt_GetCodepointKernAdvance(&info, word, nextWord);
        x += roundf(kern * scale);


        // At the end of the loop, append the glyph data that's later used for caching...
        ic::CharacterInfo glyph;
        glyph.u1 = c_x1 / (float) atlasBitmapWidth;
        glyph.v1 = c_y1 / (float) atlasBitmapHeight;
        glyph.u2 = c_x2 / (float) atlasBitmapWidth;
        glyph.v2 = c_y2 / (float) atlasBitmapHeight;

        glyph.width = glyph.u2 - glyph.u1;
        glyph.height = glyph.v2 - glyph.v1;

        glyph.shift = advanceX / (float) atlasBitmapWidth;

        
        result.characters[i] = glyph;
    }

    

    
    
    
    return result;
    */
}