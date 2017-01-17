#include "Radiograph.hpp"
#include <iostream>

Radiograph::Radiograph(std::string filename, int subtlety, int size, int age, bool isMale, int x, int y, bool isMalignant, bool hasNodule)
{
  _filename = filename;
  _subtlety = subtlety;
  _size = size;
  _age = age;
  _isMale = isMale;
  _x = x;
  _y = y;
  _isMalignant = isMalignant;
  _hasNodule = hasNodule;
}

bool Radiograph::LoadImage(std::string directory)
{
  std::string filename = getFilename();
  std::string fullName = directory + filename;

  _image = cv::imread( fullName, cv::IMREAD_COLOR ); // Read the file
  if(_image.empty() )                      // Check for invalid input
  {
      return false;
  }
  return true; 
}

void Radiograph::DisplayImage()
{
  cv::Mat image = getImage();
  if (!image.empty())
  {
    std::string windowname = getFilename();
    cv::namedWindow(windowname, cv::WINDOW_AUTOSIZE ); // Create a window for display.
    cv::imshow(windowname, image );                // Show our image inside it.
    cv::waitKey(0); // Wait for a keystroke in the window
  }
}

void Radiograph::BoxNodule()
{
  cv::Mat image = getImage();
  int x = getX();
  int y = getY();
  if (!image.empty())
  {
    double height = _size * PIXELS_PER_MM;
    int cornerX = round(_x - (height/2));
    int cornerY = round(_y - (height/2));
    int bCornerX = round(_x + (height/2));
    int bCornerY = round(_y + (height/2));
    int h = round(height);

    cv::rectangle(image, cv::Point(cornerX, cornerY), cv::Point(bCornerX, bCornerY), cv::Scalar(0, 0, 255), 1, cv::LINE_8, 0);
    DisplayImage();
  } 
}

cv::Mat Radiograph::ExtractNodule()
{
  double height = _size * PIXELS_PER_MM;
  int cornerX = round(_x - (height/2));
  int cornerY = round(_y - (height/2));
  int h = round(height);

  cv::Rect noduleRegion(cornerX, cornerY, h, h);
  return _image(noduleRegion);
}

int main()
{
  Radiograph xray("JPCLN010.jpg", 5, 25, 60, false, 647, 594, true, true);
  bool loaded = xray.LoadImage("../data/img/");
  if (!loaded)
  {
    std::cout << "Failure loading image!\n";
    return -1;
  }
  xray.BoxNodule();
  return 0;
}