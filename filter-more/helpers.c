#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //find the average of the pixel rbg values and set all three rbg to that value
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //getting colors at current location
            BYTE OGBlue = image[i][j].rgbtBlue;
            BYTE OGGreen = image[i][j].rgbtGreen;
            BYTE OGRed = image[i][j].rgbtRed;

            // Calculate the average of RGB values
            BYTE average = round(((float)OGBlue + OGGreen + OGRed) / 3);

            // Set all RGB to the average value
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //we dont want to change the height of the pixel just where its located in a width?
    //go through each pixel
    for (int i = 0; i < height; i++)
    {
        //half because we dont want override by going all the way to the end
        for (int j = 0; j < width / 2; j++)
        {
            //pixel at current location
            BYTE OGBlue = image[i][j].rgbtBlue;
            BYTE OGGreen = image[i][j].rgbtGreen;
            BYTE OGRed = image[i][j].rgbtRed;
            //we need to swap the pixel at a different j location
            // Calculate the corresponding pixel index for reflection
            int reflectedIndex = width - 1 - j;

            // Swap the original pixel with the reflected pixel
            image[i][j].rgbtBlue = image[i][reflectedIndex].rgbtBlue;
            image[i][j].rgbtGreen = image[i][reflectedIndex].rgbtGreen;
            image[i][j].rgbtRed = image[i][reflectedIndex].rgbtRed;

            // Assign the original pixel values to the reflected pixel
            image[i][reflectedIndex].rgbtBlue = OGBlue;
            image[i][reflectedIndex].rgbtGreen = OGGreen;
            image[i][reflectedIndex].rgbtRed = OGRed;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //placeholder while we calculate
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //in a 3 by 3 context? take all colors from this pixel and the surrounding pixels average the values and make it this pixel
            // (n-1,m-1)  (n,m-1)  (n+1,m-1)
            // (n-1,m)    (n,m)    (n+1,m)
            // (n-1,m+1)  (n,m+1)  (n+1, m+1)
            int pixelCount = 0;
            float sumPixelB = 0;
            float sumPixelG = 0;
            float sumPixelR = 0;
            //current pixel values
            // BYTE OGBlue = image[i][j].rgbtBlue;
            // BYTE OGGreen = image[i][j].rgbtGreen;
            // BYTE OGRed = image[i][j].rgbtRed;

            //scenarios no top no bottom no left and no right
            //top left
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    // Calculate the coordinates of the surrounding pixel
                    int ni = i + di;
                    int nj = j + dj;

                    // Check if the surrounding pixel is within the image bounds
                    //this was an actual pain to code omg
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                        // Accumulate the RGB values
                    {
                        sumPixelB += image[ni][nj].rgbtBlue;
                        sumPixelG += image[ni][nj].rgbtGreen;
                        sumPixelR += image[ni][nj].rgbtRed;
                        pixelCount++;
                    }

                }
            }
            temp[i][j].rgbtBlue = round(sumPixelB / pixelCount);
            temp[i][j].rgbtGreen = round(sumPixelG / pixelCount);
            temp[i][j].rgbtRed = round(sumPixelR / pixelCount);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    //matrix multiplcation? of gx anf gy
    //similar way of obtaining 3 by 3 pixels as last prompt but now multiplying
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int GxRed = 0;
            int GxGreen = 0;
            int GxBlue = 0;
            int GyRed = 0;
            int GyGreen = 0;
            int GyBlue = 0;
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    // Calculate the coordinates of the surrounding pixel
                    int ni = i + di;
                    int nj = j + dj;

                    // Check if the surrounding pixel is within the image bounds
                    //this was an actual pain to code omg
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                        // Accumulate the RGB values
                    {
                        //we’ll take the original red values for the 9 pixels that form a 3x3 box around the pixel, multiply them each by the corresponding value in the Gx kernel, and take the sum of the resulting values.
                        GxRed += image[ni][nj].rgbtRed * Gx[di + 1][dj + 1];//lowest 00 which would stay in bounds
                        GxGreen += image[ni][nj].rgbtGreen * Gx[di + 1][dj + 1];
                        GxBlue += image[ni][nj].rgbtBlue * Gx[di + 1][dj + 1];

                        //GY
                        GyRed += image[ni][nj].rgbtRed * Gy[di + 1][dj + 1];
                        GyGreen += image[ni][nj].rgbtGreen * Gy[di + 1][dj + 1];
                        GyBlue += image[ni][nj].rgbtBlue * Gy[di + 1][dj + 1];


                    }

                }
            }
            //algo calculation
            int comboRed = round(sqrt((double)(GxRed * GxRed + GyRed * GyRed)));//sqrt gx^2 + gy^2
            int comboGreen = round(sqrt((double)(GxGreen * GxGreen + GyGreen * GyGreen)));
            int comboBlue = round(sqrt((double)(GxBlue * GxBlue + GyBlue * GyBlue)));

            //MUST CAP AT 255
            //must have temp while the rest is trying ti finish being modified
            temp[i][j].rgbtRed = (comboRed > 255) ? 255 : comboRed;
            temp[i][j].rgbtGreen = (comboGreen > 255) ? 255 : comboGreen;
            temp[i][j].rgbtBlue = (comboBlue > 255) ? 255 : comboBlue;

        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }
    return;
}
