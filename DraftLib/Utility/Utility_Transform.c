#include "Utility_Transform.h"


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
kL3dTransform3d GdkUtilTransform_CreateTransformXYZ_2D(k64f angle, k64f x, k64f y, k64f z)
{
	kL3dTransform3d transformIdentity;
	kL3dTransform3d transformTmp;
	kL3dTransform3d transformTmp2;
	kL3dTransform3d transform;
	kL3dTransform3d_Identity(&transformIdentity);
	kL3dTransform3d_Translate(&transformIdentity, x, y, z, &transformTmp);
	kL3dTransform3d_Rotate(&transformIdentity, 0, 0, 1, angle, &transformTmp2);
	kL3dTransform3d_Mul(&transformTmp2, &transformTmp, &transform);
	return transform;
}

/**
* Create transformation matrix give rotation angle and x, y z. NOTE: rotation is only in XY plane
*
* @public					@memberof created by Chi Ho and Vincent
* @param	angle			rotation angle in XY plane
* @param	translation		X,Y,Z
* @return					transformation matirx
*/
kL3dTransform3d GdkUtilTransform_CreateTransform_2D(k64f angle, kPoint3d64f translation)
{
	return GdkUtilTransform_CreateTransformXYZ_2D(angle, translation.x, translation.y, translation.z);
}

/**
* Create inverse transformation matrix give rotation angle and x, y z. NOTE: rotation is only in XY plane
*
* @public					@memberof created by Chi Ho and Vincent
* @param	angle			rotation angle in XY plane
* @param	translation		X,Y,Z
* @return					transformation matirx
*/
kL3dTransform3d GdkUtilSurface_CreateInverseTransform(k64f angle, kPoint3d64f translation)
{
	kL3dTransform3d transformIdentity;
	kL3dTransform3d transformTmp;
	kL3dTransform3d transformTmp2;
	kL3dTransform3d transform;
	kL3dTransform3d_Identity(&transformIdentity);
	kL3dTransform3d_Translate(&transformIdentity, -translation.x, -translation.y, -translation.z, &transformTmp);
	kL3dTransform3d_Rotate(&transformIdentity, 0, 0, 1, -angle, &transformTmp2);
	kL3dTransform3d_Mul(&transformTmp, &transformTmp2, &transform);
	return transform;
}