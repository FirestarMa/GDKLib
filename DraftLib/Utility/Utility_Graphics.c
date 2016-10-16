#include "Utility_Graphics.h"
#include "..\Surface.h"

/**
* To convert point from k64f to k32f. Only to make points addable to graphics. Internal use only.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	pointList		point list to convert
* @return					Converted points for output
*/
kArrayList GdkUtilGraphics_ConvertPoints64ToPoints32(kArrayList pointList)
{
	kSize i;
	kArrayList pointListOut = kNULL;
	kPoint3d64f* pointRef = kNULL;
	kSize pointCount = 0;
	kSize newPointCount = 0;
	kSize step = 1;

	if (pointList == kNULL)
		return kNULL;

	pointCount = kArrayList_Count(pointList);
	newPointCount = pointCount;
	if (pointCount > 1000)
	{
		step = pointCount / 1000;
		newPointCount = 1000;
	}
	kArrayList_Construct(&pointListOut, kTypeOf(kPoint3d32f), newPointCount, kNULL);

	pointRef = (kPoint3d64f*)kArrayList_At(pointList, 0);
	for (i = 0; i < pointCount; i += step)
	{
		kPoint3d32f point;
		kPoint3d_Init_(&point, (k32f)pointRef[i].x, (k32f)pointRef[i].y, (k32f)pointRef[i].z);
		kArrayList_Add(pointListOut, &point);
	}
	return pointListOut;
}

/**
* Add points to graphics with specified format
*
* @public					@memberof created by Chi Ho and Vincent
* @param	graphic			graphic object to operate on
* @param	pointSize		size of the point to be drawn
* @param	shape			shape of points
* @param	color			color of points
* @param	points			points to be added to graphics
* @return					Operation status
*/
kStatus GdkUtilGraphics_AddPointsToGraphics(GdkGraphic graphic, k32f pointSize, kMarkerShape shape, kColor color, kArrayList points)
{
	//GdkSurfaceGeneralizedHeightClass* obj = GdkSurfaceGeneralizedHeight_Cast_(tool);
	GdkGraphicPointSet pointSet = kNULL;
	kStatus exception = kOK;
	kArrayList points32f = kNULL;

	if (!points) return kOK;

	kTry
	{
		if (kArrayList_ItemType(points) == kTypeOf(kPoint3d64f))
		{
			points32f = GdkUtilGraphics_ConvertPoints64ToPoints32(points);
		}
		else
		{
			points32f = points;
		}

		// Edge points
		if (points != kNULL)
		{
			kTest(GdkGraphicPointSet_Construct(&pointSet, pointSize, shape, color, kArrayList_Data(points32f), kArrayList_Count(points32f), kNULL));
			kTest(GdkGraphic_AddPointSet(graphic, pointSet));
			pointSet = kNULL;
		}
	}
		kCatch(&exception)
	{
		kDestroyRef(&pointSet);
		if (kArrayList_ItemType(points) == kTypeOf(kPoint3d64f))
		{
			kDestroyRef(&points32f);
		}
		kEndCatch(exception);
	}

	return kOK;
}

/**
* Add lines that describes a circle to graphics with specified format
*
* @public					@memberof created by Chi Ho and Vincent
* @param	graphic			graphic object to operate on
* @param	center			center of the circle
* @param	radius			radius of the circle
* @param	stepAngle		step angle for line creation
* @param	width			width of the line
* @param	color			color of points
* @return					Operation status
*/
kStatus GdkUtilGraphics_AddGraphicCircle(GdkGraphic graphic, kPoint3d64f center, k64f radius, k64f stepAngle, k32f width, kColor color)
{
	GdkGraphicPointSet lineSet = kNULL;
	kArrayList points32f = kNULL;
	kStatus exception = kOK;
	kSize i, pointCount = (kSize)(360 / stepAngle) + 1;
	kPoint3d32f startPoint;
	// should add starting point to the tail to make a closed shape
	kArrayList_Construct(&points32f, kTypeOf(kPoint3d32f), pointCount+1, kNULL);
	kTry
	{
		for (i = 0; i < pointCount; ++i)
		{
			k64f angle = i*stepAngle * M_PI / 180.0;
			k64f c = cos(angle);
			k64f s = sin(angle);
			kPoint3d32f point;
			kPoint3d_Init_(&point, (k32f)(c*radius + center.x), (k32f)(s*radius + center.y), (k32f)(center.z));
			if (i == 0)
				startPoint = point;
			kArrayList_Add(points32f, &point);
		}
		kArrayList_Add(points32f, &startPoint);

		kTest(GdkGraphicLineSet_Construct(&lineSet, width, color, kArrayList_Data(points32f), kArrayList_Count(points32f), kNULL));
		kTest(GdkGraphic_AddLineSet(graphic, lineSet));
		lineSet = kNULL;
	}
		kCatch(&exception)
	{
		kDestroyRef(&lineSet);
		kDestroyRef(&points32f);
		kEndCatch(exception);
	}

	return kOK;
}

