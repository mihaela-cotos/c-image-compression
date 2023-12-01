#include "utils.h"
#include <stdio.h>
#include <stdlib.h>


Image* create_img (unsigned int size) {
    
    Image* new_img = malloc(sizeof(Image));

    new_img->size = size;

    new_img->img_matrix = malloc(sizeof(Pixel*) * size);

    for (int i = 0; i < size; ++i) {
        new_img->img_matrix[i] = malloc(sizeof(Pixel) * size);
    }

    return new_img;
}

void fill_pixel (Pixel* pixel, unsigned char r,
                        unsigned char g, unsigned char b) {
    
    pixel->r = r;
    pixel->g = g;
    pixel->b = b;
}

void fill_img (Image* image, FILE* input_file) {

    for (int i = 0; i < image->size; ++i) {
        for (int j = 0; j < image->size; ++j) {
            unsigned char r,g,b;

            //read three colors in order to fill pixel
            fread(&r, sizeof(unsigned char), 1, input_file);
            fread(&g, sizeof(unsigned char), 1, input_file);
            fread(&b, sizeof(unsigned char), 1, input_file);

            fill_pixel(&(image->img_matrix[i][j]), r, g, b);
        }
    }

}

Pixel compute_avg_color (Image* img, Node* node) {

    unsigned int size = node->size;
    unsigned int pos_x = node->pos_x;
    unsigned int pos_y = node->pos_y;
    double avg_r = 0, avg_g = 0, avg_b = 0;

    Pixel avg_color; 

    avg_color.r = 0;
    avg_color.g = 0;
    avg_color.b = 0;

    for (int i = pos_x; i < pos_x + size; ++i) {
        for (int j = pos_y; j < pos_y + size; ++j) {
            avg_r += img->img_matrix[i][j].r;
            avg_g  += img->img_matrix[i][j].g;
            avg_b += img->img_matrix[i][j].b;
        }
    }
    
    double dummy;

    dummy = modf(avg_r / (size * size), &avg_r);
    dummy = modf(avg_g / (size * size), &avg_g);
    dummy = modf(avg_b / (size * size), &avg_b);

    avg_color.r = (unsigned int)avg_r;
    avg_color.g = (unsigned int)avg_g;
    avg_color.b = (unsigned int )avg_b;

    return avg_color;
}

unsigned int compute_similarity (Image* img, Node* node, Pixel* avg_pixel) {

    unsigned int size = node->size;
    unsigned int pos_x = node->pos_x;
    unsigned int pos_y = node->pos_y;
    unsigned long long sim_r = 0, sim_g = 0, sim_b = 0;
    double dummy = 0, res = 0;
    


    for (int i = pos_x; i < pos_x + size; ++i) {
        for (int j = pos_y; j < pos_y + size; ++j) {
            sim_r += (avg_pixel->r - img->img_matrix[i][j].r) * (avg_pixel->r - img->img_matrix[i][j].r);
            sim_g += (avg_pixel->g - img->img_matrix[i][j].g) * (avg_pixel->g - img->img_matrix[i][j].g);
            sim_b += (avg_pixel->b - img->img_matrix[i][j].b) * (avg_pixel->b - img->img_matrix[i][j].b);

        }
    }

    double mean = (sim_r + sim_g + sim_b) / (double)(3 * size * size);
    
    dummy = modf(mean, &res);

    return (unsigned int)res;

}


Node* create_node () {

    Node* new_node = malloc(sizeof(Node));

    new_node->size       = 0;
    new_node->level      = 0;
    new_node->pos_x      = 0;
    new_node->pos_y      = 0;
    new_node->good_node  = 2;

    new_node->color.r = 0;
    new_node->color.g = 0;
    new_node->color.b = 0;

    new_node->left_up    = NULL;
    new_node->left_down  = NULL;
    new_node->right_up   = NULL;
    new_node->right_down = NULL;

    return new_node;
}


