#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lz77.h"
#include "dlist.h"
#include "crc.h"
#include "dmemory.h"

#define HASH_TABLE_SIZE 16384
#define HASH_CHAIN_SIZE 3

enum {
    LENGTH_CODE = 297,
    DISTANCE_CODE = 999,
    END_OF_BLOCK = 256
};

typedef struct hash_chain {
    unsigned char** next_insert;
    unsigned char** search_end;
    unsigned char* entries[HASH_CHAIN_SIZE];
} HashChain;

HashChain* hash_table[HASH_TABLE_SIZE];

typedef struct lz77_match {
    unsigned char* position;
    size_t length;
} LZ77_Match;

void lz77_hash_table_insert(HashChain** hash_table, int hash_index, unsigned char* pos) {

    
    HashChain* chain = hash_table[hash_index];

    if (chain == NULL) {
        chain = checked_malloc(sizeof (HashChain));
        hash_table[hash_index] = chain;
        chain->search_end = chain->entries;
        chain->next_insert = chain->entries + 1;
        *(chain->entries) = pos;
        
        
    } else {
        *(chain->next_insert) = pos;

        if (chain->next_insert == chain->search_end) {
            chain->next_insert++;
            chain->search_end++;
        } else {
            chain->next_insert++;
        }

                    
       
        assert(chain->next_insert <= &(chain->entries[HASH_CHAIN_SIZE + 1]) );
        if (chain->next_insert == &(chain->entries[HASH_CHAIN_SIZE + 1])) {
            if (chain->next_insert == chain->search_end) {
                chain->next_insert = chain->entries;
                chain->search_end = chain->entries;
            } else
                chain->next_insert = chain->entries;
        }

    }

}

void lz77_search_for_match(LZ77_Match* match, int hash_index, unsigned char* pos, size_t maxlen) {

    if (hash_table[hash_index] == NULL) {

        match->length = 0;
    } else {
        HashChain* current_hash_chain = hash_table[hash_index];
        unsigned char** current_search = current_hash_chain->next_insert;
        int hash_chain_index = -1;
        int match_chain_index = 0;
        int matches_max_length = -1;

        do {

            assert(current_search >= current_hash_chain->entries && current_search <= &(current_hash_chain->entries[HASH_CHAIN_SIZE]));
            if (current_search == current_hash_chain->entries)
                current_search = &(current_hash_chain->entries[HASH_CHAIN_SIZE]);
            else
                current_search--;

            // Remove old entries

            if (pos - *current_search > 32768) {
                current_hash_chain->search_end = current_search;
                break;
            }

            hash_chain_index++;

            unsigned char* match_pos = *current_search;
            unsigned char* current_pos = pos;
            int match_length = 0;


            while (current_pos < pos + maxlen && *match_pos == *current_pos) {
                match_pos++;
                current_pos++;
                match_length++;
            }



            if (match_length > matches_max_length) {
                matches_max_length = match_length;
                match_chain_index = hash_chain_index;
            }



        } while (current_search != current_hash_chain->search_end);

        if (hash_chain_index == -1 || matches_max_length == 0) {
            match->length = 0;
            return;
        }

        match->length = matches_max_length;

        current_search = current_hash_chain->next_insert - 1;
        if (current_search - hash_chain_index >= current_hash_chain->entries)
            match->position = *(current_search - hash_chain_index);
        else {
            assert(current_search - hash_chain_index + HASH_CHAIN_SIZE >= current_hash_chain->entries);
            match->position = *(current_search - hash_chain_index + HASH_CHAIN_SIZE);
        }

    }
}

void lz77_encode(unsigned short int* dst, unsigned char* src, size_t buffer_size) {

    unsigned char* pos = src;
    unsigned char* pos_start = src;
    unsigned char* pos_end = pos_start + buffer_size;
    unsigned short int* pos_out = dst;
    LZ77_Match match;
    int hash_index;

    memset(hash_table, 0, HASH_TABLE_SIZE * sizeof (char));
    hash_index = my_hash(pos, 3) % HASH_TABLE_SIZE;

    while (pos < pos_end - 3) {

        lz77_search_for_match(&match, hash_index, pos, pos_end - pos);

        if (match.length == 0) {
            lz77_hash_table_insert(hash_table, hash_index, pos);
            *(pos_out++) = *(pos++);
            hash_index = my_hash(pos, 3) % HASH_TABLE_SIZE;

        } else {

            *pos_out = LENGTH_CODE + match.length;
            pos_out++;
            *pos_out = (long int) DISTANCE_CODE + (long int) (pos - (match.position));
            pos_out++;

            for (int i = 0; i < match.length; i++) {
                lz77_hash_table_insert(hash_table, hash_index, pos);
                pos++;
                hash_index = my_hash(pos, 3) % HASH_TABLE_SIZE;
            }
        }

    }

    while (pos < pos_end)
        *(pos_out++) = *(pos++);

    *pos_out = END_OF_BLOCK;
}

void lz77_dbg_print_buffer(unsigned short int* buffer, size_t buffer_size) {


    for (int i = 0; i < buffer_size; i++) {
        if (*(buffer + i) == END_OF_BLOCK) {
            printf("\nEND OF FILE : size %d symbols\n", i);
            printf(" Compression estimation : %f\n", (float) i / (float) buffer_size);
            break;
        }

        if (*(buffer + i) < 300) printf("%c", *((char*) (buffer + i)));
        else if (*(buffer + i) < 1000) printf("<%d,", *(buffer + i) - 297);
        else printf("%d>", *(buffer + i) - 999);


    }
}

/*
 
 Algo
 
   - read 3 string
   - compute hash
   - search in hash table
   - if no match procede to next character and step 1
   - if matches, select the longest match
   - im matches output and advance 
                
 
 
 
 
 
 
 
 
 
 
 
 
 */