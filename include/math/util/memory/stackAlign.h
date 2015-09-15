#pragma once

#if _WIN32 

#define StackAlign( x ) __declspec(align( x ))

#else 

#define StackAlign( x ) __attribute__ ((aligned ( x )))

#endif
