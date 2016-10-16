#ifndef UTILITY_DEF
#define UTILITY_DEF

#include "Utility\Utility_Debug.h"
#include "Utility\Utility_Graphics.h"
#include "Utility\Utility_Surface_Visualization.h"
#include "Utility\Utility_Transform.h"
#include "Utility\Utility_Surface.h"
/**
* Gets the region position in the predefined coordinate system. This should be called before region based data point operation.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	item			input data item
* @param	region			pointer to receive the region
* @return					operation status.
*/
GtsFx(kStatus) GdkSurface_GetRegionInCoordinate_3D(GdkInputItem item, GdkRegion3d64f *region);

#endif