#include "Utility_Surface.h"

/**
* Distance from a point to a straight line, in XY plane.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	point			target point.
* @param	slope			slope for the line.
* @param	intercept		intercept for the line.
* @return					Distance to line.
*/
k64f GdkUtilSurface_DistanceToLine_2D(kPoint64f point, k64f* slope, k64f* intercept)
{
	// y=Sx+I -> Sx + (-1)y + I = 0
	// |ax0 + by0 + c| / sqrt(a2 + b2) = d

	k64f a = *slope;
	k64f b = -1;
	k64f c = *intercept;
	k64f d = sqrt(a*a + 1);

	k64f distance = fabs(point.x * a + point.y * b + c) / d;
	return distance;
}
/**
* Best fit method for a given groups of points in 2D. Iteration until only 1/3 points left for fitting.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	points			input pointer to an array containing x and y only.
* @param	count			number of points in points.
* @param	slope			slope for output.
* @param	intercept		intercept for output.
* @return					execution status.
*/
kStatus GdkUtilSurface_FitPoint64f_Iterative_2D(const kPoint64f* points, kSize count, k64f* slope, k64f* intercept)
{
	kStatus result = kOK;
	kSize curCount = count;
	kArrayList pointList = kNULL;

	k64f maxD = -k64F_MAX;
	kSize i = 0;
	kSize index = 0;

	kArrayList_Construct(&pointList, kTypeOf(kPoint64f), count, kNULL);
	kArrayList_Import(pointList, points, kTypeOf(kPoint64f), count);

	// Fit line, x further points are removed, then fit line again.
	// Do this until we reach 1/3 of the points

	//// fit line=> find point that is the farthest to the line=> remove it=> refit the line   =====> until there are less than 33%*total count (at least 3) in pointList
	while (curCount >= 3 && curCount >= count / 3)
	{
		kPoint64f* curPoints = (kPoint64f*)kArrayList_Data(pointList);
		curCount = kArrayList_Count(pointList);
		//// use built in line fit fuction
		result = kVsLine2d_FitPoint64f(curPoints, (k32u)curCount, slope, intercept);

		maxD = -k64F_MAX;
		index = 0;
		//// loop over every point and find the maximum distance to fitted line.
		for (i = 0; i < curCount; ++i)
		{
			k64f d = GdkUtilSurface_DistanceToLine_2D(curPoints[i], slope, intercept);
			if (d > maxD)
			{
				index = i;
				maxD = d;
			}
		}
		//// remove this point from the source points
		kArrayList_Remove(pointList, index, kNULL);
	}

	kDestroyRef(&pointList);

	return result;
}

/**
* Gets the points along the assigned straight line and attach the returned array to an object
*
* @public					@memberof created by Chi Ho and Vincent
* @param	region			region for the points
* @param	slope			slope of the straight line
* @param	intercept		intercept of the straight line
* @param	x				x point for line description if sloe is invalid
* @param	allocator		kObject to be attached to
* @return					operation status.
*/
kArrayList GdkUtilSurface_GdkUtilSurface_GetLineIntersectionWithRegion_2D(GdkRegion3d64f* region, k64f slope, k64f intercept, k64f x, kAlloc allocator)
{
	kArrayList pointListOut = kNULL;
	kPoint3d32f point32_1;
	kPoint3d32f point32_2;

	kArrayList_Construct(&pointListOut, kTypeOf(kPoint3d32f), 2, allocator);
	point32_1.z = 0;
	point32_2.z = 0;

	// If the slope is invalid, use the x value for position.
	if (slope == k64F_NULL)
	{
		point32_1.x = (k32f)x;
		point32_2.x = (k32f)x;
		point32_1.y = (k32f)(region->y);
		point32_2.y = (k32f)(region->y + region->length);
	}
	else
	{
		point32_1.x = (k32f)region->x;
		point32_1.y = (k32f)(slope * point32_1.x + intercept);
		point32_2.x = (k32f)(region->x + region->width);
		point32_2.y = (k32f)(slope * point32_2.x + intercept);
		if ((point32_1.y < region->y || point32_1.y > region->y + region->length) ||
			(point32_2.y < region->y || point32_2.y > region->y + region->length))
		{
			point32_1.x = (k32f)((region->y - intercept) / slope);
			point32_1.y = (k32f)(region->y);
			point32_2.x = (k32f)((region->y + region->length - intercept) / slope);
			point32_2.y = (k32f)(region->y + region->length);
		}
	}

	kArrayList_Add(pointListOut, &point32_1);
	kArrayList_Add(pointListOut, &point32_2);

	return pointListOut;
}

