/**
* @cond ___LICENSE___
*
* Copyright (c) 2015 Koen Visscher, Paul Visscher and individual contributors.
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
#ifndef __ENGINE_TYPES_H__
#define __ENGINE_TYPES_H__

/// @addtogroup Types
/// @{

#include <stdint.h>

/**
* A signed 8 bit integer.
*
* Range:
*      @f$ 0 \le x \le 255 @f$
*/

typedef int8_t S8;

/**
* An unsigned 8 bit integer.
*
* Range:
*      @f$ -128 \le x \le 127 @f$
*/

typedef uint8_t U8;

/**
* A signed 16 bit integer.
*
* Range:
*      @f$ 0 \le x \le 65,535 @f$
*/

typedef int16_t S16;

/**
* An unsigned 16 bit integer.
*
* Range:
*      @f$ -32,768 \le x \le 32,767 @f$
*/

typedef uint16_t U16;

/**
* A signed 32 bit integer.
*
* Range:
*      @f$ -2,147,483,648 \le x \le 2,147,483,647 @f$.
*/

typedef int32_t S32;

/**
* An unsigned 32 bit integer.
*
* Range:
*       @f$ 0 \le x \le 4,294,967,295 @f$.
*/

typedef uint32_t U32;

/**
* A signed 64 bit integer.
*
* Range:
*      @f$ -9,223,372,036,854,775,808 \le x \le 9,223,372,036,854,775,807 @f$.
*/

typedef int64_t S64;

/**
* An unsigned 64 bit integer.
*
* Range:
*      @f$ 0 \le x \le 18,446,744,073,709,551,615 @f$.
*/

typedef uint64_t U64;

/**
* A 32 bit floating point number.
*
* Range:
*      3.4E @f$ \pm @f$ 38 (7 digits)
*/

typedef float F32;

/**
* A 64 bit floating point number.
*
* Range:
*      1.7E @f$ \pm @f$ 308 (15 digits)
*/

typedef double F64;

/// @}

#endif