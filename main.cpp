#include <iostream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char blank = ' ' ;
const char light = static_cast<char>(176);
const char medium = static_cast<char> (177);
const char dark = static_cast<char> (178);
const char black =  static_cast<char>(219);
const int ratio = 2; //the ratio of the height of a character to its width

std::string imageToASCII(const char* file, int w, int h)
{
    int width, height, channels;
     unsigned char* rgb = stbi_load( file, &width, &height, &channels, 0 );
     float maxDist = sqrt(channels); //the maximum distance would be the distance from when all the channels are 1 to when all the channels are 0, which would just be the sqrt of a channel*1
    //the following comments refer to 'megaPixels' which are pixels formed from the average color of widthRatio x heightRatio pixels
    int widthRatio = std::max(ceil((double)width/w),1.0); //the width of a megaPixel, which must be at least one.
    int heightRatio = std::max(ceil((double)height/h),1.0); //the height of a megaPixel
    float distance; //the distance of our megaPixel's color from white
    int* channelVals = new int[channels]; //keeps track of the values in each channel
    std::ostringstream stream;
    std::vector<float> distances;
    float minDistances = -1, maxDistances = -1;
     for (int i = 0; i < height; i+= (heightRatio)) //for each row in our image
     {
         for (int j = 0; j < width*channels; j+= channels*(widthRatio)) //for each megaPixel
         {
            //we need to find the color of our megaPixel
            int pixelWidth = std::min(width*channels - j,widthRatio); //if either dimension of the megaPixel is a decimal, the last megaPixel would have dimensions of width%w and height%h.
            int pixelHeight = std::min(height - i,heightRatio);
            for (int c = 0; c < channels; c++)
            {
                channelVals[c] = 0;
            }
             for (int h = 0; h < pixelHeight; h++) //for each row in our megaPixel
             {
                for (int g = 0; g < pixelWidth; g ++) //for each pixel in our megaPixel
                {
                    for (int c = 0; c < channels; c++) //for each channel in our pixel
                    {
                        channelVals[c] += (int)(rgb[(i+h)*width*channels + j + g + c]);
                    }
                 }
             }
            distance = 0;
            for (int c = 0; c < channels; c ++)
            {
                distance += pow(1-(channelVals[c])/(pixelWidth*pixelHeight)/255.0,2);
            }
            distance = sqrt(distance);
            distance /= maxDist;
            if (distance == 0)
            {
                for (int z = 0; z < ratio; z++)
                stream << blank;
            }
            else if (distance < .25)
            {
                for (int z = 0; z < ratio; z++)
                stream << light;
            }
            else if (distance < .5)
            {
                for (int z = 0; z < ratio; z++)
                stream << medium;
            }
            else if (distance < .75)
            {
                for (int z = 0; z < ratio; z++)
                stream << dark;
            }
            else
            {
                for (int z = 0; z < ratio; z++)
                stream << black;
            }
         }
         stream << '\n';
     }
     delete[] channelVals;
     delete rgb;
     return stream.str();
}

int main(int argc, char* argv[])
{
    std::cout << imageToASCII("illiphant.jpg", 100, 100) << std::endl;
}
