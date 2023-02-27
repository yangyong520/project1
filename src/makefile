CC = gcc
CFLAGS = -Wall
LIBS = -lm -lmosquitto
BIN = ../bin
OBJ = ../obj
BUILD = ../build

# 定义头文件搜索路径
INC = -I../include

# 定义源文件和目标文件列表
SRCS = $(wildcard ../src/*.c)
OBJS = $(SRCS:../src/%.c=$(OBJ)/%.o)

all: pub sub

pub: $(OBJ)/pub.o $(OBJ)/logger.o
	$(CC) $^ -o $(BIN)/$@ $(LIBS)

sub: $(OBJ)/sub.o $(OBJ)/logger.o
	$(CC) $^ -o $(BIN)/$@ $(LIBS)

# 根据源文件编译目标文件
$(OBJ)/%.o: ../src/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(BIN)/pub $(BIN)/sub $(OBJ)/*.o $(BUILD)/*

