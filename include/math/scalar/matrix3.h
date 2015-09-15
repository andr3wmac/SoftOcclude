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
#ifndef __MATRIX3_H__
#define __MATRIX3_H__

#include "math/types.h"
#include "math/config.h"
#include "math/scalar/vec3.h"
#include "math/generic/primaryMatrixBase.h"

#include <assert.h>

class Matrix3 : public PrimaryMatrixBase< Real, 3, 3 >
{
public:

    inline Matrix3()
    {
    }

    inline Matrix3( const Real a11, const Real a12, const Real a13,
                    const Real a21, const Real a22, const Real a23,
                    const Real a31, const Real a32, const Real a33 )
    {
        SetValues(a11, a12, a13, a21, a22, a23, a31, a32, a33);
    }

    inline Matrix3(const Matrix3 &other)
    {
        this->mRows[0] = other.mRows[0];
        this->mRows[1] = other.mRows[1];
        this->mRows[2] = other.mRows[2];
    }

    inline Matrix3(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3)
    {
        this->mRows[0] = v1;
        this->mRows[1] = v2;
        this->mRows[2] = v3;
    }

    inline Matrix3(const PrimaryMatrixBase< Real, 3, 3 > &other)
    {
        this->mRows[0] = other.mRows[0];
        this->mRows[1] = other.mRows[1];
        this->mRows[2] = other.mRows[2];
    }

    inline void SetValues( const Real a11, const Real a12, const Real a13,
                           const Real a21, const Real a22, const Real a23,
                           const Real a31, const Real a32, const Real a33 )
    {
        this->mRows[0].SetValues(a11, a12, a13);
        this->mRows[1].SetValues(a21, a22, a23);
        this->mRows[2].SetValues(a31, a32, a33);
    }

   
    /*
    inline void SetEuler(const Real yaw, const Real pitch, const Real roll)
    {
        Real cy(Mathf::Cos(yaw));
        Real cp(Mathf::Cos(pitch));
        Real cr(Mathf::Cos(roll));

        Real sy(Mathf::Sin(yaw));
        Real sp(Mathf::Sin(pitch));
        Real sr(Mathf::Sin(roll));


        Real cc = cr * cy;
        Real cs = cr * sy;
        Real sc = sr * cy;
        Real ss = sr * sy;

        SetValue( cp * cy,
                  sp * sc - cs, 
                  sp * cc + ss, 
                  cp * sy, 
                  sp * ss + cc, 
                  sp * cs - sc, 
                 -sp, 
                  cp * sr, 
                  cp * cr );
    }
    */

};


