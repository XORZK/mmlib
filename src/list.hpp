#ifndef LIST_H
#define LIST_H

#pragma once
#include "node.hpp"
#include <assert.h>
#include <stdint.h>

template <typename T> 
inline bool default_qsort_cmp(T x1, T x2) {
	return (x1 <= x2);
}

template <typename T> class list {
    private:
        int64_t s;
        linked_node<T> *head, *tail;
    public:
        list() : s(0), head(nullptr), tail(nullptr) {}
        list(linked_node<T> node) : s(1), head(&node), tail(&node) {}
        list(T value) : s(1) {
            linked_node<T> node(value);
            this->head = this->tail = &node;
        }
		list(const list<T> &l);

        ~list() {}

        void push_front(linked_node<T>* node);
        void push_front(T value);

        void push_back(linked_node<T>* node);
        void push_back(T value);

        void insert(int64_t idx, linked_node<T>* node);
        void insert(int64_t idx, T value);


        T pop_front();
        T pop_back();
        void remove(int64_t idx);
        void clear();
        void resize(int64_t new_size);

        int64_t index(T data) const;
        int64_t size() const;
        bool is_empty() const;

        void reverse();
        void sort();
        list<T> merge(list<T> ll) const;
		void remove_dupes();

        linked_node<T>* front() const;
        linked_node<T>* back() const;
        linked_node<T> get(int64_t idx) const;
        linked_node<T>* get_ptr(int64_t idx) const;

        T operator[](int64_t idx) const;
		list<T> operator+(T value) const;
		list<T> operator-(T value) const;
		void operator+=(T value);
	 	void operator-=(T value);

        void swap(int64_t k, int64_t j);

        template <typename U>
        friend std::ostream& operator<<(std::ostream& out, const list<U>& l);
};

template <typename T> list<T>::list(const list<T> &l) {
	linked_node<T> *curr = l.front();

	for (int64_t k = 0; k < l.size(); k++) {
		this->push_back(curr->value());
		curr = curr->next();
	}
}

template <typename T> void list<T>::push_front(linked_node<T>* node) {
    if (this->is_empty()) {
        this->head = this->tail = node;
    } else {
        node->next(this->head);
        this->head = node;
    }
    ++this->s;
}

template <typename T> void list<T>::push_front(T value) {
    linked_node<T> *node = new linked_node<T>(value);
    this->push_front(node);
}

template <typename T> void list<T>::push_back(linked_node<T>* node) {
    if (this->is_empty()) {
        this->head = this->tail = node;
    } else {
        node->prev(this->tail);
        this->tail = node;
    }
    ++this->s;
}

template <typename T> void list<T>::push_back(T value) {
    linked_node<T>* node = new linked_node<T>(value);
    this->push_back(node);
}

template <typename T> void list<T>::insert(int64_t idx, linked_node<T>* node) {
    assert(idx >= 0 && idx < this->s);

    if (idx == 0) this->push_front(node);
    else if (idx == this->s-1) this->push_back(node);
    else {
        linked_node<T>* loc = this->get_ptr(idx);
        loc->prev()->next(node);
        loc->prev(node);
    }
    ++this->s;
}

template <typename T> void list<T>::insert(int64_t idx, T value) { 
    linked_node<T> node(value);
    this->insert(idx, node);
}

template <typename T> T list<T>::pop_front() {
    if (!this->is_empty()) {
        linked_node<T>* removed_front = this->head;
        this->head = this->head->next();
        if (--this->s == 0) {
            this->tail = nullptr;
        }

        return removed_front->value();
    }

    throw std::out_of_range("The indexed list is empty");
}

template <typename T> T list<T>::pop_back() {
    if (!this->is_empty()) {
        linked_node<T>* removed_back = this->tail;

        this->tail = this->tail->prev();
        this->tail->next(nullptr);

        if (--this->s == 0) {
            this->head = nullptr;
        }

        return removed_back->value();
    }

    return static_cast<T>(0);
}

template <typename T> void list<T>::remove(int64_t idx) {
    if (idx == -1) 
        return;

    assert(idx >= 0 && idx < this->s);

    if (idx == 0) this->pop_front();
    else if (idx == this->s-1) this->pop_back();
    else {
        linked_node<T>* removed = this->get_ptr(idx);
        
        removed->prev()->next(removed->next());

        --this->s;
    }
}

template <typename T> void list<T>::clear() {
    this->head = this->tail = nullptr;
    this->s = 0;
}

template <typename T> void list<T>::resize(int64_t new_size) {
    if (new_size < this->s && new_size > 0) {
        linked_node<T>* new_tail = this->get_ptr(new_size-1);
        this->tail = new_tail;
        this->s = new_size;
    } else if (new_size == 0) {
        this->clear();
    }
}

template <typename T> int64_t list<T>::index(T data) const {
    int64_t idx = 0;
    linked_node<T> *ptr = this->head;

    while (ptr != nullptr) {
        if (ptr->value() == data) {
            return idx;
        }

        ptr = ptr->next();
        idx++;
    }

    return -1;
}

template <typename T> int64_t list<T>::size() const { return (this->s); } 

template <typename T> bool list<T>::is_empty() const { return (this->head == nullptr); }

template <typename T> linked_node<T>* list<T>::get_ptr(int64_t idx) const {
    assert(idx >= 0 && idx < this->s);
    linked_node<T> *ptr = this->head;

    for (int64_t k = 0; k < idx; k++) {
        ptr = (ptr->next());
    }

    return ptr;
}

template <typename T> T list<T>::operator[](int64_t idx) const {
    return this->get_ptr(idx)->value();
}

