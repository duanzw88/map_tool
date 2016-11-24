
#ifndef __ITEM_TYPES_H__
#define __ITEM_TYPES_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define T_Point     itemPoint_t
#define T_Rect      itemRect_t
#define T_Line      itemLine_t
#define T_Circle    itemCircle_t
#define T_Ellipse   itemEllipse_t
#define T_Polyline  itemPolyline_t
#define T_Polygon   itemPolygon_t
#define T_Path      itemPath_t
#define T_Pathcommand itemPathCommand_t


typedef struct T_Point     *T_Point;
typedef struct T_Rect      *T_Rect;
typedef struct T_Line      *T_Line;
typedef struct T_Circle    *T_Circle;
typedef struct T_Ellipse   *T_Ellipse;
typedef struct T_Polyline  *T_Polyline;
typedef struct T_Polygon   *T_Polygon;
typedef struct T_Path      *T_Path;
typedef struct T_Pathcommand *T_Pathcommand;

typedef enum _itemPathCommandId
{
    ITEM_PATH_CMD_ID_MOVETO = 0,
	ITEM_PATH_CMD_ID_LINETO,
	ITEM_PATH_CMD_ID_CUBIC_CURVETO,
	ITEM_PATH_CMD_ID_ARCTO,
	ITEM_PATH_CMD_ID_END
}itemPathCommandId;

// typedef enum _itemPathCommandId
// {
//     ITEM_PATH_CMD_ID_MOVETO = 0,
// 	ITEM_PATH_CMD_ID_LINETO,
// 	ITEM_PATH_CMD_ID_CUBIC_CURVETO,
// 	ITEM_PATH_CMD_ID_SMOOTH_CUBIC_CURVETO,
// 	ITEM_PATH_CMD_ID_QUADRATIC_CURVETO,
// 	ITEM_PATH_CMD_ID_SMOOTH_QUADRATIC_CURVETO,
// 	ITEM_PATH_CMD_ID_ARCTO,
// 	ITEM_PATH_CMD_ID_END
// }itemPathCommandId;

// --Path:MoveTo--
struct _itemPathCommane_Moveto
{
    float x;
    float y;
};
typedef struct _itemPathCommane_Moveto *itemPathCommand_Moveto;
// --Path:Lineto--
typedef struct _itemPathCommand_Lineto
{
    float x;
    float y;
}*itemPathCommand_Lineto;
// --Path:CubicCurveTo三次贝塞尔曲线--
typedef struct _itemPathCommand_CubicCruveto
{
    float x1;
    float y1;
    float x2;
    float y2;
    float x;
    float y;
}*itemPathCommand_CubicCruveto;

//	-- Path : SmoothCubicCurveTo 平滑曲线--
// typedef struct _itemPathCommand_SmoothCubicCurveto
// {
//     float x2;
//     float y2;
//     float x;
//     float y;
// }*itemPathCommand_SmoothCubicCurveto;

//	-- Path : QuadraticCurveTo 二次贝塞尔曲线--
// typedef struct _itemPathCommand_QuadraticCurveto
// {
//     float x1;
//     float y1;
//     float x;
//     float y;
// }*itemPathCommand_QuadraticCurveto;

//	-- Path : SmoothQuadraticCurveTo 平滑二次贝塞尔曲线--
// typedef struct _itemPathCommand_SmoothQuadraticCurveto
// {
//     float x1;
//     float y1;
//     float x;
//     float y;
// }*itemPathCommand_SmoothQuadraticCurveto;

//	-- Path : Arc 弧形--
// typedef struct _itemPathCommand_Arcto
// {
//     float rx;   //x轴半径
//     float ry;   //y轴半径
//     float x_axis_angle; //x轴旋转角度
//     float large_arc;    //角度大小 0(小角度弧) 1(大角度弧)
//     float sweep_flag;   //弧线方向 0(逆时针) 1(顺时针)
//     float x;   //起点
//     float y;   //
// }*itemPathCommand_Arcto;
//
typedef struct _itemPathCommand_Arcto
{
    float cx;   //x轴半径
    float cy;   //y轴半径
    float radius; //x轴旋转角度
    float start_angle;    //角度大小 0(小角度弧) 1(大角度弧)
    float end_angle;   //弧线方向 0(逆时针) 1(顺时针)
}*itemPathCommand_Arcto;


extern T_Point      itemPoint_new(float x,float y);
extern void         itemPoint_set_x(T_Point point,float x);
extern void         itemPoint_set_y(T_Point point,float y);
extern float        itemPoint_get_x(T_Point point);
extern float        itemPoint_get_y(T_Point point);
extern T_Rect       itemRect_new(float x,float y,float width,float height,float rx,float ry);
extern T_Line       itemLine_new(float x1,float y1,float x2,float y2);
extern T_Circle     itemCircle_new(float x,float y,float radius);
extern T_Ellipse    itemEllipse_new(float x,float y,float rx,float ry);
extern T_Polyline   itemPolyline_new();
extern void         itemPolyline_add_point(T_Polyline polyline,itemPoint_t point);
extern int          itemPolyline_get_point_count(T_Polyline polyline);
extern itemPoint_t   itemPolyline_get_point_by_index(T_Polyline polyline,int index);

extern T_Polygon    itemPolygon_new();
extern void         itemPolygon_add_point(T_Polygon polygon,itemPoint_t point);
extern int          itemPolygon_get_point_count(T_Polygon polygon);
extern itemPoint_t  itemPolygon_get_point_by_index(T_Polygon polygon,int index);

extern T_Pathcommand itemPathCommand_new(int pathId, ...);
extern itemPathCommandId itemPathCommand_get_commandid(T_Pathcommand path_command);
extern itemPathCommand_Moveto  itemPathCommand_get_moveto(T_Pathcommand path_command);
extern itemPathCommand_Lineto              itemPathCommand_get_lineto(T_Pathcommand path_command);
extern itemPathCommand_CubicCruveto        itemPathCommand_get_cubiccurveto(T_Pathcommand path_command);
//extern itemPathCommand_SmoothCubicCurveto  itemPathCommand_get_smoothcubiccurveto(T_Pathcommand path_command);
//extern itemPathCommand_QuadraticCurveto    itemPathCommand_get_quadraticcurveto(T_Pathcommand path_command);
//extern itemPathCommand_SmoothQuadraticCurveto itemPathCommand_get_smoothquadraticcurveto(T_Pathcommand path_command);
extern itemPathCommand_Arcto               itemPathCommand_get_arcto(T_Pathcommand path_command);


extern T_Path       itemPath_new();
extern void         itemPath_add_pathcommand(T_Path path,itemPathCommand_t command);
extern int          itemPath_get_pathcommand_count(T_Path path);
extern T_Pathcommand itemPath_get_pathcommand_by_index(T_Path path,int index);

#ifdef __cplusplus
}
#endif
#endif
