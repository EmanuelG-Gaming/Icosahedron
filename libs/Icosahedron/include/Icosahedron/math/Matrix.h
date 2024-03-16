#ifndef IC_MATRIX_H
#define IC_MATRIX_H

#include <cmath>

#include <iostream>
#include <array>
#include <initializer_list>

#include <Icosahedron/math/Mathf.h>
#include <Icosahedron/math/geom/Vectors.h>

namespace ic {
    /** @brief Represents an n by m column-major matrix. */
    template <typename T, std::size_t n, std::size_t m>
    struct Matrix {
        using Mat = Matrix<T, n, m>;
        
        std::array<T, n * m> values;

        /** @brief Initializes an identity matrix, with diagonal terms being 1. */
        Matrix() {
            this->identity();
        }

        /** @brief Initialises the matrix in column-major order. */
        Matrix(std::initializer_list<T> from) {
            for (auto position = from.begin(); position != from.end(); position++) {
                values[std::distance(from.begin(), position)] = *position;
            }
        }

        // Matrix overriding
        
        Mat &identity() {
            for (int i = 0; i < area(); i++) {
                values[i] = 0.0f;
            }
            for (int i = 0; i < n; i++) {
                value(i, i) = 1.0f;
            }
    
            return *this;
        }

        Mat &set_perspective(float fovDegrees, float zNear, float zFar, float aspectRatio) {
            static_assert(n == 4, "Tried to initialize a perspective matrix with a size != 4.");
            
            float fovR = float(1.0f / tan(fovDegrees * (M_PI / 180.0f) / 2.0f));           
            float range = zFar - zNear;
           
            identity();                    
            value(0, 0) = fovR / aspectRatio;            
            value(1, 1) = fovR;            
            value(2, 2) = -(zFar + zNear) / range; 
            value(3, 3) = 0.0f;

            value(2, 3) = -(2 * zFar * zNear) / range;           
            value(3, 2) = -1.0f;

            return *this;           
        }

        Mat &set_orthographic(float left, float right, float bottom, float top, float near = 0.0f, float far = 1000.0f) {
            //static_assert(is_square(), "Orthographic matrices work if they're square.");
            static_assert(n == 4, "Tried to initialize an orthographic matrix with a size != 4.");
            
            identity();
           
            value(0, 0) = 2.0f / (right - left);
            value(1, 1) = 2.0f / (top - bottom);
            value(2, 2) = -2.0f / (far - near);
           
            value(0, 3) = -(right + left) / (right - left);
            value(1, 3) = -(top + bottom) / (top - bottom);
            value(2, 3) = -(far + near) / (far - near);

            return *this;
        }

        Mat &set_look_at(ic::Vec3f cameraPosition, ic::Vec3f lookingAt, ic::Vec3f up) {
            //static_assert(is_square(), "View matrices work if they're square.");
            static_assert(n == 4, "Tried to initialize a view matrix with a size != 4.");
            
            ic::Vec3f fwd = (cameraPosition - lookingAt).nor();                
            ic::Vec3f cameraXAxis = fwd.crs(up).nor();
            ic::Vec3f cameraYAxis = cameraXAxis.crs(fwd);
           
            identity();
           
            value(0, 0) = cameraXAxis.x();
            value(0, 1) = cameraXAxis.y();
            value(0, 2) = cameraXAxis.z();

            value(1, 0) = cameraYAxis.x();
            value(1, 1) = cameraYAxis.y();
            value(1, 2) = cameraYAxis.z();

            value(2, 0) = fwd.x();
            value(2, 1) = fwd.y();
            value(2, 2) = fwd.z();
           
            value(0, 3) = -cameraXAxis.dot(cameraPosition);
            value(1, 3) = -cameraYAxis.dot(cameraPosition);
            value(2, 3) = -fwd.dot(cameraPosition);

            return *this;
        }

        template <std::size_t p>
        Mat &set_translation(ic::Vector<T, p> by) {
            identity();

            for (int i = 0; i < p; i++) {
                value(i, n - 1) = by[i];
            }

            return *this;
        }

        template <std::size_t p>
        Mat &set_scaling(ic::Vector<T, p> by) {
            identity();

            for (int i = 0; i < p; i++) {
                value(i, i) = by[i];
            }

            return *this;
        }

