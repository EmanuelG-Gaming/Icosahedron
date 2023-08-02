#ifndef IC_VECTOR_H
#define IC_VECTOR_H

#include <iostream>
#include <array>

namespace ic {
    /* An n-dimensional geometric vector. */
    template <typename T, std::size_t dimensions>
    struct Vector {
        using Vec = Vector<T, dimensions>;
        std::array<T, dimensions> values;

        Vector() {}
        Vector(std::initializer_list<T> from) {
            for (auto position = from.begin(); position != from.end(); position++) {
                values[std::distance(from.begin(), position)] = *position;
            }
        }
    
        Vec operator+(Vec &other) {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result.values[i] = values[i] + other[i];
            }
            return result;
        }

        Vec operator-(Vec &other) {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result.values[i] = values[i] - other[i];
            }
            return result;
        }
    
        Vec operator*(Vec &other) {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result.values[i] = values[i] * other[i];
            }
            return result;
        }

        Vec operator/(Vec &other) {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result.values[i] = values[i] / other[i];
            }
            return result;
        }
        
        Vec operator*(float scalar) {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result.values[i] = values[i] * scalar;
            }
            return result;
        }
        float dot(Vec &other) {
            float result = 0.0f;
            for (int i = 0; i < dimensions; i++) {
                result += values[i] * other[i];
            }
            return result;
        }

        friend std::ostream& operator<<(std::ostream &stream, Vec &vector) {
            std::size_t dims = vector.size();

            stream << "(";
            for (int i = 0; i < dims; i++) {
                stream << vector[i];
                if (i < dims - 1) stream << ", ";
            }
            stream << ")";

            return stream;
        }
        
        T& x() {
            static_assert(dimensions >= 1, "X is only avaiable for dimensions >= 1.");
            return values[0];
        }
        T& y() {
            static_assert(dimensions >= 2, "Y is only avaiable for dimensions >= 2.");
            return values[1];
        }
        T& z() {
            static_assert(dimensions >= 3, "Z is only avaiable for dimensions >= 3.");
            return values[2];
        }
        T& w() {
            static_assert(dimensions >= 4, "W is only avaiable for dimensions >= 4.");
            return values[3];
        }
        
        T& operator[](const int index) {
            return values[index];
        }

        std::size_t size() {
            return dimensions;
        }
    };    
}
#endif