template <typename T> list<T> list<T>::operator+(T value) const {
	list<T> sum;

	linked_node<T> *curr = this->head;

	for (int64_t k = 0; k < this->s; k++) {
		sum.push_back(curr->value() + value);

		curr = curr->next();
	}

	return sum;
}

template <typename T> list<T> list<T>::operator-(T value) const {
	list<T> diff;

	linked_node<T> *curr = this->head;

	for (int64_t k = 0; k < this->s; k++) {
		diff.push_back(curr->value() - value);
		curr = curr->next();
	}

	return diff;
}

template <typename T> void list<T>::operator+=(T value) {
	linked_node<T> *curr = this->head;

	for (int64_t k = 0; k < this->s; k++) {
		curr->value(curr->value() + value);
		curr = curr->next();
	}
}

template <typename T> void list<T>::operator-=(T value) {
	linked_node<T> *curr = this->head;

	for (int64_t k = 0; k < this->s; k++) {
		curr->value(curr->value() - value);
		curr = curr->next();
	}
}

template <typename T> void list<T>::swap(int64_t k, int64_t j) {
    assert(("Out of Bounds Error: List Index Out of Bounds",
           (k >= 0 && k < this->s && j >= 0 && j < this->s)));

    if (k == j) 
        return;

    linked_node<T> *a = this->get_ptr(k), *b = this->get_ptr(j);

    T tmp = a->value();
    a->value(b->value());
    b->value(tmp);
}

// <A> -> <B> -> <C> ... -> <N>
// <A> <- <B> <- <C> ... <- <N>
//
template <typename T> void list<T>::reverse() {
    linked_node<T> *front = this->head, *back = this->tail;

    for (uint64_t k = 0; k <= (this->s/2 - (this->s+1)%2) && front != nullptr && back != nullptr; k++) {
		std::swap(front, back);
        front = front->next();
        back = back->prev();
    }
}

template <typename T> 
inline linked_node<T>* partition(linked_node<T> *start, linked_node<T> *end, bool (*cmp) (T, T)) {
	if (start == nullptr || end == nullptr || start == end)
		return start;

	T pivot = end->value();

	linked_node<T> *swap_node = start->prev(),
				   *curr_node = start;

	while (curr_node != end) {
		if (cmp(curr_node->value(), pivot)) {
			swap_node = (swap_node == nullptr ? start : swap_node->next());
			swap(curr_node, swap_node);
		}

		curr_node = curr_node->next();
	}

	swap_node = (swap_node == nullptr ? start : swap_node->next());

	swap(swap_node, end);

	return swap_node;
}

template <typename T> 
inline void quicksort(linked_node<T> *start, linked_node<T> *end, bool (*cmp) (T, T)) {
	if ((start == nullptr) || (end == nullptr) || (start == end) || (start->prev() == end)) {
		return;
	}

	linked_node<T>* p = partition(start, end, cmp);

	quicksort(start, p->prev(), cmp);
	quicksort(p->next(), end, cmp);
}

template <typename T> 
inline void quicksort(list<T> &l, bool (*cmp)(T, T)) {
	quicksort(l.front(), l.back(), cmp);
}

template <typename T> 
inline void quicksort(list<T> &l) { 
	quicksort(l, &default_qsort_cmp); 
}

// Quicksort
template <typename T> void list<T>::sort() {
    quicksort(*this, 0, this->s-1, &default_qsort_cmp);
}

template <typename T> list<T> list<T>::merge(list<T> ll) const {
    list<T> merged;

    linked_node<T>* first = this->head, second = ll.front();

    for (int64_t k = 0; k < this->s; k++) {
        merged.push_back(first->value());
        first = first->next();
    }

    for (int64_t k = 0; k < ll.size(); k++) {
        merged.push_back(second->value());
        second = second->next();
    }

    return merged;
}

template <typename T> linked_node<T>* list<T>::front() const {
    return this->head;
}

template <typename T> linked_node<T>* list<T>::back() const {
    return this->tail;
}

template <typename T> linked_node<T> list<T>::get(int64_t idx) const {
    assert(idx >= 0 && idx < this->s);

    return *(this->get_ptr());
}

template <typename T> std::ostream& operator<<(std::ostream& out, const list<T>& l) {
    linked_node<T>* current = l.head;

    while (current != nullptr) {
        out << current;
        current = current->next();
    }

    return out;
}

template <typename T> std::ostream& operator<<(std::ostream& out, const list<T>* l) {
    out << *l;
    return out;
}

template <typename T> int64_t binary_search(list<T> l, T value) {
    int64_t L = 0, R = l.size()-1;

    while (L <= R) {
        int64_t M = (L+R)/2;

        T s = l[M];

        if (s == value) return M;
        else if (s > value) R = M - 1;
        else L = M + 1;
    }

    return -1;
}


// Assumes input list is ordered.
template <typename T> list<T> remove_consec_dupes(list<T> &l) {
	list<T> set;

	linked_node<T> *curr = l.front();

	for (int64_t k = 0; k < l.size(); k++) {
		if (set.size() == 0 || set.back()->value() != curr->value())
			set.push_back(curr->value());

		curr = curr->next();
	}

	return set;
}

template <typename T> list<T> remove_dupes(list<T> &l) {
	list<T> set;

	linked_node<T> *curr = l.front();

	for (int64_t k = 0; k < l.size(); k++) {
		int64_t idx = set.index(curr->value());
		if (set.size() == 0 || idx == -1) {
			set.push_back(curr->value());
		}

		curr = curr->next();
	}

	return set;
}

#endif
