#include "Surface_Image.h"

/**
* Rotate the kImage object in 180 degrees
*
* @public					@memberof created by Chi Ho and Vincent
* @param	input			input data source
* @param	width			width of the input
* @param	length			length of the input
* @return					Distance to line.
*/
GtsFx(kStatus) GdkSurface_RotateHeightMap180_2D(kImage input, k32u width, k32u length)
{
	kImage tmpImage = kNULL;
	k16s* inRow;
	k16s* outRow;
	kSize x, y;
	kImage_Construct(&tmpImage, kTypeOf(k16s), width, length, kNULL);
	// Strict copy
	for (y = 0; y < length; ++y)
	{
		inRow = (k16s*)kImage_RowAt(input, y);
		outRow = (k16s*)kImage_RowAt(tmpImage, y);
		memcpy(outRow, inRow, sizeof(k16s) * width);
	}
	// Rotate 180
	for (y = 0; y < length; ++y)
	{
		outRow = (k16s*)kImage_RowAt(input, y);
		inRow = (k16s*)kImage_RowAt(tmpImage, length - y - 1);
		for (x = 0; x < width; ++x)
		{
			outRow[x] = inRow[width - x - 1];
		}
	}
	kDestroyRef(&tmpImage);
	return kOK;
}