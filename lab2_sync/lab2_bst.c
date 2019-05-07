/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 
*	    Student name : 
*
*   lab2_bst.c :
*       - thread-safe bst code.
*       - coarse-grained, fine-grained lock code
*
*   Implement thread-safe bst for coarse-grained version and fine-grained version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

#include "lab2_sync_types.h"
/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order. 
 *  @return                 : status (success or fail)
 */
void inorder(struct lab2_node *node)
{
	if(node != NULL)
	{
		inorder(node->left);
		printf("%d ", node->key);
		inorder(node->right);
	}
}
int lab2_node_print_inorder(lab2_tree *tree) {
    // You need to implement lab2_node_print_inorder function.
	inorder(tree->root);
}

/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree *lab2_tree_create() {
    // You need to implement lab2_tree_create function.
	struct lab2_tree *temp = (struct lab2_tree *)malloc(sizeof(struct lab2_tree));
	return temp;
}

/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node * lab2_node_create(int key) {
    // You need to implement lab2_node_create function.
	struct lab2_node *temp = (struct lab2_node *)malloc(sizeof(struct lab2_node));
	temp->key = key;
	temp->left = NULL; temp->right = NULL;
	temp->mutex =(pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	return temp;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                 : satus (success or fail)
 */
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert function.
	if(tree->root == NULL)
	{
		tree->root = new_node;
		return 0;
	}
	lab2_node *cur = tree->root;
	while(1)
	{
		if(cur->key < new_node->key)
		{
			if(cur->right == NULL)
			{
				cur->right = new_node;
				return 0;
			}
			cur = cur->right;
		}
		else
		{
			if(cur->left == NULL)
			{
				cur->left = new_node;
				return 0;
			}
			cur = cur->left;
		}
	}
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node){
      // You need to implement lab2_node_insert_fg function.
	if(tree->root == NULL)
	{
		tree->root = new_node;
		return 0;
	}
	lab2_node *cur = tree->root;
	while(1)
	{
		if(cur->key < new_node->key)
		{
			if(cur->right == NULL)
			{
				cur->right = new_node;
			pthread_mutex_unlock(&(cur->mutex));
				return 0;
			}
			pthread_mutex_lock(&(cur->right->mutex));
			pthread_mutex_unlock(&(cur->mutex));
			cur = cur->right;
		}
		else
		{
			if(cur->left == NULL)
			{
				cur->left = new_node;
				pthread_mutex_unlock(&(cur->mutex));
				return 0;
			}
			pthread_mutex_lock(&(cur->left->mutex));
			pthread_mutex_unlock(&(cur->mutex));
			cur = cur->left;
		}
	}
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert_cg function.
	pthread_mutex_lock(&(tree->root->mutex));
	if(tree->root == NULL)
	{
		tree->root = new_node;
		pthread_mutex_unlock(&(tree->root->mutex));
		return 0;
	}
	lab2_node *cur = tree->root;
	while(1)
	{
		if(cur->key < new_node->key)
		{
			if(cur->right == NULL)
			{
				cur->right = new_node;
				pthread_mutex_unlock(&(tree->root->mutex));
				return 0;
			}
			cur = cur->right;
		}
		else
		{
			if(cur->left == NULL)
			{
				cur->left = new_node;
				pthread_mutex_unlock(&(tree->root->mutex));
				return 0;
			}
			cur = cur->left;
		}
	}
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove function.
	lab2_node *cur = tree->root;
	lab2_node *cur2 = NULL;
	lab2_node *Parent = NULL;
	lab2_node *Child = NULL;
	lab2_node *Left_Temp = NULL;
	if(cur->key == key)
	{
		if(cur->left == NULL && cur->right == NULL)
		{
			tree->root = NULL;
			return 0;
		}
		if(cur->left == NULL || cur->right == NULL)
		{
			tree->root = (tree->root->left != NULL) ? tree->root->left : tree->root->right;
			free(cur);
			return 0;
		}
		if(cur->left != NULL && cur->right != NULL)
		{
			cur2 = cur->right;
			if(cur2->left == NULL)
			{
				Left_Temp = cur->left;
				tree->root = cur2;
				cur2->left = Left_Temp;
				free(cur);
				return 0;
			}
			while(cur2->left != NULL)
			{
				Parent = cur2;
				cur2 = cur2->left;
			}
			Parent->left = NULL;
			cur->key = cur2->key;
			free(cur2);
		}
		return 0;
	}
	while(cur->key != key)
	{
		if(key > cur->key)
		{
			Parent = cur;
			cur = cur->right;
		}
		else
		{
			Parent = cur;
			cur = cur->left;
		}
	}
	if(cur->left == NULL && cur->right == NULL)
	{
		if(Parent->left == cur) Parent->left = NULL;
		if(Parent->right == cur) Parent->right = NULL;
		free(cur);
		return 0;
	}
	if(cur->left == NULL || cur->right == NULL)
	{
		Child = (cur->left != NULL) ? cur->left : cur->right;
		if(Parent->left == cur) Parent->left = Child;
		else Parent->right = Child;
		free(cur);
		return 0;
	}
	if(cur->left != NULL && cur->right != NULL)
	{
		cur2 = cur->right;
		if(cur2->left == NULL)
		{
			Left_Temp = cur->left;
			Child = cur2;
			if(Parent->right == cur)
			{
				Parent->right = Child;
				Child->left = Left_Temp;
			}
			else if(Parent->left == cur)
			{
				Parent->left = Child;
				Child->left = Left_Temp;
			}
			free(cur);
			return 0;
		}
		while(cur2->left != NULL)
		{
			Parent = cur2;
			cur2 = cur2->left;
		}
		Parent->left = NULL;
		cur->key = cur2->key;
		free(cur2);
	}
	return 0;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove_fg function.
	lab2_node *cur2 = NULL;
	lab2_node *cur = tree->root;
	lab2_node *Parent = NULL;
	lab2_node *Child = NULL;
	lab2_node *Left_Temp = NULL;
	if(cur->key == key)
	{
		pthread_mutex_lock(&(tree->root->mutex));
		if(cur->left == NULL && cur->right == NULL)
		{
			tree->root = NULL;
			pthread_mutex_unlock(&(tree->root->mutex));
			return 0;
		}
		if(cur->left == NULL || cur->right == NULL)
		{
			tree->root = (tree->root->left != NULL) ? tree->root->left : tree->root->right;
			free(cur);
			pthread_mutex_unlock(&(tree->root->mutex));
			return 0;
		}
		if(cur->left != NULL && cur->right != NULL)
		{
			pthread_mutex_lock(&(cur->mutex));
			cur2 = cur->right;
			pthread_mutex_lock(&(cur2->mutex));
			if(cur2->left == NULL)
			{
				Left_Temp = cur->left;
				tree->root = cur2;
				cur2->left = Left_Temp;
				free(cur);
				pthread_mutex_unlock(&(tree->root->mutex));
				pthread_mutex_unlock(&(cur->mutex));
				pthread_mutex_unlock(&(cur2->mutex));
				return 0;
			}
			pthread_mutex_unlock(&(cur->mutex));
			while(cur2->left != NULL)
			{
				pthread_mutex_lock(&(cur2->left->mutex));
				pthread_mutex_unlock(&(Parent->mutex));
				Parent = cur2;
				cur2 = cur2->left;
			}
			Parent->left = NULL;
			cur->key = cur2->key;
			pthread_mutex_unlock(&(cur2->mutex));
			pthread_mutex_unlock(&(Parent->mutex));
			pthread_mutex_unlock(&(tree->root->mutex));
			free(cur2);
		}
		return 0;
	}
	while(cur->key != key)
	{
		if(key > cur->key)
		{
			pthread_mutex_lock(&(cur->right->mutex));
			pthread_mutex_unlock(&(Parent->mutex));
			Parent = cur;
			cur = cur->right;
		}
		else
		{
			pthread_mutex_lock(&(cur->left->mutex));
			pthread_mutex_unlock(&(Parent->mutex));
			Parent = cur;
			cur = cur->left;
		}
	}
	if(cur->left == NULL && cur->right == NULL)
	{
		if(Parent->left == cur) Parent->left = NULL;
		if(Parent->right == cur) Parent->right = NULL;
		pthread_mutex_unlock(&(Parent->mutex));
		pthread_mutex_unlock(&(cur->mutex));
		free(cur);
		return 0;
	}
	if(cur->left == NULL || cur->right == NULL)
	{
		Child = (cur->left != NULL) ? cur->left : cur->right;
		pthread_mutex_lock(&(Child->mutex));
		if(Parent->left == cur) Parent->left = Child;
		else Parent->right = Child;
		pthread_mutex_unlock(&(Child->mutex));
		pthread_mutex_unlock(&(Parent->mutex));
		pthread_mutex_unlock(&(cur->mutex));
		free(cur);
		return 0;
	}
	if(cur->left != NULL && cur->right != NULL)
	{
		cur2 = cur->right;
		pthread_mutex_lock(&(cur2->mutex));
		if(cur2->left == NULL)
		{
			Left_Temp = cur->left;
			Child = cur2;
			if(Parent->right == cur)
			{
				Parent->right = Child;
				Child->left = Left_Temp;
			}
			else if(Parent->left == cur)
			{
				Parent->left = Child;
				Child->left = Left_Temp;
			}
			pthread_mutex_unlock(&(Child->mutex));	
			pthread_mutex_unlock(&(Parent->mutex));
			pthread_mutex_unlock(&(cur->mutex));
			free(cur);
			return 0;
		}
		while(cur2->left != NULL)
		{
			pthread_mutex_lock(&(cur2->left->mutex));
			pthread_mutex_unlock(&(Parent->mutex));
			Parent = cur2;
			cur2 = cur2->left;
		}
		Parent->left = NULL;
		cur->key = cur2->key;
		pthread_mutex_unlock(&(Parent->mutex));
		pthread_mutex_unlock(&(cur2->mutex));
		free(cur2);
	}
	return 0;
}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove_cg function.
	pthread_mutex_lock(&(tree->root->mutex));
	lab2_node *cur2 = NULL;
	lab2_node *cur = tree->root;
	lab2_node *Parent = NULL;
	lab2_node *Child = NULL;
	lab2_node *Left_Temp = NULL;
	if(cur->key == key)
	{
		if(cur->left == NULL && cur->right == NULL)
		{
			tree->root = NULL;
			pthread_mutex_lock(&(tree->root->mutex));
			return 0;
		}
		if(cur->left == NULL || cur->right == NULL)
		{
			tree->root = (tree->root->left != NULL) ? tree->root->left : tree->root->right;
			free(cur);
			pthread_mutex_lock(&(tree->root->mutex));
			return 0;
		}
		if(cur->left != NULL && cur->right != NULL)
		{
			cur2 = cur->right;
			if(cur2->left == NULL)
			{
				Left_Temp = cur->left;
				tree->root = cur2;
				cur2->left = Left_Temp;
				free(cur);
				pthread_mutex_lock(&(tree->root->mutex));
				return 0;
			}
			while(cur2->left != NULL)
			{
				Parent = cur2;
				cur2 = cur2->left;
			}
			Parent->left = NULL;
			cur->key = cur2->key;
			free(cur2);
		}
		pthread_mutex_lock(&(tree->root->mutex));
		return 0;

	}
	while(cur->key != key)
	{
		if(key > cur->key)
		{
			Parent = cur;
			cur = cur->right;
		}
		else
		{
			Parent = cur;
			cur = cur->left;
		}
	}
	if(cur->left == NULL && cur->right == NULL)
	{
		if(Parent->left == cur) Parent->left = NULL;
		if(Parent->right == cur) Parent->right = NULL;
		free(cur);
		pthread_mutex_unlock(&(tree->root->mutex));
		return 0;
	}
	if(cur->left == NULL || cur->right == NULL)
	{
		Child = (cur->left != NULL) ? cur->left : cur->right;
		if(Parent->left == cur) Parent->left = Child;
		else Parent->right = Child;
		free(cur);
		pthread_mutex_unlock(&(tree->root->mutex));
		return 0;
	}
	if(cur->left != NULL && cur->right != NULL)
	{
		cur2 = cur->right;
		if(cur2->left == NULL)
		{
			Left_Temp = cur->left;
			Child = cur2;
			if(Parent->right == cur)
			{
				Parent->right = Child;
				Child->left = Left_Temp;
			}
			else if(Parent->left == cur)
			{
				Parent->left = Child;
				Child->left = Left_Temp;
			}
			free(cur);
			pthread_mutex_unlock(&(tree->root->mutex));
			return 0;
		}
		while(cur2->left != NULL)
		{
			Parent = cur2;
			cur2 = cur2->left;
		}
		Parent->left = NULL;
		cur->key = cur2->key;
		free(cur2);
	}
	pthread_mutex_unlock(&(tree->root->mutex));
	return 0;
}


/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree *tree) {
    // You need to implement lab2_tree_delete function.
	free(tree);
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node *node) {
    // You need to implement lab2_node_delete function.
	free(node);
}

