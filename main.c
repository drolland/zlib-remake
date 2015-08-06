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

    DBinaryTree* huff_tree = NULL;
    HuffTreeStats* stats = NULL;
    
    char *buffer = NULL;  
    int buffer_size = file_read_into_buffer(&buffer,"test.bmp");
    if ( buffer == NULL){
        printf("Couldn't open file");
        exit(EXIT_FAILURE);
    }
    
    
            
    FILE* fd = fopen("graph1.txt","w");
    
    for (int i = 2; i < 4096; i+= 2){
        
        huff_tree = huff_build_tree(buffer+50000,i); 
        stats = huff_compute_tree_stats(huff_tree);
        //huff_print_tree_stats(stats);
        
        fprintf(fd,"%d %f\n",i,stats->total_compression_ratio_estimation);
        
        free(stats);
        dbinary_tree_free_full(huff_tree);
    }
    
     //Total file estimated compressed size
    float compression_ratio_sum = 0;
    int block_processed = 0;
    int i = 0;
    for ( i = 0; i < buffer_size-512; i+= 512){
        huff_tree = huff_build_tree(buffer+i,512); 
        stats = huff_compute_tree_stats(huff_tree);
        compression_ratio_sum += stats->total_compression_ratio_estimation;
        block_processed++;
        free(stats);
        dbinary_tree_free_full(huff_tree);
    }
    printf("Total file compression ratio estimation %f", compression_ratio_sum / block_processed);
    
    
    close(fd);
    
    return (EXIT_SUCCESS);
}

