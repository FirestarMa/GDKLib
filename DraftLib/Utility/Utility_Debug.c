#include "Utility_Debug.h"


/**
* Print debug information in debug window
*
* @public					@memberof created by Chi Ho and Vincent
* @param	format			use it like printf
* @return					Operation status
*/
GtsFx(kStatus) GdkSurfaceGeneralizedHeight_Trace(const kChar* format, ...)
{
	kStatus status = kOK;

#ifdef GDK_UTIL_TRACE
	kVarArgList argList;
	kVarArgList_Start_(argList, format);
	{
		status = kStrPrintvf(debugLine, 10240, format, argList);
	}
	kVarArgList_End_(argList);
	OutputDebugStringA(debugLine);
#endif

	return status;
}