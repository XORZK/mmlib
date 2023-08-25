OUT=main
IN=src/window.cpp src/camera.cpp src/color.cpp src/triangle.cpp test.cc
LIB=-lSDL2
DOC=doxygen.cfg

default:
	g++ -g -std=c++17 -O3 -lm $(IN) -o $(OUT) $(LIB) && ./$(OUT)

docs:
	doxygen $(DOC)

