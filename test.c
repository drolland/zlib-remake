#include "test.h"
#include "bitstream.h"
#include "dfile.h"
#include "lz77.h"



void test_bitstream_memory(){
    
    char* memory = (char*)malloc(3*sizeof(char));
    BitStream* file = bitstream_mopen(memory,3,BS_WRITE);
    
    bitstream_write(file,1);
    bitstream_write(file,0);
    bitstream_write(file,0);
    bitstream_write(file,0);
    
    bitstream_write(file,0);
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
    file = bitstream_mopen(memory,3,BS_READ);
    while ( ( c = bitstream_read(file) ) != -1){
        printf("%d ",c);
    }
    
    free(memory);
    bitstream_close(file);
    
}

void test_lz77_encode(char* filepath){
    
    char* buffer = NULL;
    
    int buffer_size = dfile_read_into_buffer(&buffer,filepath);
    
    if ( buffer_size == -1){
        printf("test_lz77_encode : Couldn't open file %s : ",filepath);
        exit(EXIT_FAILURE);
    }
    
    unsigned short int* out_buffer = checked_malloc(sizeof(unsigned short int) * buffer_size);
    
    lz77_encode(out_buffer,buffer,buffer_size);
    lz77_dbg_print_buffer(out_buffer,buffer_size);
    
    free(buffer);
    free(out_buffer);
}
