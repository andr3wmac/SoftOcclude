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
#ifndef __PRIMARY_MATRIX_BASE_H__
#define __PRIMARY_MATRIX_BASE_H__

#include "math/config.h"
#include "math/generic/genericMatrixData.h"

BEGIN_MATH_NAMESPACE

template< class TYPE, U32 ROWS, U32 COLS >
class PrimaryMatrixBase : public GenericMatrixData< TYPE, ROWS, COLS >
{
public:

    // Accessors
    FORCE_INLINE const PrimaryVectorBase< TYPE, COLS > & operator[](U32 i) const
    {
        assert(i < ROWS);

        return this->mRows[i];
    }

    FORCE_INLINE PrimaryVectorBase< TYPE, COLS > &operator[](U32 i)
    {
        assert(i < ROWS);

        return this->mRows[i];
    }

    FORCE_INLINE PrimaryMatrixBase< TYPE, ROWS, COLS > &operator=(const PrimaryMatrixBase< TYPE, ROWS, COLS > &m)
    {
        for (U32 i = 0; i < ROWS; ++i)
        {
            for (U32 j = 0; j < COLS; ++j)
            {
                this->mRows[i][j] = m.mRows[i][j];
            }
        };

        return *this;
    }

    FORCE_INLINE PrimaryMatrixBase< TYPE, ROWS, COLS > &operator*=(const PrimaryMatrixBase< TYPE, ROWS, COLS > &m)
    {
        PrimaryMatrixBase< TYPE, ROWS, COLS > newMat;

        for (U32 i = 0; i < ROWS; ++i)
        {
            for (U32 j = 0; j < COLS; ++j)
            {
                newMat[i][j] = m.DotColumn(j, this->mRows[i]);
            }
        }

        *this = newMat;

        return *this;
    }

    FORCE_INLINE PrimaryMatrixBase< TYPE, ROWS, COLS > &operator+=(const PrimaryMatrixBase< TYPE, ROWS, COLS > &m)
    {
        for (U32 i = 0; i < ROWS; ++i)
        {
            for (U32 j = 0; j < COLS; ++j)
            {
                this->mRows[i][j] += m.mRows[i][j];
            }
        }

        return *this;
    }

    FORCE_INLINE PrimaryMatrixBase< TYPE, ROWS, COLS > &operator-=(const PrimaryMatrixBase< TYPE, ROWS, COLS > &m)
    {
        for (U32 i = 0; i < ROWS; ++i)
        {
            for (U32 j = 0; j < COLS; ++j)
            {
                this->mRows[i][j] -= m.mRows[i][j];
            }
        }

        return *this;
    }

    FORCE_INLINE bool operator==(const PrimaryMatrixBase< TYPE, ROWS, COLS > &m) const
    {
        bool result = true;

        for (U32 i = 0; i < ROWS; ++i)
        {
            result &= this->mRows[i] == m.mRows[i];
        }

        return result;
    }

    FORCE_INLINE bool operator!=(const PrimaryMatrixBase< TYPE, ROWS, COLS > &m) const
    {
        return !(*this == m);
    }

    FORCE_INLINE void SetColumn(const U8 column, const PrimaryVectorBase< TYPE, ROWS > &v)
    {
        for (U32 j = 0; j < COLS; ++j)
        {
            this->mRows[j][column] = v[j];
        }
    }

    FORCE_INLINE PrimaryVectorBase< TYPE, ROWS > GetColumn(const U8 column) const
    {
        PrimaryVectorBase< TYPE, COLS > newVec;

        for (U32 j = 0; j < COLS; ++j)
        {
            newVec[j] = this->mRows[j][column];
        }

        return newVec;
    }

    FORCE_INLINE void SetRow(const U8 row, const PrimaryVectorBase< TYPE, COLS > &v)
    {
        this->mRows[row] = v;
    }

    FORCE_INLINE PrimaryVectorBase< TYPE, COLS > GetRow(const U8 row) const
    {
        return this->mRows[row];
    }

    FORCE_INLINE PrimaryMatrixBase< TYPE, COLS, ROWS > GetTranspose() const
    {
        PrimaryMatrixBase< TYPE, COLS, ROWS > transpMat;

        for (U32 i = 0; i < ROWS; ++i)
        {
            transpMat.SetColumn(i, this->mRows[i]);
        }

        return transpMat;
    }

    FORCE_INLINE PrimaryMatrixBase< TYPE, ROWS, COLS > GetAbsolute() const
    {
        PrimaryMatrixBase< TYPE, ROWS, COLS > newMat;

        for (U32 i = 0; i < ROWS; ++i)
        {
            newMat.SetRow(i, this->mRows[i].Absolute() );
        }
        
        return newMat;
    }

    FORCE_INLINE TYPE GetCofactor(const U8 row, const U8 column) const
    {
        // Make an matrix that is -1 smaller in ROWS and COLS
        PrimaryMatrixBase< TYPE, ROWS - 1, COLS - 1 > localMat;

        U32 irow = 0;

        for (U32 i = 0; i < ROWS; ++i)
        {
            if (i == row)
            {
                continue;
            }

            U32 jcol = 0;

            for (U32 j = 0; j < COLS; ++j)
            {
                if (j == column)
                {
                    continue;
                }

                localMat[irow][jcol] = this->mRows[i][j];

                jcol++;
            }

            irow++;
        }


        TYPE cof = localMat.GetDeterminant();

        if (((column + row) & 1) == 1)
        {
            cof *= -1;
        }

        return cof;
    }

