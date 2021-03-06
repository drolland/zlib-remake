/*#include "bitstream.h"
#include "dmemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_BUFFER_SIZE 16384

struct bitstream {
    int stream_type;
    int open_mode;   
    int buffer_remaining_bits;
    char* buffer;
    int buffer_pos;
};

typedef struct bitstream_file {
    BitStream bstream;
    FILE* fd;
    int file_buffer_size;
    char file_buffer[FILE_BUFFER_SIZE];
    
} BitStreamFile;

enum {
    BS_FILE,
    BS_MEMORY
};


static inline BitStream* bitstream_fopen(char* path, int open_mode){
    
    BitStreamFile* bsf = (BitStreamFile*)checked_malloc(sizeof(BitStreamFile));
    bsf->bstream.stream_type = BS_FILE;
    bsf->bstream.open_mode = open_mode;
    bsf->bstream.buffer = bsf->file_buffer;
    
    if ( open_mode == BS_READ){
        bsf->fd = fopen(path,"rb");
        bsf->bstream.buffer_remaining_bits = 0;
        bsf->file_buffer_size = -1;
    }
    else if ( open_mode == BS_WRITE){
        bsf->fd = fopen(path,"wb");
        bsf->bstream.buffer_remaining_bits = 8;
        memset(bsf->file_buffer,0,FILE_BUFFER_SIZE * sizeof(char));
    }
    else {
        fprintf(stderr,"bit_stream_fopen : Wrong open_mode, should be BS_READ or BS_WRITE\n");
        return NULL;
    }
    
    if ( bsf->fd == NULL ){
        fprintf(stderr,"bit_stream_fopen : Can't open file %s\n",path);
        return NULL;
    }
    
    return (BitStream*)bsf; 
  
}

static inline void bitstream_close(BitStream* bs){
    
    int bits_to_fill_count = ( 8 - bs->buffer_remaining_bits );
    
    while ( bits_to_fill_count-- > 0){
        bitstream_write(bs,0);
    } 
    
    
    if ( bs->stream_type == BS_FILE ){
        BitStreamFile* bsf = (BitStreamFile*)bs;
        int remaining_bytes = bsf->bstream.buffer - bsf->file_buffer;
        if ( remaining_bytes > 0)
            fwrite(bsf->file_buffer,sizeof(char),remaining_bytes,bsf->fd);
        fclose(((BitStreamFile*)bs)->fd);
    }
    free(bs);
}

static inline void bitstream_write(BitStream* bs,char bit){
            
    assert( bs->open_mode == BS_WRITE && bit >= 0 && bit <= 1);
    
    (*bs->buffer) = ( *(bs->buffer) | ( bit << bs->buffer_remaining_bits - 1 ) );
    bs->buffer_remaining_bits--;
    
    if ( bs->buffer_remaining_bits == 0 ){
        bs->buffer_remaining_bits = 8;
        bs->buffer++;
        if ( bs->stream_type == BS_FILE){
            BitStreamFile* bsf = (BitStreamFile*)bs;
            if ( bs->buffer >= bsf->file_buffer + FILE_BUFFER_SIZE ){
                bs->buffer = bsf->file_buffer;
                fwrite(bsf->file_buffer,sizeof(char),FILE_BUFFER_SIZE,bsf->fd);
                memset(bsf->file_buffer,0,FILE_BUFFER_SIZE * sizeof(char));
            }
        }
    }
} 

static inline char bitstream_read(BitStream* bs){
    
    assert(bs->open_mode == BS_READ);
    
    if ( bs->buffer_remaining_bits == 0){
        
        if ( bs->stream_type == BS_FILE ){
            BitStreamFile* bsf = (BitStreamFile*)bs;
            if ( bsf->bstream.buffer >= bsf->file_buffer + bsf->file_buffer_size - 1 )
                if ( bsf->file_buffer_size != FILE_BUFFER_SIZE && bsf->file_buffer_size != -1 )
                    return -1;
                else{
                    bsf->file_buffer_size = fread(bsf->file_buffer,sizeof(char),FILE_BUFFER_SIZE,bsf->fd);
                    bsf->bstream.buffer = bsf->file_buffer;
                }
            else {
                bs->buffer++;  
            }
        }
        else {
            bs->buffer++;
        }
        bs->buffer_remaining_bits = 8;
    }
     
    return ( *(bs->buffer) >> ( bs->buffer_remaining_bits-- - 1) ) & 0b00000001;    
}*/