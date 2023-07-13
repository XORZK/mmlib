IN=rational.cc main.cc
OUT=main.exe

default:
	g++ -std=c++17 -O3 -lm -I src/include -L src/lib $(IN) -o $(OUT) -lmingw32 -lSDL2