/**
* Check if point is in regionRef
*
* @public					@memberof created by Chi Ho and Vincent
* @param	regionRef		region 
* @param	point			point
* @return					result
*/
kBool GdkUtilSurface_IsPointInRegion_3D(GdkRegion3d64f regionRef, kPoint3d64f point)
{
	if (point.x >= regionRef.x && point.x <= regionRef.x + regionRef.width &&
		point.y >= regionRef.y && point.y <= regionRef.y + regionRef.length &&
		point.z >= regionRef.z && point.z <= regionRef.z + regionRef.height)
		return kTRUE;

	return kFALSE;
}

/**
* Check if point is in regionRef after transformation
*
* @public					@memberof created by Chi Ho and Vincent
* @param	regionRef		region
* @param	point			point
* @param	transform		transformation matrix
* @return					result
*/
kBool GdkUtilSurface_IsPointInRegionAfterTransform_3D(GdkRegion3d64f regionRef, kPoint3d64f point, kL3dTransform3d* transform)
{
	kPoint3d64f pointOut;
	kL3dTransform3d_Apply(transform, point.z, point.y, point.z, &pointOut.x, &pointOut.y, &pointOut.z);

	return GdkUtilSurface_IsPointInRegion_3D(regionRef, pointOut);
}

/**
* Check if point is in regionRef after transformation
*
* @public						@memberof created by Chi Ho and Vincent
* @param	regionRef			large region
* @param	regionToTransform	small region to be transformed
* @param	transform			transformation matrix
* @return						result
*/
kBool GdkUtilSurface_IsRegionInRegionAfterTransform(GdkRegion3d64f regionRef, GdkRegion3d64f regionToTransform, kL3dTransform3d* transform)
{
	kPoint3d64f point;
	kBool res = kTRUE;

	// For all 8 points of the cube.
	kPoint3d_Init_(&point, regionToTransform.x + regionToTransform.width, regionToTransform.y + regionToTransform.length, regionToTransform.z + regionToTransform.height);
	res &= GdkUtilSurface_IsPointInRegionAfterTransform_3D(regionRef, point, transform);
	kPoint3d_Init_(&point, regionToTransform.x + regionToTransform.width, regionToTransform.y + regionToTransform.length, regionToTransform.z - regionToTransform.height);
	res &= GdkUtilSurface_IsPointInRegionAfterTransform_3D(regionRef, point, transform);
	kPoint3d_Init_(&point, regionToTransform.x + regionToTransform.width, regionToTransform.y - regionToTransform.length, regionToTransform.z + regionToTransform.height);
	res &= GdkUtilSurface_IsPointInRegionAfterTransform_3D(regionRef, point, transform);
	kPoint3d_Init_(&point, regionToTransform.x + regionToTransform.width, regionToTransform.y - regionToTransform.length, regionToTransform.z - regionToTransform.height);
	res &= GdkUtilSurface_IsPointInRegionAfterTransform_3D(regionRef, point, transform);
	kPoint3d_Init_(&point, regionToTransform.x - regionToTransform.width, regionToTransform.y + regionToTransform.length, regionToTransform.z + regionToTransform.height);
	res &= GdkUtilSurface_IsPointInRegionAfterTransform_3D(regionRef, point, transform);
	kPoint3d_Init_(&point, regionToTransform.x - regionToTransform.width, regionToTransform.y + regionToTransform.length, regionToTransform.z - regionToTransform.height);
	res &= GdkUtilSurface_IsPointInRegionAfterTransform_3D(regionRef, point, transform);
	kPoint3d_Init_(&point, regionToTransform.x - regionToTransform.width, regionToTransform.y - regionToTransform.length, regionToTransform.z + regionToTransform.height);
	res &= GdkUtilSurface_IsPointInRegionAfterTransform_3D(regionRef, point, transform);
	kPoint3d_Init_(&point, regionToTransform.x - regionToTransform.width, regionToTransform.y - regionToTransform.length, regionToTransform.z - regionToTransform.height);
	res &= GdkUtilSurface_IsPointInRegionAfterTransform_3D(regionRef, point, transform);

	return res;
}

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
void GdkUtilSurface_ExpandRegionByOffset(GdkRegion3d64f* regionRef, k64f x, k64f y, k64f z)
{
	regionRef->x -= x / 2.0;
	regionRef->y -= y / 2.0;
	regionRef->z -= z / 2.0;
	regionRef->width += x;
	regionRef->length += y;
	regionRef->height += z;
}

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
kBool GdkUtilSurface_ConvertFromAbsoluteToRelativeRegion(GdkRegion3d64f regionRef, kL3dTransform3d* transform, kL3dTransform3d* transformInv, k64f angle, GdkRegion3d64f* result)
{
	kArrayList pointListOut = kNULL;
	kPoint3d64f centerFinal;
	kPoint3d64f center;
	k64f angleTransform;

	k64f ca, sa;
	k64f width;
	k64f length;
	kBool res = kFALSE;
	kBool roiOK = kFALSE;

	// Absolute region is orthogonal with axes, it has to be shrunk down
	// x = (1 / (cos(t) ^ 2 - sin(t) ^ 2)) * (bx * cos(t) - by * sin(t))
	// y = (1 / (cos(t) ^ 2 - sin(t) ^ 2)) * (-bx * sin(t) + by * cos(t))

	// Compute the center, it is the same as the rotated rectangle.
	// transform center and use x and y as width and length of the rectangle
	angleTransform = angle < 0 ? -angle : angle;

	ca = cos(kMath_DegToRad_(angleTransform));
	sa = sin(kMath_DegToRad_(angleTransform));

	width = 1.0 / (ca*ca - sa*sa) * (regionRef.width*ca - regionRef.length*sa);
	length = 1.0 / (ca*ca - sa*sa) * (-regionRef.width*sa + regionRef.length*ca);
	kPoint3d_Init_(&center, regionRef.x + regionRef.width / 2.0, regionRef.y + regionRef.length / 2.0, regionRef.z + regionRef.height / 2.0);
	kL3dTransform3d_Apply(transform, center.x, center.y, center.z, &centerFinal.x, &centerFinal.y, &centerFinal.z);

	result->height = regionRef.height;
	result->z = centerFinal.z - regionRef.height / 2.0;

	if ((width < 0 && length > 0) ||
		(width > 0 && length < 0))
	{
		// Unable to compute, expand a region to the limits of the Absolute region, starting at half the size, good even for 45deg
		result->width = regionRef.width / 2.0;
		result->length = regionRef.length / 2.0;
		result->x = centerFinal.x - result->width / 2.0;
		result->y = centerFinal.y - result->length / 2.0;

		do {
			GdkUtilSurface_ExpandRegionByOffset(result, 0, REGION_EXPANSION_INCREMENT, 0);
			res = GdkUtilSurface_IsRegionInRegionAfterTransform(regionRef, *result, transformInv);
		} while (res == kTRUE);
		GdkUtilSurface_ExpandRegionByOffset(result, 0, -REGION_EXPANSION_INCREMENT, 0);

		do {
			GdkUtilSurface_ExpandRegionByOffset(result, REGION_EXPANSION_INCREMENT, 0, 0);
			res = GdkUtilSurface_IsRegionInRegionAfterTransform(regionRef, *result, transformInv);
		} while (res == kTRUE);
		GdkUtilSurface_ExpandRegionByOffset(result, -REGION_EXPANSION_INCREMENT, 0, 0);
	}
	else
	{
		result->x = centerFinal.x - width / 2.0;
		result->y = centerFinal.y - length / 2.0;
		result->width = width;
		result->length = length;
	}

	return roiOK;
}

