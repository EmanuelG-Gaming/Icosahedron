#ifndef IC_QUATERNION_H
#define IC_QUATERNION_H

#include <cmath>

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/Matrices.h>


namespace ic {
    /** @brief The quaternion number system is an extension of the complex plane.
     *
     *  @note
     *  Quaternions are useful for representing 3D rotations, which can be achieved
     *  through multiplying it with a vector: p' = qpq',
     *  where q' is the conjugate of the quaternion, and p' is the resultant vector.
    */
    struct Quaternion {
        float x, y, z, w;

        /** @brief Initializes a quaternion at the origin. */
        Quaternion() {
            x = y = z = w = 0.0f;
        }
        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        
        /** @note Roll - X rotation; Pitch - Y rotation; Yaw - Z rotation */
        Quaternion from_euler(float roll, float pitch, float yaw) {
            float halfRoll = roll * 0.5f;
            float halfPitch = pitch * 0.5f;
            float halfYaw = yaw * 0.5f;
         
            // Abbreviations
            float cr = cos(halfRoll);
            float sr = sin(halfRoll);
            float cp = cos(halfPitch);
            float sp = sin(halfPitch);
            float cy = cos(halfYaw);
            float sy = sin(halfYaw);
         
            Quaternion result;
            result.w = cr * cp * cy + sr * sp * sy;
            result.x = sr * cp * cy - cr * sp * sy;
            result.y = cr * sp * cy + sr * cp * sy;
            result.z = cr * cp * sy - sr * sp * cy;
         
            return result;
        }

        /** @note In the vector: x - roll, y - pitch, z - yaw */
        ic::Vec3f to_euler() {
            ic::Vec3f result;
         
            float coefficient = 2 * (w * y - x * z);
            float sqrt1 = sqrt(1 + coefficient); 
            float sqrt2 = sqrt(1 - coefficient);
        
            result.x() = atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y));
            result.y() = -M_PI / 2.0f + 2 * atan2(sqrt1, sqrt2);
            result.z() = atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z));
         
            return result;
        }

        ic::Mat4x4 to_rotation_matrix() {
            ic::Mat4x4 result;
         
            result.value(0, 0) = w * w + x * x - y * y - z * z;
            result.value(1, 0) = 2 * (x * y - w * z);
            result.value(2, 0) = 2 * (w * y + x * z);
         
            result.value(0, 1) = 2 * (x * y + w * z);
            result.value(1, 1) = w * w - x * x + y * y - z * z;
            result.value(2, 1) = 2 * (y * z - w * x);
         
            result.value(0, 2) = 2 * (x * z - w * y);
            result.value(1, 2) = 2 * (w * x + y * z);
            result.value(2, 2) = w * w - x * x - y * y + z * z;
         
            return result;
        }

        Quaternion conjugate() {
            x = -x;
            y = -y;
            z = -z;
         
            return *this;
        }

        ic::Vec3f transform(Vec3f &reference) {
            // p' = q * p * conj(q)
            
            Quaternion temporary1 = Quaternion(reference.x(), reference.y(), reference.z(), 0.0f), temporary2 = Quaternion(*this);
            temporary2 = temporary2.conjugate();
            temporary2 = temporary2 * temporary1 * (*this);
         
            ic::Vec3f result;
          
            result.x() = temporary2.x;
            result.y() = temporary2.y;
            result.z() = temporary2.z;
         
            return result;
        } 
    
        Quaternion multiply_left(float otherX, float otherY, float otherZ, float otherW) {
            float newX = otherW * x + otherX * w + otherY * z - otherZ * y;
            float newY = otherW * y + otherY * w + otherZ * x - otherX * z;
            float newZ = otherW * z + otherZ * w + otherX * y - otherY * x;
            float newW = otherW * w - otherX * x - otherY * y - otherZ * z;
         
            x = newX;
            y = newY;
            z = newZ;
            w = newW;
         
            return *this;
        }
    
        Quaternion multiply_left(const Quaternion &other) {
            return multiply_left(other.x, other.y, other.z, other.w);
        }

        Quaternion operator*(const Quaternion &other) {
            float newX = other.w * x + other.x * w + other.y * z - other.z * y;
            float newY = other.w * y + other.y * w + other.z * x - other.x * z;
            float newZ = other.w * z + other.z * w + other.x * y - other.y * x;
            float newW = other.w * w - other.x * x - other.y * y - other.z * z;
         
            Quaternion r;
            r.x = newX;
            r.y = newY;
            r.z = newZ;
            r.w = newW;
            return r;
        }
    };
}
#endif