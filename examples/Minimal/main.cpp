//-----------------------------------------------------------------------------
// Copyright (c) 2015 Andrew Mac
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include <iostream>
#include "SoftOcclude.h"

// Camera Data
float4x4 viewMatrix(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);


float4x4 projMatrix(1.73205090f, 0.0f, 0.0f, 0.0f,
	                 0.0f, 3.07920146f, 0.0f, 0.0f,
	                 0.0f, 0.0f, 0.000500250142f, 1.0f,
	                 0.0f, 0.0f, 1.00050032f, 0.0f);

float fov = 1.04719758f;

// Model Data
Vertex cubeVerts[4] = {Vertex(-1, -1, 1),
                                    Vertex( 1, -1, 1),
                                    Vertex(-1,  1, 1),
                                    Vertex( 1,  1, 1)};

UINT cubeIndices[6] = {2, 1, 0, 1, 2, 3};

int main()
{
   float width = 640.0f;
   float height = 360.0f;

   SoftOcclusionTest* mOcclusionTest = new SoftOcclusionTest(width, height);

   SoftFrustum frustum;
   frustum.InitializeFrustum(1, 1000, width / height, fov, float3(0, 0, 0), float3(0, 0, 1), float3(0, 1, 0));

   float4x4 viewPortMatrix(width * 0.5f, 0.0f, 0.0f, 0.0f,
	                        0.0f, height * -0.5f, 0.0f, 0.0f,
	                        0.0f, 0.0f, 1.0f, 0.0f,
	                        width * 0.5f, height * 0.5f, 0.0f, 1.0f);

   // Add a 2x2x2 cube at (0, 0, 10) as an occluder.
   SoftOccluderScalar* occluder = mOcclusionTest->AddOccluder();

   occluder->AddMesh(cubeVerts, 4, cubeIndices, 6, 2);
   float4x4 cubeWorldMatrix(1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 10.0f, 1.0f);

   occluder->SetWorldMatrix(&cubeWorldMatrix);
   occluder->SetBoundsObjectSpace(float3(0, 0, 0), float3(2, 2, 2));
   occluder->SetBoundsWorldSpace(float3(0, 0, 10.0f), float3(2, 2, 2));

   // Perform this whenever occluders are dirty.
   mOcclusionTest->RefreshOccluders();

   // Add a 2x2x2 AABB at (0, 0, 20) as an occludee
   SoftOccludeeScalar* occludee = mOcclusionTest->AddOccludee();
   float4x4 occludeeWorldMatrix(1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f,
                                0.0f, 0.0f, 20.0f, 1.0f);

   occludee->SetWorldMatrix(&occludeeWorldMatrix);
   occludee->SetBoundsObjectSpace(float3(0, 0, 0), float3(1, 1, 1));
   occludee->SetBoundsWorldSpace(float3(0, 0, 20.0f), float3(1, 1, 1));

   // Perform the rendering/testing.
   mOcclusionTest->ResetInsideFrustum();
   mOcclusionTest->SetEnableFrustrumCulling(true);
   mOcclusionTest->Render(&viewMatrix, &projMatrix, &frustum);

   // Check if our occludee is visible or not.
   if ( !mOcclusionTest->IsOccludeeVisible(0) )
      std::cout << "Occludee is not visible!" << std::endl;

   std::cout << "Render complete. Press enter to output depth buffer." << std::endl;
   std::cin.ignore();

   // Save a bmp copy of depth buffer.
   mOcclusionTest->SaveDepthBuffer("depth_buffer.bmp");

   // Clean up
   delete mOcclusionTest;
}