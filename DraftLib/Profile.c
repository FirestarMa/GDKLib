#include "Profile.h"

int Test(int a, int b)
{
	return (a + b);
}

void ProfileLineFit(ProfilePoint* points, unsigned int count, double* slope, double* intercept)
{
	double sumX = 0;
	double sumXSq = 0;
	double sumXZ = 0;
	double sumZ = 0;
	double delta;
	unsigned int i;
	unsigned int validCount = 0;

	for (i = 0; i < count; i++)
	{
		if (points[i].z != ALIGN_CAL_INVALID_RANGE_DOUBLE)
		{
			sumX += points[i].x;
			sumZ += points[i].z;
			sumXSq += (points[i].x * points[i].x);
			sumXZ += (points[i].x * points[i].z);
			validCount++;
		}
	}

	delta = validCount * sumXSq - sumX * sumX;

	if (delta != 0)
	{
		*slope = (validCount * sumXZ - sumX * sumZ) / delta;
		*intercept = (sumXSq * sumZ - sumX * sumXZ) / delta;
	}
	else
	{
		*slope = 0;
		*intercept = 0;
	}
}

void ProfileTransform(ProfilePoint* points, unsigned int count, Transform2d* transform, ProfilePoint* output)
{
	unsigned int i;

	for (i = 0; i < count; i++)
	{
		if (points[i].x != ALIGN_CAL_INVALID_RANGE_DOUBLE && points[i].z != ALIGN_CAL_INVALID_RANGE_DOUBLE)
		{
			Transform2d_Apply(transform, points[i].x, points[i].z, &output[i].x, &output[i].z);
		}
		else
		{
			output[i].x = output[i].z = ALIGN_CAL_INVALID_RANGE_DOUBLE;
		}
	}
}

void Transform2d_Identity(Transform2d* transform)
{
	transform->xx = 1;
	transform->xy = 0;
	transform->xt = 0;
	transform->yx = 0;
	transform->yy = 1;
	transform->yt = 0;
}

void Transform2d_Translate(const Transform2d* transform, double xOffset, double yOffset, Transform2d* result)
{
	*result = *transform;
	result->xt += xOffset;
	result->yt += yOffset;
}

void Transform2d_Rotate(const Transform2d* transform, double degrees, Transform2d* result)
{
	Transform2d rotation = { 0 };
	double rad = degrees * M_PI / 180;
	double angleSin = sin(rad);
	double angleCos = cos(rad);

	rotation.xx = angleCos;
	rotation.xy = -angleSin;
	rotation.yx = angleSin;
	rotation.yy = angleCos;

	Transform2d_Mul(transform, &rotation, result);
}

void Transform2d_Mul(const Transform2d* right, const Transform2d* left, Transform2d* result)
{
	Transform2d r = { 0 };

	r.xx = right->xx*left->xx + right->yx*left->xy;
	r.xy = right->xy*left->xx + right->yy*left->xy;
	r.xt = right->xt*left->xx + right->yt*left->xy + left->xt;

	r.yx = right->xx*left->yx + right->yx * left->yy;
	r.yy = right->xy*left->yx + right->yy * left->yy;
	r.yt = right->xt*left->yx + right->yt*left->yy + left->yt;

	*result = r;
}

void Transform2d_Apply(const Transform2d* transform, double x, double y, double* xOut, double* yOut)
{
	*xOut = transform->xx* x + transform->xy*y + transform->xt;
	*yOut = transform->yx* x + transform->yy*y + transform->yt;
}