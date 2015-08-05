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
    
    char string[5] = "abbc";
    huffman_encode("this is an example of a huffman tree",sizeof("this is an example of a huffman tree")-1);
    huffman_encode("abcdef",sizeof("abcdef")-1);
    printf("my_hash %x\n",my_hash(buffer,buffer_size));
    
    printf("a     %x\n",my_hash("a",1));
    printf("ab    %x\n",my_hash("ab",2));
    printf("arbre %x\n",my_hash("arbre",5) );
    
    return (EXIT_SUCCESS);
}

