#pragma once
#ifndef __DEPTH_BUFFER_H__
#define __DEPTH_BUFFER_H__

#include "core/common/types.h"

#include "math/util/memory/alignedAllocator.h"

#include <vector>
#include <algorithm>
#include <assert.h>

struct DepthBufferTile
{
    U32 tileSizeX;
    U32 tileSizeY;
    S32 xStart;
    S32 xEnd;
    S32 yStart;
    S32 yEnd;

    DepthBufferTile( U32 tilex, U32 tiley,
                     S32 sx, S32 ex,
                     S32 sy, S32 ey ) :
        tileSizeX( tilex ), tileSizeY( tiley ),
        xStart( sx ), xEnd( ex ),
        yStart( sy ), yEnd( ey )
    {}
};

class SimdDepthBuffer
{
public:

    SimdDepthBuffer( U32 width, U32 height ) : 
        mWidth( width ), mHeight( height )
    {
        mDepthSequence.resize(width * height);
    }

    void Clear()
    {
        std::fill(mDepthSequence.begin(), mDepthSequence.end(), 0);
    }

    inline F32 * GetDepthSequence(U32 offset)
    {
        assert(offset < mDepthSequence.size());
        assert((offset % 4) == 0);

        return &mDepthSequence[0] + offset;
    }

    void SetTilingPatern(U32 tileX, U32 tileY)
    {
        mTileX = tileX;
        mTileY = tileY;
    }

    U32 GetWidth() const
    {
        return mWidth;
    }

    U32 GetHeight() const
    {
        return mHeight;
    }

    U32 GetTileX() const
    {
        return mTileX;
    }

    U32 GetTileY() const
    {
        return mTileY;
    }

private:

    U32 mTileX;
    U32 mTileY;
    U32 mWidth;
    U32 mHeight;

    //Align to cache line
    std::vector< F32, AlignedAllocator< F32, 32 > > mDepthSequence;
};

#endif