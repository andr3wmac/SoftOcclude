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

// Choose which depth buffer size to use
// If you change depth buffer size please do a rebuilt all 
#define DP_LARGE
//#define DP_MEDIUM
//#define DP_SMALL

extern float gOccluderSizeThreshold;
extern float gOccludeeSizeThreshold;
extern UINT  gDepthTestTasks;

#define PI 3.1415926535f

#if defined(DP_LARGE)
   const int SCREENW = 1280;
   const int SCREENH = 720;

   const int TILE_WIDTH_IN_PIXELS   = 160;
   const int TILE_HEIGHT_IN_PIXELS  = 192;
#elif defined(DP_MEDIUM)
   const int SCREENW = 640;
   const int SCREENH = 360;

   const int TILE_WIDTH_IN_PIXELS   = 80;
   const int TILE_HEIGHT_IN_PIXELS  = 96;
#elif defined(DP_SMALL)
   const int SCREENW = 320;
   const int SCREENH = 192;

   const int TILE_WIDTH_IN_PIXELS   = 40;
   const int TILE_HEIGHT_IN_PIXELS  = 48;
#endif


const int SCREENW_IN_TILES = (SCREENW + TILE_WIDTH_IN_PIXELS-1)/TILE_WIDTH_IN_PIXELS;
const int SCREENH_IN_TILES = (SCREENH + TILE_HEIGHT_IN_PIXELS-1)/TILE_HEIGHT_IN_PIXELS;

const int NUM_XFORMVERTS_TASKS = 16;

const int NUM_TILES = SCREENW_IN_TILES * SCREENH_IN_TILES;

// depending upon the scene the max #of tris in the bin should be changed.
const int MAX_TRIS_IN_BIN_MT = 1024 * 16;
const int MAX_TRIS_IN_BIN_ST = 1024 * 16;

const int YOFFSET1_ST = SCREENW_IN_TILES;
const int XOFFSET1_ST = 1;

const int YOFFSET2_ST = SCREENW_IN_TILES * MAX_TRIS_IN_BIN_ST;
const int XOFFSET2_ST = MAX_TRIS_IN_BIN_ST;

const int YOFFSET1_MT = SCREENW_IN_TILES;
const int XOFFSET1_MT = 1;
const int TOFFSET1_MT = SCREENW_IN_TILES * SCREENH_IN_TILES;

const int YOFFSET2_MT = SCREENW_IN_TILES * NUM_XFORMVERTS_TASKS * MAX_TRIS_IN_BIN_MT;
const int XOFFSET2_MT = NUM_XFORMVERTS_TASKS * MAX_TRIS_IN_BIN_MT;

const int SSE = 4;

const int AABB_VERTICES = 8;
const int AABB_INDICES  = 36;
const int AABB_TRIANGLES = 12;

const float4x4 viewportMatrix(
   0.5f*(float)SCREENW,             0.0f,  0.0f, 0.0f,
               0.0f, -0.5f*(float)SCREENH,  0.0f, 0.0f,
               0.0f,             0.0f,  1.0f, 0.0f,
   0.5f*(float)SCREENW,  0.5f*(float)SCREENH,  0.0f, 1.0f
);

const int OCCLUDER_SETS = 2;
const int OCCLUDEE_SETS = 4;

const int AVG_COUNTER = 10;

const int NUM_DT_TASKS = 50;

const int BLOCK_SIZE = 8;
#endif