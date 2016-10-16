#include "Surface_Geometry.h"

/**
* Get the squared distance between 2 points
*
* @public					@memberof created by Chi Ho and Vincent
* @param	point1			point1
* @param	point2			point2
* @return					Squared distance
*/
k64f GdkSurface_SqauredPointDistance_3D(kPoint3d64f point1, kPoint3d64f point2)
{
	return (point1.x - point2.x) * (point1.x - point2.x) +
		(point1.y - point2.y) * (point1.y - point2.y) +
		(point1.z - point2.z) * (point1.z - point2.z);
}

/**
* Get the dot product of 2 points
*
* @public					@memberof created by Chi Ho and Vincent
* @param	left			point1
* @param	Right			point2
* @return					result
*/
k64f GdkSurfaceSphereFit_DotP(kPoint3d64f Left, kPoint3d64f Right)
{
	return (Left.x * Right.x + Left.y * Right.y + Left.z * Right.z);
}

/**
* Get the cross product of 2 points
*
* @public					@memberof created by Chi Ho and Vincent
* @param	left			point1
* @param	Right			point2
* @return					result
*/
kPoint3d64f GdkSurfaceSphereFit_CrossP(kPoint3d64f Left, kPoint3d64f Right)
{
	kPoint3d64f Result;
	Result.x = Left.y * Right.z - Left.z * Right.y;
	Result.y = Left.z * Right.x - Left.x * Right.z;
	Result.z = Left.x * Right.y - Left.y * Right.x;
	return Result;
}

/**
* Get the unit vector of input
*
* @public					@memberof created by Chi Ho and Vincent
* @param	V				input vector
* @return					normalized vector
*/
kPoint3d64f GdkSurface_Normalize(kPoint3d64f V)
{
	kPoint3d64f u = V;
	kS3dMath_Normalize64f(&u);
	return u;
}