/**
* Expand region to the specified point
*
* @public						@memberof created by Chi Ho and Vincent
* @param	regionRef			target region
* @param	point				target point
* @return						void
*/
void GdkUtilSurface_ExpandRegionToPoint(GdkRegion3d64f* regionRef, kPoint3d64f point)
{
	if (point.x > regionRef->x + regionRef->width)
	{
		regionRef->width = point.x - regionRef->x;
	}
	if (point.x < regionRef->x)
	{
		regionRef->width = regionRef->width + (regionRef->x - point.x);
		regionRef->x = point.x;
	}

	if (point.y > regionRef->y + regionRef->length)
	{
		regionRef->length = point.y - regionRef->y;
	}
	if (point.y < regionRef->y)
	{
		regionRef->length = regionRef->length + (regionRef->y - point.y);
		regionRef->y = point.y;
	}

	if (point.z > regionRef->z + regionRef->height)
	{
		regionRef->height = point.z - regionRef->z;
	}
	if (point.z < regionRef->z)
	{
		regionRef->height = regionRef->height + (regionRef->z - point.z);
		regionRef->z = point.z;
	}
}

/**
* Convert rotated region back to original region
*
* @public						@memberof created by Chi Ho and Vincent
* @param	regionRef			target region
* @param	transform			transformation matrix
* @return						original region
*/
GdkRegion3d64f GdkUtilSurface_ConvertFromRelativeToAbsoluteRegion(GdkRegion3d64f regionRef, kL3dTransform3d* transform)
{
	GdkRegion3d64f result = { 0, 0, 0, 0, 0, 0 };
	kArrayList pointListOut = kNULL;
	kPoint3d64f pointFinal[8];
	kSize i = 0;

	kL3dTransform3d_Apply(transform, regionRef.x, regionRef.y, regionRef.z, &pointFinal[0].x, &pointFinal[0].y, &pointFinal[0].z);
	kL3dTransform3d_Apply(transform, regionRef.x + regionRef.width, regionRef.y, regionRef.z, &pointFinal[1].x, &pointFinal[1].y, &pointFinal[1].z);
	kL3dTransform3d_Apply(transform, regionRef.x + regionRef.width, regionRef.y + regionRef.length, regionRef.z, &pointFinal[2].x, &pointFinal[2].y, &pointFinal[2].z);
	kL3dTransform3d_Apply(transform, regionRef.x, regionRef.y + regionRef.length, regionRef.z, &pointFinal[3].x, &pointFinal[3].y, &pointFinal[3].z);

	kL3dTransform3d_Apply(transform, regionRef.x, regionRef.y, regionRef.z + regionRef.height, &pointFinal[4].x, &pointFinal[4].y, &pointFinal[4].z);
	kL3dTransform3d_Apply(transform, regionRef.x + regionRef.width, regionRef.y, regionRef.z + regionRef.height, &pointFinal[5].x, &pointFinal[5].y, &pointFinal[5].z);
	kL3dTransform3d_Apply(transform, regionRef.x + regionRef.width, regionRef.y + regionRef.length, regionRef.z + regionRef.height, &pointFinal[6].x, &pointFinal[6].y, &pointFinal[6].z);
	kL3dTransform3d_Apply(transform, regionRef.x, regionRef.y + regionRef.length, regionRef.z + regionRef.height, &pointFinal[7].x, &pointFinal[7].y, &pointFinal[7].z);

	result.x = pointFinal[0].x;
	result.y = pointFinal[0].y;
	result.z = pointFinal[0].z;
	for (i = 0; i < 8; ++i)
	{
		GdkUtilSurface_ExpandRegionToPoint(&result, pointFinal[i]);
	}

	return result;
}

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
GtsFx(kStatus) GdkSurface_ConvertRegionFromCoordinateToPixel_3D(GdkInputItem item, GdkRegion3d64f* region, k32s* xStart, k32s* xEnd, k32s* yStart, k32s* yEnd, k32s* zStart, k32s* zEnd, k32u* width, k32u* length, k32u* height)
{
	GdkDataInfo itemInfo = GdkInputItem_Info(item);
	kArrayList pointList = kNULL;
	kStatus exception = kOK;

	k32s xDim;
	k32s yDim;
	k32s zDim;

	*xStart = (k32s)((region->x - GdkInputItem_Offset(item)->x) / GdkDataInfo_Scale(itemInfo)->x);
	*xEnd = (k32s)(((region->x + region->width) - GdkInputItem_Offset(item)->x) / GdkDataInfo_Scale(itemInfo)->x) + 1;
	*yStart = (k32s)((region->y - GdkInputItem_Offset(item)->y) / GdkDataInfo_Scale(itemInfo)->y);
	*yEnd = (k32s)(((region->y + region->length) - GdkInputItem_Offset(item)->y) / GdkDataInfo_Scale(itemInfo)->y) + 1;
	*zStart = (k32s)((region->z - GdkInputItem_Offset(item)->z) / GdkDataInfo_Scale(itemInfo)->z);
	*zEnd = (k32s)(((region->z + region->height) - GdkInputItem_Offset(item)->z) / GdkDataInfo_Scale(itemInfo)->z) + 1;
	//// this shows the limitation of 9.30 at RS for the 2320 project
	if (*xStart < 0) *xStart = 0;
	if (*xEnd >(k32s)GdkSurfaceInput_ColumnCount(item)) *xEnd = (k32s)GdkSurfaceInput_ColumnCount(item);
	if (*yStart < 0) *yStart = 0;
	if (*yEnd >(k32s)GdkSurfaceInput_RowCount(item)) *yEnd = (k32s)GdkSurfaceInput_RowCount(item);
	if (*zStart < k16S_MIN) *zStart = (k32s)k16S_MIN;
	if (*zEnd >(k32s)k16S_MAX) *zEnd = (k32s)k16S_MAX;

	xDim = *xEnd - *xStart;
	yDim = *yEnd - *yStart;
	zDim = *zEnd - *zStart;

	*width = xDim < 0 ? 0 : xDim;
	*length = yDim < 0 ? 0 : yDim;
	*height = zDim < 0 ? 0 : zDim;
	return kOK;
}


