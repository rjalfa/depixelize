CPPFLAGS= -std=c++11
LDFLAGS=
OUT= build/
INCLUDE= src/
CC= g++
SRC= src/
all:main

main:image graph
	mkdir -p $(OUT)
	$(CC) -c $(SRC)main.cpp $(OUT)image.o $(OUT)graph.o -o $(OUT)depixel $(CPPFLAGS) -L$(LDFLAGS) -I$(INCLUDE)

image:
	mkdir -p $(OUT)
	$(CC) -c $(SRC)image.cpp -o $(OUT)image.o $(CPPFLAGS) -L$(LDFLAGS) -I$(INCLUDE)

graph:
	mkdir -p $(OUT)
	$(CC) -c $(SRC)graph.cpp -o $(OUT)graph.o $(CPPFLAGS) -L$(LDFLAGS) -I$(INCLUDE)


clean:
	rm -rf $(OUT)