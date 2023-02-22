#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define BLOCK_SIZE 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Open raw memory card
    FILE *fp_in = fopen(argv[1], "r");
    if (!fp_in){
        return 1;
    }

    // Configure output file
    int file_index = 0;
    char *file_name = malloc(14);
    FILE *fp_out = NULL;

    BYTE *buffer = malloc(BLOCK_SIZE);
    while (fread(buffer, 1, BLOCK_SIZE, fp_in) == BLOCK_SIZE){
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0){
            if (fp_out){
                fclose(fp_out);
            }
            file_index += 1;
            sprintf(file_name, "./jpg/%03i.jpg", file_index);
            fp_out = fopen(file_name, "w");
        }
        if (fp_out){
            fwrite(buffer, BLOCK_SIZE, 1, fp_out);
        }
    }
    if (fp_out){
        fclose(fp_out);  
    }
    fclose(fp_in);
    free(buffer);
    free(file_name);
    return 0;
}