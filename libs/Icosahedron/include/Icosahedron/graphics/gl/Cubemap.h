#ifndef IC_CUBEMAP_H
#define IC_CUBEMAP_H

#include <glad/glad.h>

namespace ic {
    class Cubemap {
        public:
            Cubemap();

            ic::Cubemap &setup();
            
            void use(int index = 0);
            void unuse();

            void dispose();
        
        private:
            GLuint textureIndex = 0;
    };
}
#endif