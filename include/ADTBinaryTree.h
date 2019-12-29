///////////////////////////////////////////////////////////////////
//
// ADT Binary Tree
//
// Abstract δυαδικό δέντρο.
//
///////////////////////////////////////////////////////////////////

#pragma once // #include το πολύ μία φορά

#include "common_types.h"


// Δέντρα και κόμβοι αναπαριστώνται από τους τύπους BinaryTree και BinaryTreeNode.

typedef struct binary_tree* BinaryTree;
typedef struct binary_tree_node* BinaryTreeNode;

typedef void (*BinaryTreeVisitFunc)(BinaryTree, BinaryTreeNode);


// TODO: documentation

BinaryTree binary_tree_create();

int binary_tree_height(BinaryTree tree);
int binary_tree_size(BinaryTree tree);

BinaryTreeNode binary_tree_root(BinaryTree tree);
BinaryTreeNode binary_tree_parent(BinaryTree tree, BinaryTreeNode node);
BinaryTreeNode binary_tree_child_left(BinaryTree tree, BinaryTreeNode node);
BinaryTreeNode binary_tree_child_right(BinaryTree tree, BinaryTreeNode node);

Pointer binary_tree_get(BinaryTree tree, BinaryTreeNode node);
void binary_tree_set(BinaryTree tree, BinaryTreeNode node, Pointer value);

void binary_tree_insert(BinaryTree tree, BinaryTreeNode node, int position, Pointer item);
void binary_tree_remove(BinaryTree tree, BinaryTreeNode node);

void binary_tree_pre_order(BinaryTree tree, BinaryTreeVisitFunc visit);
void binary_tree_in_order(BinaryTree tree, BinaryTreeVisitFunc visit);
void binary_tree_post_order(BinaryTree tree, BinaryTreeVisitFunc visit);
void binary_tree_level_order(BinaryTree tree, BinaryTreeVisitFunc visit);

void binary_tree_Destroy(BinaryTree tree);

