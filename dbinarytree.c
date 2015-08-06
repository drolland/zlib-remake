#include "dbinarytree.h"
#include <stdlib.h>
#include <assert.h>




void dbinary_tree_free_full(DBinaryTree* tree){
    
    assert(tree != NULL);
    free(tree->content);
    if ( tree->right != NULL)
        dbinary_tree_free_full(tree->right);
    if ( tree->left != NULL)
        dbinary_tree_free_full(tree->left);
    free(tree);
    
}
