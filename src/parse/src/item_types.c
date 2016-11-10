
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "mem.h"
#include "seq.h"
#include "assert.h"
#include "item_types.h"


#define T_Point     itemPoint_t
#define T_Rect      itemRect_t
#define T_Line      itemLine_t
#define T_Circle    itemCircle_t
#define T_Ellipse   itemEllipse_t
#define T_Polyline  itemPolyline_t
#define T_Polygon   itemPolygon_t
#define T_Path      itemPath_t
#define T_Pathcommand itemPathCommand_t

struct T_Point
{
    float x;
    float y;
};
struct T_Line
{
    float x1;
    float y1;
    float x2;
    float y2;
};
struct T_Rect
{
    float x;
    float y;
    float width;
    float height;
    float rx;
    float ry;
};
struct T_Circle
{
    float x;
    float y;
    float radius;
};
struct T_Ellipse
{
    float x;
    float y;
    float rx;
    float ry;
};

struct T_Polyline
{
    seq_t point_seq;
};
struct T_Polygon
{
    seq_t point_seq;
};

struct T_Path
{
    seq_t pathCommand_seq;
};

struct T_Pathcommand
{
    itemPathCommandId pathId;
    union
    {
        itemPathCommand_Moveto              tMoveto;
        itemPathCommand_Lineto              tLineto;
        itemPathCommand_CubicCruveto        tCubicCurveto;
        itemPathCommand_SmoothCubicCurveto  tSmoothCubicCurveto;
        itemPathCommand_QuadraticCurveto    tQuadraticCurveto;
        itemPathCommand_SmoothQuadraticCurveto tSmoothQuadraticCurveto;
        itemPathCommand_Arcto               tArcto;
    }pathStyle;
};


T_Point     itemPoint_new(float x,float y)
{
    T_Point point;
    NEW0(point);

    point->x = x;
    point->y = y;
    return point;
}
float itemPoint_get_x(T_Point point)
{
    assert(point);
    return point->x;
}
float itemPoint_get_y(T_Point point)
{
    assert(point);
    return point->y;
}
T_Rect itemRect_new(float x,float y,float width,float height,float rx,float ry)
{
    T_Rect rect;
    NEW0(rect);

    rect->x = x;
    rect->y = y;
    rect->width = width;
    rect->height = height;
    rect->rx = rx;
    rect->ry = ry;
    return rect;
}
T_Line      itemLine_new(float x1,float y1,float x2,float y2)
{
    T_Line line;
    NEW0(line);

    line->x1 = x1;
    line->y1 = y1;
    line->x2 = x2;
    line->y2 = y2;
    return line;
}
T_Circle itemCircle_new(float x,float y,float radius)
{
    T_Circle circle;
    NEW0(circle);

    circle->x = x;
    circle->y = y;
    circle->radius = radius;
    return circle;
}
T_Ellipse   itemEllipse_new(float x,float y,float rx,float ry)
{
    T_Ellipse ellipse;
    NEW0(ellipse);

    ellipse->x = x;
    ellipse->y = y;
    ellipse->rx = rx;
    ellipse->ry = ry;
    return ellipse;
}
T_Polyline  itemPolyline_new()
{
    T_Polyline polyline;
    NEW0(polyline);

    polyline->point_seq = seq_new(2);
    return polyline;
}
void itemPolyline_add_point(T_Polyline polyline,itemPoint_t point)
{
    assert(polyline);
    assert(point);
    seq_addend(polyline->point_seq,point);
}
int itemPolyline_get_point_count(T_Polyline polyline)
{
    assert(polyline);
    return seq_length(polyline->point_seq);
}
itemPoint_t   itemPolyline_get_point_by_index(T_Polyline polyline,int index)
{
    assert(polyline);
    assert(index >= 0 && index < seq_length(polyline->point_seq));

    return seq_get(polyline->point_seq,index);
}
T_Polygon   itemPolygon_new()
{
    T_Polygon polygon;
    NEW0(polygon);

    polygon->point_seq = seq_new(2);
    return polygon;
}
void itemPolygon_add_point(T_Polygon polygon,itemPoint_t point)
{
    assert(polygon);
    assert(point);
    // printf("polygon_add_point (%.2f,%.2f)\n",itemPoint_get_x(point),itemPoint_get_y(point));
    seq_addend(polygon->point_seq,point);
}
int itemPolygon_get_point_count(T_Polygon polygon)
{
    assert(polygon);
    return seq_length(polygon->point_seq);
}
itemPoint_t  itemPolygon_get_point_by_index(T_Polygon polygon,int index)
{
    assert(polygon);
    assert(index >= 0 && index < seq_length(polygon->point_seq));

    return seq_get(polygon->point_seq,index);
}

