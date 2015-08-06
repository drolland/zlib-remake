#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <cairo/cairo.h>
#include <math.h>
#include "huffman.h"
#include "dmemory.h"
#include "dlist.h"
#include "dbinarytree.h"
#include "derror.h"

#define MAX(a,b) a > b ? a : b

enum {
    FALSE,
    TRUE
};





DList* huff_list_insert_tree_node_sorted_by_weight(DList* list, DBinaryTree* to_insert) {

    assert(to_insert != NULL);

    /* Ignore symbols with a weight of zero*/
    if ( ((HuffSymbol*)to_insert->content)->weight == 0)
        return list;
    
    DList* new_element = ( DList*)checked_malloc(sizeof(DList*));
    new_element->content = to_insert;
    new_element->next = NULL;
    
    if (list == NULL) {
        list = new_element;
        return list;
    } 
    
    HuffSymbol* current_symbol = ((DBinaryTree*)list->content)->content;
    
    if (  ((HuffSymbol*)to_insert->content)->weight < current_symbol->weight   ){ 
        DList* temp = list;
        list = new_element;
        list->next = temp;
        return list;
    }
        
    DList* current = list;
    HuffSymbol* current_next_symbol = NULL;
           
    while (current->next != NULL){
        assert(current->next->content != NULL);
        current_next_symbol = ((DBinaryTree*)current->next->content)->content;
        if ( ((HuffSymbol*)to_insert->content)->weight < current_next_symbol->weight ){
            DList* temp = current->next;
            current->next = new_element;
            new_element->next = temp;
            return list;
        }
        else {
        current = current->next;  
        }
    }      
    current->next = new_element;
    return list;
}


void huff_weight_list_print(DList* list){
    DList* current = list;
    HuffSymbol* current_symbol;
    while(current != NULL){
        current_symbol = current->content;
        printf("symbol '%c', weight %d\n",current_symbol->value,current_symbol->weight);
        current = current->next;
    }
}



DBinaryTree* huff_build_tree(char *str, size_t len) {

    /* Weight table for the ascii alphabet*/
    int weight_table[256];
    memset(weight_table, 0, 256 * sizeof (int)); /* Zero weight table */

    /* Read the string in one pass and store weight for each symbol in the weight table*/
    for (int i = 0; i < len; i++) {
        weight_table[(unsigned char)str[i]]++;
    }

    /* DEBUG : Display weight table*/
    for (int i = 0; i < 256; i++) {
        printf("%d : %d\n", i, weight_table[i]);
    }
    
    /* Create a list of DBinaryTree Leafs containing symbols sorted by weight from weight table */
    
   DList* leaf_list = NULL;
   HuffSymbol* symbol = NULL;
   for (int i = 0; i < 256; i++) {
        DBinaryTree* leaf = (DBinaryTree*)checked_malloc(sizeof(DBinaryTree));
        symbol = (HuffSymbol*)checked_malloc(sizeof(HuffSymbol));       
        symbol->weight = weight_table[i];
        symbol->value = i;
        leaf->content = symbol;
        leaf->left = NULL;
        leaf->right = NULL;
        leaf_list = huff_list_insert_tree_node_sorted_by_weight(leaf_list,leaf);
    }
     
    /* Build huffman tree*/
    DBinaryTree* huff_tree = NULL;
    
    assert(leaf_list != NULL);
    /* If only one symbol, tree has a single leaf*/
    if ( leaf_list->next == NULL){
        huff_tree = leaf_list->content; 
        dlist_free(leaf_list);
        return huff_tree;
    }
    
     /* Create a new node from two symbols  */
    DBinaryTree* new_node = (DBinaryTree*)checked_malloc(sizeof(DBinaryTree));  
    HuffSymbol* new_content = (HuffSymbol*)checked_malloc(sizeof(HuffSymbol));
    assert ( leaf_list != NULL && leaf_list->next != NULL);
    new_node->content = new_content;
    new_node->left = leaf_list->content;
    new_node->right = leaf_list->next->content;
    new_content->weight = ((HuffSymbol*)(new_node->left->content))->weight + ((HuffSymbol*)(new_node->right->content))->weight;
    
    if ( leaf_list->next->next == NULL){
        /* If only two symbol, tree has a single node with two leaf*/     
        huff_tree = new_node;
        dlist_free(leaf_list);
        return huff_tree;
    }
    
    /* remove the two symbols from the list, do list elements mem cleanup */
    DList* temp = leaf_list;
    leaf_list = leaf_list->next->next; 
    free(temp->next);
    free(temp);
    
    /* Put the newly created node in the list*/
    leaf_list = huff_list_insert_tree_node_sorted_by_weight(leaf_list,new_node);
    
     /* Procced for each remaining pair of symbol */
    while(leaf_list->next != NULL){
        
        new_node = (DBinaryTree*)checked_malloc(sizeof(DBinaryTree));  
        new_content = (HuffSymbol*)checked_malloc(sizeof(HuffSymbol));
        new_node->content = new_content;
        new_node->left = leaf_list->content;
        new_node->right = leaf_list->next->content;
        new_content->weight = ((HuffSymbol*)(new_node->left->content))->weight + ((HuffSymbol*)(new_node->right->content))->weight;
        
        /* remove the two symbols from the list, do list elements mem cleanup */
        temp = leaf_list;
        leaf_list = leaf_list->next->next; 
        free(temp->next);
        free(temp);
        
        leaf_list = dlist_append(leaf_list,new_node);                
    }
    
    huff_tree = leaf_list->content;
    dlist_free(leaf_list);
    return huff_tree;
   
}