void create_tree (Node* root, Image* img, int size, int x, int y, int level, int factor, Stats* stats) {
    
    root->pos_x = x;
    root->pos_y = y;
    root->size = size;
    root->level = level;

    Pixel avg_color = compute_avg_color(img, root);

    double score = compute_similarity(img, root, &avg_color);

    //check level
    if (stats->nr_levels < level) {
        stats->nr_levels = level;
    }

    if (score <= factor) {
        // this means score is good
        root->good_node = 1;
        root->color.r = avg_color.r;
        root->color.g = avg_color.g;
        root->color.b = avg_color.b;

        // update stats 
        stats->nr_blocks++;

        if (stats->biggest_block < size) {
            stats->biggest_block = size;
        }

        return;
    } else {
        root->good_node = 0;
        root->left_up = create_node();
        root->left_down = create_node();
        root->right_up = create_node();
        root->right_down = create_node();

        create_tree(root->left_up, img, size / 2, root->pos_x, root->pos_y, level + 1, factor, stats);
        create_tree(root->left_down, img, size / 2, root->pos_x + size / 2, root->pos_y, level + 1, factor, stats);
        create_tree(root->right_up, img, size / 2, root->pos_x, root->pos_y + size / 2, level + 1, factor, stats);
        create_tree(root->right_down, img, size / 2, root->pos_x + size / 2, root->pos_y + size / 2, level + 1, factor, stats);

    }
    
    
}


void printf_bfs_tree(Node* root, FILE* output_file) {
    
    Queue* queue = create_queue();    

    enqueue(queue, root);

    while (!is_queue_empty(queue)) {
      //dont forget to free queue node 
      QueueNode* dequeued_node = dequeue(queue);

      //write to the outptut the node and then delete it
        fwrite(&dequeued_node->value->good_node, sizeof(unsigned char), 1, output_file);
        if (dequeued_node->value->good_node == 1) {
            //create pixel
            fwrite(&(dequeued_node->value->color.r), sizeof(unsigned char), 1, output_file);        
            fwrite(&(dequeued_node->value->color.g), sizeof(unsigned char), 1, output_file);
            fwrite(&(dequeued_node->value->color.b), sizeof(unsigned char), 1, output_file);
        } else {
            enqueue(queue, dequeued_node->value->left_up);
            enqueue(queue, dequeued_node->value->right_up);
            enqueue(queue, dequeued_node->value->right_down);
            enqueue(queue, dequeued_node->value->left_down);
        }
        free(dequeued_node);
    }

    free(queue);
}


void free_image(Image* image) {
    
    for (int i = 0; i < image->size; ++i) {
        free(image->img_matrix[i]);
    }
    free(image->img_matrix);
    free(image);
}

void free_tree(Node* root) {

    if (root == NULL) return;
 
    /* first delete both subtrees */
    free_tree(root->left_up);
    free_tree(root->left_down);
    free_tree(root->right_up);
    free_tree(root->right_down);
   
    /* then delete the node */
    free(root);
}


void create_node_from_input (Node* node,Queue* queue, FILE* input) {
    
    int res = fread(&node->good_node, sizeof(unsigned char), 1, input);

    if (node->good_node == 1) {
        unsigned char r,g,b;
        int res1 = fread(&r, sizeof(unsigned char), 1, input);
        int res2 = fread(&g, sizeof(unsigned char), 1, input);
        int res3 = fread(&b, sizeof(unsigned char), 1, input);

        node->color.r = r;
        node->color.g = g;
        node->color.b = b;


    } else if (node->good_node == 0){

        enqueue(queue, node);
    }
    
}

