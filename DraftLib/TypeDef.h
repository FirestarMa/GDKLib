#include "Def.h"

#ifndef TYPE_DEF
#define TYPE_DEF

#define REGION_EXPANSION_INCREMENT 0.01
#define GDK_SURFACE__HEIGHT_EDGE_REGION_CAPACITY (2)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
/**
* @struct  GdkSurface_EdgeLine
* @extends various
* @ingroup GDKLib
* @brief   EdgeLine related parameters
*
* Describe information the is returned by edge detection algorithm
*/
typedef  struct GdkSurface_EdgeLine
{
	kArrayList edgePoints; // points used to calculate edges
	kArrayList linePoints; // points used to define a line (two points)
	kBool lineOk;  // Specify if a line is okay
	kArrayList pointToFit;
	k64f slope;
	k64f intercept;
} GdkSurface_EdgeLine;

/**
* @struct  GdkSurface_EdgeDirection
* @extends k8u
* @ingroup GDKLib
* @brief   Edge directions
*
* Describe edge direction settings
*/
typedef enum GdkSurface_EdgeDirection
{
	GDK_SURFACE_EDGE_DIRECTION_ANY = 0,
	GDK_SURFACE_EDGE_DIRECTION_RISING = 1,
	GDK_SURFACE_EDGE_DIRECTION_FALLING = 2,
	GDK_SURFACE_EDGE_DIRECTION_RISING_FROM_ZERO = 3,
	GDK_SURFACE_EDGE_DIRECTION_FALLING_TO_ZERO = 4,
	GDK_SURFACE_EDGE_DIRECTION_AVERAGE_POINT = 5
} GdkSurface_EdgeType;

/**
* @struct  GdkSurface_EdgeFilter
* @extends k8u
* @ingroup GDKLib
* @brief   Edge filters
*
* Describe edge selection options
*/
typedef enum GdkSurface_EdgeFilter
{
	GDK_SURFACE_EDGE_FILTER_PEAK = 0,
	GDK_SURFACE_EDGE_FILTER_FIRST = 1,
	GDK_SURFACE_EDGE_FILTER_LAST = 2
} GdkSurface_EdgeFilter;

/**
* @struct  GdkSurface_EdgeSettings
* @extends Various
* @ingroup GDKLib
* @brief   Edge settings
*
* Describe edge related settings returned from GUI
*/
typedef struct GdkSurface_EdgeSettings
{
	k32u regionCount;
	GdkRegion3d64f region[GDK_SURFACE__HEIGHT_EDGE_REGION_CAPACITY];
	k64f edgeAngle;
	kBool fixedAngle;
	kBool bestFit;
	k64f lowerThreshold;
	k64f higherThreshold;
	k32u kernelSize;
	k32u dataType;
	GdkSurface_EdgeType direction;
	GdkSurface_EdgeFilter filter;
} GdkSurface_EdgeSettings;

/**
* @struct  GdkSurface_HeightPlane
* @extends k64f
* @ingroup GDKLib
* @brief   Ax + By + Cz + D = 0
*
* For the convinience of plane related calculation
*/
typedef struct GdkSurface_HeightPlane
{
	k64f a, b, c, d;
}GdkSurface_HeightPlane;

/**
* @struct  2 vectors that defines a plane
* @extends kPoint3d64f
* @ingroup GDKLib
* @brief   p defines the position; norm is the normal vector
*
* For the convinience of plane expression, same as kPlane3d64f in kS3dCommon.h
*/
typedef struct GdkSurface_Plane3d64f
{
	kPoint3d64f p;
	kPoint3d64f norm;
} GdkSurface_Plane3d64f;

#endif

