/********************************************************************************
* ReactPhysics3D physics library, http://www.reactphysics3d.com                 *
* Copyright (c) 2010-2022 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

#ifndef REACTPHYSICS3D_vector4_H
#define REACTPHYSICS3D_vector4_H

// Libraries
#include <cassert>
#include <cmath>
#include <algorithm>
#include <reactphysics3d/decimal.h>
#include <reactphysics3d/mathematics/mathematics_common.h>
#include <reactphysics3d/configuration.h>

/// fnx namespace
namespace fnx {

// Struct vector4
/**
 * This class represents a 4D vector.
 */
struct vector4 {

    public:

        // -------------------- Attributes -------------------- //

        /// Component x
        decimal x;

        /// Component y
        decimal y;

        /// Component z
        decimal z;

        /// Component w
        decimal w;

        inline auto xy() const { return fnx::vector2{x,y}; }
        inline auto zw() const { return fnx::vector2{z,w}; }
        inline auto r() const { return x; }
        inline auto g() const { return y; }
        inline auto b() const { return z; }
        inline auto a() const { return w; }

        // -------------------- Methods -------------------- //

        /// Constructor of the struct vector4
        vector4();

        vector4(decimal val)
            : vector4(val, val, val, val) {}

        vector4(const fnx::vector3& rgb, decimal alpha)
            : vector4(rgb.x, rgb.y, rgb.z, alpha) {}

        vector4(const fnx::vector3& rgb)
            : vector4(rgb.x, rgb.y, rgb.z, 1.0) {}

        /// Constructor with arguments
        vector4(decimal newX, decimal newY, decimal newZ, decimal newW);

        /// Set all the values of the vector
        void setAllValues(decimal newX, decimal newY, decimal newZ, decimal newW);

        /// Set the vector to zero
        void setToZero();

        /// Return the length of the vector
        decimal length() const;

        /// Return the square of the length of the vector
        decimal lengthSquare() const;

        /// Return the corresponding unit vector
        vector4 getUnit() const;

        /// Return one unit orthogonal vector of the current vector
        vector4 getOneUnitOrthogonalVector() const;

        /// Return true if the vector is unit and false otherwise
        bool isUnit() const;

        /// Return true if the values are not NAN OR INF
        bool isFinite() const;

        /// Return true if the current vector is the zero vector
        bool isZero() const;

        /// Dot product of two vectors
        reactphysics3d::decimal dot(const vector4& vector) const;

        /// Normalize the vector
        void normalize();

        /// Return the corresponding absolute value vector
        vector4 getAbsoluteVector() const;

        /// Return the axis with the minimal value
        int getMinAxis() const;

        /// Return the axis with the maximal value
        int getMaxAxis() const;

        /// Return the minimum value among the three components of a vector
        reactphysics3d::decimal getMinValue() const;

        /// Return the maximum value among the three components of a vector
        reactphysics3d::decimal getMaxValue() const;

        /// Overloaded operator for the equality condition
        bool operator== (const vector4& vector) const;

        /// Overloaded operator for the is different condition
        bool operator!= (const vector4& vector) const;

        /// Overloaded operator for addition with assignment
        vector4& operator+=(const vector4& vector);

        /// Overloaded operator for substraction with assignment
        vector4& operator-=(const vector4& vector);

        /// Overloaded operator for multiplication with a number with assignment
        vector4& operator*=(decimal number);

        /// Overloaded operator for division by a number with assignment
        vector4& operator/=(decimal number);

        /// Overloaded operator for value access
        reactphysics3d::decimal& operator[] (int index);

        /// Overloaded operator for value access
        const reactphysics3d::decimal& operator[] (int index) const;

        /// Overloaded less than operator for ordering to be used inside std::set for instance
        bool operator<(const vector4& vector) const;

        /// Get the string representation
        std::string to_string() const;

        /// Return a vector taking the minimum components of two vectors
        static vector4 min(const vector4& vector1, const vector4& vector2);

        /// Return a vector taking the maximum components of two vectors
        static vector4 max(const vector4& vector1, const vector4& vector2);

        /// Return the zero vector
        static vector4 zero();

        // -------------------- Friends -------------------- //

        friend vector4 operator+(const vector4& vector1, const vector4& vector2);
        friend vector4 operator-(const vector4& vector1, const vector4& vector2);
        friend vector4 operator-(const vector4& vector);
        friend vector4 operator*(const vector4& vector, decimal number);
        friend vector4 operator*(decimal number, const vector4& vector);
        friend vector4 operator*(const vector4& vector1, const vector4& vector2);
        friend vector4 operator/(const vector4& vector, decimal number);
        friend vector4 operator/(const vector4& vector1, const vector4& vector2);
};

