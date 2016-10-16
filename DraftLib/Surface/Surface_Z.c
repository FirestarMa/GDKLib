#include "Surface_Z.h"


/**
* Convert plane normal to a, b, c, d.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	plane			surface plane
* @return					Height plane in a, b, c, d mode.
*/
GdkSurface_HeightPlane GdkSurface_ToPlane3d(kPlane3d64f plane)
{
	// Parameterize the plane
	GdkSurface_HeightPlane plane3d;
	plane3d.a = plane.norm.x;
	plane3d.b = plane.norm.y;
	plane3d.c = plane.norm.z;
	plane3d.d = -plane.p.z;
	return plane3d;
}

/**
* Signed distance from target point to surface plane
*
* @public					@memberof created by Chi Ho and Vincent
* @param	plane			surface plane
* @param	point			target point
* @return					signed distance
*/
k64f GdkSurface_SignedDistance(GdkSurface_HeightPlane plane, kPoint3d64f point)
{
	return (plane.a * point.x + plane.b * point.y + plane.c * point.z + plane.d);
}

/**
* Transform the region information from mm to pixel. This function doesn't if (x,y) is out of bounds.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	item			input data item
* @param	point			target point with (x,y).
* @return					operation status.
*/
kStatus GdkSurface_GetPointZ(GdkInputItem item, kPoint3d64f* point)
{
	GdkDataInfo itemInfo = GdkInputItem_Info(item);
	k32s xStart = (k32s)((point->x - GdkInputItem_Offset(item)->x) / GdkDataInfo_Scale(itemInfo)->x);
	k32s yStart = (k32s)((point->y - GdkInputItem_Offset(item)->y) / GdkDataInfo_Scale(itemInfo)->y);
	const k16s *src;

	src = GdkSurfaceInput_RangeRowAt(item, (kSize)(yStart));
	point->z = src[xStart] * GdkDataInfo_Scale(itemInfo)->z + GdkInputItem_Offset(item)->z;
	return kOK;
}

/**
* Transform the region information from mm to pixel. Safe means it checks if (x,y) is out of bounds.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	item			input data item
* @param	point			target point with (x,y).
* @return					operation status.
*/
kStatus GdkSurface_GetPointZ_Safe(GdkInputItem item, kPoint3d64f* point)
{
	GdkDataInfo itemInfo = GdkInputItem_Info(item);
	k32s xStart = (k32s)((point->x - GdkInputItem_Offset(item)->x) / GdkDataInfo_Scale(itemInfo)->x);
	k32s yStart = (k32s)((point->y - GdkInputItem_Offset(item)->y) / GdkDataInfo_Scale(itemInfo)->y);
	kStatus exception = kOK;
	const k16s *src;

	if (xStart < 0) return kERROR;
	if (xStart >= (k32s)GdkSurfaceInput_ColumnCount(item)) return kERROR;
	if (yStart < 0) return kERROR;
	if (yStart >= (k32s)GdkSurfaceInput_RowCount(item)) return kERROR;

	src = GdkSurfaceInput_RangeRowAt(item, (kSize)(yStart));
	if (src == kNULL) return kERROR;

	point->z = src[xStart] * GdkDataInfo_Scale(itemInfo)->z + GdkInputItem_Offset(item)->z;
	return kOK;
}

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
kStatus GdkSurface_FindOutliers(k64f lowerTolerance, k64f upperTolerance, kArrayList inputPoints, kArrayList *outliers, GdkTool tool)
{
	kStatus exception = kOK;

	if (inputPoints == kNULL)
	{
		*outliers = kNULL;
		return kOK;
	}

	kTry
	{
		k32u i;
		kPoint3d64f *points = (kPoint3d64f *)kArrayList_Data(inputPoints);
		kTest(kArrayList_Construct(outliers, kTypeOf(kPoint3d64f), kArrayList_Count(inputPoints), kObject_Alloc(tool)));

		for (i = 0; i < kArrayList_Count(inputPoints); i++)
		{
			if ((points[i].z < lowerTolerance) || (points[i].z > upperTolerance))
			{
				kTest(kArrayList_Add(*outliers, &(points[i])));
			}
		}
	}
		kCatchEx(&exception)
	{
		kEndCatchEx(exception);
	}
	kFinallyEx
	{
		kEndFinallyEx();
	}
	return kOK;
}
