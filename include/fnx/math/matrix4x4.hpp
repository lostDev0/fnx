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


#ifndef REACTPHYSICS3D_MATRIX4X4_H
#define REACTPHYSICS3D_MATRIX4X4_H

// Libraries
#include <cassert>
#include <reactphysics3d/mathematics/Quaternion.h>
using namespace reactphysics3d;

/// fnx namespace
namespace fnx
{

// Class matrix4x4
/**
 * This class represents a 4x4 matrix.
 */
class matrix4x4
{

private :

    // -------------------- Attributes -------------------- //

    /// Rows of the matrix;
    //Quaternion mRows[4];
    decimal mRows[4][4];

public :

    // -------------------- Methods -------------------- //

    /// Constructor
    matrix4x4();

    /// Constructor
    matrix4x4( decimal value );

    /// Constructor
    matrix4x4( decimal a1, decimal a2, decimal a3, decimal a4,
               decimal b1, decimal b2, decimal b3, decimal b4,
               decimal c1, decimal c2, decimal c3, decimal c4,
               decimal d1, decimal d2, decimal d3, decimal d4 );

    /// Set all the values in the matrix
    void setAllValues( decimal a1, decimal a2, decimal a3, decimal a4,
                       decimal b1, decimal b2, decimal b3, decimal b4,
                       decimal c1, decimal c2, decimal c3, decimal c4,
                       decimal d1, decimal d2, decimal d3, decimal d4 );

    /// Set the matrix to zero
    void setToZero();

    /// Return a column
    Quaternion getColumn( int i ) const;

    /// Return a row
    Quaternion getRow( int i ) const;

    /// Return all data
    RP3D_FORCE_INLINE const auto getAll() const
    {
        return mRows;
    }

    /// Return the transpose matrix
    matrix4x4 getTranspose() const;

    /// Return the determinant of the matrix
    decimal getDeterminant() const;

    /// Return the trace of the matrix
    decimal getTrace() const;

    /// Return the inverse matrix
    matrix4x4 getInverse() const;

    /// Return the inverse matrix
    matrix4x4 getInverse( decimal determinant ) const;

    /// Return the matrix with absolute values
    matrix4x4 getAbsoluteMatrix() const;

    /// Set the matrix to the identity matrix
    void setToIdentity();

    /// Return the 4x4 identity matrix
    static matrix4x4 identity();

    /// Return the 4x4 zero matrix
    static matrix4x4 zero();

    /// Return a skew-symmetric matrix using a given vector that can be used
    /// to compute cross product with another vector using matrix multiplication
    static matrix4x4 computeSkewSymmetricMatrixForCrossProduct( const Quaternion& vector );

    /// Overloaded operator for addition
    friend matrix4x4 operator+( const matrix4x4& matrix1, const matrix4x4& matrix2 );

    /// Overloaded operator for substraction
    friend matrix4x4 operator-( const matrix4x4& matrix1, const matrix4x4& matrix2 );

    /// Overloaded operator for the negative of the matrix
    friend matrix4x4 operator-( const matrix4x4& matrix );

    /// Overloaded operator for multiplication with a number
    friend matrix4x4 operator*( decimal nb, const matrix4x4& matrix );

    /// Overloaded operator for multiplication with a matrix
    friend matrix4x4 operator*( const matrix4x4& matrix, decimal nb );

    /// Overloaded operator for division with a number
    friend matrix4x4 operator/( decimal nb, const matrix4x4& matrix );

    /// Overloaded operator for division with a matrix
    friend matrix4x4 operator/( const matrix4x4& matrix, decimal nb );

    /// Overloaded operator for matrix multiplication
    friend matrix4x4 operator*( const matrix4x4& matrix1, const matrix4x4& matrix2 );

    /// Overloaded operator for multiplication with a vector
    friend Quaternion operator*( const matrix4x4& matrix, const Quaternion& vector );

    /// Overloaded operator for equality condition
    bool operator==( const matrix4x4& matrix ) const;

    /// Overloaded operator for the is different condition
    bool operator!= ( const matrix4x4& matrix ) const;

    /// Overloaded operator for addition with assignment
    matrix4x4& operator+=( const matrix4x4& matrix );

    /// Overloaded operator for substraction with assignment
    matrix4x4& operator-=( const matrix4x4& matrix );

    /// Overloaded operator for multiplication with a number with assignment
    matrix4x4& operator*=( decimal nb );

    // Overloaded operator to return a row of the matrix.
    /// This operator is also used to access a matrix value using the syntax
    /// matrix[row][col].
    RP3D_FORCE_INLINE const auto operator[]( int row ) const
    {
        return mRows[row];
    }

    // Overloaded operator to return a row of the matrix.
    /// This operator is also used to access a matrix value using the syntax
    /// matrix[row][col].
    RP3D_FORCE_INLINE auto operator[]( int row )
    {
        return mRows[row];
    }

