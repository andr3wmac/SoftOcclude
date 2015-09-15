#pragma once
#ifndef __SSE41_VEC4IB_H__
#define __SSE41_VEC4IB_H__

#include "math/types.h"

#include "math/simd/vectorize.h"
#include "math/simd/generic/simdVectorBoolBase.h"

#include <smmintrin.h>
#include <limits>

class SSE41Vec4i_b : public SimdVectorBoolBase< SSE41Vec4i_b >
{
public:

    inline SSE41Vec4i_b(const __m128i &rhs) :
        mValue(rhs)
    {
    }

    inline SSE41Vec4i_b &operator=(const __m128i &rhs)
    {
        mValue = rhs;

        return *this;
    }

    inline operator __m128i() const
    {
        return mValue;
    }

private:

    __m128i mValue;
};

#endif