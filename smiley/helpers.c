#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    // Change all black pixels to a color of your choosing
    //RGBTRIPLE is a struct that will let you modify specific red, blue, green scale
    //within the pixel length
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //changing the black pixels (00)     white is FF
            if (image[i][j].rgbtBlue == 0x00 && image[i][j].rgbtGreen == 0x00 && image[i][j].rgbtRed == 0x00)
            {
                image[i][j].rgbtBlue = 0x56;
                image[i][j].rgbtBlue = 0xff;
                image[i][j].rgbtBlue = 0x10;
            }
        }
    }
}
