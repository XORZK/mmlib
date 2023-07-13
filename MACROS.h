#define MAX(x,y) ((x > y) ? x : y)
#define MIN(x,y) ((x < y) ? x : y)

template <typename T>
T gcd(T a, T b) {
    return (b == 0 ? a : gcd(b, a % b));
}