    /// Return the string representation
    std::string to_string() const;
};

// Constructor of the class matrix4x4
RP3D_FORCE_INLINE matrix4x4::matrix4x4()
{
    // Initialize all values in the matrix to zero
    setAllValues( 0.0, 0.0, 0.0, 0.0,
                  0.0, 0.0, 0.0, 0.0,
                  0.0, 0.0, 0.0, 0.0,
                  0.0, 0.0, 0.0, 0.0 );
}

// Constructor
RP3D_FORCE_INLINE matrix4x4::matrix4x4( decimal value )
{
    setAllValues( value, value, value, value,
                  value, value, value, value,
                  value, value, value, value,
                  value, value, value, value );
}

// Constructor with arguments
RP3D_FORCE_INLINE matrix4x4::matrix4x4( decimal a1, decimal a2, decimal a3, decimal a4,
                                        decimal b1, decimal b2, decimal b3, decimal b4,
                                        decimal c1, decimal c2, decimal c3, decimal c4,
                                        decimal d1, decimal d2, decimal d3, decimal d4 )
{
    // Initialize the matrix with the values
    setAllValues( a1, a2, a3, a4,
                  b1, b2, b3, b4,
                  c1, c2, c3, c4,
                  d1, d2, d3, d4 );
}

// Method to set all the values in the matrix
RP3D_FORCE_INLINE void matrix4x4::setAllValues( decimal a1, decimal a2, decimal a3, decimal a4,
        decimal b1, decimal b2, decimal b3, decimal b4,
        decimal c1, decimal c2, decimal c3, decimal c4,
        decimal d1, decimal d2, decimal d3, decimal d4 )
{
    mRows[0][0] = a1;
    mRows[0][1] = a2;
    mRows[0][2] = a3;
    mRows[0][3] = a4;
    mRows[1][0] = b1;
    mRows[1][1] = b2;
    mRows[1][2] = b3;
    mRows[1][3] = b4;
    mRows[2][0] = c1;
    mRows[2][1] = c2;
    mRows[2][2] = c3;
    mRows[2][3] = c4;
    mRows[3][0] = d1;
    mRows[3][1] = d2;
    mRows[3][2] = d3;
    mRows[3][3] = d4;
}

// Set the matrix to zero
RP3D_FORCE_INLINE void matrix4x4::setToZero()
{
    for ( std::size_t i = 0; i < 4; i++ )
    {
        for ( std::size_t k = 0; i < 4; i++ )
        {
            mRows[i][k] = 0;
        }
    }
}

// Return a column
RP3D_FORCE_INLINE Quaternion matrix4x4::getColumn( int i ) const
{
    assert( i >= 0 && i < 4 );
    return Quaternion( mRows[0][i], mRows[1][i], mRows[2][i], mRows[3][i] );
}

// Return a row
RP3D_FORCE_INLINE Quaternion matrix4x4::getRow( int i ) const
{
    assert( i >= 0 && i < 4 );
    //return mRows[i];
    return Quaternion( mRows[i][0], mRows[i][1], mRows[i][2], mRows[i][3] );
}

// Return the transpose matrix
RP3D_FORCE_INLINE matrix4x4 matrix4x4::getTranspose() const
{

    // Return the transpose matrix
    return matrix4x4( mRows[0][0], mRows[1][0], mRows[2][0], mRows[3][0],
                      mRows[0][1], mRows[1][1], mRows[2][1], mRows[3][1],
                      mRows[0][2], mRows[1][2], mRows[2][2], mRows[3][2],
                      mRows[0][3], mRows[1][3], mRows[2][3], mRows[3][3] );
}

// Return the determinant of the matrix
RP3D_FORCE_INLINE decimal matrix4x4::getDeterminant() const
{

    // Compute and return the determinant of the matrix
    //return (mRows[0][0]*(mRows[1][1]*mRows[2][2]-mRows[2][1]*mRows[1][2]) -
    //        mRows[0][1]*(mRows[1][0]*mRows[2][2]-mRows[2][0]*mRows[1][2]) +
    //        mRows[0][2]*(mRows[1][0]*mRows[2][1]-mRows[2][0]*mRows[1][1]));
    decimal c, r = 1;
    auto A = *this;
    for ( int i = 0; i < 4; i++ )
    {
        for ( int k = i + 1; k < 4; k++ )
        {
            c = A[k][i] / A[i][i];
            for ( int j = i; j < 4; j++ )
            {
                A[k][j] = A[k][j] - c * A[i][j];
            }
        }
    }
    for ( int i = 0; i < 4; i++ )
    {
        r *= A[i][i];
    }
    return r;
}

// Return the trace of the matrix
RP3D_FORCE_INLINE decimal matrix4x4::getTrace() const
{

    // Compute and return the trace
    return ( mRows[0][0] + mRows[1][1] + mRows[2][2] + mRows[3][3] );
}

// Set the matrix to the identity matrix
RP3D_FORCE_INLINE void matrix4x4::setToIdentity()
{
    mRows[0][0] = 1.0;
    mRows[0][1] = 0.0;
    mRows[0][2] = 0.0;
    mRows[0][3] = 0.0;
    mRows[1][0] = 0.0;
    mRows[1][1] = 1.0;
    mRows[1][2] = 0.0;
    mRows[1][3] = 0.0;
    mRows[2][0] = 0.0;
    mRows[2][1] = 0.0;
    mRows[2][2] = 1.0;
    mRows[2][3] = 0.0;
    mRows[3][0] = 0.0;
    mRows[3][1] = 0.0;
    mRows[3][2] = 0.0;
    mRows[3][3] = 1.0;
}

// Return the 4x4 identity matrix
RP3D_FORCE_INLINE matrix4x4 matrix4x4::identity()
{
    return matrix4x4( 1.0, 0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      0.0, 0.0, 0.0, 1.0 );
}

// Return the 4x4 zero matrix
RP3D_FORCE_INLINE matrix4x4 matrix4x4::zero()
{
    return matrix4x4( 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0 );
}

// Return the inverse matrix
RP3D_FORCE_INLINE matrix4x4 matrix4x4::getInverse() const
{

    return getInverse( getDeterminant() );
}

// Return a skew-symmetric matrix using a given vector that can be used
// to compute cross product with another vector using matrix multiplication
RP3D_FORCE_INLINE matrix4x4 matrix4x4::computeSkewSymmetricMatrixForCrossProduct( const Quaternion& vector )
{
    // TODO update for matrix 4x4
    //return matrix4x4(0, -vector.z, vector.y, vector.z, 0, -vector.x, -vector.y, vector.x, 0);
    return matrix4x4();
}

// Return the matrix with absolute values
RP3D_FORCE_INLINE matrix4x4 matrix4x4::getAbsoluteMatrix() const
{
    return matrix4x4( std::abs( mRows[0][0] ), std::abs( mRows[0][1] ), std::abs( mRows[0][2] ), std::abs( mRows[0][3] ),
                      std::abs( mRows[1][0] ), std::abs( mRows[1][1] ), std::abs( mRows[1][2] ), std::abs( mRows[1][3] ),
                      std::abs( mRows[2][0] ), std::abs( mRows[2][1] ), std::abs( mRows[2][2] ), std::abs( mRows[2][3] ),
                      std::abs( mRows[3][0] ), std::abs( mRows[3][1] ), std::abs( mRows[3][2] ), std::abs( mRows[3][3] ) );
}

// Overloaded operator for addition
RP3D_FORCE_INLINE matrix4x4 operator+( const matrix4x4& matrix1, const matrix4x4& matrix2 )
{
    return matrix4x4( matrix1.mRows[0][0] + matrix2.mRows[0][0], matrix1.mRows[0][1] + matrix2.mRows[0][1],
                      matrix1.mRows[0][2] + matrix2.mRows[0][3], matrix1.mRows[0][3] + matrix2.mRows[0][3],
                      matrix1.mRows[1][0] + matrix2.mRows[1][0], matrix1.mRows[1][1] + matrix2.mRows[1][1],
                      matrix1.mRows[1][2] + matrix2.mRows[1][3], matrix1.mRows[1][3] + matrix2.mRows[1][3],
                      matrix1.mRows[2][0] + matrix2.mRows[2][0], matrix1.mRows[2][1] + matrix2.mRows[2][1],
                      matrix1.mRows[2][2] + matrix2.mRows[2][3], matrix1.mRows[2][3] + matrix2.mRows[2][3],
                      matrix1.mRows[3][0] + matrix2.mRows[3][0], matrix1.mRows[3][1] + matrix2.mRows[3][1],
                      matrix1.mRows[3][2] + matrix2.mRows[3][3], matrix1.mRows[3][3] + matrix2.mRows[3][3] );
}

// Overloaded operator for substraction
RP3D_FORCE_INLINE matrix4x4 operator-( const matrix4x4& matrix1, const matrix4x4& matrix2 )
{
    return matrix4x4( matrix1.mRows[0][0] - matrix2.mRows[0][0], matrix1.mRows[0][1] - matrix2.mRows[0][1],
                      matrix1.mRows[0][2] - matrix2.mRows[0][2], matrix1.mRows[0][3] - matrix2.mRows[0][3],
                      matrix1.mRows[1][0] - matrix2.mRows[1][0], matrix1.mRows[1][1] - matrix2.mRows[1][1],
                      matrix1.mRows[1][2] - matrix2.mRows[1][2], matrix1.mRows[1][3] - matrix2.mRows[1][3],
                      matrix1.mRows[2][0] - matrix2.mRows[2][0], matrix1.mRows[2][1] - matrix2.mRows[2][1],
                      matrix1.mRows[2][2] - matrix2.mRows[2][2], matrix1.mRows[2][3] - matrix2.mRows[2][3],
                      matrix1.mRows[3][0] - matrix2.mRows[3][0], matrix1.mRows[3][1] - matrix2.mRows[3][1],
                      matrix1.mRows[3][2] - matrix2.mRows[3][2], matrix1.mRows[3][3] - matrix2.mRows[3][3] );
}

// Overloaded operator for the negative of the matrix
RP3D_FORCE_INLINE matrix4x4 operator-( const matrix4x4& matrix )
{
    return matrix4x4( -matrix.mRows[0][0], -matrix.mRows[0][1], -matrix.mRows[0][2], -matrix.mRows[0][3],
                      -matrix.mRows[1][0], -matrix.mRows[1][1], -matrix.mRows[1][2], -matrix.mRows[1][3],
                      -matrix.mRows[2][0], -matrix.mRows[2][1], -matrix.mRows[2][2], -matrix.mRows[2][3],
                      -matrix.mRows[3][0], -matrix.mRows[3][1], -matrix.mRows[3][2], -matrix.mRows[3][3] );
}

// Overloaded operator for multiplication with a number
RP3D_FORCE_INLINE matrix4x4 operator*( decimal nb, const matrix4x4& matrix )
{
    return matrix4x4( matrix.mRows[0][0] * nb, matrix.mRows[0][1] * nb, matrix.mRows[0][2] * nb, matrix.mRows[0][3] * nb,
                      matrix.mRows[1][0] * nb, matrix.mRows[1][1] * nb, matrix.mRows[1][2] * nb, matrix.mRows[1][3] * nb,
                      matrix.mRows[2][0] * nb, matrix.mRows[2][1] * nb, matrix.mRows[2][2] * nb, matrix.mRows[2][3] * nb,
                      matrix.mRows[3][0] * nb, matrix.mRows[3][1] * nb, matrix.mRows[3][2] * nb, matrix.mRows[3][3] * nb );
}

// Overloaded operator for multiplication with a matrix
RP3D_FORCE_INLINE matrix4x4 operator*( const matrix4x4& matrix, decimal nb )
{
    return nb * matrix;
}

// Overloaded operator for division with a number
RP3D_FORCE_INLINE matrix4x4 operator/( decimal nb, const matrix4x4& matrix )
{
    return matrix4x4( matrix.mRows[0][0] / nb, matrix.mRows[0][1] / nb, matrix.mRows[0][2] / nb, matrix.mRows[0][3] / nb,
                      matrix.mRows[1][0] / nb, matrix.mRows[1][1] / nb, matrix.mRows[1][2] / nb, matrix.mRows[1][3] / nb,
                      matrix.mRows[2][0] / nb, matrix.mRows[2][1] / nb, matrix.mRows[2][2] / nb, matrix.mRows[2][3] / nb,
                      matrix.mRows[3][0] / nb, matrix.mRows[3][1] / nb, matrix.mRows[3][2] / nb, matrix.mRows[3][3] / nb );
}

// Overloaded operator for division with a matrix
RP3D_FORCE_INLINE matrix4x4 operator/( const matrix4x4& matrix, decimal nb )
{
    return nb / matrix;
}

// Overloaded operator for matrix multiplication
RP3D_FORCE_INLINE matrix4x4 operator*( const matrix4x4& matrix1, const matrix4x4& matrix2 )
{
    return matrix4x4( matrix1.mRows[0][0] * matrix2.mRows[0][0] + matrix1.mRows[0][1] * matrix2.mRows[1][0] +
                      matrix1.mRows[0][2] * matrix2.mRows[2][0] + matrix1.mRows[0][3] * matrix2.mRows[3][0],
                      matrix1.mRows[0][0] * matrix2.mRows[0][1] + matrix1.mRows[0][1] * matrix2.mRows[1][1] + matrix1.mRows[0][2] *
                      matrix2.mRows[2][1] + matrix1.mRows[0][3] * matrix2.mRows[3][1],
                      matrix1.mRows[0][0] * matrix2.mRows[0][2] + matrix1.mRows[0][1] * matrix2.mRows[1][2] + matrix1.mRows[0][2] *
                      matrix2.mRows[2][2] + matrix1.mRows[0][3] * matrix2.mRows[3][2],
                      matrix1.mRows[0][0] * matrix2.mRows[0][3] + matrix1.mRows[0][1] * matrix2.mRows[1][3] + matrix1.mRows[0][2] *
                      matrix2.mRows[2][3] + matrix1.mRows[0][3] * matrix2.mRows[3][3],
                      matrix1.mRows[1][0] * matrix2.mRows[0][0] + matrix1.mRows[1][1] * matrix2.mRows[1][0] + matrix1.mRows[1][2] *
                      matrix2.mRows[2][0] + matrix1.mRows[1][3] * matrix2.mRows[3][0],
                      matrix1.mRows[1][0] * matrix2.mRows[0][1] + matrix1.mRows[1][1] * matrix2.mRows[1][1] + matrix1.mRows[1][2] *
                      matrix2.mRows[2][1] + matrix1.mRows[1][3] * matrix2.mRows[3][1],
                      matrix1.mRows[1][0] * matrix2.mRows[0][2] + matrix1.mRows[1][1] * matrix2.mRows[1][2] + matrix1.mRows[1][2] *
                      matrix2.mRows[2][2] + matrix1.mRows[1][3] * matrix2.mRows[3][2],
                      matrix1.mRows[1][0] * matrix2.mRows[0][3] + matrix1.mRows[1][1] * matrix2.mRows[1][3] + matrix1.mRows[1][2] *
                      matrix2.mRows[2][3] + matrix1.mRows[1][3] * matrix2.mRows[3][3],
                      matrix1.mRows[2][0] * matrix2.mRows[0][0] + matrix1.mRows[2][1] * matrix2.mRows[1][0] + matrix1.mRows[2][2] *
                      matrix2.mRows[2][0] + matrix1.mRows[2][3] * matrix2.mRows[3][0],
                      matrix1.mRows[2][0] * matrix2.mRows[0][1] + matrix1.mRows[2][1] * matrix2.mRows[1][1] + matrix1.mRows[2][2] *
                      matrix2.mRows[2][1] + matrix1.mRows[2][3] * matrix2.mRows[3][1],
                      matrix1.mRows[2][0] * matrix2.mRows[0][2] + matrix1.mRows[2][1] * matrix2.mRows[1][2] + matrix1.mRows[2][2] *
                      matrix2.mRows[2][2] + matrix1.mRows[2][3] * matrix2.mRows[3][2],
                      matrix1.mRows[2][0] * matrix2.mRows[0][3] + matrix1.mRows[2][1] * matrix2.mRows[1][3] + matrix1.mRows[2][2] *
                      matrix2.mRows[2][3] + matrix1.mRows[2][3] * matrix2.mRows[3][3],
                      matrix1.mRows[3][0] * matrix2.mRows[0][0] + matrix1.mRows[3][1] * matrix2.mRows[1][0] + matrix1.mRows[3][2] *
                      matrix2.mRows[2][0] + matrix1.mRows[2][3] * matrix2.mRows[3][0],
                      matrix1.mRows[3][0] * matrix2.mRows[0][1] + matrix1.mRows[3][1] * matrix2.mRows[1][1] + matrix1.mRows[3][2] *
                      matrix2.mRows[2][1] + matrix1.mRows[2][3] * matrix2.mRows[3][1],
                      matrix1.mRows[3][0] * matrix2.mRows[0][2] + matrix1.mRows[3][1] * matrix2.mRows[1][2] + matrix1.mRows[3][2] *
                      matrix2.mRows[2][2] + matrix1.mRows[2][3] * matrix2.mRows[3][2],
                      matrix1.mRows[3][0] * matrix2.mRows[0][3] + matrix1.mRows[3][1] * matrix2.mRows[1][3] + matrix1.mRows[3][2] *
                      matrix2.mRows[2][3] + matrix1.mRows[2][3] * matrix2.mRows[3][3] );
}

// Overloaded operator for multiplication with a vector
RP3D_FORCE_INLINE Quaternion operator*( const matrix4x4& matrix, const Quaternion& vector )
{
    return Quaternion( matrix.mRows[0][0] * vector.x + matrix.mRows[0][1] * vector.y + matrix.mRows[0][2] * vector.z +
                       matrix.mRows[0][3] * vector.w,
                       matrix.mRows[1][0] * vector.x + matrix.mRows[1][1] * vector.y + matrix.mRows[1][2] * vector.z + matrix.mRows[1][3] *
                       vector.w,
                       matrix.mRows[2][0] * vector.x + matrix.mRows[2][1] * vector.y + matrix.mRows[2][2] * vector.z + matrix.mRows[2][3] *
                       vector.w,
                       matrix.mRows[3][0] * vector.x + matrix.mRows[3][1] * vector.y + matrix.mRows[3][2] * vector.z + matrix.mRows[3][3] *
                       vector.w );
}

// Overloaded operator for equality condition
RP3D_FORCE_INLINE bool matrix4x4::operator==( const matrix4x4& matrix ) const
{
    return ( mRows[0][0] == matrix.mRows[0][0] && mRows[0][1] == matrix.mRows[0][1] &&
             mRows[0][2] == matrix.mRows[0][2] && mRows[0][3] == matrix.mRows[0][3] &&
             mRows[1][0] == matrix.mRows[1][0] && mRows[1][1] == matrix.mRows[1][1] &&
             mRows[1][2] == matrix.mRows[1][2] && mRows[1][3] == matrix.mRows[1][3] &&
             mRows[2][0] == matrix.mRows[2][0] && mRows[2][1] == matrix.mRows[2][1] &&
             mRows[2][2] == matrix.mRows[2][2] && mRows[2][3] == matrix.mRows[2][3] &&
             mRows[3][0] == matrix.mRows[3][0] && mRows[3][1] == matrix.mRows[3][1] &&
             mRows[3][2] == matrix.mRows[3][2] && mRows[3][3] == matrix.mRows[3][3] );
}

// Overloaded operator for the is different condition
RP3D_FORCE_INLINE bool matrix4x4::operator!= ( const matrix4x4& matrix ) const
{
    return !( *this == matrix );
}

// Overloaded operator for addition with assignment
RP3D_FORCE_INLINE matrix4x4& matrix4x4::operator+=( const matrix4x4& matrix )
{
    mRows[0][0] += matrix.mRows[0][0];
    mRows[0][1] += matrix.mRows[0][1];
    mRows[0][2] += matrix.mRows[0][2];
    mRows[0][3] += matrix.mRows[0][3];
    mRows[1][0] += matrix.mRows[1][0];
    mRows[1][1] += matrix.mRows[1][1];
    mRows[1][2] += matrix.mRows[1][2];
    mRows[1][3] += matrix.mRows[1][3];
    mRows[2][0] += matrix.mRows[2][0];
    mRows[2][1] += matrix.mRows[2][1];
    mRows[2][2] += matrix.mRows[2][2];
    mRows[2][3] += matrix.mRows[2][3];
    mRows[3][0] += matrix.mRows[3][0];
    mRows[3][1] += matrix.mRows[3][1];
    mRows[3][2] += matrix.mRows[3][2];
    mRows[3][3] += matrix.mRows[3][3];
    return *this;
}

// Overloaded operator for substraction with assignment
RP3D_FORCE_INLINE matrix4x4& matrix4x4::operator-=( const matrix4x4& matrix )
{
    mRows[0][0] -= matrix.mRows[0][0];
    mRows[0][1] -= matrix.mRows[0][1];
    mRows[0][2] -= matrix.mRows[0][2];
    mRows[0][3] -= matrix.mRows[0][3];
    mRows[1][0] -= matrix.mRows[1][0];
    mRows[1][1] -= matrix.mRows[1][1];
    mRows[1][2] -= matrix.mRows[1][2];
    mRows[1][3] -= matrix.mRows[1][3];
    mRows[2][0] -= matrix.mRows[2][0];
    mRows[2][1] -= matrix.mRows[2][1];
    mRows[2][2] -= matrix.mRows[2][2];
    mRows[2][3] -= matrix.mRows[2][3];
    mRows[3][0] -= matrix.mRows[3][0];
    mRows[3][1] -= matrix.mRows[3][1];
    mRows[3][2] -= matrix.mRows[3][2];
    mRows[3][3] -= matrix.mRows[3][3];
    return *this;
}

// Overloaded operator for multiplication with a number with assignment
RP3D_FORCE_INLINE matrix4x4& matrix4x4::operator*=( decimal nb )
{
    mRows[0][0] *= nb;
    mRows[0][1] *= nb;
    mRows[0][2] *= nb;
    mRows[0][3] *= nb;
    mRows[1][0] *= nb;
    mRows[1][1] *= nb;
    mRows[1][2] *= nb;
    mRows[1][3] *= nb;
    mRows[2][0] *= nb;
    mRows[2][1] *= nb;
    mRows[2][2] *= nb;
    mRows[2][3] *= nb;
    mRows[3][0] *= nb;
    mRows[3][1] *= nb;
    mRows[3][2] *= nb;
    mRows[3][3] *= nb;
    return *this;
}

// Get the string representation
RP3D_FORCE_INLINE std::string matrix4x4::to_string() const
{
    return "matrix4x4(" + std::to_string( mRows[0][0] ) + "," + std::to_string( mRows[0][1] ) + "," + std::to_string(
               mRows[0][2] ) + "," + std::to_string( mRows[0][3] ) + "," +
           std::to_string( mRows[1][0] ) + "," + std::to_string( mRows[1][1] ) + "," + std::to_string(
               mRows[1][2] ) + "," + std::to_string( mRows[1][3] ) + "," +
           std::to_string( mRows[2][0] ) + "," + std::to_string( mRows[2][1] ) + "," + std::to_string(
               mRows[2][2] ) + ")" + std::to_string( mRows[2][3] ) + "," +
           std::to_string( mRows[3][0] ) + "," + std::to_string( mRows[3][1] ) + "," + std::to_string(
               mRows[3][2] ) + ")" + std::to_string( mRows[3][3] );
}

/// @brief Equivalent to glOrtho()
RP3D_FORCE_INLINE fnx::matrix4x4 matrix_ortho( reactphysics3d::decimal l,
        reactphysics3d::decimal r,
        reactphysics3d::decimal b,
        reactphysics3d::decimal t,
        reactphysics3d::decimal n = -1.f,
        reactphysics3d::decimal f = 1.f )
{
    fnx::matrix4x4 mat;
    mat[0][0] = 2.f / ( r - l );
    mat[1][1] = 2.f / ( t - b );
    mat[2][2] = -2.f / ( f - n );
    mat[3][0] = -( r + l ) / ( r - l );
    mat[3][1] = -( t + b ) / ( t - b );
    mat[3][2] = -( f + n ) / ( f - n );
    return mat;
}

/// @brief Equivalent to glFrustum()
RP3D_FORCE_INLINE fnx::matrix4x4 set_frustum( reactphysics3d::decimal l,
        reactphysics3d::decimal r,
        reactphysics3d::decimal b,
        reactphysics3d::decimal t,
        reactphysics3d::decimal n,
        reactphysics3d::decimal f )
{
    fnx::matrix4x4 mat;
    auto temp = reactphysics3d::decimal{2.0} * n;
    auto temp2 = r - l;
    auto temp3 = t - b;
    auto temp4 = f - n;
    mat[0][0] = temp / temp2;
    mat[1][1] = temp / temp3;
    mat[2][0] = ( r + l ) / temp2;
    mat[2][1] = ( t + b ) / temp3;
    mat[2][2] = -( f + n ) / temp4;
    mat[2][3] = reactphysics3d::decimal{ -1 };
    mat[3][2] = ( -temp * f ) / temp4;
    mat[3][3] = reactphysics3d::decimal{ 0 };
    return mat;
}

/// @brief Equivalent to gluPerspective()
/// @param[in] fov_y : field of view in degrees
RP3D_FORCE_INLINE fnx::matrix4x4 matrix_perspective( const angle& fov_y, reactphysics3d::decimal aspect,
        reactphysics3d::decimal front, reactphysics3d::decimal back )
{
    float tangent = tanf( static_cast<float>( fov_y.get_as_degree() / 2.0 * fnx::DEG_TO_RAD ) ); // tangent of half fovY
    float height = front * tangent;         // half height of near plane
    float width = height * aspect;          // half width of near plane
    // params: left, right, bottom, top, near, far
    return set_frustum( -width, width, -height, height, front, back );
}

RP3D_FORCE_INLINE fnx::matrix4x4 matrix_translate( const fnx::matrix4x4& input, const fnx::vector3& position )
{
    auto ret = input;
    auto x = position.x;
    auto y = position.y;
    auto z = position.z;
    ret[0][0] += ret[0][3] * x;
    ret[1][0] += ret[1][3] * x;
    ret[2][0] += ret[2][3] * x;
    ret[3][0] += ret[3][3] * x;
    ret[0][1] += ret[0][3] * y;
    ret[1][1] += ret[1][3] * y;
    ret[2][1] += ret[2][3] * y;
    ret[3][1] += ret[3][3] * y;
    ret[0][2] += ret[0][3] * z;
    ret[1][2] += ret[1][3] * z;
    ret[2][2] += ret[2][3] * z;
    ret[3][2] += ret[3][3] * z;
    return ret;
}

RP3D_FORCE_INLINE fnx::matrix4x4 matrix_translate( const fnx::matrix4x4& input, reactphysics3d::decimal x,
        reactphysics3d::decimal y, reactphysics3d::decimal z )
{
    auto ret = input;
    ret[0][0] += ret[0][3] * x;
    ret[1][0] += ret[1][3] * x;
    ret[2][0] += ret[2][3] * x;
    ret[3][0] += ret[3][3] * x;
    ret[0][1] += ret[0][3] * y;
    ret[1][1] += ret[1][3] * y;
    ret[2][1] += ret[2][3] * y;
    ret[3][1] += ret[3][3] * y;
    ret[0][2] += ret[0][3] * z;
    ret[1][2] += ret[1][3] * z;
    ret[2][2] += ret[2][3] * z;
    ret[3][2] += ret[3][3] * z;
    return ret;
}

RP3D_FORCE_INLINE fnx::matrix4x4 matrix_rotate( const fnx::matrix4x4& input, const angle& angle,
        const fnx::vector3& position )
{
    fnx::matrix4x4 ret( input );
    auto c = static_cast<reactphysics3d::decimal>( cos( angle.get_as_radian() ) );
    auto s = static_cast<reactphysics3d::decimal>( sin( angle.get_as_radian() ) );
    auto c1 = reactphysics3d::decimal{ 1.0 } - c;
    auto x = position.x;
    auto y = position.y;
    auto z = position.z;
    auto m0 = ret[0][0], m4 = ret[1][0], m8 = ret[2][0], m12 = ret[3][0],
         m1 = ret[0][1], m5 = ret[1][1], m9 = ret[2][1], m13 = ret[3][1],
         m2 = ret[0][2], m6 = ret[1][2], m10 = ret[2][2], m14 = ret[3][2];
    // build rotation matrix
    auto r0 = x * x * c1 + c;
    auto r1 = x * y * c1 + z * s;
    auto r2 = x * z * c1 - y * s;
    auto r4 = x * y * c1 - z * s;
    auto r5 = y * y * c1 + c;
    auto r6 = y * z * c1 + x * s;
    auto r8 = x * z * c1 + y * s;
    auto r9 = y * z * c1 - x * s;
    auto r10 = z * z * c1 + c;
    // multiply rotation matrix
    ret[0][0] = r0 * m0 + r4 * m1 + r8 * m2;
    ret[0][1] = r1 * m0 + r5 * m1 + r9 * m2;
    ret[0][2] = r2 * m0 + r6 * m1 + r10 * m2;
    ret[1][0] = r0 * m4 + r4 * m5 + r8 * m6;
    ret[1][1] = r1 * m4 + r5 * m5 + r9 * m6;
    ret[1][2] = r2 * m4 + r6 * m5 + r10 * m6;
    ret[2][0] = r0 * m8 + r4 * m9 + r8 * m10;
    ret[2][1] = r1 * m8 + r5 * m9 + r9 * m10;
    ret[2][2] = r2 * m8 + r6 * m9 + r10 * m10;
    ret[3][0] = r0 * m12 + r4 * m13 + r8 * m14;
    ret[3][1] = r1 * m12 + r5 * m13 + r9 * m14;
    ret[3][2] = r2 * m12 + r6 * m13 + r10 * m14;
    return ret;
}

RP3D_FORCE_INLINE fnx::matrix4x4 matrix_scale( const fnx::matrix4x4& input,
        reactphysics3d::decimal sx,
        reactphysics3d::decimal sy,
        reactphysics3d::decimal sz )
{
    auto ret = input;
    ret[0][0] *= sx;
    ret[1][0] *= sx;
    ret[2][0] *= sx;
    ret[3][0] *= sx;
    ret[0][1] *= sy;
    ret[1][1] *= sy;
    ret[2][1] *= sy;
    ret[3][1] *= sy;
    ret[0][2] *= sz;
    ret[1][2] *= sz;
    ret[2][2] *= sz;
    ret[3][2] *= sz;
    return ret;
}

RP3D_FORCE_INLINE fnx::matrix4x4 matrix_scale( const fnx::matrix4x4& input, const fnx::vector3& scale )
{
    return matrix_scale( input, scale.x, scale.y, scale.z );
}

RP3D_FORCE_INLINE fnx::matrix4x4 matrix_inverse( const fnx::matrix4x4& input )
{
    auto a00 = input[0][0], a01 = input[0][1], a02 = input[0][2], a03 = input[0][3],
         a10 = input[1][0], a11 = input[1][1], a12 = input[1][2], a13 = input[1][3],
         a20 = input[2][0], a21 = input[2][1], a22 = input[2][2], a23 = input[2][3],
         a30 = input[3][0], a31 = input[3][1], a32 = input[3][2], a33 = input[3][3],
         b00 = a00 * a11 - a01 * a10,
         b01 = a00 * a12 - a02 * a10,
         b02 = a00 * a13 - a03 * a10,
         b03 = a01 * a12 - a02 * a11,
         b04 = a01 * a13 - a03 * a11,
         b05 = a02 * a13 - a03 * a12,
         b06 = a20 * a31 - a21 * a30,
         b07 = a20 * a32 - a22 * a30,
         b08 = a20 * a33 - a23 * a30,
         b09 = a21 * a32 - a22 * a31,
         b10 = a21 * a33 - a23 * a31,
         b11 = a22 * a33 - a23 * a32,
         det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;
    return fnx::matrix4x4(
               a11 * b11 - a12 * b10 + a13 * b09,
               a02 * b10 - a01 * b11 - a03 * b09,
               a31 * b05 - a32 * b04 + a33 * b03,
               a22 * b04 - a21 * b05 - a23 * b03,
               a12 * b08 - a10 * b11 - a13 * b07,
               a00 * b11 - a02 * b08 + a03 * b07,
               a32 * b02 - a30 * b05 - a33 * b01,
               a20 * b05 - a22 * b02 + a23 * b01,
               a10 * b10 - a11 * b08 + a13 * b06,
               a01 * b08 - a00 * b10 - a03 * b06,
               a30 * b04 - a31 * b02 + a33 * b00,
               a21 * b02 - a20 * b04 - a23 * b00,
               a11 * b07 - a10 * b09 - a12 * b06,
               a00 * b09 - a01 * b07 + a02 * b06,
               a31 * b01 - a30 * b03 - a32 * b00,
               a20 * b03 - a21 * b01 + a22 * b00 ) / det;
}

/// @brief Equivalent to gluLookAt()
RP3D_FORCE_INLINE fnx::matrix4x4 matrix_look_at( const fnx::vector3& eye,
        const fnx::vector3& target,
        const fnx::vector3& up_direction )
{
    // compute the forward vector from target to eye
    auto forward = eye - target;
    forward.normalize();    // make unit length
    // compute the left vector
    auto left = up_direction.cross( forward ); // cross product
    left.normalize();
    // recompute the orthonormal up vector
    fnx::vector3 up = forward.cross( left ); // cross product
    // init 4x4 matrix
    fnx::matrix4x4 matrix;
    // set rotation part, inverse rotation matrix: M^-1 = M^T for Euclidean transform
    auto left_x = left.x;
    auto left_y = left.y;
    auto left_z = left.z;
    auto up_x = up.x;
    auto up_y = up.y;
    auto up_z = up.z;
    auto forward_x = forward.x;
    auto forward_y = forward.y;
    auto forward_z = forward.z;
    auto eye_x = eye.x;
    auto eye_y = eye.y;
    auto eye_z = eye.z;
    matrix[0][0] = left_x;
    matrix[1][0] = left_y;
    matrix[2][0] = left_z;
    matrix[0][1] = up_x;
    matrix[1][1] = up_y;
    matrix[2][1] = up_z;
    matrix[0][2] = forward_x;
    matrix[1][2] = forward_y;
    matrix[2][2] = forward_z;
    // set translation part
    matrix[3][0] = -left_x * eye_x - left_y * eye_y - left_z * eye_z;
    matrix[3][1] = -up_x * eye_x - up_y * eye_y - up_z * eye_z;
    matrix[3][2] = -forward_x * eye_x - forward_y * eye_y - forward_z * eye_z;
    return matrix;
}

/*
RP3D_FORCE_INLINE auto matrix_to_vector(const matrix<Type, rows, 1>& input)
{
    fnx::vector4 vector;
    for (auto i = 0u;i < rows;++i)
    {
        vector[i] = input.get_values()[i];
    }
    return vector;
}
*/

/// Calculate the transposed matrix.
RP3D_FORCE_INLINE auto matrix_transpose( const matrix4x4& mat )
{
    // TODO
    auto m = mat.getAll();
    return matrix4x4( m[0][0], m[0][1], m[0][2], m[0][3],
                      m[1][0], m[1][1], m[1][2], m[1][3],
                      m[2][0], m[2][1], m[2][2], m[2][3],
                      m[3][0], m[3][1], m[3][2], m[3][3] );
}

/// Create a matrix with the given transformation parameters.
RP3D_FORCE_INLINE auto calculate_transform_matrix( const fnx::vector3& position,
        const fnx::vector3& rotation_in_degrees, const fnx::vector3& scale )
{
    auto ident = matrix4x4::identity();
    ident = fnx::matrix_rotate( ident, fnx::angle( fnx::Degree( rotation_in_degrees.x ) ), fnx::vector3{1, 0, 0} );
    ident = fnx::matrix_rotate( ident, fnx::angle( fnx::Degree( rotation_in_degrees.y ) ), fnx::vector3{0, 1, 0} );
    ident = fnx::matrix_rotate( ident, fnx::angle( fnx::Degree( rotation_in_degrees.z ) ), fnx::vector3{0, 0, 1} );
    ident = fnx::matrix_scale( ident, scale );
    ident = fnx::matrix_translate( ident, position );
    return ident;
}

/// Create a matrix with the given transformation parameters.
RP3D_FORCE_INLINE auto calculate_transform_matrix( reactphysics3d::decimal x,
        reactphysics3d::decimal y,
        reactphysics3d::decimal z,
        reactphysics3d::decimal rx,
        reactphysics3d::decimal ry,
        reactphysics3d::decimal rz,
        reactphysics3d::decimal sx,
        reactphysics3d::decimal sy,
        reactphysics3d::decimal sz )
{
    auto ident = matrix4x4::identity();
    ident = fnx::matrix_rotate( ident, fnx::angle( fnx::Degree( rx ) ), fnx::vector3{1, 0, 0} );
    ident = fnx::matrix_rotate( ident, fnx::angle( fnx::Degree( ry ) ), fnx::vector3{0, 1, 0} );
    ident = fnx::matrix_rotate( ident, fnx::angle( fnx::Degree( rz ) ), fnx::vector3{0, 0, 1} );
    ident = fnx::matrix_scale( ident, sx, sy, sz );
    ident = fnx::matrix_translate( ident, x, y, z );
    return ident;
}

}

#endif
