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

    char *buffer = NULL;  
    int buffer_size = file_read_into_buffer(&buffer,"test2.txt");
    
    //DBinaryTree* huff_tree = huff_build_tree("this is an example of a huffman tree",sizeof("this is an example of a huffman tree")-1);
    DBinaryTree* huff_tree = huff_build_tree("aabcdefffgggggggggxyzkmnppppppppppppppppp",sizeof("aabcdefffgggggggggxyzkmnppppppppppppppppp")-1);
    huff_dbg_print_tree(huff_tree);
    
    
    return (EXIT_SUCCESS);
}

