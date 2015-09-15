/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2014 Koen Visscher, Paul Visscher and individual contributors.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @endcond
 */

#pragma once
#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include "math/types.h"
#include "math/config.h"
#include "math/scalar/matrix4.h"
#include "math/scalar/matrix3.h"
#include "math/scalar/mathf.h"
#include "math/scalar/vec3.h"

#include <assert.h>


class Matrix4 : public PrimaryMatrixBase< Real, 4, 4 >
{
public:

    inline Matrix4()
    {
    }

    inline Matrix4( const Real a11, const Real a12, const Real a13, const Real a14, 
                    const Real a21, const Real a22, const Real a23, const Real a24, 
                    const Real a31, const Real a32, const Real a33, const Real a34, 
                    const Real a41, const Real a42, const Real a43, const Real a44)
    {
        SetValues(a11, a12, a13, a14, a21, a22, a23, a24, a31, a32, a33, a34, a41, a42, a43, a44);
    }

    inline Matrix4(const Vec4 &v1, const Vec4 &v2, const Vec4 &v3, const Vec4 &v4)
    {
        mRows[0] = v1;
        mRows[1] = v2;
        mRows[2] = v3;
        mRows[3] = v4;
    }

    inline Matrix4(const Matrix4 &other)
    {
        mRows[0] = other.mRows[0];
        mRows[1] = other.mRows[1];
        mRows[2] = other.mRows[2];
        mRows[3] = other.mRows[3];
    }

    inline Matrix4(const PrimaryMatrixBase< Real, 4, 4 > &other)
    {
        this->mRows[0] = other.mRows[0];
        this->mRows[1] = other.mRows[1];
        this->mRows[2] = other.mRows[2];
        this->mRows[3] = other.mRows[3];
    }

    inline void SetValues( const Real a11, const Real a12, const Real a13, const Real a14,
                           const Real a21, const Real a22, const Real a23, const Real a24,
                           const Real a31, const Real a32, const Real a33, const Real a34,
                           const Real a41, const Real a42, const Real a43, const Real a44 )
    {
        mRows[0].SetValues(a11, a12, a13, a14);
        mRows[1].SetValues(a21, a22, a23, a24);
        mRows[2].SetValues(a31, a32, a33, a34);
        mRows[3].SetValues(a41, a42, a43, a44);
    }

};

