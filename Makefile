OUT=heron
IN=src/polygon.cpp src/window.cpp src/camera.cpp src/color.cpp src/triangle.cpp src/light.cpp src/mesh.cpp test.cpp
LIB=-lSDL2

default:
	g++ -g -std=c++17 -O3 -lm $(IN) -o $(OUT) $(LIB) && ./$(OUT)

docs:
	doxygen $(DOC)

