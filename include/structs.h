#pragma once
#include <stdint.h>
#include <jpeglib.h>

typedef struct Image
{
    JSAMPLE *image_buffer; /*Points to large array of R,G,B-order data*/
    uint32_t height; /*Rows*/
    uint32_t width; /*Columns*/
    uint32_t color_channel; /*"component" values*/
} Image;