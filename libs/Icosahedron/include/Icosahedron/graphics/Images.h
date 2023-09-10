#ifndef IC_IMAGES_H
#define IC_IMAGES_H

#include <Icosahedron/graphics/Image.h>


namespace ic {
    class Images {
        public:
            static Images& get() {
                static Images ins;
                return ins;
            }

            ic::Image grayscale(ic::Image source);

        private:
            Images() {}
            ~Images() {}

        public:
            Images(Images const&) = delete;
            void operator = (Images const&) = delete;
    };
}
#endif