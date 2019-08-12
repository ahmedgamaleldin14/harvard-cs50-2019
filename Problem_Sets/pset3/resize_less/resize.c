// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bmp.h"

//make the data type LONG instead of int to be consistent with bmp file header
typedef int32_t LONG;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames and number of resizing
    LONG n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, bf_new;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    bf_new = bf;

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, bi_new;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    bi_new = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // determine padding for scanlines
    LONG input_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // write outfile's BITMAPINFOHEADER and outfile's BITMAPFILEHEADER
    // write them in order (bf then bi) to keep the file format
    bi_new.biWidth *= n;
    bi_new.biHeight *= n;
    LONG output_padding = (4 - (bi_new.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi_new.biSizeImage = ((bi_new.biWidth * 3) + output_padding) * abs(bi_new.biHeight);
    bf_new.bfSize = bi_new.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    fwrite(&bf_new, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi_new, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (LONG i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over scanlines again for vertical recopy
        for (LONG l = 0; l < n; l++)
        {
            // iterate over pixels in scanline
            for (LONG j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile n times
                for (LONG k = 0; k < n; k++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // add output padding calculated from the formula above
            for (LONG k = 0; k < output_padding; k++)
            {
                fputc(0x00, outptr);
            }

            // go back to the start of the scanline for vertical re-copying
            if (l < n - 1)
                fseek(inptr, -(bi.biWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
        }

        // skip over padding, if any
        fseek(inptr, input_padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
