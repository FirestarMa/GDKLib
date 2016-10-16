#include "..\Def.h"

/**
* Create transformation matrix give rotation angle and x, y z. NOTE: rotation is only in XY plane
*
* @public					@memberof created by Chi Ho and Vincent
* @param	angle			rotation angle in XY plane
* @param	x				translation X
* @param	y				translation Y
* @param	z				translation Z
* @return					transformation matirx
*/
kL3dTransform3d GdkUtilTransform_CreateTransformXYZ_2D(k64f angle, k64f x, k64f y, k64f z);

/**
* Create transformation matrix give rotation angle and x, y z. NOTE: rotation is only in XY plane
*
* @public					@memberof created by Chi Ho and Vincent
* @param	angle			rotation angle in XY plane
* @param	translation		X,Y,Z
* @return					transformation matirx
*/
kL3dTransform3d GdkUtilTransform_CreateTransform_2D(k64f angle, kPoint3d64f translation);

/**
* Create inverse transformation matrix give rotation angle and x, y z. NOTE: rotation is only in XY plane
*
* @public					@memberof created by Chi Ho and Vincent
* @param	angle			rotation angle in XY plane
* @param	translation		X,Y,Z
* @return					transformation matirx
*/
kL3dTransform3d GdkUtilSurface_CreateInverseTransform(k64f angle, kPoint3d64f translation);

