#include <stdio.h>
#include <assert.h>
#include "dfile.h"
#include "dmemory.h"


int dfile_read_into_buffer(char** buffer, char* file_path){
    
    FILE* fd = fopen(file_path,"rb");
    if ( fd == NULL)
        return -1;
    
    fseek(fd,0,SEEK_END);
    int length = ftell(fd);
    rewind(fd);
    
    *buffer = checked_malloc((length+1)*sizeof(char));
    
    int nb_read = fread(*buffer,sizeof(char),length,fd); 
    
    (*buffer)[nb_read] = '\0';
 
    assert( length == nb_read);
    
    fclose(fd);
    return nb_read;
      
}
