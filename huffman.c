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


typedef struct huff_symbol {
    unsigned int weight;
    int value;
} Huff_Symbol;


DList* huff_list_insert_tree_node_sorted_by_weight(DList* list, DBinaryTree* to_insert) {

    assert(to_insert != NULL);

    /* Ignore symbols with a weight of zero*/
    if ( ((Huff_Symbol*)to_insert->content)->weight == 0)
        return list;
    
    DList* new_element = ( DList*)checked_malloc(sizeof(DList*));
    new_element->content = to_insert;
    new_element->next = NULL;
    
    if (list == NULL) {
        list = new_element;
        return list;
    } 
    
    Huff_Symbol* current_symbol = ((DBinaryTree*)list->content)->content;
    
    if (  ((Huff_Symbol*)to_insert->content)->weight < current_symbol->weight   ){ 
        DList* temp = list;
        list = new_element;
        list->next = temp;
        return list;
    }
        
    DList* current = list;
    Huff_Symbol* current_next_symbol = NULL;
           
    while (current->next != NULL){
        assert(current->next->content != NULL);
        current_next_symbol = ((DBinaryTree*)current->next->content)->content;
        if ( ((Huff_Symbol*)to_insert->content)->weight < current_next_symbol->weight ){
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
    Huff_Symbol* current_symbol;
    while(current != NULL){
        current_symbol = current->content;
        printf("symbol '%c', weight %d\n",current_symbol->value,current_symbol->weight);
        current = current->next;
    }
}



DBinaryTree* huff_build_tree(char *str, size_t len) {

    /* Weight table for the ascii alphabet*/
    unsigned int weight_table[256];
    memset(weight_table, 0, 256 * sizeof (unsigned int)); /* Zero weight table */

    /* Read the string in one pass and store weight for each symbol in the weight table*/
    for (int i = 0; i < len; i++) {
        weight_table[str[i]]++;
    }

    /* DEBUG : Display weight table*/
    for (int i = 0; i < 256; i++) {
        printf("%d : %d\n", i, weight_table[i]);
    }
    
    /* Create a list of DBinaryTree Leafs containing symbols sorted by weight from weight table */
    
   DList* leaf_list = NULL;
   Huff_Symbol* to_insert = NULL;
   for (int i = 0; i < 256; i++) {
        DBinaryTree* leaf = (DBinaryTree*)checked_malloc(sizeof(DBinaryTree));
        to_insert = (Huff_Symbol*)checked_malloc(sizeof(Huff_Symbol));       
        to_insert->weight = weight_table[i];
        to_insert->value = i;
        leaf->content = to_insert;
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
    Huff_Symbol* new_content = (Huff_Symbol*)checked_malloc(sizeof(Huff_Symbol));
    assert ( leaf_list != NULL && leaf_list->next != NULL);
    new_node->content = new_content;
    new_node->left = leaf_list->content;
    new_node->right = leaf_list->next->content;
    new_content->weight = ((Huff_Symbol*)(new_node->left->content))->weight + ((Huff_Symbol*)(new_node->right->content))->weight;
    
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
        new_content = (Huff_Symbol*)checked_malloc(sizeof(Huff_Symbol));
        new_node->content = new_content;
        new_node->left = leaf_list->content;
        new_node->right = leaf_list->next->content;
        new_content->weight = ((Huff_Symbol*)(new_node->left->content))->weight + ((Huff_Symbol*)(new_node->right->content))->weight;
        
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
    text[0] = ((Huff_Symbol*)node->content)->value;
    if ( ((Huff_Symbol*)node->content)->value == ' ')
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
    sprintf(buffer,"%d",((Huff_Symbol*)node->content)->weight);
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
