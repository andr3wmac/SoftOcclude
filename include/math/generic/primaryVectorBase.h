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
#ifndef __INTEGER_VECTOR_H__
#define __INTEGER_VECTOR_H__

#include "math/config.h"
#include "math/generic/genericVectorData.h"

template< class TYPE, U32 N >
class PrimaryVectorBase : public GenericVectorData< TYPE, N >
{
public:

    // STATICS
    static U32 Width()
    {
        return N;
    }

    // Accessors
    FORCE_INLINE const TYPE & operator[](U32 i) const
    {
        assert(i < N);

        return this->mValues[i];
    }

    FORCE_INLINE TYPE &operator[](U32 i)
    {
        assert(i < N);

        return this->mValues[i];
    }

    // Self operators
    FORCE_INLINE PrimaryVectorBase< TYPE, N > &operator+=(const PrimaryVectorBase< TYPE, N > &v)
    {
        for (U32 i = 0; i < N; ++i)
        {
            this->mValues[i] += v[i];
        }

        return *(PrimaryVectorBase< TYPE, N >*)this;
    }
    
    FORCE_INLINE PrimaryVectorBase< TYPE, N > &operator-=(const PrimaryVectorBase< TYPE, N > &v)
    {
        for (U32 i = 0; i < N; ++i)
        {
            this->mValues[i] -= v[i];
        }

        return *(PrimaryVectorBase< TYPE, N >*)this;
    }

    FORCE_INLINE PrimaryVectorBase< TYPE, N > &operator /= (const TYPE s)
    {
        assert(s != 0.0f);

        for (U32 i = 0; i < N; ++i)
        {
            this->mValues[i] /= s;
        }

        return *(PrimaryVectorBase< TYPE, N >*)this;
    }

    FORCE_INLINE PrimaryVectorBase< TYPE, N > &operator *= (const TYPE s)
    {
        for (U32 i = 0; i < N; ++i)
        {
            this->mValues[i] *= s;
        }

        return *(PrimaryVectorBase< TYPE, N >*)this;
    }

    // Equality
    FORCE_INLINE bool operator==(const PrimaryVectorBase< TYPE, N > &other) const
    {
        bool result = true;

        for (U32 i = 0; i < N; ++i)
        {
            result &= Mathf::Equal(this->mValues[i], other.mValues[i]);
        }

        return result;
    }

    FORCE_INLINE bool operator!=(const PrimaryVectorBase< TYPE, N > &other) const
    {
        return !(PrimaryVectorBase< TYPE, N >(*this) == other);
    }

    // Internal intrinsics
    FORCE_INLINE TYPE Dot(const PrimaryVectorBase< TYPE, N > &v) const
    {
        TYPE result = this->mValues[0] * v.mValues[0];

        for (U32 i = 1; i < N; ++i)
        {
            result += this->mValues[i] * v.mValues[i];
        }

        return  result;
    }

    FORCE_INLINE TYPE Length2() const
    {
        return Dot(PrimaryVectorBase< TYPE, N >(*this));
    }

    FORCE_INLINE TYPE Distance2(const PrimaryVectorBase< TYPE, N > &v) const
    {
        return (PrimaryVectorBase< TYPE, N >(*this) - v).Length2();
    }

    FORCE_INLINE void SetValue(U32 axis, TYPE val)
    {
        this->mValues[axis] = val;
    }

    FORCE_INLINE PrimaryVectorBase< TYPE, N > Absolute() const
    {
        PrimaryVectorBase< TYPE, N > newVec;

        for (U32 i = 0; i < N; ++i)
        {
            newVec.SetValue(i, Mathf::Abs(this->mValues[i]));
        }

        return newVec;
    }

    FORCE_INLINE U8 MinAxis() const
    {
        U8 axis = 0;
        TYPE val = this->mValues[0];

        for (U8 i = 1; i < N; ++i)
        {
            const TYPE considerate = this->mValues[i];

            if (considerate < val)
            {
                val = considerate;
                axis = i;
            }
        }

        return axis;
    }

    FORCE_INLINE U8 MaxAxis() const
    {
        U8 axis = 0;
        TYPE val = this->mValues[0];

        for (U8 i = 1; i < N; ++i)
        {
            const TYPE considerate = this->mValues[i];

            if (considerate > val)
            {
                val = considerate;
                axis = i;
            }
        }

        return axis;
    }

