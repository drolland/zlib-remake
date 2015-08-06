#pragma once

typedef struct dbinary_tree {
    void* content;
    struct dbinary_tree* right;
    struct dbinary_tree* left;
} DBinaryTree;


DBinaryTree* dbinary_tree_append_right(DBinaryTree** tree,void* to_insert);
DBinaryTree* dbinary_tree_append_left(DBinaryTree** tree,void* to_insert);

