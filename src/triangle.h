#ifndef TRIANGLE_H
#define TRIANGLE_H

#pragma once
#include "MACROS.h"
#include "matrix.h"
#include "vector.h"
#include "triangle.h"

class triangle {
    private:
        // vertexes.
        vector<double> A, B, C;
    public:
        triangle();

        triangle(vector<double> p1, vector<double> p2);

        triangle(vector<double> p1, vector<double> p2, vector<double> p3);

        vector<double> v1() const;

        vector<double> v2() const;

        vector<double> v3() const;

        void v1(vector<double> p);

        void v2(vector<double> p);

        void v3(vector<double> p);

        double area() const;

        double perimeter() const;

        void translate(const vector<double> translation);

        void transform(const matrix<double> mat);

        void reset();

        vector<vector<double>> vertices() const;
};

std::ostream& operator<<(std::ostream& out, const triangle& t);

// Find Angle of each Vertex from the Estimated Center of Points
// Sorts Points Counter Clockwise
inline vector<vector<double>> sort_vertices(const vector<vector<double>>& vertices) {
    vector<vector<double>> sorted(vertices.get_size());
    vector<double> C = (vertices[0].get_size() == 3 ? create_vec3(0,0,1) : create_vec2(0, 0));

    for (uint64_t i = 0; i < vertices.get_size(); i++) {
        C[0] += vertices[i][0];
        C[1] += vertices[i][1];
    }

    C /= vertices.get_size();

    for (uint64_t i = 0; i < vertices.get_size(); i++) {
        vector<double> n = (vertices[i] - C);
        double dist = distance(n[0], n[1], 0.0, 0.0);

        double angle = atan2(n[1], n[0]) * 180/M_PI;
        angle += (360 * (angle < 0));
        sorted.push_back(create_vec3(angle, dist, static_cast<double>(i)));
    }

    // Sort the vector
    // Insertion Sort? --> Merge Sort (TODO)
    for (uint64_t i = 1; i < sorted.get_size(); i++) {
        uint64_t k = i;
        while (k > 0 && ((sorted[k][0] < sorted[k-1][0]) || (sorted[k][0] == sorted[k-1][0] && sorted[k][1] < sorted[k-1][1]))) {
            sorted.swap(k, k-1);
            k--;
        }
    }

    for (uint64_t i = 0; i < sorted.get_size(); i++) {
        sorted[i] = vertices[sorted[i][2]];
    }

    return sorted;
}

// Pre-condition: Assume vertices are in 2D space.
inline vector<double> centroid(vector<vector<double>> vertices) {
    vertices = sort_vertices(vertices);
    int64_t N = vertices.get_size();
    // A is the polygon's signed area -> A = (1/2) sum_{i=0}{n-1} [(x_i * y_(i+1)) - (x_(i+1) * y_i)]
    double A = 0.0, Cx = 0.0, Cy = 0.0;
    for (uint64_t i = 0; i < N; i++) {
        A += (vertices[i][0] * vertices[(i+1)%N][1]) - (vertices[(i+1)%N][0] * vertices[i][1]);
    }

    A /= 2.0;

    assert(A != 0);

    for (uint64_t i = 0; i < N; i++) {
        double T = (vertices[i][0] * vertices[(i+1)%N][1] - vertices[(i+1)%N][0] * vertices[i][1]);

        Cx += (vertices[i][0] + vertices[(i+1)%N][0]) * T; 
        Cy += (vertices[i][1] + vertices[(i+1)%N][1]) * T; 
    }

    Cx /= (6*A);
    Cy /= (6*A);

    return create_vec2(Cx, Cy);
}

// Computes the Barycentric Coordinates of a Point (possibly) within a Triangle ABC
// alpha = A(PCB)/A
// beta = A(PCA)/A
// gamma = 1 - (alpha + beta)
// We use the cross product to determine the area of triangles ABC, APC, APB, BPC
// which are then used to determine the weights (alpha, beta, gamma).
inline vector<double> barycentric(vector<double> p,
                                  vector<double> a,
                                  vector<double> b,
                                  vector<double> c) {

    if (p.get_size() == 2) { p = create_vec3(p); }
    if (a.get_size() == 2) { a = create_vec3(a); }
    if (b.get_size() == 2) { b = create_vec3(b); }
    if (c.get_size() == 2) { c = create_vec3(c); }

    double A = ((b-a).cross(c-a).magnitude())/2.0,
           ACB = ((b-p).cross(c-p).magnitude())/2.0,
           ACA = ((a-p).cross(c-p).magnitude())/2.0;
    double alpha = ACB/A, beta = ACA/A, gamma = 1 - (alpha + beta);

    return create_vec3(alpha, beta, gamma);
}