void huff_compute_tree_node_stats_recursive(HuffTreeStats* stats,DBinaryTree* node,int depth){
    
    // If node is a leaf
    if ( node->left == NULL || node->right == NULL){
        assert(node->left == NULL && node->right==NULL);
        stats->nb_symbols++;
        stats->total_weight += ((HuffSymbol*)node->content)->weight;
        stats->total_bit_size += ((HuffSymbol*)node->content)->weight * depth;
        assert(((HuffSymbol*)node->content)->value >= 0 && ((HuffSymbol*)node->content)->value < 256);
        stats->weight[((HuffSymbol*)node->content)->value] = ((HuffSymbol*)node->content)->weight;
        stats->code_length[((HuffSymbol*)node->content)->value] = depth;
        if ( depth > stats->max_code_length) stats->max_code_length = depth;
    }
    else {
        assert(node->left != NULL && node->right != NULL);
        huff_compute_tree_node_stats_recursive(stats,node->left,depth+1);
        huff_compute_tree_node_stats_recursive(stats,node->right,depth+1);
    }
}

HuffTreeStats* huff_compute_tree_stats(DBinaryTree* tree){
    HuffTreeStats* stats = (HuffTreeStats*)checked_malloc(sizeof(HuffTreeStats));
    memset(stats,0,sizeof(HuffTreeStats));
    huff_compute_tree_node_stats_recursive(stats,tree,0);
    return stats;
}

void huff_print_tree_stats(HuffTreeStats* stats){
    printf("*-- Tree Stats --*\n");
    printf("Number of symbol : %d\n",stats->nb_symbols);
    printf("Uncompressed Block Size : %d bits\n",stats->total_weight * 8);
    printf("Compressed Block Size : %d bits \n",stats->total_bit_size);
    printf("Block Compression Ratio : %f\n",(float)stats->total_bit_size / stats->total_weight / 8.0f );
    float estimation_of_tree_size_1 = 8 + stats->nb_symbols * 16;
    float estimation_of_tree_size_2 = stats->nb_symbols * ( 3 + 6 ) + 8;
    printf("Estimation 1 of Tree Size : %f bits\n",estimation_of_tree_size_1 );
    printf("Estimation 2 of Tree Size : %f bits\n",estimation_of_tree_size_2 );
    printf("Estimation 1 of total compression ratio : %f bits\n", (estimation_of_tree_size_1 + (float)stats->total_bit_size) / stats->total_weight / 8.0f );
    printf("Estimation 2 of total compression ratio : %f bits\n", (estimation_of_tree_size_2 + (float)stats->total_bit_size) / stats->total_weight / 8.0f );
    printf("Max code length : %d\n",stats->max_code_length);
}

const int WIDTH = 1024 * 4 ;
const int HEIGHT = 768 * 4 ;
const float STEP = 50.0f ;


void draw_dbg_print_node_recursive(cairo_t* cr, DBinaryTree* node,int depth){
   
    
    cairo_save(cr);
    cairo_arc(cr,0,0,STEP / 1.414f,0,2*3.15);
    cairo_stroke (cr);
    cairo_restore(cr);
        
    cairo_text_extents_t te;
    char text[2];
    text[0] = ((HuffSymbol*)node->content)->value;
    if ( ((HuffSymbol*)node->content)->value == ' ')
        text[0] = '_';
    text[1] = '\0';
    
    cairo_save(cr);
    cairo_set_font_size(cr,STEP );
    cairo_text_extents (cr, text, &te);
    cairo_translate (cr, - te.x_bearing - te.width / 2,- te.y_bearing - te.height / 2);
    cairo_show_text (cr,text);
    cairo_stroke (cr);
    cairo_restore(cr);
    

    char buffer[16];
    sprintf(buffer,"%d",((HuffSymbol*)node->content)->weight);
    cairo_set_font_size(cr,STEP / 2.0f);
    cairo_text_extents (cr, buffer, &te);
    cairo_move_to (cr, - te.x_bearing - te.width / 2, STEP * 1.5f - te.y_bearing - te.height / 2);
    cairo_show_text (cr,buffer);
    cairo_stroke (cr);
    
    int translate_distance = ( WIDTH + 800 )/ ( pow(2,depth) + 1 ) / 2;
    
    if ( node->right != NULL ){
        cairo_save(cr);
        cairo_translate(cr, translate_distance ,  STEP * 4 );
        draw_dbg_print_node_recursive(cr,node->right,depth+1);
        cairo_restore(cr);
    }
    
    if ( node->left != NULL ){
        cairo_save(cr);
        cairo_translate(cr,- translate_distance,  STEP * 4);
        draw_dbg_print_node_recursive(cr,node->left,depth+1);
        cairo_restore(cr);
    }

    
}

void huff_dbg_print_tree(DBinaryTree* tree){
    
    cairo_surface_t* surface;
    cairo_t* cr;
    
    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,WIDTH,HEIGHT);
    cr = cairo_create (surface);
    
    cairo_set_source_rgb(cr,0,0,0);
    cairo_rectangle(cr,0,0,WIDTH,HEIGHT);
    cairo_fill(cr);

    cairo_translate(cr,WIDTH/2,STEP);
    cairo_set_line_width(cr,2);
    cairo_set_source_rgb(cr,1,1,1);
    
    draw_dbg_print_node_recursive(cr,tree,1);      
    
    cairo_surface_write_to_png (surface,"tree.png");
}
