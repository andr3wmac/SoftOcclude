#pragma once
#ifndef __ALIGNED_ALLOCATOR_H__
#define __ALIGNED_ALLOCATOR_H__

#include <stdlib.h>
#include <memory>

#include "math/types.h"

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
        inline AlignedAllocator( const AlignedAllocator<U, N> & ) throw()
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

        inline pointer addres( reference ref )
        {
            return &ref;
        }

        inline const_pointer addres( const_reference ref )
        {
            return &ref;
        }

        inline pointer allocate( size_type n, typename std::allocator<void>::const_pointer hint = 0 )
        {
            pointer address = ( pointer )( ZefAlignedMalloc( sizeof( T ) * n, N ) );

            if ( address == nullptr )
            {
                throw std::bad_alloc();
            }

            return address;
        }

        inline void deallocate( pointer ptr, size_type )
        {
            ZefAlignedFree( ptr );
        }

        inline void construct( pointer p, const_reference val )
        {
            new( p ) value_type( val );
        }

        inline void destroy( pointer p )
        {

            p->~value_type();
        }

        inline size_type max_size() const
        {
            return ~size_type( 0 ) / sizeof( T );
        }

        //avoid errors
        inline bool operator==( AlignedAllocator const & ) { return true; }
        inline bool operator!=( AlignedAllocator const &a ) { return !operator==( a ); }
    };

#endif