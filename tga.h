#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t id_length;
    uint8_t color_map_type;
    uint8_t image_type;
    uint8_t color_map[5];
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t depth;
    uint8_t descriptor;
} tga_header_t;

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} pixel_t;

typedef struct {
    tga_header_t header;
    pixel_t *pixels;
} tga_t;

tga_t *tga_read(FILE *file);
void tga_write(tga_t* tga, FILE *file);
void tga_destroy(tga_t *tga);
void tga_resize(tga_t *tga, int width, int height);
void tga_crop(tga_t *tga, int x, int y, int width, int height);
void tga_info(tga_t *tga);
void tga_copy(tga_t *tga, int x1, int y1, int width, int height, int x2, int y2);
void tga_move(tga_t *tga, int x, int y);
void tga_bw(tga_t *tga);
