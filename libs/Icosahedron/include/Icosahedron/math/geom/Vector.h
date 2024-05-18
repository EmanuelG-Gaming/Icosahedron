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


        // Unary operations (vector negation)

        Vec operator-() const {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result[i] = values[i] * -1;
            }
            return result;
        }

        // Binary operations

        Vec operator+(const Vec &other) const {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result[i] = values[i] + other.values[i];
            }
            return result;
        }

        Vec operator-(const Vec &other) const {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result[i] = values[i] - other.values[i];
            }

            return result;
        }
    
        Vec operator*(const Vec &other) const {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result[i] = values[i] * other.values[i];
            }
            return result;
        }

        Vec operator/(const Vec &other) const {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result[i] = values[i] / other.values[i];
            }
            return result;
        }
        
        Vec operator*(const T scalar) const {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result[i] = values[i] * scalar;
            }
            return result;
        }
        Vec operator/(const T scalar) const {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result[i] = values[i] / scalar;
            }
            return result;
        }


        
        // Incremental operators

        Vec& operator+=(const Vec &other) {
            for (int i = 0; i < dimensions; i++) {
                values[i] = this[i] + other[i];
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



        
        float dot(const Vec &other) const {
            float result = 0.0f;
            for (int i = 0; i < dimensions; i++) {
                result += values[i] * other.values[i];
            }
            return result;
        }

        float dst(const Vec &other) const {
            float result = 0.0f;
            for (int i = 0; i < dimensions; i++) {
                T v = other[i] - values[i];
                result += v * v;
            }
            result = sqrt(result);

            return result;
        }
        
        float dst2(const Vec &other) const {
            float result = 0.0f;
            for (int i = 0; i < dimensions; i++) {
                T v = other.values[i] - values[i];
                result += v * v;
            }
            
            return result;
        }

        float len() const {
            float result = 0.0f;
            for (int i = 0; i < dimensions; i++) {
                result += values[i] * values[i];
            }
            result = sqrt(result);

            return result;
        }

        /** @brief The squared length of this vector. */
        float len2() const {
            float result = 0.0f;
            for (int i = 0; i < dimensions; i++) {
                result += values[i] * values[i];
            }
            
            return result;
        }

        Vec nor() const {
            Vec result;
            float length = len();

            if (length != 0.0f) {
                for (int i = 0; i < dimensions; i++) {
                    result.values[i] = values[i] / length;
                }
            }

            return result;
        }

        float crs(const ic::Vector<T, 2> &other) const {
            return x() * other.y() - y() * other.x();
        }
        ic::Vector<T, 3> crs(const ic::Vector<T, 3> &other) const {
            float cx = y() * other.z() - z() * other.y();
            float cy = z() * other.x() - x() * other.z();
            float cz = x() * other.y() - y() * other.x();
        
            ic::Vector<T, 3> result = { cx, cy, cz };
            return result;
        }


        ic::Vector<T, 2> perpendicular(int facing) const {
            int j = facing >= 0 ? 1 : -1;
            float ax = x();

            ic::Vector<T, 2> result;
            result.x() = j * y();
            result.y() = -j * ax;

            return result;
        }

        /** @brief Rotates this vector by an angle beginning
         *  on the positive X semiaxis, going counter-clockwise, in radians. */
        ic::Vector<T, 2> rotate(const float radians) const {
            float sine = sin(radians), cosine = cos(radians);

            ic::Vector<T, 2> result;
            result.x() = cosine * x() - sine * y();
            result.y() = sine * x() + cosine * y();

            return result;
        }

        Vec clamp(const Vec &lower, const Vec &upper) const {
            Vec result;

            for (int i = 0; i < dimensions; i++) {
                result.values[i] = std::max(lower[i], std::min(upper[i], values[i]));
            }

            return result;
        }

        /** @brief Applies linear interpolation over each vector's components. */
        Vec interpolate(const Vec &other, const float alpha) const {
            Vec result;
            for (int i = 0; i < dimensions; i++) {
                result.values[i] = values[i] * (1 - alpha) + other[i] * alpha;
            }
            return result;
        }

        friend std::ostream& operator<<(std::ostream &stream, const Vec &vector) {
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
        ic::Vector<T2, dimensions> as() const {
            ic::Vector<T2, dimensions> result;

            for (int i = 0; i < dimensions; i++) {
                result.values[i] = (T2) values[i];
            }

            return result;
        }

        T x() const { return values[0]; }
        T y() const { return values[1]; }
        T z() const { return values[2]; }
        T w() const { return values[3]; }

        T& x() { return values[0]; }
        T& y() { return values[1]; }
        T& z() { return values[2]; }
        T& w() { return values[3]; }
    

        
        T& operator[](int index) {
            return values[index];
        }
        T operator[](int index) const {
            return values[index];
        }


        std::size_t size() const {
            return dimensions;
        }
    };    
}
#endif