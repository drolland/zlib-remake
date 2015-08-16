#pragma once


/* Read the content of a file into a buffer
 *  char** buffer : A pointer to a char buffer thet will be allocated and filled with the content of the file
 *  char* file_path : path to the file
 *  Returns the amount of char read or -1 is an error occured
 */
int dfile_read_into_buffer(char** buffer,char* file_path);

