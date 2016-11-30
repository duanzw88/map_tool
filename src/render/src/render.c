
#include <stdlib.h>
#include <math.h>
#include<time.h>
#include "render.h"

void drawing_floor_grid(cairo_t *cr,int grid,float width,float height,float scale)
{
    int i;
    cairo_set_line_width(cr,0.5);
    cairo_set_source_rgba(cr,1,0,0,0.1);

    for(i = 0;i < width; i+= grid)
    {
        cairo_move_to(cr,i * scale,0);
        cairo_line_to(cr,i * scale,height * scale);
    }
    for(i = 0; i < height; i+= grid)
    {
        cairo_move_to(cr,0,i * scale);
        cairo_line_to(cr,width * scale,i * scale);
    }
    cairo_stroke_preserve(cr);
    // cairo_set_source_rgb(cr,0,0,0);
    cairo_fill(cr);
}
// void drawing_click_point(cairo_t *cr,float x,float y)
// {
//     cairo_set_line_width(cr,2);
//     cairo_set_source_rgb(cr,1,0,0);
//
//     cairo_arc(cr,x,y,2.0,0,2*M_PI);
//     cairo_stroke_preserve(cr);
//
//     cairo_fill(cr);
// }
// extern void drawing_line_by_coord(cairo_t *cr,float sx,float sy,float ex,float ey,float scale)
// {
//     int i;
//     cairo_set_line_width(cr,2);
//     cairo_set_source_rgba(cr,1,0,0,1);
//
//     cairo_move_to(cr,sx,sy);
//     cairo_line_to(cr,ex * scale,ey * scale);
//
//     cairo_stroke_preserve(cr);
//     cairo_fill(cr);
// }
void drawing_point(cairo_t *cr,float x,float y)
{
    cairo_set_line_width(cr,2);
    cairo_set_source_rgb(cr,1,0,0);

    cairo_arc(cr,x,y,2.0,0,2*M_PI);
    cairo_stroke_preserve(cr);

    cairo_fill(cr);
}
void drawing_line(cairo_t *cr,float sx,float sy,float ex,float ey)
{
    int i;
    srand((unsigned)time(NULL));
    cairo_set_line_width(cr,2);
    double r = rand()/(RAND_MAX+1.0);
    double g = rand()/(RAND_MAX+1.0);
    double b = rand()/(RAND_MAX+1.0);

    printf("r = %.2f g = %.2f b = %.2f\n",r,g,b);
    cairo_set_source_rgba(cr,r,g,b,1);

    cairo_move_to(cr,sx,sy);
    cairo_line_to(cr,ex,ey);

    cairo_stroke_preserve(cr);
    cairo_fill(cr);
}
void drawing_rect(cairo_t *cr,itemRect_t rect)
{
}
void drawing_circle(cairo_t *cr,itemCircle_t circle)
{
}
void drawing_ellipse(cairo_t *cr,itemEllipse_t ellipse)
{
}
void drawing_polyline(cairo_t *cr,feature_t feature,itemPolyline_t polyline,float x_scale,float y_scale)
{
    itemPoint_t point;
    float red = 0.0,green = 0.0,blue = 0.0,stroke_width = 0.0,alpha = 0.0;
    // // printf("drawing_polyline...\n");
    //
    color_t color = feature_get_stroke_color(feature);
    red = color_get_red(color) / 255.0;
    green = color_get_green(color) / 255.0;
    blue = color_get_blue(color) / 255.0;
    alpha = color_get_alpha(color);
    // // printf("stroke rgba(%.2f,%.2f,%.2f,%.2f)\n",red,green,blue,alpha);
    stroke_width = feature_get_stroke_width(feature);
    //
    cairo_set_line_width(cr,stroke_width*x_scale);
    cairo_set_source_rgba(cr,red,green,blue,alpha);

    point = itemPolyline_get_point_by_index(polyline,0);
    float sx = itemPoint_get_x(point) * x_scale;
    float sy = itemPoint_get_y(point) * y_scale;
    // printf("sx = %.2f xy = %.2f\n",itemPoint_get_x(point),itemPoint_get_y(point));
    cairo_move_to(cr,sx,sy);
    for(int i = 1; i < itemPolyline_get_point_count(polyline);i++)
    {
        point = itemPolyline_get_point_by_index(polyline,i);
        cairo_line_to(cr,itemPoint_get_x(point) * x_scale,itemPoint_get_y(point) * y_scale);
    }
    cairo_stroke_preserve(cr);

    color = feature_get_fill_color(feature);
    red = color_get_red(color) / 255.0;
    green = color_get_green(color) / 255.0;
    blue = color_get_blue(color) / 255.0;
    alpha = color_get_alpha(color);
    cairo_set_source_rgba(cr,red,green,blue,alpha);
    cairo_fill(cr);
}
void drawing_polygon(cairo_t *cr,feature_t feature,itemPolygon_t polygon,double x_scale)
{
    itemPoint_t point;
    float red = 0.0,green = 0.0,blue = 0.0,stroke_width = 0.0,alpha = 0.0;
    // printf("drawing_polygon...\n");

    color_t color = feature_get_stroke_color(feature);
    red = color_get_red(color) / 255.0;
    green = color_get_green(color) / 255.0;
    blue = color_get_blue(color) / 255.0;
    alpha = color_get_alpha(color);
    // printf("stroke rgba(%.2f,%.2f,%.2f,%.2f)\n",red,green,blue,alpha);
    stroke_width = feature_get_stroke_width(feature);

    cairo_set_line_width(cr,stroke_width*x_scale);
    cairo_set_source_rgba(cr,red,green,blue,alpha);

    point = itemPolygon_get_point_by_index(polygon,0);
    cairo_move_to(cr,itemPoint_get_x(point) * x_scale,itemPoint_get_y(point) * x_scale);

    for(int i = 1; i < itemPolygon_get_point_count(polygon);i++)
    {
        point = itemPolygon_get_point_by_index(polygon,i);
        cairo_line_to(cr,itemPoint_get_x(point) * x_scale,itemPoint_get_y(point) * x_scale);
        // printf("(%.2f,%.2f)\n",itemPoint_get_x(point),itemPoint_get_y(point));
    }

    cairo_stroke_preserve(cr);

    color = feature_get_fill_color(feature);
    red = color_get_red(color) / 255.0;
    green = color_get_green(color) / 255.0;
    blue = color_get_blue(color) / 255.0;
    alpha = color_get_alpha(color);
    cairo_set_source_rgba(cr,red,green,blue,alpha);
    cairo_fill(cr);
}
void drawing_path(cairo_t *cr,feature_t feature,itemPath_t path)
{
    // printf("draw path...\n");
    // itemPoint_t point;
    // itemPathCommand_t path_command;
    // float red = 0.0,green = 0.0,blue = 0.0,stroke_width = 0.0,alpha = 0.0;
    //
    // color_t color = feature_get_stroke_color(feature);
    // red = color_get_red(color) / 255.0;
    // green = color_get_green(color) / 255.0;
    // blue = color_get_blue(color) / 255.0;
    // alpha = color_get_alpha(color);
    //
    // stroke_width = feature_get_stroke_width(feature);
    // cairo_set_line_width(cr,stroke_width*x_scale);
    // cairo_set_source_rgba(cr,red,green,blue,alpha);
    // // printf("path count = %d\n",itemPath_get_pathcommand_count(path));
    // for(int i = 0; i < itemPath_get_pathcommand_count(path); i++)
    // {
    //     path_command = itemPath_get_pathcommand_by_index(path,i);
    //     itemPathCommandId id = itemPathCommand_get_commandid(path_command);
    //     if(id == ITEM_PATH_CMD_ID_MOVETO)
    //     {
    //         itemPathCommand_Moveto moveto = itemPathCommand_get_moveto(path_command);
    //         float x = moveto->x * x_scale;
    //         float y = moveto->y * y_scale;
    //         printf("draw moveto (%.2f %.2f)\n",x,y);
    //         cairo_move_to(cr,moveto->x * x_scale,moveto->y * y_scale);
    //     }
    //     else if(id == ITEM_PATH_CMD_ID_LINETO)
    //     {
    //         itemPathCommand_Lineto lineto = itemPathCommand_get_lineto(path_command);
    //         cairo_line_to(cr,lineto->x * x_scale,lineto->y * y_scale);
    //     }
    //     else if(id == ITEM_PATH_CMD_ID_CUBIC_CURVETO)
    //     {
    //         itemPathCommand_CubicCruveto curveto = itemPathCommand_get_cubiccurveto(path_command);
    //         float x1 = curveto->x1 * x_scale;
    //         float y1 = curveto->y1 * x_scale;
    //         float x2 = curveto->x2 * x_scale;
    //         float y2 = curveto->y2 * x_scale;
    //         float x = curveto->x * x_scale;
    //         float y = curveto->y * x_scale;
    //         printf("draw curveto...(%.2f %.2f %.2f %.2f %.2f %.2f)\n",x1,y1,x2,y2,x,y);
    //         cairo_curve_to(cr,curveto->x1* x_scale,curveto->y1* x_scale,curveto->x2* x_scale,curveto->y2* x_scale,curveto->x* x_scale,curveto->y* x_scale);
    //     }
    //     else if(id == ITEM_PATH_CMD_ID_ARCTO)
    //     {
    //         itemPathCommand_Arcto arcto = itemPathCommand_get_arcto(path_command);
    //         cairo_arc(cr,arcto->cx * x_scale,arcto->cy * x_scale,arcto->radius * x_scale,arcto->start_angle,arcto->end_angle);
    //     }
    //
    // }
    //
    // cairo_stroke_preserve(cr);
    //
    // color = feature_get_fill_color(feature);
    // red = color_get_red(color) / 255.0;
    // green = color_get_green(color) / 255.0;
    // blue = color_get_blue(color) / 255.0;
    // alpha = color_get_alpha(color);
    // cairo_set_source_rgba(cr,red,green,blue,alpha);
    // cairo_fill(cr);

}
