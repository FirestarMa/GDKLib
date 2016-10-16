#include "GDKUtility.h"


/**
* Gets the region position in the predefined coordinate system. This should be called before region based data point operation.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	item			input data item
* @param	region			pointer to receive the region
* @return					operation status.
*/
GtsFx(kStatus) GdkSurface_GetRegionInCoordinate_3D(GdkInputItem item, GdkRegion3d64f *region)
{
	GdkDataInfo itemInfo = GdkInputItem_Info(item);
	region->x = GdkInputItem_Offset(item)->x;
	region->width = GdkSurfaceInput_ColumnCount(item) * GdkDataInfo_Scale(itemInfo)->x;
	region->y = GdkInputItem_Offset(item)->y;
	region->length = GdkSurfaceInput_RowCount(item) * GdkDataInfo_Scale(itemInfo)->y;
	region->z = GdkDataInfo_Region(itemInfo)->z;
	region->height = GdkDataInfo_Region(itemInfo)->height;
	return kOK;
}

