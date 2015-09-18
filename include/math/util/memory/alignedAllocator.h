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
#ifndef __ALIGNED_ALLOCATOR_H__
#define __ALIGNED_ALLOCATOR_H__

#include <stdlib.h>
#include <memory>

#include "math/types.h"
#include "math/config.h"

#include "math/util/memory/malloc.h"

    template< class T, U32 N >
    class AlignedAllocator
    {
    public:

        AlignedAllocator() throw()
        {}
        AlignedAllocator( const AlignedAllocator & ) throw()
        {}
        ~AlignedAllocator() throw()
        {}
        template <class U>
        FORCE_INLINE AlignedAllocator( const AlignedAllocator<U, N> & ) throw()
        {}

        typedef T value_type;
        typedef T &reference;
        typedef const T &const_reference;
        typedef T *pointer;
        typedef const T *const_pointer;
        typedef size_t size_type;

        template < class Y >
        struct rebind
        {
            typedef AlignedAllocator< Y, N > other;
        };

        FORCE_INLINE pointer addres( reference ref )
        {
            return &ref;
        }

        FORCE_INLINE const_pointer addres( const_reference ref )
        {
            return &ref;
        }

        FORCE_INLINE pointer allocate( size_type n, typename std::allocator<void>::const_pointer hint = 0 )
        {
            pointer address = ( pointer )( ZefAlignedMalloc( sizeof( T ) * n, N ) );

            if ( address == nullptr )
            {
                throw std::bad_alloc();
            }

            return address;
        }

        FORCE_INLINE void deallocate( pointer ptr, size_type )
        {
            ZefAlignedFree( ptr );
        }

        FORCE_INLINE void construct( pointer p, const_reference val )
        {
            new( p ) value_type( val );
        }

        FORCE_INLINE void destroy( pointer p )
        {

            p->~value_type();
        }

        FORCE_INLINE size_type max_size() const
        {
            return ~size_type( 0 ) / sizeof( T );
        }

        //avoid errors
        FORCE_INLINE bool operator==( AlignedAllocator const & ) { return true; }
        FORCE_INLINE bool operator!=( AlignedAllocator const &a ) { return !operator==( a ); }
    };

#endif