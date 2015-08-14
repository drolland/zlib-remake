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
#include <math.h>
/*
 * 
 */
int main(int argc, char** argv) {

    DBinaryTree* huff_tree = NULL;
    HuffTreeStats* stats = NULL;
    
    char* file_path = "binary.o";
    
    char *buffer = NULL;  
    int buffer_size = file_read_into_buffer(&buffer,file_path);
    if ( buffer_size == -1){
        printf("Couldn't open file %s : ",file_path);
        exit(EXIT_FAILURE);
    }
    
    //huff_tree = huff_build_tree_fixed_symbol_count(buffer,512); 
    mpool_release();
    
    
    FILE* fd = fopen("graph1.txt","w");
    
    int buffer_advance = 0;
    
    for (int i = 2; i < 256; i+= 1){
        
        huff_tree = huff_build_tree_fixed_symbol_count(&buffer_advance,buffer,i,buffer_size); 
        stats = huff_compute_tree_stats(huff_tree);
        huff_print_tree_stats(stats);
        
        fprintf(fd,"%d %f\n",i,stats->total_compression_ratio_estimation);
        
        free(stats);
        mpool_release();
    }
    

    
    
    /*
    fd = fopen("graph2.txt","w");
    
    // Total file estimated compressed size
    
    float brute_force_ratio[256];
    int brute_force_advance[256];
    int brute_force_bits[256];
    int brute_force_index = 0;
    
    int total_advance = 0;
    int total_bits_size = 0;
    
    while ( total_advance < buffer_size  )
        {
        for( int j = 2; j < 256; j+=11){
    
            int total_advance_local = 0;
            
            huff_tree = huff_build_tree_fixed_symbol_count(&total_advance_local,buffer+total_advance,j,buffer_size - total_advance); 

            stats = huff_compute_tree_stats(huff_tree);
            brute_force_ratio[j] = stats->total_compression_ratio_estimation;
            brute_force_advance[j] = total_advance_local;
            brute_force_bits[j] = stats->total_bits_size;


            free(stats);
            mpool_release();
            


        }

        float brute_force_min_ratio = 100.0f;
            for (int i = 2; i < 256;i+=11)
                if ( brute_force_ratio[i] <= brute_force_min_ratio  ) {
                    brute_force_min_ratio = brute_force_ratio[i];
                    brute_force_index = i;
                }

        total_advance += brute_force_advance[brute_force_index];
        total_bits_size += brute_force_bits[brute_force_index];
        
        printf("%d-",brute_force_index);
    }
    
    printf("\nBrute force Estimation %f", (float)total_bits_size / (float)buffer_size / 8.0f);
    
    
        
    fclose(fd);
    mpool_free();
    free(buffer);*/
    
    
    char* memory = (char*)malloc(3*sizeof(char));
    BitStream* file = bitstream_mopen(memory,3,BS_WRITE);
    printf("memory[0] : 0 %d\n",(unsigned char)memory[0]);
    bitstream_write(file,1);
    bitstream_write(file,0);
    bitstream_write(file,0);
    bitstream_write(file,0);
    
    bitstream_write(file,0);
    bitstream_write(file,0);
    bitstream_write(file,0);
    bitstream_write(file,1);
    printf("memory[0] : 1 %d\n",(unsigned char)memory[0]);
    
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
    file = bitstream_mopen(memory,3,BS_READ);
    while ( ( c = bitstream_read(file) ) != -1){
        printf("%d ",c);
    }
    
    /*
    BitStream* file = bitstream_fopen("test.bmp",BS_READ);
    BitStream* file_out = bitstream_fopen("test_out.bmp",BS_WRITE);
    char character;
    while ( ( character = bitstream_read(file) ) != -1){
        bitstream_write(file_out,character);
    }
    
    bitstream_close(file_out); */   

    
    
    exit(EXIT_SUCCESS);
}

