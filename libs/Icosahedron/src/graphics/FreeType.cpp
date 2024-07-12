#include <Icosahedron/graphics/FreeType.h>
#include <iostream>

#include <cmath>
#include <cstring>


ic::TextAtlas ic::FreeType::load_atlas(const char *filePath) {
    /* load font file */
    long size;
    unsigned char *fontBuffer;
    
    FILE* fontFile = fopen(filePath, "rb");
    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile); /* how long is the file ? */
    fseek(fontFile, 0, SEEK_SET); /* reset */
    
    fontBuffer = (unsigned char*) malloc(size);
    
    fread(fontBuffer, size, 1, fontFile);
    fclose(fontFile);



    /* prepare font */
    stbtt_fontinfo info;
    if (!stbtt_InitFont(&info, fontBuffer, 0))
    {
        std::cout << "Couldn't initialize font via stb_truetype." << "\n";
    }
    
    int b_w = 512; /* bitmap width */
    int b_h = 128; /* bitmap height */
    int l_h = 64; /* line height */

    /* create a bitmap for the phrase */
    unsigned char* bitmap = (unsigned char*) calloc(b_w * b_h, 1);
    
    /* calculate font scaling */
    float scale = stbtt_ScaleForPixelHeight(&info, l_h);

    const char *word = "the quick brown fox";
    
    int x = 0;
       
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    
    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);
    
    for (int i = 32; i < 128; i++) {
        /* how wide is this character */
        int ax;
	    int lsb;

        stbtt_GetCodepointHMetrics(&info, word[i], &ax, &lsb);
        /* (Note that each Codepoint call has an alternative Glyph version which caches the work required to lookup the character word[i].) */

        /* get bounding box for character (may be offset to account for chars that dip above or below the line) */
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
        
        /* compute y (different characters have different heights) */
        int y = ascent + c_y1;
        
        /* render character (stride and offset is important here) */
        int byteOffset = x + roundf(lsb * scale) + (y * b_w);
        stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, word[i]);

        /* advance x */
        x += roundf(ax * scale);
        
        /* add kerning */
        int kern;
        kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
        x += roundf(kern * scale);
    }
    
    free(fontBuffer);
    free(bitmap);
}