        Mat &set_scaling(T by) {
            identity();

            for (int i = 0; i < n - 1; i++) {
                value(i, i) = by;
            }

            return *this;
        }

        /** @brief Sets this matrix to a rotation about the X axis. */
        Mat &set_rotation_x(float radians) {
            identity();

            value(1, 1) = ic::Mathf::cosf(radians);
            value(2, 2) = ic::Mathf::cosf(radians);
            value(2, 1) = -ic::Mathf::sinf(radians);
            value(1, 2) = ic::Mathf::sinf(radians);

            return *this;
        }

        /** @brief Sets this matrix to a rotation about the Y axis. */
        Mat &set_rotation_y(float radians) {
            identity();

            value(0, 0) = ic::Mathf::cosf(radians);
            value(2, 2) = ic::Mathf::cosf(radians);
            value(2, 0) = ic::Mathf::sinf(radians);
            value(0, 2) = -ic::Mathf::sinf(radians);

            return *this;
        }

        /** @brief Sets this matrix to a rotation about the Z axis. */
        Mat &set_rotation_z(float radians) {
            identity();

            value(0, 0) = ic::Mathf::cosf(radians);
            value(1, 1) = ic::Mathf::cosf(radians);
            value(0, 1) = -ic::Mathf::sinf(radians);
            value(1, 0) = ic::Mathf::sinf(radians);

            return *this;
        }

        /** @brief Does the same effect as rotating about the Z axis. */
        Mat &set_rotation(float radians) {
            return set_rotation_z(radians);
        }

        /** @brief Sets this matrix's columns as vectors. */
        template <std::size_t p>
        Mat &set_vectors(std::initializer_list<ic::Vector<T, p>> from) {
            int j = 0;

            for (auto element : from) {
                for (int i = 0; i < p; i++) {
                    value(i, j) = element[i];
                }

                j++;
            }

            return *this;
        }

        /** @brief Matrix-matrix multiplication. */
        Mat operator*(const Mat &other) const {
            Mat result;
            if (!is_square()) {
                printf("Project didn't implement non-square matrix multiplication.\n");
                return result;
            }

            int row = 0, column = 0;
            for (int i = 0; i < area(); i++) {
                row = (i / n) * n;
                column = (i % n);

                float sum = 0.0f;
                for (int j = 0; j < n; j++) {
                    sum += values[column + j * n] * other[row + j];
                }
                result[i] = sum;
            }

            return result;
        }

        /** @brief Matrix-vector multiplication. */
        ic::Vector<T, n> operator*(const ic::Vector<T, n> &other) const {
            ic::Vector<T, n> result;

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    result[i] = result[i] + (other[i] * value(i, j));
                }
            }

            return result;
        }


        friend std::ostream& operator<<(std::ostream &stream, const Mat &matrix) {
            std::size_t size = matrix.area();
            int r = 0;

            stream << "(";
            for (int i = 0; i < size; i++) {
                stream << matrix[i];
                if (i < size - 1) {
                    stream << ", ";
                    if (++r % m == 0) stream << "\n";
                }
            }
            stream << ")";

            return stream;
        }



        // Matrix indexing

        T& operator[](int index) {
            return values[index];
        }
        T& operator()(int x, int y) {
            return values[y * n + x];
        }

        T operator[](int index) const {
            return values[index];
        }
        T operator()(int x, int y) const {
            return values[y * n + x];
        }

        T value(int x, int y) const {
            return values[y * n + x];
        }
        T& value(int x, int y) {
            return values[y * n + x];
        }


        // Matrix size operations

        bool is_square() const{
            return (n == m);
        }

        std::size_t area() const {
            return n * m;
        }
    };

    /*
    template <typename T, std::size_t l>
    ic::Matrix<T, l, l> &matrix_identity() {
        ic::Matrix<T, l, l> result;

        std::size_t area = l*l;

        for (int i = 0; i < area; i++) {
            result.values[i] = 0.0f;
        }
        for (int i = 0; i < l; i++) {
            result(i, i) = 1.0f;
        }

        return result;
    }
    */
}
#endif