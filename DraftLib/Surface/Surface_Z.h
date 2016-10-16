#include "..\TypeDef.h"

/*
* To create ref plane:
* 1. get points from region. If there is rotation, rotate first - 
* 2. fit plane from the points; - GdkSurface_ComputePlaneFromPoints()
* 3. calculate point distance of each data in item, then the whole data item is corrected based on the ref plane; - GdkSurface_SignedDistance()
*/

/**
* Convert plane normal to a, b, c, d.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	plane			surface plane
* @return					Height plane in a, b, c, d mode.
*/
GdkSurface_HeightPlane GdkSurface_ToPlane3d(kPlane3d64f plane);

/**
* Signed distance from target point to surface plane
*
* @public					@memberof created by Chi Ho and Vincent
* @param	plane			surface plane
* @param	point			target point
* @return					signed distance
*/
k64f GdkSurface_SignedDistance(GdkSurface_HeightPlane plane, kPoint3d64f point);

/**
* Transform the region information from mm to pixel. This function doesn't if (x,y) is out of bounds.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	item			input data item
* @param	point			target point with (x,y).
* @return					operation status.
*/
kStatus GdkSurface_GetPointZ(GdkInputItem item, kPoint3d64f* point);

/**
* Transform the region information from mm to pixel
*
* @public					@memberof created by Chi Ho and Vincent
* @param	item			input data item
* @param	point			target point with (x,y).
* @return					operation status.
*/
kStatus GdkSurface_GetPointZ_Safe(GdkInputItem item, kPoint3d64f* point);

/**
* Find outliers that is outside the threshold
*
* @public					@memberof created by Chi Ho and Vincent
* @param	lowerTolerance	lower tolerance
* @param	upperTolerance	upper tolerance
* @param	inputPoints		input array list
* @param	outliers		array list for output
* @param	tool			GdkTool object to attach array list to
* @return					operation status.
*/
kStatus GdkSurface_FindOutliers(k64f lowerTolerance, k64f upperTolerance, kArrayList inputPoints, kArrayList *outliers, GdkTool tool);