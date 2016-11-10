#include <stdio.h>

#include "building.h"
#include "parse.h"
int main(int argc,char *argv[])
{
    building_t build;
    if(argc != 2)
    {
        printf("usage:main filename\n");
        return -1;
    }
    printf("start...\n");
    build = parseOpenFile(argv[1]);
    printf("building_name = %s\n",building_get_name(build));
    int floor_count = building_get_floor_count(build);
    for(int i = 0;i < floor_count;i++)
    {
        printf("%d/%d:\n",i,floor_count);
        floor_t floor = building_get_floor_by_index(build,i);
        printf("\tname = %s\n",floor_get_name(floor));
        printf("\twidht = %.2f\n",floor_get_width(floor));
        printf("\theight = %.2f\n",floor_get_height(floor));
        for(int j = 0;j < floor_get_layer_count(floor);j++)
        {
            layer_t layer = floor_get_layer_by_index(floor,j);
            printf("\t%s\n",layer_get_name(layer));
            for(int e = 0; e < layer_get_element_count(layer); e++)
            {
                element_t element = layer_get_element_by_index(layer,e);
                printf("\t  |--%s\n",element_get_name(element));
                for(int f = 0; f < element_get_feature_count(element); f++)
                {
                    feature_t feature = element_get_features_by_index(element,f);
                    printf("\t    |--%s",feature_get_geotype(feature));
                    for(int c = 0; c < feature_get_item_count(feature); c++)
                    {
                        item_t item = feature_get_item(feature,c);
                        printf(" Kind = %d",item_get_kind(item));
                        // coord_t coord = feature_get_coord_by_index(feature,c);
                        // printf("(%.2f,%.2f)",coord_get_x(coord),coord_get_y(coord));
                    }
                    printf("\n");
                }
            }
        }

    }
    return 0;
}
