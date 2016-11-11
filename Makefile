CPPFLAGS= -std=c++11
LDFLAGS = 
OUT = build/
INCLUDE = include/
CC= g++
SRC= src/
all:main

main:image graph voronoi
	mkdir -p $(OUT)
	$(CC) $(SRC)main.cpp $(OUT)image.o $(OUT)graph.o $(OUT)voronoi.o -o $(OUT)depixel $(CPPFLAGS) -I$(INCLUDE)

image:
	mkdir -p $(OUT)
	$(CC) -c $(SRC)image.cpp -o $(OUT)image.o $(CPPFLAGS) -I$(INCLUDE)

graph:
	mkdir -p $(OUT)
	$(CC) -c $(SRC)graph.cpp -o $(OUT)graph.o $(CPPFLAGS) -I$(INCLUDE)

voronoi:
	mkdir -p $(OUT)
	$(CC) -c $(SRC)voronoi.cpp -o $(OUT)voronoi.o $(CPPFLAGS) -I$(INCLUDE)

clean:
	rm -rf $(OUT)
