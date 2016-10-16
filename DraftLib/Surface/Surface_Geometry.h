#include "..\TypeDef.h"

/**
* Get the squared distance between 2 points
*
* @public					@memberof created by Chi Ho and Vincent
* @param	point1			point1
* @param	point2			point2
* @return					Squared distance
*/
k64f GdkSurface_SqauredPointDistance_3D(kPoint3d64f point1, kPoint3d64f point2);

/**
* Get the unit vector of input
*
* @public					@memberof created by Chi Ho and Vincent
* @param	V				input vector
* @return					normalized vector
*/
kPoint3d64f GdkSurface_Normalize(kPoint3d64f V);