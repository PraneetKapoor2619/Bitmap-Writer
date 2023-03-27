#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "bmp.h"

inline void trib(uint24_t *u24ptr, uint8_t b3, uint8_t b2, uint8_t b1)
{
    int value;
    value = ((b3 << 16) | (b2 << 8) | (b1 << 0));
    memcpy(u24ptr, &value, 3);
}

void fill_bmp_header(struct BMP_HEADER *bmph_ptr, struct DIB_HEADER *dibh_ptr)
{
    bmph_ptr->identifier = 0x424d;
    bmph_ptr->offset = 2 + 4 + 2 + 2 + 4;
    bmph_ptr->offset += (2 * 2) + (4 * 9);
    bmph_ptr->size = bmph_ptr->offset + dibh_ptr->raw_img_size;
    bmph_ptr->word1 = 0x00;
    bmph_ptr->word2 = 0x00;
}

void fill_dib_header(struct DIB_HEADER *dibh_ptr, uint32_t width, 
                        uint32_t height, uint32_t horizontal_resolution, 
                        uint32_t vertical_resolution)
{
    int bits_per_pixel, colors_in_palette;
    uint16_t padding;
    bits_per_pixel = 24;            /* Most used bpp */
    colors_in_palette = 0;          /* all colors in the palette */

    padding = 0;
    while (padding < 4) {
        if ((padding + (3 * width)) % 4 == 0)
            break;
        ++padding;
    }
    
    dibh_ptr->DIB_HEADER_length = (2 * 2) + (4 * 9);
    dibh_ptr->width = width;
    dibh_ptr->height = height;
    dibh_ptr->color_planes = 0x01;
    dibh_ptr->bits_per_pixel = bits_per_pixel;
    dibh_ptr->compression_method = 0x00; /* No compression */

    /* Compute the size of raw image */
    dibh_ptr->raw_img_size = (width * height) * bits_per_pixel / 8;
    dibh_ptr->raw_img_size += (padding * height);
    
    /* X dots per inch, then no. of dots per meter = X / (1/39) = X * 39 */
    dibh_ptr->horizontal_resolution = horizontal_resolution * 39;
    dibh_ptr->vertical_resolution = vertical_resolution * 39;
    dibh_ptr->colors_in_palette = colors_in_palette;
    dibh_ptr->no_imp_colors = 0x00;
}

void fill_img_headers(struct Image *img_ptr, uint32_t width, uint32_t height,
                        uint32_t hres, uint32_t vres)
{
    uint16_t padding;
    int i;
    fill_dib_header(&(img_ptr->dib), width, height, hres, vres);
    fill_bmp_header(&(img_ptr->bmp), &(img_ptr->dib));
    padding = 0;
    while (padding < 4) {
        if ((padding + (3 * width)) % 4 == 0)
            break;
        ++padding;
    }
    img_ptr->padding = padding;
    img_ptr->pixel_data = (uint8_t *) 
                        calloc(img_ptr->dib.raw_img_size, sizeof(uint8_t));
    if (img_ptr->pixel_data != NULL) {
        fprintf(stdout, "Initialized image\n"
                        "Raw image data size: ");
        for (i = 0; i < img_ptr->dib.raw_img_size; ++i);
        printf("%d bytes\n", i);
    }
    else {
        fprintf(stdout, "Failed to initialize. Exiting...\n");
        exit(1);
    }
}

void set_pixel(struct Image *img_ptr, uint32_t x, uint32_t y, 
                        uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t address = (img_ptr->dib.height - y - 1);
    address *= (3 * img_ptr->dib.width + img_ptr->padding);
    address += (3 * x);
    img_ptr->pixel_data[address + 0] = b;
    img_ptr->pixel_data[address + 1] = g;
    img_ptr->pixel_data[address + 2] = r;
}

void disp_bmp_header(struct BMP_HEADER *bmph_ptr)
{
    fprintf(stdout, "\nBMP HEADER\n==========\n");
    fprintf(stdout, "Identifier: %x\nFile Size: %d\nWord1: %x\nWord2: %x"
                    "\nOffset: %d\n\n", 
                    bmph_ptr->identifier, bmph_ptr->size,
                    bmph_ptr->word1, bmph_ptr->word2, bmph_ptr->offset);
}

