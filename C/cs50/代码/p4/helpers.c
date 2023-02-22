#include "helpers.h"
#include <math.h>

void swap(RGBTRIPLE *img1, RGBTRIPLE *img2);
BYTE root(int x, int y);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            BYTE avg = (BYTE) round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / (float) 3);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width / 2; j++){
            swap(&image[i][j], &image[i][width - j - 1]);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            copy[i][j] = image[i][j];
        }
    }
    int count;
    int r_sum, g_sum, b_sum;
    int x, y;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            count = 0;
            r_sum = 0;
            g_sum = 0;
            b_sum = 0;
            for(int dy = -1; dy < 2; dy++){
                for(int dx = -1; dx < 2; dx++){
                    x = j + dx;
                    y = i + dy;
                    if(x >= 0 && x < width && y >= 0 && y < height){
                        count++;
                        r_sum += copy[y][x].rgbtRed;
                        g_sum += copy[y][x].rgbtGreen;
                        b_sum += copy[y][x].rgbtBlue;
                    }
                }
            }
            image[i][j].rgbtRed = (BYTE) round(r_sum / (float) count);
            image[i][j].rgbtGreen = (BYTE) round(g_sum / (float) count);
            image[i][j].rgbtBlue = (BYTE) round(b_sum / (float) count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            copy[i][j] = image[i][j];
        }
    }
    int r_sum_x, r_sum_y;
    int g_sum_x, g_sum_y;
    int b_sum_x, b_sum_y;
    int x, y;
    int gx[3][3] = {
        {-1,0,1},
        {-2,0,2},
        {-1,0,1}
    };
    int gy[3][3] = {
        {-1,-2,-1},
        {0,0,0},
        {1,2,1}
    };
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            r_sum_x = 0;
            g_sum_x = 0;
            b_sum_x = 0;
            r_sum_y = 0;
            g_sum_y = 0;
            b_sum_y = 0;
            for(int dy = -1; dy < 2; dy++){
                for(int dx = -1; dx < 2; dx++){
                    x = j + dx;
                    y = i + dy;
                    if(x >= 0 && x < width && y >= 0 && y < height){
                        r_sum_x += copy[y][x].rgbtRed * gx[dy + 1][dx + 1];
                        g_sum_x += copy[y][x].rgbtGreen * gx[dy + 1][dx + 1];
                        b_sum_x += copy[y][x].rgbtBlue * gx[dy + 1][dx + 1];
                        r_sum_y += copy[y][x].rgbtRed * gy[dy + 1][dx + 1];
                        g_sum_y += copy[y][x].rgbtGreen * gy[dy + 1][dx + 1];
                        b_sum_y += copy[y][x].rgbtBlue * gy[dy + 1][dx + 1];
                    }
                }
            }
            image[i][j].rgbtRed = root(r_sum_x, r_sum_y);
            image[i][j].rgbtGreen = root(g_sum_x, g_sum_y);
            image[i][j].rgbtBlue = root(b_sum_x, b_sum_y);
        }
    }
    return;
}

void swap(RGBTRIPLE *img1, RGBTRIPLE *img2){
    BYTE tmp = (*img1).rgbtBlue;
    (*img1).rgbtBlue = (*img2).rgbtBlue;
    (*img2).rgbtBlue = tmp;
    tmp = (*img1).rgbtRed;
    (*img1).rgbtRed = (*img2).rgbtRed;
    (*img2).rgbtRed = tmp;
    tmp = (*img1).rgbtGreen;
    (*img1).rgbtGreen = (*img2).rgbtGreen;
    (*img2).rgbtGreen = tmp;
}

BYTE root(int x, int y){
    int ans = (int) round(sqrt(x * x + y * y));
    ans = ans > 255 ? 255 : ans;
    return (BYTE) ans;
}