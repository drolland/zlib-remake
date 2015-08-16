#include <stdlib.h>
#include "crc.h"


unsigned long int my_hash(char* str,size_t len){
    
    unsigned long int result = 0;
    
    for ( int i = 0; i < len; i++){
        result += str[i] * ( str[i] << (i % 4)) + str[i];
        result += result * result << 6 ; 
    }
    
    return result;
}