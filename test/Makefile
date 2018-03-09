
#VPATH  :=  %.h ./inc
#VPATH  :=  %.c ./src


# 源文件
SOURCE  := $(wildcard *.c) $(wildcard *.cpp) $(wildcard thread/*.c)
OBJECT  := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))


# 编译目标名称
TARGET  := main


# 编译参数设置
CC      := gcc
CFLAGS  := -g -Wall
LIBS    := -lwiringPi -ltinyalsa -lpthread
INCLUDE := -I.


.PHONY: build object rebuild clean cleanall


all: $(TARGET)
	@echo "Make all ..."

build: $(TARGET)
	@echo "Building ..."

object: $(OBJECT)

rebuild: cleanall build

$(TARGET): $(OBJECT)
	$(CC) $(CFLAGS)  $(INCLUDE) -o $@ $(OBJECT) $(LIBS)

clean:
	@echo "Removing compiled files ..."
	-rm *.o

cleanall: clean
	@echo "Cleaning all ..."
	-rm $(TARGET)

