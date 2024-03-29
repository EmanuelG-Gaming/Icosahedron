#include <Icosahedron/graphics/FreeType.h>
#include <filesystem>

using namespace ic;

void FreeType::load() {
    errorHandler = FT_Init_FreeType(&library);
    if (errorHandler) {
        throw std::runtime_error("FreeType library couldn't load.");
    }
}

TextAtlas FreeType::add_atlas(const std::string &name, const std::string &relativeFile, int height) {
    FT_Face font;
    std::string path = std::filesystem::current_path().string() + "/" + relativeFile;
    errorHandler = FT_New_Face(library, relativeFile.c_str(), 0, &font);

    if (errorHandler == FT_Err_Unknown_File_Format) {
        throw std::runtime_error("The font file has an unknown format."); 
    } else if (errorHandler) {
        throw std::runtime_error("At: " + path + ": Other error that occured when loading font. Perhaps the file name is not correct?");
    }
              
    FT_Set_Pixel_Sizes(font, 0, height);
    TextAtlas atlas(font);
    atlas.load();
              
    atlases[name] = atlas;

    return atlas;
}

TextAtlas &FreeType::find_atlas(const std::string &name) {
    return atlases[name];
}
              
void FreeType::dispose() {
    for (auto &atlas : atlases) {
        TextAtlas &second = atlas.second;
        second.dispose();
    }
    FT_Done_FreeType(library);
}
