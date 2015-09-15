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
#ifndef __GENERIC_MATRIX_DATA_H__
#define __GENERIC_MATRIX_DATA_H__

#include "math/types.h"
#include "math/config.h"
#include "math/scalar/mathf.h"

#include "math/generic/primaryVectorBase.h"

#include <assert.h>

BEGIN_MATH_NAMESPACE

//
// Generic data construct
//
template< class TYPE, U32 ROWS, U32 COLS >
class GenericMatrixData
{
protected:
    PrimaryVectorBase< TYPE, COLS > mRows[ROWS];
};

template< class TYPE >
class GenericMatrixData< TYPE, 2, 2 >
{
public:

    union 
    {
        PrimaryVectorBase< TYPE, 2 > mRows[2];
        struct { TYPE _11, _12, 
                      _21, _22; };
    };
};

template< class TYPE >
class GenericMatrixData< TYPE, 3, 3 >
{
public:

    union 
    {
        PrimaryVectorBase< TYPE, 3 > mRows[3];
        struct { TYPE _11, _12, _13,
                      _21, _22, _23,
                      _31, _32, _33; };
    };
};

template< class TYPE >
class GenericMatrixData< TYPE, 4, 4 >
{
public:

    union 
    {
        PrimaryVectorBase< TYPE, 4 > mRows[3];
        struct { TYPE _11, _12, _13, _14,
                      _21, _22, _23, _24,
                      _31, _32, _33, _34,
                      _41, _42, _43, _44; };
    };
};

END_MATH_NAMESPACE

#endif