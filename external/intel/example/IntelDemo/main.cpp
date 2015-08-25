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
#include "SoftwareOcclusionCulling.h"

void ParseCommandLine()
{
   LPTSTR commandLine = GetCommandLineW();
   int numArgs = 0;
   LPWSTR* argv = CommandLineToArgvW(commandLine, &numArgs);

   /*for(int i = 1; i < numArgs; i += 2) 
   {
      if(!_wcsicmp(argv[i], L"-slider1"))
      {
         gOccluderSizeThreshold = (float)_wtof(argv[i+1]);
      }
      if(!_wcsicmp(argv[i], L"-slider2"))
      {
         gOccludeeSizeThreshold = (float)_wtof(argv[i+1]);
      }
      if(!_wcsicmp(argv[i], L"-slider3"))
      {
         gDepthTestTasks = wcstoul(argv[i+1], NULL, 10);
      }
   }*/
}

// Application entry point.  Execution begins here.
//-----------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
   ParseCommandLine();

   // Prevent unused parameter compiler warnings
   UNREFERENCED_PARAMETER(hInstance);
   UNREFERENCED_PARAMETER(hPrevInstance);
   UNREFERENCED_PARAMETER(nCmdShow);

#ifdef DEBUG
   // tell VS to report leaks at any exit of the program
   _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
   CPUTResult result=CPUT_SUCCESS;
   int returnCode=0;

   // create an instance of my sample
   MySample *pSample = new MySample();
   
   // We make the assumption we are running from the executable's dir in
   // the CPUT SampleStart directory or it won't be able to use the relative paths to find the default
   // resources   
   cString ResourceDirectory;
   CPUTOSServices::GetOSServices()->GetExecutableDirectory(&ResourceDirectory);
   ResourceDirectory.append(_L("..\\..\\CPUT\\resources\\"));
   
   // Initialize the system and give it the base CPUT resource directory (location of GUI images/etc)
   // For now, we assume it's a relative directory from the executable directory.  Might make that resource
   // directory location an env variable/hardcoded later
   pSample->CPUTInitialize(ResourceDirectory); 

   // window and device parameters
   CPUTWindowCreationParams params;
   params.deviceParams.refreshRate = 60;
   params.deviceParams.swapChainBufferCount = 1;
   params.deviceParams.swapChainFormat    = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
   params.deviceParams.swapChainUsage     = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;

   // parse out the parameter settings or reset them to defaults if not specified
   cString AssetFilename;
   cString CommandLine(lpCmdLine);
   pSample->CPUTParseCommandLine(CommandLine, &params, &AssetFilename);

   // create the window and device context
   result = pSample->CPUTCreateWindowAndContext(_L("CPUTWindow DirectX 11"), params);
   ASSERT( CPUTSUCCESS(result), _L("CPUT Error creating window and context.") );
   
   // start the main message loop
   returnCode = pSample->CPUTMessageLoop();

    pSample->DeviceShutdown();

   // cleanup resources
   delete pSample;

   return returnCode;
}



