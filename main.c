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
#include "dmemory.h"
#include "bitstream.h"
#include <assert.h>
/*
 * 
 */
int main(int argc, char** argv) {

    /*DBinaryTree* huff_tree = NULL;
    HuffTreeStats* stats = NULL;
    
    char* file_path = "test.bmp";
    
    char *buffer = NULL;  
    int buffer_size = file_read_into_buffer(&buffer,file_path);
    if ( buffer_size == -1){
        printf("Couldn't open file %s : ",file_path);
        exit(EXIT_FAILURE);
    }
    
    huff_tree = huff_build_tree(buffer,512); 
    mpool_release();
    
    
    FILE* fd = fopen("graph1.txt","w");
    
    int buffer_advance = 0;
    
    for (int i = 2; i < 256; i+= 1){
        
        huff_tree = huff_build_tree_max_symbol(&buffer_advance,buffer+33000,i,buffer_size); 
        stats = huff_compute_tree_stats(huff_tree);
        //huff_print_tree_stats(stats);
        
        fprintf(fd,"%d %f\n",i,stats->total_compression_ratio_estimation);
        
        free(stats);
        mpool_release();
    }
    

    
    
    fd = fopen("graph2.txt","w");
    
    // Total file estimated compressed size
    float compression_ratio_sum = 0;
    int block_processed = 0;
    int i = 0;
    for( int j = 2; j < 256; j+=1){
        compression_ratio_sum = 0;
        block_processed = 0;
        int total_advance = 0;
        while ( total_advance < buffer_size ){
            huff_tree = huff_build_tree_max_symbol(&buffer_advance,buffer+total_advance,j,buffer_size - total_advance); 
            total_advance += buffer_advance;
            stats = huff_compute_tree_stats(huff_tree);
            compression_ratio_sum += stats->total_compression_ratio_estimation;
            block_processed++;
            //huff_print_tree_stats(stats);
            free(stats);
            mpool_release();
        }
         fprintf(fd,"%d %f\n",j,compression_ratio_sum / block_processed);
         fprintf(stdout,"%d %f\n",j,compression_ratio_sum / block_processed);
        }
        
    fclose(fd);
    mpool_free();
    free(buffer);*/
    /*
    BitStream* file = bitstream_fopen("testbit.txt",BS_WRITE);
    bitstream_write(file,0);
    bitstream_write(file,1);
    bitstream_write(file,0);
    bitstream_write(file,0);
    
    bitstream_write(file,1);
    bitstream_write(file,0);
    bitstream_write(file,0);
    bitstream_write(file,1);
    
    bitstream_write(file,0);
    bitstream_write(file,1);
    bitstream_write(file,0);
    bitstream_write(file,0);
    
    bitstream_write(file,1);
    bitstream_write(file,0);
    bitstream_write(file,0);
    bitstream_write(file,1);

    bitstream_write(file,0);
    bitstream_write(file,1);
    bitstream_write(file,0);
    bitstream_write(file,0);
    
    bitstream_write(file,1);
    
    bitstream_close(file);
    
    char c;
    file = bitstream_fopen("testbit.txt",BS_READ);
    while ( ( c = bitstream_read(file) ) != -1){
        printf("%d ",c);
    }*/
    
    BitStream* file = bitstream_fopen("test.bmp",BS_READ);
    BitStream* file_out = bitstream_fopen("test_out.bmp",BS_WRITE);
    char character;
    while ( ( character = bitstream_read(file) ) != -1){
        bitstream_write(file_out,character);
    }
    
    bitstream_close(file_out);    

    
    
    exit(EXIT_SUCCESS);
}

