#include "..\Def.h"
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
kChar debugLine[10240];

#define GDK_UTIL_TRACE

/**
* Print debug information in debug window
*
* @public					@memberof created by Chi Ho and Vincent
* @param	format			use it like printf
* @return					Operation status
*/
GtsFx(kStatus) GdkSurfaceGeneralizedHeight_Trace(const kChar* format, ...);

