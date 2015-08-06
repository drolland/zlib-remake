/* 
 * File:   main.c
 * Author: denro
 *
 * Created on 31 juillet 2015, 13:23
 */

#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include "crc.h"
#include "dfile.h"
#include <assert.h>
/*
 * 
 */
int main(int argc, char** argv) {

    DBinaryTree* huff_tree;
    
    char *buffer = NULL;  
    int buffer_size = file_read_into_buffer(&buffer,"test.bmp");
    huff_tree = huff_build_tree(buffer,512); 
       
    HuffTreeStats* stats = huff_compute_tree_stats(huff_tree);
    huff_print_tree_stats(stats);
    
    return (EXIT_SUCCESS);
}

