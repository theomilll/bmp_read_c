#include <stdio.h>
#include <stdlib.h>
#include "bmp_utils.h"

// This function converts a color image to grayscale.
int main(int argc, char* argv[]) 
{
  // Check to make sure that the user has provided two command-line arguments: the name of the input image file and the name of the output image file.
  if (argc != 3) {
    printf("Usage: %s <input_file.bmp> <output_file.bmp>\n", argv[0]);
    return 1;
  }

  // Read the input image file.
  // The `bmp_read()` function returns a pointer to a `BMPData` structure.
  // The `BMPData` structure contains the data for a BMP file.
  BMPData *bmp_data = NULL;
  BMPFileHeader file_header;
  BMPHeader bmp_header;

  bmp_data = bmp_read(argv[1], &file_header, &bmp_header);

  // If the `bmp_read()` function failed, print an error message and exit.
  if (bmp_data == NULL)
  {
    printf("Unable to process file\n");
    exit(1);
  }

  // Convert the image to grayscale.
  // Iterate over each pixel in the input image.
  // Calculate the average of the red, green, and blue values of the pixel.
  // Set the red, green, and blue values of the pixel to the average value.
  int i;
  int size = bmp_header.biWidth * bmp_header.biHeight;
  for (i = 0; i < size; i++) {
    unsigned char gray = (bmp_data[i].r * 0.11) + (bmp_data[i].g * 0.59) + (bmp_data[i].b * 0.3);
    bmp_data[i].r = bmp_data[i].g = bmp_data[i].b = gray;
  }

  // Write the grayscale image to the output file.
  // The `bmp_write()` function writes a BMP file to disk.
  if (bmp_write(argv[2], &file_header, &bmp_header, bmp_data) == -1) {
    printf("Writing error\n");
    free(bmp_data);
    exit(1);
  }

  // Free the memory that was allocated for the input image and the grayscale image.
  free(bmp_data);

  // Return 0 to indicate that the program has completed successfully.
  return 0;
}