// Constructor of the struct vector4
RP3D_FORCE_INLINE vector4::vector4() : x(0.0), y(0.0), z(0.0), w(0.0) {

}

// Constructor with arguments
RP3D_FORCE_INLINE vector4::vector4(decimal newX, decimal newY, decimal newZ, decimal newW) : x(newX), y(newY), z(newZ), w(newW) {

}

// Set the vector to zero
RP3D_FORCE_INLINE void vector4::setToZero() {
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

// Set all the values of the vector
RP3D_FORCE_INLINE void vector4::setAllValues(reactphysics3d::decimal newX, reactphysics3d::decimal newY, reactphysics3d::decimal newZ, reactphysics3d::decimal newW) {
    x = newX;
    y = newY;
    z = newZ;
    w = newW;
}

// Return the length of the vector
RP3D_FORCE_INLINE reactphysics3d::decimal vector4::length() const {
    return std::sqrt(x*x + y*y + z*z);
}

// Return the square of the length of the vector
RP3D_FORCE_INLINE reactphysics3d::decimal vector4::lengthSquare() const {
    return x*x + y*y + z*z;
}

// Scalar product of two vectors (RP3D_FORCE_INLINE)
RP3D_FORCE_INLINE reactphysics3d::decimal vector4::dot(const vector4& vector) const {
    return (x*vector.x + y*vector.y + z*vector.z + w*vector.w);
}

// Normalize the vector
RP3D_FORCE_INLINE void vector4::normalize() {
    reactphysics3d::decimal l = length();
    if (l < MACHINE_EPSILON) {
        return;
    }
    x /= l;
    y /= l;
    z /= l;
}

// Return the corresponding absolute value vector
RP3D_FORCE_INLINE vector4 vector4::getAbsoluteVector() const {
    return vector4(std::abs(x), std::abs(y), std::abs(z), std::abs(w));
}

// Return the axis with the minimal value
RP3D_FORCE_INLINE int vector4::getMinAxis() const {
    return (x < y ? (x < z ? 0 : 2) : (y < z ? 1 : 2));
}

// Return the axis with the maximal value
RP3D_FORCE_INLINE int vector4::getMaxAxis() const {
    return (x < y ? (y < z ? 2 : 1) : (x < z ? 2 : 0));
}

// Return true if the vector is unit and false otherwise
RP3D_FORCE_INLINE bool vector4::isUnit() const {
    return reactphysics3d::approxEqual(lengthSquare(), reactphysics3d::decimal(1.0));
}

// Return true if the values are not NAN OR INF
RP3D_FORCE_INLINE bool vector4::isFinite() const {
    return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
}

// Return true if the vector is the zero vector
RP3D_FORCE_INLINE bool vector4::isZero() const {
    return reactphysics3d::approxEqual(lengthSquare(), reactphysics3d::decimal(0.0));
}

// Overloaded operator for the equality condition
RP3D_FORCE_INLINE bool vector4::operator== (const vector4& vector) const {
    return (x == vector.x && y == vector.y && z == vector.z);
}

// Overloaded operator for the is different condition
RP3D_FORCE_INLINE bool vector4::operator!= (const vector4& vector) const {
    return !(*this == vector);
}

// Overloaded operator for addition with assignment
RP3D_FORCE_INLINE vector4& vector4::operator+=(const vector4& vector) {
    x += vector.x;
    y += vector.y;
    z += vector.z;
    w += vector.w;
    return *this;
}

// Overloaded operator for substraction with assignment
RP3D_FORCE_INLINE vector4& vector4::operator-=(const vector4& vector) {
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    w -= vector.w;
    return *this;
}

// Overloaded operator for multiplication with a number with assignment
RP3D_FORCE_INLINE vector4& vector4::operator*=(reactphysics3d::decimal number) {
    x *= number;
    y *= number;
    z *= number;
    w *= number;
    return *this;
}

// Overloaded operator for division by a number with assignment
RP3D_FORCE_INLINE vector4& vector4::operator/=(reactphysics3d::decimal number) {
    assert(number > std::numeric_limits<reactphysics3d::decimal>::epsilon());
    x /= number;
    y /= number;
    z /= number;
    w /= number;
    return *this;
}

// Overloaded operator for value access
RP3D_FORCE_INLINE reactphysics3d::decimal& vector4::operator[] (int index) {
    return (&x)[index];
}

// Overloaded operator for value access
RP3D_FORCE_INLINE const reactphysics3d::decimal& vector4::operator[] (int index) const {
    return (&x)[index];
}

// Overloaded operator for addition
RP3D_FORCE_INLINE vector4 operator+(const vector4& vector1, const vector4& vector2) {
    return vector4(vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z, vector1.w + vector2.w);
}

// Overloaded operator for substraction
RP3D_FORCE_INLINE vector4 operator-(const vector4& vector1, const vector4& vector2) {
    return vector4(vector1.x - vector2.x, vector1.y - vector2.y, vector1.z - vector2.z, vector1.w - vector2.w);
}

// Overloaded operator for the negative of a vector
RP3D_FORCE_INLINE vector4 operator-(const vector4& vector) {
    return vector4(-vector.x, -vector.y, -vector.z, -vector.w);
}

// Overloaded operator for multiplication with a number
RP3D_FORCE_INLINE vector4 operator*(const vector4& vector, reactphysics3d::decimal number) {
    return vector4(number * vector.x, number * vector.y, number * vector.z, number * vector.w);
}

// Overloaded operator for division by a number
RP3D_FORCE_INLINE vector4 operator/(const vector4& vector, reactphysics3d::decimal number) {
    assert(number > MACHINE_EPSILON);
    return vector4(vector.x / number, vector.y / number, vector.z / number, vector.w / number);
}

// Overload operator for division between two vectors
RP3D_FORCE_INLINE vector4 operator/(const vector4& vector1, const vector4& vector2) {
    assert(vector2.x > MACHINE_EPSILON);
    assert(vector2.y > MACHINE_EPSILON);
    assert(vector2.z > MACHINE_EPSILON);
    return vector4(vector1.x / vector2.x, vector1.y / vector2.y, vector1.z / vector2.z, vector1.w / vector2.w);
}

// Overloaded operator for multiplication with a number
RP3D_FORCE_INLINE vector4 operator*(reactphysics3d::decimal number, const vector4& vector) {
    return vector * number;
}

// Overload operator for multiplication between two vectors
RP3D_FORCE_INLINE vector4 operator*(const vector4& vector1, const vector4& vector2) {
    return vector4(vector1.x * vector2.x, vector1.y * vector2.y, vector1.z * vector2.z, vector1.w * vector2.w);
}

// Overloaded less than operator for ordering to be used inside std::set for instance
RP3D_FORCE_INLINE bool vector4::operator<(const vector4& vector) const {
    return (x == vector.x ? (y == vector.y ? z < vector.z : y < vector.y) : x < vector.x);
}

// Return a vector taking the minimum components of two vectors
RP3D_FORCE_INLINE vector4 vector4::min(const vector4& vector1, const vector4& vector2) {
    return vector4(std::min(vector1.x, vector2.x),
                   std::min(vector1.y, vector2.y),
                   std::min(vector1.z, vector2.z),
                   std::min(vector1.w, vector2.w));
}

// Return a vector taking the maximum components of two vectors
RP3D_FORCE_INLINE vector4 vector4::max(const vector4& vector1, const vector4& vector2) {
    return vector4(std::max(vector1.x, vector2.x),
                   std::max(vector1.y, vector2.y),
                   std::max(vector1.z, vector2.z),
                   std::max(vector1.w, vector2.w));
}

// Return the minimum value among the three components of a vector
RP3D_FORCE_INLINE reactphysics3d::decimal vector4::getMinValue() const {
    return std::min(std::min(std::min(x, y), z), w);
}

// Return the maximum value among the three components of a vector
RP3D_FORCE_INLINE reactphysics3d::decimal vector4::getMaxValue() const {
    return std::max(std::max(std::max(x, y), z), w);
}

// Get the string representation
RP3D_FORCE_INLINE std::string vector4::to_string() const {
    return "vector4(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "," + std::to_string(w) + ")";
}

// Return the zero vector
RP3D_FORCE_INLINE vector4 vector4::zero() {
    return vector4(0, 0, 0, 0);
}

// Function to test if two vectors are (almost) equal
RP3D_FORCE_INLINE bool approxEqual(const vector4& vec1, const vector4& vec2, reactphysics3d::decimal epsilon = MACHINE_EPSILON) {
    return reactphysics3d::approxEqual(vec1.x, vec2.x, epsilon) && reactphysics3d::approxEqual(vec1.y, vec2.y, epsilon) &&
           reactphysics3d::approxEqual(vec1.z, vec2.z, epsilon) && reactphysics3d::approxEqual(vec1.w, vec2.w, epsilon);
}

}

#endif
