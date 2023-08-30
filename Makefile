OUT=main
IN=src/window.cpp src/camera.cpp src/color.cpp src/triangle.cpp test.cpp
LIB=-lSDL2

default:
	g++ -std=c++17 -lm $(IN) -o $(OUT) $(LIB) && ./$(OUT)

docs:
	doxygen $(DOC)

