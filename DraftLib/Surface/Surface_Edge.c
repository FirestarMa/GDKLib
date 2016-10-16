#include "Surface_Edge.h"

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
GtsFx(kStatus) GdkSurface_GetEdgePoints_2D(GdkInputItem item, GdkRegion3d64f* region, GdkSurface_EdgeSettings *settings, kArrayList edgePoints)
{
	// For every line in the axis given, scan all pixels, marking the strongest edge.
	// Then fit a line, remove outliers, start again.

	//// receive tool information
	GdkDataInfo itemInfo = GdkInputItem_Info(item);
	//// define edge searching direction. If edge is expected to be vertical, searching direction should be horizontal.
	//// edgeAngle is 90 or 0, depending on setup in tool panel.
	k64f scanAngle = settings->edgeAngle + 90;

	k64f zRes = GdkDataInfo_Scale(itemInfo)->z;
	k64f zOffset = GdkInputItem_Offset(item)->z;

	k32s xStart;
	k32s xEnd;
	k32s yStart;
	k32s yEnd;
	k32s zStart;
	k32s zEnd;
	k32u width;
	k32u length;
	k32u height;
	kSize x, y;
	int i;
	//// as their name
	k64f edgeStrength;
	k64f curEdgeStrength;
	kPoint3d64f curEdgePoint = { 0.0, 0.0, 0.0 };

	kArray1 edgeArray = kNULL;
	k64f* edgeArrayData = kNULL;
	k64f lowerThreshold;
	k64f higherThreshold;
	GdkSurface_ConvertRegionFromCoordinateToPixel_3D(item, region, &xStart, &xEnd, &yStart, &yEnd, &zStart, &zEnd, &width, &length, &height);

	//// if region is too small, return error
	if (width <= settings->kernelSize || length <= settings->kernelSize || height <= 0)
	{
		return kERROR;
	}
	//// if height map, transfer threshold in mm to threshold in k16s
	if (settings->dataType == GDK_SURFACE_HEIGHT_DATA_TYPE_HEIGHT)
	{
		lowerThreshold = settings->lowerThreshold / zRes;
		higherThreshold = settings->higherThreshold / zRes;
	}
	//// if intensity, then use it directly
	else
	{
		lowerThreshold = settings->lowerThreshold;
		higherThreshold = settings->higherThreshold;
	}
	//// for debugging
	//	GdkSurfaceGeneralizedHeight_Trace("Height Edge detection: %f, %f. Direction %d, Z res %f, Z offset %f\r\n", settings->lowerThreshold, settings->higherThreshold, (int)settings->direction, zRes, zOffset);
	//// horizontal search for vertical edges
	if (settings->edgeAngle == 90.0) // Vertical edge. Search horizontally
	{
		//// for debugging
		//		GdkSurfaceGeneralizedHeight_Trace("Searching for vertical edge\r\n");
		//// fixedAngle to be re-checked
		if (settings->fixedAngle)
		{
			//// fixedAngle might mean the line is straight and angle is fixed, so number of data points is fixed. To be checked
			kArray1_Construct(&edgeArray, kTypeOf(k64f), width, kNULL);
			kArray1_Zero(edgeArray);
			edgeArrayData = kArray1_Data(edgeArray);
		}

		for (y = 0; y < length; ++y)
		{
			//// pointer to each row
			const k16s* inRow = GdkSurfaceInput_RangeRowAt(item, yStart + y);
			const k8u* inRowIntensity = kNULL;

			if (inRow == kNULL) return kERROR;

			if (settings->dataType == GDK_SURFACE_HEIGHT_DATA_TYPE_INTENSITY)
			{
				inRowIntensity = GdkSurfaceInput_IntensityRowAt(item, yStart + y);
			}
			//// go to xStart as start position
			inRow += xStart;
			if (inRowIntensity) inRowIntensity += xStart;
			//// init edge detection identifier
			curEdgeStrength = 0;
			//// here comes the detection algo..
			for (x = 0; x < width - settings->kernelSize; ++x)
			{
				k32u j;


				//ignore edges to INVALID data
				const k16s inRowX = inRow[x];
				const k16s inRowXK = inRow[x + settings->kernelSize];
				//k8u inRowIntensityX;
				//k8u inRowIntensityXK;
				kBool outsideZ = kFALSE;

				//if (inRowIntensity)
				//{
				//	inRowIntensityX = inRowIntensity[x];
				//	inRowIntensityXK = inRowIntensity[x + settings->kernelSize];
				//}

				//Check if any data within the kernel is outside of Z boundary. If so, ignore this kernel
				for (j = 0; j < settings->kernelSize; j++)
				{
					if (inRow[x + j] != k16S_NULL)
					{
						if ((inRow[x + j] < zStart) || (inRow[x + j] > zEnd)) outsideZ = kTRUE;
					}
				}
				//// if there is height data outside of the region, no valid edge here. Stop and go to next x.
				if (outsideZ) continue;

				//// get edge strength from intensity or height map
				if (settings->dataType == GDK_SURFACE_HEIGHT_DATA_TYPE_INTENSITY)
				{
					edgeStrength = GdkSurface_CalculateIntensityEdgeStrength(&inRowIntensity[x], settings->kernelSize, settings->dataType);
				}
				else
				{
					edgeStrength = GdkSurface_CalculateHeightEdgeStrength(&inRow[x], settings->kernelSize, settings->direction, zRes, zOffset);
				}

				//GdkSurfaceGeneralizedHeight_Trace("Edge detection (Raw): Row: %u (%f), X: %f, strength: %f, startValue %d, endValue %d\r\n", y, (y + yStart) * GdkDataInfo_Scale(itemInfo)->y + GdkInputItem_Offset(item)->y, (x + xStart) * GdkDataInfo_Scale(itemInfo)->x + GdkInputItem_Offset(item)->x, edgeStrength, inRowX, inRowXK);

				//// check if edge is valid. 
				if ((edgeStrength > 0) && (edgeStrength >= lowerThreshold && edgeStrength <= higherThreshold))
				{
					// We have a valid edge.
					//// don't know waht fixedAngle is 
					if (settings->fixedAngle)
					{
						// Should we consider just ecord the peak location in the search? Right now we are summing it all up regardless of the filter
						edgeArrayData[x] += edgeStrength;
						//// just for debugging
						//						GdkSurfaceGeneralizedHeight_Trace("Edge detection (fixed angle, thresholded): Row: %d(%f), X: %f, strength: %f, startValue %d(%f), endValue %d(%f)\r\n", y, (y + yStart) * GdkDataInfo_Scale(itemInfo)->y + GdkInputItem_Offset(item)->y, (x + xStart) * GdkDataInfo_Scale(itemInfo)->x + GdkInputItem_Offset(item)->x, edgeStrength, inRowX, (inRowX)* GdkDataInfo_Scale(itemInfo)->z + GdkInputItem_Offset(item)->z, inRowXK, (inRowXK)* GdkDataInfo_Scale(itemInfo)->z + GdkInputItem_Offset(item)->z);
					}
					else
					{
						//// if edge strength is valid, assign it to curent valid edge strength and record this point
						if (GdkSurface_CalculateEdgeFilter(settings->filter, edgeStrength, curEdgeStrength))
						{
							curEdgeStrength = edgeStrength;
							curEdgePoint.x = (x + xStart) * GdkDataInfo_Scale(itemInfo)->x + GdkInputItem_Offset(item)->x;
							curEdgePoint.y = (y + yStart) * GdkDataInfo_Scale(itemInfo)->y + GdkInputItem_Offset(item)->y;
							curEdgePoint.z = 0;

							//GdkSurfaceGeneralizedHeight_Trace("Edge detection (angle, filtered): Row: %d(%f), X: %f, strength: %f, startValue %d(%f), endValue %d(%f)\r\n", y, curEdgePoint.y, curEdgePoint.x, curEdgeStrength, inRowX, (inRowX)* GdkDataInfo_Scale(itemInfo)->z + GdkInputItem_Offset(item)->z, inRowXK, (inRowXK)* GdkDataInfo_Scale(itemInfo)->z + GdkInputItem_Offset(item)->z);
						}
					}
				}
			}
			//// if valid edge strength, then add it to edgePoints 
			if ((curEdgeStrength != 0) && (!settings->fixedAngle))
			{
				//GdkSurfaceGeneralizedHeight_Trace("Edge detection (angled, final): Row: %d(%f), X: %f, strength: %f\r\n", y, curEdgePoint.y, curEdgePoint.x, curEdgeStrength);
				kArrayList_Add(edgePoints, &curEdgePoint);
			}
		}
		//// now the edge points are located

		//// find the max strength point, and draw a vertical (along Y axis) edge
		if (settings->fixedAngle)
		{
			// Scan for higher number of edgecount in array: x is the position of the edge
			int maxX = 0;
			k64f maxStrength = 0;
			//// find the point with max strength
			for (i = 0; i < (int)width; ++i)
			{
				if (edgeArrayData[i] > maxStrength)
				{
					maxStrength = edgeArrayData[i];
					maxX = i;
				}
			}
			curEdgePoint.x = (maxX + xStart) * GdkDataInfo_Scale(itemInfo)->x + GdkInputItem_Offset(item)->x;
			curEdgePoint.y = (0 + yStart) * GdkDataInfo_Scale(itemInfo)->y + GdkInputItem_Offset(item)->y;
			curEdgePoint.z = 0;
			kArrayList_Add(edgePoints, &curEdgePoint);
			curEdgePoint.x = (maxX + xStart) * GdkDataInfo_Scale(itemInfo)->x + GdkInputItem_Offset(item)->x;
			curEdgePoint.y = (length + yStart) * GdkDataInfo_Scale(itemInfo)->y + GdkInputItem_Offset(item)->y;
			curEdgePoint.z = 0;
			kArrayList_Add(edgePoints, &curEdgePoint);

			//			GdkSurfaceGeneralizedHeight_Trace("Edge detection (fixed angle, final): X: %f, strength: %I64d\r\n", curEdgePoint.x, maxStrength);
		}
	}
	//// horizontal edge
	else // Beginning of horizontal edge
	{

		//// vertical searching is not row search, so malloc space for strength calculation


		k8u  *inIntensityData = kNULL;		//// Create an array to store all the data before passing to strength functions
		k16s  *inHeightData = kNULL;		//// Create an array to store all the data before passing to strength functions

		kMemAlloc(sizeof(k8u)*(settings->kernelSize + 1), inIntensityData);
		kMemAlloc(sizeof(k16s)*(settings->kernelSize + 1), inHeightData);
		//		GdkSurfaceGeneralizedHeight_Trace("Searching for horizontal edge\r\n");

		if (settings->fixedAngle)
		{
			kArray1_Construct(&edgeArray, kTypeOf(k64u), length, kNULL);
			kArray1_Zero(edgeArray);
			edgeArrayData = kArray1_Data(edgeArray);
		}
		//// loop over each column!
		for (x = 0; x < width; ++x)
		{
			curEdgeStrength = 0;
			//// loop over each row!
			for (y = 0; y < length - settings->kernelSize; ++y)
			{
				k32u j;
				//// inRow, pointer of start
				//// inRow2 pointer of end
				const k16s* inRow = GdkSurfaceInput_RangeRowAt(item, yStart + y) + xStart;
				const k16s* inRow2 = GdkSurfaceInput_RangeRowAt(item, yStart + y + settings->kernelSize) + xStart;
				const k16s inRowX = inRow[x];
				const k16s inRowXK = inRow2[x];
				//const k8u* inRowIntensity = kNULL;  
				//const k8u* inRowIntensity2 = kNULL; 
				kBool outsideZ = kFALSE;

				//if (settings->dataType == GDK_SURFACE_HEIGHT_DATA_TYPE_INTENSITY)
				//{
				//	inRowIntensity = GdkSurfaceInput_IntensityRowAt(item, yStart + y) + xStart;
				//	inRowIntensity2 = GdkSurfaceInput_IntensityRowAt(item, yStart + y + settings->kernelSize) + xStart;
				//}

				//// put the data (starting from x, size of kernel size+1, direction in column) in a pointer. This pointer will be used for edge strength calculation.
				for (j = 0; j <= settings->kernelSize; j++)
				{
					if (settings->dataType == GDK_SURFACE_HEIGHT_DATA_TYPE_INTENSITY)
					{
						inIntensityData[j] = *(GdkSurfaceInput_IntensityRowAt(item, yStart + y + j) + x + xStart);
					}

					inHeightData[j] = *(GdkSurfaceInput_RangeRowAt(item, yStart + y + j) + x + xStart);
					if (inHeightData[j] != k16S_NULL)
					{
						if ((inHeightData[j] < zStart) || (inHeightData[j] > zEnd)) outsideZ = kTRUE;
					}
				}
				//// if Z is outsied of the region, jump out and continue;
				if (outsideZ) continue;

				//// calculate edge strength
				if (settings->dataType == GDK_SURFACE_HEIGHT_DATA_TYPE_INTENSITY)
				{
					edgeStrength = GdkSurface_CalculateIntensityEdgeStrength(inIntensityData, settings->kernelSize, settings->direction);
				}
				else
				{
					edgeStrength = GdkSurface_CalculateHeightEdgeStrength(inHeightData, settings->kernelSize, settings->direction, zRes, zOffset);
				}
				//// validate edge strength
				if ((edgeStrength > 0) && (edgeStrength >= lowerThreshold && edgeStrength <= higherThreshold))
				{
					// We have a valid edge.
					if (settings->fixedAngle)
					{
						// For fixed angle, cumulate the edges at this position
						edgeArrayData[y] += edgeStrength;
					}
					else
					{
						// For variable angle, get the strongest edge.
						//// use edge filter setting in tool panel to validate strengh of each point
						if (GdkSurface_CalculateEdgeFilter(settings->filter, edgeStrength, curEdgeStrength))
						{
							curEdgeStrength = edgeStrength;
							curEdgePoint.x = (x + xStart) * GdkDataInfo_Scale(itemInfo)->x + GdkInputItem_Offset(item)->x;
							curEdgePoint.y = (y + yStart) * GdkDataInfo_Scale(itemInfo)->y + GdkInputItem_Offset(item)->y;
							curEdgePoint.z = 0;

							//GdkSurfaceGeneralizedHeight_Trace("Edge detection (height): Col: %d(%f), Row: %f, strength: %I64d, startValue %d(%f), endValue %d(%f)\r\n", x, curEdgePoint.x, curEdgePoint.y, curEdgeStrength, inRowX, (inRowX)* GdkDataInfo_Scale(itemInfo)->z + GdkInputItem_Offset(item)->z, inRowXK, (inRowXK)* GdkDataInfo_Scale(itemInfo)->z + GdkInputItem_Offset(item)->z);
						}
					}
				}
			}
			//// vincent said it is not working
			if ((!settings->fixedAngle) && (curEdgeStrength != 0))
			{
				//GdkSurfaceGeneralizedHeight_Trace("Edge detection (angled, final): Col: %d(%f), Row: %f, strength: %f\r\n", x, curEdgePoint.x, curEdgePoint.y, curEdgeStrength);
				kArrayList_Add(edgePoints, &curEdgePoint);
			}
		}
		//// vincent said it is not working
		if (settings->fixedAngle)
		{
			// Scan for higher number of edgecount in array: y is the position of the edge
			int maxY = 0;
			k64f maxCount = 0;

			for (i = 0; i < (int)length; ++i)
			{
				if (edgeArrayData[i] > maxCount)
				{
					maxCount = edgeArrayData[i];
					maxY = i;
				}
			}
			curEdgePoint.x = (0 + xStart) * GdkDataInfo_Scale(itemInfo)->x + GdkInputItem_Offset(item)->x;
			curEdgePoint.y = (maxY + yStart) * GdkDataInfo_Scale(itemInfo)->y + GdkInputItem_Offset(item)->y;
			curEdgePoint.z = 0;
			kArrayList_Add(edgePoints, &curEdgePoint);
			curEdgePoint.x = (width + xStart) * GdkDataInfo_Scale(itemInfo)->x + GdkInputItem_Offset(item)->x;
			curEdgePoint.y = (maxY + yStart) * GdkDataInfo_Scale(itemInfo)->y + GdkInputItem_Offset(item)->y;
			curEdgePoint.z = 0;
			kArrayList_Add(edgePoints, &curEdgePoint);
		}
		kMemFree(inIntensityData);
		kMemFree(inHeightData);
	}
	kDestroyRef(&edgeArray);

	return kOK;
}

