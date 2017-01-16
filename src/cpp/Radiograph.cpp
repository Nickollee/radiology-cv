#include "Radiograph.hpp"

Radiograph::Radiograph(std::string filename, int subtlety, int size, int age, bool isMale, int x, int y, bool isMalignant, bool hasNodule)
{
  _filename = filename
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
  std::string filename = Radiograph.getFilename();
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
  cv::Mat image = Radiograph.getImage();
  if (image != NULL && !image.empty())
  {
    std::string windowname = Radiograph.getFilename()
    cv::namedWindow(windowname, WINDOW_AUTOSIZE ); // Create a window for display.
    cv::imshow(windowname, image );                // Show our image inside it.
    cv::waitKey(0); // Wait for a keystroke in the window
  }
}

void Radiograph::CircleNodule()
{
  cv::Mat image = Radiograph.getImage();
  int x = Radiograph.getX();
  int y = Radiograph.getY();
  if (image != NULL && !image.empty())
  {
    cv::circle(image, cv::Point(x, y), 2048/32, cv::Scalar(0, 0, 255), 1, cv::LINE_8);
    DisplayImage();
  }   
}

int main()
{
  Radiograph xray("JPCLN010.jpg", 5, 25, 60, false, 647, 594, true, true);
  xray.LoadImage("../../data/img/");
  xray.CircleNodule();
  return 0;
}