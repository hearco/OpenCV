// Include OpenCV libraries
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>

// Include some system and other libraries
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;
using namespace cv;

void RGB2YIQ(const Mat &sourceImage, Mat &destinationImage);

int main(int argc,char* argv[])
{
  Mat input, createdYIQ;
  
  // Initialize camera
  VideoCapture cap(0);
  cap >> input;
  
  while(!waitKey(0))
  {
    cap >> input;
    RGB2YIQ(input, createdYIQ);
    imshow("YIQ", createdYIQ);
  }
  
  return 0;
}

void RGB2YIQ(const Mat &sourceImage, Mat &destinationImage)
{
    float R_1, G_1, B_1;
    float Y, I, Q;

    for (int y = 0; y < sourceImage.rows; ++y)
        for (int x = 0; x < sourceImage.cols; ++x)
        {

          // Scale to values between 0 and 1
          B_1 = (float)sourceImage.at<Vec3b>(y,x)[0] / 255.0f;
          G_1 = (float)sourceImage.at<Vec3b>(y,x)[1] / 255.0f;
          R_1 = (float)sourceImage.at<Vec3b>(y,x)[2] / 255.0f;

          // Calculate Y, I and Q values
          Y = 0.299    * R_1 + 0.587    * G_1 + 0.114    * B_1;
          I = 0.595716 * R_1 - 0.274453 * G_1 - 0.321263 * B_1;
          Q = 0.211456 * R_1 - 0.522591 * G_1 + 0.311135 * B_1;

          // Restore values to 0-255 range
          // "Y" goes from 0 to 1, so it only has to be multiplied by 255
          Y = Y * 255;

          // "I" goes from -0.5957 to 0.5957, so first move to a range from 0 to 1.1914 and then adjust it
          // to a 0-255 range
          I = (I + 0.5957) * (255 / 1.1914);

          // "Q" goes from -0.5226 a 0.5226, so first move to a range from 0 to 1.0452 and then adjust it
          // to a 0-255 range
          Q = (Q + 0.5226) * (255 / 1.0452);

          // Store values at output image
          //Y
          destinationImage.at<Vec3b>(y,x)[0] = Y;
          //I
          destinationImage.at<Vec3b>(y,x)[1] = I;
          //Q
          destinationImage.at<Vec3b>(y,x)[2] = Q;
        }
}
