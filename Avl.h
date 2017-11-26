/*
 * Avl.h
 *
 *  Created on: 2 במאי 2017
 *      Author: Neil-Liel-Tu
 */

#ifndef AVL_H_
#define AVL_H_

#include <iostream>
#include <assert.h>
#include <cstdlib>

template<class T>
class AVLnode {
public:
	T obj;
	int balance;
	AVLnode<T> *left, *right, *parent;

	AVLnode() : obj(), balance(), left(NULL), right(NULL), parent(NULL) {}

	AVLnode(const T& o) : obj(o), balance(0), left(NULL),
			right(NULL), parent(NULL) {};

	~AVLnode() {
		left = NULL;
		right = NULL;
		parent = NULL;
	}

	void setBalance(AVLnode<T> n);
	int height(AVLnode<T>* root);

};

template<class T>
void setBalance(AVLnode<T>* n) {
	n->balance = height(n->left) - height(n->right);
}

template<class T>
int height(AVLnode<T>* node) {

	if(!node) return 0;
	int left_height,right_height;

	left_height = height(node->left);

	right_height = height(node->right);
	return (left_height > right_height) ? (left_height + 1) : (right_height + 1);

}

template<class T>
class AVLtree {

	AVLnode<T>* root;
	int num_of_nodes;
	AVLnode<T>* rotateRight(AVLnode<T>* n);
	AVLnode<T>* rotateLeft(AVLnode<T>* n);
	AVLnode<T>* rotateLR(AVLnode<T>* n);
	AVLnode<T>* rotateRL(AVLnode<T>* n);

	void recursiveInsert(AVLnode<T>* root, AVLnode<T>* new_node);
	void recursiveDelete(AVLnode<T>* root);
	void rebalanceAfterRemove(AVLnode<T>* node);
	void removeNodeWithZeroSons(AVLnode<T>* nodeToRomove);
	void removeNodeWithOneSon(AVLnode<T>* nodeToRomove);
	void removeNodeWithTwoSons(AVLnode<T>* nodeToRomove);
	void removeEmptyLeaf(AVLnode<T>* node);

public:
	AVLtree(): root(NULL) ,num_of_nodes(0) {};
	~AVLtree() {

		if(root) recursiveDelete(root);
		num_of_nodes = 0;
	}

	bool insert(const T& obj);
	bool remove(const T& obj);

	AVLnode<T>* find(AVLnode<T>* root, const T& obj) const;

	AVLnode<T>* getRoot() {
		if(root) return root;
		else return NULL;
	}

	bool isEmpty() const {

		if(!root) return true;
		if(num_of_nodes == 0) return true;
		return false;
	}

	AVLnode<T>* getMax() const {

		if(!root) return NULL;
		else {
			AVLnode<T>* node= root;
			while(node->right) node = node->right;
			return node;
		}
	}

	int NumOfNodes() const{
		return num_of_nodes;
	}

	void addNumOfNodes(const int num){
		num_of_nodes = num;
	}

	void printInorder(AVLnode<T>* root) {
		if(root) {
			printInorder(root->left);
			std::cout << root->obj << std::endl;
			printInorder(root->right);
		}

	}

	int insertToTree(AVLnode<T>* p, T arr[], int i) {
		if(p){
			i = insertToTree(p->left,arr,i);
			p->obj = arr[i++];
			i = insertToTree(p->right,arr,i);
		}
		return i;

	}


	void reverseInorderRemove(AVLnode<T>* node ,int n){
		if(num_of_nodes == n) return;
		if(node->right){
			reverseInorderRemove(node->right,n);
		}
		if(!node->left){
			removeEmptyLeaf(node);
		}else{
			reverseInorderRemove(node->left,n);
		}
	}


	T* moveIntoArray() const;
};

template<class T>
static int intoArray(AVLnode<T>* root,T* array,int i) {

	if(root) {
		i = intoArray(root->left,array,i);
		array[i++] = root->obj;
		i = intoArray(root->right,array,i);

	}

	return i;
}


