#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
}Pixel;


typedef struct Image {
    Pixel** img_matrix;
    unsigned int size;
} Image;


typedef struct Stats {
    unsigned int nr_levels;
    unsigned int nr_blocks;
    unsigned int biggest_block;
}Stats;


typedef struct Node{

    unsigned int level;
    unsigned int pos_x;
    unsigned int pos_y;
    unsigned int size;
    unsigned char good_node;

    Pixel color;

    struct Node* left_up;
    struct Node* left_down;
    struct Node* right_down;
    struct Node* right_up;
}Node;


// structres for queue

typedef struct QueueNode {
    Node* value;
    struct QueueNode* next;
} QueueNode;


typedef struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
} Queue;




Image*  create_img  (unsigned int size); 
void    fill_pixel  (Pixel* pixel, unsigned char r, unsigned char g, unsigned char b);
void    fill_img    (Image* image, FILE* input_file);
Node*   create_node ();
Pixel  compute_avg_color (Image* img, Node* node);
unsigned int compute_similarity (Image* img, Node* node, Pixel* avg_pixel);
void create_tree (Node* root, Image* img, int size, int x, int y, int level, int factor, Stats* stats);
void printf_bfs_tree(Node* root, FILE* output_file);

void free_image(Image* image);
void free_tree(Node* root);


// queue functions 
QueueNode* create_queue_node(Node* tree_value);
Queue* create_queue();
void enqueue(Queue* q, Node* tree_value);
QueueNode* dequeue(Queue* q);
int is_queue_empty(Queue* q);


void create_node_from_input (Node* node,Queue* queue, FILE* input);
Node* create_tree_from_input(FILE* input);
void fill_img_from_tree(Node* root, Image* img);
Image* create_img_from_tree(Node* root);
void write_img_to_file(Image* img, FILE* output);





