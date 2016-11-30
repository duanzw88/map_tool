#include <stdio.h>
#include <gtk/gtk.h>
#include "building.h"
#include "parse.h"
#include "render.h"
#include "routenode.h"

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

static gboolean draw_callback(GtkWidget *widget,cairo_t *cr,gpointer data)
{
    int i;
    double proj_x = 0.0;
    double proj_y = 0.0;
    building_t build = (building_t)data;
    floor_t floor = building_get_floor_by_index(build,0);
    int grid = floor_get_grid(floor);
    map_width = floor_get_width(floor);
    map_height = floor_get_height(floor);
    x_scale = DEFAULT_WIDTH / map_width;
    y_scale = DEFAULT_HEIGHT / map_height;

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

    //画网格
    drawing_floor_grid(cr,grid,map_width,map_height,x_scale);
    //绘画点击事件的点
    if(click_flag != 0)
    {
        drawing_point(cr,clickPoint.x,clickPoint.y);
        building_get_near_road_point(build,clickPoint.x / x_scale,clickPoint.y / x_scale,&proj_x,&proj_y);
        drawing_line(cr,clickPoint.x,clickPoint.y,proj_x * x_scale,proj_y * x_scale);
        drawing_point(cr,proj_x * x_scale,proj_y * x_scale);

        //画导航路径
        seq_t nodes = building_route_plan_by_root(build,clickPoint.x / x_scale,clickPoint.y / x_scale,866.16,566.59);
        for(i = 1; i < seq_length(nodes); i++)
        {
            routenode_t start_node = seq_get(nodes,i-1);
            routenode_t end_node = seq_get(nodes,i);
            drawing_line(cr,
                        routenode_get_x(start_node) * x_scale,
                        routenode_get_y(start_node) * x_scale,
                        routenode_get_x(end_node) * x_scale,
                        routenode_get_y(end_node) * x_scale );
            // printf("(%.2f,%.2f)\n",routenode_get_x(node),routenode_get_y(node));
        }
    }


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
