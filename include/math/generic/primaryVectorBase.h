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
    inline const TYPE & operator[](U32 i) const
    {
        assert(i < N);

        return this->mValues[i];
    }

    inline TYPE &operator[](U32 i)
    {
        assert(i < N);

        return this->mValues[i];
    }

    // Self operators
    inline PrimaryVectorBase< TYPE, N > &operator+=(const PrimaryVectorBase< TYPE, N > &v)
    {
        for (U32 i = 0; i < N; ++i)
        {
            this->mValues[i] += v[i];
        }

        return *(PrimaryVectorBase< TYPE, N >*)this;
    }
    
    inline PrimaryVectorBase< TYPE, N > &operator-=(const PrimaryVectorBase< TYPE, N > &v)
    {
        for (U32 i = 0; i < N; ++i)
        {
            this->mValues[i] -= v[i];
        }

        return *(PrimaryVectorBase< TYPE, N >*)this;
    }

    inline PrimaryVectorBase< TYPE, N > &operator /= (const TYPE s)
    {
        assert(s != 0.0f);

        for (U32 i = 0; i < N; ++i)
        {
            this->mValues[i] /= s;
        }

        return *(PrimaryVectorBase< TYPE, N >*)this;
    }

    inline PrimaryVectorBase< TYPE, N > &operator *= (const TYPE s)
    {
        for (U32 i = 0; i < N; ++i)
        {
            this->mValues[i] *= s;
        }

        return *(PrimaryVectorBase< TYPE, N >*)this;
    }

    // Equality
    inline bool operator==(const PrimaryVectorBase< TYPE, N > &other) const
    {
        bool result = true;

        for (U32 i = 0; i < N; ++i)
        {
            result &= Mathf::Equal(this->mValues[i], other.mValues[i]);
        }

        return result;
    }

    inline bool operator!=(const PrimaryVectorBase< TYPE, N > &other) const
    {
        return !(PrimaryVectorBase< TYPE, N >(*this) == other);
    }

    // Internal intrinsics
    inline TYPE Dot(const PrimaryVectorBase< TYPE, N > &v) const
    {
        TYPE result = this->mValues[0] * v.mValues[0];

        for (U32 i = 1; i < N; ++i)
        {
            result += this->mValues[i] * v.mValues[i];
        }

        return  result;
    }

    inline TYPE Length2() const
    {
        return Dot(PrimaryVectorBase< TYPE, N >(*this));
    }

    inline TYPE Distance2(const PrimaryVectorBase< TYPE, N > &v) const
    {
        return (PrimaryVectorBase< TYPE, N >(*this) - v).Length2();
    }

    inline void SetValue(U32 axis, TYPE val)
    {
        this->mValues[axis] = val;
    }

    inline PrimaryVectorBase< TYPE, N > Absolute() const
    {
        PrimaryVectorBase< TYPE, N > newVec;

        for (U32 i = 0; i < N; ++i)
        {
            newVec.SetValue(i, Mathf::Abs(this->mValues[i]));
        }

        return newVec;
    }

    inline U8 MinAxis() const
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

    inline U8 MaxAxis() const
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

    inline void SetZero()
    {
        for (U32 i = 0; i < N; ++i)
        {
            SetValue(i, 0);
        }
    }

    inline bool IsZero() const
    {
        bool result = true;

        for (U32 i = 0; i < N; ++i)
        {
            result &= (this->mValues[i] == 0);
        }

        return result;
    }

    inline bool IsFuzzyZero() const
    {
        return Length2() < Mathf::GetEpsilon();
    }

    inline void Clear()
    {
        SetZero();
    }
};

template< class TYPE, U32 N >
inline PrimaryVectorBase< TYPE, N > operator+(const PrimaryVectorBase< TYPE, N > &v1, const PrimaryVectorBase< TYPE, N > &v2)
{
    PrimaryVectorBase< TYPE, N > newVec;

    for (U32 i = 0; i < PrimaryVectorBase< TYPE, N >::Width(); ++i)
    {
        newVec[i] = v1[i] + v2[i];
    }

    return newVec;
}

template< class TYPE, U32 N >
inline PrimaryVectorBase< TYPE, N > operator*(const PrimaryVectorBase< TYPE, N > &v1, const PrimaryVectorBase< TYPE, N > &v2)
{
    PrimaryVectorBase< TYPE, N > newVec;

    for (U32 i = 0; i < PrimaryVectorBase< TYPE, N >::Width(); ++i)
    {
        newVec[i] = v1[i] * v2[i];
    }

    return newVec;
}

template< class TYPE, U32 N >
inline PrimaryVectorBase< TYPE, N > operator-(const PrimaryVectorBase< TYPE, N > &v1, const PrimaryVectorBase< TYPE, N > &v2)
{
    PrimaryVectorBase< TYPE, N > newVec;

    for (U32 i = 0; i < PrimaryVectorBase< TYPE, N >::Width(); ++i)
    {
        newVec[i] = v1[i] - v2[i];
    }

    return newVec;
}

template< class TYPE, U32 N >
inline PrimaryVectorBase< TYPE, N > operator-(const PrimaryVectorBase< TYPE, N > &v)
{
    PrimaryVectorBase< TYPE, N > newVec;

    for (U32 i = 0; i < PrimaryVectorBase< TYPE, N >::Width(); ++i)
    {
        newVec[i] = -v[i];
    }

    return newVec;
}

template< class TYPE, U32 N >
inline PrimaryVectorBase< TYPE, N > operator*(const PrimaryVectorBase< TYPE, N > &v, const TYPE s)
{
    PrimaryVectorBase< TYPE, N > newVec;

    for (U32 i = 0; i < PrimaryVectorBase< TYPE, N >::Width(); ++i)
    {
        newVec[i] = v[i] * s;
    }

    return newVec;
}

template< class TYPE, U32 N >
inline PrimaryVectorBase< TYPE, N > operator*(TYPE s, const PrimaryVectorBase< TYPE, N > &v)
{
    return v * s;
}

template< class TYPE, U32 N >
inline PrimaryVectorBase< TYPE, N > operator/(const PrimaryVectorBase< TYPE, N > &v, TYPE s)
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
inline PrimaryVectorBase< TYPE, N > operator/(const PrimaryVectorBase< TYPE, N > &v1, const PrimaryVectorBase< TYPE, N > &v2)
{
    PrimaryVectorBase< TYPE, N > newVec;

    for (U32 i = 0; i < PrimaryVectorBase< TYPE, N >::Width(); ++i)
    {
        newVec[i] = v1[i] / v2[i];
    }

    return newVec;
}


#endif