/*
class Quaternion;

class Matrix3
{
    friend Matrix3 operator+( const Matrix3 &m1, const Matrix3 &m2 );
    friend Matrix3 operator-( const Matrix3 &m1, const Matrix3 &m2 );
    friend Matrix3 operator*( const Matrix3 &m1, const Matrix3 &m2 );
    friend Matrix3 operator*( const Matrix3 &m, const Real s );
    friend Vec3 operator*( const Matrix3 &m, const Vec3 &v );

public:

    inline Matrix3()
    {
    }
    
    inline Matrix3( const Real a11, const Real a12, const Real a13,
                      const Real a21, const Real a22, const Real a23,
                      const Real a31, const Real a32, const Real a33 )
    {
        SetValue( a11, a12, a13, a21, a22, a23, a31, a32, a33 );
    }
    
    inline Matrix3( const Matrix3 &other )
    {
        mValues[0] = other.mValues[0];
        mValues[1] = other.mValues[1];
        mValues[2] = other.mValues[2];
    }
    
    inline Matrix3( const Vec3 &v1, const Vec3 &v2, const Vec3 &v3 )
    {
        mValues[0] = v1;
        mValues[1] = v2;
        mValues[2] = v3;
    }
    
    inline Matrix3 &operator=( const Matrix3 &m )
    {
        mValues[0] = m.mValues[0];
        mValues[1] = m.mValues[1];
        mValues[2] = m.mValues[2];
    
        return *this;
    }
    
    inline Matrix3 &operator*=( const Matrix3 &m )
    {
        SetValue( m.Dotx( mValues[0] ), m.Doty( mValues[0] ), m.Dotz( mValues[0] ),
                  m.Dotx( mValues[1] ), m.Doty( mValues[1] ), m.Dotz( mValues[1] ),
                  m.Dotx( mValues[2] ), m.Doty( mValues[2] ), m.Dotz( mValues[2] ) );
    
        return *this;
    }
    
    inline Matrix3 &operator+=( const Matrix3 &m )
    {
        SetValue( mValues[0][0] + m.mValues[0][0], mValues[0][1] + m.mValues[0][1], mValues[0][2] + m.mValues[0][2],
                  mValues[1][0] + m.mValues[1][0], mValues[1][1] + m.mValues[1][1], mValues[1][2] + m.mValues[1][2],
                  mValues[2][0] + m.mValues[2][0], mValues[2][1] + m.mValues[2][1], mValues[2][2] + m.mValues[2][2] );
    
        return *this;
    }
    
    inline Matrix3 &operator-=( const Matrix3 &m )
    {
        SetValue( mValues[0][0] - m.mValues[0][0], mValues[0][1] - m.mValues[0][1], mValues[0][2] - m.mValues[0][2],
                  mValues[1][0] - m.mValues[1][0], mValues[1][1] - m.mValues[1][1], mValues[1][2] - m.mValues[1][2],
                  mValues[2][0] - m.mValues[2][0], mValues[2][1] - m.mValues[2][1], mValues[2][2] - m.mValues[2][2] );
    
        return *this;
    }
    
    inline bool operator==( const Matrix3 &m ) const
    {
        return mValues[0] == m.mValues[0] && mValues[1] == m.mValues[1] && mValues[2] == m.mValues[2];
    }
    
    inline bool operator!=( const Matrix3 &m ) const
    {
        return !( *this == m );
    }
    
    inline void SetColumn( const U8 column, const Vec3 &v )
    {
        mValues[0][column] = v[0];
        mValues[1][column] = v[1];
        mValues[2][column] = v[2];
    }
    
    inline Vec3 GetColumn( const U8 column ) const
    {
        return Vec3( mValues[0][column], mValues[1][column], mValues[2][column] );
    }
    
    inline void SetRow( const U8 row, const Vec3 &v )
    {
        mValues[row] = v;
    }
    
    inline Vec3 GetRow( const U8 row ) const
    {
        return mValues[row];
    }
    
    inline void SetEuler( const Real yaw, const Real pitch, const Real roll )
    {
        Real cy( Mathf::Cos( yaw ) );
        Real cp( Mathf::Cos( pitch ) );
        Real cr( Mathf::Cos( roll ) );
    
        Real sy( Mathf::Sin( yaw ) );
        Real sp( Mathf::Sin( pitch ) );
        Real sr( Mathf::Sin( roll ) );
    
    
        Real cc = cr * cy;
        Real cs = cr * sy;
        Real sc = sr * cy;
        Real ss = sr * sy;
    
        SetValue( cp * cy, sp * sc - cs, sp * cc + ss,
                  cp * sy, sp * ss + cc, sp * cs - sc,
                  -sp, cp * sr, cp * cr );
    }
    
    inline Matrix3 Scale( const Vec3 &v ) const
    {
        return Matrix3( mValues[0][0] * v[0], mValues[0][1] * v[1], mValues[0][2] * v[2],
                        mValues[1][0] * v[0], mValues[1][1] * v[1], mValues[1][2] * v[2],
                        mValues[2][0] * v[0], mValues[2][1] * v[1], mValues[2][2] * v[2] );
    }
    
    inline Matrix3 GetTranspose() const
    {
        return Matrix3( mValues[0][0], mValues[1][0], mValues[2][0],
                        mValues[0][1], mValues[1][1], mValues[2][1],
                        mValues[0][2], mValues[1][2], mValues[2][2] );
    }
    
    inline Matrix3 GetAbsolute() const
    {
        return Matrix3( Mathf::Abs( mValues[0][0] ), Mathf::Abs( mValues[0][1] ), Mathf::Abs( mValues[0][2] ),
                        Mathf::Abs( mValues[1][0] ), Mathf::Abs( mValues[1][1] ), Mathf::Abs( mValues[1][2] ),
                        Mathf::Abs( mValues[2][0] ), Mathf::Abs( mValues[2][1] ), Mathf::Abs( mValues[2][2] ) );
    }
    
    inline Matrix3 GetAdjoint() const
    {
        return Matrix3( GetCofactor( 0, 0 ), GetCofactor( 0, 1 ), GetCofactor( 0, 2 ),
                        GetCofactor( 1, 0 ), GetCofactor( 1, 1 ), GetCofactor( 1, 2 ),
                        GetCofactor( 2, 0 ), GetCofactor( 2, 1 ), GetCofactor( 2, 2 ) );
    }
    
    inline Matrix3 GetInverse() const
    {
        const Real det = GetDeterminant();
    
        assert( det != 0.0f );
    
        const Real s = 1.0f / det;
        return GetAdjoint().GetTranspose() * s;
    }
    
    inline void SetIdentity()
    {
        SetValue( 1.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f,
                  0.0f, 0.0f, 1.0f );
    }
    
    inline Real GetCofactor( const U8 row, const U8 column ) const
    {
        Real cof = mValues[( row + 1 ) % 3][( column + 1 ) % 3] * mValues[( row + 2 ) % 3][( column + 2 ) % 3] -
                   mValues[( row + 1 ) % 3][( column + 2 ) % 3] * mValues[( row + 2 ) % 3][( column + 1 ) % 3];
    
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
                mValues[2][0] * GetCofactor( 2, 0 );
    }
    
    inline   void SetValue( const Real a11, const Real a12, const Real a13,
                            const Real a21, const Real a22, const Real a23,
                            const Real a31, const Real a32, const Real a33 )
    {
        mValues[0].SetValues( a11, a12, a13 );
        mValues[1].SetValues( a21, a22, a23 );
        mValues[2].SetValues( a31, a32, a33 );
    }
    
    
    
    static inline Matrix3 GetZero()
    {
        return Matrix3( Vec3::GetZero(), Vec3::GetZero(), Vec3::GetZero() );
    }
    
    static inline Matrix3 GetIdentity()
    {
        Matrix3 mat;
        mat.SetIdentity();
        return mat;
    }

private:

    Vec3 mValues[3];

    inline Real Dotx( const Vec3 &v ) const
    {
        return mValues[0][0] * v[0] + mValues[1][0] * v[1] + mValues[2][0] * v[2];
    }
    
    inline Real Doty( const Vec3 &v ) const
    {
        return mValues[0][1] * v[0] + mValues[1][1] * v[1] + mValues[2][1] * v[2];
    }
    
    inline Real Dotz( const Vec3 &v ) const
    {
        return mValues[0][2] * v[0] + mValues[1][2] * v[1] + mValues[2][2] * v[2];
    }
};


inline Matrix3 operator+( const Matrix3 &m1, const Matrix3 &m2 )
{
    return Matrix3( m1.mValues[0][0] + m2.mValues[0][0], m1.mValues[0][1] + m2.mValues[0][1],
                    m1.mValues[0][2] + m2.mValues[0][2],
                    m1.mValues[1][0] + m2.mValues[1][0], m1.mValues[1][1] + m2.mValues[1][1],
                    m1.mValues[1][2] + m2.mValues[1][2],
                    m1.mValues[2][0] + m2.mValues[2][0], m1.mValues[2][1] + m2.mValues[2][1],
                    m1.mValues[2][2] + m2.mValues[2][2] );
}

inline Matrix3 operator-( const Matrix3 &m1, const Matrix3 &m2 )
{
    return Matrix3( m1.mValues[0][0] - m2.mValues[0][0], m1.mValues[0][1] - m2.mValues[0][1],
                    m1.mValues[0][2] - m2.mValues[0][2],
                    m1.mValues[1][0] - m2.mValues[1][0], m1.mValues[1][1] - m2.mValues[1][1],
                    m1.mValues[1][2] - m2.mValues[1][2],
                    m1.mValues[2][0] - m2.mValues[2][0], m1.mValues[2][1] - m2.mValues[2][1],
                    m1.mValues[2][2] - m2.mValues[2][2] );
}

inline Matrix3 operator*( const Matrix3 &m1, const Matrix3 &m2 )
{
    return Matrix3( m2.Dotx( m1.mValues[0] ), m2.Doty( m1.mValues[0] ), m2.Dotz( m1.mValues[0] ),
                    m2.Dotx( m1.mValues[1] ), m2.Doty( m1.mValues[1] ), m2.Dotz( m1.mValues[1] ),
                    m2.Dotx( m1.mValues[2] ), m2.Doty( m1.mValues[2] ), m2.Dotz( m1.mValues[2] ) );
}

inline Matrix3 operator*( const Matrix3 &m, const Real s )
{
    return Matrix3( m.mValues[0][0] * s, m.mValues[0][1] * s, m.mValues[0][2] * s,
                    m.mValues[1][0] * s, m.mValues[1][1] * s, m.mValues[1][2] * s,
                    m.mValues[2][0] * s, m.mValues[2][1] * s, m.mValues[2][2] * s );
}

inline Vec3 operator*( const Matrix3 &m, const Vec3 &v )
{
    return Vec3( m.mValues[0].Dot( v ), m.mValues[1].Dot( v ), m.mValues[2].Dot( v ) );
}

*/

#endif