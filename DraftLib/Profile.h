#define _USE_MATH_DEFINES
#define M_ABS(A)		(((A) >= 0) ? (A) : -(A))
#include <math.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define ALIGN_CAL_INVALID_RANGE_16BIT		(-32768)					// gocator transmits range data as 16-bit signed integers. 0x8000 signifies invalid range data.	
#define ALIGN_CAL_DOUBLE_MAX				(1.7976931348623157e+308)	// 64-bit double - largest positive value.	
#define ALIGN_CAL_INVALID_RANGE_DOUBLE		(-ALIGN_CAL_DOUBLE_MAX)		// floating point value to represent invalid range data.	

typedef struct
{
	double x;	// x-coordinate in engineering units (mm) - position along laser line
	double z;	// z-coordinate in engineering units (mm) - height (at the given x position)
}ProfilePoint;

typedef struct
{
	double xx;  // 2D transformation matrix
	double xy;  //
	double xt;  // [ xx xy xt ]
	double yx;  // [ yx yy yt ]
	double yy;  // [ 0  0  1  ]
	double yt;
} Transform2d;

__declspec(dllexport) void ProfileLineFit(ProfilePoint* points, unsigned int count, double* slope, double* intercept);
__declspec(dllexport) void ProfileTransform(ProfilePoint* points, unsigned int count, Transform2d* transform, ProfilePoint* output);
__declspec(dllexport) void Transform2d_Identity(Transform2d* transform);
__declspec(dllexport) void Transform2d_Translate(const Transform2d* transform, double xOffset, double yOffset, Transform2d* result);
__declspec(dllexport) void Transform2d_Rotate(const Transform2d* transform, double degrees, Transform2d* result);
__declspec(dllexport) void Transform2d_Mul(const Transform2d* right, const Transform2d* left, Transform2d* result);
__declspec(dllexport) void Transform2d_Apply(const Transform2d* transform, double x, double y, double* xOut, double* yOut);

__declspec(dllexport) int Test(int a, int b);

#ifdef __cplusplus
}
#endif