inline bool is_convex_vertex(vector<double> vA, vector<double> vB, vector<double> vC) {
    return (((vB[0] - vA[0]) * (vC[1] - vB[1])) - ((vC[0] - vB[0]) * (vB[1] - vA[1]))) > 0;
}

inline bool is_convex_vertex(vector<vector<double>>& vertices, int64_t idx) {
    assert(idx >= 0 && idx < vertices.get_size());
    int64_t N = vertices.get_size();
    vector<double> vA = vertices[mod(idx-1, N)], vB = vertices[mod(idx, N)], vC = vertices[mod(idx+1, N)];
    return is_convex_vertex(vA, vB, vC);
}

inline bool is_ear_vertex(vector<vector<double>>& vertices, int64_t idx) {
    assert(idx >= 0 && idx < vertices.get_size());
    int64_t N = vertices.get_size();
    vector<double> vA = vertices[mod(idx-1, N)], vB = vertices[mod(idx, N)], vC = vertices[mod(idx+1, N)];

    for (int64_t k = 0; k < N; k++) {
        if (k == mod(idx-1, N) || k == mod(idx, N) || k == mod(idx+1, N)) {
            continue;
        }

        vector<double> bc = barycentric(vertices[k], vA, vB, vC);
        if (!(bc[0] && bc[1] && bc[2])) {
            return false;
        }
    }

    return true;
}

inline void update_polygon_vertex(vector<vector<double>>& vertices, vector<int64_t>& reflex, vector<int64_t>& ears, int64_t idx) {
    if (is_ear_vertex(vertices, idx)) {
        if (reflex.contains(idx)) {
            reflex.remove(reflex.index(idx));
        }
        ears.push_back(idx);
    } else {
        if (ears.contains(idx)) {
            ears.remove(ears.index(idx));
        }
    }
}

inline vector<triangle> triangulate(vector<vector<double>>& vertices, vector<int64_t>& reflex, vector<int64_t>& ears) {
    vector<triangle> out;
    // If the set of vertices does not form a triangle, remove an ear vertex
    // and update the status of it's adjacent vertices
    // { P1, P2, P3, P4, P5 } -> { P1, P2, P4, P5 }
    // We want to update using Angles P1-P2-P4 and P2-P4-P5
    // If we remove index N, we update vertexes (N-1, N, N+1) and (N, N+1, N+2)
    while (vertices.get_size() > 3) {
        int64_t N = vertices.get_size();
        int64_t removal = ears[0];

        triangle t = triangle(vertices[mod(removal-1, N)],
                              vertices[mod(removal, N)],
                              vertices[mod(removal+1,N)]);

        ears.remove(0);
        vertices.remove(removal);

        for (int64_t i = 0; i < ears.get_size(); i++) {
            if (ears[i] > removal) {
                ears[i]--;
            }
        }

        for (int64_t i = 0; i < reflex.get_size(); i++) {
            if (reflex[i] > removal) {
                reflex[i]--;
            }
        }

        update_polygon_vertex(vertices, reflex, ears, removal);

        out.push_back(t);
    }

    out.push_back(triangle(vertices[0], vertices[1], vertices[2]));

    return out;
}


inline vector<triangle> triangulate(vector<vector<double>> vertices) {
    vertices = sort_vertices(vertices);
    vector<int64_t> reflex, ears;
    int64_t N = vertices.get_size();

    // By cosine law: c^2 = a^2 + b^2 - 2*a*b*cos(C)
    // cos(C) = (a^2 + b^2 - c^2)/2*a*b
    for (int64_t i = 0; i < N; i++) {
        vector<double> vA = vertices[mod(i-1, N)], vB = vertices[mod(i, N)], vC = vertices[mod(i+1, N)];

        // If det(b-a, c-b) > 0 --> vertices form a left turn -> implies convex
        // (b.x - a.x) * (c.y - b.y) - (c.x - b.x) * (b.y - a.y)
        if (!is_convex_vertex(vertices, i)) {
            reflex.push_back(i);
        } else {
            if (is_ear_vertex(vertices, i)) {
                ears.push_back(i);
            }
        }
    }

    return triangulate(vertices, reflex, ears);
}

#endif
