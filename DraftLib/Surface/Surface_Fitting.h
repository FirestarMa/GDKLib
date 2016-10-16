#include "..\TypeDef.h"
#include "Surface_Z.h"

/**
* Get the dot product of 2 points
*
* @public					@memberof created by Chi Ho and Vincent
* @param	pointList		data source
* @param	center			preliminary center, use center of gravity or center of region
* @param	centerOut		fitted center
* @param	radiusOut		fitted radius
* @return					operation status
*/
GtsFx(kStatus) GdkSurface_FitSphere(kArrayList pointList, kPoint3d64f* center, kPoint3d64f* centerOut, k64f* radiusOut);

/**
* Get the fit error
*
* @public					@memberof created by Chi Ho and Vincent
* @param	pointList		data source
* @param	center			fitted center
* @param	radiusOut		fitted radius
* @param	stdev			standard deviation for output
* @return					operation status
*/
GtsFx(kStatus) GdkSurface_ComputeSphereStdDev(kArrayList pointList, kPoint3d64f* center, k64f radius, k64f* stdev);

/**
* Signed distance from target point to surface plane
*
* @public					@memberof created by Chi Ho and Vincent
* @param	pointList		points to fit the plane
* @param	planeOut		fitted plane{p, norm}
* @param	averageHeight	average height of the points to original coordiante
* @return					signed distance
*/
kStatus GdkSurface_FitPlane(kArrayList pointList, kPlane3d64f* planeOut, k64f *averageHeight);

/**
* Compute the distance from pointList to the plane.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	plane			surface plane
* @param	pointList		points to be calculated
* @param	averageHeight	mean height among points in pointList
* @param	minHeight		minimum height among points in pointList
* @param	maxHeight		maximum height among points in pointList
* @return					operation status.
*/
kStatus GdkSurface_ComputeHeightFromPlane_3D(kPlane3d64f* plane, kArrayList pointList, k64f *averageHeight, k64f *medianHeight, k64f *minHeight, k64f *maxHeight, k64f *stdev);

/**
* Compute the distance from pointList to the plane.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	array			data source
* @param	firstIndex		start position
* @param	lastIndex		end position
* @return					void
*/
void GdkSurface_Quicksort(k64f *array, int firstIndex, int lastIndex);

/**
* Compute the angle bewteen 2 vectors
*
* @public					@memberof created by Chi Ho and Vincent
* @param	cx				start vector x
* @param	cy				start vector y
* @param	ex				end vector x
* @param	ey				end vector y
* @return					angle in degrees, ranging [0, 360]
*/
k64f GdkSurface_ComputeVectorAngle_2D(k64f cx, k64f cy, k64f ex, k64f ey);