/*

class Vec3;
class Quaternion;

class Matrix4
{
    friend Matrix4 operator+( const Matrix4 &m1, const Matrix4 &m2 );
    friend Matrix4 operator-( const Matrix4 &m1, const Matrix4 &m2 );
    friend Matrix4 operator*( const Matrix4 &m1, const Matrix4 &m2 );
    friend Matrix4 operator*( const Matrix4 &m, const Real s );
    friend Vec4 operator*( const Matrix4 &m, const Vec4 &v );

public:

    inline Matrix4()
    {
    }
    
    inline Matrix4( const Real a11, const Real a12, const Real a13, const Real a14, const Real a21, const Real a22,
                      const Real a23, const Real a24, const Real a31, const Real a32, const Real a33, const Real a34, const Real a41,
                      const Real a42,
                      const Real a43, const Real a44 )
    {
        SetValue( a11, a12, a13, a14, a21, a22, a23, a24, a31, a32, a33, a34, a41, a42, a43, a44 );
    }
    
    inline Matrix4( const Vec4 &v1, const Vec4 &v2, const Vec4 &v3, const Vec4 &v4 )
    {
        mValues[0] = v1;
        mValues[1] = v2;
        mValues[2] = v3;
        mValues[3] = v4;
    }
    
    inline Matrix4( const Matrix4 &other )
    {
        mValues[0] = other.mValues[0];
        mValues[1] = other.mValues[1];
        mValues[2] = other.mValues[2];
        mValues[3] = other.mValues[3];
    }
    
    inline Matrix4 &operator=( const Matrix4 &m )
    {
        mValues[0] = m.mValues[0];
        mValues[1] = m.mValues[1];
        mValues[2] = m.mValues[2];
        mValues[3] = m.mValues[3];
    
        return *this;
    }
    
    inline Matrix4 &operator*=( const Matrix4 &m )
    {
        SetValue( m.Dotx( mValues[0] ), m.Doty( mValues[0] ), m.Dotz( mValues[0] ), m.Dotw( mValues[0] ),
                  m.Dotx( mValues[1] ), m.Doty( mValues[1] ), m.Dotz( mValues[1] ), m.Dotw( mValues[1] ),
                  m.Dotx( mValues[2] ), m.Doty( mValues[2] ), m.Dotz( mValues[2] ), m.Dotw( mValues[2] ),
                  m.Dotx( mValues[3] ), m.Doty( mValues[3] ), m.Dotz( mValues[3] ), m.Dotw( mValues[3] ) );
    
        return *this;
    }
    
    inline Matrix4 &operator+=( const Matrix4 &m )
    {
        SetValue( mValues[0][0] + m.mValues[0][0], mValues[0][1] + m.mValues[0][1], mValues[0][2] + m.mValues[0][2],
                  mValues[0][3] + m.mValues[0][3],
                  mValues[1][0] + m.mValues[1][0], mValues[1][1] + m.mValues[1][1], mValues[1][2] + m.mValues[1][2],
                  mValues[1][3] + m.mValues[1][3],
                  mValues[2][0] + m.mValues[2][0], mValues[2][1] + m.mValues[2][1], mValues[2][2] + m.mValues[2][2],
                  mValues[2][3] + m.mValues[2][3],
                  mValues[2][0] + m.mValues[3][0], mValues[3][1] + m.mValues[3][1], mValues[3][2] + m.mValues[3][2],
                  mValues[2][3] + m.mValues[3][3] );
    
        return *this;
    }
    
    inline Matrix4 &operator-=( const Matrix4 &m )
    {
        SetValue( mValues[0][0] - m.mValues[0][0], mValues[0][1] - m.mValues[0][1], mValues[0][2] - m.mValues[0][2],
                  mValues[0][3] - m.mValues[0][3],
                  mValues[1][0] - m.mValues[1][0], mValues[1][1] - m.mValues[1][1], mValues[1][2] - m.mValues[1][2],
                  mValues[1][3] - m.mValues[1][3],
                  mValues[2][0] - m.mValues[2][0], mValues[2][1] - m.mValues[2][1], mValues[2][2] - m.mValues[2][2],
                  mValues[2][3] - m.mValues[2][3],
                  mValues[3][0] - m.mValues[3][0], mValues[3][1] - m.mValues[3][1], mValues[3][2] - m.mValues[3][2],
                  mValues[3][3] - m.mValues[3][3] );
    
        return *this;
    }
    
    inline bool operator==( const Matrix4 &m ) const
    {
        return mValues[0] == m.mValues[0] && mValues[1] == m.mValues[1] && mValues[2] == m.mValues[2] &&
               mValues[3] == m.mValues[3];
    }
    
    inline bool operator!=( const Matrix4 &m ) const
    {
        return !( *this == m );
    }
    
    inline void SetColumn( const U8 column, const Vec4 &v )
    {
        mValues[0][column] = v[0];
        mValues[1][column] = v[1];
        mValues[2][column] = v[2];
        mValues[3][column] = v[3];
    }
    
    inline Vec4 GetColumn( const U8 column ) const
    {
        return Vec4( mValues[0][column], mValues[1][column], mValues[2][column], mValues[3][column] );
    }
    
    inline void SetRow( const U8 row, const Vec4 &v )
    {
        mValues[row] = v;
    }
    
    inline Vec4 GetRow( const U8 row ) const
    {
        return mValues[row];
    }
    
    inline Matrix4 Scale( const Vec4 &v ) const
    {
        return Matrix4( mValues[0][0] * v[0], mValues[0][1] * v[1], mValues[0][2] * v[2], mValues[0][3] * v[3],
                        mValues[1][0] * v[0], mValues[1][1] * v[1], mValues[1][2] * v[2], mValues[1][3] * v[3],
                        mValues[2][0] * v[0], mValues[2][1] * v[1], mValues[2][2] * v[2], mValues[2][3] * v[3],
                        mValues[3][0] * v[0], mValues[3][1] * v[1], mValues[3][2] * v[2], mValues[3][3] * v[3] );
    }
    
    inline Matrix4 GetTranspose() const
    {
        return Matrix4( mValues[0][0], mValues[1][0], mValues[2][0], mValues[3][0],
                        mValues[0][1], mValues[1][1], mValues[2][1], mValues[3][1],
                        mValues[0][2], mValues[1][2], mValues[2][2], mValues[3][2],
                        mValues[0][3], mValues[1][3], mValues[2][3], mValues[3][3] );
    }
    
    inline Matrix4 GetAbsolute() const
    {
        return Matrix4( Mathf::Abs( mValues[0][0] ), Mathf::Abs( mValues[0][1] ), Mathf::Abs( mValues[0][2] ),
                        Mathf::Abs( mValues[0][3] ),
                        Mathf::Abs( mValues[1][0] ), Mathf::Abs( mValues[1][1] ), Mathf::Abs( mValues[1][2] ),
                        Mathf::Abs( mValues[1][3] ),
                        Mathf::Abs( mValues[2][0] ), Mathf::Abs( mValues[2][1] ), Mathf::Abs( mValues[2][2] ),
                        Mathf::Abs( mValues[2][3] ),
                        Mathf::Abs( mValues[3][0] ), Mathf::Abs( mValues[3][1] ), Mathf::Abs( mValues[3][2] ),
                        Mathf::Abs( mValues[3][3] ) );
    }
    
    inline Matrix4 GetAdjoint() const
    {
        return Matrix4( GetCofactor( 0, 0 ), GetCofactor( 0, 1 ), GetCofactor( 0, 2 ), GetCofactor( 0, 3 ),
                        GetCofactor( 1, 0 ), GetCofactor( 1, 1 ), GetCofactor( 1, 2 ), GetCofactor( 1, 3 ),
                        GetCofactor( 2, 0 ), GetCofactor( 2, 1 ), GetCofactor( 2, 2 ), GetCofactor( 2, 3 ),
                        GetCofactor( 3, 0 ), GetCofactor( 3, 1 ), GetCofactor( 3, 2 ), GetCofactor( 3, 3 ) );
    }
    
    inline Matrix4 GetInverse() const
    {
        const Real det = GetDeterminant();
    
        assert( det != 0.0f );
    
        const Real s = 1.0f / det;
    
        Matrix4 adjoint  =  GetAdjoint();
        Matrix4 adjointT = adjoint.GetTranspose();
    
        return adjointT * s;
    }
    
    inline void SetIdentity()
    {
        SetValue( 1.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 1.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f );
    }
    
    inline Real GetCofactor( const U8 row, const U8 column ) const
    {
        Real values[9];
    
        U32 valIndex = 0;
    
        for ( U32 i = 0; i < 4; ++i )
        {
            for ( U32 j = 0; j < 4; ++j )
            {
                if ( i == row || j == column )
                {
                    continue;
                }
    
                values[valIndex] = mValues[i][j];
                valIndex++;
            }
        }
    
        Matrix3 localMat( values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8] );
    
        Real cof = localMat.GetDeterminant();
    
        if ( ( ( column + row ) & 1 ) == 1 )
        {
            cof *= -1;
        }
    
        return cof;
    }
    
    inline Real GetDeterminant() const
    {
        return  mValues[0][0] * GetCofactor( 0, 0 ) +
                mValues[1][0] * GetCofactor( 1, 0 ) +
                mValues[2][0] * GetCofactor( 2, 0 ) +
                mValues[3][0] * GetCofactor( 3, 0 );
    }
    
    inline void SetValue( const Real a11, const Real a12, const Real a13, const Real a14, const Real a21, const Real a22,
                            const Real a23, const Real a24, const Real a31, const Real a32, const Real a33, const Real a34, const Real a41,
                            const Real a42,
                            const Real a43, const Real a44 )
    {
        mValues[0].SetValues( a11, a12, a13, a14 );
        mValues[1].SetValues( a21, a22, a23, a24 );
        mValues[2].SetValues( a31, a32, a33, a34 );
        mValues[3].SetValues( a41, a42, a43, a44 );
    }
    
    static inline Matrix4 GetZero()
    {
        return Matrix4( Vec4::GetZero(), Vec4::GetZero(), Vec4::GetZero(), Vec4::GetZero() );
    }
    
    static inline Matrix4 GetIdentity()
    {
        Matrix4 mat;
        mat.SetIdentity();
        return mat;
    }
    
    inline Matrix3 Extract3x3() const
    {
        return Matrix3( mValues[0][0], mValues[0][1], mValues[0][2],
                        mValues[1][0], mValues[1][1], mValues[1][2],
                        mValues[2][0], mValues[2][1], mValues[2][2] );
    }
    
    inline const Vec4 &operator[]( const U8 row ) const
    {
        return mValues[row];
    }

private:

    Vec4 mValues[4];

    inline Real Dotx( const Vec4 &v ) const
    {
        return mValues[0][0] * v[0] + mValues[1][0] * v[1] + mValues[2][0] * v[2] + mValues[3][0] * v[3];
    }
    
    inline Real Doty( const Vec4 &v ) const
    {
        return mValues[0][1] * v[0] + mValues[1][1] * v[1] + mValues[2][1] * v[2] + mValues[3][1] * v[3];
    }
    
    inline Real Dotz( const Vec4 &v ) const
    {
        return mValues[0][2] * v[0] + mValues[1][2] * v[1] + mValues[2][2] * v[2] + mValues[3][2] * v[3];
    }
    
    inline Real Dotw( const Vec4 &v ) const
    {
        return mValues[0][3] * v[0] + mValues[1][3] * v[1] + mValues[2][3] * v[2] + mValues[3][3] * v[3];
    }
};

inline Matrix4 operator+( const Matrix4 &m1, const Matrix4 &m2 )
{
    return Matrix4( m1.mValues[0][0] + m2.mValues[0][0], m1.mValues[0][1] + m2.mValues[0][1],
                    m1.mValues[0][2] + m2.mValues[0][2], m1.mValues[0][3] + m2.mValues[0][3],

                    m1.mValues[1][0] + m2.mValues[1][0], m1.mValues[1][1] + m2.mValues[1][1],
                    m1.mValues[1][2] + m2.mValues[1][2], m1.mValues[1][3] + m2.mValues[1][3],

                    m1.mValues[2][0] + m2.mValues[2][0], m1.mValues[2][1] + m2.mValues[2][1],
                    m1.mValues[2][2] + m2.mValues[2][2], m1.mValues[2][3] + m2.mValues[2][3],

                    m1.mValues[3][0] + m2.mValues[3][0], m1.mValues[3][1] + m2.mValues[3][1],
                    m1.mValues[3][2] + m2.mValues[3][2], m1.mValues[3][3] + m2.mValues[3][3] );
}

inline Matrix4 operator-( const Matrix4 &m1, const Matrix4 &m2 )
{
    return Matrix4( m1.mValues[0][0] - m2.mValues[0][0], m1.mValues[0][1] - m2.mValues[0][1],
                    m1.mValues[0][2] - m2.mValues[0][2], m1.mValues[0][3] - m2.mValues[0][3],

                    m1.mValues[1][0] - m2.mValues[1][0], m1.mValues[1][1] - m2.mValues[1][1],
                    m1.mValues[1][2] - m2.mValues[1][2], m1.mValues[1][3] - m2.mValues[1][3],

                    m1.mValues[2][0] - m2.mValues[2][0], m1.mValues[2][1] - m2.mValues[2][1],
                    m1.mValues[2][2] - m2.mValues[2][2], m1.mValues[2][3] - m2.mValues[2][3],

                    m1.mValues[3][0] - m2.mValues[3][0], m1.mValues[3][1] - m2.mValues[3][1],
                    m1.mValues[3][2] - m2.mValues[3][2], m1.mValues[3][3] - m2.mValues[3][3] );
}

inline Matrix4 operator*( const Matrix4 &m1, const Matrix4 &m2 )
{
    return Matrix4( m2.Dotx( m1.mValues[0] ), m2.Doty( m1.mValues[0] ), m2.Dotz( m1.mValues[0] ), m2.Dotw( m1.mValues[0] ),
                    m2.Dotx( m1.mValues[1] ), m2.Doty( m1.mValues[1] ), m2.Dotz( m1.mValues[1] ), m2.Dotw( m1.mValues[1] ),
                    m2.Dotx( m1.mValues[2] ), m2.Doty( m1.mValues[2] ), m2.Dotz( m1.mValues[2] ), m2.Dotw( m1.mValues[2] ),
                    m2.Dotx( m1.mValues[3] ), m2.Doty( m1.mValues[3] ), m2.Dotz( m1.mValues[3] ), m2.Dotw( m1.mValues[3] ) );
}
inline Matrix4 operator*( const Matrix4 &m, const Real s )
{
    return Matrix4( m.mValues[0][0] * s, m.mValues[0][1] * s, m.mValues[0][2] * s, m.mValues[0][3] * s,
                    m.mValues[1][0] * s, m.mValues[1][1] * s, m.mValues[1][2] * s, m.mValues[1][3] * s,
                    m.mValues[2][0] * s, m.mValues[2][1] * s, m.mValues[2][2] * s, m.mValues[2][3] * s,
                    m.mValues[3][0] * s, m.mValues[3][1] * s, m.mValues[3][2] * s, m.mValues[3][3] * s );
}

inline Vec4 operator*( const Matrix4 &m, const Vec4 &v )
{
    return Vec4( m.mValues[0].Dot( v ), m.mValues[1].Dot( v ), m.mValues[2].Dot( v ), m.mValues[3].Dot( v ) );
}

*/

#endif