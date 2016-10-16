#include "..\TypeDef.h"
#include "Utility_Transform.h"

/**
* Best fit method for a given groups of points in 2D.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	points			input pointer to an array containing x and y only.
* @param	count			number of points in points.
* @param	slope			slope for output.
* @param	intercept		intercept for output.
* @return					execution status.
*/
kStatus GdkUtilSurface_FitPoint64f_Iterative_2D(const kPoint64f* points, kSize count, k64f* slope, k64f* intercept);

/**
* Gets the intersection between the assigned straight line and the region then attach the returned array to an object
*
* @public					@memberof created by Chi Ho and Vincent
* @param	region			region for the points
* @param	slope			slope of the straight line
* @param	intercept		intercept of the straight line
* @param	x				x point for line description if sloe is invalid
* @param	allocator		kObject to be attached to
* @return					operation status.
*/
kArrayList GdkUtilSurface_GetLineIntersectionWithRegion_2D(GdkRegion3d64f* region, k64f slope, k64f intercept, k64f x, kAlloc allocator);

/**
* Check if point is in regionRef
*
* @public					@memberof created by Chi Ho and Vincent
* @param	regionRef		region
* @param	point			point
* @return					result
*/
kBool GdkUtilSurface_IsPointInRegion_3D(GdkRegion3d64f regionRef, kPoint3d64f point);

/**
* Transform the region information from mm to pixel
*
* @public					@memberof created by Chi Ho and Vincent
* @param	item			input data item
* @param	xStart			x start.
* @param	xEnd			x end.
* @param	yStart			y start.
* @param	yEnd			y end.
* @param	zStart			z start.
* @param	zEnd			z end.
* @param	width			width.
* @param	length			length.
* @param	height			height.
* @return					operation status.
*/
GtsFx(kStatus) GdkSurface_ConvertRegionFromCoordinateToPixel_3D(GdkInputItem item, GdkRegion3d64f* region, k32s* xStart, k32s* xEnd, k32s* yStart, k32s* yEnd, k32s* zStart, k32s* zEnd, k32u* width, k32u* length, k32u* height);

/**
* Distance from a point to a straight line, in XY plane.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	point			target point.
* @param	slope			slope for the line.
* @param	intercept		intercept for the line.
* @return					Distance to line.
*/
k64f GdkUtilSurface_DistanceToLine_2D(kPoint64f point, k64f* slope, k64f* intercept);

/**
* Check if point is in regionRef after transformation
*
* @public					@memberof created by Chi Ho and Vincent
* @param	regionRef		region
* @param	point			point
* @param	transform		transformation matrix
* @return					result
*/
kBool GdkUtilSurface_IsPointInRegionAfterTransform_3D(GdkRegion3d64f regionRef, kPoint3d64f point, kL3dTransform3d* transform);

/**
* Check if point is in regionRef after transformation
*
* @public						@memberof created by Chi Ho and Vincent
* @param	regionRef			large region
* @param	regionToTransform	small region to be transformed
* @param	transform			transformation matrix
* @return						result
*/
kBool GdkUtilSurface_IsRegionInRegionAfterTransform(GdkRegion3d64f regionRef, GdkRegion3d64f regionToTransform, kL3dTransform3d* transform);

/**
* Expand region by specified step
*
* @public						@memberof created by Chi Ho and Vincent
* @param	regionRef			target region
* @param	x					step in X
* @param	y					step in Y
* @param	z					step in Z
* @return						void
*/
void GdkUtilSurface_ExpandRegionByOffset(GdkRegion3d64f* regionRef, k64f x, k64f y, k64f z);

/**
* Expand region to the specified point
*
* @public						@memberof created by Chi Ho and Vincent
* @param	regionRef			target region
* @param	point				target point
* @return						void
*/
void GdkUtilSurface_ExpandRegionToPoint(GdkRegion3d64f* regionRef, kPoint3d64f point);

/**
* Convert rotated region back to original region
*
* @public						@memberof created by Chi Ho and Vincent
* @param	regionRef			target region
* @param	transform			transformation matrix
* @return						original region
*/
GdkRegion3d64f GdkUtilSurface_ConvertFromRelativeToAbsoluteRegion(GdkRegion3d64f regionRef, kL3dTransform3d* transform);

/**
* Convert region to rotated relative region
*
* @public						@memberof created by Chi Ho and Vincent
* @param	regionRef			target region
* @param	transform			transformation matrix
* @param	transformInv		inverse transformation matrix
* @param	angle				angle of rotation
* @param	result				roated new region
* @return						operation status
*/
kBool GdkUtilSurface_ConvertFromAbsoluteToRelativeRegion(GdkRegion3d64f regionRef, kL3dTransform3d* transform, kL3dTransform3d* transformInv, k64f angle, GdkRegion3d64f* result);






