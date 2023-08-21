IN=src/rational.cc src/window.cc src/color.cc src/camera.cc src/triangle.cc main.cc 
OUT=main
LIB=-lSDL2
TEST=rewrite/window.cpp rewrite/camera.cpp rewrite/color.cpp rewrite/triangle.cpp test.cc
DOC=doxygen.cfg

default:
	g++ -g -std=c++17 -O3 -lm $(IN) -o $(OUT) $(LIB) && ./$(OUT)

docs:
	doxygen $(DOC)

test:
	g++ -g -std=c++17 -O3 -lm $(TEST) -o $(OUT) $(LIB) && ./$(OUT)
