#include "RangeTree.h"


// Smaller elements go left
// larger elements go right
RangeTree::color *RangeTree::insert(unsigned char d)
{
	tree_node *t = new tree_node;
	RangeTree::color *currentColor = new RangeTree::color;
	srand (time(NULL));
	currentColor->r = rand() % 256;
	currentColor->g = rand() % 256;
	currentColor->b = rand() % 256;
	tree_node* parent;
	t->data = d;
	t->currentColor = currentColor;
	t->left = NULL;
	t->right = NULL;
	parent = NULL;
	// is this a new tree?
	if(isEmpty())
		root = t;
	else{
		tree_node* curr;
		curr = root;
		while(curr){
			parent = curr;
			if((t->data + _range) > (curr->data + _range))
				curr = curr->right;
			else if ((t->data - _range) < (curr->data - _range))
				curr = curr->left;
			else
				return curr->currentColor;
		}
		//If a color is not found
		if((t->data - _range) < (parent->data - _range)){
			//Smaller than range
			/*
				allign the data then add smaller call allign left
			*/
			t->data = allignLeft(t->data, parent->data);
			parent->left = t;
		}else{
			t->data = allignRight(t->data, parent->data);
			parent->right = t;
		}
	}
}

unsigned char RangeTree::allignLeft(unsigned char data, unsigned char parent){
	return (data - _range) + (parent - _range - data - 1);
}

unsigned char RangeTree::allignRight(unsigned char data, unsigned char parent){
}



/*void BinarySearchTree::remove(int d)
{
    //Locate the element
    bool found = false;
    if(isEmpty())
    {
        cout<<" This Tree is empty! "<<endl;
        return;
    }
    tree_node* curr;
    tree_node* parent;
    curr = root;
    while(curr != NULL)
    {
         if(curr->data == d)
         {
            found = true;
            break;
         }
         else
         {
             parent = curr;
             if(d>curr->data) curr = curr->right;
             else curr = curr->left;
         }
    }
    if(!found)
		 {
        cout<<" Data not found! "<<endl;
        return;
    }


		 // 3 cases :
    // 1. We're removing a leaf node
    // 2. We're removing a node with a single child
    // 3. we're removing a node with 2 children

    // Node with single child
    if((curr->left == NULL && curr->right != NULL)|| (curr->left != NULL
&& curr->right == NULL))
    {
       if(curr->left == NULL && curr->right != NULL)
       {
           if(parent->left == curr)
           {
             parent->left = curr->right;
             delete curr;
           }
           else
           {
             parent->right = curr->right;
             delete curr;
           }
       }
       else // left child present, no right child
       {
          if(parent->left == curr)
           {
             parent->left = curr->left;
             delete curr;
           }
           else
           {
             parent->right = curr->left;
             delete curr;
           }
       }
     return;
    }

		 //We're looking at a leaf node
		 if( curr->left == NULL && curr->right == NULL)
    {
        if(parent->left == curr) parent->left = NULL;
        else parent->right = NULL;
		 		 delete curr;
		 		 return;
    }


    //Node with 2 children
    // replace node with smallest value in right subtree
    if (curr->left != NULL && curr->right != NULL)
    {
        tree_node* chkr;
        chkr = curr->right;
        if((chkr->left == NULL) && (chkr->right == NULL))
        {
            curr = chkr;
            delete chkr;
            curr->right = NULL;
        }
        else // right child has children
        {
            //if the node's right child has a left child
            // Move all the way down left to locate smallest element

            if((curr->right)->left != NULL)
            {
                tree_node* lcurr;
                tree_node* lcurrp;
                lcurrp = curr->right;
                lcurr = (curr->right)->left;
                while(lcurr->left != NULL)
                {
                   lcurrp = lcurr;
                   lcurr = lcurr->left;
                }
		 		 		 		 curr->data = lcurr->data;
                delete lcurr;
                lcurrp->left = NULL;
           }
           else
           {
               tree_node* tmp;
               tmp = curr->right;
               curr->data = tmp->data;
		 		 		    curr->right = tmp->right;
               delete tmp;
           }

        }
		 return;
    }

}

void RangeTree::print_inorder()
{
  inorder(root);
}

void RangeTree::inorder(tree_node* p)
{
    if(p != NULL)
    {
        if(p->left) inorder(p->left);
        cout<<" "<<p->data<<" ";
        if(p->right) inorder(p->right);
    }
    else return;
}

void RangeTree::print_preorder()
{
  preorder(root);
}

void RangeTree::preorder(tree_node* p)
{
    if(p != NULL)
    {
        cout<<" "<<p->data<<" ";
        if(p->left) preorder(p->left);
        if(p->right) preorder(p->right);
    }
    else return;
}

void RangeTree::print_postorder()
{
  postorder(root);
}

void RangeTree::postorder(tree_node* p)
{
    if(p != NULL)
    {
        if(p->left) postorder(p->left);
        if(p->right) postorder(p->right);
        cout<<" "<<p->data<<" ";
    }
    else return;
}*/
