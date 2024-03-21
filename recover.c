#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check if the argument count is two
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Should open up a file
    FILE *input_file = fopen(argv[1], "r");

    // Check if the input_file is valid
    if (input_file == NULL)
    {
        printf("Could not open file\n");
        return 2;
    }

    // Create a buffer to store blocks of 512 bytes
    unsigned char buffer[512];

    // // Initialize a counter to keep track of the number of images found
    int counter_image = 0;
    // // Initialize a file pointer for the retrieved images
    FILE *output_file = NULL;

    char filename[8];

    // Read the blocks
    while (fread(buffer, sizeof(char), 512, input_file))
    {
        // Check if a byte indicates the beginning of the image
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Close the previous output file (if open)
            if (output_file != NULL)
            {
                fclose(output_file);
            }

            // Create a new image filename
            sprintf(filename, "%03i.jpg", counter_image);

            // Open a new output file
            output_file = fopen(filename, "w");

            // Check if the output file was created successfully
            if (output_file == NULL)
            {
                fclose(input_file);
                fprintf(stderr, "Could not create output file\n");
                return 3;
            }

            // Increment the image counter
            counter_image++;
        }

        // Check whether the output is used for valid input
        if (output_file != NULL)
        {
            fwrite(buffer, sizeof(char), 512, output_file);
        }
    }

    // Close the last output file
    if (output_file != NULL)
    {
        fclose(output_file);
    }

    fclose(input_file);

    return 0;
}