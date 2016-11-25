#include <stdio.h>
#include <gtk/gtk.h>
#include "building.h"
#include "parse.h"
#include "render.h"

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 740
GtkWidget *window;
GtkWidget *darea;

float x_scale = 1.0;
float y_scale = 1.0;
float map_width;
float map_height;
int click_flag = 0;
struct
{
    float x;
    float y;
}clickPoint;

// floor_t current_floor;

// static void drawing_polygon(cairo_t *cr,feature_t feature,itemPolygon_t polygon)
// {
//     itemPoint_t point;
//     float red = 0.0,green = 0.0,blue = 0.0,stroke_width = 0.0,alpha = 0.0;
//     // printf("drawing_polygon...\n");
//
//     color_t color = feature_get_stroke_color(feature);
//     red = color_get_red(color) / 255.0;
//     green = color_get_green(color) / 255.0;
//     blue = color_get_blue(color) / 255.0;
//     alpha = color_get_alpha(color);
//     // printf("stroke rgba(%.2f,%.2f,%.2f,%.2f)\n",red,green,blue,alpha);
//     stroke_width = feature_get_stroke_width(feature);
//
//     cairo_set_line_width(cr,stroke_width*x_scale);
//     cairo_set_source_rgba(cr,red,green,blue,alpha);
//
//     point = itemPolygon_get_point_by_index(polygon,0);
//     cairo_move_to(cr,itemPoint_get_x(point) * x_scale,itemPoint_get_y(point) * y_scale);
//
//     for(int i = 1; i < itemPolygon_get_point_count(polygon);i++)
//     {
//         point = itemPolygon_get_point_by_index(polygon,i);
//         cairo_line_to(cr,itemPoint_get_x(point) * x_scale,itemPoint_get_y(point) * y_scale);
//         // printf("(%.2f,%.2f)\n",itemPoint_get_x(point),itemPoint_get_y(point));
//     }
//
//     cairo_stroke_preserve(cr);
//
//     color = feature_get_fill_color(feature);
//     red = color_get_red(color) / 255.0;
//     green = color_get_green(color) / 255.0;
//     blue = color_get_blue(color) / 255.0;
//     alpha = color_get_alpha(color);
//     cairo_set_source_rgba(cr,red,green,blue,alpha);
//     cairo_fill(cr);
// }
// static void drawing_path(cairo_t *cr,feature_t feature,itemPath_t path)
// {
//     // printf("draw path...\n");
//     itemPoint_t point;
//     itemPathCommand_t path_command;
//     float red = 0.0,green = 0.0,blue = 0.0,stroke_width = 0.0,alpha = 0.0;
//
//     color_t color = feature_get_stroke_color(feature);
//     red = color_get_red(color) / 255.0;
//     green = color_get_green(color) / 255.0;
//     blue = color_get_blue(color) / 255.0;
//     alpha = color_get_alpha(color);
//
//     stroke_width = feature_get_stroke_width(feature);
//     cairo_set_line_width(cr,stroke_width*x_scale);
//     cairo_set_source_rgba(cr,red,green,blue,alpha);
//     // printf("path count = %d\n",itemPath_get_pathcommand_count(path));
//     for(int i = 0; i < itemPath_get_pathcommand_count(path); i++)
//     {
//         path_command = itemPath_get_pathcommand_by_index(path,i);
//         itemPathCommandId id = itemPathCommand_get_commandid(path_command);
//         if(id == ITEM_PATH_CMD_ID_MOVETO)
//         {
//             itemPathCommand_Moveto moveto = itemPathCommand_get_moveto(path_command);
//             float x = moveto->x * x_scale;
//             float y = moveto->y * y_scale;
//             printf("draw moveto (%.2f %.2f)\n",x,y);
//             cairo_move_to(cr,moveto->x * x_scale,moveto->y * y_scale);
//         }
//         else if(id == ITEM_PATH_CMD_ID_LINETO)
//         {
//             itemPathCommand_Lineto lineto = itemPathCommand_get_lineto(path_command);
//             cairo_line_to(cr,lineto->x * x_scale,lineto->y * y_scale);
//         }
//         else if(id == ITEM_PATH_CMD_ID_CUBIC_CURVETO)
//         {
//             itemPathCommand_CubicCruveto curveto = itemPathCommand_get_cubiccurveto(path_command);
//             float x1 = curveto->x1 * x_scale;
//             float y1 = curveto->y1 * x_scale;
//             float x2 = curveto->x2 * x_scale;
//             float y2 = curveto->y2 * x_scale;
//             float x = curveto->x * x_scale;
//             float y = curveto->y * x_scale;
//             printf("draw curveto...(%.2f %.2f %.2f %.2f %.2f %.2f)\n",x1,y1,x2,y2,x,y);
//             cairo_curve_to(cr,curveto->x1* x_scale,curveto->y1* x_scale,curveto->x2* x_scale,curveto->y2* x_scale,curveto->x* x_scale,curveto->y* x_scale);
//         }
//         else if(id == ITEM_PATH_CMD_ID_ARCTO)
//         {
//             itemPathCommand_Arcto arcto = itemPathCommand_get_arcto(path_command);
//             cairo_arc(cr,arcto->cx * x_scale,arcto->cy * x_scale,arcto->radius * x_scale,arcto->start_angle,arcto->end_angle);
//         }
//
//     }
//
//     cairo_stroke_preserve(cr);
//
//     color = feature_get_fill_color(feature);
//     red = color_get_red(color) / 255.0;
//     green = color_get_green(color) / 255.0;
//     blue = color_get_blue(color) / 255.0;
//     alpha = color_get_alpha(color);
//     cairo_set_source_rgba(cr,red,green,blue,alpha);
//     cairo_fill(cr);
//
// }
static gboolean draw_callback(GtkWidget *widget,cairo_t *cr,gpointer data)
{

    double proj_x = 0.0;
    double proj_y = 0.0;
    building_t build = (building_t)data;
    floor_t floor = building_get_floor_by_index(build,0);
    int grid = floor_get_grid(floor);
    map_width = floor_get_width(floor);
    map_height = floor_get_height(floor);
    x_scale = DEFAULT_WIDTH / map_width;
    y_scale = DEFAULT_HEIGHT / map_height;

    // g_print("width = %.2f height = %.2f\n",map_width,map_height);



    for(int j = 0;j < floor_get_layer_count(floor);j++)
    {
        layer_t layer = floor_get_layer_by_index(floor,j);
        for(int e = 0; e < layer_get_element_count(layer); e++)
        {
            element_t element = layer_get_element_by_index(layer,e);
            for(int f = 0; f < element_get_feature_count(element); f++)
            {
                feature_t   feature = element_get_features_by_index(element,f);
                item_t      item = feature_get_item(feature);
                int         kind = item_get_kind(item);
    //                 // printf("kind = %d\n",kind);
                switch (kind)
                {
                    case ITEM_KIND_POINT:

                        // drawing_point();
                        break;
                    case ITEM_KIND_LINE:
                        //drawing_line();
                        break;
                    case ITEM_KIND_RECT:
                        //drawing_rect();
                        //break;
                    case ITEM_KIND_CIRCLE:
                        //drawing_circle();
                        //break;
                    case ITEM_KIND_ELLIPSE:
                        //drawing_ellipse();
                        //break;
                	case ITEM_KIND_POLYLINE:
                        drawing_polyline(cr,feature,item_get_polyline(item),x_scale,y_scale);
                        break;
                	case ITEM_KIND_POLYGON:
                        drawing_polygon(cr,feature,item_get_polygon(item),x_scale);
                        break;
                    case ITEM_KIND_PATH:
                        //drawing_path(cr,feature,item_get_path(item));
                        break;
                }
            }
        }
    }

    //绘画点击事件的点
    if(click_flag != 0)
    {
        drawing_click_point(cr,clickPoint.x,clickPoint.y);
        building_get_near_road_point(build,clickPoint.x / x_scale,clickPoint.y / x_scale,&proj_x,&proj_y);
        drawing_click_point(cr,proj_x * x_scale,proj_y * x_scale);
        // int y_index = (int)(clickPoint.y / grid);
        // int x_index = (int)(int)(clickPoint.x / grid);
        //g_print("key = %d\n",floor_get_key(floor,clickPoint.x / x_scale,clickPoint.y / x_scale));
    }
    //画网格
    drawing_floor_grid(cr,grid,map_width,map_height,x_scale);
    return FALSE;
}

