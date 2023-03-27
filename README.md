BMP Image Writing Library
=========================
Here is a simple C code using which one can write a BMP image file. This library
is made up by [bmp.h](bmp.h) and [bmp.c](bmp.c). [bmp.h](bmp.h) contains the
defintions of the following structures:
1. BMP_HEADER: Contains the members which make up a typical BMP file header
   (BITMAPFILEHEADER in this [figure](BMPfileFormat.svg)).
2. DIB_HEADER: Contains members which make up the **D**evice **I**ndependent
   **B**itmap (DIB) header. There are many DIB header formats for different
   purposes. BITMAPINFOHEADER is used in this library because of its simplicity
   and widespread usage.
3. Image: Contains variable `bmp` and `dib` of type `struct BMP_HEADER` and
   `struct DIB_HEADER`, respectively. Along with these, an unsigned 8-bit
   integer pointer, `pixel_data`, and a 16-bit unsigned integer, `padding`, are
   also members of `Image`.  
   `padding` contains the number of null bytes which
   need to be appended to each row of raw pixel data to make the length of the
   row divisible by 4. For example, if the width of the image is 3 pixels, then
   the value of `padding` will be 3. This implies that with 3 bits per pixel, we
   need a padding of 3 bytes to make the row divisible by 4.  
   `pixel_data` is a pointer to an array of bytes. Most of these bytes store the
   value of RBG color components of every pixel in an image, whereas some of
   these bytes are just for padding the row to make it a legitimate BMP file
   row.  
  
To use this library, you need to compile `bmp.c` using a compiler supporting C99
and above (You cannot use C89 because of the `inline` keyword used in `trib`
function's definition in [bmp.c](bmp.c)). In you main project file, you need to
include `bmp.h` as is shown below (make sure the the path to the header is
correctly mentioned while including it and during compilation):
```C
#include "bmp.h"
```

To write an image, you need the following things:
1. A file pointer pointing to a file opened in binary-write (`wb`) mode.
2. Height and width of your image.
3. Horizontal and vertical resolution of your image.

You have to then fill and write image headers to file using `fill_img_headers()`
and `write_img_headers_to_fil()`, respectively.  
Once the headers have been written, you can now set the value of individual
pixels of you image using `set_pixel()`, and finally write them in one shot
using `write_pixels_to_file()`. If you wish to write all the pixels in the image
again, use `fseek()` as is shown while writing `img3.bmp` in
[write_img.c](write_img.c).  
[write_img.c](write_img.c) contains 5 examples, which I feel sufficiently informs
the user of the application and scope of this library. Example 4 and 5 produce
patterns which somewhat resemble the interference pattern in a double slit
experiment and the patterns produced on a CRO, respectively.