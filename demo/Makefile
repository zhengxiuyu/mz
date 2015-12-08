CC=clang
FLAGS=-Wall -pedantic
INC_DIRS=-I../include -I/usr/local/include
LIB_DIRS=-L../lib -L/usr/local/lib
LIBS=-lglfw3 -lGLEW -lmz -framework OpenGL -framework Foundation -framework CoreVideo -framework CoreGraphics -framework IOKit -framework AppKit
OBJS=gl_fluid.o glh.o render.o
TARGET=demo

all: main.c $(OBJS)
	$(CC) -o $(TARGET) main.c $(OBJS) $(LIBS) $(INC_DIRS) $(LIB_DIRS) $(FLAGS)
	rm $(OBJS)
	mkdir -p ../bin
	mv $(TARGET) ../bin

%.o : %.c
	$(CC) -c $< $(FLAGS) $(INC_DIR) $(FLAGS)
