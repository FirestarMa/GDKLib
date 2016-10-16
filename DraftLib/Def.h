#ifndef TOOL_DEF_H
#define TOOL_DEF_H

#include <kApi/kApi.h>
#include <kVision/Vs/kVsUtilities.h>
#include <kVision\L3d\kL3dTransform3d.h>
#include <Gdk/Tools/GdkTool.h>
#include <kVision/S3d/kS3dUtilities.h>
#include <kVision\Vs\kVsUtilities.h>
#include <kApi\kApiCfg.h>
#include <kApi\kApiDef.x.h>
#include <kVision/Common/kNullMath.h>
#include <math.h>

#define GDK_SURFACE_HEIGHT_DATA_TYPE_HEIGHT              (0)         ///< Height image
#define GDK_SURFACE_HEIGHT_DATA_TYPE_INTENSITY           (1)         ///< Intensity image

kBeginHeader()

#if defined(TOOL_EMIT)
#    define ToolFx(TYPE)            kInFx(TYPE)
#    define ToolDx(TYPE)            kInDx(TYPE)
#else
#    define ToolFx(TYPE)            kInFx(TYPE)        
#    define ToolDx(TYPE)            kInDx(TYPE)     
#    define GtsFx(TYPE)             kInFx(TYPE)  // temporary workaround      
#    define GtsDx(TYPE)             kInDx(TYPE)  
#endif

kEndHeader()

#endif
