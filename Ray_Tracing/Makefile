# the compiler: gcc for C program, define as g++ for C++
CC = gcc
CXX = g++
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
# # $(CXX) $(CFLAGS) -o out $(TARGET) $(LINKING)
CFLAGS  = -g # -Wall
LINKING = -lglut -lGL -lGLU
TARGET = 1705047_Vector.h 1705047_Camera.h 1705047_classes.h 1705047_main.cpp

all:
	$(CXX) $(CFLAGS) -o out $(TARGET) $(LINKING)
	./out

clean:
	$(RM) $(TARGET)
