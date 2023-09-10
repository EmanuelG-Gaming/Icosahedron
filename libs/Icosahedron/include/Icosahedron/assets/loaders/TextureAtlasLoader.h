#ifndef IC_TEXTURE_ATLAS_LOADER_H
#define IC_TEXTURE_ATLAS_LOADER_H

namespace ic {
    enum class TextureAtlasPacking {
        lineByLine,
        horizontal,
        vertical,
    };

    class TextureAtlasLoader {
        public:
            static TextureAtlasLoader& get() {
                static TextureAtlasLoader ins;
                return ins;
            }

            
        private:
            TextureAtlasLoader() {}
            ~TextureAtlasLoader() {}

        public:
            TextureAtlasLoader(TextureAtlasLoader const&) = delete;
            void operator = (TextureAtlasLoader const&) = delete;
    };
}
#endif