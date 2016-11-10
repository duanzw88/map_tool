

#ifndef __PARSE_H__
#define __PARSE_H__

#include "jc_errors.h"
#include "cJSON.h"
#include "building.h"
#include "floor.h"
#include "layer.h"
#include "element.h"
#include "feature.h"
#include "item.h"
#include "item_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern building_t   parseOpenFile(const char *fileName);
extern void         parseBuilding(cJSON *jsonData,building_t build);
extern floor_t      parseFloor(cJSON *floorData);
extern layer_t      parseLayer(cJSON *layerData);
extern element_t    parseElement(cJSON *elementData);
extern feature_t    parseFeature(cJSON *featureData);
extern item_t       parseItem(cJSON *itemData,const char *geo_type);
extern itemLine_t   parseLine(cJSON *lineData);
extern itemRect_t   parseRect(cJSON *rectData);
extern itemCircle_t parseCircle(cJSON *circleData);
extern itemEllipse_t parseEllipse(cJSON *ellipseData);
extern itemPolyline_t parsePolyline(cJSON *polylinesData);
extern itemPolygon_t   parsePolygon(cJSON *polygonData);

extern itemPath_t   parsePath(cJSON *pathData);

#ifdef __cplusplus
}
#endif

#endif
