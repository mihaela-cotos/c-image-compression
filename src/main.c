#include "utils.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {

    /* open input and ouptut files */

    if (strcmp(argv[1],"-c1") == 0 || strcmp(argv[1],"-c2") == 0) {

        FILE* input_file = fopen(argv[3], "rb");
        FILE* output_file = fopen(argv[4], "w");

        char dummy[100];
        char digit;
        int img_size = 0;
        int count = 0;
        int factor = atoi(argv[2]);

        fread(dummy, sizeof(char), 3, input_file);
        dummy[3] = '\0';

        fread(&digit, sizeof(char), 1, input_file);

        while (digit != ' ') {
            img_size = img_size * 10 + (digit - '0');
            fread(&digit, sizeof(char), 1, input_file);
           count++;
        }

        // jump over the rest of the text
        fread(dummy, sizeof(char),count + 5 , input_file);

        Image* image = create_img(img_size);
        Stats* stats = malloc(sizeof(Stats));
        Node* root = create_node(); 

        stats->nr_levels = 0;
        stats->nr_blocks = 0;
        stats->biggest_block = 0;

        //here we fill the image 
        fill_img(image, input_file);

        create_tree(root, image, img_size, 0, 0, 0, factor, stats);
        stats->nr_levels++;
    
        if (strcmp(argv[1],"-c1") == 0) {
            fprintf(output_file, "%d\n%d\n%d\n", 
                stats->nr_levels, stats->nr_blocks, stats->biggest_block);
        } else if (strcmp(argv[1],"-c2") == 0) {
           fwrite(&img_size, sizeof(unsigned int), 1, output_file);
           printf_bfs_tree(root, output_file);
        }

        free_image(image);
        free_tree(root);


        fclose(input_file);
        fclose(output_file);
        free(stats);
    } else {

        FILE* input_file = fopen(argv[2], "rb");
        FILE* output_file = fopen(argv[3], "w");
        
        Node* root = create_tree_from_input(input_file);
        Image* img = create_img_from_tree(root);
        write_img_to_file(img, output_file);


        fclose(input_file);
        fclose(output_file);

    }



    return 0;
}
