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