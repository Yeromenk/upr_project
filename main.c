#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <assert.h>

#include "tga.h"

int main(int argc, char **argv) {
    const char *input = NULL;
    const char *output = NULL;
    FILE *input_file = NULL;
    FILE *output_file = NULL;
    tga_t *tga = NULL;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--input") == 0) {
            input = argv[i + 1];
        } else if (strcmp(argv[i], "--output") == 0) {
            output = argv[i + 1];
        }
    }

    if (input == NULL || output == NULL) {
        printf("photoshop: illegal arguments\nusage: photoshop --input input_tga --output output_tga\n");
        return 1;
    }

    input_file = fopen(input, "r");   // read 
    output_file = fopen(output, "w"); // write
    assert(input_file); 
    assert(output_file);

    tga = tga_read(input_file); // if cant read file - close file
    fclose(input_file);
    if (tga == NULL) {
        fclose(output_file);
        return 1;
    }

    char buffer[1024];
    bzero(buffer, 1024);
    while (1) {
        char *p = fgets(buffer, 1024, stdin);
        if (p == NULL) {
            fprintf(stderr, "Error reading from stdin\n");
            return 1;
        }
        buffer[strlen(buffer) - 1] = 0;

        p = strchr(buffer, ' ');
        if (p != NULL) {
            *p = 0;
        }
        
        if (strcmp(buffer, "exit") == 0) {
            tga_write(tga, output_file);
            break;
        } else if (strcmp(buffer, "save") == 0) {
            tga_write(tga, output_file);
        } else if (strcmp(buffer, "resize") == 0) {
            int width = 0;
            int height = 0;

            sscanf(p + 1, "%d,%d", &width, &height);
            tga_resize(tga, width, height);
        } else if (strcmp(buffer, "crop") == 0) {
            int x = 0;
            int y = 0;
            int width = 0;
            int height = 0;

            sscanf(p + 1, "%d,%d,%d,%d", &x, &y, &width, &height);
            tga_crop(tga, x, y, width, height);
        } else if (strcmp(buffer, "info") == 0) {
            tga_info(tga);
        } else if (strcmp(buffer, "copy") == 0) {
            int x1 = 0;
            int y1 = 0;
            int x2 = 0;
            int y2 = 0;
            int width = 0;
            int height = 0;

            sscanf(p + 1, "%d,%d,%d,%d %d,%d", &x1, &y1, &width, &height, &x2, &y2);
            tga_copy(tga, x1, y1, width, height, x2, y2);
        } else if(strcmp(buffer, "move") == 0) {
            int x = 0;
            int y = 0;

            sscanf(p + 1, "%d,%d", &x, &y);
            tga_move(tga, x, y);
        } else if(strcmp(buffer, "bw") == 0) {
            tga_bw(tga);
        } else {
            printf("Unknown command\n");
        }

        bzero(buffer, 1024);
    }

    tga_destroy(tga);
    fclose(output_file);


    return 0;
}
