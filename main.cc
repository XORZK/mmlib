#include <SDL2/SDL.h>
#include <functional>
#include "matrix.h"

int main() {
    rat r(3, 5);

    int x = static_cast<int>(r);

    std::cout << x << "\n";
}
