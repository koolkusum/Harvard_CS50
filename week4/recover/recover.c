#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //filename was not inputted
    if (argc < 2)
    {
        printf("Improper Usage.\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    FILE *img = NULL;
    //checking if file exists
    if (file == NULL)
    {
        printf("No such file found.\n");
        return 1;
    }

    //first three of a jpeg is 0xff 0xd8 0xff    buffer[0]
    //buffer[3]&0xf0==0xe0
    //last is 0xe(1-f)
    //read 512 chunks fread shud either return 512 or 0
    //000.jpg ---> sprintf(filename, "03i.jpg", 2)//inserts 2  into i
    //file *img =fopen(filename, "w")
    //fread adn fwrite
    BYTE buffer[512];
    int jpgCount = 0;
    char filename[8]; //7 + null terminator

    //reading and checking each iteration
    //defining size of byte 512 to be one block size
    //check each 512 block if it a picture (it wont start in a middle of a block)
    //close old file and write to new one
    while (fread(buffer, 1, sizeof(BYTE) * 512, file) == sizeof(BYTE) * 512)
    {
        //if it is a jpeg beginning
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //closing current file and creating a new one to write in
            if (img != NULL)
            {
                fclose(img);
            }
            //starting at 0
            sprintf(filename, "%03d.jpg", jpgCount);
            jpgCount++;
            img = fopen(filename, "w");

        }

        if (img != NULL)
        {
            fwrite(buffer, 1, sizeof(BYTE) * 512, img);
        }
    }
    if (img != NULL)
    {
        fclose(img);
    }
    //the end of the loop we must secure memory by closing the file
    fclose(file);
    return 0;
}