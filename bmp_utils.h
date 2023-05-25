/**
 * File bmp_utils.h
 * @date May 9th, 2023
 * 
 * Implemented according to the following spec: 
 * http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
*/

#ifndef __BMP_UTILS__
#define __BMP_UTILS__

#include <inttypes.h>

typedef struct __attribute__((packed)) {
    char bfType[2];
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t brOffbits;
} BMPFileHeader;

typedef struct __attribute__((packed)) {
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BMPHeader;

typedef struct __attribute__((packed)) {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} BMPData;

BMPData *bmp_read(char *filename, BMPFileHeader *file_header, BMPHeader *bmp_header);

int bmp_write(char *filename, BMPFileHeader *file_header, BMPHeader *bmp_header, BMPData *data);

#endif //__BMP_UTILS__