/**
* Calculate edge strength value based on intensity data for a kernel at x.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	data			pointer to the position in array.
* @param	kernelSize		range for edge detection (kernel size).
* @param	direction		search direction.
* @return					edge strength.
*/
static k64f GdkSurface_CalculateIntensityEdgeStrength(const k8u *data, k32s kernelSize, GdkSurface_EdgeType direction)
{
	k32s startValue = data[0];
	k64f totalStrength = 0;
	k32s i;
	//// the input *data is a 1d array, so edge strength is only calculated in the pre-defined searching direction, e.g. vertical or horizontal
	for (i = 1; i <= kernelSize; i++)
	{
		k32s endValue = data[i];
		// Note that we have to subtract from  k16s_NULL to find the right distance when only one value is valid!
		switch (direction)
		{
			////why use if? 0-0=0...
		case GDK_SURFACE_EDGE_DIRECTION_ANY:
		{
			//// all differences counts
			if ((startValue == 0) || (endValue == 0)) totalStrength += 0;
			else totalStrength += kMath_Abs_(startValue - endValue);
		}
		break;
		case GDK_SURFACE_EDGE_DIRECTION_RISING:
		{
			//// valid only when endValue is bigger
			if ((startValue == 0) || (endValue == 0)) totalStrength += 0;
			else if (startValue < endValue) totalStrength += kMath_Abs_(endValue - startValue);
		}
		break;
		case GDK_SURFACE_EDGE_DIRECTION_RISING_FROM_ZERO:
		{
			//// valid only when startValue (value at data[0]) is 0
			if (startValue != 0) totalStrength += 0;
			else if (endValue == 0) totalStrength += 0;
			else totalStrength += kMath_Abs_(endValue);
		}
		break;
		case GDK_SURFACE_EDGE_DIRECTION_FALLING:
		{
			//// valid only when endValue is smaller
			if ((startValue == 0) || (endValue == 0)) totalStrength += 0;
			else if (startValue > endValue) totalStrength += kMath_Abs_(startValue - endValue);
		}
		break;
		case GDK_SURFACE_EDGE_DIRECTION_FALLING_TO_ZERO:
		{
			//// valid only when startValue is nonzero and endValue is zero
			if (endValue != 0) totalStrength += 0;
			else if (startValue == 0) totalStrength += 0;
			else totalStrength += kMath_Abs_(startValue);
		}
		break;
		}
	}
	return totalStrength;
}

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
static k64f GdkSurface_CalculateHeightEdgeStrength(const k16s *data, k32s kernelSize, GdkSurface_EdgeType direction, k64f zRes, k64f offset)
{
	k32s startValue = data[0];
	k64f totalStrength = 0;
	k32s i;
	////k32s value when z=0mm;
	k32s valueAtZero = (k32s)((offset / zRes) * (-1.0));

	//// same as comment in "GdkSurfaceCalculateIntensityEdgeStrength()"
	for (i = 1; i <= kernelSize; i++)
	{
		k32s endValue = data[i];
		// Note that we have to subtract from  k16s_NULL to find the right distance when only one value is valid!
		switch (direction)
		{
		case GDK_SURFACE_EDGE_DIRECTION_ANY:
		{
			if ((startValue == k16S_NULL) || (endValue == k16S_NULL)) totalStrength += 0;
			else totalStrength += kMath_Abs_(endValue - startValue);
		}
		break;
		case GDK_SURFACE_EDGE_DIRECTION_RISING:
		{
			if ((startValue == k16S_NULL) || (endValue == k16S_NULL)) totalStrength += 0;
			else if (startValue < endValue) totalStrength += kMath_Abs_(endValue - startValue);
		}
		break;
		case GDK_SURFACE_EDGE_DIRECTION_RISING_FROM_ZERO:
		{
			if (startValue != k16S_NULL) totalStrength += 0;
			else if (endValue == k16S_NULL) totalStrength += 0;
			else totalStrength += kMath_Abs_(endValue - valueAtZero);
		}
		break;
		case GDK_SURFACE_EDGE_DIRECTION_FALLING:
		{
			if ((startValue == k16S_NULL) || (endValue == k16S_NULL)) totalStrength += 0;
			else if (startValue > endValue) totalStrength += kMath_Abs_(startValue - endValue);
		}
		break;
		case GDK_SURFACE_EDGE_DIRECTION_FALLING_TO_ZERO:
		{
			if (endValue != k16S_NULL) totalStrength += 0;
			else if (startValue == k16S_NULL) totalStrength += 0;
			else totalStrength += kMath_Abs_(startValue - valueAtZero);
		}
		break;
		}
	}
	return totalStrength;
}

/**
* Check if the edge point located is the one we need.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	filter			filter type as specified in tool panel.
* @param	edgeStrength	edgeStrength at current x.
* @param	curEdgeStrength	current recognized edge strength.
* @return					operation status.
*/
static kBool GdkSurface_CalculateEdgeFilter(GdkSurface_EdgeFilter filter, k64f edgeStrength, k64f curEdgeStrength)
{
	kBool foundBetterEdgePoint = kFALSE;

	switch (filter)
	{
		//// if edge strength is the biggest, use it
	case GDK_SURFACE_EDGE_FILTER_PEAK:
	{
		if (edgeStrength > curEdgeStrength) foundBetterEdgePoint = kTRUE;
	}
	break;
	//// if the edge strength is the first one, use it
	case GDK_SURFACE_EDGE_FILTER_FIRST:
	{
		if (curEdgeStrength == 0) foundBetterEdgePoint = kTRUE;
	}
	break;
	//// if the edge strength is the last one, use it
	case GDK_SURFACE_EDGE_FILTER_LAST:
	{
		foundBetterEdgePoint = kTRUE;
	}
	break;
	}

	return foundBetterEdgePoint;
}