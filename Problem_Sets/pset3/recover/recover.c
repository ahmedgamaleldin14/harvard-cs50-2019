#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    //check if there are more or less than one argument
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    //open the file
    FILE *file = fopen(argv[1], "r");
    //check if the file is not opened
    if (file == NULL)
    {
        fprintf(stderr, "Cannot open the file\n");
        return 2;
    }

    //declaring img before assigning to prevent any errors while compiling
    FILE *img = NULL;
    int counter = 0;
    //pre-define a memory location for the filename and the buffer
    char *filename = malloc(7);
    unsigned char *buffer = malloc(512);

    //keep reading till reaching the end of the file (EOF is smaller than 512 bytes)
    while(fread(buffer, 512, 1, file) == 1)
    {
        //if encounter the start of a JPEG, start reading.
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //there is not image file to close for the first JPEG file.
            if (counter >= 1)
            {
                fclose(img);
            }

            //create a new file and start with 000.jpg
            sprintf(filename, "%03i.jpg", counter);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                fprintf(stderr, "Cannot open the image\n");
                return 2;
            }

            counter++;
        }

        if (counter >= 1)
        {
            fwrite(buffer, 512, 1, img);
        }
    }

    fclose(file);

    //free the stored memory locations
    free(filename);
    free(buffer);

    return 0;
}
