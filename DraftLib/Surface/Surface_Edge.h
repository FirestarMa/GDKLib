#include "..\TypeDef.h"

#include "..\GDKUtility.h"

/*
Edge detection flow:
1. get edge points in specified region -- GdkSurface_GetEdgePoints_2D()
      - may have to manipulate edge points a bit;
2. Fit line -- GdkUtilSurface_FitPoint64f_Iterative_2D()
3. get the intercept between the full region and the fitted line for visualization -- GdkUtilSurface_GetLinePoints_2D()
*/



/**
* Get valid edge points in input region. If there are multiple edge regions, the edge points are combined.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	tool			tool object.
* @param	item			input surface data.
* @param	region			region for edge point detection.
* @param	settings		edge detection settings in tool panel.
* @param	edgePoints		kArrayList to receive detected edge points
* @return					Operation status.
*/
GtsFx(kStatus) GdkSurface_GetEdgePoints_2D(GdkInputItem item, GdkRegion3d64f* region, GdkSurface_EdgeSettings *settings, kArrayList edgePoints);

/**
* Check if the edge point located is the one we need.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	filter			filter type as specified in tool panel.
* @param	edgeStrength	edgeStrength at current x.
* @param	curEdgeStrength	current recognized edge strength.
* @return					operation status.
*/
static kBool GdkSurface_CalculateEdgeFilter(GdkSurface_EdgeFilter filter, k64f edgeStrength, k64f curEdgeStrength);

/**
* Calculate edge strength value based on intensity data for a kernel at x.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	data			pointer to the position in array.
* @param	kernelSize		range for edge detection (kernel size).
* @param	direction		search direction.
* @return					edge strength.
*/
static k64f GdkSurface_CalculateIntensityEdgeStrength(const k8u *data, k32s kernelSize, GdkSurface_EdgeType direction);

/**
* Calculate edge strength value based on height map for a kernel at x.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	data			pointer to the position in array.
* @param	kernelSize		range for edge detection (kernel size).
* @param	direction		search direction.
* @param	zRes			z resolution.
* @param	offset			z offset.
* @return					edge strength.
*/
static k64f GdkSurface_CalculateHeightEdgeStrength(const k16s *data, k32s kernelSize, GdkSurface_EdgeType direction, k64f zRes, k64f offset);