static gboolean clicked(GtkWidget *widget,GdkEventButton *event,gpointer data)
{
    clickPoint.x = event->x;
    clickPoint.y = event->y;
    click_flag++;
    g_print("clicked(%.2f,%.2f)\n",clickPoint.x,clickPoint.y);

    gtk_widget_queue_draw(widget);
    return TRUE;
}
void init_window(const char *name)
{
    gtk_init(NULL,NULL);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window),DEFAULT_WIDTH,DEFAULT_HEIGHT);
    gtk_window_set_title(GTK_WINDOW(window),name);
    g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
    g_signal_connect(G_OBJECT(window),"button-press-event",G_CALLBACK(clicked),NULL);
    // g_signal_connect(window,"configure_event",G_CALLBACK(on_configure_event),NULL);

}
int main(int argc,char *argv[])
{
    GtkWidget *grid;
    GtkWidget *darea;

    building_t build;
    if(argc != 2)
    {
        printf("usage:main filename\n");
        return -1;
    }
    printf("start...\n");
    build = parseOpenFile(argv[1]);
    init_window(building_get_name(build));

    building_add_route(build);
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window),grid);

    darea = gtk_drawing_area_new();
    gtk_widget_set_size_request(darea,DEFAULT_WIDTH,DEFAULT_HEIGHT);
    g_signal_connect(G_OBJECT(darea),"draw",G_CALLBACK(draw_callback),build);
    gtk_grid_attach(GTK_GRID(grid),darea,0,0,1,1);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
