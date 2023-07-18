IN=src/rational.cc src/window.cc src/color.cc src/camera.cc main.cc 
OUT=main

default:
	g++ -g -std=c++17 -O3 -lm $(IN) -o $(OUT) -lSDL2 && ./$(OUT)
