#pragma once
#ifndef __ZEF_MALLOC_H__
#define __ZEF_MALLOC_H__

#include <cstddef>

#include <stdlib.h>
#include <immintrin.h>

inline void *_InternalAlignedMalloc(size_t bytes, size_t alignment)
{
    size_t offset = alignment; //+ sizeof( size_t );

    void *ptr = malloc(bytes + offset);

    if (!ptr)
    {
        return nullptr;
    }

    //offset and truncate under alignment
    void **ptr2 = (void **)(((size_t)(ptr)& ~(alignment - 1)) + offset);

    //store malloc address above the requested memory
    ptr2[-1] = ptr;

    return ptr2;
}

inline void _InternalAlignedFree(void *ptr)
{
    if (ptr != 0)
    {
        free(*((void **)(ptr)-1));
    }
}

inline void *ZefAlignedMalloc(size_t bytes, size_t alignment)
{
#if INTERNAL_ALIGNED_MALLOC 
    return _InternalAlignedMalloc(bytes, alignment);
#else
    return _mm_malloc(bytes, alignment);
#endif
}

inline void ZefAlignedFree(void *ptr)
{
#if INTERNAL_ALIGNED_MALLOC 
    return _InternalAlignedFree(ptr);
#else
    return _mm_free(ptr);
#endif
}

#endif