#pragma once


void lz77_encode(unsigned short int* dst,unsigned char* src, size_t buffer_size);
void lz77_encode_file(unsigned short int* dst,FILE* fd, size_t file_size);

void lz77_dbg_print_buffer(unsigned short int* buffer,size_t buffer_size);