/**
* Add lines to graphics with specified format
*
* @public					@memberof created by Chi Ho and Vincent
* @param	graphic			graphic object to operate on
* @param	width			width of the line
* @param	color			color of points
* @param	points			points to be added to graphics
* @return					Operation status
*/
kStatus GdkUtilGraphics_AddGraphicLines(GdkGraphic graphic, k32f width, kColor color, kArrayList points)
{
	GdkGraphicPointSet lineSet = kNULL;
	kArrayList points32f = kNULL;
	kStatus exception = kOK;

	if (!points) return kOK;

	kTry
	{
		if (kArrayList_ItemType(points) == kTypeOf(kPoint3d64f))
		{
			points32f = GdkUtilGraphics_ConvertPoints64ToPoints32(points);
		}
		else
		{
			points32f = points;
		}

		// Edge points
		if (points != kNULL)
		{
			kTest(GdkGraphicLineSet_Construct(&lineSet, width, color, kArrayList_Data(points32f), kArrayList_Count(points32f), kNULL));
			kTest(GdkGraphic_AddLineSet(graphic, lineSet));
			lineSet = kNULL;
		}
	}
		kCatch(&exception)
	{
		kDestroyRef(&lineSet);
		if (kArrayList_ItemType(points) == kTypeOf(kPoint3d64f))
		{
			kDestroyRef(&points32f);
		}
		kEndCatch(exception);
	}

	return kOK;
}

/**
* Add surface to graphic with specified format
*
* @public					@memberof created by Chi Ho and Vincent
* @param	graphic			graphic object to operate on
* @param	width			width of the line
* @param	color			color of points
* @param	region			plane region that is visible
* @param	plane			plane to be added
* @return					Operation status
*/
kStatus GdkUtilGraphics_AddGraphicPlane(GdkGraphic graphic, k32f width, kColor color, GdkRegion3d64f *region, kPlane3d64f plane)
{
	kStatus exception = kOK;
	kArray1 plotLines1 = kNULL;
	GdkGraphicLineSet lineSet = kNULL;

	if (!region) return kOK;

	kTry
	{
		kPoint3d32f* plotLine1;
		k16u max = k16U_MIN;
		k32u maxIdx = 0;
		k32u i = 0;

		k32u pointCount = 8;
		kTest(kArray1_Construct(&plotLines1, kTypeOf(kPoint3d32f), pointCount, kNULL));
		plotLine1 = kArray1_At(plotLines1, 0);

		// plane
		plotLine1[0].x = (k32f)region->x;
		plotLine1[0].y = (k32f)region->y;
		plotLine1[0].z = (k32f)0;
		plotLine1[1].x = (k32f)(region->x + region->width);
		plotLine1[1].y = (k32f)region->y;
		plotLine1[1].z = (k32f)0;
		plotLine1[2].x = (k32f)(region->x + region->width);
		plotLine1[2].y = (k32f)(region->y + region->length);
		plotLine1[2].z = (k32f)0;
		plotLine1[3].x = (k32f)region->x;
		plotLine1[3].y = (k32f)(region->y + region->length);
		plotLine1[3].z = (k32f)0;
		plotLine1[4].x = (k32f)region->x;
		plotLine1[4].y = (k32f)region->y;
		plotLine1[4].z = (k32f)0;
		plotLine1[5].x = (k32f)(region->x + region->width);
		plotLine1[5].y = (k32f)(region->y + region->length);
		plotLine1[5].z = (k32f)0;
		plotLine1[6].x = (k32f)region->x;
		plotLine1[6].y = (k32f)(region->y + region->length);
		plotLine1[6].z = (k32f)0;
		plotLine1[7].x = (k32f)(region->x + region->width);
		plotLine1[7].y = (k32f)region->y;
		plotLine1[7].z = (k32f)0;

		for (i = 0; i < pointCount; ++i)
		{
			GdkUtilGraphics_PlaneVerticalIntersect32(plane, &plotLine1[i]);
		}

		kTest(GdkGraphicLineSet_Construct(&lineSet, width, color, plotLine1, pointCount, kNULL));
		kTest(GdkGraphic_AddLineSet(graphic, lineSet));
		lineSet = kNULL;
	}
		kCatch(&exception)
	{
		kDestroyRef(&plotLines1);
		kDestroyRef(&lineSet);
		kEndCatch(exception);
	}

	return kOK;
}

/**
* Get the vertical intersection. Internal use only.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	plane			plane to be added
* @param	pt				point to receive value
* @return					Operation status
*/
void GdkUtilGraphics_PlaneVerticalIntersect32(kPlane3d64f plane, kPoint3d32f* pt)
{
	GdkSurface_HeightPlane plane3d = GdkSurface_ToPlane3d(plane);
	k64f u;
	k64f denominator = plane3d.c;
	if (denominator == 0.0f)
	{
		return;
	}
	u = (plane3d.a * pt->x + plane3d.b * pt->y + plane3d.c * pt->z + plane3d.d) / denominator;

	pt->z -= (k32f)u;
	return;
}