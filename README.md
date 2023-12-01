# Image Compression Tool

This is a simple image compression tool implemented in C. It utilizes a quadtree structure to represent and compress images.

# Introduction
The image compression tool reads an input image file, applies compression using a quadtree structure, and produces a compressed output file. The compression can be configured with a factor to control the trade-off between image quality and file size.

# File Formats
### Input Image
The input image is expected to be in the P6 format. Each pixel is represented by three unsigned characters (red, green, blue), and the image size and maximum color value are specified in the header.

### Output Image
The output image is also in the P6 format. The header contains the image size and maximum color value. The pixel data follows the header.

# Using the Project
### Compression
To compress an image, use the following command:
```
./compress -c1/-c2 factor input_file output_file
```
- c1: Perform compression with level statistics output.
- c2: Perform compression with quadtree structure output.
- factor: A compression factor to control the trade-off between image quality and file size.
- input_file: The path to the input image file.
- output_file: The path to the output compressed file.

### Decompression
To decompress an image, use the following command:

```
./compress -d input_file output_file
```

- d: Decompress the input file.
- input_file: The path to the compressed input file.
- output_file: The path to the decompressed output image file.

# Build

To build the image compression tool, use the provided Makefile:

```
make
```
This will generate an executable named quadtree.