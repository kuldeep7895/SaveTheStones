CC=g++

CFLAGS=-c -Wall

OBJS=maze_render.o maze_generate.o functions.h

PKG=-w -lSDL2 -lSDL2_image

all: maze

maze: maze_render.o maze_generate.o
	$(CC) $(OBJS) $(PKG) -o maze

maze_render.o: maze_render.cpp game.cpp functions.h
	$(CC) $(CFLAGS) maze_render.cpp game.cpp $(PKG)
	
maze_generate.o: maze_generate.cpp functions.h
	$(CC) $(CFLAGS) maze_generate.cpp $(PKG)
	
clean:
	rm -rf *o maze
