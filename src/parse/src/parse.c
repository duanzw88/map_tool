#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "cJSON.h"

#include "parse.h"


building_t parseOpenFile(const char *fileName)
{
    // printf("parseOpenFile...\n");
    FILE *file;
    char *data;
    long len;
    cJSON *jsonData;
    building_t build;
    //
    file = fopen(fileName,"rb");
    fseek(file,0,SEEK_END);
    len = ftell(file);
    fseek(file,0,SEEK_SET);
    data = ALLOC(len+1);
    fread(data,1,len,file);
    jsonData = cJSON_Parse(data);
    char *name = cJSON_GetObjectItem(jsonData,"name")->valuestring;
    build = building_new(name);
    parseBuilding(jsonData,build);
    FREE(data);
    // printf("parseOpenFile...end");
    return build;
}
void  parseBuilding(cJSON *jsonData,building_t build)
{
    // printf("parseData.....\n");
    floor_t floor;
    cJSON *floorsJson = cJSON_GetObjectItem(jsonData,"floors");
    for(int i = 0;i < cJSON_GetArraySize(floorsJson); i++)
    {
        cJSON *floorJSON = cJSON_GetArrayItem(floorsJson,i);
        floor = parseFloor(floorJSON);
        building_add_floor(build,floor);
    }
}

floor_t parseFloor(cJSON *floorData)
{
    // printf("parseFloor....\n");
    floor_t floor;
    layer_t layer;
    char *name = cJSON_GetObjectItem(floorData,"name")->valuestring;
    float width = cJSON_GetObjectItem(floorData,"width")->valuedouble;
    float height = cJSON_GetObjectItem(floorData,"height")->valuedouble;
    floor = floor_new_by_attribute(name,width,height);
    cJSON *layers = cJSON_GetObjectItem(floorData,"layers");
    for(int l = 0;l < cJSON_GetArraySize(layers);l++)
    {
        cJSON *layerData = cJSON_GetArrayItem(layers,l);
        layer = parseLayer(layerData);
        floor_add_layer(floor,layer);
    }
    //
    return floor;
}

layer_t parseLayer(cJSON *layerData)
{
    element_t element;

    char *layer_name = cJSON_GetObjectItem(layerData,"name")->valuestring;
    layer_t layer = layer_new(layer_name);

    cJSON *elements = cJSON_GetObjectItem(layerData,"feature_collections");
    for(int i = 0; i < cJSON_GetArraySize(elements); i++)
    {
        cJSON *elementData = cJSON_GetArrayItem(elements,i);
        element = parseElement(elementData);
        layer_add_element(layer,element);
    }
    return layer;
}

element_t    parseElement(cJSON *elementData)
{
    feature_t feature;
    // printf("parseElement\n");
    char *type = cJSON_GetObjectItem(elementData,"type")->valuestring;
    char *name = cJSON_GetObjectItem(elementData,"name")->valuestring;
    element_t element = element_new(type,name);

    cJSON *features = cJSON_GetObjectItem(elementData,"features");
    for(int i = 0; i < cJSON_GetArraySize(features); i++)
    {
        cJSON *featureData = cJSON_GetArrayItem(features,i);
        feature = parseFeature(featureData);
        element_add_features(element,feature);
    }

    return element;
}
feature_t    parseFeature(cJSON *featureData)
{
    // printf("parseFeature...\n");
    color_t color;
    feature_t feature = feature_new();
    item_t item;
    // printf("%s\n",cJSON_Print(featureData));
    char *type = cJSON_GetObjectItem(featureData,"type")->valuestring;
    char *geo_type = cJSON_GetObjectItem(featureData,"geoType")->valuestring;
    feature_set_type(feature,type);
    feature_set_geotype(feature,geo_type);

    cJSON *itemsData = cJSON_GetObjectItem(featureData,"coords");
    item = parseItem(itemsData,geo_type);
    feature_add_item(feature,item);
    // for(int i = 0; i < cJSON_GetArraySize(itemsData); i++)
    // {
    //     cJSON *itemData = cJSON_GetArrayItem(itemsData,i);
    //     item = parseItem(itemData,geo_type);
    //     feature_add_item(feature,item);
    // }
    //

    cJSON *styleData = cJSON_GetObjectItem(featureData,"style");
    cJSON *fillData = cJSON_GetObjectItem(styleData,"fill");
    cJSON *strokeData = cJSON_GetObjectItem(styleData,"stroke");
    cJSON *fillColorData = cJSON_GetObjectItem(fillData,"color");
    int r = cJSON_GetObjectItem(fillColorData,"r")->valueint;
    int g = cJSON_GetObjectItem(fillColorData,"g")->valueint;
    int b = cJSON_GetObjectItem(fillColorData,"b")->valueint;
    float a = cJSON_GetObjectItem(fillColorData,"a")->valuedouble;
    color = color_new(r,g,b,a);
    feature_set_fill_color(feature,color);
    //
    cJSON *strokeColorData = cJSON_GetObjectItem(strokeData,"color");
    r = cJSON_GetObjectItem(strokeColorData,"r")->valueint;
    g = cJSON_GetObjectItem(strokeColorData,"g")->valueint;
    b = cJSON_GetObjectItem(strokeColorData,"b")->valueint;
    a = cJSON_GetObjectItem(strokeColorData,"a")->valuedouble;
    color = color_new(r,g,b,a);
    feature_set_stroke_color(feature,color);
    //
    int width = cJSON_GetObjectItem(strokeData,"width")->valueint;
    feature_set_stroke_width(feature,width);


    return feature;
}

