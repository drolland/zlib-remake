#pragma once

#include "dbinarytree.h"

typedef struct huff_symbol {
    unsigned int weight;
    int value;
} HuffSymbol;

/*  Construct a Huffman tree from the given string
 *  Returns a Huffman Tree ( A DBinaryTree with Huff_Symbol for content )
 */
DBinaryTree* huff_build_tree(char* str,size_t len);
DBinaryTree* huff_build_tree_max_symbol(int* buffer_advance,char *str, int max_symbol,size_t maxlen);

void huff_dbg_print_tree(DBinaryTree* tree);

typedef struct huff_tree_stats{
    int nb_symbols;
    int weight[256];
    int total_weight;
    int code_length[256];
    int max_code_length;
    int total_bit_size;
    
    /* Compression estimation computed from above*/
    
    int block_uncompressed_size;
    int block_compressed_size; 
    float block_compression_ratio;
    int tree_size_estimation;
    float total_compression_ratio_estimation;
 
} HuffTreeStats;

HuffTreeStats* huff_compute_tree_stats(DBinaryTree* tree);
void huff_print_tree_stats(HuffTreeStats* stats);




