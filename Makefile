VPATH = src libs
MKDIR = mkdir
ALG_INCLUDE_DIR = src/algorithm/include
PARSE_INCLUDE_DIR = src/parse/include
RENDER_INCLUDE_DIR = src/render/include
INCLUDE_DIR = include

CC=gcc
CFLAGS= -std=c99
AR=ar
RM=rm -rf

OBJ_DIR=./objs
LIB_DIR=./libs

objects = except.o \
		assert.o \
		atom.o \
		mem.o \
		array.o \
		seq.o \
		ring.o \
		table.o \
		sparsearray.o \
		vector2d.o \
		jc_errors.o \
		cJSON.o \
		parse.o \
		building.o \
		floor.o \
		layer.o \
		element.o \
		feature.o \
		item_types.o \
		item.o \
		color.o \
		envelop.o \
		routenode.o \
		route.o \
		indexminipq.o \
		wdigraph.o \
		render.o
OBJS = $(addprefix $(OBJ_DIR)/,$(objects))

lib=libmap.a
LIB=$(addprefix $(LIB_DIR)/,$(lib))

EXAMPLE_DIR=./example
examples=main \
		gtk_main \
		test_envelop
EXAMPLES=$(addprefix $(EXAMPLE_DIR)/,$(examples))

all:$(OBJ_DIR) $(LIB) $(EXAMPLES)
	$(RM) $(OBJ_DIR)

lib:$(OBJ_DIR) $(LIB)

$(OBJ_DIR):
	$(MKDIR) $@

$(LIB):$(OBJS)
	ar crs $@ $(OBJS)

$(OBJ_DIR)/except.o:algorithm/src/except.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/assert.o:algorithm/src/assert.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/atom.o:algorithm/src/atom.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/mem.o:algorithm/src/memchk.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/array.o:algorithm/src/array.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/seq.o:algorithm/src/seq.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/ring.o:algorithm/src/ring.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/table.o:algorithm/src/table.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(ALG_INCLUDE_DIR)

$(OBJ_DIR)/sparsearray.o:algorithm/src/sparsearray.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/vector2d.o:algorithm/src/vector2d.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(ALG_INCLUDE_DIR)


$(OBJ_DIR)/jc_errors.o:parse/src/jc_errors.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR)

$(OBJ_DIR)/cJSON.o:parse/src/cJSON.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR)
$(OBJ_DIR)/parse.o:parse/src/parse.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/building.o:parse/src/building.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/floor.o:parse/src/floor.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/layer.o:parse/src/layer.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/element.o:parse/src/element.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/feature.o:parse/src/feature.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/item_types.o:parse/src/item_types.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/item.o:parse/src/item.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/color.o:parse/src/color.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/envelop.o:parse/src/envelop.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/quadnode.o:parse/src/quadnode.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/quadtree.o:parse/src/quadtree.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/routenode.o:parse/src/routenode.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/route.o:parse/src/route.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/indexminipq.o:parse/src/indexminipq.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)
$(OBJ_DIR)/wdigraph.o:parse/src/wdigraph.c
	$(CC) $(CFLAGS) -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(ALG_INCLUDE_DIR)


$(OBJ_DIR)/render.o:render/src/render.c
	$(CC) `pkg-config --cflags gtk+-3.0` -o $@ -c $^ -I$(PARSE_INCLUDE_DIR) -I$(RENDER_INCLUDE_DIR) -L$(LIB_DIR) -lmap `pkg-config --libs gtk+-3.0`

$(EXAMPLE_DIR)/main:example/main.c
	$(CC) $(CFLAGS) -o $@  $< -I$(PARSE_INCLUDE_DIR) -L$(LIB_DIR) -lmap
$(EXAMPLE_DIR)/test_envelop:example/test_envelop.c
	$(CC) $(CFLAGS) -o $@  $< -I$(PARSE_INCLUDE_DIR) -L$(LIB_DIR) -lmap


$(EXAMPLE_DIR)/gtk_main:example/gtk_main.c
	$(CC) -g `pkg-config --cflags gtk+-3.0` $(CFLAGS) -o $@  $< -I$(PARSE_INCLUDE_DIR) -I$(RENDER_INCLUDE_DIR) -L$(LIB_DIR) -lmap `pkg-config --libs gtk+-3.0`
clean:
	$(RM) $(OBJ_DIR) $(LIB) $(EXAMPLES)
