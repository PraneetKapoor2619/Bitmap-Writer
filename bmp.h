#ifndef __BMP_H__
#define __BMP_H__

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct { uint8_t byte[3]; } uint24_t;
#pragma pack(pop)

struct BMP_HEADER {
    uint16_t identifier;
    uint32_t size;
    uint16_t word1;
    uint16_t word2;
    uint32_t offset;
};

struct DIB_HEADER {
    uint32_t DIB_HEADER_length;
    uint32_t width;
    uint32_t height;
    uint16_t color_planes;
    uint16_t bits_per_pixel;
    uint32_t compression_method;
    uint32_t raw_img_size;
    int32_t horizontal_resolution;
    int32_t vertical_resolution;
    uint32_t colors_in_palette;
    uint32_t no_imp_colors;
};

struct Image {
    struct BMP_HEADER bmp;
    struct DIB_HEADER dib;
    uint8_t *pixel_data;
    uint8_t padding;
};

void trib(uint24_t *, uint8_t, uint8_t, uint8_t);
int lendian_fwrite(void *, size_t, FILE *);

void fill_bmp_header(struct BMP_HEADER *, struct DIB_HEADER *);
void fill_dib_header(struct DIB_HEADER *, uint32_t, uint32_t,
                        uint32_t, uint32_t);
void fill_img_headers(struct Image *, uint32_t, uint32_t, uint32_t, uint32_t);
void set_pixel(struct Image *, uint32_t, uint32_t, uint8_t, uint8_t, uint8_t);

void disp_bmp_header(struct BMP_HEADER *);
void disp_dib_header(struct DIB_HEADER *);
void disp_img_headers(struct Image *);
void disp_pixel_data(struct Image *);

void write_img_headers_to_file(struct Image *, FILE *);
void write_pixels_to_file(struct Image *, FILE *);

#endif