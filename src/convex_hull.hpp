#ifndef CONVEX_HULL_HPP
#define CONVEX_HULL_HPP

#pragma once
#include "compare.hpp"
#include "pair.hpp"
#include "list.hpp"
#include "triangle.hpp"

#include "vec.hpp"
#include "mat.hpp"

inline list<vec3<double>> sort_ccw(list<vec3<double>> &points, 
								   vec3<double> p0) {
	list<vec3<double>> sorted = points - p0;

	quicksort(sorted, compare::counter_clockwise);

	sorted += p0;

	return sorted;
}

inline list<vec3<double>> sort_cw(list<vec3<double>> &points,
								  vec3<double> p0) {
	list<vec3<double>> sorted = points - p0;

	quicksort(sorted, compare::clockwise);

	sorted += p0;

	return sorted;
}

inline list<vec2<double>> sort_ccw(list<vec2<double>> &points, vec2<double> p0) {
	list<vec2<double>> sorted = points - p0;

	quicksort(sorted, compare::counter_clockwise);

	sorted += p0;

	return sorted;
}

inline list<vec2<double>> sort_cw(list<vec2<double>> &points, vec2<double> p0) {
	list<vec2<double>> sorted = points - p0;

	quicksort(sorted, compare::clockwise);

	sorted += p0;

	sorted.push_front(sorted.pop_back());

	return sorted;
}

namespace convex_hull {

	// TODO: Fix this -> Same as sorting counter clockwise.
	inline list<vec2<double>> sorted_polar_coords(list<vec2<double>> &points) {
		int64_t N = points.size();

		list<vec2<double>> sorted;

		linked_node<vec2<double>> *current = points.front();

		vec2<double> p0 = points[0];

		for (int64_t k = 0; k < N; k++) {
			if (p0.y() > current->value().y())
				p0 = current->value();

			current = current->next();
		}

		return sort_ccw(points, p0);
	}

	// O(n log n)
	inline list<vec2<double>> graham_scan(list<vec2<double>> &points) {
		int64_t N = points.size();

		list<vec2<double>> sorted = sorted_polar_coords(points), stack;

		linked_node<vec2<double>> *head = sorted.front();

		for (int64_t k = 0; k < N; k++) {
			vec2<double> P = head->value();

			linked_node<vec2<double>> *back = (stack.size() > 1 ? stack.back() : nullptr);

			while (stack.size() > 1 && cross(back->prev()->value(), back->value(), P) < 0) {
				stack.pop_back();
				back = stack.back();
			}

			stack.push_back(P);
			head = head->next();
		}

		return stack;
	}

	// O(n^3)
	inline list<vec2<double>> brute_force(list<vec2<double>> &points) {
		list<vec2<double>> hull;

		int64_t N = points.size();

		// For every possible line in the set of points, it checks whether all points are on the same side of the line.
		// Points on the same side of a line can be checked using a cross product -> cross(a,b,c)
		
		linked_node<vec2<double>> *node_a = points.front();

		for (int64_t k = 0; k < N; k++) {
			vec2<double> A = node_a->value();
			linked_node<vec2<double>> *node_b = node_a->next();
			for (int64_t i = k+1; i < N; i++) {
				vec2<double> B = node_b->value();

				bool valid_segment = true;
				int64_t K = 0xFF;

				linked_node<vec2<double>> *node_c = points.front();

				for (int64_t j = 0; j < N; j++) {
					if (j != k && j != i) {
						vec2<double> C = node_c->value();

						int64_t D = direction(A, B, C);

						if (K == 0xFF) K = D;

						if (K != D) { 
							valid_segment = false; 
							break;
						}
					}

					node_c = node_c->next();
				}

				if (valid_segment) {
					hull.push_back(A);
					hull.push_back(B);
				}

				node_b = node_b->next();
			}
			
			node_a = node_a->next();
		}

		quicksort(hull, &compare::x);

		return remove_consec_dupes(hull);
	}

	// Assume A is sorted in a CW order from the right-most point, 
	// and B is sorted in a CCW order from the left-most point.
	inline pair<linked_node<vec2<double>>*, linked_node<vec2<double>>*> merge_bottom(list<vec2<double>> &A, list<vec2<double>> &B) {
		linked_node<vec2<double>> *L = A.front(), 
								  *R = B.front();

		// Bottom Tangent.
		while (true) {
			linked_node<vec2<double>> *NL = L->next(),
									  *NR = R->next();

			int64_t DL = direction(L->value(), R->value(), NL->value()),
					RL = direction(L->value(), R->value(), NR->value());

			bool G = (L->value().y() < R->value().y());

			if ((G && DL == -1) || (!G && DL == 1)) {
				L = NL;
			} else if ((G && RL == -1) || (!G && RL == 1)) {
				R = NR;
			} else {
				break;
			}
		}

		return pair(L, R);
	}

