#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <algorithm>
#include <iostream> 
#include <assert.h>
#include <sstream>
#include <memory>
using namespace std;

// AvlTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x  
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// string toString(string msg )      --> Print tree in sorted order
// Comparable  removeMin  -->  You are not allowed simply to do a find Min and then a remove on that value.  
//                            That would require that you traverse the tree twice  (once to find and once to delete).  We want a true removeMin that 
//                            only traverses in order to delete.
// ******************ERRORS********************************


//HINT:   The class you use as Comparable must have overloaded operators for > == and <.
//        nullptr is another way of saying NULL.  It has some interesting properties for implicit conversions.
//        && is an "rvalue reference".  They are beyond the scope of this class.  A good explanation is at http://thbecker.net/articles/rvalue_references/section_01.html

template <typename Comparable>
class AvlTree
{
public:
	AvlTree() : root{ nullptr }
	{
		size = 0;
	}

	int getSize(){ return size; }
	AvlTree(const AvlTree & rhs) : root{ nullptr }
	{
		size = rhs.size;
		root = clone(rhs.root);
	}

	AvlTree(AvlTree && rhs) : root{ rhs.root }
	{
		rhs.root = nullptr;
	}

	~AvlTree()
	{
		makeEmpty();
	}

	/* Deep copy. */
	AvlTree & operator=(const AvlTree & rhs)
	{
		AvlTree copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	/* Move. */
	AvlTree & operator=(AvlTree && rhs)
	{
		std::swap(root, rhs.root);

		return *this;
	}

	/* Find the smallest item in the tree. Throw UnderflowException if empty. */
	const Comparable & findMin() const
	{
		assert(!isEmpty());
			return findMin(root)->element;
	}

	/* Find the largest item in the tree.	 */
	const Comparable & findMax() const
	{
		assert(!isEmpty());
			return findMax(root)->element;
	}

	/* Returns true if x is found in the tree. */
	bool contains(const Comparable & toFind) const
	{
		return contains(toFind, root);
	}

	/* Test if the tree is logically empty.	 */
	bool isEmpty() const
	{
		return root == nullptr;
	}

	/* String contaings the tree contents in sorted order. */
	string toString(string treeString)
	{
		ostringstream outStream;
		outStream << treeString << endl;
		if (isEmpty())
			outStream << "Empty tree" << endl;
		else
			toString(root,0,outStream);
		outStream << "END " << treeString << endl;
		return outStream.str();
	}

	/* String contains sorted list of tree contens. */
	string toString2()
	{
		ostringstream outStream;
		toString2(root,outStream);
		return outStream.str();
	}

	/* Make the tree logically empty. */
	void makeEmpty()
	{
		makeEmpty(root);
	}

	/*	Insert x into the tree; 	 */
	void insert(const Comparable & toInsert)
	{
		size++;
		insert(toInsert, root);
	}

	/**
	 * Insert toInsert into the tree; 
	 * && is termed an rvalue reference.
	 * a good explanation is at http://thbecker.net/articles/rvalue_references/section_01.html
	 */
	void insert(Comparable && toInsert)
	{
		size++;
		insert(std::move(toInsert), root);
	}

	/* Remove x from the tree. Nothing is done if x is not found.	 */
	void remove(const Comparable & toDelete)
	{
		size--;
		remove(toDelete, root);
	}
	/* remove smallest element from the tree.  Return the value foudn there*/
	Comparable removeMin(){
		size--;
		AvlNode * min = removeMin(root);
		assert(min != NULL);
		return min->element;
	}

private:
	struct AvlNode
	{
		Comparable element;
		AvlNode   *left;
		AvlNode   *right;
		int       height;

		AvlNode(const Comparable & ele, AvlNode *lt, AvlNode *rt, int h = 0)
			: element{ ele }, left{ lt }, right{ rt }, height{ h } { }

		AvlNode(Comparable && ele, AvlNode *lt, AvlNode *rt, int h = 0)
			: element{ std::move(ele) }, left{ lt }, right{ rt }, height{ h } { }
	};

	AvlNode *root;
	int size;


	/**
	 * Internal method to insert into a subtree.
	 * toInsert is the item to insert.
	 * "node" is the node that roots the subtree.
	 * Set the new root of the subtree.
	 */
	void insert(const Comparable & toInsert, AvlNode * & node)
	{
		if (node == nullptr)
			node = new AvlNode{ toInsert, nullptr, nullptr };
		else if (toInsert < node->element)
			insert(toInsert, node->left);
		else 
			insert(toInsert, node->right);

		balance(node);
	}

	/**
	 * Internal method to insert into a subtree.
	 * toInsert is the item to insert.
	 * "node" is the node that roots the subtree.
	 * Set the new root of the subtree.
	 */
	void insert(Comparable && toInsert, AvlNode * & node)
	{
		if (node == nullptr)
			node = new AvlNode{ std::move(toInsert), nullptr, nullptr };
		else if (toInsert < node->element)
			insert(std::move(toInsert), node->left);
		else 
			insert(std::move(toInsert), node->right);

		balance(node);
	}

	/**
	 * Internal method to remove from a subtree.
	 * toRemove is the item to remove.
	 * "node" is the node that roots the subtree.
	 * Set the new root of the subtree.
	 */
	void remove(const Comparable & toRemove, AvlNode * & node)
	{
		if (node == nullptr)
			return;   // Item not found; do nothing

		if (toRemove < node->element)
			remove(toRemove, node->left);
		else if (node->element < toRemove)
			remove(toRemove, node->right);
		else if (node->left != nullptr && node->right != nullptr) // Two children
		{
			node->element = findMin(node->right)->element;
			remove(node->element, node->right);
		}
		else
		{
			AvlNode *oldNode = node;
			node = (node->left != nullptr) ? node->left : node->right;
			delete oldNode;
		}

		balance(node);
	}

	/*
	 * removes the smallest node
	 */

	AvlNode*  removeMin(AvlNode * & node)
	{
		AvlNode *oldNode;
		if(!node) return nullptr;
		if(!node->left)
		{
			oldNode = node;
			node = node->right;
			return oldNode;
		}
		AvlNode *returnNode = removeMin(node->left);
		balance(node);
		return returnNode;
	}


	static const int ALLOWED_IMBALANCE = 1;

	// Assume node is balanced or within one of being balanced
	void balance(AvlNode * & node)
	{
		if (node == nullptr)
			return;

		if (height(node->left) - height(node->right) > ALLOWED_IMBALANCE)
			if (height(node->left->left) >= height(node->left->right))
				rotateWithLeftChild(node);
			else
				doubleWithLeftChild(node);
		else
			if (height(node->right) - height(node->left) > ALLOWED_IMBALANCE)
				if (height(node->right->right) >= height(node->right->left))
					rotateWithRightChild(node);
				else
					doubleWithRightChild(node);

		node->height = max(height(node->left), height(node->right)) + 1;
	}

	/**
	 * Internal method to find the smallest item in a subtree node.
	 * Return node containing the smallest item.
	 */
	AvlNode * findMin(AvlNode *node) const
	{
		if (node == nullptr)
			return nullptr;
		if (node->left == nullptr)
			return node;
		return findMin(node->left);
	}

	/**
	 * Internal method to find the largest item in a subtree node.
	 * Return node containing the largest item.
	 */
	AvlNode * findMax(AvlNode *node) const
	{
		if (node != nullptr)
			while (node->right != nullptr)
				node = node->right;
		return node;
	}


	/**
	 * Internal method to test if an item is in a subtree.
	 * toFInd is item to search for.
	 * node is the node that roots the tree.
	 */
	bool contains(const Comparable & toFind, AvlNode *node) const
	{
		if (node == nullptr)
			return false;
		else if (toFind < node->element)
			return contains(toFind, node->left);
		else if (node->element < toFind)
			return contains(toFind, node->right);
		else
			return true;    // Match
	}

	/**
	 * Internal method to make subtree empty.
	 */
	void makeEmpty(AvlNode * & node)
	{
		if (node != nullptr)
		{
			makeEmpty(node->left);
			makeEmpty(node->right);
			delete node;
		}
		node = nullptr;
	}

	/**
	 * Internal method to create a string for a subtree rooted at "node" in sorted order.
	 */
	void toString(AvlNode *node, int indent, ostringstream &outStream)
	{   
		if(!node) return;
		
		toString(node->right, indent+4,outStream);

		for(int i=0; i<indent;++i)
		{
			outStream << " ";
		}
		outStream << node->element;
		outStream << '\n';
		toString(node->left, indent+4,outStream);
	}

	void toString2(AvlNode *node, ostringstream &outStream)
	{
		if (!node) return;
		toString2(node->right, outStream);
		outStream << node->element << " ";
		toString2(node->left, outStream);
	}

	/**
	 * Internal method to clone subtree.
	 */
	AvlNode * clone(AvlNode *node) const
	{
		if (node == nullptr)
			return nullptr;
		else
			return new AvlNode{ node->element, clone(node->left), clone(node->right), node->height };
	}
	// Avl manipulations
	/**
	 * Return the height of node "node" or -1 if nullptr.
	 */
	int height(AvlNode *node) const
	{
		return node == nullptr ? -1 : node->height;
	}

	int max(int lhs, int rhs) const
	{
		return lhs > rhs ? lhs : rhs;
	}

	/**
	 * Rotate binary tree node with left child.
	 * For AVL trees, this is a single rotation for case 1.
	 * Update heights, then set new root.
	 */
	void rotateWithLeftChild(AvlNode * & k2)
	{
		AvlNode *k1 = k2->left;
		k2->left = k1->right;
		k1->right = k2;
		k2->height = max(height(k2->left), height(k2->right)) + 1;
		k1->height = max(height(k1->left), k2->height) + 1;
		k2 = k1;
	}

	/**
	 * Rotate binary tree node with right child.
	 * For AVL trees, this is a single rotation for case 4.
	 * Update heights, then set new root.
	 */
	void rotateWithRightChild(AvlNode * & k1)
	{
		AvlNode *k2 = k1->right;
		k1->right = k2->left;
		k2->left = k1;
		k1->height = max(height(k1->left), height(k1->right)) + 1;
		k2->height = max(height(k2->right), k1->height) + 1;
		k1 = k2;
	}

	/**
	 * Double rotate binary tree node: first left child.
	 * with its right child; then node k3 with new left child.
	 * For AVL trees, this is a double rotation for case 2.
	 * Update heights, then set new root.
	 */
	void doubleWithLeftChild(AvlNode * & k3)
	{
		rotateWithRightChild(k3->left);
		rotateWithLeftChild(k3);
	}

	/**
	 * Double rotate binary tree node: first right child.
	 * with its left child; then node parent with new right child.
	 * For AVL trees, this is a double rotation for case 3.
	 * Update heights, then set new root.
	 */
	void doubleWithRightChild(AvlNode * & k1)
	{
		rotateWithLeftChild(k1->right);
		rotateWithRightChild(k1);
	}
};

#endif
