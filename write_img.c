#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "bmp.h"

int main(int argc, char **argv)
{
    struct Image img1, img2, img3;
    int height, width, hres, vres;

    /* Writing img1.bmp */
    height = 2;
    width = 2;
    hres = 300;
    vres = 300;

    FILE *img_file_ptr;
    img_file_ptr = fopen("img1.bmp", "wb");

    fill_img_headers(&img1, width, height, hres, vres);
    disp_img_headers(&img1);
    write_img_headers_to_file(&img1, img_file_ptr);
    disp_pixel_data(&img1);

    set_pixel(&img1, 0, 0, 0, 0, 0xff);
    set_pixel(&img1, 0, 1, 0xff, 0, 0);
    set_pixel(&img1, 1, 0, 0, 0xff, 0);
    set_pixel(&img1, 1, 1, 0xff, 0xff, 0xff);

    disp_pixel_data(&img1);
    write_pixels_to_file(&img1, img_file_ptr);
    fclose(img_file_ptr);

    /* Writing img2.bmp */
    height = 1;
    width = 3;
    hres = 300;
    vres = 300;

    FILE *fptr;
    fptr = fopen("img2.bmp", "wb");
    fill_img_headers(&img2, width, height, hres, vres);
    set_pixel(&img2, 0, 0, 0x00, 0x00, 0xff);
    set_pixel(&img2, 1, 0, 0x00, 0xff, 0x00);
    set_pixel(&img2, 2, 0, 0xff, 0x00, 0x00);
    disp_img_headers(&img2);
    disp_pixel_data(&img2);
    write_img_headers_to_file(&img2, fptr);
    write_pixels_to_file(&img2, fptr);
    fclose(fptr);

    /* Writing img3.bmp */
    height = 3;
    width = 3;
    hres = 300;
    vres = 300;

    fptr = fopen("img3.bmp", "wb");
    fill_img_headers(&img3, width, height, hres, vres);
    set_pixel(&img3, 0, 0, 0x00, 0x00, 0xff);
    set_pixel(&img3, 1, 0, 0x00, 0xff, 0x00);
    set_pixel(&img3, 2, 0, 0xff, 0x00, 0x00);
    set_pixel(&img3, 0, 1, 0x00, 0x00, 0x00);
    set_pixel(&img3, 1, 1, 0xff, 0xff, 0xff);
    set_pixel(&img3, 2, 1, 0x00, 0x00, 0x00);
    set_pixel(&img3, 0, 2, 0xff, 0x00, 0x00);
    set_pixel(&img3, 1, 2, 0x00, 0xff, 0x00);
    set_pixel(&img3, 2, 2, 0x00, 0x00, 0xff);
    write_img_headers_to_file(&img3, fptr);
    write_pixels_to_file(&img3, fptr);
    fseek(fptr, img3.bmp.offset, SEEK_SET);
    write_pixels_to_file(&img3, fptr);
    fclose(fptr);

    /* Writing img4.bmp */
    struct Image img4;
    uint8_t r, b, g;
    const float deg_to_rad = 60;
    height = 640;
    width = 640;
    hres = 300;
    vres = 300;
    fptr = fopen("img4.bmp", "wb");
    fill_img_headers(&img4, width, height, hres, vres);
    write_img_headers_to_file(&img4, fptr);
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            r = (uint8_t) ceil(255 * fabs(sin(x * deg_to_rad)));
            b = (uint8_t) ceil(255 * fabs(cos(x * deg_to_rad)));
            g = (uint8_t) ceil(255 * fabs(tan(x * deg_to_rad)));
            //printf("%f\n", abs(sin(x)));
            set_pixel(&img4, x, y, r, 0x00, 0x00);
        }
    }
    write_pixels_to_file(&img4, fptr);
    fclose(fptr);
    
    /* Writing img5.bmp */
    struct Image img5;
    height = 640;
    width = 640;
    hres = 300;
    vres = 300;
    fptr = fopen("img5.bmp", "wb");
    fill_img_headers(&img5, width, height, hres, vres);
    write_img_headers_to_file(&img5, fptr);
    for (float x = 0; x < width; x += 0.1) {
        float y = ceil(height * fabs(sin(x * deg_to_rad)));
        set_pixel(&img5, y, x, 0x0f, 0xff, 0x0f);
    }
    write_pixels_to_file(&img5, fptr);
    fclose(fptr);

    return 0;
}