	inline pair<linked_node<vec2<double>>*, linked_node<vec2<double>>*> merge_top(list<vec2<double>> &A, list<vec2<double>> &B) {
		// L = P, R = Q 
		linked_node<vec2<double>> *L = A.back(),
								  *R = B.back();

		while (true) {
			linked_node<vec2<double>> *PL = L->prev(), 
									  *PR = R->prev();

			int64_t DL = direction(L->value(), R->value(), PL->value()),
					RL = direction(L->value(), R->value(), PR->value());

			bool G = (L->value().y() < R->value().y());

			if ((G && DL == 1) || (!G && DL == -1)) {
				L = PL;
			} else if ((G && RL == 1) || (!G && RL == -1)) {
				R = PR;
			} else {
				break;
			}
		}

		return pair(L, R);
	}

	inline list<vec2<double>> merge(list<vec2<double>> &L, list<vec2<double>> &R) {
		quicksort(L, &compare::x);
		quicksort(R, &compare::x);

		vec2<double> P = L.back()->value(), Q = R.front()->value();

		list<vec2<double>> A = sort_cw(L, P), B = sort_ccw(R, Q);

		pair<linked_node<vec2<double>>*, linked_node<vec2<double>>*> bottom = convex_hull::merge_bottom(A, B);

		A.push_back(A.pop_front());
		B.push_back(B.pop_front());

		pair<linked_node<vec2<double>>*, linked_node<vec2<double>>*> top = convex_hull::merge_top(A, B);

		list<vec2<double>> merge;
		linked_node<vec2<double>> *curr_l = top.key(), *curr_r = bottom.value(),
								  *end_l = bottom.key(), *end_r = top.value();

		while (curr_l->next() != end_l) {
			merge.push_back(curr_l->value());
			curr_l = curr_l->prev();		
		} 

		while (curr_r->prev() != end_r) {
			merge.push_back(curr_r->value());
			curr_r = curr_r->next();
		}

		return merge;
	}

	// O(n log n)
	inline list<vec2<double>> divide_conquer(list<vec2<double>> &points,
											 bool sorted = false) {
		if (!sorted) 
			quicksort(points, &compare::x);

		int64_t N = points.size();

		if (N <= 5) 
			return convex_hull::brute_force(points);

		list<vec2<double>> L, R;

		linked_node<vec2<double>> *current = points.front();

		for (int64_t k = 0; k < N; k++) {
			(k < N/2 ? L.push_back(current->value()) : R.push_back(current->value()));
	
			current = current->next();
		}

		list<vec2<double>> left = convex_hull::divide_conquer(L),
						   right = convex_hull::divide_conquer(R);

		return merge(left, right);
	}

	// O(n^4)
	inline list<triangle> brute_force(list<vec3<double>> &points) {
		list<vec3<double>> hull;

		int64_t N = points.size();

		linked_node<vec3<double>> *node_a = points.front();

		for (int64_t k = 0; k < N; k++) {
			vec3<double> A = node_a->value();
			linked_node<vec3<double>> *node_b = node_a->next();

			for (int64_t i = k+1; i < N; i++) {
				vec3<double> B = node_b->value();

				linked_node<vec3<double>> *node_c = node_b->next();
				for (int64_t j = i+1; j < N; j++) {
					vec3<double> C = node_c->value();

					bool valid_segment = true;
					int64_t sign = 0xFF;

					linked_node<vec3<double>> *node_d = points.front();

					for (int64_t l = 0; l < N; l++) {
						if ((l != k && l != i && l != j) && 
							(A != B && A != C && B != C)) {
							vec3<double> D = node_d->value();

							int64_t E = direction(A, B, C, D);

							if (sign == 0xFF || sign == 0) 
								sign = E;

							if (sign != E) {
								valid_segment = false;
								break;
							}
						}

						node_d = node_d->next();
					}

					if (valid_segment) {
						hull.push_back(A);
						hull.push_back(B);
						hull.push_back(C);
					}

					node_c = node_c->next();
				}

				node_b = node_b->next();
			}

			node_a = node_a->next();
		}

		list<triangle> triangles;

		linked_node<vec3<double>> *node = hull.front();

		for (int64_t k = 0 ; k < hull.size(); k += 3) {
			linked_node<vec3<double>> *n = node->next(),
									  *nn = n->next();

			vec3<double> p0 = node->value(),
						 p1 = n->value(),
						 p2 = nn->value();

			triangle T = triangle(p0, p1, p2);

			triangles.push_back(T);

			node = nn->next();
		}

		return triangles;
	}
};

#endif