T_Pathcommand itemPathCommand_new(int pathId, ...)
{
    va_list var;
    T_Pathcommand command;
    NEW0(command);
    float x = 0.0,y = 0.0,x1 = 0.0, y1 = 0.0,x2 = 0.0,y2 = 0.0;
    command->pathId = pathId;
    // printf("pathId = %d\n",pathId);
    va_start(var,pathId);
    switch (pathId)
    {
        case ITEM_PATH_CMD_ID_MOVETO:
            //移动Move
            command->pathStyle.tMoveto = CALLOC(1,sizeof(struct _itemPathCommane_Moveto));
            x = va_arg(var,double);
            y = va_arg(var,double);
            command->pathStyle.tMoveto->x = x;
            command->pathStyle.tMoveto->y = y;
            break;
    	case ITEM_PATH_CMD_ID_LINETO:
            command->pathStyle.tLineto = CALLOC(1,sizeof( struct _itemPathCommand_Lineto));
            x = va_arg(var,double);
            y = va_arg(var,double);
            command->pathStyle.tLineto->x = x;
            command->pathStyle.tLineto->y = y;
            break;
    	case ITEM_PATH_CMD_ID_CUBIC_CURVETO:
            command->pathStyle.tCubicCurveto = CALLOC(1,sizeof(struct _itemPathCommand_CubicCruveto));
            x1 = va_arg(var,double);
            y1 = va_arg(var,double);
            x2 = va_arg(var,double);
            y2 = va_arg(var,double);
            x = va_arg(var,double);
            y = va_arg(var,double);
            command->pathStyle.tCubicCurveto->x1 = x1;
            command->pathStyle.tCubicCurveto->y1 = y1;
            command->pathStyle.tCubicCurveto->x2 = x2;
            command->pathStyle.tCubicCurveto->y2 = y2;
            command->pathStyle.tCubicCurveto->x = x;
            command->pathStyle.tCubicCurveto->y = y;
            break;
    	case ITEM_PATH_CMD_ID_SMOOTH_CUBIC_CURVETO:
            command->pathStyle.tSmoothCubicCurveto = CALLOC(1,sizeof(struct _itemPathCommand_SmoothCubicCurveto));
            x2 = va_arg(var,double);
            y2 = va_arg(var,double);
            x = va_arg(var,double);
            y = va_arg(var,double);
            command->pathStyle.tSmoothCubicCurveto->x2 = x2;
            command->pathStyle.tSmoothCubicCurveto->y2 = y2;
            command->pathStyle.tSmoothCubicCurveto->x = x;
            command->pathStyle.tSmoothCubicCurveto->y = y;
            break;
    	case ITEM_PATH_CMD_ID_QUADRATIC_CURVETO:
            command->pathStyle.tQuadraticCurveto = CALLOC(1,sizeof(struct _itemPathCommand_QuadraticCurveto));
            x1 = va_arg(var,double);
            y1 = va_arg(var,double);
            x = va_arg(var,double);
            y = va_arg(var,double);
            command->pathStyle.tQuadraticCurveto->x1 = x1;
            command->pathStyle.tQuadraticCurveto->y1 = y1;
            command->pathStyle.tQuadraticCurveto->x = x;
            command->pathStyle.tQuadraticCurveto->y = y;
            break;
    	case ITEM_PATH_CMD_ID_SMOOTH_QUADRATIC_CURVETO:
            command->pathStyle.tSmoothQuadraticCurveto = CALLOC(1,sizeof(struct _itemPathCommand_SmoothQuadraticCurveto));
            x1 = va_arg(var,double);
            y1 = va_arg(var,double);
            x = va_arg(var,double);
            y = va_arg(var,double);
            command->pathStyle.tSmoothQuadraticCurveto->x1 = x1;
            command->pathStyle.tSmoothQuadraticCurveto->y1 = y1;
            command->pathStyle.tSmoothQuadraticCurveto->x = x;
            command->pathStyle.tSmoothQuadraticCurveto->y = y;
            break;
    	case ITEM_PATH_CMD_ID_ARCTO:
            command->pathStyle.tArcto = CALLOC(1,sizeof(struct _itemPathCommand_Arcto));
            float rx = va_arg(var,double);
            float ry = va_arg(var,double);
            float x_axis_angle = va_arg(var,double);
            float large_arc = va_arg(var,double);
            float sweep_flag = va_arg(var,double);
            x = va_arg(var,double);
            y = va_arg(var,double);
            command->pathStyle.tArcto->rx = rx;
            command->pathStyle.tArcto->ry = ry;
            command->pathStyle.tArcto->x_axis_angle = x_axis_angle;
            command->pathStyle.tArcto->large_arc = large_arc;
            command->pathStyle.tArcto->sweep_flag = sweep_flag;
            command->pathStyle.tArcto->x = x;
            command->pathStyle.tArcto->y = y;
            break;
    	case ITEM_PATH_CMD_ID_END:
            command->pathStyle.tLineto = CALLOC(1,sizeof( struct _itemPathCommand_Lineto));
            x = va_arg(var,double);
            y = va_arg(var,double);
            command->pathStyle.tLineto->x = x;
            command->pathStyle.tLineto->y = y;
            break;
    }
    va_end(var);

    return command;
}
itemPathCommandId itemPathCommand_get_commandid(T_Pathcommand path_command)
{
    assert(path_command);
    return path_command->pathId;
}
itemPathCommand_Moveto  itemPathCommand_get_moveto(T_Pathcommand path_command)
{
    assert(path_command);
    assert(path_command->pathId == ITEM_PATH_CMD_ID_MOVETO);

    return path_command->pathStyle.tMoveto;
}
itemPathCommand_Lineto              itemPathCommand_get_lineto(T_Pathcommand path_command)
{
    assert(path_command);
    assert(path_command->pathId == ITEM_PATH_CMD_ID_LINETO);
    return path_command->pathStyle.tLineto;
}
itemPathCommand_CubicCruveto        itemPathCommand_get_cubiccurveto(T_Pathcommand path_command)
{
    assert(path_command);
    assert(path_command->pathId == ITEM_PATH_CMD_ID_CUBIC_CURVETO);
    return path_command->pathStyle.tCubicCurveto;
}
itemPathCommand_SmoothCubicCurveto  itemPathCommand_get_smoothcubiccurveto(T_Pathcommand path_command)
{
    assert(path_command);
    assert(path_command->pathId == ITEM_PATH_CMD_ID_SMOOTH_CUBIC_CURVETO);
    return path_command->pathStyle.tSmoothCubicCurveto;
}
itemPathCommand_QuadraticCurveto    itemPathCommand_get_quadraticcurveto(T_Pathcommand path_command)
{
    assert(path_command);
    assert(path_command->pathId == ITEM_PATH_CMD_ID_QUADRATIC_CURVETO);
    return path_command->pathStyle.tQuadraticCurveto;
}
itemPathCommand_SmoothQuadraticCurveto itemPathCommand_get_smoothquadraticcurveto(T_Pathcommand path_command)
{
    assert(path_command);
    assert(path_command->pathId == ITEM_PATH_CMD_ID_SMOOTH_QUADRATIC_CURVETO);
    return path_command->pathStyle.tSmoothQuadraticCurveto;
}
itemPathCommand_Arcto itemPathCommand_get_arcto(T_Pathcommand path_command)
{
    assert(path_command);
    assert(path_command->pathId == ITEM_PATH_CMD_ID_ARCTO);

    return path_command->pathStyle.tArcto;

}

T_Path itemPath_new()
{
    T_Path path;
    NEW0(path);

    path->pathCommand_seq = seq_new(10);
    return path;
}
void itemPath_add_pathcommand(T_Path path,itemPathCommand_t command)
{
    assert(path);
    assert(command);

    seq_addend(path->pathCommand_seq,command);
}
int itemPath_get_pathcommand_count(T_Path path)
{
    assert(path);
    return seq_length(path->pathCommand_seq);
}
T_Pathcommand itemPath_get_pathcommand_by_index(T_Path path,int index)
{
    assert(path);
    assert(index >= 0 && index < seq_length(path->pathCommand_seq));

    return seq_get(path->pathCommand_seq,index);
}
