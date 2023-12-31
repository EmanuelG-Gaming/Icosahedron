#ifndef IC_VECTOR_H
#define IC_VECTOR_H

#include <cmath>
#include <iostream>
#include <array>
#include <cstring>
#include <stdexcept>

#include <Icosahedron/graphics/Color.h>


namespace ic {
    /** @brief Represents an n-dimensional geometric vector. */
    template <typename T, std::size_t dimensions>
    struct Vector {
        using Vec = Vector<T, dimensions>;
        std::array<T, dimensions> values;

        /** @brief Initializes a vector at the origin. */
        Vector() {
            memset(&values, 0, sizeof(values));
        }

        /** @brief Initializes a vector whose coordinates correspond to the elements of a list. */
        Vector(std::initializer_list<T> from) {
            memset(&values, 0, sizeof(values));
            for (auto position = from.begin(); position != from.end(); position++) {
                values[std::distance(from.begin(), position)] = *position;
            }
        }

        Vector(T x) {
            if (size() < 1) {
                throw std::runtime_error("This vector doesn't have any dimensions.\n");
            }
            memset(&values, 0, sizeof(values));

            values[0] = x;
        }

        Vector(T x, T y) {
            if (size() < 2) {
                throw std::runtime_error("This vector doesn't have at least two dimensions.\n");
            }
            memset(&values, 0, sizeof(values));

            values[0] = x;
            values[1] = y;
        }

        Vector(T x, T y, T z) {
            if (size() < 3) {
                throw std::runtime_error("This vector doesn't have at least three dimensions.\n");
            }
            memset(&values, 0, sizeof(values));

            values[0] = x;
            values[1] = y;
            values[2] = z;
        }

        Vector(T x, T y, T z, T w) {
            if (size() < 4) {
                throw std::runtime_error("This vector doesn't have at least four dimensions.\n");
            }
            memset(&values, 0, sizeof(values));

            values[0] = x;
            values[1] = y;
            values[2] = z;
            values[3] = w;
        }


        Vector(ic::Color color) {
            if (size() < 3) {
                throw std::runtime_error("This vector doesn't have at least three dimensions.\n");
            }
            memset(&values, 0, sizeof(values));

            values[0] = color.r;
            values[1] = color.g;
            values[2] = color.b;
        }


        Vec operator+(Vec other) {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result.values[i] = values[i] + other[i];
            }
            return result;
        }

        Vec operator-(Vec other) {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result.values[i] = values[i] - other[i];
            }
            return result;
        }
    
        Vec operator*(Vec other) {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result.values[i] = values[i] * other[i];
            }
            return result;
        }

        Vec operator/(Vec other) {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result.values[i] = values[i] / other[i];
            }
            return result;
        }
        
        Vec operator+(T scalar) {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result.values[i] = values[i] + scalar;
            }
            return result;
        }
        Vec operator-(T scalar) {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result.values[i] = values[i] - scalar;
            }
            return result;
        }
        Vec operator*(T scalar) {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result.values[i] = values[i] * scalar;
            }
            return result;
        }
        Vec operator/(T scalar) {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result.values[i] = values[i] / scalar;
            }
            return result;
        }

        
        // Incremental operators

        Vec& operator+=(const Vec& other) {
            for (int i = 0; i < dimensions; i++) {
                values[i] = values[i] + other.values[i];
            }
            return *this;
        }

        Vec& operator-=(const Vec& other) {
            for (int i = 0; i < dimensions; i++) {
                values[i] = values[i] - other[i];
            }
            return *this;
        }
    
        Vec& operator*=(const Vec& other) {
            for (int i = 0; i < dimensions; i++) {
                values[i] = values[i] * other[i];
            }
            return *this;
        }

        Vec& operator/=(const Vec& other) {
            for (int i = 0; i < dimensions; i++) {
                values[i] = values[i] / other[i];
            }
            return *this;
        }
        
        Vec& operator+=(T scalar) {
            for (int i = 0; i < dimensions; i++) {
                values[i] = values[i] + scalar;
            }
            return *this;
        }
        Vec& operator-=(T scalar) {
            for (int i = 0; i < dimensions; i++) {
                values[i] = values[i] - scalar;
            }
            return *this;
        }
        Vec& operator*=(T scalar) {
            for (int i = 0; i < dimensions; i++) {
                values[i] = values[i] * scalar;
            }
            return *this;
        }
        Vec& operator/=(T scalar) {
            for (int i = 0; i < dimensions; i++) {
                values[i] = values[i] / scalar;
            }
            return *this;
        }


        
        float dot(Vec &other) {
            float result = 0.0f;
            for (int i = 0; i < dimensions; i++) {
                result += values[i] * other[i];
            }
            return result;
        }

        float dst(Vec &other) {
            float result = 0.0f;
            for (int i = 0; i < dimensions; i++) {
                T v = other[i] - values[i];
                result += v * v;
            }
            result = sqrt(result);

            return result;
        }
        
        float dst2(Vec &other) {
            float result = 0.0f;
            for (int i = 0; i < dimensions; i++) {
                T v = other[i] - values[i];
                result += v * v;
            }
            
            return result;
        }

        float len() {
            float result = 0.0f;
            for (int i = 0; i < dimensions; i++) {
                result += values[i] * values[i];
            }
            result = sqrt(result);

            return result;
        }

        /** @brief The squared length of this vector. */
        float len2() {
            float result = 0.0f;
            for (int i = 0; i < dimensions; i++) {
                result += values[i] * values[i];
            }
            
            return result;
        }

        Vec nor() {
            Vec result;
            float length = len();

            if (length != 0.0f) {
                for (int i = 0; i < dimensions; i++) {
                    result.values[i] = values[i] / length;
                }
            }

            return result;
        }

        float crs(ic::Vector<T, 2> &other) {
            return x() * other.y() - y() * other.x();
        }
        ic::Vector<T, 3> crs(ic::Vector<T, 3> &other) {
            float cx = y() * other.z() - z() * other.y();
            float cy = z() * other.x() - x() * other.z();
            float cz = x() * other.y() - y() * other.x();
        
            ic::Vector<T, 3> result = { cx, cy, cz };
        
            return result;
        }


        ic::Vector<T, 2> perpendicular(int facing) {
            int j = facing >= 0 ? 1 : -1;
            float ax = x();

            ic::Vector<T, 2> result;
            result.x() = j * y();
            result.y() = -j * ax;

            return result;
        }

        /** @brief Rotates this vector by an angle beginning
         *  on the positive X semiaxis, going counter-clockwise, in radians. */
        ic::Vector<T, 2> rotate(float radians) {
            float sine = sin(radians), cosine = cos(radians);

            ic::Vector<T, 2> result;
            result.x() = cosine * x() - sine * y();
            result.y() = sine * x() + cosine * y();

            return result;
        }

        Vec clamp(Vec lower, Vec upper) {
            Vec result;

            for (int i = 0; i < dimensions; i++) {
                result.values[i] = std::max(lower[i], std::min(upper[i], values[i]));
            }

            return result;
        }

        /** @brief Applies linear interpolation over each vector's components. */
        Vec interpolate(Vec &other, float alpha) {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result.values[i] = values[i] * (1 - alpha) + other[i] * alpha;
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

        template <typename T2>
        ic::Vector<T2, dimensions> as() {
            ic::Vector<T2, dimensions> result;

            for (int i = 0; i < dimensions; i++) {
                result.values[i] = (T2) values[i];
            }

            return result;
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