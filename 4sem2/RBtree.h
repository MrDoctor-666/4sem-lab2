#pragma once
#include"Stack.h"

template <class K, class V>
class RedBlackTree {
private:
	class Node {
	public:
		K key;
		V value;
		Node* parent;
		Node* left;
		Node* right;
		bool color; //0 - red, 1 - black
		Node(K key, V value, Node* left = nullptr, Node* right = nullptr, Node* parent = nullptr, bool color = 0) {
			this->key = key;
			this->value = value;
			this->left = left;
			this->right = right;
			this->parent = parent;
			this->color = color;
		}
		~Node() {};
		//friend class RedBlackTree;
	};

	class dft_Iterator // depth-first traverse
	{
	private:
		Stack<Node*> stack;
		Node* Icurrent;
		Node* nil;
	public:
		dft_Iterator(Node* cur, Node* nill) {
			Icurrent = cur;
			nil = nill;
		};
		Node* next() {
			Node* temp = Icurrent;
			if (Icurrent->right != nil) stack.push(Icurrent->right);
			if (Icurrent->left != nil) {
				Icurrent = Icurrent->left;
			}
			else {
				if (!stack.isEmpty()) Icurrent = stack.pop();
				else Icurrent = nil;
			}
			return temp;
		};
		bool has_next() {
			if (Icurrent != nil)
				return true;
			return false;
		};
		~dft_Iterator() {

		}
	};

	Node* root;
	Node* nil;
	int size;

	void recovery(Node*); //recover properties after input
	void leftRotate(Node*); 
	void rightRotate(Node*);
	void recoveryRemove(Node* x); //recover properties after remove

public:
	RedBlackTree() { 
		nil = new Node(0, 0, nullptr, nullptr, nullptr, 1);
		root = nil; 
		size = 0;
	}
	~RedBlackTree() { clear(); delete root; }

	void insert(K, V); //add element with key and value
	void remove(K); //remove element by key
	V find(K); //find element by key
	void clear(); //clear all
	K* get_keys(); //return all keys
	V* get_values(); //return values
	void print(); //print in console

	bool get_color(K); //for tests
	int get_size() { return size; }

	friend class Node;
};

template<typename K, typename V>
void RedBlackTree<K, V>::insert(K key, V value) {
	Node* cur = root, * parent;
	bool moveRight;
	size++;

	if (root == nil) { root = new Node(key, value, nil, nil, nil, 1); return; } //Tree is empty
	parent = cur;
	while (cur != nil) {
		parent = cur;
		if (key > cur->key) {
			cur = cur->right; moveRight = true;
		} //right
		else {
			cur = cur->left; moveRight = false;
		} //left
	}

	if (moveRight) {
		parent->right = new Node(key, value, nil, nil, parent, 0);
		recovery(parent->right);
	}
	else {
		parent->left = new Node(key, value, nil, nil, parent, 0);
		recovery(parent->left);
	}

}

template<typename K, typename V>
void RedBlackTree<K, V>::recovery(Node* newNode) {
	Node* parent, * grandParent;
	while (newNode->parent->color == 0) {
		parent = newNode->parent;
		grandParent = parent->parent;
		if (grandParent->left == parent) {
			//3 cases
			if (grandParent->right->color == 0) {
				//I
				grandParent->right->color = 1;
				grandParent->left->color = 1;
				grandParent->color = 0;
				newNode = grandParent;
			}
			else {
				if (parent->right == newNode) {
					//II
					newNode = parent;
					leftRotate(newNode);
				}
					//III
				newNode->parent->color = 1;
				newNode->parent->parent->color = 0;
				rightRotate(newNode->parent->parent);
			}
		}
		else {
			if (grandParent->left->color == 0) {
				//I
				grandParent->right->color = 1;
				grandParent->left->color = 1;
				grandParent->color = 0;
				newNode = grandParent;
			}
			else {
				if (parent->left == newNode) {
					//II
					newNode = parent;
					rightRotate(newNode);
				}
				//III
				newNode->parent->color = 1;
				newNode->parent->parent->color = 0;
				leftRotate(newNode->parent->parent);
			}
		}
	}
	root->color = 1;
}

template<typename K, typename V>
void RedBlackTree<K, V>::leftRotate(Node* curx) {
	if (curx->right == nil) return;
	Node* cury = curx->right;

	if (cury->left != nil) {
		curx->right = cury->left;
		cury->left->parent = curx;
	}
	else curx->right = nil;

	if (cury != nil) cury->parent = curx->parent;
	if (curx->parent != nil) {
		if (curx == curx->parent->left)
			curx->parent->left = cury;
		else
			curx->parent->right = cury;
	}
	else {
		cury->parent = nil;
		root = cury;
	}
	// connect curx and cury
	cury->left = curx;
	if (curx != nil) curx->parent = cury;
	
}

template<typename K, typename V>
void RedBlackTree<K, V>::rightRotate(Node* curx) {

	Node* cury = curx->left;

	curx->left = cury->right;
	if (cury->right != nil) cury->right->parent = curx;

	if (cury != nil) cury->parent = curx->parent;
	if (curx->parent != nil) {
		if (curx == curx->parent->right)
			curx->parent->right = cury;
		else
			curx->parent->left = cury;
	}
	else {
		root = cury;
	}

	cury->right = curx;
	if (curx != nil) curx->parent = cury;
}

