#include <iostream>

template <typename T>
bool test(T x1, T x2, bool (*cmp)(T, T)) {
	return cmp(x1, x2);
}

bool compare(double a, double b) {
	return (a > b);
}

int main(void) {
	std::cout << test<double>(1.0, 0.0, compare) << "\n";
}
