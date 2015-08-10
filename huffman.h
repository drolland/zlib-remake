#pragma once

#include "dbinarytree.h"
#include "bitstream.h"

typedef struct huff_symbol {
    unsigned int weight;
    int value;
} HuffSymbol;

/*  Construct a Huffman tree from the given string and a given block size
 *  Returns a Huffman Tree ( A DBinaryTree with Huff_Symbol for content )
 */
DBinaryTree* huff_build_tree_fixed_size_block(char* str,size_t len);

/*  Construct a Huffman Tree from the given string.
 *  Stop when the huffman tree contain max_symbol or maxlen has been reached
 *  Return the block size in first paramter int* block_size
 */
DBinaryTree* huff_build_tree_fixed_symbol_count(int* block_size,char *str, int max_symbol,size_t maxlen);

/*  Output the tree header in the provided BitStream
 */
void huff_write_tree_header(BitStream* bs,DBinaryTree* tree);

/*  Encode and Output data in the provided 
 *
 */
void huff_encode_fixed_symbol_count(BitStream* bs,DBinaryTree* tree,char* str,size_t len);

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