Node* create_tree_from_input(FILE* input) {

    Node* root = malloc(sizeof(Node));
    Queue* queue = create_queue();

    unsigned char type;
    unsigned int size;

    fread(&root->size, sizeof(unsigned int), 1, input);
    fread(&root->good_node, sizeof(unsigned char), 1, input);

    if (root->good_node == 1) {
        root->pos_x = 0;
        root->pos_y = 0;
        root->level = 0;

        unsigned char r,g,b;
        int res1 = fread(&r, sizeof(unsigned char), 1, input);
        int res2 = fread(&g, sizeof(unsigned char), 1, input);
        int res3 = fread(&b, sizeof(unsigned char), 1, input);

        root->color.r = r;
        root->color.g = g;
        root->color.b = b;
        free(queue);
        return root;
    }
    root->pos_x = 0;
    root->pos_y = 0;
    root->level = 0;
    enqueue(queue, root);

    while(!is_queue_empty(queue)) {
        //dequeue a node check
        QueueNode* queue_node = dequeue(queue);
        Node* node = queue_node->value;

        node->left_up = create_node();
        node->left_up->size = node->size / 2;
        node->left_up->pos_x = node->pos_x;
        node->left_up->pos_y = node->pos_y;
        node->left_up->level = node->level + 1;

        node->left_down = create_node();
        node->left_down->size = node->size / 2;
        node->left_down->pos_x = node->pos_x + node->size / 2;
        node->left_down->pos_y = node->pos_y;
        node->left_down->level = node->level + 1;

        node->right_up = create_node();
        node->right_up->size = node->size / 2;
        node->right_up->pos_x = node->pos_x;
        node->right_up->pos_y = node->pos_y + node->size / 2;
        node->right_up->level = node->level + 1;

        node->right_down = create_node();
        node->right_down->size = node->size / 2;
        node->right_down->pos_x = node->pos_x + node->size / 2;
        node->right_down->pos_y = node->pos_y + node->size / 2;
        node->right_down->level = node->level + 1;

        create_node_from_input(node->left_up, queue, input);
        create_node_from_input(node->right_up, queue, input);
        create_node_from_input(node->right_down, queue, input);
        create_node_from_input(node->left_down, queue, input);
        free(queue_node);
        //create 4 children with corresponding level size position
    }
    free(queue);

    return root;
}

void fill_img_from_tree(Node* root, Image* img) {

    if (root == NULL) return;

    if (root->good_node == 1) {
        for (int i = root->pos_x; i < root->pos_x + root->size; ++i) {
            for (int j = root->pos_y; j < root->pos_y + root->size; ++j) {
                img->img_matrix[i][j].r = root->color.r;
                img->img_matrix[i][j].g = root->color.g;
                img->img_matrix[i][j].b = root->color.b;
            }
        }
    }
 
    /* first delete both subtrees */
    fill_img_from_tree(root->left_up, img);
    fill_img_from_tree(root->left_down, img);
    fill_img_from_tree(root->right_up, img);
    fill_img_from_tree(root->right_down, img);
   
}

Image* create_img_from_tree(Node* root) {

    Image* img = malloc(sizeof(Image));

    img->size = root->size;

    img->img_matrix = malloc(sizeof(Pixel*) * img->size);

    for (int i = 0; i < img->size; ++i) {
        img->img_matrix[i] = malloc(sizeof(Pixel) * img->size);
    }
    fill_img_from_tree(root, img);

    free_tree(root);
    return img;

}

void write_img_to_file(Image* img, FILE* output) {
        
    int count = 0, size_copy = img->size;
    char* buf = malloc(sizeof(char) * 25);
    snprintf(buf, 25, "P6\n%d %d\n255\n", img->size, img->size);

    while(size_copy > 0) {
        count++;
        size_copy /= 10;
    }
    
    fwrite(buf, sizeof(char), 9 + 2 * count, output);

    for (int i = 0; i < img->size; ++i) {
        for (int j = 0; j < img->size; ++j) {
            fwrite(&img->img_matrix[i][j].r, sizeof(unsigned char), 1, output);
            fwrite(&img->img_matrix[i][j].g, sizeof(unsigned char), 1, output);
            fwrite(&img->img_matrix[i][j].b, sizeof(unsigned char), 1, output);
        } 
    }


    free_image(img);

    free(buf);
}
 