    FORCE_INLINE void SetZero()
    {
        for (U32 i = 0; i < N; ++i)
        {
            SetValue(i, 0);
        }
    }

    FORCE_INLINE bool IsZero() const
    {
        bool result = true;

        for (U32 i = 0; i < N; ++i)
        {
            result &= (this->mValues[i] == 0);
        }

        return result;
    }

    FORCE_INLINE bool IsFuzzyZero() const
    {
        return Length2() < Mathf::GetEpsilon();
    }

    FORCE_INLINE void Clear()
    {
        SetZero();
    }
};

template< class TYPE, U32 N >
FORCE_INLINE PrimaryVectorBase< TYPE, N > operator+(const PrimaryVectorBase< TYPE, N > &v1, const PrimaryVectorBase< TYPE, N > &v2)
{
    PrimaryVectorBase< TYPE, N > newVec;

    for (U32 i = 0; i < PrimaryVectorBase< TYPE, N >::Width(); ++i)
    {
        newVec[i] = v1[i] + v2[i];
    }

    return newVec;
}

template< class TYPE, U32 N >
FORCE_INLINE PrimaryVectorBase< TYPE, N > operator*(const PrimaryVectorBase< TYPE, N > &v1, const PrimaryVectorBase< TYPE, N > &v2)
{
    PrimaryVectorBase< TYPE, N > newVec;

    for (U32 i = 0; i < PrimaryVectorBase< TYPE, N >::Width(); ++i)
    {
        newVec[i] = v1[i] * v2[i];
    }

    return newVec;
}

template< class TYPE, U32 N >
FORCE_INLINE PrimaryVectorBase< TYPE, N > operator-(const PrimaryVectorBase< TYPE, N > &v1, const PrimaryVectorBase< TYPE, N > &v2)
{
    PrimaryVectorBase< TYPE, N > newVec;

    for (U32 i = 0; i < PrimaryVectorBase< TYPE, N >::Width(); ++i)
    {
        newVec[i] = v1[i] - v2[i];
    }

    return newVec;
}

template< class TYPE, U32 N >
FORCE_INLINE PrimaryVectorBase< TYPE, N > operator-(const PrimaryVectorBase< TYPE, N > &v)
{
    PrimaryVectorBase< TYPE, N > newVec;

    for (U32 i = 0; i < PrimaryVectorBase< TYPE, N >::Width(); ++i)
    {
        newVec[i] = -v[i];
    }

    return newVec;
}

template< class TYPE, U32 N >
FORCE_INLINE PrimaryVectorBase< TYPE, N > operator*(const PrimaryVectorBase< TYPE, N > &v, const TYPE s)
{
    PrimaryVectorBase< TYPE, N > newVec;

    for (U32 i = 0; i < PrimaryVectorBase< TYPE, N >::Width(); ++i)
    {
        newVec[i] = v[i] * s;
    }

    return newVec;
}

template< class TYPE, U32 N >
FORCE_INLINE PrimaryVectorBase< TYPE, N > operator*(TYPE s, const PrimaryVectorBase< TYPE, N > &v)
{
    return v * s;
}

template< class TYPE, U32 N >
FORCE_INLINE PrimaryVectorBase< TYPE, N > operator/(const PrimaryVectorBase< TYPE, N > &v, TYPE s)
{
    assert(s != TYPE(0));
    
    PrimaryVectorBase< TYPE, N > newVec;

    for (U32 i = 0; i < PrimaryVectorBase< TYPE, N >::Width(); ++i)
    {
        newVec[i] = v[i] / s;
    }

    return newVec;
}

template< class TYPE, U32 N >
FORCE_INLINE PrimaryVectorBase< TYPE, N > operator/(const PrimaryVectorBase< TYPE, N > &v1, const PrimaryVectorBase< TYPE, N > &v2)
{
    PrimaryVectorBase< TYPE, N > newVec;

    for (U32 i = 0; i < PrimaryVectorBase< TYPE, N >::Width(); ++i)
    {
        newVec[i] = v1[i] / v2[i];
    }

    return newVec;
}


#endif