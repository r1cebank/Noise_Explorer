#pragma once
#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;

class RangeTree {
	public:
		struct color{
			int r,b,g;
		};
		struct tree_node{
			tree_node* left;
			tree_node* right;
			unsigned char data;
			color *currentColor;
		};
		tree_node* root;
		int _range;
		RangeTree(int);
		RangeTree();
		bool isEmpty() const { return root==NULL; }
		/*
		void print_inorder();
		void inorder(tree_node*);
		void print_preorder();
		void preorder(tree_node*);
		void print_postorder();
		void postorder(tree_node*);
		*/
		color *insert(unsigned char);
		//void remove(long);
		unsigned char allignLeft(unsigned char, unsigned char);
		unsigned char allignRight(unsigned char, unsigned char);
		void setRange(int range) { _range = range; }
};

