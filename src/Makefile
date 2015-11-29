CC=clang
FLAGS=-Wall -pedantic
INC_DIRS =-I../include
OBJS=particles.o grid.o domain.o sim.o
TARGET=libmz.a

all: $(OBJS)
	ar rcs $(TARGET) $(OBJS)
	rm $(OBJS)
	mkdir -p ../lib
	mv $(TARGET) ../lib

%.o: %.c
	$(CC) -c $< $(FLAGS) $(INC_DIRS)