template<class T>
T* AVLtree<T>::moveIntoArray() const {

	T* array = (T*)std::malloc(sizeof(T)*num_of_nodes);
	if(!array)	return NULL;
	intoArray(root,array,0);
	return array;
}


template<class T>
AVLnode<T>* AVLtree<T>::rotateRight(AVLnode<T>* n) {

	AVLnode<T>* nl = n->left;
	nl->parent = n->parent;

	n->left = nl->right;
	if(n->left) {
		n->left->parent = n;
	}

	nl->right = n;
	n->parent = nl;

	if(nl->parent) { //parent exits
		if(nl->parent->right) {
			if(nl->parent->right == n) nl->parent->right = nl;
			else nl->parent->left = nl;
		}
	} else {
		root = nl;
	}

	setBalance(n);
	setBalance(nl);

	return nl;
}

template<class T>
AVLnode<T>* AVLtree<T>::rotateLeft(AVLnode<T>* n) {

	AVLnode<T>* nr = n->right;
	nr->parent = n->parent;

	n->right = nr->left;
	if(n->right) {
		n->right->parent = n;
	}

	nr->left = n;
	n->parent = nr;

	if(nr->parent) { //parent exits
		if(nr->parent->right) {
			if(nr->parent->right == n) nr->parent->right = nr;
			else nr->parent->left = nr;
		}

	} else {
		root = nr;
	}


	setBalance(n);
	setBalance(nr);

	return nr;
}

template<class T>
AVLnode<T>* AVLtree<T>::rotateLR(AVLnode<T>* n) {
	n->left = rotateLeft(n->left);
	return rotateRight(n);

}

template<class T>
AVLnode<T>* AVLtree<T>::rotateRL(AVLnode<T>* n) {
	n->right = rotateRight(n->right);
	return rotateLeft(n);
}

template<class T>
AVLnode<T>* AVLtree<T>::find(AVLnode<T>* root, const T& obj) const {

	if(root) {
		if(root->obj == obj)
			return root;
		else if(root->obj < obj)
			return find(root->right, obj);
		else
			return find(root->left, obj);
	}

	return NULL; //object was not found in the tree
}

template<class T>
void AVLtree<T>::recursiveInsert(AVLnode<T>* root, AVLnode<T>* new_node) {

	if(root->obj > new_node->obj) {
		if(!root->left) {
			root->left = new_node;
			new_node->parent = root;
		} else {
			recursiveInsert(root->left,new_node);
		}
	} else {
		if(!root->right) {
			root->right = new_node;
			new_node->parent = root;
		} else {
			recursiveInsert(root->right,new_node);
		}
	}

	setBalance(root);
	if(root->balance >= 2) {
		if(root->left->balance >= 0) rotateRight(root);
		else if(root->left->balance == -1) rotateLR(root);
	}

	if(root->balance <= -2) {
		if(root->right->balance <=0) rotateLeft(root);
		else if(root->right->balance == 1) rotateRL(root);
	}

}

template<class T>
bool AVLtree<T>::insert(const T& obj) {

	if(find(root,obj)) return false; //node already exits
	AVLnode<T>* new_node = NULL;
	new_node = new AVLnode<T>(obj);
	assert(new_node);

	if(!root) root = new_node;
	else recursiveInsert(root,new_node);
	num_of_nodes++;

	return true;

}

template<class T>
void AVLtree<T>::recursiveDelete(AVLnode<T>* root) {

	if(root) {
		recursiveDelete(root->left);
		recursiveDelete(root->right);
		delete root;
	}
}

template<class T>
bool AVLtree<T>::remove(const T& obj) {


	AVLnode<T>* node = find(root,obj);
	if(!node) return false;
	else {
		int num_of_sons = 0;
		if(node->right) num_of_sons++;
		if(node->left) num_of_sons++;

		switch(num_of_sons) {
		case 0:
				removeNodeWithZeroSons(node);
				break;
		case 1:
				removeNodeWithOneSon(node);
				break;
		case 2:
				removeNodeWithTwoSons(node);
				break;
		}
	}
	num_of_nodes--;
	if(num_of_nodes == 0) root = NULL;
	return true;
}

