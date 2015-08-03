#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <cairo/cairo.h>
#include <math.h>
#include "huffman.h"
#include "dmemory.h"

enum {
    FALSE,
    TRUE
};


typedef struct huffman_tree_node {
    struct huffman_tree_node* next;
    struct huffman_tree_node* rhl; /* Right leaf */
    struct huffman_tree_node* lhl; /* Left leaf */
    unsigned int weight;
    char symbol;
} Huffman_Tree_Node;

void huffman_weight_list_insert(Huffman_Tree_Node** list, Huffman_Tree_Node* to_insert) {

    assert(to_insert != NULL);

    /* Ignore symbols with a weight of zero*/
    if ( to_insert->weight == 0)
        return;
    
    if (*list == NULL) {
        *list = to_insert;
        return;
    } 
    
    if ( (*list)->weight > to_insert->weight){
        Huffman_Tree_Node* temp = *list;
        *list = to_insert;
        (*list)->next = temp;
    }
        
    Huffman_Tree_Node* current = *list;
           
    while (current->next != NULL){
            if ( current->next->weight > to_insert->weight ){
                Huffman_Tree_Node* temp = current->next;
                current->next = to_insert;
                current->next->next = temp;
                return;
            }
            current = current->next;          
        }      
    current->next = to_insert;
}


void huffman_weight_list_print(Huffman_Tree_Node* list){
    Huffman_Tree_Node* current = list;
    while(current != NULL){
        printf("symbol '%c', weight %d\n",current->symbol,current->weight);
        current = current->next;
    }
}

const int WIDTH = 1024 * 2 ;
const int HEIGHT = 768 * 2 ;
const float STEP = 50.0f;

void draw_tree_node_recursive(cairo_t* cr, Huffman_Tree_Node* node,int depth){
   
    /*
    cairo_save(cr);
    cairo_arc(cr,0,0,STEP / 1.414f,0,2*3.15);
    cairo_stroke (cr);
    cairo_restore(cr);
        
    cairo_text_extents_t te;
    char text[2];
    text[0] = node->symbol;
    if ( node->symbol == ' ')
        text[0] = '_';
    text[1] = '\0';
    
    cairo_save(cr);
    cairo_set_font_size(cr,STEP );
    cairo_text_extents (cr, text, &te);
    cairo_translate (cr, - te.x_bearing - te.width / 2,- te.y_bearing - te.height / 2);
    cairo_show_text (cr,text);
    cairo_stroke (cr);
    cairo_restore(cr);
    

    cairo_set_font_size(cr,STEP / 2.0f);
    cairo_text_extents (cr, "142", &te);
    cairo_move_to (cr, STEP - te.x_bearing , - te.y_bearing - te.height / 2);
    cairo_show_text (cr,"142");
    cairo_stroke (cr);
    
    
    
    if ( node->rhl != NULL ){
        cairo_save(cr);
        cairo_translate(cr,STEP * 8 / depth , depth * STEP );
        draw_tree_node_recursive(cr,node->rhl,depth+1);
        cairo_restore(cr);
    }
    
    if ( node->lhl != NULL ){
        cairo_save(cr);
        cairo_translate(cr,-STEP * 8 / depth, depth * STEP );
        draw_tree_node_recursive(cr,node->lhl,depth+1);
        cairo_restore(cr);
    }
*/
    
}

void huffman_tree_print(Huffman_Tree_Node* tree,int depth){
    /*
    cairo_surface_t* surface;
    cairo_t* cr;
    

    
    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,WIDTH,HEIGHT);
    cr = cairo_create (surface);

    cairo_translate(cr,WIDTH/2,STEP*4);
    cairo_set_line_width(cr,2);
    cairo_set_source_rgb(cr,0.5f,0.7f,0);
    
    draw_tree_node_recursive(cr,tree,1);      
    
    cairo_surface_write_to_png (surface,"tree.png");*/
}

void huffman_build_weight_list(Huffman_Tree_Node** weight_list,char *str, int len) {

    unsigned int weight_table[256];
    memset(weight_table, 0, 256 * sizeof (unsigned int)); /* Zero weight table */

    /* Read the string in one pass and store weight for each symbol in the weight table*/
    for (int i = 0; i < len; i++) {
        weight_table[str[i]]++;
    }

    /* Display weight table*/
    for (int i = 0; i < 256; i++) {
        printf("%d : %d\n", i, weight_table[i]);
    }

    /* Convert weight table into weight list*/
    Huffman_Tree_Node* list = NULL;
    Huffman_Tree_Node* to_insert = NULL;
    for (int i = 0; i < 256; i++) {
        to_insert = (Huffman_Tree_Node*)checked_malloc(sizeof(Huffman_Tree_Node));                
        to_insert->symbol = i;
        to_insert->weight = weight_table[i];
        to_insert->next = NULL;
        to_insert->rhl = NULL;
        to_insert->lhl = NULL;
        huffman_weight_list_insert(&list,to_insert);
    }

    /* DEBUG : Display sorted weight list*/
    huffman_weight_list_print(list);
    
    /* Output in parameter, the calcucated sym wight list*/
    *weight_list = list;

}

void huffman_build_tree(Huffman_Tree_Node** tree,Huffman_Tree_Node* list){
    
    assert(list != NULL);
           
    /* If only one symbol, tree has a single leaf*/
    if ( list->next == NULL){
        assert(list->next == NULL);
        *tree = list; 
        return;
    }
        
    /* Create a new node from two symbols  */
    Huffman_Tree_Node* new_node = (Huffman_Tree_Node*)checked_malloc(sizeof(Huffman_Tree_Node));  
    new_node->next = NULL;
    assert ( list != NULL && list->next != NULL);
    new_node->lhl = list;
    new_node->rhl = list->next;
    new_node->weight = new_node->lhl->weight + new_node->rhl->weight;
    
    if ( list->next->next == NULL){
        /* If only two symbol, tree has a single node with two leaf*/
        *tree = list;
        return;
    }
    
    /* remove the two symbols from the list */
    list = list->next->next; 
    
    /* Put the newly created node in the list*/
    huffman_weight_list_insert(&list,new_node);
      
    
    /* Procced for each remaining pair of symbol */
    while(list->next != NULL){
        
        new_node = (Huffman_Tree_Node*)checked_malloc(sizeof(Huffman_Tree_Node));
        new_node->next = NULL;
        new_node->lhl = list;
        new_node->rhl = list->next;
        new_node->weight = new_node->lhl->weight + new_node->rhl->weight;
        list = list->next->next; 
        huffman_weight_list_insert(&list,new_node);
        
    }
    
    *tree = list;
    
}


void huffman_encode(char* str, size_t len) {
    
    Huffman_Tree_Node* weight_list = NULL;
    Huffman_Tree_Node* tree = NULL;
    huffman_build_weight_list(&weight_list,str, len);
    huffman_build_tree(&tree,weight_list);
    huffman_tree_print(tree,0);
    
}