    FORCE_INLINE PrimaryMatrixBase< TYPE, ROWS, COLS > GetAdjoint() const
    {
        PrimaryMatrixBase< TYPE, ROWS, COLS > localMat;

        for (U32 i = 0; i < ROWS; ++i)
        {
            for (U32 j = 0; j < COLS; ++j)
            {
                localMat[i][j] = GetCofactor(i, j);
            }
        }

        return localMat;
    }

    FORCE_INLINE PrimaryMatrixBase< TYPE, ROWS, COLS > GetInverse() const
    {
        const TYPE det = GetDeterminant();

        assert( det != 0.0f );

        const TYPE s = 1.0f / det;
        return GetAdjoint().GetTranspose() * s;
    }

    FORCE_INLINE void SetIdentity()
    {
        for (U32 i = 0; i < ROWS; ++i)
        {
            for (U32 j = 0; j < COLS; ++j)
            {
                if (i == j)
                {
                    this->mRows[i][j] = 1.0;
                }
                else
                {
                    this->mRows[i][j] = 0.0;
                }    
            }
        }
    }

    FORCE_INLINE TYPE GetDeterminant() const
    {
        TYPE result = 0;

        for (U32 i = 0; i < ROWS; ++i)
        {
            result += this->mRows[i][0] * GetCofactor(i, 0);
        }

        return result;
    }

    static FORCE_INLINE PrimaryMatrixBase< TYPE, ROWS, COLS > GetZero()
    {
        PrimaryMatrixBase< TYPE, ROWS, COLS > localMat;

        for (U32 i = 0; i < ROWS; ++i)
        {
            for (U32 j = 0; j < COLS; ++j)
            {
                localMat[i][j] = 0.0;
            }
        }

        return localMat;
    }

    static FORCE_INLINE PrimaryMatrixBase< TYPE, ROWS, COLS > GetIdentity()
    {
        PrimaryMatrixBase< TYPE, ROWS, COLS > mat;
        mat.SetIdentity();
        return mat;
    }

    FORCE_INLINE TYPE DotColumn( U32 tcol, const PrimaryVectorBase< TYPE, COLS > &v) const
    {
        TYPE result = 0;

        for (U32 i = 0; i < ROWS; ++i)
        {
            result += this->mRows[i][tcol] * v[i];
        }

        return result;
    }
};

template< class TYPE, U32 ROWS, U32 COLS >
FORCE_INLINE PrimaryMatrixBase< TYPE, ROWS, COLS > operator+( 
       const PrimaryMatrixBase< TYPE, ROWS, COLS > &m1, 
       const PrimaryMatrixBase< TYPE, ROWS, COLS > &m2 )
{
    PrimaryMatrixBase< TYPE, ROWS, COLS > newMat;
    
    for (U32 i = 0; i < ROWS; ++i)
    {
        for (U32 j = 0; j < COLS; ++j)
        {
            newMat[i][j] = m1.mRows[i][j] += m2.mRows[i][j];
        }
    }
    
    return newMat;
}

template< class TYPE, U32 ROWS, U32 COLS >
FORCE_INLINE PrimaryMatrixBase< TYPE, ROWS, COLS > operator-( 
        const PrimaryMatrixBase< TYPE, ROWS, COLS > &m1, 
        const PrimaryMatrixBase< TYPE, ROWS, COLS > &m2 )
{
    PrimaryMatrixBase< TYPE, ROWS, COLS > newMat;
    
    for (U32 i = 0; i < ROWS; ++i)
    {
        for (U32 j = 0; j < COLS; ++j)
        {
           newMat[i][j] = m1.mRows[i][j] -= m2.mRows[i][j];
        }
    }
    
    return newMat;
}

template< class TYPE, U32 ROWS, U32 COLS >
FORCE_INLINE PrimaryMatrixBase< TYPE, ROWS, COLS > operator*( 
        const PrimaryMatrixBase< TYPE, ROWS, COLS > &m1, 
        const PrimaryMatrixBase< TYPE, ROWS, COLS > &m2 )
{
    PrimaryMatrixBase< TYPE, ROWS, COLS > newMat;
    
    for (U32 i = 0; i < ROWS; ++i)
    {
        for (U32 j = 0; j < COLS; ++j)
        {
            newMat[i][j] = m2.DotColumn(j, m1.mRows[i]);
        }
    }
    
    return newMat;
}

template< class TYPE, U32 ROWS, U32 COLS >
FORCE_INLINE PrimaryVectorBase< TYPE, ROWS > operator*( 
        const PrimaryMatrixBase< TYPE, ROWS, COLS > &m, 
        const PrimaryVectorBase< TYPE, COLS > &v )
{
    PrimaryVectorBase< TYPE, ROWS > newVec;
    
    for (U32 i = 0; i < ROWS; ++i)
    {
        newVec[i] = m.mRows[i].Dot( v );   
    }
    
    return newVec;
}

END_MATH_NAMESPACE


#endif