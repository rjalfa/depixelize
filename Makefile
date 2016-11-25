DECL=
CPPFLAGS= -std=c++11
LDFLAGS = -lglut -lGL -lGLEW
OUT = build/
INCLUDE = include/
CC= g++ -g
SRC= src/
all:main

main:common image graph voronoi spline
	mkdir -p $(OUT)
	$(CC) $(SRC)main.cpp $(OUT)common.o $(OUT)image.o $(OUT)graph.o $(OUT)voronoi.o $(OUT)spline.o -o $(OUT)depixel $(DECL) $(CPPFLAGS) -I$(INCLUDE) $(LDFLAGS)

image:common
	mkdir -p $(OUT)
	$(CC) -c $(SRC)image.cpp -o $(OUT)image.o $(CPPFLAGS) -I$(INCLUDE) $(LDFLAGS)

graph:common
	mkdir -p $(OUT)
	$(CC) -c $(SRC)graph.cpp -o $(OUT)graph.o $(CPPFLAGS) -I$(INCLUDE) $(LDFLAGS)

voronoi:common
	mkdir -p $(OUT)
	$(CC) -c $(SRC)voronoi.cpp -o $(OUT)voronoi.o $(CPPFLAGS) -I$(INCLUDE) $(LDFLAGS)

spline:common
	mkdir -p $(OUT)
	$(CC) -c $(SRC)spline.cpp -o $(OUT)spline.o $(CPPFLAGS) -I$(INCLUDE) $(LDFLAGS)

common:
	mkdir -p $(OUT)
	$(CC) -c $(SRC)common.cpp -o $(OUT)common.o $(CPPFLAGS) -I$(INCLUDE) $(LDFLAGS)	

clean:
	rm -rf $(OUT)
