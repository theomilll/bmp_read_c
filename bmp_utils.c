#include <stdio.h>
#include <stdlib.h>
#include "bmp_utils.h"

void bmp_print_file_header(BMPFileHeader *file_header)
{
    printf("--------------------------------------------\n");
    printf("BMP File Header\n");
    printf("--------------------------------------------\n");
    if (file_header == NULL) {
        printf("NULL Header\n");
        return;
    }

    printf("bfType: %c%c\n", file_header->bfType[0], file_header->bfType[1]);
    printf("bfSize: %u\n", file_header->bfSize);
    printf("bfReserved1: %u\n", file_header->bfReserved1);
    printf("bfReserved2: %u\n", file_header->bfReserved2);
    printf("brOffbits: %u\n", file_header->brOffbits);
    printf("--------------------------------------------\n");
}

void bmp_print_header(BMPHeader *bmp_header)
{
    printf("--------------------------------------------\n");
    printf("BMP Header\n");
    printf("--------------------------------------------\n");
    if (bmp_header == NULL) {
        printf("NULL Header\n");
        return;
    }

    printf("biSize: %u\n", bmp_header->biSize);
    printf("biWidth: %u\n", bmp_header->biWidth);
    printf("biHeight: %u\n", bmp_header->biHeight);
    printf("biPlanes: %u\n", bmp_header->biPlanes);
    printf("biBitCount: %u\n", bmp_header->biBitCount);
    printf("biCompression: %u\n", bmp_header->biCompression);
    printf("biSizeImage: %u\n", bmp_header->biSizeImage);
    printf("biXPelsPerMeter: %u\n", bmp_header->biXPelsPerMeter);
    printf("biYPelsPerMeter: %u\n", bmp_header->biYPelsPerMeter);
    printf("biClrUsed: %u\n", bmp_header->biClrUsed);
    printf("biClrImportant: %u\n", bmp_header->biClrImportant);
    printf("--------------------------------------------\n");
}

BMPData *bmp_read(char *filename, BMPFileHeader *file_header, BMPHeader *bmp_header)
{
    FILE *fp;
    BMPData *bmp_data = NULL;
    unsigned int bmp_size = 0;

    // Verify function parameters
    if (filename == NULL || file_header == NULL || bmp_header == NULL) {
        printf("ERROR: Invalid function parameters.\n");
        printf("Filename: %s\n", filename);
        printf("File header addr: %p\n", file_header);
        printf("BMP header addr: %p\n", bmp_header);
        return bmp_data;
    }

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("ERROR: Unable to open file for reading: %s\n", filename);
        return bmp_data;
    }

    fread(file_header, sizeof(BMPFileHeader), 1, fp);
    bmp_print_file_header(file_header);

    if (file_header->bfType[0] != 'B' && file_header->bfType[0] != 'M') {
        printf("ERROR: %s is not a BMP file\n", filename);
        fclose(fp);
        return bmp_data;
    }

    printf("Successfully opened %s with %u bytes\n", filename, file_header->bfSize);

    fread(bmp_header, sizeof(BMPHeader), 1, fp);
    bmp_print_header(bmp_header);

    if (bmp_header->biSize < 40 || bmp_header->biPlanes != 1 || bmp_header->biCompression != 0) {
        printf("ERROR: Invalid bitmap header.\n");
        fclose(fp);
        return bmp_data;
    }

    printf("Bitmap with %u x %u pixels\nand %u bits per pixel\n",
           bmp_header->biWidth, bmp_header->biHeight, bmp_header->biBitCount);

    bmp_size = bmp_header->biWidth * bmp_header->biHeight;
    bmp_data = (BMPData *) malloc(bmp_size * sizeof(BMPData));
    if (bmp_data == NULL) {
        printf("ERROR: Unable to allocate memory for bitmap data\n");
        fclose(fp);
        return bmp_data;
    }

    unsigned int read_elements = fread(bmp_data, sizeof(BMPData), bmp_size, fp);
    if (read_elements != bmp_size) {
        printf("ERROR reading BMP data(%u). Expected %u.\n", read_elements, bmp_size);
        free(bmp_data);
        bmp_data = NULL;
        fclose(fp);
        return bmp_data;
    }

    printf("Successfully loaded %u pixels from bitmap file.\n", read_elements);

    fclose(fp);
    return bmp_data;
}

int bmp_write(char *filename, BMPFileHeader *file_header, BMPHeader *bmp_header, BMPData *data)
{
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Error opening the file for writing.\n");
        return -1;
    }

    fwrite(file_header, sizeof(BMPFileHeader), 1, fp);
    fwrite(bmp_header, sizeof(BMPHeader), 1, fp);

    unsigned int size = bmp_header->biWidth * bmp_header->biHeight;
    fwrite(data, sizeof(BMPData), size, fp);

    fclose(fp);
    return 0;
}
