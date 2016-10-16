#include "..\TypeDef.h"
//#include "..\Surface.h"

/**
* Add points to graphics with specified format
*
* @public					@memberof created by Chi Ho and Vincent
* @param	graphic			graphic object to operate on
* @param	pointSize		size of the point to be drawn
* @param	shape			shape of points
* @param	color			color of points
* @param	points			points to be added to graphics
* @return					Operation status
*/
kStatus GdkUtilGraphics_AddPointsToGraphics(GdkGraphic graphic, k32f pointSize, kMarkerShape shape, kColor color, kArrayList points);

/**
* To convert point from k64f to k32f. Only to make points addable to graphics. Internal use only.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	pointList		point list to convert
* @return					Converted points for output
*/
kArrayList GdkUtilGraphics_ConvertPoints64ToPoints32(kArrayList pointList);

/**
* Add lines to graphics with specified format
*
* @public					@memberof created by Chi Ho and Vincent
* @param	graphic			graphic object to operate on
* @param	width			width of the line
* @param	color			color of points
* @param	points			points to be added to graphics
* @return					Operation status
*/
kStatus GdkUtilGraphics_AddGraphicLines(GdkGraphic graphic, k32f width, kColor color, kArrayList points);

/**
* Add surface to graphic with specified format
*
* @public					@memberof created by Chi Ho and Vincent
* @param	graphic			graphic object to operate on
* @param	width			width of the line
* @param	color			color of points
* @param	region			plane region that is visible
* @param	plane			plane to be added
* @return					Operation status
*/
kStatus GdkUtilGraphics_AddGraphicPlane(GdkGraphic graphic, k32f width, kColor color, GdkRegion3d64f *region, kPlane3d64f plane);

/**
* Get the vertical intersection. Internal use only.
*
* @public					@memberof created by Chi Ho and Vincent
* @param	plane			plane to be added
* @param	pt				point to receive value
* @return					Operation status
*/
void GdkUtilGraphics_PlaneVerticalIntersect32(kPlane3d64f plane, kPoint3d32f* pt);

/**
* Add lines that describes a circle to graphics with specified format
*
* @public					@memberof created by Chi Ho and Vincent
* @param	graphic			graphic object to operate on
* @param	center			center of the circle
* @param	radius			radius of the circle
* @param	stepAngle		step angle for line creation
* @param	width			width of the line
* @param	color			color of points
* @return					Operation status
*/
kStatus GdkUtilGraphics_AddGraphicCircle(GdkGraphic graphic, kPoint3d64f center, k64f radius, k64f stepAngle, k32f width, kColor color);