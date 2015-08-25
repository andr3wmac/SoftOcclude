//--------------------------------------------------------------------------------------
// Copyright 2013 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED "AS IS."
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.
//--------------------------------------------------------------------------------------

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "../common/SoftMath.h"

#define PI 3.1415926535f

const int NUM_XFORMVERTS_TASKS = 16;

// depending upon the scene the max #of tris in the bin should be changed.
const int MAX_TRIS_IN_BIN_MT = 1024 * 16;
const int MAX_TRIS_IN_BIN_ST = 1024 * 16;

const int SSE = 4;

const int AABB_VERTICES = 8;
const int AABB_INDICES  = 36;
const int AABB_TRIANGLES = 12;

const int OCCLUDER_SETS = 2;
const int OCCLUDEE_SETS = 4;

const int AVG_COUNTER = 10;

const int NUM_DT_TASKS = 50;

const int BLOCK_SIZE = 8;

struct RasterizerData
{
   int mScreenWidth;
   int mScreenHeight;
   int mTileWidthInPixels;
   int mTileHeightInPixels;
   int mScreenWidthInTiles;
   int mScreenHeightInTiles;
   int mNumTiles;

   int mXOffset1_ST;
   int mYOffset1_ST;
   int mXOffset2_ST;
   int mYOffset2_ST;

   int mXOffset1_MT;
   int mYOffset1_MT;
   int mTOffset1_MT;
   int mXOffset2_MT;
   int mYOffset2_MT;

   float4x4 mViewportMatrix;

   void setScreenSize(int width, int height)
   {
      mScreenWidth = width;
      mScreenHeight = height;

      mTileWidthInPixels = int(mScreenWidth / 8);
      mTileHeightInPixels = int(mScreenHeight / 3.75);

      mScreenWidthInTiles = (mScreenWidth + mTileWidthInPixels-1)/mTileWidthInPixels;
      mScreenHeightInTiles = (mScreenHeight + mTileHeightInPixels-1)/mTileHeightInPixels;

      mNumTiles = mScreenWidthInTiles * mScreenHeightInTiles;

      mYOffset1_ST = mScreenWidthInTiles;
      mXOffset1_ST = 1;
      mYOffset2_ST = mScreenWidthInTiles * MAX_TRIS_IN_BIN_ST;
      mXOffset2_ST = MAX_TRIS_IN_BIN_ST;

      mYOffset1_MT = mScreenWidthInTiles;
      mXOffset1_MT = 1;
      mTOffset1_MT = mScreenWidthInTiles * mScreenHeightInTiles;
      mYOffset2_MT = mScreenWidthInTiles * NUM_XFORMVERTS_TASKS * MAX_TRIS_IN_BIN_MT;
      mXOffset2_MT = NUM_XFORMVERTS_TASKS * MAX_TRIS_IN_BIN_MT;

      mViewportMatrix = float4x4(0.5f*(float)mScreenWidth, 0.0f,  0.0f, 0.0f,
                                             0.0f, -0.5f*(float)mScreenHeight,  0.0f, 0.0f,
                                             0.0f, 0.0f, 1.0f, 0.0f,
                                             0.5f*(float)mScreenWidth,  0.5f*(float)mScreenHeight, 0.0f, 1.0f);
   }
};

#endif