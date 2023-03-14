#ifndef _list_h_
#define _list_h_

#include <stddef.h>
#include <iostream.h>
#include "common.h"

template <class T>
class List {
public:

	struct Node {
		T value;
		Node* next;
		Node(const T& val, Node* next = NULL) : value(val), next(next) {}
	};

	List() : head(0), tail(0), size(0) {}
	~List() {
		while (head) {
			Node* temp = head;
			head = head->next;
			delete temp;
		}
		tail = 0;
		size = 0;
	}
	T& getHead() {
		return head->value;
	}

	const T& getHead() const {
		return head->value;
	}

	unsigned getSize() const { return size; }

	void addToTail(T val) {
		Node* temp = new Node(val);
		if (!head) head = tail = temp;
		else {
			tail->next = temp;
			tail = temp;
		}
		size++;
	}

	void deleteHead() {
		if (!head) return;
		Node* temp = head;
		head = head->next;
		delete temp;
		size--;
		if (size == 0) head = tail = 0;
	}

	void eraseById(int id) {
		if (id > size) return;
		if (id == 1) {
			deleteHead();
			return;
		}
		Node* temp = head;
		Node* prev = 0;
		for (int i = 0; i < size; i++) {
			if (id == (i + 1)) {
				if (id == size) {
					delete temp;
					tail = prev;
					tail->next = 0;
					size--;
					return;
				}
				prev->next = temp->next;
				delete temp;
				size--;
				return;
			}
			prev = temp;
			temp = temp->next;

		}
	}

	void erase(T t) {
		Node* temp = head;
		for (int i = 0; i < size; i++) {
			if (t == temp->value) {
				eraseById(i + 1);
				return;
			}
			temp = temp->next;
		}
		return;
	}

private:
	Node* head, * tail;
	unsigned size;
	friend class Iterator;

public:
	class Iterator {
	public:
		Iterator(List<T>* lst, Node* p) : myList(lst), curr(p) {}

		void operator++() {
			if (curr) curr = curr->next;
		}

		T& operator*() {
			return curr->value;
		}

		int operator==(const Iterator& iter) const {
			return curr == iter.curr;
		}

		int operator!=(const Iterator& iter) const {
			return !(*this == iter);
		}

	private:
		Node* curr;
		List<T>* myList;
	};

	Iterator begin() { return Iterator(this, head); }

	Iterator end() { return Iterator(this, 0); }
};

#endif // !_list_h_