item_t  parseItem(cJSON *itemData,const char *geo_type)
{
    item_t item;
    if(strcmp(geo_type,"LineString") == 0)
    {
        item = item_new(ITEM_KIND_POLYLINE);
        item_set_polyline(item,parsePolyline(itemData));
    }
    else if(strcmp(geo_type,"Polygon") == 0)
    {
        item = item_new(ITEM_KIND_POLYGON);
        item_set_polygon(item,parsePolygon(itemData));
    }
    else if(strcmp(geo_type,"Path") == 0)
    {
        item = item_new(ITEM_KIND_PATH);
        itemPath_t item_path = parsePath(itemData);
        item_set_path(item,item_path);
    }

    return item;
}

itemLine_t   parseLine(cJSON *lineData)
{
    return NULL;
}
itemRect_t   parseRect(cJSON *rectData)
{
    return NULL;
}
itemCircle_t parseCircle(cJSON *circleData)
{
    return NULL;
}
itemEllipse_t parseEllipse(cJSON *ellipseData)
{
    return NULL;
}
itemPolygon_t   parsePolygon(cJSON *polygonData)
{
    itemPolygon_t item_polygon = itemPolygon_new();
    itemPoint_t item_point;
    for(int i = 0;i < cJSON_GetArraySize(polygonData);i++)
    {
        cJSON *polygon = cJSON_GetArrayItem(polygonData,i);
        float x = cJSON_GetArrayItem(polygon,0)->valuedouble;
        float y = cJSON_GetArrayItem(polygon,1)->valuedouble;
        item_point = itemPoint_new(x,y);
        itemPolygon_add_point(item_polygon,item_point);
    }
    return item_polygon;
}
itemPolyline_t parsePolyline(cJSON *polylinesData)
{
    itemPolyline_t item_polyline = itemPolyline_new();
    itemPoint_t item_point;
    for(int i = 0; i < cJSON_GetArraySize(polylinesData); i++)
    {
        cJSON *polylineData = cJSON_GetArrayItem(polylinesData,i);
        float x = cJSON_GetArrayItem(polylineData,0)->valuedouble;
        float y = cJSON_GetArrayItem(polylineData,1)->valuedouble;
        // printf("x = %.2f y = %.2f\n",x,y);
        item_point = itemPoint_new(x,y);
        itemPolyline_add_point(item_polyline,item_point);
    }
    return item_polyline;
}
itemPath_t   parsePath(cJSON *pathsData)
{
    itemPath_t item_path = itemPath_new();
    for(int i = 0;i < cJSON_GetArraySize(pathsData); i++)
    {
        cJSON *pathData = cJSON_GetArrayItem(pathsData,i);
        char *cmd = cJSON_GetObjectItem(pathData,"cmd")->valuestring;
        // printf("cmd = %s\t",cmd);
        if(strcmp(cmd,"M") == 0)
        {
            // printf("Move...\n");
            float x = cJSON_GetObjectItem(pathData,"x")->valuedouble;
            float y = cJSON_GetObjectItem(pathData,"y")->valuedouble;
            itemPathCommand_t path_command_moveto = itemPathCommand_new(ITEM_PATH_CMD_ID_MOVETO,x,y);
            itemPath_add_pathcommand(item_path,path_command_moveto);
        }
        else if(strcmp(cmd,"L") == 0 || strcmp(cmd,"l") == 0 || strcmp(cmd,"H") == 0 || strcmp(cmd,"h") == 0 || strcmp(cmd,"V") == 0 || strcmp(cmd,"v") == 0)
        {
            // printf("line...\n");
            float x = cJSON_GetObjectItem(pathData,"x")->valuedouble;
            float y = cJSON_GetObjectItem(pathData,"y")->valuedouble;
            itemPathCommand_t path_command_lineto = itemPathCommand_new(ITEM_PATH_CMD_ID_LINETO,x,y);
            itemPath_add_pathcommand(item_path,path_command_lineto);
        }
        else if(strcmp(cmd,"C") == 0 || strcmp(cmd,"c") == 0)
        {
            // printf("cruve...\n");
            float x1 = cJSON_GetObjectItem(pathData,"x1")->valuedouble;
            float y1 = cJSON_GetObjectItem(pathData,"y1")->valuedouble;
            float x2 = cJSON_GetObjectItem(pathData,"x2")->valuedouble;
            float y2 = cJSON_GetObjectItem(pathData,"y2")->valuedouble;
            float x = cJSON_GetObjectItem(pathData,"x")->valuedouble;
            float y = cJSON_GetObjectItem(pathData,"x")->valuedouble;
            itemPathCommand_t path_command_curveto = itemPathCommand_new(ITEM_PATH_CMD_ID_CUBIC_CURVETO,x1,y1,x2,y2,x,y);
            itemPath_add_pathcommand(item_path,path_command_curveto);
        }
        else if(strcmp(cmd,"S") == 0 || strcmp(cmd,"s") == 0)
        {
            // printf("smooth curve...\n");
            float x2 = cJSON_GetObjectItem(pathData,"x2")->valuedouble;
            float y2 = cJSON_GetObjectItem(pathData,"y2")->valuedouble;
            float x = cJSON_GetObjectItem(pathData,"x")->valuedouble;
            float y = cJSON_GetObjectItem(pathData,"x")->valuedouble;
            itemPathCommand_t path_command_smooth_curveto = itemPathCommand_new(ITEM_PATH_CMD_ID_SMOOTH_CUBIC_CURVETO,x2,y2,x,y);
            itemPath_add_pathcommand(item_path,path_command_smooth_curveto);
        }
        else if(strcmp(cmd,"Q") == 0 || strcmp(cmd,"q") == 0)
        {
            // printf("quadratic...\n");
            itemPathCommand_t path_command_quadraticto = itemPathCommand_new(ITEM_PATH_CMD_ID_QUADRATIC_CURVETO);
            itemPath_add_pathcommand(item_path,path_command_quadraticto);
        }
        else if(strcmp(cmd,"T") == 0 || strcmp(cmd,"t") == 0)
        {
            // printf("smooth quadratic...\n");
            itemPathCommand_t path_command_smooth_quadraticto = itemPathCommand_new(ITEM_PATH_CMD_ID_SMOOTH_QUADRATIC_CURVETO);
            itemPath_add_pathcommand(item_path,path_command_smooth_quadraticto);
        }
        else if(strcmp(cmd,"A") == 0 || strcmp(cmd,"a") == 0)
        {
            // printf("arc...\n");
            float rx = cJSON_GetObjectItem(pathData,"rx")->valuedouble;
            float ry = cJSON_GetObjectItem(pathData,"ry")->valuedouble;
            float x_axis = cJSON_GetObjectItem(pathData,"xAxisRotation")->valuedouble;
            float larg_arc = cJSON_GetObjectItem(pathData,"largeArcFlag")->valuedouble;
            float sweep = cJSON_GetObjectItem(pathData,"sweepFlag")->valuedouble;
            float x = cJSON_GetObjectItem(pathData,"x")->valuedouble;
            float y = cJSON_GetObjectItem(pathData,"x")->valuedouble;
            itemPathCommand_t path_command_arcto = itemPathCommand_new(ITEM_PATH_CMD_ID_ARCTO,rx,ry,x_axis,larg_arc,sweep,x,y);
            itemPath_add_pathcommand(item_path,path_command_arcto);
        }
        else if(strcmp(cmd,"Z") == 0 || strcmp(cmd,"z") == 0)
        {
            // printf("close...\n");
            float x = cJSON_GetObjectItem(pathData,"x")->valuedouble;
            float y = cJSON_GetObjectItem(pathData,"y")->valuedouble;
            itemPathCommand_t path_command_lineto = itemPathCommand_new(ITEM_PATH_CMD_ID_END,x,y);
            itemPath_add_pathcommand(item_path,path_command_lineto);
        }
    }

    return item_path;
}