template<typename K, typename V>
void RedBlackTree<K, V>::remove(K key) {
	if (root == nil) throw std::out_of_range("Tree is empty");
	Node* nodeToDelete = root;
	while (nodeToDelete->key != key) {
		if (key > nodeToDelete->key) nodeToDelete = nodeToDelete->right;
		else nodeToDelete = nodeToDelete->left;
		if (nodeToDelete == nil) throw std::out_of_range("No such element"); //error
	}

	Node* x, *min;
	bool originalColor = nodeToDelete->color;
	if (nodeToDelete->left == nil) {
		//only one branch, deleting nodeToDelete 
		//putting his child instead of him
		x = nodeToDelete->right;
		if (nodeToDelete->parent == nil) root = x;
		else if (nodeToDelete->parent->left == nodeToDelete) nodeToDelete->parent->left = x;
		else nodeToDelete->parent->right = x;
		x->parent = nodeToDelete->parent;
	}
	else if (nodeToDelete->right == nil) {
		//same as before - other branch is existing
		x = nodeToDelete->left;
		if (nodeToDelete->parent == nil) root = x;
		else if (nodeToDelete->parent->left == nodeToDelete) nodeToDelete->parent->left = x;
		else nodeToDelete->parent->right = x;
		x->parent = nodeToDelete->parent;
	}
	else {
		Node* temp = nodeToDelete;
		min = nodeToDelete->right;
		while (min->left != nil) min = min->left;
		originalColor = min->color;
		nodeToDelete = min;
		x = nodeToDelete->right;
		if (nodeToDelete->parent == temp) x->parent = nodeToDelete;
		else {
			//swap NodetoDelete and NodetoDelete->right (x)
			if (nodeToDelete->parent == nil) {
				root = x; 
				nodeToDelete->right->parent = nil;
			}
			else if (nodeToDelete->parent->right == nodeToDelete) nodeToDelete->parent->right = x;
			else nodeToDelete->parent->left = x; 
			x->parent = nodeToDelete->parent;

			nodeToDelete->right = temp->right;
			nodeToDelete->right->parent = nodeToDelete;


		}
		//swap min and nodeToDelete
		if (temp->parent == nil) root = nodeToDelete;
		else if (temp == temp->parent->left) temp->parent->left = nodeToDelete;
		else temp->parent->right = nodeToDelete;

		nodeToDelete->parent = temp->parent;
		nodeToDelete->left = temp->left;
		nodeToDelete->left->parent = nodeToDelete;
		nodeToDelete->color = temp->color; //?
	}

	if (originalColor == 1) recoveryRemove(x);
	size--;
}

template<typename K, typename V>
void RedBlackTree<K, V>::recoveryRemove(Node* x) {
	Node* brother;
	while (x != root && x->color == 1) {
		if (x == x->parent->left) {
			brother = x->parent->right;
			if (brother->color == 0) {
				brother->color = 1;
				x->parent->color = 0;
				leftRotate(x->parent);
				brother = x->parent->right;
			}
			if (brother->left->color == 1 && brother->right->color == 1) {
				brother->color = 0;
				x = x->parent;
			}
			else {
				if (brother->right->color == 1) {
					brother->left->color = 1;
					brother->color = 0;
					rightRotate(brother);
					brother = x->parent->right;
				}
				brother->color = x->parent->color;
				x->parent->color = 1;
				brother->right->color = 1;
				leftRotate(x->parent);
				x = root;
			}
		}
		else {
			brother = x->parent->left;
			if (brother->color == 0) {
				brother->color = 1;
				x->parent->color = 0;
				rightRotate(x->parent);
				brother = x->parent->left;
			}
			if (brother->right->color == 1 && brother->left->color == 1) {
				brother->color = 0;
				x = x->parent;
			}
			else {
				if (brother->left->color == 1) {
					brother->right->color = 1;
					brother->color = 0;
					leftRotate(brother);
					brother = x->parent->left;
				}
				brother->color = x->parent->color;
				x->parent->color = 1;
				brother->left->color = 1;
				rightRotate(x->parent);
				x = root;
			}
		}
	}
	x->color = 1;
}

template<typename K, typename V>
void RedBlackTree<K, V>::clear() {
	while (root != nil) {
		remove(root->key); std::cout << std::endl; print();
	}
}

template<typename K, typename V>
V RedBlackTree<K, V>::find(K findKey) {
	Node* cur = root;
	while (cur->key != findKey) {
		if (findKey > cur->key) cur = cur->right;
		else cur = cur->left;
		if (cur == nil) throw std::out_of_range("No such element"); //error
	}

	return cur->value;
}

template<typename K, typename V>
K* RedBlackTree<K, V>::get_keys() {
	K* arr = new K[size]; int i = 0;
	dft_Iterator iter(root, nil);
	while (iter.has_next())
	{
		arr[i] = iter.next()->key; i++;
	}
	return arr;
}

template<typename K, typename V>
V* RedBlackTree<K, V>::get_values() {
	V* arr = new V[size]; int i = 0;
	dft_Iterator iter(root, nil);
	while (iter.has_next())
	{
		arr[i] = iter.next()->value; i++;
	}
	return arr;
}

template<typename K, typename V>
void RedBlackTree<K, V>::print() {
	dft_Iterator iter(root, nil);
	Node* cur;
	while (iter.has_next())
	{
		cur = iter.next();
		std::cout << "\nKey: " << cur->key << " Color: " << cur->color;
		std::cout << "    Kids " << cur->left->key << " " << cur->right->key;
	}
}

template<typename K, typename V>
bool RedBlackTree<K, V>::get_color(K findKey) {
	Node* cur = root;
	while (cur->key != findKey) {
		if (findKey > cur->key) cur = cur->right;
		else cur = cur->left;
		if (cur == nil) throw std::out_of_range("No such element"); //error
	}

	return cur->color;
}

#include "Stack.cpp"