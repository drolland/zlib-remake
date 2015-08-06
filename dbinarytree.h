#pragma once

typedef struct dbinary_tree {
    void* content;
    struct dbinary_tree* right;
    struct dbinary_tree* left;
} DBinaryTree;


void dbinary_tree_free_full(DBinaryTree* tree);