void disp_dib_header(struct DIB_HEADER *dibh_ptr)
{
    fprintf(stdout, "\nDIB HEADER\n==========\n");
    fprintf(stdout, "DIB header length: %d\nWidth: %d"
                     "\nHeight: %d"
                     "\nColor planes: %d"
                     "\nBPP: %d\nCompression method: %d"
                     "\nRaw image size: %d\nHorizontal resolution: %d"
                     "\nVertical resolution: %d"
                     "\nColors in the palette: %d"
                     "\nNumber of important colors: %d\n\n",
                     dibh_ptr->DIB_HEADER_length, dibh_ptr->width,
                     dibh_ptr->height, 
                     dibh_ptr->color_planes, dibh_ptr->bits_per_pixel, 
                     dibh_ptr->compression_method,
                     dibh_ptr->raw_img_size, dibh_ptr->horizontal_resolution,
                     dibh_ptr->vertical_resolution,
                     dibh_ptr->colors_in_palette, dibh_ptr->no_imp_colors);
}

void disp_img_headers(struct Image *img_ptr)
{
    disp_bmp_header(&(img_ptr->bmp));
    disp_dib_header(&(img_ptr->dib));
}

void disp_pixel_data(struct Image *img_ptr)
{
    int i, count, y;
    count = 0;
    y = img_ptr->dib.width - 1;
    fprintf(stdout, "\nBase address: %06x\n", img_ptr->pixel_data);
    for (i = 0; i < img_ptr->dib.raw_img_size; ++i) {
        if (count == ((3 * img_ptr->dib.width) + img_ptr->padding)) {
            fprintf(stdout, "\n");
            count = 0;
        }
        if (count == 0) {
            fprintf(stdout, "[y = %d] %02x   ", y--, i);
        }
        fprintf(stdout, "%02x ", img_ptr->pixel_data[i]);
        ++count;
    }
    fprintf(stdout, "\n");
}

int lendian_fwrite(void *ptr, size_t size, FILE *fptr)
{
    int i, count, shift;
    uint8_t byte;
    i = size;
    count = 0;
    for (; i > 0; --i) {
        shift = 8 * (i - 1);
        if (size == 1)
            byte = (*(uint8_t *) ptr) >> shift;
        else if (size == 2)
            byte = (*(uint16_t *) ptr) >> shift;
        else if (size == 4)
            byte = (*(uint32_t *) ptr) >> shift;
        else if (size == 8)
            byte = (*(uint64_t *) ptr) >> shift;
        fwrite(&byte, sizeof(byte), 1, fptr);
        ++count;
    }
    return count;
}

void write_img_headers_to_file(struct Image *img_ptr, FILE *fptr)
{
    lendian_fwrite(&(img_ptr->bmp.identifier), sizeof(img_ptr->bmp.identifier), 
                    fptr);
    fwrite(&(img_ptr->bmp.size), sizeof(img_ptr->bmp.size), 1, fptr);
    fwrite(&(img_ptr->bmp.word1), sizeof(img_ptr->bmp.word1), 1, fptr);
    fwrite(&(img_ptr->bmp.word2), sizeof(img_ptr->bmp.word2), 1, fptr);
    fwrite(&(img_ptr->bmp.offset), sizeof(img_ptr->bmp.offset), 1, fptr);

    fwrite(&(img_ptr->dib.DIB_HEADER_length), 
                sizeof(img_ptr->dib.DIB_HEADER_length), 1, fptr);
    fwrite(&(img_ptr->dib.width), sizeof(img_ptr->dib.width), 1, fptr);
    fwrite(&(img_ptr->dib.height), sizeof(img_ptr->dib.height), 1, fptr);
    fwrite(&(img_ptr->dib.color_planes), 
                sizeof(img_ptr->dib.color_planes), 1, fptr);
    fwrite(&(img_ptr->dib.bits_per_pixel), 
                sizeof(img_ptr->dib.bits_per_pixel), 1, fptr);
    fwrite(&(img_ptr->dib.compression_method), 
                sizeof(img_ptr->dib.compression_method), 1, fptr);
    fwrite(&(img_ptr->dib.raw_img_size), 
                sizeof(img_ptr->dib.raw_img_size), 1, fptr);
    fwrite(&(img_ptr->dib.horizontal_resolution), 
                sizeof(img_ptr->dib.horizontal_resolution), 1, fptr);
    fwrite(&(img_ptr->dib.vertical_resolution), 
                sizeof(img_ptr->dib.vertical_resolution), 1, fptr);
    fwrite(&(img_ptr->dib.colors_in_palette), 
                sizeof(img_ptr->dib.colors_in_palette), 1, fptr);
    fwrite(&(img_ptr->dib.no_imp_colors), 
                sizeof(img_ptr->dib.no_imp_colors), 1, fptr);
}

void write_pixels_to_file(struct Image *img_ptr, FILE *fptr)
{
    int i;
    for (i = 0; i < img_ptr->dib.raw_img_size; ++i) {
        fwrite(&(img_ptr->pixel_data[i]), sizeof(uint8_t), 1, fptr);
    }
}