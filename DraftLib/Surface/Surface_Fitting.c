#include "Surface_Fitting.h"

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
GtsFx(kStatus) GdkSurface_FitSphere(kArrayList pointList, kPoint3d64f* center, kPoint3d64f* centerOut, k64f* radiusOut)
{
	kPoint3d64f* pointPtr = kNULL;
	kStatus exception = kOK;
	k32s ix;

	kPoint3d64f pointAccumulator = { 0, 0, 0 };
	k64s pointCount = 0;
	k64s pointListCount = 1;
	k64f pointAccTemp = 0;

	k64f a = center->x;
	k64f b = center->y;
	k64f c = center->z;
	k64f r = 0;
	k64f Li = 0;
	k64f Li_ = 0;
	k64f La_ = 0;
	k64f Lb_ = 0;

	k64f Xsum = 0;
	k64f Xsumsq = 0;
	k64f Xsumcube = 0;
	k64f Ysum = 0;
	k64f Ysumsq = 0;
	k64f Ysumcube = 0;
	k64f Zsum = 0;
	k64f Zsumsq = 0;
	k64f Zsumcube = 0;

	k64f XYsum = 0;
	k64f XZsum = 0;
	k64f YZsum = 0;

	k64f X2Ysum = 0;
	k64f X2Zsum = 0;
	k64f Y2Xsum = 0;
	k64f Y2Zsum = 0;
	k64f Z2Xsum = 0;
	k64f Z2Ysum = 0;

	int n = 0;
	int N = 300;
	double Nstop = 1e-6;
	double A;
	double B;
	double C;
	double Rsq;

	kTry
	{
		// Start with a0 = _x, b0 = _y, c0 = _z
		// Then iterate 3 times (until goal reached)
		//     For all points, compute the values Li = sqrt((xi - a)2 + (yi - b)2 + (zi - c)2) and sum them in _L
		//     Also compute _La _Lb _Lc -> (a-xi)/Li
		//     a is now _x + _L_La, and so on
		//     r is now _Li

		pointCount = kArrayList_Count(pointList);
		if (pointCount >= 4)
		{
			pointPtr = kArrayList_Data(pointList);
			for (ix = 0; ix < pointCount; ++ix)
			{
				Xsum += pointPtr->x;
				Xsumsq += pointPtr->x * pointPtr->x;
				Xsumcube += pointPtr->x * pointPtr->x * pointPtr->x;

				Ysum += pointPtr->y;
				Ysumsq += pointPtr->y * pointPtr->y;
				Ysumcube += pointPtr->y * pointPtr->y * pointPtr->y;

				Zsum += pointPtr->z;
				Zsumsq += pointPtr->z * pointPtr->z;
				Zsumcube += pointPtr->z * pointPtr->z * pointPtr->z;

				XYsum += pointPtr->x * pointPtr->y;
				XZsum += pointPtr->x * pointPtr->z;
				YZsum += pointPtr->y * pointPtr->z;

				X2Ysum += pointPtr->x * pointPtr->x * pointPtr->y;
				X2Zsum += pointPtr->x * pointPtr->x * pointPtr->z;
				Y2Xsum += pointPtr->y * pointPtr->y * pointPtr->x;
				Y2Zsum += pointPtr->y * pointPtr->y * pointPtr->z;
				Z2Xsum += pointPtr->z * pointPtr->z * pointPtr->x;
				Z2Ysum += pointPtr->z * pointPtr->z * pointPtr->y;

				pointPtr++;
			}

		}


		{
			//http://imaginaryz.blogspot.ca/2011/04/least-squares-fit-sphere-to-3d-data.html
			//Least Squares Fit a sphere A,B,C with radius squared Rsq to 3D data
			//
			//    P is a structure that has been computed with the data earlier.
			//    pointCount is the number of elements; the length of X,Y,Z are identical.
			//    P's members are logically named.
			//
			//    X[n] is the x component of point n
			//    Y[n] is the y component of point n
			//    Z[n] is the z component of point n
			//
			//    A is the x coordiante of the sphere
			//    B is the y coordiante of the sphere
			//    C is the z coordiante of the sphere
			//    Rsq is the radius squared of the sphere.
			//
			//This method should converge; maybe 5-100 iterations or more.

			//Other method
			//http://www.sci.utah.edu/~balling/FEtools/doc_files/LeastSquaresFitting.pdf

			double A2;
			double B2;
			double C2;
			double QS;
			double QB;

			double Q0;
			double Q1;
			double Q2;

			double aA, aB, aC, nA, nB, nC, dA, dB, dC;


			double Xn = Xsum / pointCount;        //sum( X[n] )
			double Xn2 = Xsumsq / pointCount;    //sum( X[n]^2 )
			double Xn3 = Xsumcube / pointCount;    //sum( X[n]^3 )
			double Yn = Ysum / pointCount;        //sum( Y[n] )
			double Yn2 = Ysumsq / pointCount;    //sum( Y[n]^2 )
			double Yn3 = Ysumcube / pointCount;    //sum( Y[n]^3 )
			double Zn = Zsum / pointCount;        //sum( Z[n] )
			double Zn2 = Zsumsq / pointCount;    //sum( Z[n]^2 )
			double Zn3 = Zsumcube / pointCount;    //sum( Z[n]^3 )

			double XY = XYsum / pointCount;        //sum( X[n] * Y[n] )
			double XZ = XZsum / pointCount;        //sum( X[n] * Z[n] )
			double YZ = YZsum / pointCount;        //sum( Y[n] * Z[n] )
			double X2Y = X2Ysum / pointCount;    //sum( X[n]^2 * Y[n] )
			double X2Z = X2Zsum / pointCount;    //sum( X[n]^2 * Z[n] )
			double Y2X = Y2Xsum / pointCount;    //sum( Y[n]^2 * X[n] )
			double Y2Z = Y2Zsum / pointCount;    //sum( Y[n]^2 * Z[n] )
			double Z2X = Z2Xsum / pointCount;    //sum( Z[n]^2 * X[n] )
			double Z2Y = Z2Ysum / pointCount;    //sum( Z[n]^2 * Y[n] )

			//Reduction of multiplications
			double F0 = Xn2 + Yn2 + Zn2;
			double F1 = 0.5*F0;
			double F2 = -8.0*(Xn3 + Y2X + Z2X);
			double F3 = -8.0*(X2Y + Yn3 + Z2Y);
			double F4 = -8.0*(X2Z + Y2Z + Zn3);

			//Set initial conditions:
			A = Xn;
			B = Yn;
			C = Zn;

			//First iteration computation:
			A2 = A*A;
			B2 = B*B;
			C2 = C*C;
			QS = A2 + B2 + C2;
			QB = -2 * (A*Xn + B*Yn + C*Zn);

			//Set initial conditions:
			Rsq = F0 + QB + QS;

			//First iteration computation:
			Q0 = 0.5*(QS - Rsq);
			Q1 = F1 + Q0;
			Q2 = 8 * (QS - Rsq + QB + F0);

			//Iterate N times, ignore stop condition.
			while (n != N){
				n++;

				//Compute denominator:
				aA = Q2 + 16 * (A2 - 2 * A*Xn + Xn2);
				aB = Q2 + 16 * (B2 - 2 * B*Yn + Yn2);
				aC = Q2 + 16 * (C2 - 2 * C*Zn + Zn2);
				aA = (aA == 0) ? 1.0 : aA;
				aB = (aB == 0) ? 1.0 : aB;
				aC = (aC == 0) ? 1.0 : aC;

				//Compute next iteration
				nA = A - ((F2 + 16 * (B*XY + C*XZ + Xn*(-A2 - Q0) + A*(Xn2 + Q1 - C*Zn - B*Yn))) / aA);
				nB = B - ((F3 + 16 * (A*XY + C*YZ + Yn*(-B2 - Q0) + B*(Yn2 + Q1 - A*Xn - C*Zn))) / aB);
				nC = C - ((F4 + 16 * (A*XZ + B*YZ + Zn*(-C2 - Q0) + C*(Zn2 + Q1 - A*Xn - B*Yn))) / aC);

				//Check for stop condition
				dA = (nA - A);
				dB = (nB - B);
				dC = (nC - C);
				if ((dA*dA + dB*dB + dC*dC) <= Nstop)
				{
					break;
				}

				//Compute next iteration's values
				A = nA;
				B = nB;
				C = nC;
				A2 = A*A;
				B2 = B*B;
				C2 = C*C;
				QS = A2 + B2 + C2;
				QB = -2 * (A*Xn + B*Yn + C*Zn);
				Rsq = F0 + QB + QS;
				Q0 = 0.5*(QS - Rsq);
				Q1 = F1 + Q0;
				Q2 = 8 * (QS - Rsq + QB + F0);
			}
		}

		(*centerOut).x = A;
		(*centerOut).y = B;
		(*centerOut).z = C;
		*radiusOut = sqrt(Rsq);
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
GtsFx(kStatus) GdkSurface_SphereStdDev(kArrayList pointList, kPoint3d64f* center, k64f radius, k64f* stdev)
{
	kPoint3d64f* pointPtr = kNULL;
	kStatus exception = kOK;
	k32s ix;

	kPoint3d64f pointAccumulator = { 0, 0, 0 };
	k64s pointCount = 0;
	k64s pointListCount = 1;
	k64f pointAccTemp = 0;

	k64f Xsum = 0;

	double var;

	kTry
	{
		pointCount = kArrayList_Count(pointList);
		if (pointCount >= 4)
		{
			pointPtr = kArrayList_Data(pointList);
			for (ix = 0; ix < pointCount; ++ix)
			{
				// distance
				k64f distance = kS3D_MATH_DIST_3D(pointPtr, center);

				Xsum += (distance - radius) * (distance - radius);

				pointPtr++;
			}
		}

		var = Xsum / pointCount;
		*stdev = sqrt(var);
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

/**
* Signed distance from target point to surface plane
*
* @public					@memberof created by Chi Ho and Vincent
* @param	pointList		points to fit the plane
* @param	planeOut		fitted plane{p, norm}
* @param	averageHeight	average height of the points to original coordiante
* @return					signed distance
*/
kStatus GdkSurface_FitPlane(kArrayList pointList, kPlane3d64f* planeOut, k64f *averageHeight)
{
	kSize i = 0;
	kPoint3d64f* points = kArrayList_Data(pointList);

	*averageHeight = 0;
	for (i = 0; i < kArrayList_Count(pointList); ++i)
	{
		*averageHeight += points[i].z;
	}
	*averageHeight /= (k64f)kArrayList_Count(pointList);

	return kS3dPlane3d_FitPoint64f(points, (k32u)kArrayList_Count(pointList), planeOut);
}

/**
* Compute the distance from pointList to the plane.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	plane			surface plane
* @param	pointList		points to be calculated
* @param	averageHeight	mean height among points in pointList
* @param	medianHeight	median height among points in pointList
* @param	minHeight		minimum height among points in pointList
* @param	maxHeight		maximum height among points in pointList
* @param	stdev			standard deviation of all points to the plane
* @return					operation status.
*/
kStatus GdkSurface_ComputeHeightFromPlane_3D(kPlane3d64f* plane, kArrayList pointList, k64f *averageHeight, k64f *medianHeight, k64f *minHeight, k64f *maxHeight, k64f *stdev)
{
	kSize i = 0;
	GdkSurface_HeightPlane planeNew = GdkSurface_ToPlane3d(*plane);
	kPoint3d64f* point = (kPoint3d64f*)kArrayList_Data(pointList);
	kArray1 distanceArray;
	k64f* distancePtr;
	kSize pointCount;
	k64f distance = 0;

	pointCount = kArrayList_Count(pointList);
	kCheck(kArray1_Construct(&distanceArray, kTypeOf(k64f), kArrayList_Count(pointList), kNULL));
	distancePtr = kArray1_At(distanceArray, 0);
	*averageHeight = 0;
	*minHeight = k64F_MAX;
	*maxHeight = -k64F_MAX;
	for (i = 0; i < kArrayList_Count(pointList); ++i)
	{
		k64f height = GdkSurface_SignedDistance(planeNew, point[i]);
		distancePtr[i] = height;
		*averageHeight += height;
		*minHeight = kMin_(*minHeight, height);
		*maxHeight = kMax_(*maxHeight, height);
	}
	*averageHeight /= (k64f)kArrayList_Count(pointList);

	// Compute standard deviation
	kNullMath_Stdev64f(distancePtr, pointCount, stdev);

	// Extract median
	GdkSurface_Quicksort(distancePtr, 0, (int)(pointCount - 1));
	distance = distancePtr[pointCount / 2];
	*medianHeight = distance < 0 ? -distance : distance;

	return kOK;
}

/**
* Compute the distance from pointList to the plane.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	array			data source
* @param	firstIndex		start position
* @param	lastIndex		end position
* @return					void
*/
void GdkSurface_Quicksort(k64f *array, int firstIndex, int lastIndex)
{
	//declaring index variables
	int pivotIndex, index1, index2;
	k64f temp;

	if (firstIndex < lastIndex)
	{
		//Assigning first element index as pivot element
		pivotIndex = firstIndex;
		index1 = firstIndex;
		index2 = lastIndex;

		//Sorting in Ascending order with quick sort
		while (index1 < index2)
		{
			while (array[index1] <= array[pivotIndex] && index1 < lastIndex)
			{
				index1++;
			}
			while (array[index2]>array[pivotIndex])
			{
				index2--;
			}

			if (index1<index2)
			{
				//Swapping opertation
				temp = array[index1];
				array[index1] = array[index2];
				array[index2] = temp;
			}
		}

		//At the end of first iteration, swap pivot element with index2 element
		temp = array[pivotIndex];
		array[pivotIndex] = array[index2];
		array[index2] = temp;

		//Recursive call for quick sort, with partitioning
		GdkSurface_Quicksort(array, firstIndex, index2 - 1);
		GdkSurface_Quicksort(array, index2 + 1, lastIndex);
	}
}

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
k64f GdkSurface_ComputeVectorAngle_2D(k64f cx, k64f cy, k64f ex, k64f ey)
{
	k64f dy = ey - cy;
	k64f dx = ex - cx;
	k64f theta = atan2(dy, dx); // range (-PI, PI]
	theta *= 180 / M_PI; // rads to degs, range (-180, 180]
	if (theta < 0) theta = 360 + theta; // range [0, 360)
	return theta;
}

