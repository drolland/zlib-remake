#include "test.h"
#include "bitstream.h"
#include "dfile.h"
#include "lz77.h"
#include "zlib.h"


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

void test_lz77_encode_file(char* filepath){
    
    
    
    FILE* fd = fopen(filepath,"rb");
    
    if ( fd == NULL ){
        printf("test_lz77_encode_file : Couldn't open file %s : ",filepath);
        exit(EXIT_FAILURE);
    }
    
    fseek(fd,0,SEEK_END);
    int file_size = ftell(fd);
    rewind(fd);

    
    unsigned short int* out_buffer = checked_malloc(sizeof(unsigned short int) * file_size);
    
    lz77_encode_file(out_buffer,fd,file_size);
    lz77_dbg_print_buffer(out_buffer,file_size);
    

    free(out_buffer);
}

void test_decompress(char* filepath){
    
    char* a = "a";
    char b[100];
    long unsigned int b_size = 100;
    
    compress(b,&b_size,a,strlen(a));
    

    printf("b : %s\n",b);
    printf("b_size : %d\n",b_size);
    printf("*b : %d\n",*b);
    BitStream* bs = bitstream_mopen(b,b_size,BS_READ);
    
    
    char c;
    
    unsigned char CINFO = 0;
     for(int i = 0; i < 4;i++){
        c = bitstream_read(bs);
        CINFO |= c << (3-i);
    }
    printf("CINFO : %d\n",CINFO);
    
    unsigned char CM = 0;
    for(int i = 0; i < 4;i++){
        c = bitstream_read(bs);
        CM |= c << (3-i);
    }
    printf("CM : %d\n",CM);
    
    unsigned char CMF = 0;
    CMF = CM | CINFO << 4;
    
    unsigned char FLEVEL = 0;
    for(int i = 0; i < 2;i++){
        c = bitstream_read(bs);
        FLEVEL |= c << (1-i);
    }
    printf("FLEVEL : %d\n",FLEVEL);
    
    unsigned char FDICT = bitstream_read(bs);
    printf("FDICT : %d\n",FDICT);
    
    unsigned char FCHECK = 0;
    for(int i = 0; i < 5;i++){
        c = bitstream_read(bs);
        FCHECK |= c << (4-i);
    }
    printf("FCHECK : %d\n",FCHECK);
    
    unsigned char FLAG = 0;
    FLAG = FLEVEL << 6 | FDICT << 5 | FCHECK;

    printf("(CMF * 256 + FLAG) % 31 = %d \n",(CMF*256+FLAG)%31);
    
    if ( FDICT == 1){
        // Eat Adler 32
        printf("eating dictionary\n");
        for(int i = 0; i < 4; i++)
            c = bitstream_read(bs);
    }
    
    for(int i = 0; i < 64; i++){
        c = bitstream_read(bs);
        printf("%d.",c);
    }
    
}