#pragma once
#include <iostream>
#include "errors.hpp"
#include "vector.hpp"

template <typename T>
class Tree {
protected:
	struct Node {
		T value;
		Array<Node*> children;

		Node(T val) { value = val; }
	};

	Node* root;

public:
	Tree() { root = nullptr; }
	virtual ~Tree() { clear(root); }

	void addChild(T parentValue, T childvalue) {
		if (root == nullptr) {
			root = new Node(parentValue);
		}

		Node* parent = findNode(root, parentValue);
		if (parent == nullptr) return;

		parent->children.pushback(new Node(childvalue));
	}

	Node* findNode(Node* node, T value) {
		if (node == nullptr) return;
		if (node->value == value) return node;

		for (int i = 0; i < node->children.getsize(); i++) {
			Node* found = findNode(node->children[i].value);
			if (found) return found;
		}
		return nullptr;
	}

	bool contains(T value) {
		return findNode(root, value) != nullptr;
	}

	Node* getRoot() const { return root; }

	void clear(Node* node) {
		if (node == nullptr) return;
		for (int i = 0; i < node->children.getsize(); ++i) {
			clear(node->children[i]);
		}
		delete node;
	}

	bool remove(T value) {
		if (root == nullptr) return false;

		if (root->value == value) {
			clear(root); root = nullptr;
			return true;
		}

		Node* parent = findParent(root, value);
		if (parent == nullptr) return false;

		bool result = false;

		for (int i = 0; i < parent->children.getsize() && result == false; i++) {
			if (parent->children[i]->value == value) {
				clear(parent->children[i]);
				for (int j = i; j < parent->children.getsize() - 1; j++) {
					parent->children[j] = parent->children[j + 1];
				}
				result = true;
			}
		}
		return result;
	}
};
