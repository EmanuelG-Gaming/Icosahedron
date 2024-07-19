#include <Icosahedron/assets/loaders/FontLoader.h>
#include <iostream>

#include <cmath>
#include <cstring>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>


ic::TextAtlas ic::FontLoader::load(const char *filePath, const char *text, int fontWidth, int fontHeight, int lineHeight) {
    ic::TextAtlas result;

    /* load font file */
    long size;
    unsigned char* fontBuffer;

    FILE* fontFile = fopen(filePath, "rb");
    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile); /* how long is the file ? */
    fseek(fontFile, 0, SEEK_SET); /* reset */

    fontBuffer = (unsigned char*)malloc(size);

    fread(fontBuffer, size, 1, fontFile);
    fclose(fontFile);

    /* prepare font */
    stbtt_fontinfo info;
    if (!stbtt_InitFont(&info, fontBuffer, 0))
    {
        printf("failed\n");
    }

    int b_w = fontWidth; /* bitmap width */
    int b_h = fontHeight; /* bitmap height */
    int l_h = lineHeight; /* line height */

    /* create a bitmap for the phrase */
    unsigned char* bitmap = (unsigned char*)calloc(b_w * b_h, sizeof(unsigned char));

    /* calculate font scaling */
    float scale = stbtt_ScaleForPixelHeight(&info, l_h);

    int x = 0;

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

    ascent *= scale;
    descent *= scale;

    int i;
    for (i = 0; i < strlen(text); ++i) {
        /* get bounding box for character (may be offset to account for chars that dip above or below the line */
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, text[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

        /* compute y (different characters have different heights */
        int y = ascent + c_y1;

        /* render character (stride and offset is important here) */
        int byteOffset = x + (y  * b_w);
        stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, text[i]);

        /* how wide is this character */
        int ax;
        stbtt_GetCodepointHMetrics(&info, text[i], &ax, 0);
        x += ax * scale;

        /* add kerning */
        int kern;
        kern = stbtt_GetCodepointKernAdvance(&info, text[i], text[i + 1]);
        x += kern * scale;

        ic::CharacterInfo glyph;
        glyph.u1 = c_x1 / (float) b_w;
        glyph.v1 = c_y1 / (float) b_h;
        glyph.u2 = c_x2 / (float) b_w;
        glyph.v2 = c_y2 / (float) b_h;

        glyph.width = glyph.u2 - glyph.u1;
        glyph.height = glyph.v2 - glyph.v1;

        glyph.shift = ax / (float) b_w;
    }

    result.atlasWidth = b_w;
    result.atlasHeight = b_h;

    result.initialize_texture(bitmap);

    free(fontBuffer);
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