template<class T>
void AVLtree<T>::rebalanceAfterRemove(AVLnode<T>* node) {

	int privious_height = height(node);
	while(node) { // while node->parent != NULL
		setBalance(node);

		if(node->balance >= 2) {
			if(node->left->balance >= 0) rotateRight(node);
			else if(node->left->balance == -1) rotateLR(node);
		}

		if(node->balance <= -2) {
			if(node->right->balance <=0) rotateLeft(node);
			else if(node->right->balance == 1) rotateRL(node);
		}

		if(height(node) == privious_height) break;

		node = node->parent;
	}
}

template<class T>
void AVLtree<T>::removeNodeWithZeroSons(AVLnode<T>* nodeToRomove) {

	if(nodeToRomove->parent) {
		if(nodeToRomove->parent->left == nodeToRomove) {
			nodeToRomove->parent->left = NULL;
		} else if(nodeToRomove->parent->right == nodeToRomove) {
			nodeToRomove->parent->right = NULL;
		}
		rebalanceAfterRemove(nodeToRomove->parent);
	} else {
		root = NULL;
	}

	delete nodeToRomove;
}

template<class T>
void AVLtree<T>::removeNodeWithOneSon(AVLnode<T>* nodeToRomove) {

	AVLnode<T>* son = NULL;
	if(nodeToRomove->left) son = nodeToRomove->left;
	if(nodeToRomove->right) son = nodeToRomove->right;

	if(nodeToRomove->parent) {
		if(nodeToRomove->parent->left == nodeToRomove) {
			son->parent = nodeToRomove->parent;
			nodeToRomove->parent->left = son;
		} else if(nodeToRomove->parent->right == nodeToRomove) {
			son->parent = nodeToRomove->parent;
			nodeToRomove->parent->right = son;
		}

		rebalanceAfterRemove(son->parent);

	} else {
		son->parent = NULL;
		root = son;
	}
	delete nodeToRomove;
}

template<class T>
static bool NodeSwap(AVLnode<T>* root,AVLnode<T>* toRemove, AVLnode<T>* node) {

	bool flag = false;
	AVLnode<T>* save_right; // +1

	if(toRemove->right == node) {
		node->left = toRemove->left;
		if(toRemove->left) toRemove->left->parent = node;
	} else if(toRemove->left == node) {
		node->right = toRemove->right;
		if(toRemove->right) toRemove->right->parent = node;
	} else {
		node->left = toRemove->left;
		if(node->left) node->left->parent = node; // here
		save_right = node->right;//+1
		node->right = toRemove->right;
		if(node->right) node->right->parent = node; // here

		node->parent->left = save_right;//+1
		if(save_right) save_right->parent = node->parent;//+1
	}

	//-2

	node->parent = toRemove->parent;
	if(!node->parent) flag = true; //root = node;
	if(toRemove->parent) {
		if(toRemove->parent->left == toRemove) toRemove->parent->left = node;
		else if(toRemove->parent->right == toRemove) {
			toRemove->parent->right = node;
		}
	}


	toRemove->right = NULL;
	toRemove->left = NULL;
	delete toRemove;
	return flag;
}

template<class T>
void AVLtree<T>::removeNodeWithTwoSons(AVLnode<T>* nodeToRomove) {

	AVLnode<T> *parent, *inOrderNode = nodeToRomove->right;
	while(inOrderNode->left != NULL) {
		inOrderNode = inOrderNode->left;
	}

	parent = inOrderNode->parent;
	bool flag = (parent != nodeToRomove);
	if(NodeSwap(root,nodeToRomove, inOrderNode)) root = inOrderNode;
	if(flag) rebalanceAfterRemove(parent);
	rebalanceAfterRemove(inOrderNode); //+1

}

template<class T>
void AVLtree<T>::removeEmptyLeaf(AVLnode<T>* node){
	if(node->parent->left == node) {
		node->parent->left = NULL;
	} else if(node->parent->right == node) {
		node->parent->right = NULL;
	}
	delete node;
	num_of_nodes--;
}


#endif /* AVL_H_ */
