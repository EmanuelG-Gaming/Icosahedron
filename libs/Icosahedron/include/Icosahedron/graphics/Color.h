#ifndef IC_COLOR_H
#define IC_COLOR_H

#include <iostream>

#include <string>
#include <stdint.h>

using color_t = uint8_t;

namespace ic {
    /** @brief Stores an RGBA color with components ranging from 0 to 255. */
    struct Color {
        color_t r, g, b, a;

        Color() : r(0), g(0), b(0), a(255) {}
        Color(color_t r, color_t g, color_t b) : r(r), g(g), b(b), a(255) {}
        Color(color_t r, color_t g, color_t b, color_t a) : r(r), g(g), b(b), a(a) {}
        
        Color operator*(Color &other) {
            float f = 255.0f;

            float nr1 = r / f;
            float ng1 = g / f;
            float nb1 = b / f;
            float na1 = a / f;

            float nr2 = other.r / f;
            float ng2 = other.g / f;
            float nb2 = other.b / f;
            float na2 = other.a / f;

            Color result;

            result.r = (color_t)((nr1 * nr2) * f);
            result.g = (color_t)((ng1 * ng2) * f);
            result.b = (color_t)((nb1 * nb2) * f);
            result.a = (color_t)((na1 * na2) * f);

            return result;
        }
        Color operator*(float value) {
            Color result;

            result.r = (color_t)(r * value);
            result.g = (color_t)(g * value);
            result.b = (color_t)(b * value);
            result.a = (color_t)(a * value);

            return result;
        }

        Color operator+(Color &other) {
            Color result;

            result.r = r + other.r;
            result.g = g + other.g;
            result.b = b + other.b;
            result.a = a + other.a;

            return result;
        }
        Color operator-(Color &other) {
            Color result;

            result.r = r - other.r;
            result.g = g - other.g;
            result.b = b - other.b;
            result.a = b - other.a;

            return result;
        }
        

        Color hexadecimal_to_RGB(int hexadecimal) {
            r = (hexadecimal >> 16) & 0xFF;
            g = (hexadecimal >> 8) & 0xFF;
            b = (hexadecimal) & 0xFF;
            a = 255; // Hardcoded value

            return *this;
        }

        /** @brief Converts a base 16 description without the hash sign into an RGB color. */
        Color hexadecimal_to_RGB(const std::string &stringValue) {
            int tempR, tempG, tempB;
            sscanf(stringValue.c_str(), "%02x%02x%02x", &tempR, &tempG, &tempB);
            
            r = (color_t) tempR;
            g = (color_t) tempG;
            b = (color_t) tempB;
            a = 255; // Hardcoded value again

            return *this;
        }

        Color hexadecimal_to_RGBA(int hexadecimal) {
            r = (hexadecimal >> 24) & 0xFF;
            g = (hexadecimal >> 16) & 0xFF;
            b = (hexadecimal >> 8) & 0xFF;
            a = (hexadecimal) & 0xFF;

            return *this;
        }

        /** @brief Converts a base 16 description without the hash sign into an RGBA color. */
        Color hexadecimal_to_RGBA(const std::string &stringValue) {
            int tempR, tempG, tempB, tempA;
            sscanf(stringValue.c_str(), "%02x%02x%02x%02x", &tempR, &tempG, &tempB, &tempA);
            
            r = (color_t) tempR;
            g = (color_t) tempG;
            b = (color_t) tempB;
            a = (color_t) tempA;

            return *this;
        }
        
        Color interpolate(Color &other, float alpha) {
            Color result;

            float quontient = (1.0f - alpha);
            float cr = quontient * r + other.r * alpha;
            float cg = quontient * g + other.g * alpha;
            float cb = quontient * b + other.b * alpha;
            float ca = quontient * a + other.a * alpha;
            
            result.r = (color_t) cr;
            result.g = (color_t) cg;
            result.b = (color_t) cb;
            result.a = (color_t) ca;
            
            return result;
        }
        Color midpoint(Color &other) {
            return interpolate(other, 0.5f);
        }

        friend std::ostream& operator<<(std::ostream &stream, Color &color) {
            if (color.a == 255) {
                stream << "(" << (unsigned)color.r << ", " << (unsigned)color.g << ", " << (unsigned)color.b << ")";
            } else {
                stream << "(" << (unsigned)color.r << ", " << (unsigned)color.g << ", " << (unsigned)color.b << ", " << (unsigned)color.a << ")";
            }

            return stream;
        }
    };
}
#endif