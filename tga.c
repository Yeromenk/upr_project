#include "tga.h"

#include <stdlib.h>
#include <string.h>

//read a tga 
tga_t *tga_read(FILE *file) {
    tga_t *tga;

    if (file == NULL) {
        fprintf(stderr, "TGA file is NULL\n");
        return NULL;
    }

    tga = (tga_t *)malloc(sizeof(tga_t));
    tga->pixels = NULL;

    // Read header from input.tga
    if (fread(&tga->header, sizeof(tga_header_t), 1, file) != 1) {
        fprintf(stderr, "Unable to read from TGA file\n");
        tga_destroy(tga);
        return NULL;
    }
    
    // Set origin to left upper corner
    tga->header.x_origin = 0;
    tga->header.y_origin = 0;

    // Read pixels
    tga->pixels = (pixel_t*)malloc(sizeof(pixel_t) * tga->header.width * tga->header.height);
    if (fread(tga->pixels, sizeof(pixel_t) * tga->header.width * tga->header.height, 1, file) != 1) {
        fprintf(stderr, "Unable to read pixels from TGA file\n");
        tga_destroy(tga);
        return NULL;
    }

    return tga;
}

void tga_destroy(tga_t *tga) {
    // Destroy all data from memory
    if (tga == NULL) {
        return;
    }

    if (tga->pixels != NULL) {
        free(tga->pixels);
    }
    free(tga);
}

void tga_write(tga_t* tga, FILE *file) {
    if (tga == NULL || file == NULL) {
        fprintf(stderr, "TGA or FILE is NULL");
        return;
    }
    // Read from start of file
    fseek(file, 0, SEEK_SET);
    // Write header
    if (fwrite(&tga->header, sizeof(tga_header_t), 1, file) != 1) {
        fprintf(stderr, "Unable to write to a file\n");
        return;
    }

    // Write pixels
    if (fwrite(tga->pixels, sizeof(pixel_t) * tga->header.width * tga->header.height, 1, file) != 1) {
        fprintf(stderr, "Unable to write pixels to a file\n");
        return;
    }
}

void tga_resize(tga_t *tga, const int width, const int height) {
    if (tga == NULL) {
        fprintf(stderr, "TGA is NULL\n");
        return;
    }
    // old sizes
    pixel_t *old_pixels = tga->pixels;
    const int old_width = tga->header.width;
    const int old_height = tga->header.height;
    pixel_t *new_pixels = (pixel_t *)malloc(sizeof(pixel_t) * width * height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // example: 10 / 250 = 0.04, then 0.04 * 500 = 20
            float x_percent = x / (float)width;
            float y_percent = y / (float)height;
            int x_old = x_percent * old_width;
            int y_old = y_percent * old_height;

            // Copy pixels from old size to new size
            memcpy(&new_pixels[y * width + x], &old_pixels[y_old * old_width + x_old], sizeof(pixel_t));
        }
    }
    // new sizes
    tga->pixels = new_pixels;
    tga->header.width = width;
    tga->header.height = height;
    free(old_pixels);
}

void tga_crop(tga_t *tga, int x, int y, int width, int height) {
    if (tga == NULL) {
        fprintf(stderr, "TGA is NULL\n");
        return;
    }

    pixel_t *old_pixels = tga->pixels;
    const int old_width = tga->header.width;
    pixel_t *new_pixels = (pixel_t *)malloc(sizeof(pixel_t) * width * height);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // Copy pixels from crop dimensions
            memcpy(&new_pixels[i * width + j], &old_pixels[(y + i) * old_width + x + j], sizeof(pixel_t));
        }
        
    }

    tga->pixels = new_pixels;
    tga->header.width = width;
    tga->header.height = height;
    free(old_pixels);
}

void tga_info(tga_t *tga) {
    printf("width: %d, height: %d\n", tga->header.width, tga->header.height);
}

void tga_copy(tga_t *tga, int x1, int y1, int width, int height, int x2, int y2) {
    if (tga == NULL) {
        fprintf(stderr, "TGA is NULL\n");
        return;
    }

    const int image_width = tga->header.width;
    const int image_height = tga->header.height;
    pixel_t *pixels_copy = (pixel_t *)malloc(sizeof(pixel_t) * image_width * image_height);
    // Make a copy of pixels
    memcpy(pixels_copy, tga->pixels, sizeof(pixel_t) * image_width * image_height);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // Copy pixels from x1,y1 to x2,y2
            memcpy(&tga->pixels[(y2 + i) * image_width + x2 + j], &pixels_copy[(y1 + i) * image_width + x1 + j], sizeof(pixel_t));
        }
    }

    free(pixels_copy);
}

void tga_move(tga_t *tga, int x, int y) {
    if (tga == NULL) {
        fprintf(stderr, "TGA is NULL\n");
        return;
    }

    const int width = tga->header.width;
    const int height = tga->header.height;
    pixel_t *pixels_copy = (pixel_t *)malloc(sizeof(pixel_t) * width * height);
    // Make a copy of pixels
    memcpy(pixels_copy, tga->pixels, sizeof(pixel_t) * width * height);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int to_x = 0;
            int to_y = 0;

            // Magic tric to move pixels
            if (x >= 0) {
                if (j < width - x) {
                    to_x = j + x;
                } else {
                    to_x = j + x - width;
                }
            } else {
                if (j + x >= 0) {
                    to_x = j + x;
                } else {
                    to_x = width + j + x;
                }
            }

            if (y >= 0) {
                if (i < height - y) {
                    to_y = i + y;
                } else {
                    to_y = i + y - height;
                }
            } else {
                if (i + y >= 0) {
                    to_y = i + y;
                } else {
                    to_y = height + i + y;
                }
            }

            memcpy(&tga->pixels[to_y * width + to_x], &pixels_copy[i * width + j], sizeof(pixel_t));
        }
    }

    free(pixels_copy);
}

void tga_bw(tga_t *tga) {
    if (tga == NULL) {
        fprintf(stderr, "TGA is NULL\n");
        return;
    }

    for (int i = 0; i < tga->header.width * tga->header.height; i++) {
        // Formula from here: https://en.wikipedia.org/wiki/Grayscale#Converting_color_to_grayscale
        float value = tga->pixels[i].blue * 0.0722f + tga->pixels[i].green * 0.7152f + tga->pixels[i].red * 0.2126f;
        pixel_t *pixel = &tga->pixels[i];

        pixel->red = value;
        pixel->blue = value;
        pixel->green = value;
    }
}
