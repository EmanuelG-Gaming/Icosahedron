#ifndef IC_COLOR_H
#define IC_COLOR_H

#include <iostream>

#include <string>
#include <stdint.h>
#include <Icosahedron/math/geom/Vectors.h>

using color_t = uint8_t;

namespace ic {
    /** @brief Stores an RGB color with components ranging from 0 to 255. */
    struct Color {
        color_t r, g, b;

        Color() {}
        Color(color_t r, color_t g, color_t b) : r(r), g(g), b(b) {}
        Color(ic::Vec3f source) : r((color_t) source[0]), g((color_t) source[1]), b((color_t) source[2]) {}
        
        Color operator*(Color &other) {
            float f = 255.0f;

            float nr1 = r / f;
            float ng1 = g / f;
            float nb1 = b / f;

            float nr2 = other.r / f;
            float ng2 = other.g / f;
            float nb2 = other.b / f;

            Color result;

            result.r = (color_t)((nr1 * nr2) * f);
            result.g = (color_t)((ng1 * ng2) * f);
            result.b = (color_t)((nb1 * nb2) * f);

            return result;
        }
        Color operator*(float value) {
            Color result;

            result.r = (color_t)(r * value);
            result.g = (color_t)(g * value);
            result.b = (color_t)(b * value);

            return result;
        }

        Color operator+(Color &other) {
            Color result;

            result.r = r + other.r;
            result.g = g + other.g;
            result.b = b + other.b;

            return result;
        }
        Color operator-(Color &other) {
            Color result;

            result.r = r - other.r;
            result.g = g - other.g;
            result.b = b - other.b;

            return result;
        }
        

        Color hexadecimal_to_RGB(int hexadecimal) {
            r = (hexadecimal >> 16) & 0xFF;
            g = (hexadecimal >> 8) & 0xFF;
            b = (hexadecimal) & 0xFF;

            return *this;
        }

        /** @brief Converts a base 16 description without the hash sign into an RGB color. */
        Color hexadecimal_to_RGB(const std::string &stringValue) {
            int tempR, tempG, tempB;
            sscanf(stringValue.c_str(), "%02x%02x%02x", &tempR, &tempG, &tempB);
            
            r = (color_t) tempR;
            g = (color_t) tempG;
            b = (color_t) tempB;

            return *this;
        }
        
        Color interpolate(Color &other, float alpha) {
            Color result;

            float cr = (1 - alpha) * r + other.r * alpha;
            float cg = (1 - alpha) * g + other.g * alpha;
            float cb = (1 - alpha) * b + other.b * alpha;
            
            result.r = (color_t) cr;
            result.g = (color_t) cg;
            result.b = (color_t) cb;
            
            return result;
        }
        Color midpoint(Color &other) {
            return interpolate(other, 0.5f);
        }

        friend std::ostream& operator<<(std::ostream &stream, Color &color) {
            stream << "(" << (unsigned)color.r << ", " << (unsigned)color.g << ", " << (unsigned)color.b << ")";

            return stream;
        }
    };
}
#endif